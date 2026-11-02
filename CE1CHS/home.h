/* Defines the fully owner-drawn CE1CHS home page. */
/* Depends on the core text renderer and the multilingual UI string table. */
#ifndef CE1_HOME_H
#define CE1_HOME_H

#ifndef _WIN32_WCE
#define _WIN32_WCE 100
#endif

#include <windows.h>
#include "ce1core.h"

#define CE1_HOME_ACTION_NONE     (-1)
#define CE1_HOME_ACTION_OPEN      0
#define CE1_HOME_ACTION_SETTINGS  1
#define CE1_HOME_ACTION_ABOUT     2

void Ce1HomeInit(void);
void Ce1HomeCancel(HWND hWnd);
BOOL Ce1HomePaint(HWND hWnd, CE1COREHANDLE hCore, HBITMAP hBackground,
                  int commandBarHeight);
void Ce1HomeMouseDown(HWND hWnd, int commandBarHeight, LPARAM lParam);
void Ce1HomeMouseMove(HWND hWnd, int commandBarHeight, LPARAM lParam);
int Ce1HomeMouseUp(HWND hWnd, int commandBarHeight, LPARAM lParam);

#endif
