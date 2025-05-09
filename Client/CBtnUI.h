#pragma once
#include "CUI.h"

class CPanelUI;

//여기서 원형을 알려주지 않으면 함수 포인터(정확한 클래스의 구조)를 
//모르기에 이상한 지점에 접근할 수 있어 에러가 생김. 

//멤버함수 포인터 접근할 때에는 정확한 클래스 정보를 알아야 함. 
#include "CScene.h"
#include "CObject.h"
#include "CScene_Shop.h"
typedef void(*BTN_FUNC) (DWORD_PTR, DWORD_PTR);

//툴 씬이 보유하고 있는 특정 멤버 함수를 받아와야만 함. 

//버튼 UI는 범용적으로 바라는데, 이런 식으로 멤버를 쓰는 게 맞나?
//어떤 오브젝트의 기능을 쓰고 싶으면? 범용적으로 쓸 방법 없을까?

//이렇게 하면 SCENE이나, 오브젝트에서 파생된 것을 쓸 수 있음. 
typedef void(CScene:: *SCENE_MEMFUNC)(void);
typedef void(CObject::*OBJECT_MEMFUNC)(void);
typedef void(CScene:: *SCENE_MEMFUNC2)(DWORD_PTR, DWORD_PTR);

class CBtnUI :
    public CUI
{

private:
    //함수 포인터를 불러와서 버튼이 눌렸을 때 그걸 씀.
    BTN_FUNC        m_pFunc;
    DWORD_PTR       m_param1;
    DWORD_PTR       m_param2;

    //마우스 on일때 쓸 함수
    BTN_FUNC        m_pFuncMouseOn;
    DWORD_PTR       m_param1MouseOn;
    DWORD_PTR       m_param2MouseOn;


    //꼭짓점을 둥글게 할건지
    bool            m_bIsRoundedRect;
    float           m_fradiusX;
    float           m_fradiusY;

    //MouseOn일때와 평상시일때 색
    D2D1::ColorF    m_colorMouseOn;         //0~1 정규화 시켜야됨
    D2D1::ColorF    m_colorNormal;

    bool                m_bPanelCreated;
    bool                m_bMouseOnCalled;   // MouseOn 콜백이 호출되었는지 여부
    vector<CObject*>    m_vTempObjects;

    //씬 멥버 함수를 얻기 위한 객체와, 호출 할 함수. 
    SCENE_MEMFUNC       m_pSceneFunc;
    CScene*             m_pSceneInst;
    //씬 멥버 함수를 얻기 위한 객체와, 호출 할 함수. 
    SCENE_MEMFUNC2      m_pSceneFunc2;
    CScene*             m_pSceneInst2;
    DWORD_PTR           m_lParam;
    DWORD_PTR           m_wParam;

    //콜백 함수
    std::function<void()> m_callback;

    //이전 프레임에 포커스 되었는가?
    bool            m_bPrevFocus;

public:
    void SetIsRound(bool _bIsRoundedRect, float _fradiusX, float _fradiusY) 
    { 
        m_bIsRoundedRect = _bIsRoundedRect; 
        m_fradiusX = _fradiusX;
        m_fradiusY = _fradiusY;
    }

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


    void SetOnCallBack(BTN_FUNC _pFunc, DWORD_PTR _param1, DWORD_PTR _param2)
    {
        m_pFuncMouseOn = _pFunc;
        m_param1MouseOn = _param1;
        m_param2MouseOn = _param2;
    }

    void SetClickedCallBack(BTN_FUNC _pFunc, DWORD_PTR _param1, DWORD_PTR _param2) {
        m_pFunc = _pFunc;
        m_param1 = _param1;
        m_param2 = _param2;
    }

    void SetClickedCallBack(CScene* _pScene, SCENE_MEMFUNC2 _pSceneFunc, DWORD_PTR _lParam, DWORD_PTR _wParam)
    {
        m_pSceneFunc2 = _pSceneFunc;
        m_pSceneInst2 = _pScene;
        m_lParam = _lParam;
        m_wParam = _wParam;
    }

    void SetClickedCallBack(CScene* _pScene, SCENE_MEMFUNC _pSceneFunc);
    void SetClickedCallBack(const std::function<void()>& callback);
    
public:
    vector<CObject*>& GetTrashCan() { return m_vTempObjects; }

public:
    virtual void update();
    virtual void render(HDC _dc);
    virtual void render(Gdiplus::Graphics* _pDGraphics);
    virtual void render(ID2D1HwndRenderTarget* _pRender);

    CLONE(CBtnUI)

public:
    CBtnUI();
    ~CBtnUI();
};

