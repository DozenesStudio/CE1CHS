/* Defines immutable 1bpp CE1CHS logo bitmaps. */
/* Rows are WORD-aligned and bits are stored most-significant-bit first. */
#ifndef CE1_LOGO_H
#define CE1_LOGO_H

#ifndef _WIN32_WCE
#define _WIN32_WCE 100
#endif

#include <windows.h>

#define CE1_LOGO16_WIDTH  16
#define CE1_LOGO16_HEIGHT 16
#define CE1_LOGO16_STRIDE 2
#define CE1_LOGO16_BYTES  32

#define CE1_LOGO32_WIDTH  32
#define CE1_LOGO32_HEIGHT 32
#define CE1_LOGO32_STRIDE 4
#define CE1_LOGO32_BYTES  128

extern const BYTE g_ce1Logo16[CE1_LOGO16_BYTES];
extern const BYTE g_ce1Logo32[CE1_LOGO32_BYTES];

#endif
