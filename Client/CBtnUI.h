#pragma once
#include "CUI.h"

//여기서 원형을 알려주지 않으면 함수 포인터(정확한 클래스의 구조)를 
//모르기에 이상한 지점에 접근할 수 있어 에러가 생김. 

//멤버함수 포인터 접근할 때에는 정확한 클래스 정보를 알아야 함. 
#include "CScene.h"
#include "CObject.h"
typedef void(*BTN_FUNC) (DWORD_PTR, DWORD_PTR);



//툴 씬이 보유하고 있는 특정 멤버 함수를 받아와야만 함. 


//버튼 UI는 범용적으로 바라는데, 이런 식으로 멤버를 쓰는 게 맞나?
//어떤 오브젝트의 기능을 쓰고 싶으면? 범용적으로 쓸 방법 없을까?

//이렇게 하면 SCENE이나, 오브젝트에서 파생된 것을 쓸 수 있음. 
typedef void(CScene:: *SCENE_MEMFUNC)(void);
typedef void(CObject::*OBJECT_MEMFUNC)(void);

class CBtnUI :
    public CUI
{

private:
    //함수 포인터를 불러와서 버튼이 눌렸을 때 그걸 씀.
    BTN_FUNC    m_pFunc;
    DWORD_PTR   m_param1;
    DWORD_PTR   m_param2;

    //씬 멥버 함수를 얻기 위한 객체와, 호출 할 함수. 
    SCENE_MEMFUNC   m_pSceneFunc;
    CScene*         m_pSceneInst;

public:
    virtual void MouseOn();
    virtual void MouseLbtnDown();
    virtual void MouseLbtnUp();
    virtual void MouseLbtnClicked();

    void SetClickedCallBack(BTN_FUNC _pFunc, DWORD_PTR _param1, DWORD_PTR _param2) {
        m_pFunc = _pFunc;
        m_param1 = _param1;
        m_param2 = _param2;
    }

    void SetClickedCallBack(CScene* _pScene, SCENE_MEMFUNC _pSceneFunc);

    CLONE(CBtnUI)

public:
    CBtnUI();
    ~CBtnUI();
};

