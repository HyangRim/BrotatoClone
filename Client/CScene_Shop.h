#pragma once
#include "CScene.h"
#include "Direct2DMgr.h"

class CPanelUI;

class CScene_Shop :
    public CScene
{
private:
    vector<CPanelUI*>   m_vItemPanels;
    vector<int>         item_numbers;

private:
    ScrollArea m_scrollArea;                //����ü ���Ǵ� Direct2DMgr��
    vector<CObject*> m_scrollContent;       //��ũ�� ������ ������ ��ü��

public:
    void CreateScrollArea();
    void UpdateScrollPosition(float deltaY);
    void RenderScrollArea(ID2D1HwndRenderTarget* _pRender);
    ScrollArea& GetScrollArea() { return m_scrollArea; }

public:
    virtual void update();
    virtual void render(ID2D1HwndRenderTarget* _pRender);

public:
    void ReRollItem();
    void PurchaseItem(DWORD_PTR lParam, DWORD_PTR wParam);

public:
    void CreateInfoPanel();

public:
    virtual void Enter();		//�ش� Scene�� ���� �� ȣ��.
    virtual void Exit();		//�ش� Scene�� Ż�� �� ȣ��.

public:
    CScene_Shop();
    ~CScene_Shop();
};

