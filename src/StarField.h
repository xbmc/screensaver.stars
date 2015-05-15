#pragma once

#include "types.h"
#ifdef WIN32
#include <d3d11.h>
#endif

class CStarField
{
protected:
	struct ST_STAR
	{
		float v;
		float x;
		float y;
		float z;
		float rx;
		float ry;
		float rz;
		float sx;
		float sy;
		bool  plot;
	};

	struct ST_ROTATION
	{
		float a, b, c;
		float avel, bvel, cvel;
		float aacc, bacc, cacc;

	};

	struct ST_FIELD
	{
		float fMinX;
		float fMaxX;
		float fWidth;
		float fMinY;
		float fMaxY;
		float fHeight;
		float fMinZ;
		float fMaxZ;
		float fLength;
	};

	struct ST_SCREEN
	{
		int iWidth;
		int iHeight;
		int iMidX;
		int iMidY;
		float fZoom;
	};

	struct ST_CUSTOMVERTEX
	{
		float x, y, z;		// The transformed position for the vertex.
                CRGBA color;
	};

public:
	CStarField(void);
	CStarField(unsigned int nNumStars, float fGamma, float fBrightness, 
			   float fSpeed, float fZoom, float fExpanse, void* pContext);
	virtual ~CStarField(void);

	int Create(int iWidth, int iHeight);
	void Destroy(void);
	int  RenderFrame(void);

protected:
	void DrawStar(float x1, float y1, float x2, float y2, int iBrightness);
	void DoDraw(void);
	void SetPalette(unsigned int nIndex, int iRed, int iGreen, int iBlue);

	void ResetStar(ST_STAR* pStar);
	
	char GammaCorrect(unsigned char c, float g);
	
	float RangeRand(float min, float range)
	{
		return min + (float)rand() / (float)RAND_MAX * range;
	}	
#ifdef WIN32
	void InitDXStuff(void);
#endif

protected:
	ST_SCREEN m_Screen;
	ST_FIELD m_Field;
	ST_ROTATION m_Cam;	
	ST_STAR* m_pStars;	
	unsigned int m_nStarCnt;	
	unsigned int m_nDrawnStars;	

	float m_fGammaValue;
	float m_fBrightness;
	float m_fBrightTable[256];

	float m_fMaxVelocity;
	float m_fVelocity;
	float m_fZoom;
	float m_fFieldExpanse;

	CRGBA m_dwPalette[256];

	ST_CUSTOMVERTEX* m_pVertices;
	ST_CUSTOMVERTEX* m_pCurVertice;
#ifdef WIN32
	ID3D11DeviceContext* m_pContext;
	ID3D11Buffer*        m_pVBuffer;
	ID3D11PixelShader*   m_pPShader;
#endif
};


