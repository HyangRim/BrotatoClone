#pragma once
#include "CObject.h"

struct tWeaponInfo {
    wstring         m_sName;            //���� �̸�
    WEAPON_TYPE     m_eType;            //���� Ÿ��
    int             m_iDMG;             //���� �⺻ ������

    float           m_iMeleeCoef;       //�ٰŸ� ������ ���
    float           m_iRangeCoef;       //���Ÿ� ������ ���
    float           m_fCritialDMG;      //ũ��Ƽ�� ������ ���
    float           m_fCritialAcc;      //ũ��Ƽ�� ������ Ȯ��
    float           m_fCooldown;        //���� ���� ��ٿ�
    float           m_fRecogRange;      //���� �� �νĹ���
    int             m_iPenet;           //���� ƨ�� ��ġ. 
};

class CTexture;
class CPlayer;
class CScene;

class CWeapon :
    public CObject
{
    
private:
    Vec2            m_vWeaponOffset;        //���� ��ġ ������(�÷��̾� �ֺ� ���� Offset)
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

    void SetWeaponOffset(Vec2 _vWeaponOffset) { m_vWeaponOffset = _vWeaponOffset; }

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
};

