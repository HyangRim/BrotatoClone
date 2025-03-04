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
	tree->tag = L"tree_icon";
	tree->m_eItemType = ITEM_TYPE::PASSIVE;
	tree->m_AddMaxHP = 3;
	m_sItems.insert(make_pair(L"tree", tree));

	Item* lumberjack_shirt = new Item;
	lumberjack_shirt->tag = L"lumberjack_shirt_icon";
	lumberjack_shirt->m_eItemType = ITEM_TYPE::PASSIVE;
	lumberjack_shirt->m_AddMaxHP = 2;
	m_sItems.insert(make_pair(L"lumberjack_shirt", lumberjack_shirt));

	Item* propeller_hat = new Item;
	propeller_hat->tag = L"propeller_hat_icon";
	propeller_hat->m_eItemType = ITEM_TYPE::PASSIVE;
	propeller_hat->m_fDamageCoef = 1.f;
	m_sItems.insert(make_pair(L"propeller_hat", propeller_hat));

	Item* butterfly = new Item;
	butterfly->tag = L"butterfly_icon";
	butterfly->m_eItemType = ITEM_TYPE::PASSIVE;
	butterfly->m_iCriticalAcc = -5;
	butterfly->m_fSpeed = 5.f;
	m_sItems.insert(make_pair(L"butterfly", butterfly));

	Item* mushroom = new Item;
	mushroom->tag = L"mushroom_icon";
	mushroom->m_eItemType = ITEM_TYPE::PASSIVE;
	mushroom->m_fAttackSpeedCoef = 10.f;
	mushroom->m_fDamageCoef = -3.f;
	m_sItems.insert(make_pair(L"mushroom", mushroom));

	Item* toxic_sludge = new Item;
	toxic_sludge->tag = L"toxic_sludge_icon";
	toxic_sludge->m_eItemType = ITEM_TYPE::PASSIVE;
	toxic_sludge->m_fMeleeCoef = 5.f;
	m_sItems.insert(make_pair(L"toxic_sludge", toxic_sludge));

	Item* hedgehog = new Item;
	hedgehog->tag = L"hedgehog_icon";
	hedgehog->m_eItemType = ITEM_TYPE::PASSIVE;
	hedgehog->m_fMeleeCoef = 5.f;
	m_sItems.insert(make_pair(L"hedgehog", hedgehog));

	Item* charcoal = new Item;
	charcoal->tag = L"charcoal_icon";
	charcoal->m_eItemType = ITEM_TYPE::PASSIVE;
	charcoal->m_fMeleeCoef = 5.f;
	m_sItems.insert(make_pair(L"charcoal", charcoal));

	Item* scared_sausage = new Item;
	scared_sausage->tag = L"scared_sausage_icon";
	scared_sausage->m_eItemType = ITEM_TYPE::PASSIVE;
	scared_sausage->m_fMeleeCoef = 5.f;
	m_sItems.insert(make_pair(L"scared_sausage", scared_sausage));


	Item* weird_ghost = new Item;
	weird_ghost->tag = L"weird_ghost_icon";
	weird_ghost->m_eItemType = ITEM_TYPE::PASSIVE;
	weird_ghost->m_fMeleeCoef = 5.f;
	m_sItems.insert(make_pair(L"weird_ghost", weird_ghost));

	Item* head_injury = new Item;
	head_injury->tag = L"head_injury_icon";
	head_injury->m_eItemType = ITEM_TYPE::PASSIVE;
	head_injury->m_fMeleeCoef = 5.f;
	m_sItems.insert(make_pair(L"head_injury", head_injury));

	Item* baby_elephant = new Item;
	baby_elephant->tag = L"baby_elephant_icon";
	baby_elephant->m_eItemType = ITEM_TYPE::PASSIVE;
	baby_elephant->m_fMeleeCoef = 5.f;
	m_sItems.insert(make_pair(L"baby_elephant", baby_elephant));


	Item* alien_worm = new Item;
	alien_worm->tag = L"alien_worm_icon";
	alien_worm->m_eItemType = ITEM_TYPE::PASSIVE;
	alien_worm->m_fMeleeCoef = 5.f;
	m_sItems.insert(make_pair(L"alien_worm", alien_worm));

	Item* duct_tape = new Item;
	duct_tape->tag = L"duct_tape_icon";
	duct_tape->m_eItemType = ITEM_TYPE::PASSIVE;
	duct_tape->m_fMeleeCoef = 5.f;
	m_sItems.insert(make_pair(L"duct_tape", duct_tape));

	Item* cute_monkey = new Item;
	cute_monkey->tag = L"cute_monkey_icon";
	cute_monkey->m_eItemType = ITEM_TYPE::PASSIVE;
	cute_monkey->m_fMeleeCoef = 5.f;
	m_sItems.insert(make_pair(L"cute_monkey", cute_monkey));
}
