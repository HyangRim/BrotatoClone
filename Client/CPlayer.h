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
class CWeapon;
class CPlayer :
    public CObject
{
private:
    vector<CObject*>        m_vecColObj;
    vector<CWeapon*>        m_vecWeapon;        //�÷��̾ ������ �ִ� Weapon����. 
    PLAYER_STATE            m_eCurState;
    PLAYER_STATE            m_ePrevState;
    int                     m_iDir;
    int                     m_iPrevDir;

public:
    virtual void update();
    virtual void render(HDC _dc);
    virtual void render(Gdiplus::Graphics* _pDGraphics);

public:
    void AddWeapon(CWeapon* _pWeapon);

private:
    void CreateMissile();
    void update_move();
    void update_state();
    void update_animation();


    virtual void OnCollisionEnter(CCollider* _pOther);

    //�÷��̾� ������Ʈ�� ����Ǹ� 
    CLONE(CPlayer)

public:
    CPlayer();
    ~CPlayer();
};
