#pragma once

class CMonster;

class CMonFactory
{
public:
	static float m_fRecog;
	static float m_fSpeed;
public:
	static CMonster* CreateMonster(MON_TYPE _eType, Vec2 _vPos);
	static void SetDropItemRecog(float _fRecog) {
		m_fRecog = _fRecog;
	}

	static void SetDropItemSpeed(float _fSpeed) {
		m_fSpeed = _fSpeed;
	}

private:
	CMonFactory() {}
	~CMonFactory() {}
};

