#pragma once
#include "CUI.h"


class CPanelUI;
//���⼭ ������ �˷����� ������ �Լ� ������(��Ȯ�� Ŭ������ ����)�� 
//�𸣱⿡ �̻��� ������ ������ �� �־� ������ ����. 

//����Լ� ������ ������ ������ ��Ȯ�� Ŭ���� ������ �˾ƾ� ��. 
#include "CScene.h"
#include "CObject.h"
#include "CScene_Shop.h"
typedef void(*SLIDER_FUNC) (DWORD_PTR, DWORD_PTR);

//�� ���� �����ϰ� �ִ� Ư�� ��� �Լ��� �޾ƿ;߸� ��. 

//��ư UI�� ���������� �ٶ�µ�, �̷� ������ ����� ���� �� �³�?
//� ������Ʈ�� ����� ���� ������? ���������� �� ��� ������?

//�̷��� �ϸ� SCENE�̳�, ������Ʈ���� �Ļ��� ���� �� �� ����. 
typedef void(CScene::* SCENE_MEMFUNC)(void);
typedef void(CObject::* OBJECT_MEMFUNC)(void);

class CSliderUI :
    public CUI
{

private:

    D2D1::ColorF    m_colorMouseOn;         //0~1 ����ȭ ���Ѿߵ�
    D2D1::ColorF    m_colorNormal;

    float           m_fRatio;               //0~1 ���̷� �� �����̴� ��. 

    //�� ��� �Լ��� ��� ���� ��ü��, ȣ�� �� �Լ�. 
    SCENE_MEMFUNC   m_pSceneFunc;
    CScene* m_pSceneInst;

public:
    void SetColor(D2D1::ColorF _mouseOn, D2D1::ColorF _normal)
    {
        m_colorMouseOn = _mouseOn;
        m_colorNormal = _normal;
    }

public:
    virtual void MouseOn();
    virtual void MouseLbtnDown();
    virtual void MouseLbtnUp();
    virtual void MouseLbtnClicked();

    void SetSliderValue(float _fValue);
    float GetSliderValue() { return m_fRatio; }

    void SetClickedCallBack(CScene* _pScene, SCENE_MEMFUNC _pSceneFunc);


public:
    virtual void update();
    virtual void render(HDC _dc);
    virtual void render(Gdiplus::Graphics* _pDGraphics);
    virtual void render(ID2D1HwndRenderTarget* _pRender);

    CLONE(CSliderUI)

public:
    CSliderUI();
    virtual ~CSliderUI();
};

