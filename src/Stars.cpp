/*
 * Pyro Screensaver for XBox Media Center
 * Copyright (c) 2004 Team XBMC
 *
 * Ver 1.0 15 Nov 2004	Chris Barnett (Forza)
 *
 * Adapted from the Pyro screen saver by
 *
 *  Jamie Zawinski <jwz@jwz.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <xbmc_scr_dll.h>
#include <GL/gl.h>

#include "StarField.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

CStarField* g_pStarField = NULL;

struct ST_SETTINGS
{
	char szScrName[1024];
	int iWidth;
	int iHeight;
	
	int   iNumStars;
	float fGamma;
	float fBrightness;
	float fSpeed;
	float fZoom;
	float fExpanse;
	void* pContext;
};

struct ST_SETTINGS g_Settings = 
{
	"", 0, 0, 1000, 1.f, 0.2f, 10.0f, 1.5f, 1.5f, NULL
};

ADDON_STATUS ADDON_Create(void* hdl, void* props)
{
  if (!props)
    return ADDON_STATUS_UNKNOWN;

  SCR_PROPS* scrprops = (SCR_PROPS*)props;

  g_Settings.iWidth = scrprops->width;
  g_Settings.iHeight = scrprops->height;
  g_Settings.pContext = scrprops->device;

  return ADDON_STATUS_NEED_SETTINGS;
}

extern "C" void Start()
{
  srand(time(NULL));

  g_pStarField = new CStarField(g_Settings.iNumStars,
      g_Settings.fGamma,
      g_Settings.fBrightness,
      g_Settings.fSpeed,
      g_Settings.fZoom,
      g_Settings.fExpanse,
      g_Settings.pContext);
  if (g_pStarField)
    g_pStarField->Create(g_Settings.iWidth, g_Settings.iHeight);
}

extern "C" void Render()
{	
  if (g_pStarField)
  {
    g_pStarField->RenderFrame();
  }
}

extern "C" void ADDON_Stop()
{
  delete g_pStarField;
  g_pStarField = NULL;
}

void ADDON_Destroy()
{
}

ADDON_STATUS ADDON_GetStatus()
{
  return ADDON_STATUS_OK;
}

bool ADDON_HasSettings()
{
  return true;
}

unsigned int ADDON_GetSettings(ADDON_StructSetting ***sSet)
{
  return 0;
}

ADDON_STATUS ADDON_SetSetting(const char *strSetting, const void *value)
{
  if (strcmp(strSetting, "numstars") == 0)
    g_Settings.iNumStars = *(int*)value;
  if (strcmp(strSetting, "gamma") == 0)
    g_Settings.fGamma = *(float*)value;
  if (strcmp(strSetting, "brightness") == 0)
    g_Settings.fBrightness = *(float*)value;
  if (strcmp(strSetting, "speed") == 0)
    g_Settings.fSpeed = *(float*)value;
  if (strcmp(strSetting, "zoom") == 0)
    g_Settings.fZoom = *(float*)value;
  if (strcmp(strSetting, "expanse") == 0)
    g_Settings.fExpanse = *(float*)value;

  return ADDON_STATUS_OK;
}

void ADDON_FreeSettings()
{
}

void ADDON_Announce(const char *flag, const char *sender, const char *message, const void *data)
{
}

void GetInfo(SCR_INFO *info)
{
}
