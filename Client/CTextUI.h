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

    wstring             m_Text;                 //����� �ؽ�Ʈ
    int                 m_iFontSize;            //��Ʈ ũ��         
    Color               m_textColor;            //�ؽ�Ʈ ����

    bool                m_bdrawOutline;         // �ܰ����� �׸��� ����
    Color               m_cgdiPlusOutlineColor; // �ܰ��� ���� (GDI+��)
    float               m_foutlineThickness;    // �ܰ��� �β�

    int                 m_iNumber;              //ȭ�鿡 ����� ����
    float               m_fAcc;                 //���� �ð�
public:
    //UI��� ����
    void SetMode(TextUIMode _mode) { m_mode = _mode; }
    TextUIMode GetMode() const { return m_mode; }

    // TEXT ���: �ؽ�Ʈ ����
    void SetText(const wstring& _text) {
        if (m_mode == TextUIMode::TEXT) {
            m_Text = _text;
        }
    }

    // NUMBER ���: ���� ���� �� ������Ʈ
    void SetNumber(int _number) {
        if (m_mode == TextUIMode::NUMBER) {
            m_iNumber = _number;
        }
    }

    // �ؽ�Ʈ ����
    //void SetText(const wstring& _text) { m_Text = _text; }
    const wstring& GetText() const { return m_Text; }

    // ��Ʈ ũ�� ����
    void SetFontSize(int _size) { m_iFontSize = _size; }
    int GetFontSize() const { return m_iFontSize; }

    // �ؽ�Ʈ ���� ���� (GDI+)
    void SetGdiPlusTextColor(const Color& _color) { m_textColor = _color; }
    Color GetGdiPlusTextColor() const { return m_textColor; }

    // �ܰ��� ����
    void SetDrawOutline(bool _draw) { m_bdrawOutline = _draw; }
    bool GetDrawOutline() { return m_bdrawOutline; }

    // �ܰ��� ���� (GDI+��)
    void SetGdiPlusOutlineColor(const Color& _color) { m_cgdiPlusOutlineColor = _color; }
    Color GetGdiPlusOutlineColor() const { return m_cgdiPlusOutlineColor; }

    // �ܰ��� �β�
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

