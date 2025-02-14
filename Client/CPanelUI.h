#pragma once
#include "CUI.h"
class CPanelUI :
    public CUI
{

private:
    Vec2        m_vDragStart;

public:

    virtual void update();
    virtual void MouseOn();
    virtual void MouseLbtnDown();
    virtual void MouseLbtnUp();

    CLONE(CPanelUI)

public:
    CPanelUI();
    ~CPanelUI();
};


