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

}
