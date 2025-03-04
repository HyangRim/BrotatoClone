#include "pch.h"
#include "CScene.h"
#include "CSceneMgr.h"
#include "CObject.h"
#include "CPlayer.h"
#include "CCollider.h"
#include "CKnife.h"
#include "CKnifeAI.h"
#include "CDamageUI.h"
#include "CTextUI.h"

#include "CTimeMgr.h"

CKnife::CKnife()
	: m_fCoolTime(0.f)
	, m_Speed(800.f)
	, m_fwieldDuration(0.25f)
	, m_fwieldElapsed(0.f)
	, m_pAI(nullptr)
{
	tWeaponInfo	tInfo = {};
	SetName(L"Knife");
	tInfo.m_sName = L"Knife";
	tInfo.m_sIconImageKey = L"knife_icon";
	tInfo.m_eType = WEAPON_TYPE::KNIFE;
	tInfo.m_fCooldown = 1.01f;
	tInfo.m_fCritialDMG = 2.5f;
	tInfo.m_fCritialAcc = 20;
	tInfo.m_fRecogRange = 150;
	tInfo.m_iPenet = 1;
	tInfo.m_iDMG = 12;

	SetInfo(tInfo);
	SetScale(Vec2(10.f, 10.f));
	CreateCollider();
	GetCollider()->SetScale(GetScale());
	m_pPlayer = (CPlayer*)CSceneMgr::GetInstance()->GetCurScene()->GetPlayer();
}

CKnife::~CKnife()
{
}

void CKnife::update()
{
	if (nullptr == m_pAI) {
		//Ÿ�� ���� �����ϱ�.
		CWeapon::update();


		m_fCoolTime += fDT;

		if (nullptr != GetTarget() && (Getinfo().m_fCooldown < m_fCoolTime)) {
			Vec2 targetPos = GetTarget()->GetPos();
			Vec2 direction = targetPos - GetPos();
			Vec2 thisPos = GetPos();

			//���� ��Ÿ� �ȿ� ���� ��. 
			if (direction.Length() <= Getinfo().m_fRecogRange) {
				direction.Normalize();
				targetPos.x = GetPos().x + direction.x * Getinfo().m_fRecogRange;
				targetPos.y = GetPos().y + direction.y * Getinfo().m_fRecogRange;
				ShotMissile(targetPos);

				m_fCoolTime = 0.f;
			}
		}
	}
	else {
		m_fwieldElapsed += fDT;

		// 0 ~ 2����.
		// 0 ~ 1�� Ÿ���������� ���� ��.
		// 1 ~ 2�� Ÿ���������� ���ƿ��� ��. 
		float fProgress = (m_fwieldElapsed / (m_fwieldDuration * 0.5f));

		if (fProgress <= 1) {
			// 0 ~ 1 
			m_pAI->update(fProgress);
		}
		else {
			// 1 ~ 2
			m_pAI->update(1.f - (fProgress - 1.f));
		}


		if (m_fwieldElapsed > m_fwieldDuration) {
			delete m_pAI;
			m_pAI = nullptr;
			m_fwieldElapsed = 0.f;
		}
	}
}

std::pair<int, bool> CKnife::GetDamage()
{
	int default_damage = Getinfo().m_iDMG;
	bool Critical = false;

	if (random_distribution(random_generator) < Getinfo().m_fCritialAcc) {
		default_damage = static_cast<int>(static_cast<float>(default_damage) * Getinfo().m_fCritialDMG);
		Critical = true;
	}
	std::pair<int, bool> res;
	res.first = default_damage;
	res.second = Critical;
	return res;
}

void CKnife::ShotMissile(Vec2 _vDir)
{
	m_pAI = new CKnifeAI;
	m_pAI->SetOriginPos(GetPos());
	//Vec2 vTargetPos = _vDir * Getinfo().m_fRecogRange;
	m_pAI->SetTargetPos(_vDir);
	m_pAI->SetOwner(this);
}


void CKnife::render(Gdiplus::Graphics* _pDGraphics)
{
	Vec2 vPos = GetPos();
	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(vPos);
	Vec2 vScale = GetScale();
	Pen pen(Color(255, 255, 0, 0), 2.0f);
	_pDGraphics->DrawEllipse(&pen,
		vRenderPos.x - vScale.x / 2.f,
		vRenderPos.y - vScale.y / 2.f,
		vScale.x,
		vScale.y);
}

void CKnife::render(ID2D1HwndRenderTarget* _pRender)
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

	// �Ķ��� �귯�� ����
	ID2D1SolidColorBrush* pBrush = nullptr;
	HRESULT hr = _pRender->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Red),
		&pBrush
	);
	if (SUCCEEDED(hr))
	{
		// �� �β� 2.0f�� Ÿ�� �׸���
		_pRender->DrawEllipse(ellipse, pBrush, 2.0f);
		pBrush->Release();
	}
}


