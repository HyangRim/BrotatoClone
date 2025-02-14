#pragma once
#include "global.h"

//오브젝트라는 건, 가장 부모격인 녀석. 오브젝트마다 성향이 있음 
//어떤 건 UI, 어떤 건 캐릭터... 

//오브젝트를 일괄적으로 관리할 녀석이 필요함. -> Scene

//이 오브젝트는 부모 클래스. -> 상속받은 오브젝트도 Scene의 업데이트 시점에
//부모쪽에 구현되어있는 update뿐만 아니라, 각 오브젝트(자식)들 마다의 update를 하고 싶음


//충돌할 오브젝트, 충돌하지 않을 오브젝트(UI, 배경등등...) <- 이런 식으로 계속 트리처럼 나누는 건 예외가 있을수도 있음. 
// 따라서 부품기반의 구조가 필요함(컴포넌트 구조)


#include "CCamera.h"

class CCollider;
class CAnimator;


class CObject
{

private:
	wstring			m_ObjName;
	Vec2			m_vPos;
	Vec2			m_vScale;

	//Component
	CCollider*		m_pCollider;
	CAnimator*		m_pAnimator;


	bool			m_bAlive;			//자기 자신이 활성화 or 비활성화. (삭제 전용)
	bool			m_bEnable;			//일시적인 활성화 or 비활성화. 

public:
	void SetPos(Vec2 _vPos) { m_vPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }

	Vec2 GetPos() { return m_vPos; }
	Vec2 GetScale() { return m_vScale; }


	void SetName(const wstring& _strName) { m_ObjName = _strName; }
	const wstring& GetName() { return m_ObjName; }


	CCollider* GetCollider() { return m_pCollider; }
	CAnimator* GetAnimator() { return m_pAnimator; }


	void CreateCollider();
	void CreateAnimator();

	virtual void OnCollision(CCollider* _pOther) {};
	virtual void OnCollisionEnter(CCollider* _pOther) {};
	virtual void OnCollisionExit(CCollider* _pOther) {};


	bool IsDead() {
		return !m_bAlive;
	}

private:
	void SetDead() { m_bAlive = false; }

public:
	virtual void update() = 0;
	virtual void finalupdate();
	virtual void render(HDC _dc);

	void component_render(HDC _dc);
	virtual CObject* Clone() = 0;

public:
	CObject();
	CObject(const CObject& _origin);
	virtual ~CObject();

	//복사 생성자의 문제. -> 단순히 복사만 하면 m_pCollider의 주소가 그대로 복사됨.
	//따라서 m_pCollider을 따로 만들어줘야 함. 
	//m_bAlive도 가져올 필요는 없음. 

	friend class CEventMgr;
};