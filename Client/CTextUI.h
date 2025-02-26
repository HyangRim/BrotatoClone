#pragma once


class CTextUI
{
private:
    CObject*            m_pOwner;
    TextUIMode          m_mode;

    //offset����
    //object�� pos��������
    Vec2                m_vOffsetLT;            //�׷��� ��ġ�»��
    Vec2                m_vOffsetRB;            //�׷��� ��ġ���ϴ�

    FONT_TYPE           m_fontType;             //��Ʈ(DEFAULT, KR)

    wstring             m_Text;                 //����� �ؽ�Ʈ
    int                 m_iFontSize;            //��Ʈ ũ��         
    D2D1::ColorF        m_colorText;            //�ؽ�Ʈ ����

    bool                m_bdrawOutline;         //�ܰ����� �׸��� ����
    float               m_fOutlineThickness;    //�ܰ��� �β�
    D2D1::ColorF        m_colorOutline;         //�ܰ��� ����

    int                 m_iNumber;              //ȭ�鿡 ����� ����
    float               m_fAcc;                 //���� �ð�

    D2D1::ColorF        m_RedColor;

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

public:
    void SetOffsetLT(Vec2 _vOffsetLT) { m_vOffsetLT = _vOffsetLT; }
    void SetOffsetRB(Vec2 _vOffsetRB) { m_vOffsetRB = _vOffsetRB; }

public:
    // �ؽ�Ʈ ����
    //void SetText(const wstring& _text) { m_Text = _text; }
    const wstring& GetText() const { return m_Text; }

    // ��Ʈ ũ�� ����
    void SetFontSize(int _size) { m_iFontSize = _size; }
    int GetFontSize() const { return m_iFontSize; }

    void SetFontType(FONT_TYPE _eType) { m_fontType = _eType; }
    FONT_TYPE GetFontType() { return m_fontType; }

    // ���� ����
    void SetTextColor(D2D1::ColorF _colorText) { m_colorText = _colorText; }
    D2D1::ColorF GetTextColor() const { return m_colorText; }

public:
    // �ܰ��� ����
    void SetDrawOutline(bool _draw) { m_bdrawOutline = _draw; }
    bool GetDrawOutline() { return m_bdrawOutline; }

    // �ܰ��� �β�
    void SetOutlineThickness(float _thickness) { m_fOutlineThickness = _thickness; }
    float GetOutlineThickness() const { return m_fOutlineThickness; }

    // �ܰ��� ����
    void SetOutlineColor(D2D1::ColorF _colorOutline) { m_colorOutline = _colorOutline; }
    D2D1::ColorF GetOutlineColor() const { return m_colorOutline; }

public:
    //���ʼ�����
    void SetTime(int _iStartNum) { m_iNumber = _iStartNum; }

public:
    void UpdateNumber();
    virtual void render(ID2D1HwndRenderTarget* _pRender);
    virtual void update();

public:
    CTextUI();
    ~CTextUI();

    friend class CObject;
};

