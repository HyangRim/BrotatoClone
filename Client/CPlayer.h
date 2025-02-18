#pragma once
#include "CObject.h"


enum class PLAYER_STATE {
    IDLE,
    WALK,
    ATTACK,
    JUMP,
    DEAD,
};

class CTexture;
class CPlayer :
    public CObject
{

private:
    vector<CObject*>        m_vecColObj;
    PLAYER_STATE            m_eCurState;
    PLAYER_STATE            m_ePrevState;
    int                     m_iDir;
    int                     m_iPrevDir;

public:
    virtual void update();
    virtual void render(HDC _dc);

private:
    void CreateMissile();
    void update_move();
    void update_state();
    void update_animation();


    virtual void OnCollisionEnter(CCollider* _pOther);

    //플레이어 오브젝트는 복사되면 
    CLONE(CPlayer)

public:
    CPlayer();
    ~CPlayer();
};
