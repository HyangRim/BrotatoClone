#pragma once
#include "CObject.h"
#include "CharacterInfoMgr.h"

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

    //캐릭터에 의해 추가될 수 있는 파라미터

    tCharacterInfo*         m_stCharacterInfo;
    
    float			m_fDefaultSpeed;	// 기본 스피드.
    int				m_AddMaxHP;			// + 최대 체력
    float			m_fDamageCoef;		//최종뎀 %
    float			m_fMeleeCoef;		//근거리 최종뎀 %
    float			m_fRangeCoef;		//원거리 최종뎀 %
    float			m_fAttackSpeedCoef; // 공격속도 %
    float			m_iCriticalAcc;		// 크확 %
    float			m_fSpeed;			// 속도 계수.
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


    //Duration관련 
private:
    float m_fStepSoundDelay;

    float m_fUnderAttackSoundDelay;

private:
    //무기 위치 고정된 offset값. 
    const Vec2 weaponOffsetPos[6] = {
        Vec2(-40, -30), Vec2(40, -30),
        Vec2(-40, 0), Vec2(40, 0),
        Vec2(-40, 30), Vec2(40, 30),
    };

public:
    void SetCharacterParam(tCharacterInfo* _characterInfo) { m_tPlayerInfo.m_stCharacterInfo = _characterInfo; }
    playerParameter& GetCharacterParam() { return m_tPlayerInfo; }

public:
    virtual void update();
    virtual void render(HDC _dc);
    virtual void render(Gdiplus::Graphics* _pDGraphics);
    virtual void render(ID2D1HwndRenderTarget* _pRender);

public:
    bool AddWeapon(CWeapon* _pWeapon);  //무기 리스트에 무기 넣는 함수. 
    bool DeleteWeapon(CWeapon* _pWeapon);
    size_t  GetWeaponCount() { return m_listWeapon.size(); }

    const playerParameter& GetPlayerInfo() { return m_tPlayerInfo; }


    //웨폰 리스트 전해주는 함수. 
    const list<CWeapon*>& GetPlayerWeapons() { return m_listWeapon; }

    void AddExp(int _iExp);
    void AddCoin(int _iCoin);

private:
    void PlayerLevelUp();
    void CreateMissile();
    void update_move();
    void update_state();
    void update_animation();

    void PlayWalkSound();

    virtual void OnCollisionEnter(CCollider* _pOther);

    //플레이어 오브젝트는 복사되면 
    CLONE(CPlayer)

public:
    CPlayer();
    ~CPlayer();
};
