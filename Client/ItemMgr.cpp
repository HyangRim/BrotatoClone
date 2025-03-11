#include "pch.h"
#include "ItemMgr.h"

ItemMgr::ItemMgr()
	: m_bBaseCharacterIsAdded(false)
{

}

ItemMgr::~ItemMgr()
{

}

void ItemMgr::update()
{
	if (m_basicCharacter != nullptr && m_bBaseCharacterIsAdded == false)
	{
		m_vPassiveItems.push_back(m_basicCharacter);
		m_bBaseCharacterIsAdded = true;
	}
}

void ItemMgr::Clear()
{
	Safe_Delete_Vec(m_vWeaponsItems);
	Safe_Delete_Vec(m_vPassiveItems);
	m_basicCharacter = nullptr;
	m_bBaseCharacterIsAdded = false;
}

void ItemMgr::init()
{
	init_character();
	init_passive();
	init_weapon();
}


void ItemMgr::init_character()
{
	/*
	struct {
		float   m_fDefaultSpeed;    // �⺻ ���ǵ�
		int     m_iAddMaxHP;         // + �ִ� ü��
		float   m_fDamageCoef;      // ���� ������ %
		float   m_fMeleeCoef;       // �ٰŸ� ���� ������ %
		float   m_fRangeCoef;       // ���Ÿ� ���� ������ %
		float   m_fAttackSpeedCoef; // ���� �ӵ� %
		int     m_iCriticalAcc;     // ũ��Ƽ�� Ȯ�� %
		float   m_fSpeed;           // �ӵ� ���
	} PassiveStats;                 // �нú� ������ ���� ���� (PASSIVE Ÿ���� �� ���)
	*/

	//////////////////////����ٴ�///////////////////////////////////
	Item* well_rounded = new Item(ITEM_TYPE::CHARACTER);
	well_rounded->m_sName = L"����ٴ�";
	well_rounded->tag = L"well_rounded";
	well_rounded->m_eItemType = ITEM_TYPE::CHARACTER;

	well_rounded->PassiveStats.m_iAddMaxHP = 3;
	well_rounded->PassiveStats.m_fDamageCoef = 1.f;
	well_rounded->PassiveStats.m_fMeleeCoef = 1.f;
	well_rounded->PassiveStats.m_fRangeCoef = 1.f;

	m_sItems.insert(make_pair(L"well_rounded", well_rounded));
	//////////////////////����ٴ�///////////////////////////////////

	//////////////////////������///////////////////////////////////
	Item* ranger = new Item(ITEM_TYPE::CHARACTER);
	ranger->m_sName = L"������";
	ranger->tag = L"ranger";
	ranger->m_eItemType = ITEM_TYPE::CHARACTER;

	ranger->PassiveStats.m_fAttackSpeedCoef = 5.f;

	m_sItems.insert(make_pair(L"ranger", ranger));
	//////////////////////������///////////////////////////////////
	
	//////////////////////������///////////////////////////////////
	Item* mage = new Item(ITEM_TYPE::CHARACTER);
	mage->m_sName = L"������";
	mage->tag = L"mage";
	mage->m_eItemType = ITEM_TYPE::CHARACTER;

	mage->PassiveStats.m_fRangeCoef = 30.f;

	m_sItems.insert(make_pair(L"mage", mage));
	//////////////////////������///////////////////////////////////

	//////////////////////����///////////////////////////////////
	Item* crazy = new Item(ITEM_TYPE::CHARACTER);
	crazy->m_sName = L"����";
	crazy->tag = L"crazy";
	crazy->m_eItemType = ITEM_TYPE::CHARACTER;

	crazy->PassiveStats.m_fDefaultSpeed = 10.f;
	crazy->PassiveStats.m_fMeleeCoef = 8.f;
	crazy->PassiveStats.m_fRangeCoef = 8.f;

	m_sItems.insert(make_pair(L"crazy", crazy));
	//////////////////////����///////////////////////////////////

	//////////////////////�׶���///////////////////////////////////
	Item* chunky = new Item(ITEM_TYPE::CHARACTER);
	chunky->m_sName = L"�׶���";
	chunky->tag = L"chunky";
	chunky->m_eItemType = ITEM_TYPE::CHARACTER;

	chunky->PassiveStats.m_iAddMaxHP = 30;
	chunky->PassiveStats.m_fMeleeCoef = 5.f;

	m_sItems.insert(make_pair(L"chunky", chunky));
	//////////////////////�׶���///////////////////////////////////

	//////////////////////������///////////////////////////////////
	Item* brawler = new Item(ITEM_TYPE::CHARACTER);
	brawler->m_sName = L"������";
	brawler->tag = L"brawler";
	brawler->m_eItemType = ITEM_TYPE::CHARACTER;

	brawler->PassiveStats.m_iAddMaxHP = 10;
	brawler->PassiveStats.m_fMeleeCoef = 20.f;

	m_sItems.insert(make_pair(L"brawler", brawler));
	//////////////////////������///////////////////////////////////
}

