#include "pch.h"
#include "CObject.h"

#include "CTimeMgr.h"

#include "CCollider.h"
#include "CAnimator.h"
#include "CResMgr.h"
#include "CTexture.h"
#include "CRigidbody.h"
#include "CGravity.h"
#include "CImage.h"
#include "Direct2DMgr.h"
#include "CTextUI.h"


CObject::CObject()
	: m_vPos{}
	, m_vScale{} 
	, m_pCollider(nullptr)
	, m_pAnimator(nullptr)
	, m_pRigidBody(nullptr)
	, m_pGravity(nullptr)
	, m_pImage(nullptr)
	, m_pTextUI(nullptr)
	, m_bAlive(true)
	, m_bEnable(true)
	, m_fWaveDuration(1.f)
	, m_fWaveElapsed(0.f)
	, m_bFlipX(false)
{
	m_vRenderScale = m_vScale;
}

CObject::CObject(const CObject& _origin)
	: m_ObjName(_origin.m_ObjName)
	, m_vPos(_origin.m_vPos)
	, m_vScale(_origin.m_vScale)
	, m_pCollider(nullptr)
	, m_bAlive(true)
	, m_bEnable(true)
	, m_pAnimator(nullptr)
	, m_pRigidBody(nullptr)
	, m_pGravity(nullptr)
	, m_pImage(nullptr)
	, m_pTextUI(nullptr)
	, m_bFlipX(false)
	, m_fWaveDuration(_origin.m_fWaveDuration)
	, m_fWaveElapsed(0.f)
{
	m_vRenderScale = m_vScale;
	if (_origin.m_pCollider != nullptr) {
		m_pCollider = new CCollider(*_origin.m_pCollider);
		m_pCollider->m_pOwner = this;
	}

	if (_origin.m_pAnimator != nullptr) {
		m_pAnimator = new CAnimator(*_origin.m_pAnimator);
		m_pAnimator->m_pOwner = this;
	}

	if (_origin.m_pGravity != nullptr) {
		m_pGravity = new CGravity(*_origin.m_pGravity);
		m_pGravity->m_pOwner = this;
	}

	if (_origin.m_pRigidBody != nullptr) {
		m_pRigidBody = new CRigidbody(*_origin.m_pRigidBody);
		m_pRigidBody->m_pOwner = this;
	}

	if (_origin.m_pImage != nullptr) {
		m_pImage = new CImage(*_origin.m_pImage);
		m_pImage->m_pOwner = this;
	}

	if (_origin.m_pTextUI != nullptr) {
		m_pTextUI = new CTextUI(*_origin.m_pTextUI);
		m_pTextUI->m_pOwner = this;
	}

}

CObject::~CObject() {
	if (m_pCollider != nullptr) delete m_pCollider;
	if (m_pAnimator != nullptr) delete m_pAnimator;
	if (m_pGravity != nullptr)  delete m_pGravity;
	if (m_pRigidBody != nullptr)delete m_pRigidBody;
	if (m_pImage != nullptr) delete m_pImage;
	if (m_pTextUI != nullptr) delete m_pTextUI;
}


void CObject::update()
{
	ShakeScale();

	m_vPrevPos = m_vPos;
}

void CObject::finalupdate()
{
	if (m_pAnimator) m_pAnimator->finalupdate();
	if (m_pGravity)  m_pGravity->finalupdate();
	if (m_pRigidBody)m_pRigidBody->finalupdate();
	if (m_pCollider) m_pCollider->finalupdate();
	//if( m_pImage) m_pImage->finalupdate();

}

void CObject::render(HDC _dc)
{
	//진짜 좌표.
	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(m_vPos);

	Rectangle(_dc, (int)(vRenderPos.x - m_vRenderScale.x / 2.f), (int)(vRenderPos.y - m_vRenderScale.y / 2.f),
		(int)(vRenderPos.x + m_vRenderScale.x / 2.f), (int)(vRenderPos.y + m_vRenderScale.y / 2.f));
		
	component_render(_dc);
}

void CObject::render(Gdiplus::Graphics* _pDGraphics)
{
	//진짜 좌표.
	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(m_vPos);
	Gdiplus::Pen pen(Gdiplus::Color(255, 0, 0, 0), 1.0f);
	Gdiplus::Rect rect(
		(int)(vRenderPos.x - m_vRenderScale.x / 2.f),
		(int)(vRenderPos.y - m_vRenderScale.y / 2.f),
		(int)m_vRenderScale.x,
		(int)m_vRenderScale.y
	);

	_pDGraphics->DrawRectangle(&pen, rect);
	component_render(_pDGraphics);
}

