#pragma once
class CWaveMgr
{
	SINGLE(CWaveMgr)


private:
	int			m_iWaveLevel;
	float		m_fWaveDuration;			//1���̺� 20�� ~ 5+ �ؼ� 10���̺� 70�ʱ���
	float		m_fWaveElapsed;	

	float		m_fWaveSpawnTime;
	float		m_fWaveSpawnElapsed;


	bool		m_bWaving;

	CObject*	m_pWaveComplete;


private:
	//�������� �� ����, ���Ÿ� Ȯ��. �� 100.
	const Vec2 WaveMonsterProbability[11] = {
		Vec2(100, 0),
		Vec2(100, 0),
		Vec2(95, 5),
		Vec2(90, 10),
		Vec2(85, 15),
		Vec2(85, 15),
		Vec2(80, 20),
		Vec2(80, 20),
		Vec2(75, 25),
		Vec2(75, 25),
		Vec2(75, 25)
	};


public:
	void update();
public:

	void WaveStart();
	void WaveClear();
	void WaveInit();
	void WavePause();
	
	void SetWabing(bool _bWave) { m_bWaving = _bWave; }
	int GetLevel() const { return m_iWaveLevel; }
	float GetWaveDuration() const { return m_fWaveDuration; }

private:
	void SpawnMob();
	void WaveLevelUp();
};

