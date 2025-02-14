#pragma once
#include "CObject.h"
class CMissile :
    public CObject
{

private:
    float    m_fTheta;
    Vec2     m_vDir;//미사일 방향

public:
    virtual void update();
    virtual void render(HDC _dc);

    void SetDir(float _fTheta) { m_fTheta = _fTheta; }
    void SetDir(Vec2 _vDir) 
    { 
        m_vDir = _vDir; 
        m_vDir.Normalize();
    }

public:
    virtual void OnCollision(CCollider* _pOther) {};
    virtual void OnCollisionEnter(CCollider* _pOther);
    virtual void OnCollisionExit(CCollider* _pOther) {};


private:
    CLONE(CMissile)

public:
    CMissile();
    ~CMissile();
};