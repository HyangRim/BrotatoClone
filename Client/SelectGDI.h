#pragma once
class SelectGDI
{

private:
	HDC			m_dc;
	HPEN		m_hDefaultPen;
	HBRUSH		m_hDefaultBrush;


public:
	SelectGDI(HDC _dc, PEN_TYPE _ePenType);
	SelectGDI(HDC _dc, BRUSH_TYPE _ePenType);
	~SelectGDI();
};

