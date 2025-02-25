#include "pch.h"
#include "CMobSpawner.h"

#include "CObject.h"
#include "CBirth_Monster.h"

#include "Direct2DMgr.h"
#include "CImage.h"


void CMobSpawner::MobSpawn(MON_TYPE _eType, Vec2 _vPos)
{

	CBirth_Monster* pBirth_Mon = nullptr;
	switch (_eType) {
	case MON_TYPE::NORMAL:
	{
		pBirth_Mon = new CBirth_Monster;
		pBirth_Mon->SetMonType(_eType);
		pBirth_Mon->SetPos(_vPos);
		pBirth_Mon->SetScale(Vec2(75.f, 75.f));

		pBirth_Mon->CreateImage();
		pBirth_Mon->AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"BirthEnemy"));
		CreateObject(pBirth_Mon, GROUP_TYPE::GROUND);
	}
	break;

	case MON_TYPE::RANGE:
	{
		pBirth_Mon = new CBirth_Monster;
		pBirth_Mon->SetMonType(_eType);
		pBirth_Mon->SetPos(_vPos);
		pBirth_Mon->SetScale(Vec2(75.f, 75.f));

		pBirth_Mon->CreateImage();
		pBirth_Mon->AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"BirthEnemy"));
		CreateObject(pBirth_Mon, GROUP_TYPE::GROUND);
	}
	break;
	}


	assert(pBirth_Mon);
	return;
}