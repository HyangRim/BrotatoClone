#include "pch.h"
#include "CFontMgr.h"
#include "CPathMgr.h"


CFontMgr::CFontMgr()
    :m_iFontCount(0)
    ,m_fontFamilies(nullptr)
{

}

CFontMgr::~CFontMgr()
{
    delete[] m_fontFamilies;
}

FontFamily* CFontMgr::GetFont(FONT_TYPE _eType)
{
    if ((UINT)_eType < m_iFontCount) {
        return &m_fontFamilies[(UINT)_eType];
    }
    return nullptr; // 잘못된 요청에 대해 nullptr 반환
}

void CFontMgr::init()
{
    wstring strFilePath = CPathMgr::GetInstance()->GetContentPath();

    // 기본 폰트 및 한국어 폰트 경로 설정
    wstring defaultFont = strFilePath + L"font\\Anybody-Medium.ttf";
    wstring krFont = strFilePath + L"font\\NotoSansKR-Medium.ttf";

    // AddFontFile에 파일 경로 전달 (std::wstring -> const WCHAR*)
    m_fontCollection.AddFontFile(defaultFont.c_str());
    m_fontCollection.AddFontFile(krFont.c_str());

    m_iFontCount = m_fontCollection.GetFamilyCount();
    m_fontFamilies = new FontFamily[m_iFontCount];

    int numFound = 0;
    m_fontCollection.GetFamilies(m_iFontCount, m_fontFamilies, &numFound);
}
