#include "pch.h"
#include "CPlayer.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CScene_Start.h"
#include "CkeyMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"
#include "ItemMgr.h"
#include "Direct2DMgr.h"
#include "CWaveMgr.h"
#include "CWalkEffect.h"

#include "CWeapon.h"

#include "CMissile.h"
#include "CTexture.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CAnimation.h"

#include "CRigidbody.h"
#include "CGravity.h"
#include "CImage.h"

#include "CSoundMgr.h"

CPlayer::CPlayer()
	: m_eCurState(PLAYER_STATE::IDLE)
	, m_ePrevState(PLAYER_STATE::IDLE)
	, m_iDir(1)
	, m_iPrevDir(1)
	, m_tPlayerInfo{}
	, m_fStepSoundDelay(0.2f)
	, m_fUnderAttackSoundDelay(0.02f)
{
	//m_pTex = CResMgr::GetInstance()->LoadTexture(L"PlayerTex", L"texture\\Tenshi.bmp");

	//플레이어 Info초기화
	m_tPlayerInfo.m_iCoin = 0;
	m_tPlayerInfo.m_iLevel = 1;
	m_tPlayerInfo.m_iMaxHP = 3;
	m_tPlayerInfo.m_iMaxEXP = 20;
	m_tPlayerInfo.m_iCurEXP = 0;
	m_tPlayerInfo.m_iCurHP = m_tPlayerInfo.m_iMaxHP;

	CreateCollider();
	GetCollider()->SetOffsetPos(Vec2(0.f, 0.f));
	GetCollider()->SetScale(Vec2(15.f, 15.f));

	CreateRigidBody();
	

	//다리 오브젝트 추가. 

	//이미지 추가. 
	//0번
	AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"legs"));
	CImage* image = GetImage(0);
	image->ScaleCustomizing(Vec2(0.5f, 0.3f), true);
	image->SetOffset(Vec2(0.f, 15.f));
	//1번
	AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"potato"));

	//2번 눈
	//3번 입

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
	if (CScene::GetPause()) return;

	//피격음 딜레이. 
	m_fUnderAttackSoundDelay -= fDT;

	CObject::update();

	//이미지 업뎃 
	if (ItemMgr::GetInstance()->GetBasicCharacter() != nullptr && GetImageCount() <= 2)
	{
		Item* baseCharacter = ItemMgr::GetInstance()->GetBasicCharacter();

		wstring eyesTag = baseCharacter->tag + L"_eyes";
		wstring mouthTag = baseCharacter->tag + L"_mouth";

		AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(eyesTag));
		AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(mouthTag));
	}

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
		m_tPlayerInfo.m_iCoin+=10;
	}

	if (KEY_TAP(KEY::ENTER)) {
		SetPos(Vec2(640.f, 384.f));
	}

	GetAnimator()->update();


	//발걸음 소리 업데이트 관련
	if (PLAYER_STATE::WALK == m_eCurState) {
		m_fStepSoundDelay -= fDT;
		if (m_fStepSoundDelay < 0.f) {

			//0.35초마다 초기화하기. 
			PlayWalkSound();
			m_fStepSoundDelay = 0.3f;
		}
	}
	else {
		//다른 상태일 때는 발걸음 초기화. 
		m_fStepSoundDelay = 0.f;
	}

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
	//CreateObject((CObject*)_pWeapon, GROUP_TYPE::WEAPON);

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
	//동적 할당한 무기 해제. DeleteObject로 해야할수도?
	delete *targetWeaponIter;
	return true;
}

