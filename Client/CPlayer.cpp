#include "pch.h"
#include "CPlayer.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CkeyMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"

#include "CMissile.h"
#include "CTexture.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

CPlayer::CPlayer()
{
	//m_pTex = CResMgr::GetInstance()->LoadTexture(L"PlayerTex", L"texture\\Tenshi.bmp");


	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 12.f));
	GetCollider()->SetScale(Vec2(25.f, 25.f));

	CTexture* m_pTex = CResMgr::GetInstance()->LoadTexture(L"PlayerTex", L"texture\\link_0.bmp");

	//우리 텍스쳐가 완벽하게 편집이 되어 있어서 저렇게 수월하게 할 수 있었음...
	//근데 그렇지 않은 경우가 정말 많음. 
	CreateAnimator();
	GetAnimator()->CreateAnimation(L"WALK_DOWN", m_pTex, Vec2(0.f, 260.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.1f, 10);
	GetAnimator()->Play(L"WALK_DOWN", true);

	CAnimation* pAnim = GetAnimator()->FindAnimation(L"WALK_DOWN");

	for (int idx = 0; idx < pAnim->GetMaxFrame(); idx++) {
		pAnim->GetFrame(idx).vOffset = Vec2(0.f, -20.f);
	}
	//pAnim->GetFrame(0).vOffset = Vec2(0.f, -20.f);
}

CPlayer::~CPlayer()
{

}

void CPlayer::update()
{
	Vec2 vPos = GetPos();
	if (KEY_HOLD(KEY::W)) {
		vPos.y -= 200.f * fDT;
	}
	if (KEY_HOLD(KEY::S)) {
		vPos.y += 200.f * fDT;
	}
	if (KEY_HOLD(KEY::A)) {
		vPos.x -= 200.f * fDT;
	}
	if (KEY_HOLD(KEY::D)) {
		vPos.x += 200.f * fDT;
	}

	if (KEY_TAP(KEY::Z)) {
		CreateMissile();
	}
	SetPos(vPos);

	GetAnimator()->update();
}

void CPlayer::render(HDC _dc)
{
	//컴포넌트(충돌체, etc...) 가 있는 경우 렌더. 
	component_render(_dc);
}

void CPlayer::CreateMissile()
{
	Vec2 vMissilePos = GetPos();
	vMissilePos.y -= GetScale().y / 2.f;

	CMissile* pMissile = new CMissile;

	pMissile->SetName(L"Missile_Player");
	pMissile->SetPos(vMissilePos);
	pMissile->SetScale(Vec2(25.f, 25.f));
	pMissile->SetDir(Vec2(0.f, -1.f));


	//이걸 바로 추가하는 게 아니라, 이벤트를 등록하는 것. 
	CreateObject(pMissile, GROUP_TYPE::PROJ_PLAYER);
}

