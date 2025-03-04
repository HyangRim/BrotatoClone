#pragma once
#include "CScene.h"
class CScene_Shop :
    public CScene
{
private:

public:
    virtual void update();
    virtual void render(ID2D1HwndRenderTarget* _pRender);

public:
    void CreateInfoPanel();

public:
    virtual void Enter();		//해당 Scene에 진입 시 호출.
    virtual void Exit();		//해당 Scene에 탈출 시 호출.

public:
    CScene_Shop();
    ~CScene_Shop();
};

