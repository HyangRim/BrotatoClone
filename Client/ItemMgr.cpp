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
}

void ItemMgr::init()
{
	init_passive();
	init_weapon();
}

void ItemMgr::init_passive()
{
	Item* tree = new Item(ITEM_TYPE::PASSIVE);
	tree->m_sName = L"³ª¹«";
	tree->tag = L"tree";
	tree->m_eItemType = ITEM_TYPE::PASSIVE;
	tree->m_iBasePrice = 5;
	m_sItems.insert(make_pair(L"tree", tree));

	Item* lumberjack_shirt = new Item(ITEM_TYPE::PASSIVE);
	lumberjack_shirt->m_sName = L"·³¹ö Àè ¼ÅÃ÷";
	lumberjack_shirt->tag = L"lumberjack_shirt";
	lumberjack_shirt->m_eItemType = ITEM_TYPE::PASSIVE;
	lumberjack_shirt->m_iBasePrice = 6;
	m_sItems.insert(make_pair(L"lumberjack_shirt", lumberjack_shirt));

	Item* propeller_hat = new Item(ITEM_TYPE::PASSIVE);
	propeller_hat->m_sName = L"ÇÁ·ÎÆç·¯ ¸ðÀÚ";
	propeller_hat->tag = L"propeller_hat";
	propeller_hat->m_eItemType = ITEM_TYPE::PASSIVE;
	propeller_hat->m_iBasePrice = 7;
	m_sItems.insert(make_pair(L"propeller_hat", propeller_hat));

	Item* butterfly = new Item(ITEM_TYPE::PASSIVE);
	butterfly->m_sName = L"³ªºñ";
	butterfly->tag = L"butterfly";
	butterfly->m_eItemType = ITEM_TYPE::PASSIVE;
	butterfly->m_iBasePrice = 8;
	m_sItems.insert(make_pair(L"butterfly", butterfly));

	Item* mushroom = new Item(ITEM_TYPE::PASSIVE);
	mushroom->m_sName = L"¹ö¼¸";
	mushroom->tag = L"mushroom";
	mushroom->m_eItemType = ITEM_TYPE::PASSIVE;
	mushroom->m_iBasePrice = 9;
	m_sItems.insert(make_pair(L"mushroom", mushroom));

	Item* toxic_sludge = new Item(ITEM_TYPE::PASSIVE);
	toxic_sludge->m_sName = L"µ¶¼º ½½·¯Áö";
	toxic_sludge->tag = L"toxic_sludge";
	toxic_sludge->m_eItemType = ITEM_TYPE::PASSIVE;
	toxic_sludge->m_iBasePrice = 10;
	m_sItems.insert(make_pair(L"toxic_sludge", toxic_sludge));

	Item* hedgehog = new Item(ITEM_TYPE::PASSIVE);
	hedgehog->m_sName = L"°í½¿µµÄ¡";
	hedgehog->tag = L"hedgehog";
	hedgehog->m_eItemType = ITEM_TYPE::PASSIVE;
	hedgehog->m_iBasePrice = 11;
	m_sItems.insert(make_pair(L"hedgehog", hedgehog));

	Item* charcoal = new Item(ITEM_TYPE::PASSIVE);
	charcoal->m_sName = L"½¡";
	charcoal->tag = L"charcoal";
	charcoal->m_eItemType = ITEM_TYPE::PASSIVE;
	charcoal->m_iBasePrice = 12;
	m_sItems.insert(make_pair(L"charcoal", charcoal));

	Item* scared_sausage = new Item(ITEM_TYPE::PASSIVE);
	scared_sausage->m_sName = L"°Ì¸ÔÀº ¼Ò½ÃÁö";
	scared_sausage->tag = L"scared_sausage";
	scared_sausage->m_eItemType = ITEM_TYPE::PASSIVE;
	scared_sausage->m_iBasePrice = 13;
	m_sItems.insert(make_pair(L"scared_sausage", scared_sausage));


	Item* weird_ghost = new Item(ITEM_TYPE::PASSIVE);
	weird_ghost->m_sName = L"ÀÌ»óÇÑ À¯·É";
	weird_ghost->tag = L"weird_ghost";
	weird_ghost->m_eItemType = ITEM_TYPE::PASSIVE;
	weird_ghost->m_iBasePrice = 14;
	m_sItems.insert(make_pair(L"weird_ghost", weird_ghost));

	Item* head_injury = new Item(ITEM_TYPE::PASSIVE);
	head_injury->m_sName = L"¸Ó¸® ºÎ»ó";
	head_injury->tag = L"head_injury";
	head_injury->m_eItemType = ITEM_TYPE::PASSIVE;
	head_injury->m_iBasePrice = 15;
	m_sItems.insert(make_pair(L"head_injury", head_injury));

	Item* baby_elephant = new Item(ITEM_TYPE::PASSIVE);
	baby_elephant->m_sName = L"¾Æ±â ÄÚ³¢¸®";
	baby_elephant->tag = L"baby_elephant";
	baby_elephant->m_eItemType = ITEM_TYPE::PASSIVE;
	baby_elephant->m_iBasePrice = 16;
	m_sItems.insert(make_pair(L"baby_elephant", baby_elephant));


	Item* alien_worm = new Item(ITEM_TYPE::PASSIVE);
	alien_worm->m_sName = L"¿Ü°è ¹ú·¹";
	alien_worm->tag = L"alien_worm";
	alien_worm->m_eItemType = ITEM_TYPE::PASSIVE;
	alien_worm->m_iBasePrice = 17;
	m_sItems.insert(make_pair(L"alien_worm", alien_worm));

	Item* duct_tape = new Item(ITEM_TYPE::PASSIVE);
	duct_tape->m_sName = L"´ö Å×ÀÌÇÁ";
	duct_tape->tag = L"duct_tape";
	duct_tape->m_eItemType = ITEM_TYPE::PASSIVE;;
	duct_tape->m_iBasePrice = 1;
	m_sItems.insert(make_pair(L"duct_tape", duct_tape));

	Item* cute_monkey = new Item(ITEM_TYPE::PASSIVE);
	cute_monkey->m_sName = L"±Í¿©¿î ¿ø¼þÀÌ";
	cute_monkey->tag = L"cute_monkey";
	cute_monkey->m_eItemType = ITEM_TYPE::PASSIVE;
	cute_monkey->m_iBasePrice = 999;
	m_sItems.insert(make_pair(L"cute_monkey", cute_monkey));
}

