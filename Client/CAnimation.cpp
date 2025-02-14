#include "pch.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CTimeMgr.h"
#include "CTexture.h"
#include "CObject.h"
#include "CCamera.h"


CAnimation::CAnimation()
	:m_pAnimator(nullptr)
	,m_pTex(nullptr)
	,m_iCurFrm(0)
	,m_fAccTime(0.f)
	,m_bFinish(false)
	,m_iFrameCount(0)
{

}


//발쪽으로 애니메이션을 맞춤...
//탑뷰 형태의 게임은 발쪽으로. 따라서 총알또한 그만큼 아래로 내려줌...

CAnimation::~CAnimation()
{

}

void CAnimation::update()
{
	if (m_bFinish) return;
	m_fAccTime += fDT;
	//프레임이 계속 돌아가는데, 만약 내가 한 번 재생만 원한다면?
	if (m_vecFrm[m_iCurFrm].fDuration < m_fAccTime) {

		//일시 정지해서 fDT가 2.344면 프레임 Duration 뺀 값을 넣는게 맞지 않나?
		m_iCurFrm++;
		if (m_vecFrm.size() <= m_iCurFrm) {

			//끝났다는 의미의 -1
			m_iCurFrm = -1;
			m_bFinish = true;
			m_fAccTime = 0.f;
			return;
		}
		m_fAccTime -= m_vecFrm[m_iCurFrm].fDuration;
	}

}

void CAnimation::render(HDC _dc)
{

	if (m_bFinish) return;
	CObject* pObj = m_pAnimator->GetObj();
	Vec2 vPos = pObj->GetPos();

	vPos += m_vecFrm[m_iCurFrm].vOffset;			// Object Position에 Offset만큼 추가 이동 위치. 
	vPos = CCamera::GetInstance()->GetRenderPos(vPos);
	TransparentBlt(_dc,
			(int)(vPos.x - m_vecFrm[m_iCurFrm].vSlice.x / 2.f),
			(int)(vPos.y - m_vecFrm[m_iCurFrm].vSlice.x / 2.f),
			(int)(m_vecFrm[m_iCurFrm].vSlice.x),
			(int)(m_vecFrm[m_iCurFrm].vSlice.y),
			m_pTex->GetDC(),
			(int)(m_vecFrm[m_iCurFrm].vLT.x),
			(int)(m_vecFrm[m_iCurFrm].vLT.y),
			(int)(m_vecFrm[m_iCurFrm].vSlice.x),
			(int)(m_vecFrm[m_iCurFrm].vSlice.y),
			RGB(255,0,255)
		);
	int a = 0;
}

void CAnimation::Create(CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT iFrameCount)
{
	m_pTex = _pTex;
	tAnimFrm frm = {};
	for (UINT frameIDX = 0; frameIDX < iFrameCount; frameIDX++) {
		frm.fDuration = _fDuration;
		frm.vLT = _vLT + _vStep * (float)frameIDX;
		frm.vSlice = _vSliceSize;

		m_vecFrm.push_back(frm);
	}

}

