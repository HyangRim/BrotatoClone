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
    //플레이어 파라미터 관련
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
    //플레이어 파라미터 관련
    playerParameter         m_tPlayerInfo;
    
private:
    vector<CObject*>        m_vecColObj;
    list<CWeapon*>          m_listWeapon;       //플레이어가 가지고 있는 Weapon리스트.
    PLAYER_STATE            m_eCurState;
    PLAYER_STATE            m_ePrevState;
    int                     m_iDir;
    int                     m_iPrevDir;

private:
    //무기 위치 고정된 offset값. 
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
    bool AddWeapon(CWeapon* _pWeapon);  //무기 리스트에 무기 넣는 함수. 
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

    //플레이어 오브젝트는 복사되면 
    CLONE(CPlayer)

public:
    CPlayer();
    ~CPlayer();
};
