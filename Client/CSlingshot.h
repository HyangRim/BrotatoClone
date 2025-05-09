#pragma once
#include "CWeapon.h"
class CSlingshot :
    public CWeapon
{
private:
    float       m_fCoolTime;


public:
    virtual void ShotMissile(Vec2 _vDir);
public:
    virtual void update();
    virtual void render(Gdiplus::Graphics* _pDGraphics);
    virtual void render(ID2D1HwndRenderTarget* _pRender);

public:
    CLONE(CSlingshot)
public:
    CSlingshot();
    virtual ~CSlingshot();
};



