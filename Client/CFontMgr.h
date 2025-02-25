#pragma once

class CFontMgr
{
	SINGLE(CFontMgr);
private:
	PrivateFontCollection	m_fontCollection;
	UINT					m_iFontCount;
	FontFamily*				m_fontFamilies;

	IDWriteFactory* m_pDWriteFactory;
	std::vector<IDWriteFontCollection*> m_fontCollections; // 여러 폰트 컬렉션 저장
	std::vector<std::wstring> m_fontPaths;

public:
	int GetFontCount() { return m_iFontCount; }
	FontFamily* GetFont(FONT_TYPE _eType);
public:
	void init();
	IDWriteTextFormat* GetTextFormat(FONT_TYPE type, float fontSize, const std::wstring& locale = L"en-us");
};

