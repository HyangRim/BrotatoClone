#pragma once
#include "CScene.h"
#include "Direct2DMgr.h"

class CPanelUI;
class CWeapon;

class CScene_Shop :
    public CScene
{
private:
    vector<CPanelUI*>   m_vItemPanels;
    vector<int>         item_numbers;

private:
    CPanelUI*           m_statPanel;

private:
    ScrollArea          m_scrollArea;                //����ü ���Ǵ� Direct2DMgr��
    vector<CObject*>    m_scrollContent;       //��ũ�� ������ ������ ��ü��

private:
    vector<CWeapon*>    m_vWeaponlist;

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
    void CreateInfoPanel(Vec2 _panelPos);

public:
    virtual void Enter();		//�ش� Scene�� ���� �� ȣ��.
    virtual void Exit();		//�ش� Scene�� Ż�� �� ȣ��.


public:
    void CreateBackGround(Direct2DMgr* _pD2DMgr, Vec2 _vResolution);
    void CreateShopWaveTextUI(wchar_t* buffer, size_t bufferSize);
    void CreateItemTextUI(wchar_t* buffer, size_t bufferSize);
    void CreateWeaponTextUI(wchar_t* buffer, size_t bufferSize);
    void CreateResetBtn(Direct2DMgr* _pD2DMgr);
    void CreateRemainingCoinText(wchar_t* buffer, size_t bufferSize);
    void CreateRemainingCoinImage(Direct2DMgr* _pD2DMgr);
    void CreateNextWaveBtn(wchar_t* buffer, size_t bufferSize);
    void CreateShowItemPanels(Direct2DMgr* _pD2DMgr, wchar_t* buffer, size_t bufferSize);
    void CreatePossessedWeapons(Direct2DMgr* _pD2DMgr);



public:
    CScene_Shop();
    ~CScene_Shop();
};

