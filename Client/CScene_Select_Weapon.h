#pragma once
#include "CScene.h"
#include "CharacterInfoMgr.h"

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
    virtual void Enter();		//�ش� Scene�� ���� �� ȣ��.
    virtual void Exit();		//�ش� Scene�� Ż�� �� ȣ��.
    
public:
    CScene_Select_Weapon();
    ~CScene_Select_Weapon();

    friend class CScene_Select_Character;
};
