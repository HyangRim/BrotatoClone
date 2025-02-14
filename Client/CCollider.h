#pragma once

class CObject;
//오브젝트의 영역을 검사하는 역할을 가짐. 

//충돌체가 그냥 오브젝트의 포지션에 있다면, 충돌체가 머리에만있을 경우, 몸통에 충돌체 따로 있을 경우...
//부위별로 따로 충돌체 두는 그런 거 못해서.



//1. 충돌은 그룹별. (그래서 Scene에서도 그룹별로 나눴음...)
//ex) Player <-> Monster이렇게. 
class CCollider
{
private:
	static UINT g_iNextID;		//

	CObject*	m_pOwner;		//collider를 소유하고 있는 오브젝트. 

	Vec2		m_vOffsetPos;	//오브젝트로부터 상대적인 위치. 
	Vec2		m_vFinalPos;	//매 프레임 finalupdate에서 계산됨. 
	Vec2		m_vScale;		//충돌체의 크기. 

	UINT		m_iID;			//충돌체 고유한 ID값. 
	UINT		m_iCol;

public:

	void SetOffsetPos(Vec2 _vPos) { m_vOffsetPos = _vPos; }
	void SetScale(Vec2 _vScale) { m_vScale = _vScale; }

	Vec2 GetOffsetPos() { return m_vOffsetPos; }
	Vec2 GetScale() { return m_vScale; }

	Vec2 GetFinalPos() { return m_vFinalPos; }

	CObject* GetObj() { return m_pOwner; }

	UINT GetID() { return m_iID; }

public:

	void finalupdate();
	void render(HDC _dc);


public:
	//충돌 시점 함수. 
	void OnCollision(CCollider* _pOther);
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);


	//복사 생성자 삭제. 
	CCollider& operator = (CCollider& _origin) = delete;

public:
	CCollider();
	CCollider(const CCollider& _origin);

	
	~CCollider();

	friend class CObject;
};

