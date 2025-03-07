#pragma once
// sound �뵵 - ��� ���� �߿�




class CSound;
class CObject;
class CSliderUI;

#define SOUNDBUFFERSIZE 64

//����ȭ�� ���� ���� ������� ���� ���. 
static wstring walkSoundKey[4] = { L"Step1", L"Step2",L"Step3",L"Step4" };
const float	   fWalkDuration = 0.3f;
float		   fWalkElapsed = 0.f;


struct SoundInfo {
	FMOD::Sound*	m_pSound = nullptr;
	FMOD::Channel*	m_pChannel = nullptr;
	bool			isBGM = false;
};

class CSoundMgr
{
	SINGLE(CSoundMgr)

private:
	unordered_map <wstring, SoundInfo*> m_mapSounds;

	FMOD::System* m_pSystem;			//�ý��� ������
	FMOD::Sound* m_pSound;			//���� 1��. 
	FMOD::Channel* m_pChannel;

	FMOD::ChannelGroup* m_pBGMChannelGroup;
	FMOD::ChannelGroup* m_pSFXChannelGroup;

public:
	CSliderUI*		m_pMasterSoundSlider;
	CSliderUI*		m_pBGMSoundSlider;
	CSliderUI*		m_pSFXSoundSlider;

	CObject*		m_pMasterSoundRatio;
	CObject*		m_pBGMSoundRatio;
	CObject*		m_pSFXSoundRatio;

	float			m_fMasterRatio;
	float			m_fBGMRatio;
	float			m_fSFXRatio;

	int				m_iStepIdx;
	 
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

	void InitVolumePointer();
	bool IsOptionPanel();

	void SetMasterRatio(float _fRatio) { m_fMasterRatio = _fRatio; }
	float GetMasterRatio() { return m_fMasterRatio; }

	void SetBGMRatio(float _fRatio) { m_fBGMRatio = _fRatio; }
	float GetBGMRatio() { return m_fBGMRatio; }

	void SetSFXRatio(float _fRatio) { m_fSFXRatio = _fRatio; }
	float GetSFXRatio() { return m_fSFXRatio; }


	void SetBGMChannelVolume(float _fVolume);
	void SetSFXChannelVolume(float _fVolume);

public:
};