#include "pch.h"
#include "func.h"



#include "CObject.h"
#include "CEventMgr.h"

void CreateObject(CObject* _pObj, GROUP_TYPE _eGroup)
{
	tEvent createEvn = {};
	createEvn.eEven = EVENT_TYPE::CREATE_OBJECT;
	createEvn.lParam = (DWORD_PTR)_pObj;
	createEvn.wParam = (DWORD_PTR)_eGroup;


	CEventMgr::GetInstance()->AddEvent(createEvn);
}

void DeleteObject(CObject* _pObj)
{
	tEvent deleteEvn = {};
	deleteEvn.eEven = EVENT_TYPE::DELETE_OBJECT;
	deleteEvn.lParam = (DWORD_PTR)_pObj;

	CEventMgr::GetInstance()->AddEvent(deleteEvn);
}

void ChangeScene(SCENE_TYPE _eNext)
{
	tEvent changeEvn = {};
	changeEvn.eEven = EVENT_TYPE::SCENE_CHANGE;
	changeEvn.lParam = (DWORD_PTR)_eNext;

	CEventMgr::GetInstance()->AddEvent(changeEvn);
}

void ChangeAIState(AI* _pAI, MON_STATE _eNextState)
{
	tEvent changeEvn = {};
	changeEvn.eEven = EVENT_TYPE::CHANGE_AI_STATE;
	changeEvn.lParam = (DWORD_PTR)_pAI;
	changeEvn.wParam = (DWORD_PTR)_eNextState;

	CEventMgr::GetInstance()->AddEvent(changeEvn);
}

void Fscanf(char* _pOutBuff, FILE* _pFile)
{
	int idx = 0;
	while (true) {
		char c = (char)getc(_pFile);
		if (c == '\n') {
			_pOutBuff[idx++] = '\0';
			break;
		}

		_pOutBuff[idx++] = c;
	}

}

void SaveWstring(const wstring& _str, FILE* _pFile)
{
	//Animation의 이름을 저장한다.(데이터 직렬화)
	const wchar_t* pStrName = _str.c_str();
	size_t iLen = _str.length();

	//문장 길이 저장. 
	fwrite(&iLen, sizeof(size_t), 1, _pFile);

	//문자열 저장. 
	fwrite(pStrName, sizeof(wchar_t), iLen, _pFile);
}

void LoadWstring(wstring& _str, FILE* _pFile)
{

	size_t iLen = 0;
	wchar_t szBuffer[256] = {};
	//문장 길이 저장. 
	fread(&iLen, sizeof(size_t), 1, _pFile);

	//문자열 저장. 
	fread(szBuffer, sizeof(wchar_t), iLen, _pFile);

	_str = szBuffer;
}

Vec2 lerp(const Vec2& a, const Vec2& b, float t)
{
	Vec2 result;
	result.x = a.x + t * (b.x - a.x);
	result.y = a.y + t * (b.y - a.y);

	return result;
}
