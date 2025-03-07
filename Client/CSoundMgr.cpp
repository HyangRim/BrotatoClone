#include "pch.h"
#include "CSoundMgr.h"
#include "CResMgr.h"
#include "CSound.h" 
#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CCore.h"

CSoundMgr::CSoundMgr()
	: m_pSystem(nullptr)
	, m_pChannel(nullptr)
	, m_pSound(nullptr)
	, m_fMasterRatio(1.f)
	, m_fBGMRatio(1.f)
	, m_fSFXRatio(1.f)
	, m_iStepIdx(0)
{
	InitVolumePointer();
}

CSoundMgr::~CSoundMgr()
{
	Release();
}

HRESULT CSoundMgr::init()
{
	FMOD_RESULT ret;
	ret = FMOD::System_Create(&m_pSystem);

	if (ret != FMOD_OK) {
		return E_FAIL;
	}

	ret = m_pSystem->init(SOUNDBUFFERSIZE, FMOD_INIT_NORMAL, 0);

	if (ret != FMOD_OK) {
		return E_FAIL;
	}

	ret = m_pSystem->createChannelGroup("BGM", &m_pBGMChannelGroup);
	if (ret != FMOD_OK) {
		return E_FAIL;
	}

	ret = m_pSystem->createChannelGroup("SFX", &m_pSFXChannelGroup);
	if (ret != FMOD_OK) {
		return E_FAIL;
	}

	return S_OK;
}

void CSoundMgr::Release()
{
	//Sound는 내부에 해제하는 것이 있어서 SAFE_DELETE_MAP 함수 안 쓴다.
	for (auto soundData : m_mapSounds) {
		soundData.second->m_pSound->release();
		delete soundData.second;
	}
	m_mapSounds.clear();

	if (m_pBGMChannelGroup) {
		m_pBGMChannelGroup->release();
		m_pBGMChannelGroup = nullptr;
	}

	if (m_pSFXChannelGroup) {
		m_pSFXChannelGroup->release();
		m_pSFXChannelGroup = nullptr;
	}

	if (nullptr != m_pSystem) {
		m_pSystem->release();
		m_pSystem->close();
	}
}

void CSoundMgr::update()
{
	m_pSystem->update();
}

void CSoundMgr::AddSound(wstring _keyName, wstring _fileName, bool _bgm, bool _loop)
{
	FMOD_RESULT ret;

	auto soundIter = m_mapSounds.find(_keyName);
	
	if (soundIter != m_mapSounds.end()) {
		return;
	}

	SoundInfo* info = new SoundInfo;
	info->isBGM = _bgm;

	wstring strFilePath = CPathMgr::GetInstance()->GetContentPath();
	strFilePath += _fileName;

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::string strFilePathNarrow = converter.to_bytes(strFilePath);
	
	if (_bgm) {
		//BGM일때.
		ret = m_pSystem->createStream(strFilePathNarrow.c_str(), FMOD_LOOP_NORMAL, nullptr, &info->m_pSound);
	}
	else {
		//BGM이 아닌 SFX일때. 

		if (_loop) {
			ret = m_pSystem->createSound(strFilePathNarrow.c_str(), FMOD_LOOP_NORMAL, nullptr, &info->m_pSound);
		}
		else {
			ret = m_pSystem->createSound(strFilePathNarrow.c_str(), FMOD_DEFAULT, nullptr, &info->m_pSound);
		}
	}

	if (ret != FMOD_OK) assert(false);

	m_mapSounds.insert(make_pair(_keyName, info));
}

void CSoundMgr::Play(wstring _keyName, float _volume)
{
	auto soundIter = m_mapSounds.find(_keyName);

	if (soundIter == m_mapSounds.end()) {
		assert(false);
	}

	if (soundIter->second->isBGM) {
		m_pSystem->playSound(soundIter->second->m_pSound, m_pBGMChannelGroup, false, &soundIter->second->m_pChannel);

		soundIter->second->m_pChannel->setVolume(_volume);
	}
	else {
		m_pSystem->playSound(soundIter->second->m_pSound, m_pSFXChannelGroup, false, &soundIter->second->m_pChannel);

		soundIter->second->m_pChannel->setVolume(_volume);
	}

}

void CSoundMgr::Stop(wstring _keyName)
{
	auto soundIter = m_mapSounds.find(_keyName);

	if (soundIter == m_mapSounds.end()) {
		assert(false);
	}

	soundIter->second->m_pChannel->stop();
}

void CSoundMgr::Pause(wstring _keyName)
{
	auto soundIter = m_mapSounds.find(_keyName);

	if (soundIter == m_mapSounds.end()) {
		assert(false);
	}

	soundIter->second->m_pChannel->setPaused(true);
}

void CSoundMgr::Resume(wstring _keyName)
{
	auto soundIter = m_mapSounds.find(_keyName);

	if (soundIter == m_mapSounds.end()) {
		assert(false);
	}

	soundIter->second->m_pChannel->setPaused(false);
}


bool CSoundMgr::IsPlaySound(wstring& _keyName)
{
	auto soundIter = m_mapSounds.find(_keyName);

	if (soundIter == m_mapSounds.end()) {
		assert(false);
	}

	bool isPlay = false;

	soundIter->second->m_pChannel->isPlaying(&isPlay);

	return isPlay;
}

bool CSoundMgr::IsPauseSound(wstring _keyName)
{
	return false;
}

void CSoundMgr::InitVolumePointer()
{
	m_pMasterSoundSlider = nullptr;
	m_pBGMSoundSlider = nullptr;
	m_pSFXSoundSlider = nullptr;

	m_pMasterSoundRatio = nullptr;
	m_pBGMSoundRatio = nullptr;
	m_pSFXSoundRatio = nullptr;
}

bool CSoundMgr::IsOptionPanel()
{
	if (nullptr != m_pMasterSoundSlider &&
		nullptr != m_pMasterSoundRatio &&
		nullptr != m_pBGMSoundSlider &&
		nullptr != m_pBGMSoundRatio &&
		nullptr != m_pSFXSoundSlider &&
		nullptr != m_pSFXSoundRatio) {
		return true;
	}
	return false;
}

void CSoundMgr::SetBGMChannelVolume(float _fVolume)
{
	if (m_pBGMChannelGroup) {
		FMOD_RESULT result = m_pBGMChannelGroup->setVolume(_fVolume);
		//wprintf(L"BGM Volume: %f\n", _fVolume);
		if (result != FMOD_OK)
		{
			// 에러 처리: 필요시 로그 출력 또는 assert 처리
			assert(false);
		}
	}
}

void CSoundMgr::SetSFXChannelVolume(float _fVolume)
{
	if (m_pSFXChannelGroup) {
		FMOD_RESULT result = m_pSFXChannelGroup->setVolume(_fVolume);
		//wprintf(L"SFX Volume: %f\n", _fVolume);
		if (result != FMOD_OK)
		{
			// 에러 처리: 필요시 로그 출력 또는 assert 처리
			assert(false);
		}
	}
}

void CSoundMgr::PlayWalkSound()
{
	//Play(walkSoundKey[0], 1.f);
	Play(walkSoundKey[m_iStepIdx], 0.3f);
	m_iStepIdx = (m_iStepIdx + 1) % 4;
}

