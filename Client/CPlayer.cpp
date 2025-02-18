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

#include "CRigidbody.h"
#include "CGravity.h"

CPlayer::CPlayer()
	: m_eCurState(PLAYER_STATE::IDLE)
	, m_ePrevState(PLAYER_STATE::IDLE)
	, m_iDir(1)
	, m_iPrevDir(1)
{
	//m_pTex = CResMgr::GetInstance()->LoadTexture(L"PlayerTex", L"texture\\Tenshi.bmp");


	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 15.f));
	GetCollider()->SetScale(Vec2(25.f, 25.f));

	CreateRigidBody();

	CTexture* m_pTex = CResMgr::GetInstance()->LoadTexture(L"PlayerTex", L"texture\\link_0.bmp");

	//�츮 �ؽ��İ� �Ϻ��ϰ� ������ �Ǿ� �־ ������ �����ϰ� �� �� �־���...
	//�ٵ� �׷��� ���� ��찡 ���� ����. 

	
	CreateAnimator();

	
	GetAnimator()->LoadAnimation(L"animation\\player_idle_down.anim");
	GetAnimator()->LoadAnimation(L"animation\\player_idle_left.anim");
	GetAnimator()->LoadAnimation(L"animation\\player_idle_up.anim");
	GetAnimator()->LoadAnimation(L"animation\\player_idle_right.anim");

	GetAnimator()->LoadAnimation(L"animation\\player_walk_down.anim");
	GetAnimator()->LoadAnimation(L"animation\\player_walk_left.anim");
	GetAnimator()->LoadAnimation(L"animation\\player_walk_up.anim");
	GetAnimator()->LoadAnimation(L"animation\\player_walk_right.anim");
	

	//IDLE �ִϸ��̼� �߰�
	
	/*
	GetAnimator()->CreateAnimation(L"IDLE_DOWN", m_pTex, Vec2(0.f, 0.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.1f, 3);
	GetAnimator()->CreateAnimation(L"IDLE_LEFT", m_pTex, Vec2(0.f, 65.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.1f, 3);
	GetAnimator()->CreateAnimation(L"IDLE_UP", m_pTex, Vec2(0.f, 130.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.1f, 1);
	GetAnimator()->CreateAnimation(L"IDLE_RIGHT", m_pTex, Vec2(0.f, 195.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.1f, 3);
	
	//�ȱ� �ִϸ��̼� �߰�
	GetAnimator()->CreateAnimation(L"WALK_DOWN", m_pTex, Vec2(0.f, 260.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.1f, 10);
	GetAnimator()->CreateAnimation(L"WALK_LEFT", m_pTex, Vec2(0.f, 325.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.1f, 10);
	GetAnimator()->CreateAnimation(L"WALK_UP", m_pTex, Vec2(0.f, 390.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.1f, 10);
	GetAnimator()->CreateAnimation(L"WALK_RIGHT", m_pTex, Vec2(0.f, 455.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.1f, 10);
	*/
	
	//Animation �����غ���
	GetAnimator()->Play(L"IDLE_DOWN", true);

	
	GetAnimator()->FindAnimation(L"IDLE_DOWN")->Save(L"animation\\player_idle_down.anim");
	GetAnimator()->FindAnimation(L"IDLE_LEFT")->Save(L"animation\\player_idle_left.anim");
	GetAnimator()->FindAnimation(L"IDLE_UP")->Save(L"animation\\player_idle_up.anim");
	GetAnimator()->FindAnimation(L"IDLE_RIGHT")->Save(L"animation\\player_idle_right.anim");

	GetAnimator()->FindAnimation(L"WALK_DOWN")->Save(L"animation\\player_walk_down.anim");
	GetAnimator()->FindAnimation(L"WALK_LEFT")->Save(L"animation\\player_walk_left.anim");
	GetAnimator()->FindAnimation(L"WALK_UP")->Save(L"animation\\player_walk_up.anim");
	GetAnimator()->FindAnimation(L"WALK_RIGHT")->Save(L"animation\\player_walk_right.anim");
	
	//CreateGravity();
}

CPlayer::~CPlayer()
{

}

void CPlayer::update()
{
	update_move();
	update_state();
	update_animation();

	Vec2 vPos = GetPos();


	if (KEY_TAP(KEY::Z)) {
		CreateMissile();
	}

	if (KEY_TAP(KEY::ENTER)) {
		SetPos(Vec2(640.f, 384.f));
	}

	GetAnimator()->update();


	//�� �ڿ� ����� ������ ���縦 ������� ���Ŀ� �߻��ϴ� ���°� ���밡��. 
	//EX) �浹, �ݶ��̴� ���. 
	m_ePrevState = m_eCurState;
	m_iPrevDir = m_iDir;
}

