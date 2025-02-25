#include "pch.h"
#include "CImage.h"
#include "CObject.h"
#include "Direct2DMgr.h"


CImage::CImage()
	: m_pOwner(nullptr)
	, m_pBitmap(nullptr)
	, m_fRatio(1.0f)
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
	m_pOwner->GetRenderScale();

	if (m_pOwner->GetObjType() == GROUP_TYPE::UI)
	{
		
	}
	else
		vPos = CCamera::GetInstance()->GetRenderPos(vPos);
	/////////////수정사항////////////////////////

	Vec2 vScale = m_pOwner->GetRenderScale();

	if (nullptr == m_pBitmap) return;

	float left = vPos.x - (vScale.x / 2.f);
	float top = vPos.y - (vScale.y / 2.f);
	float right = vPos.x + (vScale.x / 2.f);
	float down = vPos.y + (vScale.y / 2.f);

	D2D1_RECT_F rect = D2D1::RectF(left, top, right, down);

	// 5. HP 비율에 따라 채워질 너비를 계산합니다.
	float fillWidth = (right - left) * m_fRatio;

	// 채워지는 부분의 렌더링 영역
	D2D1_RECT_F fillRect = D2D1::RectF(
		left,         // 시작 X 좌표
		top,          // 시작 Y 좌표
		left + fillWidth, // 끝 X 좌표는 HP 비율에 따라 조정
		down          // 끝 Y 좌표
	);
	// 6. 원본 이미지에서 잘라낼 영역을 설정합니다.
	D2D1_SIZE_F bitmapSize = m_pBitmap->GetSize();
	D2D1_RECT_F sourceRect = D2D1::RectF(
		0,                           // 원본 이미지의 X 시작점
		0,                           // 원본 이미지의 Y 시작점
		bitmapSize.width * m_fRatio, // 원본 이미지의 X 끝점은 비율에 따라 조정
		bitmapSize.height            // 원본 이미지의 Y 끝점
	);

	D2D1_MATRIX_3X2_F originalMaxrix;
	_renderTarget->GetTransform(&originalMaxrix);

	if (m_pOwner->GetFlipX()) {
		float centerX = (left + right) / 2.f;
		D2D1_MATRIX_3X2_F flipMatrix =
			D2D1::Matrix3x2F::Translation(-centerX, 0) *
			D2D1::Matrix3x2F::Scale(-1.f, 1.f) *
			D2D1::Matrix3x2F::Translation(centerX, 0);

		_renderTarget->SetTransform(flipMatrix * originalMaxrix);
	}
	
	//_renderTarget->DrawBitmap(m_pBitmap, rect);
	_renderTarget->DrawBitmap(
		m_pBitmap,
		fillRect,                // 렌더링할 대상 영역 (HP 비율 적용)
		1.0f,                    // 불투명도
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		&sourceRect              // 원본 이미지에서 잘라낼 영역 (HP 비율 적용)
	);
	
	if (m_pOwner->GetFlipX()) {
		_renderTarget->SetTransform(originalMaxrix);
	}
}

