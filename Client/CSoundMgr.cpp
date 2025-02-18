#include "pch.h"
#include "CSoundMgr.h"
#include "CResMgr.h"
#include "CSound.h" 
#include "CPathMgr.h"
#include "CCore.h"

CSoundMgr::CSoundMgr()
	: m_pSystem(nullptr)
	, m_pChannel(nullptr)
	, m_pSound(nullptr)
{
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

	m_pSystem->playSound(soundIter->second->m_pSound, nullptr, false, &soundIter->second->m_pChannel);

	soundIter->second->m_pChannel->setVolume(_volume);
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

bool CSoundMgr::IsPlaySound(wstring _keyName)
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
