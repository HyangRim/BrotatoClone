#pragma once

struct CharacterInfo
{

};

class CharacterInfoMgr
{
	SINGLE(CharacterInfoMgr);

private:
	//�ӽ�, Ȯ�� x
	map<wstring, CharacterInfo*> m_characterStatData;

public:
	void init();
};

