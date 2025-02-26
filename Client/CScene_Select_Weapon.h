#pragma once
#include "CScene.h"

class CScene_Select_Weapon :
    public CScene
{
private:

public:
    virtual void update();
    virtual void render(ID2D1HwndRenderTarget* _pRender);

public:
    virtual void Enter();		//해당 Scene에 진입 시 호출.
    virtual void Exit();		//해당 Scene에 탈출 시 호출.
    
public:
    CScene_Select_Weapon();
    ~CScene_Select_Weapon();
};
