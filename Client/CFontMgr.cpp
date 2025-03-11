#include "pch.h"
#include "CFontMgr.h"
#include "CPathMgr.h"
#include "CFontCollectionLoader.h"


CFontMgr::CFontMgr()
    : m_pDWriteFactory(nullptr)
{
    DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
}

CFontMgr::~CFontMgr()
{
    delete[] m_fontFamilies;

    for (auto fontCollection : m_fontCollections) {
        if (fontCollection) fontCollection->Release();
    }
    if (m_pDWriteFactory) m_pDWriteFactory->Release();
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
    wstring strContentPath = CPathMgr::GetInstance()->GetContentPath();

    // ��Ʈ ��� ����
    m_fontPaths = {
        strContentPath + L"font\\Anybody-Medium.ttf",
        strContentPath + L"font\\NotoSansKR-Medium.ttf"
    };

    // Font Collection Loader ���
    auto loader = new CFontCollectionLoader(m_fontPaths);

    HRESULT hr = m_pDWriteFactory->RegisterFontCollectionLoader(loader);

    if (SUCCEEDED(hr)) {
        for (UINT32 i = 0; i < m_fontPaths.size(); ++i) {
            IDWriteFontCollection* pCollection = nullptr;

            hr = m_pDWriteFactory->CreateCustomFontCollection(
                loader, &i, sizeof(i), &pCollection
            );

            if (SUCCEEDED(hr)) {
                m_fontCollections.push_back(pCollection);
            }
        }

        loader->Release();
        m_pDWriteFactory->UnregisterFontCollectionLoader(loader);
    }
}

IDWriteTextFormat* CFontMgr::GetTextFormat(FONT_TYPE type, float fontSize, const std::wstring& locale)
{
    if ((size_t)type >= m_fontCollections.size()) return nullptr;

    IDWriteTextFormat* pTextFormat = nullptr;
    HRESULT hr = m_pDWriteFactory->CreateTextFormat(
        m_fontPaths[(size_t)type].c_str(), // ��Ʈ �̸�
        m_fontCollections[(size_t)type],  // ��Ʈ �÷���
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        fontSize,
        locale.c_str(),
        &pTextFormat
    );
   
    return SUCCEEDED(hr) ? pTextFormat : nullptr;
}

