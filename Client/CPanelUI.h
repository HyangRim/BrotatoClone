#pragma once
#include "CUI.h"
class CPanelUI :
    public CUI
{

private:
    Vec2            m_vDragStart;
    bool            m_bCanMove;

    float           m_fradiusX;
    float           m_fradiusY;

    //MouseOn일때와 평상시일때 색
    D2D1::ColorF    m_colorMouseOn;         //0~1 정규화 시켜야됨
    float           m_fMouseOnAlpha;        //마우스on일때 alpha값

    D2D1::ColorF    m_colorNormal;
    float           m_fNormalAlpha;         //일반상태일떄 alpha값
public:
    void SetCanMove(bool _b) { m_bCanMove = _b; }

public:
    void SetRadius(float _fradiusX, float _fradiusY)
    {
        m_fradiusX = _fradiusX;
        m_fradiusY = _fradiusY;
    }
    void SetColor(D2D1::ColorF _mouseOn, D2D1::ColorF _normal)
    {
        m_colorMouseOn = _mouseOn;
        m_colorNormal = _normal;
    }
    void SetNormalAlpha(float _v) { m_fNormalAlpha = _v; }
    void SetMouseOnAlpha(float _v) { m_fMouseOnAlpha = _v; }


public:
    virtual void update();
    virtual void MouseOn();
    virtual void MouseLbtnDown();
    virtual void MouseLbtnUp();
    virtual void render(HDC _dc);
    virtual void render(Gdiplus::Graphics* _pDGraphics);
    virtual void render(ID2D1HwndRenderTarget* _pRender);

    CLONE(CPanelUI)
public:
    CPanelUI();
    ~CPanelUI();
};


