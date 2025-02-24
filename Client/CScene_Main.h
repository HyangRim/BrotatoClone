#pragma once
#include "CScene.h"

struct mainPanel {
    CObject* pTitleGround;
    CObject* pTitleBack;
    CObject* pTitleMid;
    CObject* pTitlefront;
    CObject* pTitleBrotato;
    CObject* pTitlelights;
    CObject* pTitlelogo;
};
class CScene_Main :
    public CScene
{


private:
    mainPanel m_tMainPanel;

    float     m_fPanelMoveDuration;
    float     m_fPanelMoveElapsed;
    float     m_fPanelMoveWave;
public:
    virtual void update();
    virtual void finalupdate();
    //부모로부터 상속받은 가상함수인지, 일반 함수인지 보통은 구별 안되서 virtual씀. 
    //virtual을 안적어도 가상함수이긴 함... 그래도 명시적으로. 

    virtual void render(HDC _dc);
    virtual void render(Gdiplus::Graphics* _pDGraphics);
    virtual void render(ID2D1HwndRenderTarget* _pRender);

    virtual void Enter();
    virtual void Exit();


public:

public:
    CScene_Main();
    ~CScene_Main();
};

