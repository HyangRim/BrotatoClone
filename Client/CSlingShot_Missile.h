#pragma once
#include "CMissile.h"
class CSlingShot_Missile :
    public CMissile
{
private:
    int         m_iBounce;


public:
    virtual void update();
    virtual void OnCollisionEnter(CCollider* _pOther);


private:
    void ReDirection(CObject* _ReachObj);
public:

    void SetBounce(int _iBounce) { m_iBounce = _iBounce; }
    

private:
    CLONE(CSlingShot_Missile)

public:
    CSlingShot_Missile();
    ~CSlingShot_Missile();
};

