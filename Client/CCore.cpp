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

#include "SelectGDI.h"
#include "Resource.h"



//CCore* CCore::g_pInst = nullptr;

CCore::CCore() 
	: m_hWnd(0), m_ptResolution{}, m_hDC(0)
	, m_arrBrush{}, m_arrPen{}
{
}


CCore::~CCore() {
	//m_hWnd�� �����ִ� m_hDC�� �������ش�. 
	ReleaseDC(m_hWnd, m_hDC);


	
	for (auto penIDX = 0; penIDX < (UINT)PEN_TYPE::END; penIDX++) {
		DeleteObject(m_arrPen[penIDX]);
	}

	DestroyMenu(m_hMenu);
}


int CCore::init(HWND _hWnd, POINT _ptResolution) {
	m_hWnd = _hWnd;
	m_ptResolution = _ptResolution;

	//�ػ󵵿� �°� ������ ũ�� ����. 
	ChangeWindowSize(Vec2((float)_ptResolution.x, (float)_ptResolution.y), false);

	//�޴��� ����. 
	m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCEW(IDC_CLIENT));
	//���ٰ� �׷���? m_hWnd���ٰ� �׷���. 
	m_hDC = GetDC(m_hWnd);


	//���� ���۸� �뵵�� �ؽ��� �� ���� �����. 
	m_pMemTex = CResMgr::GetInstance()->CreateTexture(L"BackBuffer", (UINT)(m_ptResolution.x), (UINT)(m_ptResolution.y));

	//���� ��� �� �� �� �귯�� ����. 
	CreateBrushPen();

	//Manager initialize
	CPathMgr::GetInstance()->init();
	CTimeMgr::GetInstance()->init();
	CkeyMgr::GetInstance()->init();
	CSoundMgr::GetInstance()->init();
	CCamera::GetInstance()->init();
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
	CSoundMgr::GetInstance()->update();

	// ============
	//	Scene Update
	// ============

	CSceneMgr::GetInstance()->update();

	// �浹 üũ. 
	CCollisionMgr::GetInstance()->update();

	//UI �̺�Ʈ üũ
	CUIMgr::GetInstance()->update();


	// ============
	//	Rendering
	// ============
	

	//ȭ�� �ʱ�ȭ(�� �簢������ �� ����)

	Clear();

	//���ٰ� �׷����ϴ��� �˷��ֱ�. 
	CSceneMgr::GetInstance()->render(m_pMemTex->GetDC());
	CCamera::GetInstance()->render(m_pMemTex->GetDC());

	BitBlt(m_hDC, 0, 0, m_ptResolution.x, m_ptResolution.y, m_pMemTex->GetDC(), 0, 0, SRCCOPY);
	
	CTimeMgr::GetInstance()->render();


	//���������� ���� �� ���� �Ŀ� EventMgr���� �̺�Ʈ ���� ó��.
	// ============
	//	�̺�Ʈ ���� ó��.
	// ============
	CEventMgr::GetInstance()->update();

}

void CCore::DockMenu()
{
	//�� Scene���� ����� �޴��� ���δ�. 
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
	SelectGDI gdi(m_pMemTex->GetDC(), BRUSH_TYPE::BLACK);
	Rectangle(m_pMemTex->GetDC(), -1, -1, m_ptResolution.x + 1, m_ptResolution.y + 1);
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