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
	// �ؽ�Ʈ ���� ����

	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);   // ���� �߾� ����
	format.SetLineAlignment(StringAlignmentCenter); // ���� �߾� ����

	WCHAR text[10];
	swprintf_s(text, L"%d", m_iTest);

	// GraphicsPath�� �ؽ�Ʈ ��� ����
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

	// �ܰ��� �׸��� (������)
	Pen outlinePen(Color(255, 255, 255, 255), 4); // ������ �ܰ���, �β� 4px
	_pDGraphics->DrawPath(&outlinePen, &path);

	// �ؽ�Ʈ ���� ä��� (������)
	SolidBrush fillBrush(Color(255, 255, 0, 0)); // ������ ä���
	_pDGraphics->FillPath(&fillBrush, &path);

	/*
	// �ؽ�Ʈ �׸� ���� ���� (������ �߾ӿ� ǥ��)
	RectF rect(50.0f, 50.0f, 300.0f, 200.0f);

	// ���� ���ڸ� ���ڿ��� ��ȯ�Ͽ� ���
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