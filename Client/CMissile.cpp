#include "pch.h"
#include "CMissile.h"

#include "CDamageUI.h"

#include "CTimeMgr.h"
#include "CCollider.h"
#include "CTextUI.h"


CMissile::CMissile()
	: m_vDir(Vec2(1.f, 0.f))
	, m_fTheta(PI / 4.f)
	, m_fSpeed(1000.f)
	, m_fLifeTime(5.f)
	, m_iDamage(12)
	, m_bIsCritial(false)
	, m_fLifeElapsedTime(0.f)
{
	m_vDir.Normalize();
	CreateCollider();
	GetCollider()->SetScale(Vec2(15.f, 15.f));
}

CMissile::~CMissile()
{

}

void CMissile::update()
{
	Vec2 vPos = GetPos();
	//일시정지, 중단점 걸었을 때 정상적으로 가기.

	vPos.x += m_fSpeed * fDT * m_vDir.x;
	vPos.y += m_fSpeed * fDT * m_vDir.y;

	m_fLifeElapsedTime += fDT;

	SetPos(vPos);

	if (m_fLifeElapsedTime > m_fLifeTime) {
		DeleteObject(this);
	}
}

void CMissile::render(HDC _dc)
{
	Vec2 vPos = GetPos();
	Vec2 vScale = GetScale();

	//부모 클래스 만들기. 
	//CObject::render(_dc);

	Ellipse(_dc, (int)(vPos.x - vScale.x / 2.f), (int)(vPos.y - vScale.y / 2.f),
		(int)(vPos.x + vScale.x / 2.f), (int)(vPos.y + vScale.y / 2.f));

	component_render(_dc);
}

void CMissile::render(Gdiplus::Graphics* _pDGraphics)
{
	Vec2 vPos = GetPos();
	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(vPos);
	Vec2 vScale = GetScale();
	Pen pen(Color(255, 0, 0, 0), 1.0f);
	_pDGraphics->DrawEllipse(&pen,
		vRenderPos.x - vScale.x / 2.f,
		vRenderPos.y - vScale.y / 2.f,
		vScale.x,
		vScale.y);

	component_render(_pDGraphics);
}

void CMissile::render(ID2D1HwndRenderTarget* _pRender)
{
	// 실제 좌표 및 스케일 값
	Vec2 vPos = GetPos();
	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(vPos);
	Vec2 vScale = GetScale();

	// 중심이 vPos, x축 반지름은 vScale.x/2, y축 반지름은 vScale.y/2인 타원 생성
	D2D1_ELLIPSE ellipse = D2D1::Ellipse(
		D2D1::Point2F(vRenderPos.x, vRenderPos.y),
		vScale.x / 2.f,
		vScale.y / 2.f
	);

	// 브러시 생성 (예: 검은색)
	ID2D1SolidColorBrush* pBrush = nullptr;
	HRESULT hr = _pRender->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black),
		&pBrush
	);
	if (SUCCEEDED(hr))
	{
		// 선 두께 1.0f로 타원 외곽선 그리기
		_pRender->DrawEllipse(ellipse, pBrush, 1.0f);
		pBrush->Release();
	}

	// 컴포넌트 렌더링 (component_render 함수도 Direct2D로 업데이트되어 있어야 함)
	component_render(_pRender);
}

void CMissile::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();

	if (pOtherObj->GetName() == L"Monster") {

		//파라미터 세팅. 
		Vec2 objRenderScale = pOtherObj->GetRenderScale();
		wstring damage = std::to_wstring(m_iDamage);

		//DamageUI 오브젝트 만들기. 
		CDamageUI* damageText = new CDamageUI;
		Vec2 textPos = _pOther->GetObj()->GetPos();
		//textPos.y -= (objRenderScale.y + 3.f);
		damageText->SetPos(textPos);
		damageText->SetPivotPos();
		damageText->SetDuration(1.5f);
		if (!m_bIsCritial) {
			damageText->CreateTextUI(damage, Vec2(-20.f, -objRenderScale.y - 10.f), Vec2(20.f, -objRenderScale.y - 5.f)
				, 16, D2D1::ColorF::White
				, true
				, 1.f, D2D1::ColorF::Black
				, FONT_TYPE::KR
				, TextUIMode::TEXT, 0);
			damageText->GetTextUI()->SetCamAffected(true);
		}
		else {
			damageText->CreateTextUI(damage, Vec2(-20.f, -objRenderScale.y - 10.f), Vec2(20.f, -objRenderScale.y - 5.f)
				, 16, D2D1::ColorF::Yellow
				, true
				, 1.f, D2D1::ColorF::Black
				, FONT_TYPE::KR
				, TextUIMode::TEXT, 0);
			damageText->GetTextUI()->SetCamAffected(true);
		}
		CreateObject(damageText, GROUP_TYPE::IMAGE);
		//CreateObject	
		DeleteObject(this);	
	}
	
}