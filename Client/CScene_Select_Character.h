#pragma once
#include "CScene.h"
class CScene_Select_Character :
    public CScene
{

public:
    virtual void update();
    virtual void render(ID2D1HwndRenderTarget* _pRender);

public:
    virtual void Enter();		//�ش� Scene�� ���� �� ȣ��.
    virtual void Exit();		//�ش� Scene�� Ż�� �� ȣ��.

public:
    CScene_Select_Character();
    ~CScene_Select_Character();
};

