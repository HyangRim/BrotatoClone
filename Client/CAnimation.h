#pragma once


#include "global.h"
class CAnimator;
class CTexture;

//프레임마다 가지고 있을 정보.

//vLT : 좌상단. vSlice : 자를 영역. fDuration : 각 프레임에서 머무는 시간. 
//각 프레임에서 머무는 시간. 
struct tAnimFrm {
	Vec2	vLT;
	Vec2	vSlice;
	Vec2	vOffset;
	float	fDuration;
};

class CAnimation
{
private:
	wstring					m_strName;
	CAnimator*				m_pAnimator;

	CTexture*				m_pTex;			//Animation이 사용하는 텍스쳐
	vector<tAnimFrm>		m_vecFrm;		//모든 프레임 정보
	int						m_iCurFrm;		//현재 프레임. 
	float					m_fAccTime;		//시간 누적
	int						m_iFrameCount;


	bool					m_bFinish;		//애니메이션이 끝났음을 알림. 



public:
	void update();
	void render(HDC _dc);
	void Create(CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT iFrameCount);


public:
	const wstring& GetName() { return m_strName; }
	bool	IsFinish() { return m_bFinish; }

public:
	void SetName(const wstring& _strName) { m_strName = _strName; }
	void SetFrame(int _iframeIDX) {
		m_bFinish = false;
		m_iCurFrm = _iframeIDX;
		m_fAccTime = 0.f;
	}

	tAnimFrm& GetFrame(int _iIDX) {
		return m_vecFrm[_iIDX];
	}
	
	int GetMaxFrame() { return static_cast<UINT>(m_vecFrm.size()); }
public:
	CAnimation();
	~CAnimation();

	friend class CAnimator;
};

