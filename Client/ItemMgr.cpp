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
		float   m_fDefaultSpeed;    // 기본 스피드
		int     m_iAddMaxHP;         // + 최대 체력
		float   m_fDamageCoef;      // 최종 데미지 %
		float   m_fMeleeCoef;       // 근거리 최종 데미지 %
		float   m_fRangeCoef;       // 원거리 최종 데미지 %
		float   m_fAttackSpeedCoef; // 공격 속도 %
		int     m_iCriticalAcc;     // 크리티컬 확률 %
		float   m_fSpeed;           // 속도 계수
	} PassiveStats;                 // 패시브 아이템 관련 정보 (PASSIVE 타입일 때 사용)
	*/

	//////////////////////다재다능///////////////////////////////////
	Item* well_rounded = new Item(ITEM_TYPE::CHARACTER);
	well_rounded->m_sName = L"다재다능";
	well_rounded->tag = L"well_rounded";
	well_rounded->m_eItemType = ITEM_TYPE::CHARACTER;

	well_rounded->PassiveStats.m_iAddMaxHP = 3;
	well_rounded->PassiveStats.m_fDamageCoef = 1.f;
	well_rounded->PassiveStats.m_fMeleeCoef = 1.f;
	well_rounded->PassiveStats.m_fRangeCoef = 1.f;

	m_sItems.insert(make_pair(L"well_rounded", well_rounded));
	//////////////////////다재다능///////////////////////////////////

	//////////////////////레인저///////////////////////////////////
	Item* ranger = new Item(ITEM_TYPE::CHARACTER);
	ranger->m_sName = L"레인저";
	ranger->tag = L"ranger";
	ranger->m_eItemType = ITEM_TYPE::CHARACTER;

	ranger->PassiveStats.m_fAttackSpeedCoef = 5.f;

	m_sItems.insert(make_pair(L"ranger", ranger));
	//////////////////////레인저///////////////////////////////////
	
	//////////////////////마법사///////////////////////////////////
	Item* mage = new Item(ITEM_TYPE::CHARACTER);
	mage->m_sName = L"마법사";
	mage->tag = L"mage";
	mage->m_eItemType = ITEM_TYPE::CHARACTER;

	mage->PassiveStats.m_fRangeCoef = 30.f;

	m_sItems.insert(make_pair(L"mage", mage));
	//////////////////////마법사///////////////////////////////////

	//////////////////////광인///////////////////////////////////
	Item* crazy = new Item(ITEM_TYPE::CHARACTER);
	crazy->m_sName = L"광인";
	crazy->tag = L"crazy";
	crazy->m_eItemType = ITEM_TYPE::CHARACTER;

	crazy->PassiveStats.m_fDefaultSpeed = 10.f;
	crazy->PassiveStats.m_fMeleeCoef = 8.f;
	crazy->PassiveStats.m_fRangeCoef = 8.f;

	m_sItems.insert(make_pair(L"crazy", crazy));
	//////////////////////광인///////////////////////////////////

	//////////////////////뚱뚱이///////////////////////////////////
	Item* chunky = new Item(ITEM_TYPE::CHARACTER);
	chunky->m_sName = L"뚱뚱이";
	chunky->tag = L"chunky";
	chunky->m_eItemType = ITEM_TYPE::CHARACTER;

	chunky->PassiveStats.m_iAddMaxHP = 30;
	chunky->PassiveStats.m_fMeleeCoef = 5.f;

	m_sItems.insert(make_pair(L"chunky", chunky));
	//////////////////////뚱뚱이///////////////////////////////////

	//////////////////////검투사///////////////////////////////////
	Item* brawler = new Item(ITEM_TYPE::CHARACTER);
	brawler->m_sName = L"검투사";
	brawler->tag = L"brawler";
	brawler->m_eItemType = ITEM_TYPE::CHARACTER;

	brawler->PassiveStats.m_iAddMaxHP = 10;
	brawler->PassiveStats.m_fMeleeCoef = 20.f;

	m_sItems.insert(make_pair(L"brawler", brawler));
	//////////////////////검투사///////////////////////////////////
}

