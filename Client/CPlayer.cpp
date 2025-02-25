#include "pch.h"
#include "CPlayer.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CkeyMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"
#include "Direct2DMgr.h"
#include "CWeapon.h"

#include "CMissile.h"
#include "CTexture.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

#include "CRigidbody.h"
#include "CGravity.h"
#include "CImage.h"

CPlayer::CPlayer()
	: m_eCurState(PLAYER_STATE::IDLE)
	, m_ePrevState(PLAYER_STATE::IDLE)
	, m_iDir(1)
	, m_iPrevDir(1)
	, m_tPlayerInfo{}
{
	//m_pTex = CResMgr::GetInstance()->LoadTexture(L"PlayerTex", L"texture\\Tenshi.bmp");

	//플레이어 Info초기화
	m_tPlayerInfo.m_iCoin = 0;
	m_tPlayerInfo.m_iLevel = 1;
	m_tPlayerInfo.m_iMaxHP = 10;
	m_tPlayerInfo.m_iMaxEXP = 20;
	m_tPlayerInfo.m_iCurEXP = 0;
	m_tPlayerInfo.m_iCurHP = m_tPlayerInfo.m_iMaxHP;

	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));
	GetCollider()->SetScale(Vec2(15.f, 15.f));

	CreateRigidBody();
	
	/////////////////
	
	///////////////////

	CTexture* m_pTex = CResMgr::GetInstance()->LoadTexture(L"PlayerTex", L"texture\\link_0.bmp");
	//Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"texture\\link_0.bmp",L"PlayerTex", false);
	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"texture\\entities\\player\\potato.png", L"PlayerBody", false);
	Direct2DMgr::GetInstance()->LoadAndStoreBitmap(L"texture\\entities\\player\\legs.png", L"PlayerLegs", false);
	//ID2D1Bitmap* m_pBit = Direct2DMgr::GetInstance()->GetStoredBitmap(L"PlayerTex");


	//우리 텍스쳐가 완벽하게 편집이 되어 있어서 저렇게 수월하게 할 수 있었음...
	//근데 그렇지 않은 경우가 정말 많음. 

	
	//CreateAnimator();

	
	//GetAnimator()->LoadAnimation(L"animation\\player_idle_down.anim");
	//GetAnimator()->LoadAnimation(L"animation\\player_idle_left.anim");
	//GetAnimator()->LoadAnimation(L"animation\\player_idle_up.anim");
	//GetAnimator()->LoadAnimation(L"animation\\player_idle_right.anim");

	//GetAnimator()->LoadAnimation(L"animation\\player_walk_down.anim");
	//GetAnimator()->LoadAnimation(L"animation\\player_walk_left.anim");
	//GetAnimator()->LoadAnimation(L"animation\\player_walk_up.anim");
	//GetAnimator()->LoadAnimation(L"animation\\player_walk_right.anim");
	

	//IDLE 애니메이션 추가
	
	/*
	GetAnimator()->CreateAnimation(L"IDLE_DOWN", m_pTex, m_pBit,Vec2(0.f, 0.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.1f, 3);
	GetAnimator()->CreateAnimation(L"IDLE_LEFT", m_pTex, m_pBit,Vec2(0.f, 65.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.1f, 3);
	GetAnimator()->CreateAnimation(L"IDLE_UP", m_pTex, m_pBit,Vec2(0.f, 130.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.1f, 1);
	GetAnimator()->CreateAnimation(L"IDLE_RIGHT", m_pTex, m_pBit, Vec2(0.f, 195.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.1f, 3);
	
	//걷기 애니메이션 추가
	GetAnimator()->CreateAnimation(L"WALK_DOWN", m_pTex, m_pBit, Vec2(0.f, 260.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.1f, 10);
	GetAnimator()->CreateAnimation(L"WALK_LEFT", m_pTex, m_pBit, Vec2(0.f, 325.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.1f, 10);
	GetAnimator()->CreateAnimation(L"WALK_UP", m_pTex, m_pBit, Vec2(0.f, 390.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.1f, 10);
	GetAnimator()->CreateAnimation(L"WALK_RIGHT", m_pTex, m_pBit, Vec2(0.f, 455.f), Vec2(60.f, 65.f), Vec2(60.f, 0.f), 0.1f, 10);
	
	
	//Animation 저장해보기
	GetAnimator()->Play(L"IDLE_DOWN", true);

	
	GetAnimator()->FindAnimation(L"IDLE_DOWN")->Save(L"animation\\player_idle_down.anim");
	GetAnimator()->FindAnimation(L"IDLE_LEFT")->Save(L"animation\\player_idle_left.anim");
	GetAnimator()->FindAnimation(L"IDLE_UP")->Save(L"animation\\player_idle_up.anim");
	GetAnimator()->FindAnimation(L"IDLE_RIGHT")->Save(L"animation\\player_idle_right.anim");

	GetAnimator()->FindAnimation(L"WALK_DOWN")->Save(L"animation\\player_walk_down.anim");
	GetAnimator()->FindAnimation(L"WALK_LEFT")->Save(L"animation\\player_walk_left.anim");
	GetAnimator()->FindAnimation(L"WALK_UP")->Save(L"animation\\player_walk_up.anim");
	GetAnimator()->FindAnimation(L"WALK_RIGHT")->Save(L"animation\\player_walk_right.anim");
	*/
	//CreateGravity();

	//다리 오브젝트 추가. 

	//이미지 추가. 
	AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"PlayerBody"));
	AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"PlayerLegs"));

	//AddImage(L"PlayerLegs");

	//GetImage()->SetBitmap(Direct2DMgr::GetInstance()->GetStoredBitmap(L"PlayerBody"));
}

