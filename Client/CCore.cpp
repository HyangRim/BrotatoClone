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



//CCore* CCore::g_pInst = nullptr;

CCore::CCore() 
	: m_hWnd(0), m_ptResolution{}, m_hDC(0)
	, m_hBit(0), m_memDC(0)
	, m_arrBrush{}, m_arrPen{}
{
}


CCore::~CCore() {
	//m_hWnd에 엮여있던 m_hDC를 해제해준다. 
	ReleaseDC(m_hWnd, m_hDC);


	//Create로 만든 것은 DeleteDC로 해야한다. 
	DeleteDC(m_memDC);
	DeleteObject(m_hBit);
	
	for (auto penIDX = 0; penIDX < (UINT)PEN_TYPE::END; penIDX++) {
		DeleteObject(m_arrPen[penIDX]);
	}
}


int CCore::init(HWND _hWnd, POINT _ptResolution) {
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	//해상도에 맞게 윈도우 크기 조정. 

	RECT rt = { 0,0,m_ptResolution.x, m_ptResolution.y };
	//테두리 포함해서, 크기 보정해주는 것. 
	//rt 포인터에다가 다시 값을 변환해서 넣어줌. 
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, true);


	//타이틀이나 그런 걸 제외한, 순수하게 물체가 그려질 수 있는 영역.
	//첫 번째로 들어가는 건, 어떤 윈도우인지. 
	//SetWindow는 타이틀창이나 테두리까지 포함한 크기임. 
	SetWindowPos(m_hWnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);
	
	//어디다가 그려요? m_hWnd에다가 그려요. 
	m_hDC = GetDC(m_hWnd);


	//이중 버퍼링 용도의 비트맵과 DC를 만든다. 
	//그림을 그릴 수 있는 건 윈도우에 달려있는, 해상도만큼의 픽셀(묶어서 비트맵이라고 부름)

	//결국 똑같은 해상도로 비트맵을 가지면 되는 거 아닌가? 해서 똑같은 해상도의 비트맵을 만듬. 
	//Compatible은 호환성. (호환성을 가지고 있는 비트맵)
	//비트맵 아이디 값을 받았음. 
	m_hBit = CreateCompatibleBitmap(m_hDC, m_ptResolution.x, m_ptResolution.y);

	//새로 만든 목적지의 DC가 필요함. (어떤 구조인지에 대한 Device Context가 필요함)
	m_memDC = CreateCompatibleDC(m_hDC);

	//이번엔 그림을 그릴 타겟, 목적지를 교체해주는 것. 위에서 만든 비트맵을 전달해줌. 
	//되돌아 리턴되는 건 원래 비트맵이지 않을까? 기본적으로 들어가있는 1픽셀짜리 Default 비트맵. 
	HBITMAP holdBit = (HBITMAP)SelectObject(m_memDC, m_hBit);
	DeleteObject(holdBit);


	//자주 사용 할 펜 및 브러쉬 생성. 
	CreateBrushPen();

	//Manager initialize
	CPathMgr::GetInstance()->init();
	CTimeMgr::GetInstance()->init();
	CkeyMgr::GetInstance()->init();
	CSceneMgr::GetInstance()->init();


	return S_OK;
}


void CCore::progress() {

	// ============
	//	Manager Update
	// ============
	CTimeMgr::GetInstance()->update();
	CkeyMgr::GetInstance()->update();
	CCamera::GetInstance()->update();

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
	Rectangle(m_memDC, -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);

	//어디다가 그려야하는지 알려주기. 
	CSceneMgr::GetInstance()->render(m_memDC);

	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, m_memDC, 0, 0, SRCCOPY);
	
	CTimeMgr::GetInstance()->render();


	//렌더링까지 전부 다 끝난 후에 EventMgr에서 이벤트 지연 처리.
	// ============
	//	이벤트 지연 처리.
	// ============
	CEventMgr::GetInstance()->update();

}


void CCore::CreateBrushPen()
{
	// Hollow Brush
	m_arrBrush[(UINT)BRUSH_TYPE::HOLLOW] = (HBRUSH)GetStockObject(HOLLOW_BRUSH);

	//R,G,B Pen
	m_arrPen[(UINT)PEN_TYPE::RED] = (HPEN)CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_arrPen[(UINT)PEN_TYPE::GREEN] = (HPEN)CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_arrPen[(UINT)PEN_TYPE::BLUE] = (HPEN)CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

}