void ItemMgr::init_passive()
{
	Item* tree = new Item(ITEM_TYPE::PASSIVE);
	tree->m_sName = L"나무";
	tree->tag = L"tree";
	tree->m_eItemType = ITEM_TYPE::PASSIVE;
	tree->m_iBasePrice = 5;

	tree->PassiveStats.m_iAddMaxHP = 3;
	m_sItems.insert(make_pair(L"tree", tree));

	Item* lumberjack_shirt = new Item(ITEM_TYPE::PASSIVE);
	lumberjack_shirt->m_sName = L"럼버 잭 셔츠";
	lumberjack_shirt->tag = L"lumberjack_shirt";
	lumberjack_shirt->m_eItemType = ITEM_TYPE::PASSIVE;
	lumberjack_shirt->m_iBasePrice = 6;

	lumberjack_shirt->PassiveStats.m_iAddMaxHP = 3;
	m_sItems.insert(make_pair(L"lumberjack_shirt", lumberjack_shirt));

	Item* propeller_hat = new Item(ITEM_TYPE::PASSIVE);
	propeller_hat->m_sName = L"프로펠러 모자";
	propeller_hat->tag = L"propeller_hat";
	propeller_hat->m_eItemType = ITEM_TYPE::PASSIVE;
	propeller_hat->m_iBasePrice = 7;

	propeller_hat->PassiveStats.m_fDamageCoef = 1.f;
	m_sItems.insert(make_pair(L"propeller_hat", propeller_hat));

	Item* butterfly = new Item(ITEM_TYPE::PASSIVE);
	butterfly->m_sName = L"나비";
	butterfly->tag = L"butterfly";
	butterfly->m_eItemType = ITEM_TYPE::PASSIVE;
	butterfly->m_iBasePrice = 8;

	butterfly->PassiveStats.m_fDefaultSpeed = 5.f;
	butterfly->PassiveStats.m_iCriticalAcc = 5;
	m_sItems.insert(make_pair(L"butterfly", butterfly));

	Item* mushroom = new Item(ITEM_TYPE::PASSIVE);
	mushroom->m_sName = L"버섯";
	mushroom->tag = L"mushroom";
	mushroom->m_eItemType = ITEM_TYPE::PASSIVE;
	mushroom->m_iBasePrice = 9;

	mushroom->PassiveStats.m_fAttackSpeedCoef = 10.f;
	mushroom->PassiveStats.m_fDamageCoef = 3.f;
	m_sItems.insert(make_pair(L"mushroom", mushroom));

	Item* toxic_sludge = new Item(ITEM_TYPE::PASSIVE);
	toxic_sludge->m_sName = L"독성 슬러지";
	toxic_sludge->tag = L"toxic_sludge";
	toxic_sludge->m_eItemType = ITEM_TYPE::PASSIVE;
	toxic_sludge->m_iBasePrice = 10;

	toxic_sludge->PassiveStats.m_fMeleeCoef = 5.f;
	m_sItems.insert(make_pair(L"toxic_sludge", toxic_sludge));

	Item* hedgehog = new Item(ITEM_TYPE::PASSIVE);
	hedgehog->m_sName = L"고슴도치";
	hedgehog->tag = L"hedgehog";
	hedgehog->m_eItemType = ITEM_TYPE::PASSIVE;
	hedgehog->m_iBasePrice = 11;

	hedgehog->PassiveStats.m_fMeleeCoef = 2.f;
	hedgehog->PassiveStats.m_fRangeCoef = 1.f;
	hedgehog->PassiveStats.m_iAddMaxHP = 1;
	m_sItems.insert(make_pair(L"hedgehog", hedgehog));

	Item* charcoal = new Item(ITEM_TYPE::PASSIVE);
	charcoal->m_sName = L"숯";
	charcoal->tag = L"charcoal";
	charcoal->m_eItemType = ITEM_TYPE::PASSIVE;
	charcoal->m_iBasePrice = 12;

	charcoal->PassiveStats.m_fDamageCoef = 1.f;
	charcoal->PassiveStats.m_fRangeCoef = 1.f;
	m_sItems.insert(make_pair(L"charcoal", charcoal));

	Item* scared_sausage = new Item(ITEM_TYPE::PASSIVE);
	scared_sausage->m_sName = L"겁먹은 소시지";
	scared_sausage->tag = L"scared_sausage";
	scared_sausage->m_eItemType = ITEM_TYPE::PASSIVE;
	scared_sausage->m_iBasePrice = 13;

	scared_sausage->PassiveStats.m_fDamageCoef = 5.f;
	m_sItems.insert(make_pair(L"scared_sausage", scared_sausage));


	Item* weird_ghost = new Item(ITEM_TYPE::PASSIVE);
	weird_ghost->m_sName = L"이상한 유령";
	weird_ghost->tag = L"weird_ghost";
	weird_ghost->m_eItemType = ITEM_TYPE::PASSIVE;
	weird_ghost->m_iBasePrice = 14;

	weird_ghost->PassiveStats.m_iAddMaxHP = 5;
	m_sItems.insert(make_pair(L"weird_ghost", weird_ghost));

	Item* head_injury = new Item(ITEM_TYPE::PASSIVE);
	head_injury->m_sName = L"머리 부상";
	head_injury->tag = L"head_injury";
	head_injury->m_eItemType = ITEM_TYPE::PASSIVE;
	head_injury->m_iBasePrice = 15;

	head_injury->PassiveStats.m_fDamageCoef = 10.f;
	head_injury->PassiveStats.m_iAddMaxHP = 10;
	m_sItems.insert(make_pair(L"head_injury", head_injury));

	Item* baby_elephant = new Item(ITEM_TYPE::PASSIVE);
	baby_elephant->m_sName = L"아기 코끼리";
	baby_elephant->tag = L"baby_elephant";
	baby_elephant->m_eItemType = ITEM_TYPE::PASSIVE;
	baby_elephant->m_iBasePrice = 16;

	baby_elephant->PassiveStats.m_fDefaultSpeed = 5.f;
	m_sItems.insert(make_pair(L"baby_elephant", baby_elephant));


	Item* alien_worm = new Item(ITEM_TYPE::PASSIVE);
	alien_worm->m_sName = L"외계 벌레";
	alien_worm->tag = L"alien_worm";
	alien_worm->m_eItemType = ITEM_TYPE::PASSIVE;
	alien_worm->m_iBasePrice = 17;

	alien_worm->PassiveStats.m_iAddMaxHP = 2;
	alien_worm->PassiveStats.m_fDefaultSpeed = 3.f;
	m_sItems.insert(make_pair(L"alien_worm", alien_worm));

	Item* duct_tape = new Item(ITEM_TYPE::PASSIVE);
	duct_tape->m_sName = L"덕 테이프";
	duct_tape->tag = L"duct_tape";
	duct_tape->m_eItemType = ITEM_TYPE::PASSIVE;;
	duct_tape->m_iBasePrice = 1;

	duct_tape->PassiveStats.m_iCriticalAcc = 5;
	m_sItems.insert(make_pair(L"duct_tape", duct_tape));

	Item* cute_monkey = new Item(ITEM_TYPE::PASSIVE);
	cute_monkey->m_sName = L"귀여운 원숭이";
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
	knife->m_sName = L"칼";
	knife->tag = L"knife";
	knife->m_eItemType = ITEM_TYPE::WEAPON;
	knife->m_iBasePrice = 10;
	knife->m_tWeaponInfo.m_sIconImageKey = L"knife_icon";
	knife->m_tWeaponInfo.m_iDMG = 10;
	knife->m_tWeaponInfo.m_fMeleeCoef = 20.f;
	m_sItems.insert(make_pair(L"knife", knife));

	Item* pistol = new Item(ITEM_TYPE::WEAPON);
	pistol->m_sName = L"권총";
	pistol->tag = L"pistol";
	pistol->m_eItemType = ITEM_TYPE::WEAPON;
	pistol->m_iBasePrice = 11;
	pistol->m_tWeaponInfo.m_sIconImageKey = L"pistol_icon";
	pistol->m_tWeaponInfo.m_iDMG = 12;
	pistol->m_tWeaponInfo.m_fRangeCoef = 20.f;
	pistol->m_tWeaponInfo.m_fCritialAcc = 5.f;
	m_sItems.insert(make_pair(L"pistol", pistol));

	Item* slingshot = new Item(ITEM_TYPE::WEAPON);
	slingshot->m_sName = L"새총";
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