void ItemMgr::init_weapon()
{
	Item* knife = new Item(ITEM_TYPE::WEAPON);
	knife->m_sName = L"Ä®";
	knife->tag = L"knife";
	knife->m_eItemType = ITEM_TYPE::WEAPON;
	knife->m_iBasePrice = 10;
	knife->m_tWeaponInfo.m_sIconImageKey = L"knife_icon";
	m_sItems.insert(make_pair(L"knife", knife));

	Item* pistol = new Item(ITEM_TYPE::WEAPON);
	pistol->m_sName = L"±ÇÃÑ";
	pistol->tag = L"pistol";
	pistol->m_eItemType = ITEM_TYPE::WEAPON;
	pistol->m_iBasePrice = 11;
	pistol->m_tWeaponInfo.m_sIconImageKey = L"pistol_icon";
	m_sItems.insert(make_pair(L"pistol", pistol));

	Item* slingshot = new Item(ITEM_TYPE::WEAPON);
	slingshot->m_sName = L"»õÃÑ";
	slingshot->tag = L"slingshot";
	slingshot->m_eItemType = ITEM_TYPE::WEAPON;
	slingshot->m_iBasePrice = 12;
	slingshot->m_tWeaponInfo.m_sIconImageKey = L"slingshot_icon";
	m_sItems.insert(make_pair(L"slingshot", slingshot));
}
