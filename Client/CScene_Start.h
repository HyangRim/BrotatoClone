#pragma once
#include "CScene.h"

class CPanelUI;
class CScene_Start :
    public CScene
{

private:
    ID2D1Bitmap* g_pBitmap; // 전역 또는 멤버 변수로 선언
    Vec2    m_vForcePos;
    float   m_fForceRadius;
    float   m_fCurRadius;
    float   m_fForce;
    bool    m_bUseForce;
    CPanelUI* m_pPausePanel;
    vector<CObject*> m_vecPauseObj;


    CPanelUI* m_pFailPanel;
    vector<CObject*> m_vecFailObj;
    vector<CObject*> m_vecUpgradeObj;

    //옵션 Obj 모아놓는 벡터. 
    vector<CObject*>    m_vecOptionObjs;
    
private:
    ScrollArea          m_scrollArea;                //구조체 정의는 Direct2DMgr에
    vector<CObject*>    m_scrollContent;       //스크롤 영역에 정의할 객체들

private:
    float       m_fFailDuration; 
    bool        m_bFailed;

public:
    virtual void update();
    //부모로부터 상속받은 가상함수인지, 일반 함수인지 보통은 구별 안되서 virtual씀. 
    //virtual을 안적어도 가상함수이긴 함... 그래도 명시적으로. 

    virtual void render(HDC _dc);
    virtual void render(Gdiplus::Graphics* _pDGraphics);
    virtual void render(ID2D1HwndRenderTarget* _pRender);

    virtual void Enter();
    virtual void Exit();


public:
    void CreateScrollArea();
    void UpdateScrollPosition(float deltaY);
    void RenderScrollArea(ID2D1HwndRenderTarget* _pRender);
    ScrollArea& GetScrollArea() { return m_scrollArea; }
public:
    void CreateForce();
    void SceneFailed();
    void callPlayerUpgrade(int upgradeIdx);
    void CreateLevelUpShop();

private:
    void CreatePause();
    void CreateLeftBtns();
    void CreateMiddleInfo();
    void CreateInfoPanel();

    void OnPause();
    void OffPause();

private:
    void CreateOptionPanel();
    void OffOptionPanel();
    void OptionPanelupdate();

    void ChangeMasterRatio();
    void ChangeBGMRatio();
    void ChangeSFXRatio();



public:
    CScene_Start();
    ~CScene_Start();
};

