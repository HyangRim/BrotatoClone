#include "pch.h"
#include "CAnimator.h"
#include "CAnimation.h"



CAnimator::CAnimator()
	: m_pOwner(nullptr)
	, m_pCurAnim(nullptr)
	, m_mapAnim{}
	, m_bRepeat(false)
{
}

CAnimator::~CAnimator()
{
	Safe_Delete_UnordedMap(m_mapAnim);
}



void CAnimator::update()
{

}

void CAnimator::finalupdate()
{
	if (nullptr != m_pCurAnim) {
		m_pCurAnim->update();

		if (m_bRepeat && m_pCurAnim->IsFinish()) {
			m_pCurAnim->SetFrame(0);
		}
	}
}

void CAnimator::render(HDC _dc)
{
	if (nullptr != m_pCurAnim) {
		m_pCurAnim->render(_dc);
	}
}

void CAnimator::CreateAnimation(const wstring& _strName, CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT _iFrameCount)
{
	CAnimation* pAnim = FindAnimation(_strName);
	assert(nullptr == pAnim);

	pAnim = new CAnimation;

	pAnim->SetName(_strName);
	pAnim->m_pAnimator = this;
	pAnim->Create(_pTex, _vLT, _vSliceSize, _vStep, _fDuration, _iFrameCount);

	m_mapAnim.insert(make_pair(_strName, pAnim));
}

CAnimation* CAnimator::FindAnimation(const wstring& _strName)
{
	auto animIter = m_mapAnim.find(_strName);

	if (animIter == m_mapAnim.end()) return nullptr;
	
	return animIter->second;
}

void CAnimator::Play(const wstring& _strName, bool _bRepeat)
{
	m_pCurAnim = FindAnimation(_strName);
	m_bRepeat = _bRepeat;
}