void ItemMgr::init_passive()
{
	Item* tree = new Item(ITEM_TYPE::PASSIVE);
	tree->m_sName = L"����";
	tree->tag = L"tree";
	tree->m_eItemType = ITEM_TYPE::PASSIVE;
	tree->m_iBasePrice = 5;

	tree->PassiveStats.m_iAddMaxHP = 3;
	m_sItems.insert(make_pair(L"tree", tree));

	Item* lumberjack_shirt = new Item(ITEM_TYPE::PASSIVE);
	lumberjack_shirt->m_sName = L"���� �� ����";
	lumberjack_shirt->tag = L"lumberjack_shirt";
	lumberjack_shirt->m_eItemType = ITEM_TYPE::PASSIVE;
	lumberjack_shirt->m_iBasePrice = 6;

	lumberjack_shirt->PassiveStats.m_iAddMaxHP = 3;
	m_sItems.insert(make_pair(L"lumberjack_shirt", lumberjack_shirt));

	Item* propeller_hat = new Item(ITEM_TYPE::PASSIVE);
	propeller_hat->m_sName = L"�����緯 ����";
	propeller_hat->tag = L"propeller_hat";
	propeller_hat->m_eItemType = ITEM_TYPE::PASSIVE;
	propeller_hat->m_iBasePrice = 7;

	propeller_hat->PassiveStats.m_fDamageCoef = 1.f;
	m_sItems.insert(make_pair(L"propeller_hat", propeller_hat));

	Item* butterfly = new Item(ITEM_TYPE::PASSIVE);
	butterfly->m_sName = L"����";
	butterfly->tag = L"butterfly";
	butterfly->m_eItemType = ITEM_TYPE::PASSIVE;
	butterfly->m_iBasePrice = 8;

	butterfly->PassiveStats.m_fDefaultSpeed = 5.f;
	butterfly->PassiveStats.m_iCriticalAcc = 5;
	m_sItems.insert(make_pair(L"butterfly", butterfly));

	Item* mushroom = new Item(ITEM_TYPE::PASSIVE);
	mushroom->m_sName = L"����";
	mushroom->tag = L"mushroom";
	mushroom->m_eItemType = ITEM_TYPE::PASSIVE;
	mushroom->m_iBasePrice = 9;

	mushroom->PassiveStats.m_fAttackSpeedCoef = 10.f;
	mushroom->PassiveStats.m_fDamageCoef = 3.f;
	m_sItems.insert(make_pair(L"mushroom", mushroom));

	Item* toxic_sludge = new Item(ITEM_TYPE::PASSIVE);
	toxic_sludge->m_sName = L"���� ������";
	toxic_sludge->tag = L"toxic_sludge";
	toxic_sludge->m_eItemType = ITEM_TYPE::PASSIVE;
	toxic_sludge->m_iBasePrice = 10;

	toxic_sludge->PassiveStats.m_fMeleeCoef = 5.f;
	m_sItems.insert(make_pair(L"toxic_sludge", toxic_sludge));

	Item* hedgehog = new Item(ITEM_TYPE::PASSIVE);
	hedgehog->m_sName = L"����ġ";
	hedgehog->tag = L"hedgehog";
	hedgehog->m_eItemType = ITEM_TYPE::PASSIVE;
	hedgehog->m_iBasePrice = 11;

	hedgehog->PassiveStats.m_fMeleeCoef = 2.f;
	hedgehog->PassiveStats.m_fRangeCoef = 1.f;
	hedgehog->PassiveStats.m_iAddMaxHP = 1;
	m_sItems.insert(make_pair(L"hedgehog", hedgehog));

	Item* charcoal = new Item(ITEM_TYPE::PASSIVE);
	charcoal->m_sName = L"��";
	charcoal->tag = L"charcoal";
	charcoal->m_eItemType = ITEM_TYPE::PASSIVE;
	charcoal->m_iBasePrice = 12;

	charcoal->PassiveStats.m_fDamageCoef = 1.f;
	charcoal->PassiveStats.m_fRangeCoef = 1.f;
	m_sItems.insert(make_pair(L"charcoal", charcoal));

	Item* scared_sausage = new Item(ITEM_TYPE::PASSIVE);
	scared_sausage->m_sName = L"�̸��� �ҽ���";
	scared_sausage->tag = L"scared_sausage";
	scared_sausage->m_eItemType = ITEM_TYPE::PASSIVE;
	scared_sausage->m_iBasePrice = 13;

	scared_sausage->PassiveStats.m_fDamageCoef = 5.f;
	m_sItems.insert(make_pair(L"scared_sausage", scared_sausage));


	Item* weird_ghost = new Item(ITEM_TYPE::PASSIVE);
	weird_ghost->m_sName = L"�̻��� ����";
	weird_ghost->tag = L"weird_ghost";
	weird_ghost->m_eItemType = ITEM_TYPE::PASSIVE;
	weird_ghost->m_iBasePrice = 14;

	weird_ghost->PassiveStats.m_iAddMaxHP = 5;
	m_sItems.insert(make_pair(L"weird_ghost", weird_ghost));

	Item* head_injury = new Item(ITEM_TYPE::PASSIVE);
	head_injury->m_sName = L"�Ӹ� �λ�";
	head_injury->tag = L"head_injury";
	head_injury->m_eItemType = ITEM_TYPE::PASSIVE;
	head_injury->m_iBasePrice = 15;

	head_injury->PassiveStats.m_fDamageCoef = 10.f;
	head_injury->PassiveStats.m_iAddMaxHP = 10;
	m_sItems.insert(make_pair(L"head_injury", head_injury));

	Item* baby_elephant = new Item(ITEM_TYPE::PASSIVE);
	baby_elephant->m_sName = L"�Ʊ� �ڳ���";
	baby_elephant->tag = L"baby_elephant";
	baby_elephant->m_eItemType = ITEM_TYPE::PASSIVE;
	baby_elephant->m_iBasePrice = 16;

	baby_elephant->PassiveStats.m_fDefaultSpeed = 5.f;
	m_sItems.insert(make_pair(L"baby_elephant", baby_elephant));


	Item* alien_worm = new Item(ITEM_TYPE::PASSIVE);
	alien_worm->m_sName = L"�ܰ� ����";
	alien_worm->tag = L"alien_worm";
	alien_worm->m_eItemType = ITEM_TYPE::PASSIVE;
	alien_worm->m_iBasePrice = 17;

	alien_worm->PassiveStats.m_iAddMaxHP = 2;
	alien_worm->PassiveStats.m_fDefaultSpeed = 3.f;
	m_sItems.insert(make_pair(L"alien_worm", alien_worm));

	Item* duct_tape = new Item(ITEM_TYPE::PASSIVE);
	duct_tape->m_sName = L"�� ������";
	duct_tape->tag = L"duct_tape";
	duct_tape->m_eItemType = ITEM_TYPE::PASSIVE;;
	duct_tape->m_iBasePrice = 1;

	duct_tape->PassiveStats.m_iCriticalAcc = 5;
	m_sItems.insert(make_pair(L"duct_tape", duct_tape));

	Item* cute_monkey = new Item(ITEM_TYPE::PASSIVE);
	cute_monkey->m_sName = L"�Ϳ��� ������";
	cute_monkey->tag = L"cute_monkey";
	cute_monkey->m_eItemType = ITEM_TYPE::PASSIVE;
	cute_monkey->m_iBasePrice = 999;

	cute_monkey->PassiveStats.m_fMeleeCoef = 3.f;
	cute_monkey->PassiveStats.m_fRangeCoef = 3.f;
	m_sItems.insert(make_pair(L"cute_monkey", cute_monkey));
}

