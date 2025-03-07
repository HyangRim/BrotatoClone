#include "pch.h"
#include "CImage.h"
#include "CObject.h"
#include "Direct2DMgr.h"
#include "CWeapon.h"
#include "CUI.h"


//CImage사용법
//이미지 한장을 띄우는 객체라면 vOffset설정 안해도됨
//하지만 이미지를 가지는 객체가 계층구조(parent가 존재할경우)가 있을경우 계층구조 내에서 위치를 지정해야됨
//parent중심 기준 offset을 설정할것.


/* 예제
CObject* characterImage = new CSpriteUI;
	characterImage->AddImage(image->GetBitmap());

	Vec2 vPos = Vec2(35.f, 35.f) - (panel->GetScale() / 2.f);
	characterImage->GetImage(0)->SetOffset(vPos);

	characterImage->SetObjType(GROUP_TYPE::UI);
	characterImage->SetPos(panel->GetPos());

	Image가 포함된 객체의 위치를 부모로 지정하고 offset을 설정하면됨
*/
CImage::CImage()
	: m_pOwner(nullptr)
	, m_pBitmap(nullptr)
	, m_fRatio(1.0f)
	, m_vOffSet(Vec2(0.f,0.f))
{

}

CImage::~CImage()
{

}


void CImage::finalupdate()
{
	//Vec2 vObjectPos = m_pOwner->GetPos();
	//m_vFinalPos = m_vOffSet + vObjectPos;

	//CImage를 쓴다는건 이미지를 한장 띄울 CSpriteUI를 쓴다. -> CUI*로 캐스팅해도 문제없지않을까?
	//다른 클래스에서 이걸쓸까?
	//그냥 CSpriteUI에 image컴포넌트 만들고 CSpriteUI객체를 child화 시키는게 더 낫지 않나해서

	CUI* pOwnerUI = dynamic_cast<CUI*>(m_pOwner);
	if (pOwnerUI)
	{
		m_vFinalPos = pOwnerUI->GetFinalPos() + m_vOffSet;
	}
	else
	{
		m_vFinalPos = m_pOwner->GetPos() + m_vOffSet;
	}
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

	Vec2 vPos = m_pOwner->GetFinalPos();
	//m_vFinalPos
	m_pOwner->GetRenderScale();

	//임시적으로 GROUP_TYPE이 UI거나 DEFAULT인 경우에만 RenderPos 적용 x -> 화면상에서 고정된 위치
	// -> MAIN화면의 뒷배경, START화면의 HP,XP,재화 표시 UI, (캐릭선택,무기선택)씬
	if (m_pOwner->GetObjType() == GROUP_TYPE::UI || m_pOwner->GetObjType() == GROUP_TYPE::DEFAULT)
	{
		//vPos = CCamera::GetInstance()->GetRenderPos(vPos);
	}
	else
		m_vFinalPos = CCamera::GetInstance()->GetRenderPos(m_vFinalPos);
	/////////////수정사항////////////////////////

	//Vec2 vScale = m_pOwner->GetRenderScale();
	Vec2 vScale;
	if (m_bScaleCustom)
	{
		vScale = m_pOwner->GetRenderScale() * m_vScale;
	}
	else
		vScale = m_pOwner->GetRenderScale();


	if (nullptr == m_pBitmap) return;

	float left = m_vFinalPos.x - (vScale.x / 2.f);
	float top = m_vFinalPos.y - (vScale.y / 2.f);
	float right = m_vFinalPos.x + (vScale.x / 2.f);
	float down = m_vFinalPos.y + (vScale.y / 2.f);

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


	//이 부분은 무기 전용 렌더링 코드
	CWeapon* pWeapon = dynamic_cast<CWeapon*>(m_pOwner);
	float weaponAngle = 0.f;

	if (nullptr != pWeapon) {
		CObject* pTarget = pWeapon->SpecTarget();

		if (nullptr != pTarget) {
			Vec2 WeaponPos = m_vFinalPos;
			Vec2 targetPos = pTarget->GetPos();
			targetPos = CCamera::GetInstance()->GetRenderPos(targetPos);

			Vec2 dir = targetPos - WeaponPos;

			if (dir.Length() > 0.f) {
				dir.Normalize();
				weaponAngle = std::atan2(dir.y, dir.x);
			}
		}

		D2D1_MATRIX_3X2_F originalMaxrix;
		_renderTarget->GetTransform(&originalMaxrix);

		D2D1_POINT_2F center = D2D1::Point2F(m_vFinalPos.x, m_vFinalPos.y);
		D2D1_MATRIX_3X2_F rotationMatrix = D2D1::Matrix3x2F::Rotation(weaponAngle * (180.f / PI), center);

		_renderTarget->SetTransform(rotationMatrix * originalMaxrix);

		if (m_pBitmap) {
			_renderTarget->DrawBitmap(m_pBitmap, rect);
		}

		_renderTarget->SetTransform(originalMaxrix);
	}
	else {

		//Weapon이 아닐 때 적용되는 렌더링 코드. EX)몬스터, 캐릭터 등등. 
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

}

