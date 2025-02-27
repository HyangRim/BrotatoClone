#pragma once
// sound 용도 - 헤더 순서 중요




class CSound;

#define SOUNDBUFFERSIZE 64

//최적화를 위한 뭔가 깔끔하지 않은 방법. 
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

	FMOD::System* m_pSystem;			//시스템 포인터
	FMOD::Sound* m_pSound;			//사운드 1개. 
	FMOD::Channel* m_pChannel;
	 
public:
	HRESULT init();
	void Release();
	void update();			//반복해줘야 정상적 실행 가능.

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