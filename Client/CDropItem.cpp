#include "pch.h"
#include "CCollider.h"
#include "CDropItem.h"
#include "CSoundMgr.h"



CDropItem::CDropItem()
	: m_iExp(1)
	, m_iCoin(1)
{
	SetScale(Vec2(15.f, 15.f));
	GetCollider()->SetScale(Vec2(13.f, 13.f));
}

CDropItem::~CDropItem()
{
	
}

void CDropItem::update()
{
	CMonster::update();
}

void CDropItem::OnCollisionEnter(CCollider* _pOther)
{
	static std::uniform_int_distribution<int> sound_rand(1, 5);
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player") {
		//�÷��̾ ������, ����. 
		int randomDropSound = sound_rand(rng);
		wstring SoundKey = dropItemKey + std::to_wstring(randomDropSound);
		CSoundMgr::GetInstance()->Play(SoundKey);
		DeleteObject(this);
	}
}
