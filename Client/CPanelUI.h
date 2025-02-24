#pragma once
#include "CUI.h"
class CPanelUI :
    public CUI
{

private:
    Vec2        m_vDragStart;
    bool        m_bCanMove;

public:
    void SetCanMove(bool _b) { m_bCanMove = _b; }

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


