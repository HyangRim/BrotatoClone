#include "pch.h"
#include "CEventMgr.h"

#include "CObject.h"
#include "CSceneMgr.h"
#include "CUIMgr.h"
#include "CScene.h"


CEventMgr::CEventMgr() 
	: m_vecEvent{}
{

}

CEventMgr::~CEventMgr() {


}

void CEventMgr::update()
{
	//==================================================
	//이전 프레임에서 등록해준 Dead Object들을 삭제한다.
	//==================================================

	for (auto deadObjectPtr  : m_vecDeadScheduled) {
		delete deadObjectPtr;
	}

	m_vecDeadScheduled.clear();

	//  ===============
	//    Event 처리.
	//  ===============
	for (const auto& event_node : m_vecEvent) {
		Excute(event_node);
	}

	m_vecEvent.clear();
}

void CEventMgr::Excute(const tEvent& _eve)
{

	switch (_eve.eEven) {
	case EVENT_TYPE::CREATE_OBJECT:
	{
		// lParam : Object Address
		// wParam : Group Type
		CObject* pNewObj = (CObject*)_eve.lParam;
		GROUP_TYPE eType = (GROUP_TYPE)_eve.wParam;
		

		CSceneMgr::GetInstance()->GetCurScene()->AddObject(pNewObj, eType);
	}
		break;
	case EVENT_TYPE::DELETE_OBJECT:
	{
		// lParam : Object Address
		// wParam : NULL
		// Object를 Dead 상태로 변경하고 -> 삭제 예정 오브젝트들을 모아둔다.
		CObject* pDeleteObj = (CObject*)_eve.lParam;
		pDeleteObj->SetDead();
		m_vecDeadScheduled.push_back(pDeleteObj);

	}

		break;
	case EVENT_TYPE::SCENE_CHANGE:
	{
		// lParam : Next Scene Type
		// wParam : NULL
		CSceneMgr::GetInstance()->ChangeScene((SCENE_TYPE)_eve.lParam);

		//이전 Scene에 대한 UI를 가리키고 있었기에 그거 해제. 
		CUIMgr::GetInstance()->SetFocusedUI(nullptr);
	}
		break;
	}
}
