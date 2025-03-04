#pragma once
#include "CObject.h"
class CMissile :
    public CObject
{

private:
    float    m_fTheta;
    Vec2     m_vDir;//미사일 방향
    int      m_iDamage;
    float    m_fLifeTime;
    float    m_fLifeElapsedTime;
    bool     m_bIsCritial;


public:
    virtual void update();
    virtual void render(HDC _dc);
    virtual void render(Gdiplus::Graphics* _pDGraphics);
    virtual void render(ID2D1HwndRenderTarget* _pRender);

    void SetDir(float _fTheta) { m_fTheta = _fTheta; }
    void SetDir(Vec2 _vDir) 
    { 
        m_vDir = _vDir; 
        m_vDir.Normalize();
    }
    void SetDamage(int _iDamage) {
        m_iDamage = _iDamage;
    }

    void SetCritical(bool _bCritical) {
        m_bIsCritial = _bCritical;
    }

    int GetDamage() const { return m_iDamage; }
    bool GetCritical() const { return m_bIsCritial; }
public:
    virtual void OnCollision(CCollider* _pOther) {};
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther) {};


private:
    CLONE(CMissile)

public:
    CMissile();
    virtual ~CMissile();
};