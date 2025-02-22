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

struct playerParameter {
    //�÷��̾� �Ķ���� ����
    int                     m_iLevel;
    int                     m_iMaxHP;
    int                     m_iCurHP;
    int                     m_iMaxEXP;
    int                     m_iCurEXP;
    int                     m_iCoin;
};
class CPlayer :
    public CObject
{


private:
    //�÷��̾� �Ķ���� ����
    playerParameter         m_tPlayerInfo;
    
private:
    vector<CObject*>        m_vecColObj;
    list<CWeapon*>          m_listWeapon;       //�÷��̾ ������ �ִ� Weapon����Ʈ.
    PLAYER_STATE            m_eCurState;
    PLAYER_STATE            m_ePrevState;
    int                     m_iDir;
    int                     m_iPrevDir;

private:
    //���� ��ġ ������ offset��. 
    const Vec2 weaponOffsetPos[6] = {
        Vec2(-40, -30), Vec2(40, -30),
        Vec2(-40, 0), Vec2(40, 0),
        Vec2(-40, 30), Vec2(40, 30),
    };

public:
    virtual void update();
    virtual void render(HDC _dc);
    virtual void render(Gdiplus::Graphics* _pDGraphics);

public:
    bool AddWeapon(CWeapon* _pWeapon);  //���� ����Ʈ�� ���� �ִ� �Լ�. 
    bool DeleteWeapon(CWeapon* _pWeapon);

    const playerParameter& GetPlayerInfo() { return m_tPlayerInfo; }

    void AddExp(int _iExp);
    void AddCoin(int _iCoin);



private:
    void PlayerLevelUp();
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
