#pragma once

struct CharacterInfo
{

};

class CharacterInfoMgr
{
	SINGLE(CharacterInfoMgr);

private:
	//임시, 확정 x
	map<wstring, CharacterInfo*> m_characterStatData;

public:
	void init();
};

