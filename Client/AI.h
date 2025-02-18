#pragma once

class CMonster;
class CState;
class AI
{

private:
	map<MON_STATE, CState*>		m_mapstate;
	//STATE 패턴
	CMonster*					m_pOwner;
	CState*						m_pCurState; //몬스터의 현재 상태. 


public:
	void update();

public:
	void AddState(CState* _pState);
	CState* GetState(MON_STATE _eState);
	CMonster* GetOwner() { return m_pOwner; }
	
	void ChangeState(MON_STATE _eNextState);
	void SetCurState(MON_STATE _eState);


public:
	AI();
	~AI();


	friend class CMonster;
};

