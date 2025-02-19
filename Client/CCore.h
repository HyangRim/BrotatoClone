#pragma once

#include "define.h"

class CTexture;

class CCore {
	SINGLE(CCore);

private:
	HWND		m_hWnd;				//���� ������ �ڵ�. 
	POINT		m_ptResolution;		//���� ������ �ػ�

	HDC			m_hDC;				//���� �����쿡 Draw �� DC



	CTexture*	m_pMemTex;			//����� �ؽ���. 


	//���� ����ϴ� GDI Object ��
	HBRUSH		m_arrBrush[(UINT)BRUSH_TYPE::END];
	HPEN		m_arrPen[(UINT)PEN_TYPE::END];

	//�޴���.
	HMENU		m_hMenu;	//Tool Scene������ ���. 


private:
	//GDI Plus ����. 
	ULONG_PTR			gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;

	Gdiplus::Bitmap*	m_buffer;

	//���� �׷��� �뵵�� Graphics
	//render�Լ������� ����ٰ� �׸�. 
	Gdiplus::Graphics*	m_pDGraphics;

	//�� �׷��� �뵵�� Graphics(m_pDGraphics�� �ִ� ���� �ű�)
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