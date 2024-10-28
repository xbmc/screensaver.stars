/*
 *  Copyright (C) 2004-2021 Team Kodi (https://kodi.tv)
 *  Copyright (C) 2004 Chris Barnett (Forza)
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSE.md for more information.
 */

/*
 *  based on "stars5" from hugo elias (http://freespace.virgin.net/hugo.elias)
 */

#include "StarField.h"

#include <stdio.h>
#include <memory.h>
#include <algorithm>
#include <stddef.h>

const int POINTSPERSTAR = 2;

CStarField::CStarField(void)
  : m_nStarCnt(1000)
  , m_fGammaValue(1.0f)
  , m_fBrightness(0.3f)
  , m_fMaxVelocity(10.f)
  , m_fVelocity(0.f)
  , m_fZoom(3.f/2.f)
  , m_fFieldExpanse(1.5f)
{
  memset(&m_Screen, 0, sizeof(m_Screen));
  memset(&m_Field, 0, sizeof(m_Field));
  memset(&m_Cam, 0, sizeof(m_Cam));
  memset(&m_dwPalette, 0, sizeof(m_dwPalette));

  for (int i = 0; i < 256; i++)
  {
    m_fBrightTable[i] = 0.f;
  }
}

CStarField::CStarField(unsigned int nNumStars, float fGamma, float fBrightness,
                       float fSpeed, float fZoom, float fExpanse, void* pContext)
  : m_nStarCnt(nNumStars)
  , m_fGammaValue(fGamma)
  , m_fBrightness(fBrightness)
  , m_fMaxVelocity(fSpeed)
  , m_fVelocity(0.f)
  , m_fZoom(fZoom)
  , m_fFieldExpanse(fExpanse)
#ifdef WIN32
  , m_pContext(reinterpret_cast<ID3D11DeviceContext*>(pContext))
#endif
{
  memset(&m_Screen, 0, sizeof(m_Screen));
  memset(&m_Field, 0, sizeof(m_Field));
  memset(&m_Cam, 0, sizeof(m_Cam));
  memset(&m_dwPalette, 0, sizeof(m_dwPalette));

  for (int i = 0; i < 256; i++)
  {
    m_fBrightTable[i] = 0.f;
  }

#ifdef WIN32
  InitDXStuff();
#endif
}

CStarField::~CStarField(void)
{
  Destroy();
}

#ifdef WIN32
const BYTE PixelShader[] =
{
     68,  88,  66,  67,  18, 124,
    182,  35,  30, 142, 196, 211,
     95, 130,  91, 204,  99,  13,
    249,   8,   1,   0,   0,   0,
    124,   1,   0,   0,   4,   0,
      0,   0,  48,   0,   0,   0,
    124,   0,   0,   0, 188,   0,
      0,   0,  72,   1,   0,   0,
     65, 111, 110,  57,  68,   0,
      0,   0,  68,   0,   0,   0,
      0,   2, 255, 255,  32,   0,
      0,   0,  36,   0,   0,   0,
      0,   0,  36,   0,   0,   0,
     36,   0,   0,   0,  36,   0,
      0,   0,  36,   0,   0,   0,
     36,   0,   0,   2, 255, 255,
     31,   0,   0,   2,   0,   0,
      0, 128,   0,   0,  15, 176,
      1,   0,   0,   2,   0,   8,
     15, 128,   0,   0, 228, 176,
    255, 255,   0,   0,  83,  72,
     68,  82,  56,   0,   0,   0,
     64,   0,   0,   0,  14,   0,
      0,   0,  98,  16,   0,   3,
    242,  16,  16,   0,   1,   0,
      0,   0, 101,   0,   0,   3,
    242,  32,  16,   0,   0,   0,
      0,   0,  54,   0,   0,   5,
    242,  32,  16,   0,   0,   0,
      0,   0,  70,  30,  16,   0,
      1,   0,   0,   0,  62,   0,
      0,   1,  73,  83,  71,  78,
    132,   0,   0,   0,   4,   0,
      0,   0,   8,   0,   0,   0,
    104,   0,   0,   0,   0,   0,
      0,   0,   1,   0,   0,   0,
      3,   0,   0,   0,   0,   0,
      0,   0,  15,   0,   0,   0,
    116,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,
      3,   0,   0,   0,   1,   0,
      0,   0,  15,  15,   0,   0,
    122,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,
      3,   0,   0,   0,   2,   0,
      0,   0,   3,   0,   0,   0,
    122,   0,   0,   0,   1,   0,
      0,   0,   0,   0,   0,   0,
      3,   0,   0,   0,   2,   0,
      0,   0,  12,   0,   0,   0,
     83,  86,  95,  80,  79,  83,
     73,  84,  73,  79,  78,   0,
     67,  79,  76,  79,  82,   0,
     84,  69,  88,  67,  79,  79,
     82,  68,   0, 171,  79,  83,
     71,  78,  44,   0,   0,   0,
      1,   0,   0,   0,   8,   0,
      0,   0,  32,   0,   0,   0,
      0,   0,   0,   0,   0,   0,
      0,   0,   3,   0,   0,   0,
      0,   0,   0,   0,  15,   0,
      0,   0,  83,  86,  95,  84,
     65,  82,  71,  69,  84,   0,
    171, 171
};


