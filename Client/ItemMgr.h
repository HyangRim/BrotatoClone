#pragma once

#include "CWeapon.h"

class CImage;

struct Item
{
	//�����, �нú� ���δ� ������ �������� ����
	wstring		tag;							//�̹��� �±� ����(������â�� ǥ���� ������ �̹���)
	ITEM_TYPE	m_eItemType;					//WEAPON , PASSIVE

	//�������� �������ִ� �߰��� �ɷ�ġ��....
};



class ItemMgr
{
	SINGLE(ItemMgr);

private:

	vector<Item*>	m_vWeaponsItems;			//����� ����
	vector<Item*>	m_vPassiveItems;			//�нú�� ����
	Item*			m_basicCharacter;

public:
	vector<Item*>& GetWeaponItems() { return m_vWeaponsItems; }
	vector<Item*>& GetPassiveItems() { return m_vPassiveItems; }
	Item* GetBasicCharacter() { return m_basicCharacter; }

public:
	void AddWeapons(Item* _weapon) { m_vWeaponsItems.push_back(_weapon); }
	void AddPassive(Item* _passive) { m_vPassiveItems.push_back(_passive); }
	void SetBasicCharacter(Item* _basicCharacter) { m_basicCharacter = _basicCharacter; }

public:
	void Clear();

public:
	void init();
};

