#pragma once
#include "CMonster.h"


class AI;
class CTexture;
class CDropItem :
    public CMonster
{

private:
    int m_iExp;
    int m_iCoin;


public:
    virtual void update();

private:
    virtual void OnCollisionEnter(CCollider* _pOther);

public:
    int GetEXP() { return m_iExp; }
    int GetCoin() { return m_iCoin; }

    CLONE(CDropItem);

public:
    CDropItem();
    ~CDropItem();
};

