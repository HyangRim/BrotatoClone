#pragma once
#include "CScene.h"
class CScene_Test :
    public CScene
{
private:
    float           m_fAcc; //Ÿ�̸�
    int            m_iTest;


public:
    virtual void update();
    virtual void finalupdate();
    virtual void render(HDC _dc);
    virtual void render(Gdiplus::Graphics* _pDGraphics);

public:
    virtual void Enter();		//�ش� Scene�� ���� �� ȣ��.
    virtual void Exit();	//�ش� Scene�� Ż�� �� ȣ��.

public:
    CScene_Test();
    ~CScene_Test();
};

