#include "pch.h"
#include "CObject.h"
#include "CCollider.h"
#include "CPlayer.h"
#include "CMissile.h"
#include "CPistol.h"
#include "CScene.h"
#include "CTimeMgr.h"
#include "CSceneMgr.h"
#include "Direct2DMgr.h"
#include "CSoundMgr.h"

CPistol::CPistol()
	: m_fCoolTime(0.f)
{
	tWeaponInfo	tInfo = {};

	tInfo.m_sName = L"Pistol";
	tInfo.m_eType = WEAPON_TYPE::PISTOL;
	tInfo.m_fCooldown = 1.2f;
	tInfo.m_fCritialDMG = 2.f;
	tInfo.m_fCritialAcc = 5;
	tInfo.m_fRecogRange = 400;
	tInfo.m_iPenet = 1;
	tInfo.m_iDMG = 12;

	SetInfo(tInfo);
	SetScale(Vec2(10.f, 10.f));
	m_pPlayer = (CPlayer*)CSceneMgr::GetInstance()->GetCurScene()->GetPlayer();
}

CPistol::~CPistol()
{
}
void CPistol::update()
{
	//Ÿ�� ���� �����ϱ�.
	CWeapon::update();

	m_fCoolTime += fDT;

	//��Ÿ�� �ֱ⸶�� AND Ÿ���� ���� �� Ÿ���� ���� ���� ���. 
	if (nullptr != GetTarget() && (Getinfo().m_fCooldown < m_fCoolTime)) {
		Vec2 targetPos = GetTarget()->GetPos();
		Vec2 direction = targetPos - GetPos();

		//���� ��Ÿ� �ȿ� ���� ��. 
		if (direction.Length() <= Getinfo().m_fRecogRange) {
			direction.Normalize();
			ShotMissile(direction);

			m_fCoolTime = 0.f;
		}
	}


}

void CPistol::render(Gdiplus::Graphics* _pDGraphics)
{
	Vec2 vPos = GetPos();
	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(vPos);
	Vec2 vScale = GetScale();
	Pen pen(Color(255, 0, 0, 255), 2.0f);
	_pDGraphics->DrawEllipse(&pen,
		vRenderPos.x - vScale.x / 2.f,
		vRenderPos.y - vScale.y / 2.f,
		vScale.x,
		vScale.y);
}

void CPistol::render(ID2D1HwndRenderTarget* _pRender)
{
	// ���� ��ǥ ���
	Vec2 vPos = GetPos();
	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(vPos);
	Vec2 vScale = GetScale();

	// Direct2D������ DrawEllipse �Լ��� ����մϴ�.
	// ���� �߽��� vRenderPos, ����/���� �������� ���� vScale.x/2, vScale.y/2 �� �����մϴ�.
	D2D1_ELLIPSE ellipse = D2D1::Ellipse(
		D2D1::Point2F(vRenderPos.x, vRenderPos.y),
		vScale.x / 2.f,
		vScale.y / 2.f
	);

	// ����� �귯�� ����
	ID2D1SolidColorBrush* pBrush = nullptr;
	HRESULT hr = _pRender->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Blue),
		&pBrush
	);
	if (SUCCEEDED(hr))
	{
		// �� �β� 2.0f�� Ÿ�� �׸���
		_pRender->DrawEllipse(ellipse, pBrush, 2.0f);
		pBrush->Release();
	}
}

void CPistol::ShotMissile(Vec2 _vDir)
{
	CMissile* pMissile = new CMissile;

	pMissile->SetName(L"Missile_Player");
	pMissile->SetPos(GetPos());
	pMissile->SetScale(Vec2(5.f, 5.f));
	pMissile->SetDir(_vDir);

	//���� ������ �� �Ѱ��ֱ�
	int default_damage = Getinfo().m_iDMG;
	if (random_distribution(random_generator) < Getinfo().m_fCritialAcc) {
		default_damage = static_cast<int>(static_cast<float>(default_damage) * Getinfo().m_fCritialDMG);
		pMissile->SetCritical(true);
	}
	pMissile->SetDamage(default_damage);
	pMissile->CreateImage();
	pMissile->AddImage(Direct2DMgr::GetInstance()->GetStoredBitmap(L"bullet_player"));
	

	int randomRC = distribution(rng) % 3;
	if (randomRC == 0) {
		CSoundMgr::GetInstance()->Play(L"gun_pistol_shot_01");
	}
	else if (randomRC == 1) {
		CSoundMgr::GetInstance()->Play(L"gun_pistol_shot_02");
	}
	else if (randomRC == 2) {
		CSoundMgr::GetInstance()->Play(L"gun_pistol_shot_03");
	}


	CreateObject(pMissile, GROUP_TYPE::PROJ_PLAYER);
}


