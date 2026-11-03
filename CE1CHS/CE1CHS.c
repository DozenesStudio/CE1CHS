/* Implements the CE1CHS Windows CE executable and its single UI thread. */
/* Uses native navigation controls and direct core text drawing. */
#ifndef _WIN32_WCE
#define _WIN32_WCE 100
#endif

#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include "ce1core.h"
#include "document.h"
#include "ui.h"
#include "home.h"
#include "reader.h"
#include "settings.h"
#include "about.h"
#include "resource.h"

#define CE1_PAGE_HEIGHT       16
#define CE1_STD_IMAGE_COUNT   15
#define CE1_COMMAND_BAR_ID    1
#define CE1_CMD_BACK          1001
#define CE1_CMD_OPEN          1002
#define CE1_WM_DRAW_ERROR     (WM_USER + 1)

typedef struct CE1APP
{
    CE1COREHANDLE hCore;
    CE1DOCUMENT document;
    TCHAR documentPath[MAX_PATH];
    DWORD pageStart;
    HWND hCommandBar;
    HICON hIcon;
    HBITMAP hBackground;
    HBITMAP hLogo1;
    HBITMAP hLogo2;
    BOOL hibernated;
    BOOL documentSuspended;
    BOOL drawErrorShown;
} CE1APP;

static CE1APP g_app;

static void Ce1ShowError(HWND hWnd, LPCTSTR pszCaption, DWORD error)
{
    LPCTSTR pszMessage;

    switch (error)
    {
    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
        pszMessage = TEXT("The file or path was not found.");
        break;
    case ERROR_ACCESS_DENIED:
        pszMessage = TEXT("Access to the file was denied.");
        break;
    case ERROR_NOT_ENOUGH_MEMORY:
        pszMessage = TEXT("Not enough memory.");
        break;
    case ERROR_NOT_SUPPORTED:
        pszMessage = TEXT("The document exceeds the 256 KB limit.");
        break;
    case ERROR_INVALID_PARAMETER:
        pszMessage = TEXT("An absolute path and valid parameters are required.");
        break;
    default:
        pszMessage = TEXT("The operation failed.");
        break;
    }
    MessageBox(hWnd, pszMessage, pszCaption, MB_OK);
}

static void Ce1CopyString(LPTSTR pszDestination, LPCTSTR pszSource)
{
    while ((*pszDestination++ = *pszSource++) != 0)
    {
    }
}

static void Ce1LoadBitmaps(HINSTANCE hInstance)
{
    g_app.hBackground = LoadBitmap(hInstance,
                                   MAKEINTRESOURCE(IDB_BACKGROUND));
    g_app.hLogo1 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_LOGO1));
    g_app.hLogo2 = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_LOGO2));
}

static void Ce1ReleaseBitmaps(void)
{
    if (g_app.hBackground != NULL)
    {
        DeleteObject(g_app.hBackground);
        g_app.hBackground = NULL;
    }
    if (g_app.hLogo1 != NULL)
    {
        DeleteObject(g_app.hLogo1);
        g_app.hLogo1 = NULL;
    }
    if (g_app.hLogo2 != NULL)
    {
        DeleteObject(g_app.hLogo2);
        g_app.hLogo2 = NULL;
    }
}

static BOOL Ce1NextArgument(LPCTSTR *ppCursor, LPTSTR pszArgument)
{
    LPCTSTR p;
    DWORD length;
    BOOL quoted;

    p = *ppCursor;
    while (*p == ' ' || *p == '\t')
    {
        ++p;
    }

    pszArgument[0] = 0;
    if (*p == 0)
    {
        *ppCursor = p;
        return TRUE;
    }

    quoted = FALSE;
    if (*p == '"')
    {
        quoted = TRUE;
        ++p;
    }

    length = 0;
    while (*p != 0)
    {
        if (quoted)
        {
            if (*p == '"')
            {
                break;
            }
        }
        else if (*p == ' ' || *p == '\t')
        {
            break;
        }

        if (length >= MAX_PATH - 1)
        {
            return FALSE;
        }
        pszArgument[length++] = *p++;
    }

    if (quoted)
    {
        if (*p != '"')
        {
            return FALSE;
        }
        ++p;
        if (*p != 0 && *p != ' ' && *p != '\t')
        {
            return FALSE;
        }
    }

    if (length == 0)
    {
        return FALSE;
    }
    pszArgument[length] = 0;
    *ppCursor = p;
    return TRUE;
}

