#include "pch.h"
#include "CCore.h"


#include "CObject.h"
#include "CTimeMgr.h"
#include "CkeyMgr.h"
#include "CSceneMgr.h"
#include "CPathMgr.h"
#include "CCollisionMgr.h"
#include "CEventMgr.h"
#include "CCamera.h"
#include "CUIMgr.h"
#include "CTexture.h"
#include "CResMgr.h"
#include "CSoundMgr.h"
#include "Direct2DMgr.h"
#include "CFontMgr.h"
#include "CharacterInfoMgr.h"

#include "SelectGDI.h"
#include "Resource.h"



//CCore* CCore::g_pInst = nullptr;

CCore::CCore() 
	: m_hWnd(0), m_ptResolution{}, m_hDC(0)
	, m_arrBrush{}, m_arrPen{}
	, m_pGraphics(nullptr), m_pDGraphics(nullptr)
{
}


CCore::~CCore() {
	//m_hWnd에 엮여있던 m_hDC를 해제해준다. 
	ReleaseDC(m_hWnd, m_hDC);

	for (auto penIDX = 0; penIDX < (UINT)PEN_TYPE::END; penIDX++) {
		DeleteObject(m_arrPen[penIDX]);
	}

	DestroyMenu(m_hMenu);
	delete m_pGraphics;
	delete m_pDGraphics;
	delete m_buffer;
	Gdiplus::GdiplusShutdown(gdiplusToken);
}


int CCore::init(HWND _hWnd, POINT _ptResolution) {
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	//해상도에 맞게 윈도우 크기 조정. 
	ChangeWindowSize(Vec2((float)_ptResolution.x, (float)_ptResolution.y), false);

	//메뉴바 생성. 
	m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCEW(IDC_CLIENT));
	//어디다가 그려요? m_hWnd에다가 그려요. 
	m_hDC = GetDC(m_hWnd);
	m_pGraphics = new Gdiplus::Graphics(m_hDC);

	//이중 버퍼링 용도의 텍스쳐 한 장을 만든다. 
	m_pMemTex = CResMgr::GetInstance()->CreateTexture(L"BackBuffer", (UINT)(m_ptResolution.x), (UINT)(m_ptResolution.y));
	m_pDGraphics = new Gdiplus::Graphics(m_pMemTex->GetDC());
	RECT psRect;
	
	GetClientRect(m_hWnd, &psRect);
	m_buffer = new Bitmap(psRect.right - psRect.left, psRect.bottom - psRect.top);

	//m_pDGraphics는 메모리 비트맵. (임시로 그리는 곳.)
	m_pDGraphics = new Gdiplus::Graphics(m_buffer);

	//자주 사용 할 펜 및 브러쉬 생성. 
	CreateBrushPen();

	//Manager initialize
	CPathMgr::GetInstance()->init();

	CTimeMgr::GetInstance()->init();

	CkeyMgr::GetInstance()->init();

	CSoundMgr::GetInstance()->init();

	CCamera::GetInstance()->init();
	Direct2DMgr::GetInstance()->init(m_hWnd);
	CSceneMgr::GetInstance()->init();


	CFontMgr::GetInstance()->init();
	CharacterInfoMgr::GetInstance()->init();

	return S_OK;
}


void CCore::progress() {

	// ============
	//	Manager Update
	// ============
	CTimeMgr::GetInstance()->update();
	CkeyMgr::GetInstance()->update();
	CCamera::GetInstance()->update();
	CSoundMgr::GetInstance()->update();

	// ============
	//	Scene Update
	// ============

	CSceneMgr::GetInstance()->update();

	// 충돌 체크. 
	CCollisionMgr::GetInstance()->update();

	//UI 이벤트 체크
	CUIMgr::GetInstance()->update();


	// ============
	//	Rendering
	// ============
	

	//화면 초기화(흰 사각형으로 덧 씌움)
	//Clear();

	//어디다가 그려야하는지 알려주기. 
	
	// GDI버전.
	// Draw시작.
	ID2D1HwndRenderTarget* pRenderTarget = Direct2DMgr::GetInstance()->GetRenderTarget();
	//한 프레임당 그림 그리기 시작. 
	pRenderTarget->BeginDraw();
	Clear();

	// GDI버전 렌더링. 
	//CSceneMgr::GetInstance()->render(m_pMemTex->GetDC());
	//CCamera::GetInstance()->render(m_pMemTex->GetDC());

	// GDI+버전 렌더링. 
	//CSceneMgr::GetInstance()->render(m_pDGraphics);
	//CCamera::GetInstance()->render(m_pDGraphics);

	//Direct2D 버전 렌더링
	CSceneMgr::GetInstance()->render(pRenderTarget);
	
	//한 프레임 렌더링 끝. 
	HRESULT hr = pRenderTarget->EndDraw();
	if (FAILED(hr)) {
		MessageBox(nullptr, L"렌더링 실패!", L"오류", MB_OK);
	}

	// Draw 끝. 


	//m_DGraphics
	//m_pGraphics->DrawImage(m_pMemTex->GetBitmap(), 0, 0, m_ptResolution.x, m_ptResolution.y);
	//GDI(Legacy 방식)
	//BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, m_pMemTex->GetDC(), 0, 0, SRCCOPY);
	
	//GDI+ 방식
	//m_pGraphics->DrawImage(m_buffer, 0, 0);


	CTimeMgr::GetInstance()->render();


	//렌더링까지 전부 다 끝난 후에 EventMgr에서 이벤트 지연 처리.
	// ============
	//	이벤트 지연 처리.
	// ============
	CEventMgr::GetInstance()->update();

}

void CCore::DockMenu()
{
	//툴 Scene에서 사용할 메뉴를 붙인다. 
	SetMenu(m_hWnd, m_hMenu);
	ChangeWindowSize(GetResolution(), true);
}

void CCore::DivideMenu()
{
	SetMenu(m_hWnd, nullptr);
	ChangeWindowSize(GetResolution(), false);
}

void CCore::ChangeWindowSize(Vec2 _vResolution, bool _bMenu)
{
	RECT rt = { 0,0,(long)_vResolution.x, (long)_vResolution.y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, _bMenu);
	SetWindowPos(m_hWnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);
}



void CCore::Clear()
{
	//GDI+ 방식
	//m_pDGraphics->Clear(Color(255, 120, 120, 120));
	
	//GDI LEGACY방식
	//SelectGDI gdi(m_pMemTex->GetDC(), BRUSH_TYPE::BLACK);
	//Rectangle(m_pMemTex->GetDC(), -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);

	//Direct2D방식
	ID2D1HwndRenderTarget* pRenderTarget = Direct2DMgr::GetInstance()->GetRenderTarget();

	pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	//pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF(0.5f, 0.5f, 0.5f, 1.0f)));
}

void CCore::CreateBrushPen()
{
	// Hollow Brush
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	m_arrBrush[(UINT)BRUSH_TYPE::BLACK] = (HBRUSH)GetStockObject(BLACK_BRUSH);


	//R,G,B Pen
	m_arrPen[(UINT)PEN_TYPE::RED] = (HPEN)CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_arrPen[(UINT)PEN_TYPE::GREEN] = (HPEN)CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_arrPen[(UINT)PEN_TYPE::BLUE] = (HPEN)CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

}