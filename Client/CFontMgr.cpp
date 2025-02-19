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
    return nullptr; // �߸��� ��û�� ���� nullptr ��ȯ
}

void CFontMgr::init()
{
    wstring strFilePath = CPathMgr::GetInstance()->GetContentPath();

    // �⺻ ��Ʈ �� �ѱ��� ��Ʈ ��� ����
    wstring defaultFont = strFilePath + L"font\\Anybody-Medium.ttf";
    wstring krFont = strFilePath + L"font\\NotoSansKR-Medium.ttf";

    // AddFontFile�� ���� ��� ���� (std::wstring -> const WCHAR*)
    m_fontCollection.AddFontFile(defaultFont.c_str());
    m_fontCollection.AddFontFile(krFont.c_str());

    m_iFontCount = m_fontCollection.GetFamilyCount();
    m_fontFamilies = new FontFamily[m_iFontCount];

    int numFound = 0;
    m_fontCollection.GetFamilies(m_iFontCount, m_fontFamilies, &numFound);
}
