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

    //�̰� ���� ȭ�� Wave ����.
    float     m_fPanelMoveDuration;
    float     m_fPanelMoveElapsed;
    float     m_fPanelMoveWave;

    //�̰� ���� ȭ�� �������� Wave�뵵
    float     m_fBrotatoDuration;
    float     m_fBrotatoElased;
    float     m_fBrotatoWave;

public:
    void ClearPlayer();

public:
    virtual void update();
    virtual void finalupdate();
    //�θ�κ��� ��ӹ��� �����Լ�����, �Ϲ� �Լ����� ������ ���� �ȵǼ� virtual��. 
    //virtual�� ����� �����Լ��̱� ��... �׷��� ���������. 

    virtual void render(HDC _dc);
    virtual void render(Gdiplus::Graphics* _pDGraphics);
    virtual void render(ID2D1HwndRenderTarget* _pRender);

    virtual void Enter();
    virtual void Exit();


public:
    void StartBtn();
public:
    CScene_Main();
    ~CScene_Main();
};

