/* Defines bounded document buffering for the CE1CHS executable. */
/* Depends on Windows CE base types and an absolute object-store path. */
#ifndef CE1_DOCUMENT_H
#define CE1_DOCUMENT_H

#ifndef _WIN32_WCE
#define _WIN32_WCE 100
#endif

#include <windows.h>

#define CE1_DOCUMENT_MAX_BYTES 262144UL

typedef struct CE1DOCUMENT
{
    BYTE *pBytes;
    DWORD length;
    BOOL loaded;
} CE1DOCUMENT;

/* Loads at most 256 KB. The caller provides zero-initialized storage. */
/* Failure leaves any previously loaded document unchanged. */
BOOL Ce1DocumentLoad(CE1DOCUMENT *pDocument, LPCTSTR pszPath);

/* Releases the document buffer. Repeated calls are safe. */
BOOL Ce1DocumentClear(CE1DOCUMENT *pDocument);

#endif
