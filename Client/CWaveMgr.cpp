#include "pch.h"
#include "CCore.h"
#include "CWaveMgr.h"
#include "CTimeMgr.h"
#include "CScene.h"
#include "CSceneMgr.h"
#include "CMobSpawner.h"
#include "CMonFactory.h"
#include "CObject.h"
#include "CSpriteUI.h"

CWaveMgr::CWaveMgr()
	: m_iWaveLevel(1)
	, m_fWaveDuration(20.f)
	, m_fWaveElapsed(0.f)
	, m_pWaveComplete(nullptr)
	, m_bWaving(false)
	, m_fWaveSpawnTime(2.f)
	, m_fWaveSpawnElapsed(0.f)
{

}

CWaveMgr::~CWaveMgr() {
	if (nullptr != m_pWaveComplete) DeleteObject(m_pWaveComplete);

}

void CWaveMgr::update()
{
	//만약 이게 StartScene이 아니다 || 웨이빙 중이 아니면 통과. 
	if (CSceneMgr::GetInstance()->IsWaveScene() && m_bWaving) {
		m_fWaveElapsed += fDT;
		m_fWaveSpawnElapsed += fDT;

		if ((m_fWaveSpawnElapsed > m_fWaveSpawnTime) && (m_fWaveDuration - m_fWaveElapsed > 5.f)) {
			m_fWaveSpawnElapsed = 0.f;
			SpawnMob();
		}

		if (m_fWaveElapsed >= m_fWaveDuration) {
			m_fWaveElapsed = 0.f;
			WaveClear();
		}
	}
	else if (CSceneMgr::GetInstance()->IsWaveScene() && !m_bWaving) {
		
		if ((CSceneMgr::GetInstance()->GetCurScene()->GetGroupObject(GROUP_TYPE::DROP_ITEM).size() == 0)
			&& (CSceneMgr::GetInstance()->GetCurScene()->GetGroupObject(GROUP_TYPE::MONSTER).size() == 0)) {
			ChangeScene(SCENE_TYPE::SHOP);
		}
	}
}

void CWaveMgr::WaveStart()
{
	m_bWaving = true;
	
	CMonFactory::SetDropItemRecog(250.f);
	CMonFactory::SetDropItemSpeed(250.f);
	m_fWaveSpawnTime = (2.f - (m_iWaveLevel * 0.07f)) + float_distribution(rng);

}

void CWaveMgr::WaveClear()
{

	Vec2 vResolution = CCore::GetInstance()->GetResolution();
	m_bWaving = false;

	//==========================================================================
	//========================웨이브 완료 문구 뜨기=============================
	m_pWaveComplete = new CSpriteUI;
	wstring waveLevel = L"웨이브 완료!";
	waveLevel += std::to_wstring(CWaveMgr::GetInstance()->GetLevel());
	m_pWaveComplete->SetScale(Vec2(100.f, 100.f));
	m_pWaveComplete->SetPos(Vec2(vResolution.x / 2.f, 75.f));
	m_pWaveComplete->CreateTextUI(waveLevel, Vec2(-100.f, -100.f), Vec2(100.f, 100.f)
		, 24, D2D1::ColorF::White, true, 1.f, D2D1::ColorF::Black
		, FONT_TYPE::KR, TextUIMode::TEXT, 0);
	m_pWaveComplete->SetName(L"WavePanel");
	m_pWaveComplete->SetObjType(GROUP_TYPE::UI);

	CreateObject(m_pWaveComplete, GROUP_TYPE::UI);
	//==========================================================================
	//웨이브를 성공적으로 통과했으니, 웨이브 레벨 업. 
	CMonFactory::SetDropItemRecog(9999.f);
	CMonFactory::SetDropItemSpeed(500.f);
	WaveLevelUp();
	CSceneMgr::GetInstance()->GetCurScene()->DeleteGroup(GROUP_TYPE::MONSTER);
}

void CWaveMgr::WaveInit()
{
	m_iWaveLevel = 1;
	m_fWaveDuration = 20.f;
	m_fWaveElapsed = 0;
	m_bWaving = false;
	m_fWaveSpawnTime = 1.f;
	m_fWaveSpawnElapsed = 0.f;

	if (nullptr != m_pWaveComplete) {
		DeleteObject(m_pWaveComplete);
		m_pWaveComplete = nullptr;
	}
}

void CWaveMgr::SpawnMob()
{
	static std::uniform_int_distribution<int> MobSpawnCount(1, 3);
	static std::uniform_int_distribution<int> MobSpawnPos(50, 1000);
	int mobSpawnNum = MobSpawnCount(rng);
	

	for (int mobIdx = 0; mobIdx < mobSpawnNum; mobIdx++) {

		//좌표 정해주기. 
		float mx = static_cast<float>(MobSpawnPos(rng));
		float my = static_cast<float>(MobSpawnPos(rng));
		Vec2 vMonPos(mx, my);

		//무슨 몬스터일까. 
		int mobProbability = distribution(rng);
		
		if (0 <= mobProbability && mobProbability <= WaveMonsterProbability[m_iWaveLevel].x) {
			CMobSpawner::MobSpawn(MON_TYPE::NORMAL, vMonPos);
		}
		else {
			CMobSpawner::MobSpawn(MON_TYPE::RANGE, vMonPos);
		}
	}
}

void CWaveMgr::WaveLevelUp()
{
	m_iWaveLevel++;
	m_fWaveSpawnElapsed = 0.f;
	m_fWaveSpawnTime = 1.f;
	m_fWaveElapsed = 0.f;
	m_fWaveDuration += 5.f;
}