void CPlayer::PushSceneWeapons()
{
	//씬 시작할 때, 즉 StartScene으로 갈 대 이 함수 불러주면 됨. 
	for (auto weaponIter : m_listWeapon) {
		CScene* curScene = CSceneMgr::GetInstance()->GetCurScene();
		weaponIter->SetCurScene(curScene);
		curScene->AddObject(weaponIter, GROUP_TYPE::WEAPON);
	}
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
		vPos.y -= 100.f * fDT;
	}
	if (KEY_HOLD(KEY::S)) {
		vPos.y += 100.f * fDT;
	}
	if (KEY_HOLD(KEY::A)) {
		vPos.x -= 100.f * fDT;
	}
	if (KEY_HOLD(KEY::D)) {
		vPos.x += 100.f * fDT;
	}

	float speedValue = 1.f + m_tPlayerInfo.m_fSpeed;

	if (nullptr != m_tPlayerInfo.m_stCharacterInfo) {
		speedValue -= 1.f;
		speedValue += m_tPlayerInfo.m_stCharacterInfo->m_fSpeed;
	}
	
	if (KEY_HOLD(KEY::W)) {
		Vec2 moveVec = Vec2(pRigid->GetVelocity().x, -50.f) * speedValue;
		pRigid->AddVelocity(moveVec);
	}
	if (KEY_HOLD(KEY::S)) {
		Vec2 moveVec = Vec2(pRigid->GetVelocity().x, 50.f) * speedValue;
		pRigid->AddVelocity(moveVec);
	}
	if (KEY_HOLD(KEY::A)) {
		Vec2 moveVec = Vec2(-50.f, pRigid->GetVelocity().y) * speedValue;
		pRigid->AddVelocity(moveVec);
	}
	if (KEY_HOLD(KEY::D)) {
		Vec2 moveVec = Vec2(50.f, pRigid->GetVelocity().y) * speedValue;
		pRigid->AddVelocity(moveVec);
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
	if (L"Monster" == pOtherObj->GetName()) {

		if (m_fUnderAttackSoundDelay >= 0.f) return;
		int randomRC = distribution(rng) % 4;

		if (randomRC == 0) {
			CSoundMgr::GetInstance()->Play(L"underattack1");
		}
		else if (randomRC == 1) {
			CSoundMgr::GetInstance()->Play(L"underattack2");
		}
		else if (randomRC == 2) {
			CSoundMgr::GetInstance()->Play(L"underattack3");
		}
		else if (randomRC == 3) {
			CSoundMgr::GetInstance()->Play(L"underattack4");
		}

		m_tPlayerInfo.m_iCurHP -= 1;

		if (m_tPlayerInfo.m_iCurHP <= 0) {
			if (CSceneMgr::GetInstance()->IsWaveScene()) {
				//(CScene_Start*)(CSceneMgr::GetInstance()->GetCurScene());
				CScene_Start* startScene = dynamic_cast<CScene_Start*>(CSceneMgr::GetInstance()->GetCurScene());
				if (nullptr == startScene) assert(nullptr);

				startScene->SceneFailed();

				m_tPlayerInfo.m_iCurHP = m_tPlayerInfo.m_iMaxHP;
			}
		}

		m_fUnderAttackSoundDelay = 0.35f;
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
	m_tPlayerInfo.m_iMaxEXP += 10;
	m_tPlayerInfo.m_iMaxHP += 1;
	m_tPlayerInfo.m_iCurHP += 1;

	CWaveMgr::GetInstance()->PlayerLevelUp();
}

void CPlayer::PlayWalkSound()
{
	//최적화를 위해 따로 만들어야 할듯. 
	CSoundMgr::GetInstance()->PlayWalkSound();


	CObject* walkEffect = new CWalkEffect;
	walkEffect->SetPos(GetPos());
	walkEffect->SetName(L"walkEffect");
	CreateObject(walkEffect, GROUP_TYPE::GROUND);
}

void CPlayer::upgradeParameter(int upgradeIDX)
{

	if (upgradeIDX == 0) {
		m_tPlayerInfo.m_fAttackSpeedCoef += 0.05f;
		wprintf(L"Attack Speed Upgrade\n");
	}
	else if (upgradeIDX == 1) {
		m_tPlayerInfo.m_iCriticalAcc += 3;
		wprintf(L"Critical Acc Upgrade\n");
	}
	else if (upgradeIDX == 2) {
		m_tPlayerInfo.m_AddMaxHP += 3;
		m_tPlayerInfo.m_iMaxHP += 3;
		m_tPlayerInfo.m_iCurHP += 3;
		wprintf(L"MaxHP Upgrade\n");
	}
	else if (upgradeIDX == 3) {
		m_tPlayerInfo.m_fMeleeCoef += 2;
		wprintf(L"Melee Coef Upgrade\n");
	}
	else if (upgradeIDX == 4) {
		m_tPlayerInfo.m_fDamageCoef += 0.05f;
		wprintf(L"Damage Coef Upgrade\n");
	}
	else if (upgradeIDX == 5) {
		m_tPlayerInfo.m_fRangeCoef += 1;
		wprintf(L"Range Coef Upgrade\n");
	}
	else if (upgradeIDX == 6) {
		m_tPlayerInfo.m_fSpeed += 0.03f;
		wprintf(L"Speed Upgrade\n");
	}
}