#pragma once

#include "CWeapon.h"

class CImage;

struct Item
{
	//�����, �нú� ���δ� ������ �������� ����
	wstring		tag;							//�̹��� �±� ����(������â�� ǥ���� ������ �̹���)
	ITEM_TYPE	m_eItemType;					//WEAPON , PASSIVE

	//�������� �������ִ� �߰��� �ɷ�ġ��....
	float			m_fDefaultSpeed;	// �⺻ ���ǵ�.
	int				m_AddMaxHP;			// + �ִ� ü��
	float			m_fDamageCoef;		//������ %
	float			m_fMeleeCoef;		//�ٰŸ� ������ %
	float			m_fRangeCoef;		//���Ÿ� ������ %
	float			m_fAttackSpeedCoef; // ���ݼӵ� %
	int				m_iCriticalAcc;		// ũȮ %
	float			m_fSpeed;			// �ӵ� ���.
};



class ItemMgr
{
	SINGLE(ItemMgr);

private:
	unordered_map<wstring, Item*>	m_sItems;			//��� ������ ���� ����

	vector<Item*>					m_vWeaponsItems;			//����� ����
	vector<Item*>					m_vPassiveItems;			//�нú�� ����
	Item*							m_basicCharacter;



public:
	vector<Item*>& GetWeaponItems() { return m_vWeaponsItems; }
	vector<Item*>& GetPassiveItems() { return m_vPassiveItems; }
	Item* GetBasicCharacter() { return m_basicCharacter; }

	Item* GetItem(const wstring& _tag) { return m_sItems[_tag]; }

public:
	void AddWeapons(Item* _weapon) { m_vWeaponsItems.push_back(_weapon); }
	void AddPassive(Item* _passive) { m_vPassiveItems.push_back(_passive); }
	void SetBasicCharacter(Item* _basicCharacter) { m_basicCharacter = _basicCharacter; }

public:
	void Clear();

public:
	void init();
};

