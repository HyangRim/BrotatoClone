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
    virtual void Enter();		//해당 Scene에 진입 시 호출.
    virtual void Exit();		//해당 Scene에 탈출 시 호출.

public:
    CScene_Select_Character();
    ~CScene_Select_Character();
};