void CStarField::InitDXStuff(void)
{
  ID3D11Device* pDevice = nullptr;
  m_pContext->GetDevice(&pDevice);

  CD3D11_BUFFER_DESC vbDesc(sizeof(ST_CUSTOMVERTEX) * m_nStarCnt * POINTSPERSTAR, D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
  pDevice->CreateBuffer(&vbDesc, nullptr, &m_pVBuffer);

  pDevice->CreatePixelShader(PixelShader, sizeof(PixelShader), nullptr, &m_pPShader);

  SAFE_RELEASE(pDevice);
}
#endif // WIN32

int CStarField::Create(int iWidth, int iHeight)
{
  m_Screen.iWidth = iWidth;
  m_Screen.iHeight = iHeight;
  m_Screen.iMidX = iWidth / 2;
  m_Screen.iMidY = iHeight / 2;

  m_Field.fMinX   = -320.f;
  m_Field.fMaxX   =  320.f;
  m_Field.fWidth  = m_Field.fMaxX - m_Field.fMinX;
  m_Field.fMinY   = -320.f;
  m_Field.fMaxY   =  320.f;
  m_Field.fHeight = m_Field.fMaxY - m_Field.fMinY;
  m_Field.fMinZ   = -128.f;
  m_Field.fMaxZ   = 640.f;
  m_Field.fLength = m_Field.fMaxZ - m_Field.fMinZ;

  if (m_pStars)
  {
    delete[] m_pStars;
  }

  m_pStars = new ST_STAR[m_nStarCnt];

  if (m_pStars == nullptr)
  {
    return -1;
  }

  unsigned int n;

  for (n = 0; n < 256; n++)
  {
    double br = pow(256 / (double)std::max(n, (unsigned)1), 2) - pow(256 / 255., 2);

    if (br < 0)
    {
      br = 0.;
    }

    br = pow(1. - pow((1. - m_fBrightness), br), 0.66);

    m_fBrightTable[n] = (float)(br * 255);
  }

  for (n = 0; n < m_nStarCnt; n++)
  {
    ResetStar(&m_pStars[n]);
    m_pStars[n].z = RangeRand(m_Field.fMinZ, m_Field.fLength);
  }

  for (n = 1; n < 256; n++)
  {
    unsigned char c = GammaCorrect(n, m_fGammaValue);
    SetPalette(n, c, c, c);
  }

#ifndef WIN32
  if (m_pVertices)
  {
    delete[] m_pVertices;
  }

  m_pVertices = new ST_CUSTOMVERTEX[m_nStarCnt * POINTSPERSTAR];

  if (m_pVertices == nullptr)
  {
    return -1;
  }

  m_pCurVertice = m_pVertices;

  std::string fraqShader = kodi::addon::GetAddonPath("resources/shaders/" GL_TYPE_STRING "/frag.glsl");
  std::string vertShader = kodi::addon::GetAddonPath("resources/shaders/" GL_TYPE_STRING "/vert.glsl");
  if (!LoadShaderFiles(vertShader, fraqShader) || !CompileAndLink())
    return -1;

  glGenBuffers(1, &m_vertexVBO);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
#endif

  return 0;
}

char CStarField::GammaCorrect(unsigned char c, float g)
{
  return (char)(pow(((float)c) / 255.f, 1.f / g) * 255.f);
}

void CStarField::Destroy(void)
{
  delete[] m_pStars;
  m_pStars = nullptr;

  m_pCurVertice = nullptr;
#ifndef WIN32
  delete[] m_pVertices;
  m_pVertices = nullptr;

  glDeleteBuffers(1, &m_vertexVBO);
  m_vertexVBO = 0;

#else
  SAFE_RELEASE(m_pVBuffer);
  SAFE_RELEASE(m_pPShader);
#endif
}

int CStarField::RenderFrame(void)
{
  if (m_pStars == nullptr)
  {
    return -1;
  }

  m_Screen.fZoom = (float)m_Screen.iMidX * m_fZoom;
  m_fVelocity += (m_fMaxVelocity - m_fVelocity) * .01f;

  m_Cam.aacc = (m_Cam.aacc + RangeRand(-.00001f, .00002f)) * .99f;
  m_Cam.bacc = (m_Cam.bacc + RangeRand(-.00001f, .00002f)) * .99f;
  m_Cam.cacc = (m_Cam.cacc + RangeRand(-.00001f, .00002f)) * .992f;
  m_Cam.avel = (m_Cam.avel + m_Cam.aacc) * .99f;
  m_Cam.bvel = (m_Cam.bvel + m_Cam.bacc) * .99f;
  m_Cam.cvel = (m_Cam.cvel + m_Cam.cacc) * .992f;
  m_Cam.a    = (m_Cam.a + m_Cam.avel) * .99f;
  m_Cam.b    = (m_Cam.b + m_Cam.bvel) * .99f;
  m_Cam.c    = (m_Cam.c + m_Cam.cvel) * .992f;

  float sina = (float)sin(-m_Cam.a);
  float cosa = (float)cos(-m_Cam.a);
  float sinb = (float)sin(-m_Cam.b);
  float cosb = (float)cos(-m_Cam.b);
  float sinc = (float)sin(-m_Cam.c);
  float cosc = (float)cos(-m_Cam.c);

#ifndef WIN32
  m_pCurVertice = m_pVertices;
#else
  D3D11_MAPPED_SUBRESOURCE res = {};
  if (SUCCEEDED(m_pContext->Map(m_pVBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res)))
    m_pCurVertice = (ST_CUSTOMVERTEX*)res.pData;
#endif
  m_nDrawnStars = 0;

  for (unsigned int n = 0; n < m_nStarCnt; n++)
  {
    ST_STAR* pStar = &m_pStars[n];

    // move star
    pStar->z -= pStar->v * m_fVelocity;

    if (pStar->z < m_Field.fMinZ)
    {
      ResetStar(pStar);
    }

    // rotate star
    // rotate wrt a
    pStar->rx = pStar->x * cosa + pStar->z * sina;
    pStar->rz = pStar->z * cosa - pStar->x * sina;
    // rotate wrt b
    pStar->ry = pStar->y * cosb + pStar->rz * sinb;
    pStar->rz = pStar->rz * cosb - pStar->y * sinb;
    // rotate wrt c
    float fTempRX = pStar->rx * cosc + pStar->ry * sinc;
    pStar->ry = pStar->ry * cosc - pStar->rx * sinc;
    pStar->rx = fTempRX;

    // prepare to plot
    float nsx = (float)m_Screen.iMidX + pStar->rx / pStar->rz * m_Screen.fZoom;
    float nsy = (float)m_Screen.iMidY - pStar->ry / pStar->rz * m_Screen.fZoom;

    int dist = (int)pStar->rz;

    float xd = nsx - pStar->sx;
    float yd = nsy - pStar->sy;

    float len = sqrt(xd*xd + yd*yd);

    float c = m_fBrightTable[std::min(dist >> 1, 255)];

    // Unused because stars are almost invisible.
    /*
    if (len > 2)
      c /= (len / 2);
    */

    // plot star
    if (pStar->rz > 1.0f)
    {
      if (pStar->plot)
      {
        if (pStar->sx > 2 && pStar->sx < m_Screen.iWidth &&
            pStar->sy > 2 && pStar->sy < m_Screen.iHeight - 2 &&
            nsx > 2 && nsx < m_Screen.iWidth &&
            nsy > 2 && nsy < m_Screen.iHeight - 2)
        {
          DrawStar(pStar->sx, pStar->sy, nsx, nsy, (int)c);
        }
      }

      pStar->sx = nsx;
      pStar->sy = nsy;
      pStar->plot = true;
    }
  }

  DoDraw();

  return 0;
}

void CStarField::ResetStar(ST_STAR* pStar)
{
  pStar->v = 1.f / (float)pow(.1f + 9.9f * (float)rand() / (float)RAND_MAX, m_fFieldExpanse);
  pStar->x = RangeRand(m_Field.fMinX, m_Field.fWidth);
  pStar->y = RangeRand(m_Field.fMinY, m_Field.fHeight);
  pStar->z = m_Field.fMaxZ;
  pStar->plot = false;
}

void CStarField::DrawStar(float x1, float y1, float x2, float y2, int iBrightness)
{
  if (
#ifndef WIN32
    m_pVertices == nullptr ||
#endif
    m_pCurVertice == nullptr)
  {
    return;
  }

#ifndef WIN32
  m_pCurVertice->x = (x1 - m_Screen.iWidth / 2.0f) / m_Screen.iWidth * 2.0f;
  m_pCurVertice->y = (y1 - m_Screen.iHeight / 2.0f) / m_Screen.iHeight * 2.0f;
  m_pCurVertice->z = 0.0f;
  m_pCurVertice->color = m_dwPalette[iBrightness];
  m_pCurVertice++;

  m_pCurVertice->x = ((x2 > x1 ? x2 + 1 : x2 - 1) - m_Screen.iWidth / 2.0f) / m_Screen.iWidth * 2.0f;
  m_pCurVertice->y = ((y2 > y1 ? y2 + 1 : y2 - 1) - m_Screen.iHeight / 2.0f) / m_Screen.iHeight * 2.0f;
  m_pCurVertice->z = 0.0f;
  m_pCurVertice->color = m_dwPalette[iBrightness];
  m_pCurVertice++;
#else
  m_pCurVertice->x = x1;
  m_pCurVertice->y = y1;
  m_pCurVertice->z = 0.0f;
  m_pCurVertice->color = m_dwPalette[iBrightness];
  m_pCurVertice++;

  m_pCurVertice->x = x2 > x1 ? x2 + 1 : x2 - 1;
  m_pCurVertice->y = y2 > y1 ? y2 + 1 : y2 - 1;
  m_pCurVertice->z = 0.0f;
  m_pCurVertice->color = m_dwPalette[iBrightness];
  m_pCurVertice++;
#endif

  m_nDrawnStars++;
}

void CStarField::DoDraw(void)
{
  if (m_nStarCnt == 0 || m_nDrawnStars == 0)
  {
    return;
  }
#ifndef WIN32
  glClear(GL_COLOR_BUFFER_BIT);

  size_t nVSize = m_nDrawnStars * POINTSPERSTAR;

  EnableShader();

  glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(ST_CUSTOMVERTEX)*nVSize, m_pVertices, GL_STATIC_DRAW);

  glVertexAttribPointer(m_aPosition, 3, GL_FLOAT, GL_FALSE, sizeof(ST_CUSTOMVERTEX), BUFFER_OFFSET(offsetof(ST_CUSTOMVERTEX, x)));
  glEnableVertexAttribArray(m_aPosition);

  glVertexAttribPointer(m_aColor, 4, GL_FLOAT, GL_FALSE, sizeof(ST_CUSTOMVERTEX), BUFFER_OFFSET(offsetof(ST_CUSTOMVERTEX, color)));
  glEnableVertexAttribArray(m_aColor);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glEnable(GL_BLEND);
  glDrawArrays(GL_LINES, 0, nVSize);

  glDisableVertexAttribArray(m_aPosition);
  glDisableVertexAttribArray(m_aColor);

  DisableShader();

#else
  ID3D11RenderTargetView* renderTargetView;
  m_pContext->OMGetRenderTargets(1, &renderTargetView, nullptr);
  float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
  m_pContext->ClearRenderTargetView(renderTargetView, clearColor);
  SAFE_RELEASE(renderTargetView);

  m_pContext->Unmap(m_pVBuffer, 0);
  m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
  UINT strides = sizeof(ST_CUSTOMVERTEX), offsets = 0;
  m_pContext->IASetVertexBuffers(0, 1, &m_pVBuffer, &strides, &offsets);
  m_pContext->PSSetShader(m_pPShader, nullptr, 0);
  m_pContext->Draw(m_nDrawnStars * POINTSPERSTAR, 0);
#endif // !WIN32
}

void CStarField::SetPalette(unsigned int nIndex, int iRed, int iGreen, int iBlue)
{
  if (nIndex < 256)
  {
    m_dwPalette[nIndex] = CRGBA(iRed / 255.0f, iGreen / 255.0f, iBlue / 255.0f, 1.0f);
  }
}

#ifndef WIN32
void CStarField::OnCompiledAndLinked()
{
  m_aPosition = glGetAttribLocation(ProgramHandle(), "a_position");
  m_aColor = glGetAttribLocation(ProgramHandle(), "a_color");
}
#endif // !WIN32
