#include "pch.h"
#include "CMonster.h"
#include "CBirth_Monster.h"
#include "CMonFactory.h"
#include "CTimeMgr.h"



CBirth_Monster::CBirth_Monster()
	:m_fDeleteDuration(3.f)
	,m_fDeleteElapsed(0.f)
{
	CreateImage();
}

CBirth_Monster::~CBirth_Monster()
{
	CMonster* pMon = CMonFactory::CreateMonster(m_eMonType, GetPos());
	CreateObject(pMon, GROUP_TYPE::MONSTER);
}

void CBirth_Monster::update()
{
	m_fDeleteElapsed += fDT;
	if (m_fDeleteElapsed > m_fDeleteDuration) {
		m_fDeleteElapsed = 0.f;
		DeleteObject(this);
	}
}
