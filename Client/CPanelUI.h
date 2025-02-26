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
    D2D1::ColorF    m_colorNormal;

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


