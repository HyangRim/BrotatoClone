#pragma once
#include "CUI.h"

enum class TextUIMode
{
    TEXT,
    NUMBER,
};

class CTextUI :
    public CUI
{
private:
    TextUIMode          m_mode;

    wstring             m_Text;                 //출력할 텍스트
    int                 m_iFontSize;            //폰트 크기         
    Color               m_textColor;            //텍스트 색상

    bool                m_bdrawOutline;         // 외곽선을 그릴지 여부
    Color               m_cgdiPlusOutlineColor; // 외곽선 색상 (GDI+용)
    float               m_foutlineThickness;    // 외곽선 두께

    int                 m_iNumber;              //화면에 출력할 숫자
    float               m_fAcc;                 //누적 시간
public:
    //UI모드 설정
    void SetMode(TextUIMode _mode) { m_mode = _mode; }
    TextUIMode GetMode() const { return m_mode; }

    // TEXT 모드: 텍스트 설정
    void SetText(const wstring& _text) {
        if (m_mode == TextUIMode::TEXT) {
            m_Text = _text;
        }
    }

    // NUMBER 모드: 숫자 설정 및 업데이트
    void SetNumber(int _number) {
        if (m_mode == TextUIMode::NUMBER) {
            m_iNumber = _number;
        }
    }

    // 텍스트 설정
    //void SetText(const wstring& _text) { m_Text = _text; }
    const wstring& GetText() const { return m_Text; }

    // 폰트 크기 설정
    void SetFontSize(int _size) { m_iFontSize = _size; }
    int GetFontSize() const { return m_iFontSize; }

    // 텍스트 색상 설정 (GDI+)
    void SetGdiPlusTextColor(const Color& _color) { m_textColor = _color; }
    Color GetGdiPlusTextColor() const { return m_textColor; }

    // 외곽선 설정
    void SetDrawOutline(bool _draw) { m_bdrawOutline = _draw; }
    bool GetDrawOutline() { return m_bdrawOutline; }

    // 외곽선 색상 (GDI+용)
    void SetGdiPlusOutlineColor(const Color& _color) { m_cgdiPlusOutlineColor = _color; }
    Color GetGdiPlusOutlineColor() const { return m_cgdiPlusOutlineColor; }

    // 외곽선 두께
    void SetOutlineThickness(float _thickness) { m_foutlineThickness = _thickness; }
    float GetOutlineThickness() const { return m_foutlineThickness; }

public:
    void UpdateNumber();
    virtual void render(Gdiplus::Graphics* _pDGraphics);
    virtual void render(ID2D1HwndRenderTarget* _pRender);
    virtual void update();

public:
    CLONE(CTextUI);

public:
    CTextUI();
    ~CTextUI();
};

