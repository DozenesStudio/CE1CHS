/* Implements the CE1CHS core context and lifecycle entry points. */
/* Depends on the Windows CE heap, font lifecycle, and page layout modules. */
#include "ce1core.h"
#include "font.h"
#include "layout.h"

#define CE1_CORE_SIGNATURE       0x31454831UL
#define CE1_CORE_STATE_ACTIVE    1UL
#define CE1_CORE_STATE_HIBERNATE 2UL
#define CE1_CORE_STATE_DESTROY   3UL
#define CE1_DEFAULT_FONT_PATH    TEXT("\\CE1CHS\\HZK16")

typedef struct CE1CORE
{
    DWORD signature;
    DWORD state;
    CE1FONT font;
} CE1CORE;

static BOOL Ce1CoreIsValid(CE1COREHANDLE hCore)
{
    return (hCore != NULL && hCore->signature == CE1_CORE_SIGNATURE);
}

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    (void)hInstance;
    (void)dwReason;
    (void)lpReserved;
    return TRUE;
}

BOOL CE1CoreCreate(LPCTSTR pszFontPath, CE1COREHANDLE *phCore)
{
    CE1CORE *pCore;
    HANDLE hHeap;
    DWORD savedError;
    int pathLength;

    if (phCore == NULL)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    *phCore = NULL;

    if (pszFontPath == NULL || pszFontPath[0] == 0)
    {
        pszFontPath = CE1_DEFAULT_FONT_PATH;
    }

    pathLength = lstrlen(pszFontPath);
    if (pathLength <= 0 || pathLength >= MAX_PATH)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    hHeap = GetProcessHeap();
    if (hHeap == NULL)
    {
        return FALSE;
    }

    pCore = (CE1CORE *)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(CE1CORE));
    if (pCore == NULL)
    {
        SetLastError(ERROR_NOT_ENOUGH_MEMORY);
        return FALSE;
    }

    pCore->signature = CE1_CORE_SIGNATURE;
    pCore->state = CE1_CORE_STATE_ACTIVE;
    if (!Ce1FontOpen(&pCore->font, pszFontPath))
    {
        savedError = GetLastError();
        if (pCore->font.hFile != NULL)
        {
            Ce1FontClose(&pCore->font);
        }
        HeapFree(hHeap, 0, (LPVOID)pCore);
        SetLastError(savedError);
        return FALSE;
    }

    *phCore = pCore;
    return TRUE;
}

BOOL CE1CoreDestroy(CE1COREHANDLE *phCore)
{
    CE1CORE *pCore;
    HANDLE hHeap;

    if (phCore == NULL || *phCore == NULL)
    {
        return TRUE;
    }

    pCore = *phCore;
    if (!Ce1CoreIsValid(pCore))
    {
        SetLastError(ERROR_INVALID_HANDLE);
        return FALSE;
    }

    hHeap = GetProcessHeap();
    if (hHeap == NULL)
    {
        return FALSE;
    }

    if (!Ce1FontClose(&pCore->font))
    {
        return FALSE;
    }

    pCore->state = CE1_CORE_STATE_DESTROY;

    if (!HeapFree(hHeap, 0, (LPVOID)pCore))
    {
        return FALSE;
    }

    *phCore = NULL;
    return TRUE;
}

BOOL CE1CoreHibernate(CE1COREHANDLE hCore)
{
    if (!Ce1CoreIsValid(hCore))
    {
        SetLastError(ERROR_INVALID_HANDLE);
        return FALSE;
    }

    if (hCore->state == CE1_CORE_STATE_HIBERNATE)
    {
        return TRUE;
    }

    if (hCore->state != CE1_CORE_STATE_ACTIVE)
    {
        SetLastError(ERROR_INVALID_HANDLE);
        return FALSE;
    }

    if (!Ce1FontHibernate(&hCore->font))
    {
        return FALSE;
    }

    hCore->state = CE1_CORE_STATE_HIBERNATE;
    return TRUE;
}

BOOL CE1CoreActivate(CE1COREHANDLE hCore)
{
    if (!Ce1CoreIsValid(hCore))
    {
        SetLastError(ERROR_INVALID_HANDLE);
        return FALSE;
    }

    if (hCore->state == CE1_CORE_STATE_ACTIVE)
    {
        return TRUE;
    }

    if (hCore->state != CE1_CORE_STATE_HIBERNATE)
    {
        SetLastError(ERROR_INVALID_HANDLE);
        return FALSE;
    }

    if (!Ce1FontActivate(&hCore->font))
    {
        return FALSE;
    }

    hCore->state = CE1_CORE_STATE_ACTIVE;
    return TRUE;
}

BOOL CE1CoreDrawPage(CE1COREHANDLE hCore, HDC hDC, const BYTE *pBuffer,
                     DWORD bufLen, DWORD startOffset, const RECT *pRect,
                     DWORD *pNextOffset)
{
    if (!Ce1CoreIsValid(hCore))
    {
        SetLastError(ERROR_INVALID_HANDLE);
        return FALSE;
    }

    if (hCore->state != CE1_CORE_STATE_ACTIVE)
    {
        SetLastError(ERROR_INVALID_HANDLE);
        return FALSE;
    }

    if (hDC == NULL)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    return Ce1LayoutPage(&hCore->font, hDC, pBuffer, bufLen, startOffset,
                         pRect, pNextOffset);
}

BOOL CE1CoreMeasurePage(CE1COREHANDLE hCore, const BYTE *pBuffer,
                        DWORD bufLen, DWORD startOffset, int width, int height,
                        DWORD *pNextOffset)
{
    RECT rect;

    if (!Ce1CoreIsValid(hCore))
    {
        SetLastError(ERROR_INVALID_HANDLE);
        return FALSE;
    }

    if (hCore->state != CE1_CORE_STATE_ACTIVE)
    {
        SetLastError(ERROR_INVALID_HANDLE);
        return FALSE;
    }

    rect.left = 0;
    rect.top = 0;
    rect.right = width;
    rect.bottom = height;
    return Ce1LayoutPage(&hCore->font, NULL, pBuffer, bufLen, startOffset,
                         &rect, pNextOffset);
}
