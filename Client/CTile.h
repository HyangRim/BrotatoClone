#pragma once
#include "CObject.h"


class CTexture;
struct ID2D1Bitmap;

class CTile :
    public CObject
{

private:
    CTexture*       m_pTileTex;
    ID2D1Bitmap*    m_pTileBit;
    int             m_iImgIdx;

public:
    void SetTexture(CTexture* _pTex) {
        m_pTileTex = _pTex;
    }

    void AddImgIdx() {
        ++m_iImgIdx;
    }

    void SetImgIdx(int _index) { m_iImgIdx = _index; }

private:
    virtual void update();
    virtual void render(HDC _dc);
    virtual void render(Gdiplus::Graphics* _pDGraphics);
    virtual void render(ID2D1HwndRenderTarget* _pRender);

public:
    virtual void Save(FILE* _pFile);
    virtual void Load(FILE* _pFile);
    CLONE(CTile);
public:
    CTile();
    ~CTile();
};

