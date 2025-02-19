#pragma once
#include "CUI.h"
class CTextUI :
    public CUI
{
private:
    wstring             m_Text;                 //출력할 텍스트
    int                 m_iFontSize;            //폰트 크기
    //COLORREF            m_textColor;          
    Color               m_textColor;     //텍스트 색상


public:
    // 텍스트 설정
    void SetText(const std::wstring& _text) { m_Text = _text; }
    const wstring& GetText() const { return m_Text; }

    // 폰트 크기 설정
    void SetFontSize(int _size) { m_iFontSize = _size; }
    int GetFontSize() const { return m_iFontSize; }

    // 텍스트 색상 설정 (GDI+)
    void SetGdiPlusTextColor(const Gdiplus::Color& _color) { m_textColor = _color; }
    Gdiplus::Color GetGdiPlusTextColor() const { return m_textColor; }

    virtual void render(Gdiplus::Graphics* _pDGraphics);

public:
    CLONE(CTextUI);

public:
    CTextUI();
    ~CTextUI();
};

