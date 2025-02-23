#pragma once
#include "CWeapon.h"

class CKnifeAI;
class CKnife :
    public CWeapon
{
private:
    float       m_fCoolTime;
    float       m_Speed; 
    CKnifeAI*   m_pAI;               //�ֵθ� �� ���� AI

    float       m_fwieldDuration;   //�ֵθ��� �� �ɸ��� �ð�. (���ٰ� ���ƿ��� �ͱ���)
    float       m_fwieldElapsed;    //���� �ֵθ��°� �󸶸�ŭ�� �ð��� �ɷȴ���.

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

