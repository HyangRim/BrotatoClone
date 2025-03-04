#pragma once
#include "CScene.h"

class CPanelUI;
class CScene_Start :
    public CScene
{

private:
    ID2D1Bitmap* g_pBitmap; // ���� �Ǵ� ��� ������ ����
    Vec2    m_vForcePos;
    float   m_fForceRadius;
    float   m_fCurRadius;
    float   m_fForce;
    bool    m_bUseForce;
    CPanelUI* m_pPausePanel;
    vector<CObject*> m_vecPauseObj;


    CPanelUI* m_pFailPanel;
    vector<CObject*> m_vecFailObj;
    

private:
    float       m_fFailDuration; 
    bool        m_bFailed;

public:
    virtual void update();
    //�θ�κ��� ��ӹ��� �����Լ�����, �Ϲ� �Լ����� ������ ���� �ȵǼ� virtual��. 
    //virtual�� ����� �����Լ��̱� ��... �׷��� ���������. 

    virtual void render(HDC _dc);
    virtual void render(Gdiplus::Graphics* _pDGraphics);
    virtual void render(ID2D1HwndRenderTarget* _pRender);

    virtual void Enter();
    virtual void Exit();


public:
    void CreateForce();
    void SceneFailed();

private:
    void CreatePause();
    void CreateLeftBtns();
    void CreateMiddleInfo();
    void CreateInfoPanel();

    void OnPause();
    void OffPause();


public:
    CScene_Start();
    ~CScene_Start();
};

