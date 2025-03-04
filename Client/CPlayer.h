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
    //�÷��̾� �Ķ���� ����
    int                     m_iLevel;
    int                     m_iMaxHP;
    int                     m_iCurHP;
    int                     m_iMaxEXP;
    int                     m_iCurEXP;
    int                     m_iCoin;

    //ĳ���Ϳ� ���� �߰��� �� �ִ� �Ķ����

    tCharacterInfo*         m_stCharacterInfo;
    
    float			m_fDefaultSpeed;	// �⺻ ���ǵ�.
    int				m_AddMaxHP;			// + �ִ� ü��
    float			m_fDamageCoef;		//������ %
    float			m_fMeleeCoef;		//�ٰŸ� ������ %
    float			m_fRangeCoef;		//���Ÿ� ������ %
    float			m_fAttackSpeedCoef; // ���ݼӵ� %
    float			m_iCriticalAcc;		// ũȮ %
    float			m_fSpeed;			// �ӵ� ���.
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


    //Duration���� 
private:
    float m_fStepSoundDelay;

    float m_fUnderAttackSoundDelay;

private:
    //���� ��ġ ������ offset��. 
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
    bool AddWeapon(CWeapon* _pWeapon);  //���� ����Ʈ�� ���� �ִ� �Լ�. 
    bool DeleteWeapon(CWeapon* _pWeapon);
    size_t  GetWeaponCount() { return m_listWeapon.size(); }

    const playerParameter& GetPlayerInfo() { return m_tPlayerInfo; }


    //���� ����Ʈ �����ִ� �Լ�. 
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

    //�÷��̾� ������Ʈ�� ����Ǹ� 
    CLONE(CPlayer)

public:
    CPlayer();
    ~CPlayer();
};
