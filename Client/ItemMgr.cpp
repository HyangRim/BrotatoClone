#include "pch.h"
#include "ItemMgr.h"

ItemMgr::ItemMgr()
{

}

ItemMgr::~ItemMgr()
{

}

void ItemMgr::Clear()
{
	Safe_Delete_Vec(m_vWeaponsItems);
	Safe_Delete_Vec(m_vPassiveItems);
	m_basicCharacter = nullptr;
}

void ItemMgr::init()
{
	Item* tree = new Item;
	tree->m_sName = L"³ª¹«";
	tree->tag = L"tree";
	tree->m_eItemType = ITEM_TYPE::PASSIVE;
	tree->m_AddMaxHP = 3;
	tree->m_iBasePrice = 5;
	m_sItems.insert(make_pair(L"tree", tree));

	Item* lumberjack_shirt = new Item;
	lumberjack_shirt->m_sName = L"·³¹ö Àè ¼ÅÃ÷";
	lumberjack_shirt->tag = L"lumberjack_shirt";
	lumberjack_shirt->m_eItemType = ITEM_TYPE::PASSIVE;
	lumberjack_shirt->m_AddMaxHP = 2;
	lumberjack_shirt->m_iBasePrice = 6;
	m_sItems.insert(make_pair(L"lumberjack_shirt", lumberjack_shirt));

	Item* propeller_hat = new Item;
	propeller_hat->m_sName = L"ÇÁ·ÎÆç·¯ ¸ðÀÚ";
	propeller_hat->tag = L"propeller_hat";
	propeller_hat->m_eItemType = ITEM_TYPE::PASSIVE;
	propeller_hat->m_fDamageCoef = 1.f;
	propeller_hat->m_iBasePrice = 7;
	m_sItems.insert(make_pair(L"propeller_hat", propeller_hat));

	Item* butterfly = new Item;
	butterfly->m_sName = L"³ªºñ";
	butterfly->tag = L"butterfly";
	butterfly->m_eItemType = ITEM_TYPE::PASSIVE;
	butterfly->m_iCriticalAcc = -5;
	butterfly->m_fSpeed = 5.f;
	butterfly->m_iBasePrice = 8;
	m_sItems.insert(make_pair(L"butterfly", butterfly));

	Item* mushroom = new Item;
	mushroom->m_sName = L"¹ö¼¸";
	mushroom->tag = L"mushroom";
	mushroom->m_eItemType = ITEM_TYPE::PASSIVE;
	mushroom->m_fAttackSpeedCoef = 10.f;
	mushroom->m_fDamageCoef = -3.f;
	mushroom->m_iBasePrice = 9;
	m_sItems.insert(make_pair(L"mushroom", mushroom));

	Item* toxic_sludge = new Item;
	toxic_sludge->m_sName = L"µ¶¼º ½½·¯Áö";
	toxic_sludge->tag = L"toxic_sludge";
	toxic_sludge->m_eItemType = ITEM_TYPE::PASSIVE;
	toxic_sludge->m_fMeleeCoef = 5.f;
	toxic_sludge->m_iBasePrice = 10;
	m_sItems.insert(make_pair(L"toxic_sludge", toxic_sludge));

	Item* hedgehog = new Item;
	hedgehog->m_sName = L"°í½¿µµÄ¡";
	hedgehog->tag = L"hedgehog";
	hedgehog->m_eItemType = ITEM_TYPE::PASSIVE;
	hedgehog->m_fMeleeCoef = 5.f;
	hedgehog->m_iBasePrice = 11;
	m_sItems.insert(make_pair(L"hedgehog", hedgehog));

	Item* charcoal = new Item;
	charcoal->m_sName = L"½¡";
	charcoal->tag = L"charcoal";
	charcoal->m_eItemType = ITEM_TYPE::PASSIVE;
	charcoal->m_fMeleeCoef = 5.f;
	charcoal->m_iBasePrice = 12;
	m_sItems.insert(make_pair(L"charcoal", charcoal));

	Item* scared_sausage = new Item;
	scared_sausage->m_sName = L"°Ì¸ÔÀº ¼Ò½ÃÁö";
	scared_sausage->tag = L"scared_sausage";
	scared_sausage->m_eItemType = ITEM_TYPE::PASSIVE;
	scared_sausage->m_fMeleeCoef = 5.f;
	scared_sausage->m_iBasePrice = 13;
	m_sItems.insert(make_pair(L"scared_sausage", scared_sausage));


	Item* weird_ghost = new Item;
	weird_ghost->m_sName = L"ÀÌ»óÇÑ À¯·É";
	weird_ghost->tag = L"weird_ghost";
	weird_ghost->m_eItemType = ITEM_TYPE::PASSIVE;
	weird_ghost->m_fMeleeCoef = 5.f;
	weird_ghost->m_iBasePrice = 14;
	m_sItems.insert(make_pair(L"weird_ghost", weird_ghost));

	Item* head_injury = new Item;
	head_injury->m_sName = L"¸Ó¸® ºÎ»ó";
	head_injury->tag = L"head_injury";
	head_injury->m_eItemType = ITEM_TYPE::PASSIVE;
	head_injury->m_fMeleeCoef = 5.f;
	head_injury->m_iBasePrice = 15;
	m_sItems.insert(make_pair(L"head_injury", head_injury));

	Item* baby_elephant = new Item;
	baby_elephant->m_sName = L"¾Æ±â ÄÚ³¢¸®";
	baby_elephant->tag = L"baby_elephant";
	baby_elephant->m_eItemType = ITEM_TYPE::PASSIVE;
	baby_elephant->m_fMeleeCoef = 5.f;
	baby_elephant->m_iBasePrice = 16;
	m_sItems.insert(make_pair(L"baby_elephant", baby_elephant));


	Item* alien_worm = new Item;
	alien_worm->m_sName = L"¿Ü°è ¹ú·¹";
	alien_worm->tag = L"alien_worm";
	alien_worm->m_eItemType = ITEM_TYPE::PASSIVE;
	alien_worm->m_fMeleeCoef = 5.f;
	alien_worm->m_iBasePrice = 17;
	m_sItems.insert(make_pair(L"alien_worm", alien_worm));

	Item* duct_tape = new Item;
	duct_tape->m_sName = L"´ö Å×ÀÌÇÁ";
	duct_tape->tag = L"duct_tape";
	duct_tape->m_eItemType = ITEM_TYPE::PASSIVE;
	duct_tape->m_fMeleeCoef = 5.f;
	duct_tape->m_iBasePrice = 1;
	m_sItems.insert(make_pair(L"duct_tape", duct_tape));

	Item* cute_monkey = new Item;
	cute_monkey->m_sName = L"±Í¿©¿î ¿ø¼þÀÌ";
	cute_monkey->tag = L"cute_monkey";
	cute_monkey->m_eItemType = ITEM_TYPE::PASSIVE;
	cute_monkey->m_fMeleeCoef = 5.f;
	cute_monkey->m_iBasePrice = 999;
	m_sItems.insert(make_pair(L"cute_monkey", cute_monkey));
}
