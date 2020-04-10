/*
 *  Copyright (C) 2004-2020 Team Kodi (https://kodi.tv)
 *  Copyright (C) 2004 Chris Barnett (Forza)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

/*
 * Adapted from the Pyro screen saver by
 *
 *  Jamie Zawinski <jwz@jwz.org>
 */

#include "StarField.h"

#include <kodi/addon-instance/Screensaver.h>
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

class ATTRIBUTE_HIDDEN CScreensaverStars
  : public kodi::addon::CAddonBase,
    public kodi::addon::CInstanceScreensaver
{
public:
  CScreensaverStars();

  bool Start() override;
  void Stop() override;
  void Render() override;

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
