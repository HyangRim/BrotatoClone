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
    virtual void Enter();		//�ش� Scene�� ���� �� ȣ��.
    virtual void Exit();		//�ش� Scene�� Ż�� �� ȣ��.

public:
    CScene_Shop();
    ~CScene_Shop();
};

