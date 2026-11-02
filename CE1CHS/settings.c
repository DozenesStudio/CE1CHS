/* Implements settings drawing and the captured language option. */
#include "settings.h"
#include "ui.h"

#define CE1_SETTINGS_MARGIN         12
#define CE1_SETTINGS_NARROW_MARGIN  4
#define CE1_SETTINGS_OPTION_HEIGHT  28
#define CE1_SETTINGS_HEADER_GAP     12
#define CE1_SETTINGS_LABEL_GAP      4
#define CE1_SETTINGS_TEXT_HEIGHT    16

typedef struct CE1SETTINGSLAYOUT
{
    RECT title;
    RECT label;
    RECT option;
} CE1SETTINGSLAYOUT;

static int g_settingsAction = CE1_SETTINGS_ACTION_NONE;
static BOOL g_settingsPressed;

static DWORD Ce1SettingsError(void)
{
    DWORD error;

    error = GetLastError();
    if (error == ERROR_SUCCESS)
    {
        error = ERROR_GEN_FAILURE;
    }
    return error;
}

static DWORD Ce1SettingsTextLength(const BYTE *pText)
{
    DWORD length;

    length = 0;
    while (pText[length] != 0)
    {
        ++length;
    }
    return length;
}

static DWORD Ce1SettingsTextWidth(const BYTE *pText, DWORD start, DWORD end)
{
    DWORD offset;
    DWORD width;
    BYTE value;

    offset = start;
    width = 0;
    while (offset < end && pText[offset] != 0)
    {
        value = pText[offset];
        if (value == '\r' || value == '\n')
        {
            break;
        }
        if (value < 0x20 || value == 0x7F)
        {
            ++offset;
        }
        else if (value < 0x80)
        {
            width += 8;
            ++offset;
        }
        else
        {
            width += 16;
            offset += (offset + 1 < end && pText[offset + 1] != 0) ? 2 : 1;
        }
    }
    return width;
}

static BOOL Ce1SettingsDrawText(CE1COREHANDLE hCore, HDC hDC,
                                const BYTE *pText, int left, int top,
                                int width, BOOL centered)
{
    DWORD length;
    DWORD nextOffset;
    DWORD drawOffset;
    DWORD textWidth;
    RECT rect;
    int x;

    if (hCore == NULL || hDC == NULL || pText == NULL || width <= 0)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    length = Ce1SettingsTextLength(pText);
    if (!CE1CoreMeasurePage(hCore, pText, length, 0, width,
                            CE1_SETTINGS_TEXT_HEIGHT, &nextOffset))
    {
        return FALSE;
    }
    if (nextOffset == 0 || nextOffset > length)
    {
        SetLastError(ERROR_INVALID_DATA);
        return FALSE;
    }

    x = left;
    if (centered)
    {
        textWidth = Ce1SettingsTextWidth(pText, 0, nextOffset);
        if (textWidth < (DWORD)width)
        {
            x += (width - (int)textWidth) / 2;
        }
    }
    rect.left = x;
    rect.top = top;
    rect.right = left + width;
    rect.bottom = top + CE1_SETTINGS_TEXT_HEIGHT;
    if (!CE1CoreDrawPage(hCore, hDC, pText, length, 0, &rect, &drawOffset))
    {
        return FALSE;
    }
    if (drawOffset != nextOffset)
    {
        SetLastError(ERROR_INVALID_DATA);
        return FALSE;
    }
    return TRUE;
}

