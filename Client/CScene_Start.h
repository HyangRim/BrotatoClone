#pragma once
#include "CScene.h"
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


    //�ӽ� ����.
    float   m_ftempDuration;
    float   m_ftempElapsed;

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

public:
    CScene_Start();
    ~CScene_Start();
};

