#pragma once
#include "CScene.h"
class CScene_Test :
    public CScene
{
private:
    float           m_fAcc; //타이머
    int            m_iTest;


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

