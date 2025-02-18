#pragma once


class CTexture;
class CRes;
class CResMgr
{
	SINGLE(CResMgr)
private:
	map<wstring, CRes*> m_mapTex;

public:
	CTexture* LoadTexture(const wstring& _strKey, const wstring& _strRelativePath);
	CTexture* FindTexture(const wstring& _strKey);


	//기존엔 파일로부터 로딩하는 것밖에 없음...
	//그런데 내가 직접 메모리 상에서 CREATE하는 경우도 있지 않나? 
	CTexture* CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight);
};

