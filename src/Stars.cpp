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

#include <kodi/addon-instance/Screensaver.h>
#ifndef WIN32
#include <GL/gl.h>
#endif

#include "StarField.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

struct ST_SETTINGS
{
  int   iNumStars;
  float fGamma;
  float fBrightness;
  float fSpeed;
  float fZoom;
  float fExpanse;
};

class CScreensaverStars
  : public kodi::addon::CAddonBase,
    public kodi::addon::CInstanceScreensaver
{
public:
  CScreensaverStars();

  virtual bool Start() override;
  virtual void Stop() override;
  virtual void Render() override;

private:
  ST_SETTINGS m_Settings;
  CStarField* m_pStarField;
};

CScreensaverStars::CScreensaverStars()
  : m_pStarField(nullptr)
{
  m_Settings.iNumStars = kodi::GetSettingInt("numstars");
  m_Settings.fGamma = kodi::GetSettingFloat("gamma");
  m_Settings.fBrightness = kodi::GetSettingFloat("brightness");
  m_Settings.fSpeed = kodi::GetSettingFloat("speed");
  m_Settings.fZoom = kodi::GetSettingFloat("zoom");
  m_Settings.fExpanse = kodi::GetSettingFloat("expanse");
}

bool CScreensaverStars::Start()
{
  srand(time(nullptr));

  m_pStarField = new CStarField(m_Settings.iNumStars,
      m_Settings.fGamma,
      m_Settings.fBrightness,
      m_Settings.fSpeed,
      m_Settings.fZoom,
      m_Settings.fExpanse,
      Device());
  if (!m_pStarField)
    return false;
  m_pStarField->Create(Width(), Height());
  return true;
}

void CScreensaverStars::Render()
{
  if (m_pStarField)
  {
    m_pStarField->RenderFrame();
  }
}

void CScreensaverStars::Stop()
{
  delete m_pStarField;
  m_pStarField = nullptr;
}

ADDONCREATOR(CScreensaverStars);
