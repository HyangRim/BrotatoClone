#pragma once
#include "CObject.h"


struct tMonInfo {
    MON_TYPE    m_eMonType;
    float       m_fSpeed;           //�ӵ�. 
    int         m_iHP;              //ü��
    float       m_fRecogRange;      //���� ����
    float       m_fAttRange;        //���� ����
    int         m_fAtt;             //���ݷ�
};

class AI;
class CMonster :
    public CObject
{
private:
    tMonInfo    m_tInfo;
    AI*         m_pAI;

public:
    float GetSpeed() { return m_tInfo.m_fSpeed; }
    void SetSpeed(float _f) { m_tInfo.m_fSpeed = _f; }
    void SetAI(AI* _AI);
    const tMonInfo& Getinfo() { return m_tInfo; }

    void TakeDamaged(int _iDamage);


private:
    void KnifeDamageUI();


private:
    void SetMonInfo(const tMonInfo& _info) {
        m_tInfo = _info;
    }

public:
    virtual void OnCollisionEnter(CCollider* _pOther);


protected:
    virtual void update();
    CLONE(CMonster)


public:
    CMonster();
    virtual ~CMonster();

    friend class CMonFactory;
};