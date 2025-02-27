#include "pch.h"
#include "CDamageUI.h"
#include "CTimeMgr.h"

CDamageUI::CDamageUI()
	: m_fDestroyDuration(0.f)
	, m_fDestoryElapsed(0.f)
	, m_fWaveDuration(0.5f)
	, m_fWaveElapsed(0.f)
	, m_fWaveLength(2.f)
	, m_vPivotPos{}
{

}

CDamageUI::~CDamageUI()
{
}

void CDamageUI::start()
{
	CObject::start();
}

void CDamageUI::update()
{
	CObject::update();

	m_fDestoryElapsed += fDT;
	m_fWaveElapsed += fDT;


	float angle = 2 * PI * m_fWaveElapsed / m_fWaveDuration;
	float delta = sin(angle);

	Vec2 vWavePos = m_vPivotPos;

	vWavePos.y = m_vPivotPos.y + (delta * m_fWaveLength);
	SetPos(vWavePos);

	if (m_fWaveElapsed > m_fWaveDuration) {
		m_fWaveElapsed = 0.f;
	}

	if (m_fDestoryElapsed > m_fDestroyDuration) {
		DeleteObject(this);
	}
}

void CDamageUI::render(ID2D1HwndRenderTarget* _pRender)
{
	CObject::render(_pRender);
}



