/* Defines the four-page UI state and multilingual text identifiers. */
/* Text data is GB2312 for Chinese and ASCII for English. */
#ifndef CE1_UI_H
#define CE1_UI_H

#ifndef _WIN32_WCE
#define _WIN32_WCE 100
#endif

#include <windows.h>

typedef enum CE1PAGE
{
    PAGE_HOME = 0,
    PAGE_READER,
    PAGE_SETTINGS,
    PAGE_ABOUT,
    PAGE_COUNT
} CE1PAGE;

typedef enum CE1LANGUAGE
{
    CE1_LANG_CHINESE = 0,
    CE1_LANG_ENGLISH,
    CE1_LANG_COUNT
} CE1LANGUAGE;

typedef enum CE1STRINGID
{
    UI_STR_APP_TITLE = 0,
    UI_STR_AUTHOR,
    UI_STR_OPEN,
    UI_STR_SETTINGS,
    UI_STR_ABOUT,
    UI_STR_LANGUAGE,
    UI_STR_LANGUAGE_NAME,
    UI_STR_ABOUT_TAGLINE,
    UI_STR_ABOUT_AUTHOR,
    UI_STR_MADE_IN_CHINA,
    UI_STR_COUNT
} CE1STRINGID;

typedef struct CE1APPSTATE
{
    CE1PAGE page;
    CE1LANGUAGE language;
} CE1APPSTATE;

void Ce1UiInit(void);
BOOL Ce1UiSetPage(CE1PAGE page);
CE1PAGE Ce1UiGetPage(void);
BOOL Ce1UiSetLanguage(CE1LANGUAGE language);
CE1LANGUAGE Ce1UiGetLanguage(void);
const BYTE *Ce1UiText(CE1STRINGID id);

/* Returns the client area below the native command bar. */
BOOL Ce1UiContentRect(HWND hWnd, int commandBarHeight, RECT *pRect);

/* Draws a loaded 1-bpp or 2-bpp bitmap without palette processing. */
BOOL Ce1UiDrawBitmap(HDC hDC, HBITMAP hBitmap, int x, int y);

#endif