static BOOL Ce1SettingsLayout(HWND hWnd, int commandBarHeight,
                              CE1SETTINGSLAYOUT *pLayout)
{
    RECT client;
    int width;
    int margin;
    int left;
    int right;

    if (!Ce1UiContentRect(hWnd, commandBarHeight, &client))
    {
        return FALSE;
    }

    width = client.right - client.left;
    if (width >= 32)
    {
        margin = CE1_SETTINGS_MARGIN;
    }
    else if (width >= 24)
    {
        margin = CE1_SETTINGS_NARROW_MARGIN;
    }
    else
    {
        margin = 0;
    }
    left = client.left + margin;
    right = client.right - margin;

    pLayout->title.left = left;
    pLayout->title.right = right;
    pLayout->title.top = client.top + margin;
    pLayout->title.bottom = pLayout->title.top + CE1_SETTINGS_TEXT_HEIGHT;

    pLayout->label.left = left;
    pLayout->label.right = right;
    pLayout->label.top = pLayout->title.bottom + CE1_SETTINGS_HEADER_GAP;
    pLayout->label.bottom = pLayout->label.top + CE1_SETTINGS_TEXT_HEIGHT;

    pLayout->option.left = left;
    pLayout->option.right = right;
    pLayout->option.top = pLayout->label.bottom + CE1_SETTINGS_LABEL_GAP;
    pLayout->option.bottom = pLayout->option.top + CE1_SETTINGS_OPTION_HEIGHT;
    return TRUE;
}

static int Ce1SettingsHitTest(HWND hWnd, int commandBarHeight, LPARAM lParam)
{
    CE1SETTINGSLAYOUT layout;
    POINT point;

    if (!Ce1SettingsLayout(hWnd, commandBarHeight, &layout))
    {
        return CE1_SETTINGS_ACTION_NONE;
    }
    point.x = (int)(short)LOWORD(lParam);
    point.y = (int)(short)HIWORD(lParam);
    if (PtInRect(&layout.option, point))
    {
        return CE1_SETTINGS_ACTION_LANGUAGE;
    }
    return CE1_SETTINGS_ACTION_NONE;
}

static void Ce1SettingsInvalidateOption(HWND hWnd, int commandBarHeight)
{
    CE1SETTINGSLAYOUT layout;

    if (Ce1SettingsLayout(hWnd, commandBarHeight, &layout))
    {
        InvalidateRect(hWnd, &layout.option, TRUE);
    }
}

static BOOL Ce1SettingsDrawControl(CE1COREHANDLE hCore, HDC hDC,
                                   const RECT *pRect, const BYTE *pText,
                                   BOOL pressed)
{
    RECT fill;
    HBRUSH hBrush;
    int textTop;

    if (!Rectangle(hDC, pRect->left, pRect->top, pRect->right, pRect->bottom))
    {
        return FALSE;
    }
    if (pressed)
    {
        hBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);
        if (hBrush == NULL)
        {
            return FALSE;
        }
        fill.left = pRect->left + 1;
        fill.top = pRect->top + 1;
        fill.right = pRect->right - 1;
        fill.bottom = pRect->bottom - 1;
        if (fill.right > fill.left && fill.bottom > fill.top &&
            !FillRect(hDC, &fill, hBrush))
        {
            return FALSE;
        }
    }

    textTop = pRect->top +
              (pRect->bottom - pRect->top - CE1_SETTINGS_TEXT_HEIGHT) / 2;
    return Ce1SettingsDrawText(hCore, hDC, pText, pRect->left + 4, textTop,
                               pRect->right - pRect->left - 8, TRUE);
}

void Ce1SettingsInit(void)
{
    g_settingsAction = CE1_SETTINGS_ACTION_NONE;
    g_settingsPressed = FALSE;
}

void Ce1SettingsCancel(HWND hWnd)
{
    int action;

    action = g_settingsAction;
    g_settingsAction = CE1_SETTINGS_ACTION_NONE;
    g_settingsPressed = FALSE;
    if (action != CE1_SETTINGS_ACTION_NONE)
    {
        ReleaseCapture();
    }
    (void)hWnd;
}

