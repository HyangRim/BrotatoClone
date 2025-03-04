#pragma once

#include "CWeapon.h"

class CImage;

struct Item
{
	//무기던, 패시브 템인던 무조건 가져야할 정보
	wstring		tag;							//이미지 태그 정보(아이템창에 표시할 아이콘 이미지)
	ITEM_TYPE	m_eItemType;					//WEAPON , PASSIVE

	//아이템이 가질수있는 추가될 능력치들....
	float			m_fDefaultSpeed;	// 기본 스피드.
	int				m_AddMaxHP;			// + 최대 체력
	float			m_fDamageCoef;		//최종뎀 %
	float			m_fMeleeCoef;		//근거리 최종뎀 %
	float			m_fRangeCoef;		//원거리 최종뎀 %
	float			m_fAttackSpeedCoef; // 공격속도 %
	int				m_iCriticalAcc;		// 크확 %
	float			m_fSpeed;			// 속도 계수.
};



class ItemMgr
{
	SINGLE(ItemMgr);

private:
	unordered_map<wstring, Item*>	m_sItems;			//모든 아이템 정보 저장

	vector<Item*>					m_vWeaponsItems;			//무기류 저장
	vector<Item*>					m_vPassiveItems;			//패시브류 저장
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

