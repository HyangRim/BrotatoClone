#include "pch.h"
#include "CCamera.h"

#include "CObject.h"
#include "CCore.h"


#include "CCore.h"
#include "CkeyMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"
#include "CTexture.h"


CCamera::CCamera() 
	: m_pTargetObj(nullptr)
	, m_fTime(0.5f)
	, m_fSpeed(0.f)
	, m_fAccTime(0.5f)
	, m_pVeilTex(nullptr)
{
}

CCamera::~CCamera() {

}




void CCamera::init()
{
	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	m_pVeilTex = CResMgr::GetInstance()->CreateTexture(L"cameraVeil", (UINT)vResolution.x, (UINT)vResolution.y);
}

void CCamera::update()
{
	if (m_pTargetObj != nullptr) {
		if (m_pTargetObj->IsDead()) m_pTargetObj = nullptr;
		m_vLookAt = m_pTargetObj->GetPos();
	}
	
	if (KEY_HOLD(KEY::UP)) {
		m_vLookAt.y -= 500.f * fDT;
	}
	if (KEY_HOLD(KEY::DOWN)) {
		m_vLookAt.y += 500.f * fDT;
	}
	if (KEY_HOLD(KEY::RIGHT)) {
		m_vLookAt.x += 500.f * fDT;
	}
	if (KEY_HOLD(KEY::LEFT)) {
		m_vLookAt.x -= 500.f * fDT;
	}

	//화면 중앙좌표와 카메라 LootAt 좌표간의 차이 값. 
	CalDiff();


}

void CCamera::render(HDC _dc)
{
	if (m_listCamEffect.empty()) return;


	//시간 누적값을 체크해서.
	tCamEffect& effect = m_listCamEffect.front();
	effect.fCurTime += fDT;
	

	float fRatio = 0.f;
	fRatio = effect.fCurTime / effect.fDuration;

	if (fRatio < 0.f)
		fRatio = 0.f;
	if (fRatio > 1.f)
		fRatio = 1.f;

	int iAlpha = 0;
	if (CAM_EFFECT::FADE_OUT == effect.eEffect) {
		iAlpha = (int)(255.f * fRatio);
	}
	else if (CAM_EFFECT::FADE_IN == effect.eEffect) {
		iAlpha = (int)(255.f * (1.f - fRatio));
	}
	
	BLENDFUNCTION bf = {};

	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = 0;
	bf.SourceConstantAlpha = iAlpha;

	AlphaBlend(_dc, 0, 0
		, (int)m_pVeilTex->Width(), (int)m_pVeilTex->Height(),
		m_pVeilTex->GetDC(),
		0, 0
		,(int)m_pVeilTex->Width(), (int)m_pVeilTex->Height(), bf);
	

	//이펙트 진행 비율. 
	if (effect.fDuration < effect.fCurTime) {
		m_listCamEffect.pop_front();
	}
}

void CCamera::render(Gdiplus::Graphics* _pDGraphics)
{
}


void CCamera::CalDiff()
{
	//이전 LookAt과 현재 Look의 차이값을 보정해서 현재의 LookAt을 구한다.
	//방향 벡터.

	m_fAccTime += fDT;
	if (m_fAccTime >= m_fTime) {
		m_vCurLookAt = m_vLookAt;
	}
	else {
		Vec2 vLookDir = m_vLookAt - m_prevLookAt;

		if (!vLookDir.IsZero()) {
			m_vCurLookAt = m_prevLookAt + vLookDir.Normalize() * m_fSpeed * fDT;
		}
	}

	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	Vec2 vCenter = vResolution / 2;


	//
	m_vDiff = m_vCurLookAt - vCenter;
	m_prevLookAt = m_vCurLookAt;
}

//1. 최적화 
//카메라가 찍는 범위들의 타일들만 렌더링 되도록.

