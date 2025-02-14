#pragma once

#include "global.h"
class CRes
{

private:
	wstring		m_strKey;			//리소스의 고유한 키값
	wstring		m_strRelativePath;	//리소스 상대경로

public:
	void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
	void SetRelativePath(const wstring& _strPath) { m_strRelativePath = _strPath; }


	const wstring& GetKey() { return m_strKey; }
	const wstring& GetRelativePath() { return m_strRelativePath; }

public:
	CRes();
	virtual ~CRes();
};

