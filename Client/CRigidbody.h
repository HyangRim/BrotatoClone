#pragma once


class CObject;
class CRigidbody
{

private:
	CObject*	m_pOwner;		//collider�� �����ϰ� �ִ� ������Ʈ. 

	Vec2		m_vForce;		//ũ��, ����. 
	Vec2		m_vAccel;		//���ӵ�. 
	Vec2		m_vAccelA;		//���ӵ� �߰�. 
	Vec2		m_vVelocity;	//�ӵ�. (ũ�� : �ӷ�, ����)
	Vec2		m_vMaxVelocity;	//�ִ� �ӷ�. 

	float		m_fMass;		//��ü�� ����. 
	float		m_fFricCoeff;	//���� ���. 

	
public:
	void finalupdate();
	
private:
	void update_gravity();


public:
	void AddForce(Vec2 _vF) { m_vForce += _vF; }
	void SetMass(float _fMass) { m_fMass = _fMass; }
	float GetMass() { return m_fMass; }
	float GetSpeed() { return m_vVelocity.Length(); }
	Vec2 GetVelocity() { return m_vVelocity; }

	void SetVelocity(Vec2 _v) { m_vVelocity = _v; }
	void AddVelocity(Vec2 _v) { m_vVelocity += _v; }
	void SetMaxVelocity(Vec2 _vVelocity) { m_vMaxVelocity = _vVelocity; }
	void SetAccelAlpha(Vec2 _vAccel){ m_vAccelA = _vAccel; }

public:
	CRigidbody();
	~CRigidbody();

private:
	void Move();

public:
	friend class CObject;
};

