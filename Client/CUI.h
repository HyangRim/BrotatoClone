#pragma once
#include "CObject.h"

//UI는 계층화 되어 있음 부모 UI, 자식 UI
//씬에는 그 계층화된 부모 UI만 넣어줌. 그러면 실제로는 연계된 UI가 들어감. 


class CUI :
    public CObject
{

private:
    vector<CUI*>        m_vecChildUI;
    CUI*                m_pParentUI;
    Vec2                m_vFinalPos;

    bool                m_bCamAffected;     //UI가 카메라의 영향을 받는 유무. 
    bool                m_bMouseOn;         //UI위에 마우스가 있는지.
    bool                m_bLbtnDown;        //UI에 왼쪽 버튼이 눌린 적이 있는지. 
public:

    Vec2 GetFinalPos() { return m_vFinalPos; }
    CUI* GetParent() { return m_pParentUI; }
    bool IsMouseOn() { return m_bMouseOn; }
    bool IsLbtnDown() { return m_bLbtnDown; }

    void AddChild(CUI* _pUI) 
    { 
        m_vecChildUI.push_back(_pUI);
        _pUI->m_pParentUI = this;
    }
    const vector<CUI*>& GetChildsUI() { return m_vecChildUI; }
public:
    virtual void update();
    virtual void finalupdate();
    virtual void render(HDC _dc);

private:
    void update_child();
    void finalupdate_child();
    void render_child(HDC _dc);


    void MouseOnCheck();

public:
    virtual void MouseOn();             //마우스가 UI위에 올라와 있을 때. 
    
    virtual void MouseLbtnDown();       //눌렸을 때.
    virtual void MouseLbtnUp();         //떼졌을 때.
    virtual void MouseLbtnClicked();    //클릭. 

    virtual CUI* Clone() = 0;

public:
    CUI(bool _bcamAff);
    CUI(const CUI& _origin);
    virtual ~CUI();

    friend class CUIMgr;
};

