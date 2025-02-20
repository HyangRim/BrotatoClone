#pragma once
class CObject;
class CKnifeAI
{
private:
	CObject* m_pOwner;
	Vec2	 m_vTargetPos;
	Vec2	 m_vOriginPos;

public:
	void SetOwner(CObject* _pOwner) {
		m_pOwner = _pOwner;
	}
	void SetTargetPos(Vec2 _vPos) {
		m_vTargetPos = _vPos;
	}
	void SetOriginPos(Vec2 _vPos) {
		m_vOriginPos = _vPos;
	}

public:
	void update(float _f);


public:
	CKnifeAI();
	~CKnifeAI();
};