void CObject::render(ID2D1HwndRenderTarget* _pRender)
{
	//진짜 좌표.
	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(m_vPos);

	float left = vRenderPos.x - m_vRenderScale.x / 2.f;
	float top = vRenderPos.y - m_vRenderScale.y / 2.f;
	float right = vRenderPos.x + m_vRenderScale.x / 2.f;
	float bottom = vRenderPos.y + m_vRenderScale.y / 2.f;

	D2D1_RECT_F rect = D2D1::RectF(left, top, right, bottom);
	ID2D1SolidColorBrush* pBrush = nullptr;
	HRESULT hr = _pRender->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Black), &pBrush);

	if (SUCCEEDED(hr))
	{
		_pRender->DrawRectangle(rect, pBrush);
		pBrush->Release();
	}

	component_render(_pRender);
}

void CObject::component_render(HDC _dc)
{
	if (m_pAnimator != nullptr) m_pAnimator->render(_dc);

	if (m_pCollider != nullptr)	m_pCollider->render(_dc);
	if (m_pImage != nullptr)	m_pImage->render(_dc);
}

void CObject::component_render(Gdiplus::Graphics* _pDGraphics)
{
	if (m_pAnimator != nullptr) m_pAnimator->render(_pDGraphics);

	if (m_pCollider != nullptr)	m_pCollider->render(_pDGraphics);
}

void CObject::component_render(ID2D1HwndRenderTarget* _pRender)
{
	if (m_pAnimator != nullptr) m_pAnimator->render(_pRender);

	if (m_pCollider != nullptr)	m_pCollider->render(_pRender);

	if (m_pImage != nullptr)	m_pImage->render(_pRender);

	if (m_pTextUI != nullptr)	m_pTextUI->render(_pRender);
}

void CObject::CreateCollider()
{
	m_pCollider = new CCollider;

	m_pCollider->m_pOwner = this;
}

void CObject::CreateAnimator()
{
	m_pAnimator = new CAnimator;
	m_pAnimator->m_pOwner = this;
}

void CObject::CreateRigidBody()
{
	m_pRigidBody = new CRigidbody;
	m_pRigidBody->m_pOwner = this;
}

void CObject::CreateGravity()
{
	m_pGravity = new CGravity;
	m_pGravity->m_pOwner = this;
}

void CObject::CreateImage()
{
	m_pImage = new CImage;
	m_pImage->m_pOwner = this;
}

void CObject::CreateTextUI(const wstring& _text, Vec2 _offsetLT, Vec2 _offsetRB		//text , 좌상단 offset, 우하단 offset
	, int _fontSize, D2D1::ColorF _colorText										//글자크기, 글자색상(ColoF)
	, bool _bDrawOutline															//글자외곽선 여부(false라면 다 0으로 작성할것)
	, float _fOutlineThickness, D2D1::ColorF _colorOutline							//글자외곽선 두께, 글자외곽선 색상
	, FONT_TYPE _eType																//폰트 타입(DEFAULT, KR)
	, TextUIMode _eMode, int _iStartNum)											//(TEXT,NUMBER)모드 , NUMBER모드일 경우 감소시작할 숫자(타이머))
{
	m_pTextUI = new CTextUI;
	m_pTextUI->m_pOwner = this;

	m_pTextUI->SetMode(_eMode);
	m_pTextUI->SetText(_text);

	m_pTextUI->SetFontSize(_fontSize);
	m_pTextUI->SetTextColor(_colorText);

	m_pTextUI->SetDrawOutline(_bDrawOutline);
	m_pTextUI->SetOutlineThickness(_fOutlineThickness);
	m_pTextUI->SetOutlineColor(_colorOutline);

	m_pTextUI->SetFontType(_eType);

	m_pTextUI->SetOffsetLT(_offsetLT);
	m_pTextUI->SetOffsetRB(_offsetRB);
}


void CObject::ShakeScale()
{
	Vec2 OriginalScale = GetScale();
	Vec2 ScaleWave = OriginalScale * 0.3f;
	float waveScale = 1.f;
	if (IsMove()) {
		waveScale = 1.5f;
	}
	m_fWaveElapsed += (fDT * waveScale);

	float angle = 2 * PI * m_fWaveElapsed / m_fWaveDuration;
	float delta = 0.2f * sin(angle);

	m_vRenderScale.x = m_vScale.x + ((1.f + delta) * ScaleWave.x);
	m_vRenderScale.y = m_vScale.y + ((1.f - delta) * ScaleWave.y);

	if (m_fWaveElapsed > m_fWaveDuration) {
		m_fWaveElapsed = 0.f;
	}
}