/* Implements transactional whole-file loading for CE1CHS documents. */
/* Depends only on CE 1.0 file APIs and page-backed virtual memory. */
#include "document.h"

static DWORD Ce1DocumentError(void)
{
    DWORD error;

    error = GetLastError();
    if (error == ERROR_SUCCESS)
    {
        error = ERROR_GEN_FAILURE;
    }
    return error;
}

BOOL Ce1DocumentClear(CE1DOCUMENT *pDocument)
{
    if (pDocument == NULL)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    if (pDocument->pBytes != NULL)
    {
        if (!VirtualFree((LPVOID)pDocument->pBytes, 0, MEM_RELEASE))
        {
            return FALSE;
        }
    }

    pDocument->pBytes = NULL;
    pDocument->length = 0;
    pDocument->loaded = FALSE;
    return TRUE;
}

BOOL Ce1DocumentLoad(CE1DOCUMENT *pDocument, LPCTSTR pszPath)
{
    HANDLE hFile;
    BYTE *pNewBytes;
    DWORD fileSize;
    DWORD fileSizeHigh;
    DWORD bytesRead;
    DWORD pathLength;
    DWORD savedError;
    BOOL success;

    if (pDocument == NULL || pszPath == NULL || pszPath[0] != '\\')
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    pathLength = 0;
    while (pathLength < MAX_PATH && pszPath[pathLength] != 0)
    {
        ++pathLength;
    }
    if (pathLength <= 1 || pathLength >= MAX_PATH)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    hFile = INVALID_HANDLE_VALUE;
    pNewBytes = NULL;
    fileSize = 0;
    fileSizeHigh = 0;
    bytesRead = 0;
    savedError = ERROR_SUCCESS;
    success = FALSE;

    hFile = CreateFile(pszPath, GENERIC_READ, FILE_SHARE_READ, NULL,
                       OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        savedError = Ce1DocumentError();
        goto cleanup;
    }

    SetLastError(ERROR_SUCCESS);
    fileSize = GetFileSize(hFile, &fileSizeHigh);
    if (fileSize == 0xFFFFFFFFUL && GetLastError() != ERROR_SUCCESS)
    {
        savedError = Ce1DocumentError();
        goto cleanup;
    }

    if (fileSizeHigh != 0 || fileSize > CE1_DOCUMENT_MAX_BYTES)
    {
        savedError = ERROR_NOT_SUPPORTED;
        goto cleanup;
    }

    if (fileSize != 0)
    {
        pNewBytes = (BYTE *)VirtualAlloc(NULL, fileSize, MEM_COMMIT,
                                        PAGE_READWRITE);
        if (pNewBytes == NULL)
        {
            savedError = Ce1DocumentError();
            goto cleanup;
        }

        if (!ReadFile(hFile, (LPVOID)pNewBytes, fileSize, &bytesRead, NULL))
        {
            savedError = Ce1DocumentError();
            goto cleanup;
        }
        if (bytesRead != fileSize)
        {
            savedError = ERROR_READ_FAULT;
            goto cleanup;
        }
    }

    if (!CloseHandle(hFile))
    {
        savedError = Ce1DocumentError();
        hFile = INVALID_HANDLE_VALUE;
        goto cleanup;
    }
    hFile = INVALID_HANDLE_VALUE;

    if (!Ce1DocumentClear(pDocument))
    {
        savedError = Ce1DocumentError();
        goto cleanup;
    }

    pDocument->pBytes = pNewBytes;
    pDocument->length = fileSize;
    pDocument->loaded = TRUE;
    success = TRUE;

cleanup:
    if (hFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hFile);
    }
    if (!success && pNewBytes != NULL)
    {
        VirtualFree((LPVOID)pNewBytes, 0, MEM_RELEASE);
    }
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
