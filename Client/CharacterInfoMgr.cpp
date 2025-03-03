#include "pch.h"
#include "CharacterInfoMgr.h"
CharacterInfoMgr::CharacterInfoMgr()
{

}

CharacterInfoMgr::~CharacterInfoMgr()
{

}

void CharacterInfoMgr::init()
{
	/*
	tCharacterInfo NormalInfo = {
	NormalInfo.m_sName = L"다재다능",
	NormalInfo.m_eType = CHAR_TYPE::WELL_ROUNDED,
	NormalInfo.m_fDefaultSpeed = 100.f,
	NormalInfo.m_AddMaxHP = 5,
	NormalInfo.m_fDamageCoef = 0.f,
	NormalInfo.m_fMeleeCoef = 1.f,
	NormalInfo.m_fRangeCoef = 1.f,
	NormalInfo.m_fAttackSpeedCoef = 1.f,
	NormalInfo.m_iCriticalAcc = 1.f,
	NormalInfo.m_fSpeed = 1.f
	};*/

	tCharacterInfo* NormalInfo = new tCharacterInfo;
	NormalInfo->m_sName = L"다재다능";
	NormalInfo->m_eType = CHAR_TYPE::WELL_ROUNDED;
	NormalInfo->m_fDefaultSpeed = 100.f;
	NormalInfo->m_AddMaxHP = 5;
	NormalInfo->m_fDamageCoef = 0.f;
	NormalInfo->m_fMeleeCoef = 1.f;
	NormalInfo->m_fRangeCoef = 1.f;
	NormalInfo->m_fAttackSpeedCoef = 1.f;
	NormalInfo->m_iCriticalAcc = 1.f;
	NormalInfo->m_fSpeed = 1.f;

	m_characterStatData.insert(make_pair(L"well_rounded", NormalInfo));
}