static BOOL Ce1ParseCommandLine(LPCTSTR pszCommandLine, LPTSTR pszFontPath,
                                LPTSTR pszInitialPath)
{
    LPCTSTR cursor;
    TCHAR argument[MAX_PATH];

    pszFontPath[0] = 0;
    pszInitialPath[0] = 0;
    if (pszCommandLine == NULL)
    {
        return TRUE;
    }

    cursor = pszCommandLine;
    if (!Ce1NextArgument(&cursor, argument))
    {
        return FALSE;
    }
    if (argument[0] == 0)
    {
        return TRUE;
    }

    if (argument[0] == '-' && argument[1] == 'f' && argument[2] == 0)
    {
        if (!Ce1NextArgument(&cursor, pszFontPath) || pszFontPath[0] == 0)
        {
            return FALSE;
        }
        if (!Ce1NextArgument(&cursor, pszInitialPath))
        {
            return FALSE;
        }
    }
    else
    {
        Ce1CopyString(pszInitialPath, argument);
    }

    if (!Ce1NextArgument(&cursor, argument) || argument[0] != 0)
    {
        return FALSE;
    }
    if ((pszFontPath[0] != 0 && pszFontPath[0] != '\\') ||
        (pszInitialPath[0] != 0 && pszInitialPath[0] != '\\'))
    {
        return FALSE;
    }
    return TRUE;
}

static int Ce1CommandBarHeight(void)
{
    if (g_app.hCommandBar == NULL)
    {
        return 0;
    }
    return CommandBar_Height(g_app.hCommandBar);
}

static void Ce1UpdateScrollbar(HWND hWnd)
{
    SCROLLINFO info;

    info.cbSize = sizeof(info);
    info.fMask = SIF_RANGE | SIF_POS | SIF_DISABLENOSCROLL;
    info.nMin = 0;
    info.nMax = 0;
    info.nPage = 0;
    info.nPos = 0;
    info.nTrackPos = 0;
    if (Ce1UiGetPage() == PAGE_READER &&
        g_app.document.loaded && g_app.document.length > 0)
    {
        info.nMax = (int)g_app.document.length - 1;
        info.nPos = (int)g_app.pageStart;
        if (info.nPos < 0 || info.nPos > info.nMax)
        {
            info.nPos = info.nMax;
        }
    }
    (void)SetScrollInfo(hWnd, SB_VERT, &info, TRUE);
}

static void Ce1UpdatePageControls(HWND hWnd)
{
    if (g_app.hCommandBar != NULL)
    {
        (void)SendMessage(g_app.hCommandBar, TB_SETSTATE, CE1_CMD_BACK,
                          (LPARAM)MAKELONG(Ce1UiGetPage() == PAGE_HOME ?
                                            0 : TBSTATE_ENABLED, 0));
    }
    Ce1UpdateScrollbar(hWnd);
}

static void Ce1RequestRepaint(HWND hWnd)
{
    if (!InvalidateRect(hWnd, NULL, TRUE))
    {
        Ce1ShowError(hWnd, TEXT("Redraw"), GetLastError());
    }
}

static void Ce1ChangePage(HWND hWnd, CE1PAGE page)
{
    if (Ce1UiGetPage() == PAGE_HOME && page != PAGE_HOME)
    {
        Ce1HomeCancel(hWnd);
    }
    if (Ce1UiGetPage() == PAGE_SETTINGS && page != PAGE_SETTINGS)
    {
        Ce1SettingsCancel(hWnd);
    }
    (void)Ce1UiSetPage(page);
    g_app.drawErrorShown = FALSE;
    Ce1UpdatePageControls(hWnd);
    Ce1RequestRepaint(hWnd);
}

