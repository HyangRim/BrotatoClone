#pragma once
#include "CScene.h"
class CScene_Test :
    public CScene
{
private:

public:
    virtual void update();
    virtual void finalupdate();
    virtual void render(HDC _dc);
    virtual void render(Gdiplus::Graphics* _pDGraphics);

public:
    virtual void Enter();		//해당 Scene에 진입 시 호출.
    virtual void Exit();	//해당 Scene에 탈출 시 호출.

public:
    CScene_Test();
    ~CScene_Test();
};

