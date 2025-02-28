#pragma once
#include "CScene.h"
class CScene_Run_End :
    public CScene
{

public:
    virtual void update();
    //virtual void finalupdate();
    virtual void render(ID2D1HwndRenderTarget* _pRender);

public:
    virtual void Enter();		//해당 Scene에 진입 시 호출.
    virtual void Exit();		//해당 Scene에 탈출 시 호출.

public:
    CScene_Run_End();
    ~CScene_Run_End();
};

