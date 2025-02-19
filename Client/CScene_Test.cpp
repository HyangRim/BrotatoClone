#include "pch.h"
#include "CScene_Test.h"
#include "CTimeMgr.h"
#include "CPanelUI.h"
#include "CCore.h"
#include "CScene.h"
#include "CCamera.h"
#include "CFontMgr.h"

CScene_Test::CScene_Test()
	: m_fAcc(0.f)
	, m_iTest(0)
{

}

CScene_Test::~CScene_Test()
{

}


void CScene_Test::update()
{
	CScene::update();

	m_fAcc += fDT;
	if (m_fAcc >= 1.f)
	{
		m_iTest += (int)(m_fAcc / 1.f);
		m_fAcc = 0.f;
	}
}

void CScene_Test::finalupdate()
{
	CScene::finalupdate();
}

void CScene_Test::render(HDC _dc)
{
	//CScene::render(_dc);
}

void CScene_Test::render(Gdiplus::Graphics* _pDGraphics)
{
	CScene::render(_pDGraphics);

	/*
	_pDGraphics->SetSmoothingMode(SmoothingModeAntiAlias);
	FontFamily* defaultFont = CFontMgr::GetInstance()->GetFont(FONT_TYPE::KR);

	assert(defaultFont);

	//Font font(defaultFont, 48, FontStyleRegular, UnitPixel);

	SolidBrush brush(Color(255, 0, 0));
	// 텍스트 정렬 설정

	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);   // 가로 중앙 정렬
	format.SetLineAlignment(StringAlignmentCenter); // 세로 중앙 정렬

	WCHAR text[10];
	swprintf_s(text, L"%d", m_iTest);

	// GraphicsPath로 텍스트 경로 생성
	GraphicsPath path;
	path.AddString(
		text,
		(int)wcslen(text),
		defaultFont,
		FontStyleBold,
		48,
		PointF(300.0f, 100.0f),
		&format
	);

	// 외곽선 그리기 (검은색)
	Pen outlinePen(Color(255, 255, 255, 255), 4); // 검은색 외곽선, 두께 4px
	_pDGraphics->DrawPath(&outlinePen, &path);

	// 텍스트 내부 채우기 (빨간색)
	SolidBrush fillBrush(Color(255, 255, 0, 0)); // 빨간색 채우기
	_pDGraphics->FillPath(&fillBrush, &path);

	/*
	// 텍스트 그릴 영역 설정 (윈도우 중앙에 표시)
	RectF rect(50.0f, 50.0f, 300.0f, 200.0f);

	// 현재 숫자를 문자열로 변환하여 출력
	WCHAR text[10];
	swprintf_s(text, L"%d", m_iTest);
	_pDGraphics->DrawString(text, -1, &font, rect, &format, &brush);
	*/

	
}

void CScene_Test::Enter()
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	CUI* pPanelUI = new CPanelUI;
	pPanelUI->SetName(L"ParentUI2");
	pPanelUI->SetScale(Vec2(500.f, 300.f));
	pPanelUI->SetPos(Vec2(vResolution.x - pPanelUI->GetScale().x, 0.f));

	AddObject(pPanelUI, GROUP_TYPE::UI);

	CCamera::GetInstance()->SetLookAt(vResolution / 2.f);
}

void CScene_Test::Exit()
{

}