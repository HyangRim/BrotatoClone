#pragma once
#include "CScene.h"
#include "CharacterInfoMgr.h"

class CBtnUI;

class CScene_Select_Character :
    public CScene
{
private:
    vector<CBtnUI*> m_vCharaters;
public:
    virtual void update();
    virtual void render(ID2D1HwndRenderTarget* _pRender);

private:
    void CharacterMapping(CBtnUI* _btn, int i, int j, wstring& tag);

public:
    virtual void Enter();		//�ش� Scene�� ���� �� ȣ��.
    virtual void Exit();		//�ش� Scene�� Ż�� �� ȣ��.

public:
    void CreatePlayerObject();
    void CreateBackGround(Direct2DMgr* _pD2DMgr, Vec2 _vResolution);
    void CreateBackBtn();
    void CreateItemSelectText(Vec2 _vResolution);
    void CreateCharacterItemBtn();

public:
    CScene_Select_Character();
    ~CScene_Select_Character();
};