void ItemMgr::init_weapon()
{
	Item* knife = new Item(ITEM_TYPE::WEAPON);
	knife->m_sName = L"Į";
	knife->tag = L"knife";
	knife->m_eItemType = ITEM_TYPE::WEAPON;
	knife->m_iBasePrice = 10;
	knife->m_tWeaponInfo.m_sIconImageKey = L"knife_icon";
	knife->m_tWeaponInfo.m_iDMG = 10;
	knife->m_tWeaponInfo.m_fMeleeCoef = 20.f;
	m_sItems.insert(make_pair(L"knife", knife));

	Item* pistol = new Item(ITEM_TYPE::WEAPON);
	pistol->m_sName = L"����";
	pistol->tag = L"pistol";
	pistol->m_eItemType = ITEM_TYPE::WEAPON;
	pistol->m_iBasePrice = 11;
	pistol->m_tWeaponInfo.m_sIconImageKey = L"pistol_icon";
	pistol->m_tWeaponInfo.m_iDMG = 12;
	pistol->m_tWeaponInfo.m_fRangeCoef = 20.f;
	pistol->m_tWeaponInfo.m_fCritialAcc = 5.f;
	m_sItems.insert(make_pair(L"pistol", pistol));

	Item* slingshot = new Item(ITEM_TYPE::WEAPON);
	slingshot->m_sName = L"����";
	slingshot->tag = L"slingshot";
	slingshot->m_eItemType = ITEM_TYPE::WEAPON;
	slingshot->m_iBasePrice = 12;
	slingshot->m_tWeaponInfo.m_sIconImageKey = L"slingshot_icon";
	slingshot->m_tWeaponInfo.m_iDMG = 5;
	slingshot->m_tWeaponInfo.m_fRangeCoef = 20.f;
	slingshot->m_tWeaponInfo.m_fCritialAcc = 5.f;
	slingshot->m_tWeaponInfo.m_iPenet = 2;
	m_sItems.insert(make_pair(L"slingshot", slingshot));
}