CPlayer::~CPlayer()
{
	for (auto weapon : m_listWeapon) {
		delete weapon;
	}
}

void CPlayer::update()
{
	CObject::update();

	update_move();
	update_state();
	update_animation();

	Vec2 vPos = GetPos();


	if (KEY_TAP(KEY::Z)) {
		CreateMissile();
	}

	if (KEY_TAP(KEY::K))
	{
		m_tPlayerInfo.m_iCurHP--;
	}
	if (KEY_TAP(KEY::L))
	{
		m_tPlayerInfo.m_iCurEXP++;
	}
	if (KEY_TAP(KEY::J))
	{
		m_tPlayerInfo.m_iCoin++;
	}

	if (KEY_TAP(KEY::ENTER)) {
		SetPos(Vec2(640.f, 384.f));
	}

	GetAnimator()->update();


	//맨 뒤에 놔줘야 이전과 현재를 맞춰놔야 이후에 발생하는 상태가 적용가능. 
	//EX) 충돌, 콜라이더 등등. 
	m_ePrevState = m_eCurState;
	m_iPrevDir = m_iDir;
}

void CPlayer::render(HDC _dc)
{
	//컴포넌트(충돌체, etc...) 가 있는 경우 렌더. 
	component_render(_dc);
}

void CPlayer::render(Gdiplus::Graphics* _pDGraphics)
{
	component_render(_pDGraphics);
}

void CPlayer::render(ID2D1HwndRenderTarget* _pRender)
{
	component_render(_pRender);
}

bool CPlayer::AddWeapon(CWeapon* _pWeapon)
{
	if (m_listWeapon.size() >= 6) return false;
	m_listWeapon.push_back(_pWeapon);

	int weaponOffsetPosIdx = 0;
	// 무기 추가 되면서 Offset Pos 조정. 
	for (auto weaponIter : m_listWeapon) {
		weaponIter->SetWeaponOffset(weaponOffsetPos[weaponOffsetPosIdx]);
		weaponOffsetPosIdx++;
	}
	CreateObject((CObject*)_pWeapon, GROUP_TYPE::WEAPON);

	return true;
}

bool CPlayer::DeleteWeapon(CWeapon* _pWeapon)
{
	if (m_listWeapon.size() <= 1) return false;
	list<CWeapon*>::iterator targetWeaponIter;

	//무기 리스트에서 동일한 포인터값을 찾는 이터레이터 찾기.
	for (list<CWeapon*>::iterator weaponIter = m_listWeapon.begin(); weaponIter != m_listWeapon.end(); weaponIter++) {
		if (*weaponIter == _pWeapon) {
			//찾아서 그 이터레이터를 지우면서, 그 주소값 반환.
			targetWeaponIter = m_listWeapon.erase(weaponIter);
			break;
		}
	}
	//동적 할당한 무기 해제. 
	delete *targetWeaponIter;
	return true;
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

void CPlayer::update_move()
{
	CRigidbody* pRigid = GetRigidbody();

	if (nullptr == pRigid) return;
	Vec2 vPos = GetPos();

	if (KEY_HOLD(KEY::W)) {
		vPos.y -= 200.f * fDT;
		//pRigid->AddForce(Vec2(0.f, -200.f));
	}
	if (KEY_HOLD(KEY::S)) {
		vPos.y += 200.f * fDT;
		//pRigid->AddForce(Vec2(0.f, 200.f));
	}
	if (KEY_HOLD(KEY::A)) {
		vPos.x -= 200.f * fDT;
		//pRigid->AddForce(Vec2(-200.f, 0.f));
	}
	if (KEY_HOLD(KEY::D)) {
		vPos.x += 200.f * fDT;
		//pRigid->AddForce(Vec2(200.f, 0.f));
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

	SetPos(vPos);
}

void CPlayer::update_state()
{
	CRigidbody* pRigid = GetRigidbody();

	if (nullptr == pRigid) return;

	if (KEY_TAP(KEY::W)) {
		m_iDir = 0;
		m_eCurState = PLAYER_STATE::WALK;
	}
	if (KEY_TAP(KEY::D)) {
		m_iDir = 1;
		SetFlipX(false);		//X축 이미지 그대로 출력. 
		m_eCurState = PLAYER_STATE::WALK;
	}
	if (KEY_TAP(KEY::S)) {
		m_iDir = 2;
		m_eCurState = PLAYER_STATE::WALK;
	}
	if (KEY_TAP(KEY::A)) {
		m_iDir = 3;
		SetFlipX(true);			//X축 이미지 플립하여 출력. 
		m_eCurState = PLAYER_STATE::WALK;
	}

	if (0.f == pRigid->GetSpeed()) {
		m_eCurState = PLAYER_STATE::IDLE;
	}

}

void CPlayer::update_animation()
{
	if (nullptr == GetAnimator()) return;
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
		if (vPos.y < pOtherObj->GetPos().y) {//내가 땅보다 위에 있을 때. 

		}
	}
}

void CPlayer::AddExp(int _iExp)
{
	m_tPlayerInfo.m_iCurEXP += _iExp;

	if (m_tPlayerInfo.m_iCurEXP >= m_tPlayerInfo.m_iMaxEXP) {
		PlayerLevelUp();
	}
}

void CPlayer::AddCoin(int _iCoin)
{
	m_tPlayerInfo.m_iCoin += _iCoin;
}

void CPlayer::PlayerLevelUp()
{
	m_tPlayerInfo.m_iCurEXP -= m_tPlayerInfo.m_iMaxEXP;
	m_tPlayerInfo.m_iMaxHP += 1;
	m_tPlayerInfo.m_iCurHP += 1;
}

