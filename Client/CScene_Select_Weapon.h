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
    virtual void Enter();		//�ش� Scene�� ���� �� ȣ��.
    virtual void Exit();		//�ش� Scene�� Ż�� �� ȣ��.
    
public:
    CScene_Select_Weapon();
    ~CScene_Select_Weapon();
};
