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

    //Vec2 m_vOffset;
    bool            m_bBackGround;


    //일반상태 내부색, 외곽선색
    D2D1::ColorF    m_colorFillNormal;
    D2D1::ColorF    m_colorRoundedNormal;
    //마우스올렸을때 내부색, 외곽선색
    D2D1::ColorF    m_colorFillMouseOn;
    D2D1::ColorF    m_colorRoundedMouseOn;

    bool            m_bIsRoundedRect;
    float           m_fradiusX;
    float           m_fradiusY;
public:
    CLONE(CSpriteUI);

public:
    //void SetOffset(Vec2 _vOffset) { m_vOffset = _vOffset; }
    //Vec2 GetOffset() const { return m_vOffset; }
    void SetBackGround(bool _bBackGround) { m_bBackGround = _bBackGround; }
    void SetBackGroundColor(D2D1::ColorF _fillMouseOn, D2D1::ColorF _roundedMouseOn
                            , D2D1::ColorF _fillNormal, D2D1::ColorF _roundedNormal)
    {
        m_colorFillMouseOn = _fillMouseOn;
        m_colorRoundedMouseOn = _roundedMouseOn;
        m_colorFillNormal = _fillNormal;
        m_colorRoundedNormal = _roundedNormal;
    }
    void SetIsRound(bool _bIsRoundedRect, float _fradiusX, float _fradiusY)
    {
        m_bIsRoundedRect = _bIsRoundedRect;
        m_fradiusX = _fradiusX;
        m_fradiusY = _fradiusY;
    }

public:
    virtual void MouseLbtnClicked();    //클릭. 
    void finalupdate();
    CSpriteUI();
    virtual ~CSpriteUI();
    void SetTexture(const wstring& _bgKey, const wstring& _bgPath
        , const wstring& _fillKey, const wstring& _fillPath
        , const wstring& _frameKey, const wstring& _framePath);
    void SetHpRatio(float _ratio);

    void update();
  
    virtual void render(Graphics* _pDGraphics);  // GDI+ 기반 렌더링 함수
    virtual void render(ID2D1HwndRenderTarget* _pRender);
};