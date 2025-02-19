#pragma once
#include "CUI.h"
class CTextUI :
    public CUI
{
private:
    wstring             m_Text;                 //����� �ؽ�Ʈ
    int                 m_iFontSize;            //��Ʈ ũ��
    //COLORREF            m_textColor;          
    Color               m_textColor;     //�ؽ�Ʈ ����


public:
    // �ؽ�Ʈ ����
    void SetText(const std::wstring& _text) { m_Text = _text; }
    const wstring& GetText() const { return m_Text; }

    // ��Ʈ ũ�� ����
    void SetFontSize(int _size) { m_iFontSize = _size; }
    int GetFontSize() const { return m_iFontSize; }

    // �ؽ�Ʈ ���� ���� (GDI+)
    void SetGdiPlusTextColor(const Gdiplus::Color& _color) { m_textColor = _color; }
    Gdiplus::Color GetGdiPlusTextColor() const { return m_textColor; }

    virtual void render(Gdiplus::Graphics* _pDGraphics);

public:
    CLONE(CTextUI);

public:
    CTextUI();
    ~CTextUI();
};

