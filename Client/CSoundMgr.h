#pragma once
// sound 용도 - 헤더 순서 중요




class CSound;

#define SOUNDBUFFERSIZE 64

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


	bool IsPlaySound(wstring _keyName);
	bool IsPauseSound(wstring _keyName);

public:
};