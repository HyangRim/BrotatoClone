#include "pch.h"
#include "CTextUI.h"
#include "CFontMgr.h"
#include "CTimeMgr.h"

CTextUI::CTextUI()
	:CUI(false)
	, m_Text(L"TEST")
	, m_iFontSize(20)
	, m_textColor(Gdiplus::Color(255,0,0,0))
    , m_bdrawOutline(false)
    , m_cgdiPlusOutlineColor(Gdiplus::Color(255,0,0,0))
    , m_foutlineThickness(2.f)
    , m_iNumber(0)
    , m_fAcc(0.f)
{

}

CTextUI::~CTextUI()
{

}

void CTextUI::UpdateNumber()
{
    if (m_mode == TextUIMode::NUMBER) {
        // ���� �ð� ������Ʈ
        m_fAcc += fDT;

        // 1�ʸ��� ���� ����
        if (m_fAcc >= 1.f) {
            m_iNumber += static_cast<int>(m_fAcc / 1.f); // �� ������ ����
            m_fAcc = fmod(m_fAcc, 1.f);               // ���� �ð� ����
        }
    }
}

// GDI+�� ����� ������
void CTextUI::render(Gdiplus::Graphics* _pDGraphics)
{
    if (m_Text.empty()) return;

    FontFamily* defaultFont = CFontMgr::GetInstance()->GetFont(FONT_TYPE::DEFALUT);
    Gdiplus::Font font(defaultFont, static_cast<Gdiplus::REAL>(m_iFontSize), Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    Gdiplus::SolidBrush brush(m_textColor);

    Gdiplus::StringFormat format;
    format.SetAlignment(Gdiplus::StringAlignmentCenter);     // ���� �߾� ����
    format.SetLineAlignment(Gdiplus::StringAlignmentCenter); // ���� �߾� ����

    wstring output;
    if (m_mode == TextUIMode::TEXT) {
        output = m_Text;
    }
    else if (m_mode == TextUIMode::NUMBER) {
        wchar_t buffer[10];
        swprintf_s(buffer, L"%d", m_iNumber);
        output = buffer;
    }
    m_Text = output;

    Gdiplus::GraphicsPath path;
    path.AddString(
        m_Text.c_str(),
        -1,
        defaultFont,
        font.GetStyle(),
        static_cast<Gdiplus::REAL>(m_iFontSize),
        Gdiplus::PointF(static_cast<Gdiplus::REAL>(GetPos().x), static_cast<Gdiplus::REAL>(GetPos().y)),
        nullptr
    );
   
    if (m_bdrawOutline) {
        Gdiplus::Pen outlinePen(m_cgdiPlusOutlineColor, static_cast<Gdiplus::REAL>(m_foutlineThickness));
        outlinePen.SetLineJoin(Gdiplus::LineJoinRound); //�ε巴��
       
        _pDGraphics->DrawPath(&outlinePen, &path);
    }

    Gdiplus::SolidBrush fillBrush(m_textColor);
    _pDGraphics->FillPath(&fillBrush, &path);
}

void CTextUI::render(ID2D1HwndRenderTarget* _pRender)
{
}

void CTextUI::update()
{
    UpdateNumber();
}

