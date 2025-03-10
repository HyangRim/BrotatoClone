#include "pch.h"
#include "CScene_Test.h"
#include "CTimeMgr.h"
#include "CPanelUI.h"
#include "CCore.h"
#include "CScene.h"
#include "CCamera.h"
#include "CFontMgr.h"
#include "CTextUI.h"
#include "CSpriteUI.h"
#include "Direct2DMgr.h"
#include "CGround.h"
#include "CImage.h"

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
}

void CScene_Test::finalupdate()
{
	CScene::finalupdate();
}

void CScene_Test::render(HDC _dc)
{
	CScene::render(_dc);

	// Direct2DMgr �ν��Ͻ� ��������
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();

	//////////////////////////////////////////////////////
	vector<std::pair<D2D1_RECT_F, wstring>> bitmapsToRender = {
	{ D2D1::RectF(0, 0, 512, 512), L"BaseMap1" },
	{ D2D1::RectF(200, 200, 300, 300), L"BaseMap2" }
	};

	// ��� ��Ʈ�� ������
	//pD2DMgr->RenderAllBitmaps(bitmapsToRender);
	//////////////////////////////////////////////////////

	for (int idx = 0; idx < bitmapsToRender.size(); idx++) {
		pD2DMgr->RenderBitmap(bitmapsToRender[idx].first, bitmapsToRender[idx].second);
	}
}

void CScene_Test::render(Gdiplus::Graphics* _pDGraphics)
{
	// �θ� Ŭ������ render ȣ��
	CScene::render(_pDGraphics);

}

void CScene_Test::render(ID2D1HwndRenderTarget* _pRender)
{
	CScene::render(_pRender);
}

void CScene_Test::Enter()
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	
	CCamera::GetInstance()->SetLookAt(vResolution / 2.f);
}

void CScene_Test::Exit()
{

}