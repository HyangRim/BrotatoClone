#include "pch.h"
#include "CWalkEffect.h"
#include "CTimeMgr.h"
#include "Direct2DMgr.h"
#include "CImage.h"

constexpr const wchar_t StepParticleKey[8][20] =
{
	L"particle_1",
	L"particle_2",
	L"particle_3",
	L"particle_4",
	L"particle_5",
	L"particle_6",
	L"particle_7",
	L"particle_8"
};


CWalkEffect::CWalkEffect()
	: m_fParticleChangeCool(0.1f)
	, m_iParticleImageIndex(0)
	, m_fEffectLifeDuration(0.8f)
	, m_fEffectLifeElapsed(0.f)
	, m_vMaxScale(30.f, 30.f)
	, m_vMinScale(0.f, 0.f)
{
	SetScale(m_vMaxScale);
	CreateImage();
	AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"particle_1"));

}

CWalkEffect::~CWalkEffect()
{
}

void CWalkEffect::update()
{
	m_fParticleChangeCool -= fDT;
	m_fEffectLifeElapsed += fDT;

	Vec2 effectScale = lerp(m_vMaxScale, m_vMinScale, m_fEffectLifeElapsed / m_fEffectLifeDuration);

	SetScale(effectScale);

	

	if (m_fParticleChangeCool <= 0.f) {
		GetImage(0)->SetBitmap(Direct2DMgr::GetInstance()->GetStoredBitmap(StepParticleKey[m_iParticleImageIndex]));

		m_iParticleImageIndex = (m_iParticleImageIndex + 1) % 8;

		m_fParticleChangeCool = 0.1f;
	}

	if (m_fEffectLifeDuration <= m_fEffectLifeElapsed) {
		DeleteObject(this);
	}
}


