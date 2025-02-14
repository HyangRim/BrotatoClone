#include "pch.h"
#include "CUIMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CUI.h"

#include "CkeyMgr.h"

CUIMgr::CUIMgr() 
	: m_pFocusedUI(nullptr)
{

}

CUIMgr::~CUIMgr() {

}

void CUIMgr::update()
{
	bool bLbtnTap = KEY_TAP(KEY::LBTN);
	bool bLbtnAway = KEY_AWAY(KEY::LBTN);
	// 1. FocusedUI 확인
	m_pFocusedUI = GetFocusedUI();

	if (!m_pFocusedUI) return;

	// 2. FocusedUI 내에서 부모 UI 포함, 자식 UI들 중 실제 타겟팅 된 UI를 가져온다. 
	CUI* pTargetUI = GetTargetedUI(m_pFocusedUI);

	if (nullptr != pTargetUI) {
		pTargetUI->MouseOn();

		if (bLbtnTap) {//눌리기까지 함. 
			pTargetUI->MouseLbtnDown();
			pTargetUI->m_bLbtnDown = true;
		}
		else if (bLbtnAway)
		{
			//이번에 안 눌렀는데, 해당 위에 있다.
			pTargetUI->MouseLbtnUp();
			if (pTargetUI->m_bLbtnDown) {
				pTargetUI->MouseLbtnClicked();
			}
			//왼쪽 버튼 떼면 눌렸던 체크를 다시 해제한다. 
			pTargetUI->m_bLbtnDown = false;
		}
	}	
}


//특정 키 눌렸을 때 어떤 UI를 보여줘야 한다. 
void CUIMgr::SetFocusedUI(CUI* _pUI)
{
	//이미 포커스 된 거나, nullptr값이 들어올 경우에. 
	if (m_pFocusedUI == _pUI || nullptr == _pUI) {
		m_pFocusedUI = _pUI;
		return;
	}

	m_pFocusedUI = _pUI;

	CScene* pCurScene = CSceneMgr::GetInstance()->GetCurScene();
	vector<CObject*>& vecUI = pCurScene->GetUIGroup();


	vector<CObject*>::iterator CUIIter = vecUI.begin();
	//왼쪽 버튼 TAP이 발생했다는 전제가 있는 UI들. 
	for (; CUIIter != vecUI.end(); CUIIter++) {
		if (m_pFocusedUI == *CUIIter) {
			break;
		}
	}


	vecUI.erase(CUIIter);
	vecUI.push_back(m_pFocusedUI);
}

CUI* CUIMgr::GetFocusedUI()
{
	CScene* pCurScene = CSceneMgr::GetInstance()->GetCurScene();
	vector<CObject*>& vecUI = pCurScene->GetUIGroup();

	bool bLbtnTap = KEY_TAP(KEY::LBTN);

	//기존 포커싱 UI를 받아두고 변경되었는지 확인한다. 
	CUI* pFocusedUI = m_pFocusedUI;

	//왼쪽 클릭이 없다면, Focus가 전환될 이유도 없기에 그냥 리턴. 
	if (!bLbtnTap) return pFocusedUI;


	vector<CObject*>::iterator targetIter = vecUI.end();
	vector<CObject*>::iterator CUIIter = vecUI.begin();
	//왼쪽 버튼 TAP이 발생했다는 전제가 있는 UI들. 
	for (; CUIIter != vecUI.end(); CUIIter++) {
		if (((CUI*)*CUIIter)->IsMouseOn()) {
			targetIter = CUIIter;
		}
	}

	//마우스가 눌리긴 했는데 허공을 눌렀을 경우, 받아갈 놈이 없음.
	if (vecUI.end() == targetIter) {
		return nullptr;
	}

	pFocusedUI = (CUI*)*targetIter;
	//계속 위에서 교체되다가 보면, 최종적인 Focus를 가져가는 UI가 된다. 
	//그리고 벡터 내부에서 순서를 바꿔줘야함. 

	vecUI.erase(targetIter);
	vecUI.push_back(pFocusedUI);

	return pFocusedUI;
}

CUI* CUIMgr::GetTargetedUI(CUI* _pParentUI)
{
	//1. 부모 UI포함, 모든 자식 UI를 검사. 그 안에서 가장 우선순위 높은 건 가장 아래 자식.(자식 아래까지 내려가야함)

	//2. 여러 UI가 타겟중일때, 우선 순위가 제일 높은 건 아래 자식. 
	//3. UI가 겹쳐있다면? <- UI 배치부터가 잘못되었다. 

	//이런 트리 구조 순회는 Level 순회라고 한다. -> 재귀함수 방식으로 하면 부모 -> 자식 -> 더 자식 순서대로. 
	//우리가 원하는 것은 더 깊게가 아닌, 같은 계층의 UI는 같이 검사. 
	//따라서 Queue를 활용한다. 
	//0(부모)에서 1,2(자식)넣고, 1에서 3,4,5.. 넣고.. 이러면 된다. 
	bool bLbtnAway = KEY_AWAY(KEY::LBTN);

	CUI* pTargetUI = nullptr;

	//계속 함수 올 때마다 할당하는 건 낭비.
	static list<CUI*> levelQueue;
	static vector<CUI*> vecNoneTarget;

	//그냥 비워준다.
	levelQueue.clear();
	vecNoneTarget.clear();

	levelQueue.push_back(_pParentUI);

	while (!levelQueue.empty()) {
		CUI* pUI = levelQueue.front();

		levelQueue.pop_front();

		// 큐에서 꺼내온 UI가 TargetUI인지 확인.
		if (pUI->IsMouseOn()) {
			if (nullptr != pTargetUI) {
				vecNoneTarget.push_back(pTargetUI);
			}

			pTargetUI = pUI;
		}
		else {
			vecNoneTarget.push_back(pUI);
		}

		const vector<CUI*>& vecChild = pUI->GetChildsUI();
		for (auto& child : vecChild) {
			levelQueue.push_back(child);
		}
	}
	if (bLbtnAway) {
		for (auto& noneChild : vecNoneTarget) {
			noneChild->m_bLbtnDown = false;
		}
	}

	return pTargetUI;
}
