#include "pch.h"
#include "CPathMgr.h"

#include "CCore.h"

CPathMgr::CPathMgr() 
	: m_szContentPath{}
{

}

CPathMgr::~CPathMgr() {

}

void CPathMgr::init()
{

	//Ctrl + F5쓰면, 혹은 일반적으로 해도 프로젝트 폴더가 됨.
	//즉 비주얼 스튜디오에서 실행하는거랑, 프로그램 자체 실행이랑
	//경로값이 달라짐. 
	GetCurrentDirectory(255, m_szContentPath);
	//상위폴더로 나가고(\\을 만날때까지 찾고 거기에 null문자 넣기.) 
	int dirLen = static_cast<int>(wcslen(m_szContentPath));
	for (int dirIDX = dirLen - 1; dirIDX >= 0; dirIDX--) {
		if ('\\' == m_szContentPath[dirIDX]) {
			m_szContentPath[dirIDX] = '\0';
			break;
		}
	}
	//  + bin\\content\\ 경로 붙여주기
	wcscat_s(m_szContentPath, 255, L"\\bin\\content\\");
	//상대 경로 설정 완료.
	//SetWindowText(CCore::GetInstance()->GetMainHwnd(), m_szContentPath);
}

wstring CPathMgr::GetRelativePath(const wchar_t* _filepath)
{
	wstring strFilePath = _filepath;
	size_t iAbsLen = wcslen(m_szContentPath);
	size_t iFullLen = strFilePath.length();
	wstring strRelativePath = strFilePath.substr(iAbsLen, iFullLen - iAbsLen);
	
	return strRelativePath;
}
