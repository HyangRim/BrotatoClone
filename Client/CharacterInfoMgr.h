#pragma once

struct tCharacterInfo
{
	wstring         m_sName;			// 플레이어 이름. 
	CHAR_TYPE		m_eType;			// 플레이어 타입

	float			m_fDefaultSpeed;	// 기본 스피드. 

	int				m_AddMaxHP;			// + 최대 체력
	float			m_fDamageCoef;		//최종뎀 %
	float			m_fMeleeCoef;		//근거리 최종뎀 %
	float			m_fRangeCoef;		//원거리 최종뎀 %
	float			m_fAttackSpeedCoef; // 공격속도 %
	float			m_iCriticalAcc;		// 크확 %
	float			m_fSpeed;			// 속도 계수.
};

class CharacterInfoMgr
{
	SINGLE(CharacterInfoMgr);

private:
	//임시, 확정 x
	map<wstring, tCharacterInfo*> m_characterStatData;

public:
	void init();
};

