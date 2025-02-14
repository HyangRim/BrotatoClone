#pragma once

#include "define.h"

class CCore {
	SINGLE(CCore);

private:
	HWND	m_hWnd;				//메인 윈도우 핸들. 
	POINT	m_ptResolution;		//메인 윈도우 해상도

	HDC		m_hDC;				//메인 윈도우에 Draw 할 DC


	HBITMAP m_hBit;				//사본용 . 
	HDC		m_memDC;


	//자주 사용하는 GDI Object 펜
	HBRUSH	m_arrBrush[(UINT)BRUSH_TYPE::END];
	HPEN	m_arrPen[(UINT)PEN_TYPE::END];

private:
	void CreateBrushPen();

public:
	int init(HWND _hWnd, POINT _ptResolution);
	void progress();

public:
	HWND GetMainHwnd() { return m_hWnd; }
	HDC GetMainDC() { return m_hDC; }
	POINT GetResolution() { return m_ptResolution; }

	HBRUSH GetBrush(BRUSH_TYPE _eType) { return m_arrBrush[(UINT)_eType]; }
	HPEN GetPen(PEN_TYPE _eType) { return m_arrPen[(UINT)_eType]; }

};