void CPlayer::render(HDC _dc)
{
	//������Ʈ(�浹ü, etc...) �� �ִ� ��� ����. 
	component_render(_dc);
}

void CPlayer::render(Gdiplus::Graphics* _pDGraphics)
{
	component_render(_pDGraphics);
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


	//�̰� �ٷ� �߰��ϴ� �� �ƴ϶�, �̺�Ʈ�� ����ϴ� ��. 
	CreateObject(pMissile, GROUP_TYPE::PROJ_PLAYER);
}

void CPlayer::update_move()
{
	CRigidbody* pRigid = GetRigidbody();

	if (KEY_HOLD(KEY::W)) {
		pRigid->AddForce(Vec2(0.f, -200.f));
	}
	if (KEY_HOLD(KEY::S)) {
		pRigid->AddForce(Vec2(0.f, 200.f));
	}
	if (KEY_HOLD(KEY::A)) {
		pRigid->AddForce(Vec2(-200.f, 0.f));
	}
	if (KEY_HOLD(KEY::D)) {
		pRigid->AddForce(Vec2(200.f, 0.f));
	}

	if (KEY_TAP(KEY::W)) {
		pRigid->AddVelocity(Vec2(pRigid->GetVelocity().x, -100.f));
	}
	if (KEY_TAP(KEY::S)) {
		pRigid->AddVelocity(Vec2(pRigid->GetVelocity().x, 100.f));
	}
	if (KEY_TAP(KEY::A)) {
		pRigid->AddVelocity(Vec2(-100.f, pRigid->GetVelocity().y));
	}
	if (KEY_TAP(KEY::D)) {
		pRigid->AddVelocity(Vec2(100.f, pRigid->GetVelocity().y));
	}

}

void CPlayer::update_state()
{
	CRigidbody* pRigid = GetRigidbody();

	if (KEY_TAP(KEY::W)) {
		m_iDir = 0;
		m_eCurState = PLAYER_STATE::WALK;
	}
	if (KEY_TAP(KEY::D)) {
		m_iDir = 1;
		m_eCurState = PLAYER_STATE::WALK;
	}
	if (KEY_TAP(KEY::S)) {
		m_iDir = 2;
		m_eCurState = PLAYER_STATE::WALK;
	}
	if (KEY_TAP(KEY::A)) {
		m_iDir = 3;
		m_eCurState = PLAYER_STATE::WALK;
	}

	if (0.f == pRigid->GetSpeed()) {
		m_eCurState = PLAYER_STATE::IDLE;
	}

}

void CPlayer::update_animation()
{
	if (m_ePrevState == m_eCurState && m_iPrevDir == m_iDir) return;

	switch (m_eCurState) {
	case PLAYER_STATE::IDLE: 
	{
		if (m_iDir == 0) {
			GetAnimator()->Play(L"IDLE_UP", true);
		}
		else if (m_iDir == 1) {
			GetAnimator()->Play(L"IDLE_RIGHT", true);
		}
		else if (m_iDir == 2) {
			GetAnimator()->Play(L"IDLE_DOWN", true);
		}
		else if (m_iDir == 3) {
			GetAnimator()->Play(L"IDLE_LEFT", true);
		}
	}
		break;
	case PLAYER_STATE::WALK:
		if (m_iDir == 0) {
			GetAnimator()->Play(L"WALK_UP", true);
		}
		else if (m_iDir == 1) {
			GetAnimator()->Play(L"WALK_RIGHT", true);
		}
		else if (m_iDir == 2) {
			GetAnimator()->Play(L"WALK_DOWN", true);
		}
		else if (m_iDir == 3) {
			GetAnimator()->Play(L"WALK_LEFT", true);
		}
		break;
	case PLAYER_STATE::ATTACK:

		break;
	case PLAYER_STATE::DEAD:

		break;
	}
}

void CPlayer::OnCollisionEnter(CCollider* _pOther)
{

	CObject* pOtherObj = _pOther->GetObj();
	if (L"Ground" == pOtherObj->GetName()) {
		Vec2 vPos = GetPos();
		if (vPos.y < pOtherObj->GetPos().y) {//���� ������ ���� ���� ��. 

		}
	}
}

