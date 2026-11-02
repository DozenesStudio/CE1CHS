/* Defines the settings page and its single language option. */
#ifndef CE1_SETTINGS_H
#define CE1_SETTINGS_H

#ifndef _WIN32_WCE
#define _WIN32_WCE 100
#endif

#include <windows.h>
#include "ce1core.h"

#define CE1_SETTINGS_ACTION_NONE     (-1)
#define CE1_SETTINGS_ACTION_LANGUAGE  0

void Ce1SettingsInit(void);
void Ce1SettingsCancel(HWND hWnd);
BOOL Ce1SettingsPaint(HWND hWnd, CE1COREHANDLE hCore, int commandBarHeight);
void Ce1SettingsMouseDown(HWND hWnd, int commandBarHeight, LPARAM lParam);
void Ce1SettingsMouseMove(HWND hWnd, int commandBarHeight, LPARAM lParam);
int Ce1SettingsMouseUp(HWND hWnd, int commandBarHeight, LPARAM lParam);

#endif
