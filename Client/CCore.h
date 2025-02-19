#pragma once

#include "define.h"

class CTexture;

class CCore {
	SINGLE(CCore);

private:
	HWND		m_hWnd;				//메인 윈도우 핸들. 
	POINT		m_ptResolution;		//메인 윈도우 해상도

	HDC			m_hDC;				//메인 윈도우에 Draw 할 DC



	CTexture*	m_pMemTex;			//백버퍼 텍스쳐. 


	//자주 사용하는 GDI Object 펜
	HBRUSH		m_arrBrush[(UINT)BRUSH_TYPE::END];
	HPEN		m_arrPen[(UINT)PEN_TYPE::END];

	//메뉴바.
	HMENU		m_hMenu;	//Tool Scene에서만 사용. 


private:
	//GDI Plus 관련. 
	ULONG_PTR			gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;

	Gdiplus::Bitmap*	m_buffer;

	//이중 그래픽 용도의 Graphics
	//render함수에서는 여기다가 그림. 
	Gdiplus::Graphics*	m_pDGraphics;

	//본 그래픽 용도의 Graphics(m_pDGraphics에 있는 것을 옮김)
	Gdiplus::Graphics*	m_pGraphics;

private:
	void Clear();
	void CreateBrushPen();

public:
	int init(HWND _hWnd, POINT _ptResolution);
	void progress();
	void DockMenu();
	void DivideMenu();
	void ChangeWindowSize(Vec2 _vResolution, bool _bMenu);


public:
	HWND GetMainHwnd() { return m_hWnd; }
	HDC GetMainDC() { return m_hDC; }
	Gdiplus::Graphics* GetDGraphics() { return m_pDGraphics; }
	Gdiplus::Graphics* GetGraphics() { return m_pGraphics; }

	POINT GetResolution() { return m_ptResolution; }

	HBRUSH GetBrush(BRUSH_TYPE _eType) { return m_arrBrush[(UINT)_eType]; }
	HPEN GetPen(PEN_TYPE _eType) { return m_arrPen[(UINT)_eType]; }

};