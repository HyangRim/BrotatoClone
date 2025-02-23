#pragma once
#include "CWeapon.h"

class CKnifeAI;
class CKnife :
    public CWeapon
{
private:
    float       m_fCoolTime;
    float       m_Speed; 
    CKnifeAI*   m_pAI;               //휘두를 때 전용 AI

    float       m_fwieldDuration;   //휘두르는 데 걸리는 시간. (갔다가 돌아오는 것까지)
    float       m_fwieldElapsed;    //현재 휘두르는게 얼마만큼의 시간이 걸렸는지.

public:
    int GetDamage();

private:
    virtual void ShotMissile(Vec2 _vDir);
public:
    virtual void update();
    virtual void render(Gdiplus::Graphics* _pDGraphics);
    virtual void render(ID2D1HwndRenderTarget* _pRender);

public:
    CLONE(CKnife)

public:
    CKnife();
    ~CKnife();
};

