#pragma once
#include "CScene.h"
#include "CharacterInfoMgr.h"

class CBtnUI;
class Direct2DMgr;

class CScene_Select_Weapon :
    public CScene
{
private:
    tCharacterInfo* m_stSelectedInfo;

public:
    virtual void update();
    virtual void render(ID2D1HwndRenderTarget* _pRender);

public:
    void SetSelectedCharacterInfo(tCharacterInfo* _CharacterInfo) { m_stSelectedInfo = _CharacterInfo; }

    void WeaponMapping(CBtnUI* _btn, int i, int j, wstring& tag);

public:
    virtual void Enter();		//해당 Scene에 진입 시 호출.
    virtual void Exit();		//해당 Scene에 탈출 시 호출.
    
public:
    void BackBtnCallBack();

public:
    void CreateBackGround(Direct2DMgr* _pD2DMgr, Vec2 _vResolution);
    void CreateBackBtn();
    void CreateWeaponSelectText(Vec2 _vResolution);
    void CreateSelectedCharacterInfoPanel(Direct2DMgr* _pD2DMgr, Vec2 _vResolution);
    void CreateWeaponItemBtn();

public:
    CScene_Select_Weapon();
    ~CScene_Select_Weapon();

    friend class CScene_Select_Character;
};
