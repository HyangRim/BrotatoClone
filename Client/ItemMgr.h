#pragma once

#include "CWeapon.h"

class CImage;

struct Item
{
	//무기던, 패시브 템인던 무조건 가져야할 정보
	wstring		tag;							//이미지 태그 정보(아이템창에 표시할 아이콘 이미지)
	ITEM_TYPE	m_eItemType;					//WEAPON , PASSIVE

	//아이템이 가질수있는 추가될 능력치들....
};



class ItemMgr
{
	SINGLE(ItemMgr);

private:

	vector<Item*>	m_vWeaponsItems;			//무기류 저장
	vector<Item*>	m_vPassiveItems;			//패시브류 저장
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

