#pragma once


class CTextUI
{
private:
    CObject*            m_pOwner;
    TextUIMode          m_mode;

    //offset개념
    //object의 pos기준으로
    Vec2                m_vOffsetLT;            //그려질 위치좌상단
    Vec2                m_vOffsetRB;            //그려질 위치우하단

    FONT_TYPE           m_fontType;             //폰트(DEFAULT, KR)

    wstring             m_Text;                 //출력할 텍스트
    int                 m_iFontSize;            //폰트 크기         
    D2D1::ColorF        m_colorText;            //텍스트 색상

    bool                m_bdrawOutline;         //외곽선을 그릴지 여부
    float               m_fOutlineThickness;    //외곽선 두께
    D2D1::ColorF        m_colorOutline;         //외곽선 색상

    int                 m_iNumber;              //화면에 출력할 숫자
    float               m_fAcc;                 //누적 시간

    D2D1::ColorF        m_RedColor;

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

public:
    void SetOffsetLT(Vec2 _vOffsetLT) { m_vOffsetLT = _vOffsetLT; }
    void SetOffsetRB(Vec2 _vOffsetRB) { m_vOffsetRB = _vOffsetRB; }

public:
    // 텍스트 설정
    //void SetText(const wstring& _text) { m_Text = _text; }
    const wstring& GetText() const { return m_Text; }

    // 폰트 크기 설정
    void SetFontSize(int _size) { m_iFontSize = _size; }
    int GetFontSize() const { return m_iFontSize; }

    void SetFontType(FONT_TYPE _eType) { m_fontType = _eType; }
    FONT_TYPE GetFontType() { return m_fontType; }

    // 글자 색상
    void SetTextColor(D2D1::ColorF _colorText) { m_colorText = _colorText; }
    D2D1::ColorF GetTextColor() const { return m_colorText; }

public:
    // 외곽선 설정
    void SetDrawOutline(bool _draw) { m_bdrawOutline = _draw; }
    bool GetDrawOutline() { return m_bdrawOutline; }

    // 외곽선 두께
    void SetOutlineThickness(float _thickness) { m_fOutlineThickness = _thickness; }
    float GetOutlineThickness() const { return m_fOutlineThickness; }

    // 외곽선 색상
    void SetOutlineColor(D2D1::ColorF _colorOutline) { m_colorOutline = _colorOutline; }
    D2D1::ColorF GetOutlineColor() const { return m_colorOutline; }

public:
    //몇초셀건지
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

