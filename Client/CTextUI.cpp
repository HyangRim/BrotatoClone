#include "pch.h"
#include "CTextUI.h"
#include "CFontMgr.h"

CTextUI::CTextUI()
	:CUI(false)
	, m_Text(L"TEST")
	, m_iFontSize(20)
	, m_textColor(Gdiplus::Color(255,0,0,0))
{

}

CTextUI::~CTextUI()
{

}

// GDI+를 사용한 렌더링
void CTextUI::render(Gdiplus::Graphics* _pDGraphics)
{
    if (m_Text.empty()) return;

    FontFamily* defaultFont = CFontMgr::GetInstance()->GetFont(FONT_TYPE::DEFALUT);
    Gdiplus::Font font(defaultFont, static_cast<Gdiplus::REAL>(m_iFontSize), Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    Gdiplus::SolidBrush brush(m_textColor);

    Gdiplus::StringFormat format;
    format.SetAlignment(Gdiplus::StringAlignmentCenter);   // 가로 중앙 정렬
    format.SetLineAlignment(Gdiplus::StringAlignmentCenter); // 세로 중앙 정렬

    GraphicsPath path;
    path.AddString(
        m_Text.c_str(),
        (int)wcslen(m_Text.c_str()),
        defaultFont,
        FontStyleBold,
        48,
        PointF(300.0f, 100.0f),
        &format
    );

    Gdiplus::RectF layoutRect(
        static_cast<Gdiplus::REAL>(GetPos().x),
        static_cast<Gdiplus::REAL>(GetPos().y),
        static_cast<Gdiplus::REAL>(GetScale().x),
        static_cast<Gdiplus::REAL>(GetScale().y)
    );

    _pDGraphics->DrawString(m_Text.c_str(), -1, &font, layoutRect, &format, &brush);
}

