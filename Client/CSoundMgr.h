#pragma once
// sound �뵵 - ��� ���� �߿�




class CSound;

#define SOUNDBUFFERSIZE 64

//����ȭ�� ���� ���� ������� ���� ���. 
static wstring walkSoundKey[4] = { L"step1", L"step2",L"step3",L"step4" };
const float	   fWalkDuration = 0.3f;
float		   fWalkElapsed = 0.f;


struct SoundInfo {
	FMOD::Sound*	m_pSound = nullptr;
	FMOD::Channel*	m_pChannel = nullptr;
};

class CSoundMgr
{
	SINGLE(CSoundMgr)

private:
	unordered_map <wstring, SoundInfo*> m_mapSounds;

	FMOD::System* m_pSystem;			//�ý��� ������
	FMOD::Sound* m_pSound;			//���� 1��. 
	FMOD::Channel* m_pChannel;
	 
public:
	HRESULT init();
	void Release();
	void update();			//�ݺ������ ������ ���� ����.

public:

	void AddSound(wstring _keyName, wstring _fileName, bool _bgm = false, bool _loop = false);
	void Play(wstring _keyName, float _volume = 1.0f);
	void Stop(wstring _keyName);
	void Pause(wstring _keyName);
	void Resume(wstring _keyName);
	void PlayWalkSound();


	bool IsPlaySound(wstring& _keyName);
	bool IsPauseSound(wstring _keyName);


public:
};