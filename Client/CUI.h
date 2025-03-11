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
    UI_TYPE             m_eUIType;


protected:
    bool                m_bCamAffected;     //UI가 카메라의 영향을 받는 유무. 
    bool                m_bMouseOn;         //UI위에 마우스가 있는지.
    bool                m_bLbtnDown;        //UI에 왼쪽 버튼이 눌린 적이 있는지. 

protected:
    Vec2                m_vOffset;
    Vec2                m_vFinalPos;

public:
    void SetOffset(Vec2 _vOffset) { m_vOffset = _vOffset; }
    Vec2 GetOffset() const { return m_vOffset; }

    virtual Vec2 GetFinalPos() override
    {
        if (GetParent())
        {
            return GetParent()->GetFinalPos() + m_vOffset;
        }
        return GetPos() + m_vOffset;
    }

public:
    void SetUIType(UI_TYPE _eType) { m_eUIType = _eType; }
    UI_TYPE GetUIType() { return m_eUIType; }
    //  Vec2 GetFinalPos() { return m_vFinalPos; }
    CUI* GetParent() { return m_pParentUI; }
    bool IsMouseOn() { return m_bMouseOn; }
    bool IsLbtnDown() { return m_bLbtnDown; }

    void AddChild(CUI* _pUI) 
    { 
        m_vecChildUI.push_back(_pUI);
        _pUI->m_pParentUI = this;
    }
    template<typename T>
    T* AddChild(Vec2 _vOffset)
    {
        T* pChild = new T;
        pChild->SetOffset(_vOffset);
        pChild->SetParent(this);
        m_vecChildUI.push_back(pChild);
        return pChild;
    }

public:
    void SetParent(CUI* _parent) { m_pParentUI = _parent; }

    const vector<CUI*>& GetChildsUI() { return m_vecChildUI; }
public:
    virtual void update();
    virtual void finalupdate();

    void update_child();
    void finalupdate_child();

    virtual void render(HDC _dc);
    virtual void render(Gdiplus::Graphics* _pDGraphics);
    virtual void render(ID2D1HwndRenderTarget* _pRender);

    void render_child(HDC _dc);
    void render_child(Gdiplus::Graphics* _pDGraphics);
    void render_child(ID2D1HwndRenderTarget* _pRender);

   
public:
    virtual void MouseOn();             //마우스가 UI위에 올라와 있을 때. 
    virtual void MouseLbtnDown();       //눌렸을 때.
    virtual void MouseLbtnUp();         //떼졌을 때.
    virtual void MouseLbtnClicked();    //클릭. 
    void MouseOnCheck();

    virtual CUI* Clone() = 0;

public:
    CUI(bool _bcamAff);
    CUI(const CUI& _origin);
    virtual ~CUI();

    friend class CUIMgr;
};