static BOOL Ce1CreateCommandBar(HWND hWnd, HINSTANCE hInstance)
{
    TBBUTTON buttons[2];

    g_app.hCommandBar = CommandBar_Create(hInstance, hWnd, CE1_COMMAND_BAR_ID);
    if (g_app.hCommandBar == NULL)
    {
        return FALSE;
    }
    if (CommandBar_AddBitmap(g_app.hCommandBar, HINST_COMMCTRL,
                             IDB_STD_SMALL_COLOR, CE1_STD_IMAGE_COUNT,
                             0, 0) == -1)
    {
        return FALSE;
    }

    buttons[0].iBitmap = STD_UNDO;
    buttons[0].idCommand = CE1_CMD_BACK;
    buttons[0].fsState = TBSTATE_ENABLED;
    buttons[0].fsStyle = TBSTYLE_BUTTON;
    buttons[0].dwData = 0;
    buttons[0].iString = 0;
    buttons[1].iBitmap = STD_FILEOPEN;
    buttons[1].idCommand = CE1_CMD_OPEN;
    buttons[1].fsState = TBSTATE_ENABLED;
    buttons[1].fsStyle = TBSTYLE_BUTTON;
    buttons[1].dwData = 0;
    buttons[1].iString = 0;
    if (!CommandBar_AddButtons(g_app.hCommandBar, 2, buttons))
    {
        return FALSE;
    }
    if (!CommandBar_AddAdornments(g_app.hCommandBar, 0, 0))
    {
        return FALSE;
    }

    Ce1UpdatePageControls(hWnd);
    return TRUE;
}

static void Ce1OpenDocument(HWND hWnd, LPCTSTR pszPath)
{
    if (!Ce1DocumentLoad(&g_app.document, pszPath))
    {
        Ce1ShowError(hWnd, TEXT("Open document"), GetLastError());
        return;
    }

    Ce1CopyString(g_app.documentPath, pszPath);
    g_app.pageStart = 0;
    g_app.documentSuspended = FALSE;
    Ce1ChangePage(hWnd, PAGE_READER);
}

