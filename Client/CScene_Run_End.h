#pragma once
#include "CScene.h"
#include "Direct2DMgr.h"

class CPanelUI;

class CScene_Run_End :
    public CScene
{
private:
    ScrollArea          m_scrollArea;                //����ü ���Ǵ� Direct2DMgr��
    vector<CObject*>    m_scrollContent;       //��ũ�� ������ ������ ��ü��

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
    virtual void Enter();		//�ش� Scene�� ���� �� ȣ��.
    virtual void Exit();		//�ش� Scene�� Ż�� �� ȣ��.

public:  
    void CreateMainPanel();
    void CreateInfoPanel(CPanelUI *_backPanel);
    void CreateItemInfoPanel(CPanelUI* _backPanel);
    void CreateWeaponInfoPanel(CPanelUI* _backPanel);


public:
    CScene_Run_End();
    ~CScene_Run_End();
};

