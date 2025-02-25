#pragma once

class CMonster;

class CMonFactory
{
public:
	static CMonster* CreateMonster(MON_TYPE _eType, Vec2 _vPos);
private:


private:
	CMonFactory() {}
	~CMonFactory() {}
};

