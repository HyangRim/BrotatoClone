#pragma once


class CObject;
class CCamera
{

	SINGLE(CCamera)

private:
	//그릴때만 현재 카메라의 좌표에 따라서 계산해서 찍힌다.
	//충돌 자체는 본 좌표계에 그대로. 

	Vec2			m_vLookAt;			//카메라가 보는 위치
	Vec2			m_vCurLookAt;		//이전 위치와 현재 위치 보간. 
	Vec2			m_prevLookAt;		//카메라가 보는 이전 프레임 위치. 
	

	CObject*		m_pTargetObj;		//카메라 타켓 오브젝트 <- 카메라가 해당 타겟을 쫒아다님. 

	Vec2			m_vDiff;			//해상도 중심위치와, 카메라 LootAt간의 차이 값. 
	float			m_fTime;			// 타겟을 따라가는데 걸리는 시간. 
	float			m_fSpeed;			//타겟을 따라가는 속도. 
	float			m_fAccTime;			//누적 시간. 

public:
	void SetLookAt(Vec2 _vLook) { 
		m_vLookAt = _vLook; 
		float fmoveDistance = (m_vLookAt - m_prevLookAt).Length();
		m_fSpeed = fmoveDistance / m_fTime;
		m_fAccTime = 0.f;
	
	}
	void SetTarget(CObject* _pTarget) { m_pTargetObj = _pTarget; }


	Vec2 GetLookAt() { return m_vCurLookAt; }
	Vec2 GetRenderPos(Vec2 _vObjPos) {
		return _vObjPos - m_vDiff;
	}

	Vec2 GetRealPos(Vec2 _vRenderPos) { { return _vRenderPos + m_vDiff; } }

public:
	void update();

private:
	void CalDiff();
};

