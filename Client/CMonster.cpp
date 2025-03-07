#include "pch.h"
#include "CMonster.h"
#include "CWeapon.h"
#include "CMissile.h"
#include "CKnife.h"

#include "AI.h"
#include "CTimeMgr.h"
#include "CCollider.h"
#include "CMonFactory.h"
#include "CScene.h"
#include "CSceneMgr.h"

#include "CDamageUI.h"
#include "CTextUI.h"




CMonster::CMonster()
	: m_tInfo{}
	, m_pAI(nullptr)
{
	CreateCollider();
	GetCollider()->SetScale(Vec2(45.f,	45.f));
}

CMonster::~CMonster()
{
	if (nullptr != m_pAI)
		delete m_pAI;

	if (m_tInfo.m_eMonType != MON_TYPE::DROP_ITEM) {
		//DropItem �����. 
		Vec2 vPos = GetPos();
		CMonster* pMon = CMonFactory::CreateMonster(MON_TYPE::DROP_ITEM, vPos);
		CreateObject(pMon, GROUP_TYPE::DROP_ITEM);
	}
}

void CMonster::update()
{
	CObject::update();
	if(nullptr != m_pAI) m_pAI->update();

	//Flipüũ. 
	
	Vec2 playerPos = CSceneMgr::GetInstance()->GetCurScene()->GetPlayer()->GetPos();
	Vec2 monPos = GetPos();
	Vec2 Dir = playerPos - monPos;

	//�̹��� Flip üũ. ����ٰ� �̰� �ִ°� �³�?
	if (Dir.x > 0) {//�÷��̾ ���ͺ��� �����ʿ� ���� ��. 
		SetFlipX(false);
	}
	else {//�÷��̾ ���ͺ��� ���ʿ� ���� ��. 
		SetFlipX(true);
	}
}


void CMonster::SetAI(AI* _AI)
{
	m_pAI = _AI;
	m_pAI->m_pOwner = this;
}



void CMonster::TakeDamaged(int _iDamage)
{
}



void CMonster::OnCollisionEnter(CCollider* _pOther)
{
	CObject* pOtherObj = _pOther->GetObj();

	
	if (pOtherObj->GetName() == L"Missile_Player") {

		m_tInfo.m_iHP -= ((CMissile*)pOtherObj)->GetDamage();

		if (m_tInfo.m_iHP <= 0) {

			DeleteObject(this);
		}
	}
	else if (pOtherObj->GetName() == L"Knife") {

		auto DamageInfo = ((CKnife*)pOtherObj)->GetDamage();
		m_tInfo.m_iHP -= DamageInfo.first;
		bool IsCritical = DamageInfo.second;

		//�Ķ���� ����. 
		Vec2 objRenderScale = pOtherObj->GetRenderScale();
		wstring damage = std::to_wstring(DamageInfo.first);

		//DamageUI ������Ʈ �����. 
		CDamageUI* damageText = new CDamageUI;
		Vec2 textPos = GetPos();
		//textPos.y -= (objRenderScale.y + 3.f);
		damageText->SetPos(textPos);
		damageText->SetPivotPos();
		damageText->SetDuration(1.5f);
		if (!IsCritical) {
			damageText->CreateTextUI(damage, Vec2(-20.f, -objRenderScale.y - 10.f), Vec2(20.f, -objRenderScale.y - 5.f)
				, 16, D2D1::ColorF::White
				, true
				, 1.f, D2D1::ColorF::Black
				, FONT_TYPE::KR
				, TextUIMode::TEXT, 0);
			damageText->GetTextUI()->SetCamAffected(true);
		}
		else {
			damageText->CreateTextUI(damage, Vec2(-20.f, -objRenderScale.y - 10.f), Vec2(20.f, -objRenderScale.y - 5.f)
				, 16, D2D1::ColorF::Yellow
				, true
				, 1.f, D2D1::ColorF::Black
				, FONT_TYPE::KR
				, TextUIMode::TEXT, 0);
			damageText->GetTextUI()->SetCamAffected(true);
		}
		CreateObject(damageText, GROUP_TYPE::IMAGE);

		if (m_tInfo.m_iHP <= 0) {
			DeleteObject(this);
		}


	}
	
}


