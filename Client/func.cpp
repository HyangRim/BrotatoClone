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

	int a = 0;

	CEventMgr::GetInstance()->AddEvent(changeEvn);
}
