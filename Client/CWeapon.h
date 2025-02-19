#pragma once
#include "CObject.h"


struct tWeaponInfo {
    wstring         m_sName;
    WEAPON_TYPE     m_eType;
    int             m_iDMG;
    float           m_fCritialDMG;
    float           m_fCritialAcc;
    float           m_fCooldown;
    float           m_fRecogRange;
    int             m_iPenet;
};

class CTexture;

class CWeapon :
    public CObject
{
    
private:
    Vec2            m_vRotation;
    tWeaponInfo     m_tWeaponInfo;


public:
    const tWeaponInfo& Getinfo() { return m_tWeaponInfo; }
    void SetInfo(const tWeaponInfo& _info) {
        m_tWeaponInfo = _info;
    }
    
public:
    virtual void update();
    virtual void render(Gdiplus::Graphics* _pDGraphics);
    


public:
    CWeapon();
    virtual ~CWeapon();

    CLONE(CWeapon)
};