static void Ce1OpenDialog(HWND hWnd)
{
    OPENFILENAME ofn;
    TCHAR path[MAX_PATH];
    BYTE *pBytes;
    DWORD i;

    pBytes = (BYTE *)(LPVOID)&ofn;
    for (i = 0; i < sizeof(ofn); ++i)
    {
        pBytes[i] = 0;
    }
    path[0] = 0;

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFilter = TEXT("Text files (*.txt)\0*.txt\0All files (*.*)\0*.*\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFile = path;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrInitialDir = TEXT("\\");
    ofn.lpstrTitle = TEXT("Open GB2312 document");
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = TEXT("txt");

    if (GetOpenFileName(&ofn))
    {
        Ce1OpenDocument(hWnd, path);
    }
}

static void Ce1NextPage(HWND hWnd)
{
    RECT rect;
    DWORD nextOffset;

    if (!g_app.document.loaded || g_app.documentSuspended ||
        g_app.hibernated || g_app.pageStart >= g_app.document.length)
    {
        return;
    }
    if (!Ce1ReaderContentRect(hWnd, Ce1CommandBarHeight(), &rect))
    {
        Ce1ShowError(hWnd, TEXT("Next page"), GetLastError());
        return;
    }
    if (rect.right <= rect.left || rect.bottom - rect.top < CE1_PAGE_HEIGHT)
    {
        return;
    }

    if (!CE1CoreMeasurePage(g_app.hCore, g_app.document.pBytes,
                            g_app.document.length, g_app.pageStart,
                            rect.right - rect.left, rect.bottom - rect.top,
                            &nextOffset))
    {
        Ce1ShowError(hWnd, TEXT("Next page"), GetLastError());
        return;
    }
    if (nextOffset <= g_app.pageStart || nextOffset > g_app.document.length)
    {
        Ce1ShowError(hWnd, TEXT("Next page"), ERROR_INVALID_DATA);
        return;
    }
    if (nextOffset == g_app.document.length)
    {
        return;
    }

    g_app.pageStart = nextOffset;
    Ce1UpdateScrollbar(hWnd);
    Ce1RequestRepaint(hWnd);
}

static void Ce1PreviousPage(HWND hWnd)
{
    RECT rect;
    DWORD scanOffset;
    DWORD nextOffset;
    DWORD previousOffset;
    BOOL found;

    if (!g_app.document.loaded || g_app.documentSuspended ||
        g_app.hibernated || g_app.pageStart == 0)
    {
        return;
    }
    if (!Ce1ReaderContentRect(hWnd, Ce1CommandBarHeight(), &rect))
    {
        Ce1ShowError(hWnd, TEXT("Previous page"), GetLastError());
        return;
    }
    if (rect.right <= rect.left || rect.bottom - rect.top < CE1_PAGE_HEIGHT)
    {
        return;
    }

    scanOffset = 0;
    previousOffset = 0;
    found = FALSE;
    while (scanOffset < g_app.pageStart)
    {
        if (!CE1CoreMeasurePage(g_app.hCore, g_app.document.pBytes,
                                g_app.document.length, scanOffset,
                                rect.right - rect.left, rect.bottom - rect.top,
                                &nextOffset))
        {
            Ce1ShowError(hWnd, TEXT("Previous page"), GetLastError());
            return;
        }
        if (nextOffset <= scanOffset || nextOffset > g_app.document.length)
        {
            Ce1ShowError(hWnd, TEXT("Previous page"), ERROR_INVALID_DATA);
            return;
        }
        if (nextOffset >= g_app.pageStart)
        {
            previousOffset = scanOffset;
            found = TRUE;
            break;
        }
        scanOffset = nextOffset;
    }

    if (!found)
    {
        Ce1ShowError(hWnd, TEXT("Previous page"), ERROR_INVALID_DATA);
        return;
    }
    g_app.pageStart = previousOffset;
    Ce1UpdateScrollbar(hWnd);
    Ce1RequestRepaint(hWnd);
}

static void Ce1SeekPage(HWND hWnd, DWORD offset)
{
    if (!g_app.document.loaded || g_app.document.pBytes == NULL ||
        g_app.document.length == 0 || g_app.documentSuspended ||
        g_app.hibernated)
    {
        return;
    }
    if (offset >= g_app.document.length)
    {
        offset = g_app.document.length - 1;
    }
    offset = Ce1ReaderAlignOffset(&g_app.document, offset);
    if (offset != g_app.pageStart)
    {
        g_app.pageStart = offset;
        Ce1RequestRepaint(hWnd);
    }
    Ce1UpdateScrollbar(hWnd);
}

static void Ce1ThumbPosition(HWND hWnd, WPARAM wParam)
{
    SCROLLINFO info;
    DWORD offset;

    info.cbSize = sizeof(info);
    info.fMask = SIF_POS;
    info.nMin = 0;
    info.nMax = 0;
    info.nPage = 0;
    info.nPos = 0;
    info.nTrackPos = 0;
    if (GetScrollInfo(hWnd, SB_VERT, &info))
    {
        offset = (DWORD)info.nPos;
    }
    else
    {
        offset = (DWORD)HIWORD(wParam);
    }
    Ce1SeekPage(hWnd, offset);
}

static void Ce1VerticalScroll(HWND hWnd, WPARAM wParam)
{
    if (Ce1UiGetPage() != PAGE_READER)
    {
        return;
    }

    switch (LOWORD(wParam))
    {
    case SB_LINEUP:
    case SB_PAGEUP:
        Ce1PreviousPage(hWnd);
        break;
    case SB_LINEDOWN:
    case SB_PAGEDOWN:
        Ce1NextPage(hWnd);
        break;
    case SB_TOP:
        Ce1SeekPage(hWnd, 0);
        break;
    case SB_BOTTOM:
        Ce1SeekPage(hWnd, g_app.document.length);
        break;
    case SB_THUMBPOSITION:
        Ce1ThumbPosition(hWnd, wParam);
        break;
    case SB_THUMBTRACK:
    case SB_ENDSCROLL:
    default:
        break;
    }
}

static LPCTSTR Ce1DrawCaption(CE1PAGE page)
{
    switch (page)
    {
    case PAGE_HOME:
        return TEXT("Draw home");
    case PAGE_READER:
        return TEXT("Draw page");
    case PAGE_SETTINGS:
        return TEXT("Draw settings");
    case PAGE_ABOUT:
        return TEXT("Draw about");
    default:
        return TEXT("Draw page");
    }
}

static void Ce1Paint(HWND hWnd)
{
    CE1PAGE page;
    DWORD paintError;
    BOOL success;
    int commandBarHeight;

    page = Ce1UiGetPage();
    commandBarHeight = Ce1CommandBarHeight();
    success = FALSE;
    if (page == PAGE_HOME)
    {
        success = Ce1HomePaint(hWnd, g_app.hCore, g_app.hBackground,
                               commandBarHeight);
    }
    else if (page == PAGE_READER)
    {
        success = Ce1ReaderPaint(hWnd, g_app.hCore, &g_app.document,
                                 g_app.pageStart,
                                 g_app.hibernated || g_app.documentSuspended,
                                 commandBarHeight);
    }
    else if (page == PAGE_SETTINGS)
    {
        success = Ce1SettingsPaint(hWnd, g_app.hCore, commandBarHeight);
    }
    else if (page == PAGE_ABOUT)
    {
        success = Ce1AboutPaint(hWnd, g_app.hCore, g_app.hIcon,
                                g_app.hLogo1, g_app.hLogo2,
                                commandBarHeight);
    }

    if (success)
    {
        g_app.drawErrorShown = FALSE;
    }
    else if (!g_app.drawErrorShown)
    {
        paintError = GetLastError();
        if (paintError == ERROR_SUCCESS)
        {
            paintError = ERROR_GEN_FAILURE;
        }
        g_app.drawErrorShown = TRUE;
        (void)PostMessage(hWnd, CE1_WM_DRAW_ERROR, (WPARAM)page,
                          (LPARAM)paintError);
    }
}

static void Ce1Hibernate(HWND hWnd)
{
    if (g_app.hCore != NULL && CE1CoreHibernate(g_app.hCore))
    {
        g_app.hibernated = TRUE;
    }

    if (g_app.document.loaded && Ce1DocumentClear(&g_app.document))
    {
        g_app.documentSuspended = (g_app.documentPath[0] != 0);
    }
    (void)hWnd;
}

static void Ce1Activate(HWND hWnd)
{
    BOOL restored;

    restored = FALSE;
    if (g_app.hibernated)
    {
        if (!CE1CoreActivate(g_app.hCore))
        {
            Ce1ShowError(hWnd, TEXT("Activate"), GetLastError());
            return;
        }
        g_app.hibernated = FALSE;
        restored = TRUE;
    }

    if (g_app.documentSuspended)
    {
        if (!Ce1DocumentLoad(&g_app.document, g_app.documentPath))
        {
            Ce1ShowError(hWnd, TEXT("Restore document"), GetLastError());
            return;
        }
        g_app.documentSuspended = FALSE;
        restored = TRUE;
        if (g_app.pageStart >= g_app.document.length)
        {
            g_app.pageStart = 0;
        }
    }
    if (restored)
    {
        Ce1UpdateScrollbar(hWnd);
        Ce1RequestRepaint(hWnd);
    }
}

static LRESULT CALLBACK Ce1WndProc(HWND hWnd, UINT message, WPARAM wParam,
                                   LPARAM lParam)
{
    int action;
    int commandBarHeight;

    switch (message)
    {
    case WM_CREATE:
        Ce1LoadBitmaps(((LPCREATESTRUCT)lParam)->hInstance);
        return 0;

    case WM_COMMAND:
        if (LOWORD(wParam) == CE1_CMD_BACK)
        {
            if (Ce1UiGetPage() != PAGE_HOME)
            {
                Ce1ChangePage(hWnd, PAGE_HOME);
            }
            return 0;
        }
        if (LOWORD(wParam) == CE1_CMD_OPEN)
        {
            Ce1OpenDialog(hWnd);
            return 0;
        }
        break;

    case WM_KEYDOWN:
        if (Ce1UiGetPage() == PAGE_READER)
        {
            if (wParam == VK_PRIOR)
            {
                Ce1PreviousPage(hWnd);
                return 0;
            }
            if (wParam == VK_NEXT)
            {
                Ce1NextPage(hWnd);
                return 0;
            }
        }
        break;

    case WM_VSCROLL:
        Ce1VerticalScroll(hWnd, wParam);
        return 0;

    case WM_SIZE:
        Ce1UpdateScrollbar(hWnd);
        InvalidateRect(hWnd, NULL, TRUE);
        return 0;

    case WM_LBUTTONDOWN:
        commandBarHeight = Ce1CommandBarHeight();
        if (Ce1UiGetPage() == PAGE_HOME)
        {
            Ce1HomeMouseDown(hWnd, commandBarHeight, lParam);
            return 0;
        }
        if (Ce1UiGetPage() == PAGE_SETTINGS)
        {
            Ce1SettingsMouseDown(hWnd, commandBarHeight, lParam);
            return 0;
        }
        break;

    case WM_MOUSEMOVE:
        commandBarHeight = Ce1CommandBarHeight();
        if (Ce1UiGetPage() == PAGE_HOME)
        {
            Ce1HomeMouseMove(hWnd, commandBarHeight, lParam);
            return 0;
        }
        if (Ce1UiGetPage() == PAGE_SETTINGS)
        {
            Ce1SettingsMouseMove(hWnd, commandBarHeight, lParam);
            return 0;
        }
        break;

    case WM_LBUTTONUP:
        commandBarHeight = Ce1CommandBarHeight();
        if (Ce1UiGetPage() == PAGE_HOME)
        {
            action = Ce1HomeMouseUp(hWnd, commandBarHeight, lParam);
            if (action == CE1_HOME_ACTION_OPEN)
            {
                Ce1ChangePage(hWnd, PAGE_READER);
                Ce1OpenDialog(hWnd);
            }
            else if (action == CE1_HOME_ACTION_SETTINGS)
            {
                Ce1ChangePage(hWnd, PAGE_SETTINGS);
            }
            else if (action == CE1_HOME_ACTION_ABOUT)
            {
                Ce1ChangePage(hWnd, PAGE_ABOUT);
            }
            return 0;
        }
        if (Ce1UiGetPage() == PAGE_SETTINGS)
        {
            action = Ce1SettingsMouseUp(hWnd, commandBarHeight, lParam);
            if (action == CE1_SETTINGS_ACTION_LANGUAGE)
            {
                (void)Ce1UiSetLanguage(
                    Ce1UiGetLanguage() == CE1_LANG_CHINESE ?
                    CE1_LANG_ENGLISH : CE1_LANG_CHINESE);
                Ce1RequestRepaint(hWnd);
            }
            return 0;
        }
        break;

    case WM_PAINT:
        Ce1Paint(hWnd);
        return 0;

    case CE1_WM_DRAW_ERROR:
        if ((CE1PAGE)wParam == Ce1UiGetPage() && g_app.drawErrorShown)
        {
            Ce1ShowError(hWnd, Ce1DrawCaption((CE1PAGE)wParam),
                         (DWORD)lParam);
        }
        return 0;

    case WM_HIBERNATE:
        Ce1Hibernate(hWnd);
        return 0;

    case WM_ACTIVATE:
        if (LOWORD(wParam) != WA_INACTIVE)
        {
            Ce1Activate(hWnd);
        }
        return DefWindowProc(hWnd, message, wParam, lParam);

    case WM_CLOSE:
        if (!DestroyWindow(hWnd))
        {
            Ce1ShowError(hWnd, TEXT("Close"), GetLastError());
        }
        return 0;

    case WM_DESTROY:
        Ce1ReleaseBitmaps();
        Ce1DocumentClear(&g_app.document);
        CE1CoreDestroy(&g_app.hCore);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPTSTR lpCmdLine, int nCmdShow)
{
    WNDCLASS windowClass;
    HWND hWnd;
    MSG msg;
    TCHAR fontPath[MAX_PATH];
    TCHAR initialPath[MAX_PATH];
    int messageResult;

    (void)hPrevInstance;
    Ce1UiInit();
    Ce1HomeInit();
    Ce1SettingsInit();

    if (!Ce1ParseCommandLine(lpCmdLine, fontPath, initialPath))
    {
        MessageBox(NULL,
                   TEXT("Expected: CE1CHS.exe [-f \\path\\HZK16] [\\path\\file.txt]"),
                   TEXT("CE1CHS"), MB_OK);
        return 0;
    }

    if (!CE1CoreCreate(fontPath[0] == 0 ? NULL : fontPath, &g_app.hCore))
    {
        Ce1ShowError(NULL, TEXT("Initialize CE1CHS"), GetLastError());
        return 0;
    }

    g_app.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    if (g_app.hIcon == NULL)
    {
        Ce1ShowError(NULL, TEXT("Initialize CE1CHS"), GetLastError());
        CE1CoreDestroy(&g_app.hCore);
        return 0;
    }

    windowClass.style = 0;
    windowClass.lpfnWndProc = (WNDPROC)Ce1WndProc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = hInstance;
    windowClass.hIcon = g_app.hIcon;
    windowClass.hCursor = NULL;
    windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = TEXT("CE1CHSWindow");

    if (windowClass.hbrBackground == NULL)
    {
        Ce1ShowError(NULL, TEXT("Initialize CE1CHS"), GetLastError());
        CE1CoreDestroy(&g_app.hCore);
        return 0;
    }

    if (RegisterClass(&windowClass) == 0)
    {
        Ce1ShowError(NULL, TEXT("Initialize CE1CHS"), GetLastError());
        CE1CoreDestroy(&g_app.hCore);
        return 0;
    }

    InitCommonControls();
    hWnd = CreateWindow(TEXT("CE1CHSWindow"), TEXT("CE1CHS"),
                        WS_VISIBLE | WS_VSCROLL,
                        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                        CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
    if (hWnd == NULL)
    {
        Ce1ShowError(NULL, TEXT("Create window"), GetLastError());
        CE1CoreDestroy(&g_app.hCore);
        return 0;
    }
    if (!Ce1CreateCommandBar(hWnd, hInstance))
    {
        Ce1ShowError(hWnd, TEXT("Create command bar"), GetLastError());
        DestroyWindow(hWnd);
        return 0;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    if (initialPath[0] != 0)
    {
        Ce1OpenDocument(hWnd, initialPath);
    }

    msg.wParam = 0;
    for (;;)
    {
        messageResult = GetMessage(&msg, NULL, 0, 0);
        if (messageResult == 0)
        {
            break;
        }
        if (messageResult == -1)
        {
            Ce1ShowError(NULL, TEXT("Message loop"), GetLastError());
            msg.wParam = 0;
            break;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Ce1ReleaseBitmaps();
    Ce1DocumentClear(&g_app.document);
    CE1CoreDestroy(&g_app.hCore);
    return (int)msg.wParam;
}
