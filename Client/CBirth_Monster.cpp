#include "pch.h"
#include "CBirth_Monster.h"
#include "CTimeMgr.h"


CBirth_Monster::CBirth_Monster()
	:m_fDeleteDuration(3.f)
	,m_fDeleteElapsed(0.f)
{
	CreateImage();
}

CBirth_Monster::~CBirth_Monster()
{
}

void CBirth_Monster::update()
{
	m_fDeleteElapsed += fDT;
	if (m_fDeleteElapsed > m_fDeleteDuration) {
		m_fDeleteElapsed = 0.f;
		DeleteObject(this);
	}
}