BOOL Ce1SettingsPaint(HWND hWnd, CE1COREHANDLE hCore, int commandBarHeight)
{
    PAINTSTRUCT paint;
    RECT client;
    CE1SETTINGSLAYOUT layout;
    HDC hDC;
    HBRUSH hBrush;
    DWORD savedError;
    BOOL success;

    hDC = BeginPaint(hWnd, &paint);
    if (hDC == NULL)
    {
        return FALSE;
    }

    savedError = ERROR_SUCCESS;
    success = FALSE;
    if (!Ce1UiContentRect(hWnd, commandBarHeight, &client))
    {
        savedError = Ce1SettingsError();
        goto cleanup;
    }
    if (client.right <= client.left || client.bottom <= client.top)
    {
        success = TRUE;
        goto cleanup;
    }
    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
    if (hBrush == NULL || !FillRect(hDC, &client, hBrush))
    {
        savedError = Ce1SettingsError();
        goto cleanup;
    }
    if (client.right - client.left < 16 || client.bottom - client.top < 16)
    {
        success = TRUE;
        goto cleanup;
    }
    if (!Ce1SettingsLayout(hWnd, commandBarHeight, &layout))
    {
        savedError = Ce1SettingsError();
        goto cleanup;
    }

    if (!Ce1SettingsDrawText(hCore, hDC, Ce1UiText(UI_STR_SETTINGS),
                             layout.title.left, layout.title.top,
                             layout.title.right - layout.title.left, FALSE) ||
        !Ce1SettingsDrawText(hCore, hDC, Ce1UiText(UI_STR_LANGUAGE),
                             layout.label.left, layout.label.top,
                             layout.label.right - layout.label.left, FALSE) ||
        !Ce1SettingsDrawControl(
            hCore, hDC, &layout.option, Ce1UiText(UI_STR_LANGUAGE_NAME),
            g_settingsAction == CE1_SETTINGS_ACTION_LANGUAGE &&
            g_settingsPressed))
    {
        savedError = Ce1SettingsError();
        goto cleanup;
    }
    success = TRUE;

cleanup:
    EndPaint(hWnd, &paint);
    if (!success)
    {
        if (savedError == ERROR_SUCCESS)
        {
            savedError = ERROR_GEN_FAILURE;
        }
        SetLastError(savedError);
    }
    return success;
}

void Ce1SettingsMouseDown(HWND hWnd, int commandBarHeight, LPARAM lParam)
{
    int action;

    action = Ce1SettingsHitTest(hWnd, commandBarHeight, lParam);
    if (action == CE1_SETTINGS_ACTION_NONE)
    {
        return;
    }
    g_settingsAction = action;
    g_settingsPressed = TRUE;
    SetCapture(hWnd);
    Ce1SettingsInvalidateOption(hWnd, commandBarHeight);
}

void Ce1SettingsMouseMove(HWND hWnd, int commandBarHeight, LPARAM lParam)
{
    BOOL pressed;

    if (g_settingsAction == CE1_SETTINGS_ACTION_NONE)
    {
        return;
    }
    pressed = (Ce1SettingsHitTest(hWnd, commandBarHeight, lParam) ==
               g_settingsAction);
    if (pressed != g_settingsPressed)
    {
        g_settingsPressed = pressed;
        Ce1SettingsInvalidateOption(hWnd, commandBarHeight);
    }
}

int Ce1SettingsMouseUp(HWND hWnd, int commandBarHeight, LPARAM lParam)
{
    int action;
    int hit;
    BOOL activate;

    if (g_settingsAction == CE1_SETTINGS_ACTION_NONE)
    {
        return CE1_SETTINGS_ACTION_NONE;
    }
    action = g_settingsAction;
    hit = Ce1SettingsHitTest(hWnd, commandBarHeight, lParam);
    activate = (g_settingsPressed && hit == action);
    g_settingsAction = CE1_SETTINGS_ACTION_NONE;
    g_settingsPressed = FALSE;
    ReleaseCapture();
    Ce1SettingsInvalidateOption(hWnd, commandBarHeight);
    return activate ? action : CE1_SETTINGS_ACTION_NONE;
}
