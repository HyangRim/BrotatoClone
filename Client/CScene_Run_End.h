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
    virtual void Enter();		//�ش� Scene�� ���� �� ȣ��.
    virtual void Exit();		//�ش� Scene�� Ż�� �� ȣ��.

public:
    CScene_Run_End();
    ~CScene_Run_End();
};

