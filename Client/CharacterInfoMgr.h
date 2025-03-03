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

tCharacterInfo NormalInfo = {
	NormalInfo.m_sName = L"다재다능",
	NormalInfo.m_eType = CHAR_TYPE::WELL_ROUNDED,
	NormalInfo.m_fDefaultSpeed = 100.f,
	NormalInfo.m_AddMaxHP = 5,
	NormalInfo.m_fDamageCoef = 0.f,
	NormalInfo.m_fMeleeCoef = 1.f,
	NormalInfo.m_fRangeCoef = 1.f,
	NormalInfo.m_fAttackSpeedCoef = 1.f,
	NormalInfo.m_iCriticalAcc = 1.f,
	NormalInfo.m_fSpeed = 1.f
};
/*
tCharacterInfo NormalInfo = {
	NormalInfo.m_sName = L"레인저",
	NormalInfo.m_eType = CHAR_TYPE::RANGER,
	NormalInfo.m_fDefaultSpeed = 80.f,
	NormalInfo.m_AddMaxHP = 0,
	NormalInfo.m_fDamageCoef = 0.f,
	NormalInfo.m_fMeleeCoef = 1.f,
	NormalInfo.m_fRangeCoef = 1.5f,
	NormalInfo.m_fAttackSpeedCoef = 1.f,
	NormalInfo.m_iCriticalAcc = 1.f,
	NormalInfo.m_fSpeed = 1.f
};
*/

class CharacterInfoMgr
{
	SINGLE(CharacterInfoMgr);

private:
	//임시, 확정 x
	map<wstring, tCharacterInfo*> m_characterStatData;
public:
	tCharacterInfo* GetCharacterInfo(const wstring& tag)
	{
		return m_characterStatData[tag];
	}

public:
	void init();
};

