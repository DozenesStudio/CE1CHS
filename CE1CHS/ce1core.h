/* Public C interface for the CE1CHS display core DLL. */
/* Depends only on Windows CE 1.0 base types and calling conventions. */
#ifndef CE1CORE_H
#define CE1CORE_H

#ifndef _WIN32_WCE
#define _WIN32_WCE 100
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CE1CORE *CE1COREHANDLE;

/* Creates an active context. A null or empty path selects the default font path. */
BOOL CE1CoreCreate(LPCTSTR pszFontPath, CE1COREHANDLE *phCore);

/* Destroys a context and clears the caller handle. Null handles are accepted. */
BOOL CE1CoreDestroy(CE1COREHANDLE *phCore);

/* Releases transient resources and marks the context inactive. */
BOOL CE1CoreHibernate(CE1COREHANDLE hCore);

/* Restores transient resources and marks the context active. */
BOOL CE1CoreActivate(CE1COREHANDLE hCore);

/* Draws one page and returns the first offset not consumed by that page. */
BOOL CE1CoreDrawPage(CE1COREHANDLE hCore, HDC hDC, const BYTE *pBuffer,
                     DWORD bufLen, DWORD startOffset, const RECT *pRect,
                     DWORD *pNextOffset);

/* Measures one page without drawing, using the same pagination rules. */
BOOL CE1CoreMeasurePage(CE1COREHANDLE hCore, const BYTE *pBuffer,
                        DWORD bufLen, DWORD startOffset, int width, int height,
                        DWORD *pNextOffset);

#ifdef __cplusplus
}
#endif

#endif
