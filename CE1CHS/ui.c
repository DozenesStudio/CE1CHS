/* Implements fixed multilingual text storage and four-page UI state. */
/* Uses immutable byte strings and no heap or CRT storage. */
#include "ui.h"

static const BYTE g_empty[] = { 0 };

static const BYTE g_zhAppTitle[] =
{
    'C','E','1','C','H','S',
    0xBA,0xBA, 0xD7,0xD6, 0xCF,0xD4, 0xCA,0xBE,
    0xCF,0xB5, 0xCD,0xB3,
    'V','1','.','0','.','0', 0
};
static const BYTE g_zhAuthor[] = { 'B','y',' ','D','o','z','e','n','e','r',0 };
static const BYTE g_zhOpen[] =
{
    0xB4,0xF2, 0xBF,0xAA, 0xB9,0xFA, 0xB1,0xEA,
    0xC2,0xEB, 0xCE,0xC4, 0xB1,0xBE, 0
};
static const BYTE g_zhSettings[] = { 0xC9,0xE8, 0xD6,0xC3, 0 };
static const BYTE g_zhAbout[] = { 0xB9,0xD8, 0xD3,0xDA, 0 };
static const BYTE g_zhLanguage[] =
{
    0xBD,0xE7, 0xC3,0xE6, 0xD3,0xEF, 0xD1,0xD4, 0
};
static const BYTE g_zhLanguageName[] = { 0xD6,0xD0, 0xCE,0xC4, 0 };
static const BYTE g_zhAboutTagline[] =
{
    0xCA,0xD7, 0xB8,0xF6, 0xD4,0xDA,
    'W','i','n','C','E','1','.','0',
    0xD6,0xD0, 0xCF,0xD4, 0xCA,0xBE, 0xBA,0xBA, 0xD7,0xD6,
    0xB5,0xC4, 0xC8,0xED, 0xBC,0xFE, 0xCF,0xB5, 0xCD,0xB3, 0
};
static const BYTE g_zhAboutAuthor[] =
{
    0xD7,0xF7, 0xD5,0xDF, 0xA3,0xBA,
    'D','o','z','e','n','e','r','(',
    'D','o','z','e','n','e','s','S','t','u','d','i','o',')',0
};
static const BYTE g_zhMadeInChina[] =
{
    'M','a','d','e',' ','i','n',' ','C','h','i','n','a',' ','2','0','2','6',0
};

static const BYTE g_enAppTitle[] = "CE1CHS Chinese Display System V1.0.0";
static const BYTE g_enAuthor[] = "By Dozener";
static const BYTE g_enOpen[] = "Open GB2312 Text";
static const BYTE g_enSettings[] = "Settings";
static const BYTE g_enAbout[] = "About";
static const BYTE g_enLanguage[] = "Interface Language";
static const BYTE g_enLanguageName[] = "English";
static const BYTE g_enAboutTagline[] =
    "The first Chinese display system for WinCE 1.0";
static const BYTE g_enAboutAuthor[] = "Author: Dozener(DozenesStudio)";
static const BYTE g_enMadeInChina[] = "Made in China 2026";

static const BYTE *const g_strings[CE1_LANG_COUNT][UI_STR_COUNT] =
{
    {
        g_zhAppTitle, g_zhAuthor, g_zhOpen, g_zhSettings, g_zhAbout,
        g_zhLanguage, g_zhLanguageName, g_zhAboutTagline, g_zhAboutAuthor,
        g_zhMadeInChina
    },
    {
        g_enAppTitle, g_enAuthor, g_enOpen, g_enSettings, g_enAbout,
        g_enLanguage, g_enLanguageName, g_enAboutTagline, g_enAboutAuthor,
        g_enMadeInChina
    }
};

static CE1APPSTATE g_uiState;

void Ce1UiInit(void)
{
    g_uiState.page = PAGE_HOME;
    g_uiState.language = CE1_LANG_CHINESE;
}

BOOL Ce1UiSetPage(CE1PAGE page)
{
    if (page < PAGE_HOME || page >= PAGE_COUNT)
    {
        return FALSE;
    }
    g_uiState.page = page;
    return TRUE;
}

CE1PAGE Ce1UiGetPage(void)
{
    return g_uiState.page;
}

BOOL Ce1UiSetLanguage(CE1LANGUAGE language)
{
    if (language < CE1_LANG_CHINESE || language >= CE1_LANG_COUNT)
    {
        return FALSE;
    }
    g_uiState.language = language;
    return TRUE;
}

CE1LANGUAGE Ce1UiGetLanguage(void)
{
    return g_uiState.language;
}

const BYTE *Ce1UiText(CE1STRINGID id)
{
    if (id < UI_STR_APP_TITLE || id >= UI_STR_COUNT)
    {
        return g_empty;
    }
    return g_strings[g_uiState.language][id];
}

BOOL Ce1UiContentRect(HWND hWnd, int commandBarHeight, RECT *pRect)
{
    if (pRect == NULL || commandBarHeight < 0)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }
    if (!GetClientRect(hWnd, pRect))
    {
        return FALSE;
    }
    pRect->top += commandBarHeight;
    if (pRect->top > pRect->bottom)
    {
        pRect->top = pRect->bottom;
    }
    return TRUE;
}

BOOL Ce1UiDrawBitmap(HDC hDC, HBITMAP hBitmap, int x, int y)
{
    BITMAP bitmap;
    HDC hMemoryDC;
    HGDIOBJ hOldBitmap;
    BOOL success;

    if (hDC == NULL || hBitmap == NULL)
    {
        return FALSE;
    }
    if (!GetObject(hBitmap, sizeof(bitmap), &bitmap))
    {
        return FALSE;
    }
    if (bitmap.bmWidth <= 0 || bitmap.bmHeight <= 0)
    {
        return FALSE;
    }

    hMemoryDC = CreateCompatibleDC(hDC);
    if (hMemoryDC == NULL)
    {
        return FALSE;
    }
    hOldBitmap = SelectObject(hMemoryDC, hBitmap);
    if (hOldBitmap == NULL)
    {
        DeleteDC(hMemoryDC);
        return FALSE;
    }

    success = BitBlt(hDC, x, y, bitmap.bmWidth, bitmap.bmHeight,
                     hMemoryDC, 0, 0, SRCCOPY);
    SelectObject(hMemoryDC, hOldBitmap);
    DeleteDC(hMemoryDC);
    return success;
}
