#pragma once

struct tCharacterInfo
{
	wstring         m_sName;			// �÷��̾� �̸�. 
	CHAR_TYPE		m_eType;			// �÷��̾� Ÿ��

	float			m_fDefaultSpeed;	// �⺻ ���ǵ�. 

	int				m_AddMaxHP;			// + �ִ� ü��
	float			m_fDamageCoef;		//������ %
	float			m_fMeleeCoef;		//�ٰŸ� ������ %
	float			m_fRangeCoef;		//���Ÿ� ������ %
	float			m_fAttackSpeedCoef; // ���ݼӵ� %
	float			m_iCriticalAcc;		// ũȮ %
	float			m_fSpeed;			// �ӵ� ���.
};

tCharacterInfo NormalInfo = {
	NormalInfo.m_sName = L"����ٴ�",
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
	NormalInfo.m_sName = L"������",
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
	//�ӽ�, Ȯ�� x
	map<wstring, tCharacterInfo*> m_characterStatData;
public:
	tCharacterInfo* GetCharacterInfo(const wstring& tag)
	{
		return m_characterStatData[tag];
	}

public:
	void init();
};

