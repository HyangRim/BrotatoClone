#pragma once
#include "CObject.h"

struct tWeaponInfo {
    wstring         m_sName;            //무기 이름
    wstring         m_sIconImageKey;    //무기 이미지 Key값. 
    WEAPON_TYPE     m_eType;            //무기 타입

    int             m_iDMG;             //무기 기본 데미지
    float           m_fMeleeCoef;       //근거리 데미지 계수
    float           m_fRangeCoef;       //원거리 데미지 계수
    float           m_fCritialDMG;      //크리티컬 데미지 계수
    float           m_fCritialAcc;      //크리티컬 데미지 확률
    float           m_fCooldown;        //무기 공격 쿨다운
    float           m_fRecogRange;      //무기 적 인식범위
    int             m_iPenet;           //무기 튕김 수치. 
};

class CTexture;
class CPlayer;
class CScene;

class CWeapon :
    public CObject
{
    
private:
    Vec2            m_vWeaponOffset;        //무기 위치 오프셋(플레이어 주변 부유 Offset)
    Vec2            m_vRotation;
    tWeaponInfo     m_tWeaponInfo;
    CTexture*       m_pTexture;
    CObject*        m_pTarget;
    CScene*         m_pCurScene;


protected:

    CPlayer*        m_pPlayer;


public:
    const tWeaponInfo& Getinfo() { return m_tWeaponInfo; }
    void SetInfo(const tWeaponInfo& _info) {
        m_tWeaponInfo = _info;
    }

    virtual void ShotMissile(Vec2 _vDir);

    CObject* GetTarget() { return m_pTarget; }
    void SetPlayer() { m_pPlayer = (CPlayer*)(CSceneMgr::GetInstance()->GetPlayer()); }
    void SetWeaponOffset(Vec2 _vWeaponOffset) { m_vWeaponOffset = _vWeaponOffset; }

    void SetCurScene(CScene* _pScene) { m_pCurScene = _pScene; }

private:
    CObject* SpecTarget();
    
    
public:
    virtual void update();
    virtual void render(Gdiplus::Graphics* _pDGraphics);
    virtual void render(ID2D1HwndRenderTarget* _pRender);
    
public:
    CWeapon();
    virtual ~CWeapon();

    CLONE(CWeapon)

    friend class CImage;
};

