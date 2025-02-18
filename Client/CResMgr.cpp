#include "pch.h"
#include "CResMgr.h"

#include "CPathMgr.h"
#include "CTexture.h"


CResMgr::CResMgr() {

}

CResMgr::~CResMgr() {

	Safe_Delete_Map(m_mapTex);
}

CTexture* CResMgr::LoadTexture(const wstring& _strKey, const wstring& _strRelativePath)
{
	//이미 텍스쳐가 map에 있는지 없는지.
	CTexture* pTex = FindTexture(_strKey);

	//이미 있다면 그냥 리턴해주기. 
	if (pTex != nullptr) return pTex;


	wstring strFilePath = CPathMgr::GetInstance()->GetContentPath();
	strFilePath += _strRelativePath;

	pTex = new CTexture;
	pTex->Load(strFilePath);

	//키랑 상대경로를 리소스가 스스로 알도록 하자. 
	//스스로가 어떤 이름인지, 어떤 경로인지 알도록 하자. 
	pTex->SetKey(_strKey);
	pTex->SetRelativePath(_strRelativePath);


	m_mapTex.insert(make_pair(_strKey, pTex));
	return pTex;
}

CTexture* CResMgr::FindTexture(const wstring& _strKey)
{
	auto TextureIter = m_mapTex.find(_strKey);

	if (TextureIter == m_mapTex.end()) return nullptr;
	else return (CTexture*)TextureIter->second;
}

CTexture* CResMgr::CreateTexture(const wstring& _strKey, UINT _iWidth, UINT _iHeight)
{
	//이미 텍스쳐가 map에 있는지 없는지.
	CTexture* pTex = FindTexture(_strKey);

	//이미 있다면 그냥 리턴해주기. 
	if (pTex != nullptr) return pTex;


	pTex = new CTexture;
	pTex->Create(_iWidth, _iHeight);

	//키랑 상대경로를 리소스가 스스로 알도록 하자. 
	//스스로가 어떤 이름인지, 어떤 경로인지 알도록 하자. 
	pTex->SetKey(_strKey);

	m_mapTex.insert(make_pair(_strKey, pTex));

	return pTex;
}
