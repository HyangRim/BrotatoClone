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

class CharacterInfoMgr
{
	SINGLE(CharacterInfoMgr);

private:
	//�ӽ�, Ȯ�� x
	map<wstring, tCharacterInfo*> m_characterStatData;

public:
	void init();
};

