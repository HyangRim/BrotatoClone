#include "pch.h"
#include "CImage.h"
#include "CObject.h"
#include "Direct2DMgr.h"


CImage::CImage()
	: m_pOwner(nullptr)
	, m_pBitmap(nullptr)
{
}

CImage::~CImage()
{
}


void CImage::render(HDC _dc)
{
}

void CImage::render(ID2D1HwndRenderTarget* _renderTarget)
{
	/////////////수정사항////////////////////////
	// 1. 단순 GetPos -> 방향키로 이동하면 Image는 고정된 상태임
	// 2. GetRenderPos -> 방향키로 이동하면 Image도 Camera에 맞게 이동됨
	// 3. GetRealPos -> ????

	Vec2 vPos = m_pOwner->GetPos();
	vPos = CCamera::GetInstance()->GetRenderPos(vPos);
	/////////////수정사항////////////////////////


	Vec2 vScale = m_pOwner->GetScale();

	if (nullptr == m_pBitmap) return;

	float left = vPos.x - (vScale.x / 2.f);
	float top = vPos.y - (vScale.y / 2.f);
	float right = vPos.x + (vScale.x / 2.f);
	float down = vPos.y + (vScale.y / 2.f);

	D2D1_RECT_F rect = D2D1::RectF(left, top, right, down);

	_renderTarget->DrawBitmap(m_pBitmap, rect);
}

