#pragma once
#include "CUI.h"
class CSpriteUI 
    : public CUI
{
private:
    //CTexture* m_pTexture; // GDI+ Bitmap 객체 (PNG 이미지)
    CTexture* m_pBgTexture;     // 배경 텍스처
    CTexture* m_pFillTexture;   // 채워지는 텍스처
    CTexture* m_pFrameTexture;  // 테두리 텍스처
    float m_fHpRatio;            // HP 비율 (0.0 ~ 1.0)

public:
    CLONE(CSpriteUI);

public:
    CSpriteUI();
    virtual ~CSpriteUI();
    void SetTexture(const wstring& _bgKey, const wstring& _bgPath
        , const wstring& _fillKey, const wstring& _fillPath
        , const wstring& _frameKey, const wstring& _framePath);
    void SetHpRatio(float _ratio);
    virtual void render(Graphics* _pDGraphics);  // GDI+ 기반 렌더링 함수
};