#pragma once
#include "CScene.h"
#include "Direct2DMgr.h"

class CPanelUI;

class CScene_Run_End :
    public CScene
{
private:
    ScrollArea          m_scrollArea;                //구조체 정의는 Direct2DMgr에
    vector<CObject*>    m_scrollContent;       //스크롤 영역에 정의할 객체들

public:
    virtual void update();
    //virtual void finalupdate();
    virtual void render(ID2D1HwndRenderTarget* _pRender);

public:
    void CreateScrollArea();
    void UpdateScrollPosition(float deltaY);
    void RenderScrollArea(ID2D1HwndRenderTarget* _pRender);
    ScrollArea& GetScrollArea() { return m_scrollArea; }

public:
    virtual void Enter();		//해당 Scene에 진입 시 호출.
    virtual void Exit();		//해당 Scene에 탈출 시 호출.

public:  
    void CreateMainPanel();
    void CreateInfoPanel(CPanelUI *_backPanel);
    void CreateItemInfoPanel(CPanelUI* _backPanel);
    void CreateWeaponInfoPanel(CPanelUI* _backPanel);


public:
    CScene_Run_End();
    ~CScene_Run_End();
};

