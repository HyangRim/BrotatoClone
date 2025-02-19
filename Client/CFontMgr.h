#pragma once
class CFontMgr
{
	SINGLE(CFontMgr);
private:
	PrivateFontCollection	m_fontCollection;
	UINT					m_iFontCount;
	FontFamily*				m_fontFamilies;

public:
	int GetFontCount() { return m_iFontCount; }
	FontFamily* GetFont(FONT_TYPE _eType);
public:
	void init();
};

