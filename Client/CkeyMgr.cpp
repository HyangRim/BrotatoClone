
#include "pch.h"
#include "CkeyMgr.h"
#include "CCore.h"


int g_arrVK[(int)KEY::LAST] = {
	VK_LEFT,	//LEFT,
	VK_RIGHT,	//RIGHT,
	VK_UP,		//UP,
	VK_DOWN,	//DOWN,

	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',
	'A',
	'S',
	'D',
	'F',
	'G',
	'H',
	'J',
	'K',
	'L',
	'Z',
	'X',
	'C',
	'V',
	'B',
	'N',
	'M',

	VK_MENU,
	VK_CONTROL,
	VK_LSHIFT,
	VK_SPACE,
	VK_RETURN,
	VK_ESCAPE,

	VK_LBUTTON,
	VK_RBUTTON,

	//LAST,
};

CkeyMgr::CkeyMgr() {

}

CkeyMgr::~CkeyMgr() {

}

void CkeyMgr::init()
{

	for (int keyIDX = 0; keyIDX < (int)KEY::LAST; keyIDX++) {
		m_vecKey.push_back(tKeyInfo{ KEY_STATE::NONE, false });
	}

	//아래처럼 상태확인, 이전에 눌렸는지.
	//m_vecKey[(int)KEY::LEFT].bPrev
	//m_vecKey[(int)KEY::LEFT].eState
}

//뭔가를 하고 있다가, 창을 바꾸면 키를 떼지는 것까지는 구현해야하지 않을까?
//TAB상태였던 애는 바로 떼진 AWAY로 가야함 -> NONE
//HOLD였던 애는 AWAY로 갔다가 -> NONE

void CkeyMgr::update() {

	//윈도우 포커싱 알아내기 
	HWND hMainWnd = CCore::GetInstance()->GetMainHwnd();
	HWND hWnd = GetFocus();//현재 포커싱 되어 있는 윈도우의 핸들값을 알려줌. 


	//지금 포커싱 중인게, 이 프로그램의 메인 윈도우라면 같은지 아닌지만 확인해주면 됨.
	//윈도우 포커싱 중일 때, 키 이벤트 동작. 
	if (hMainWnd == hWnd) {

		for (int keyIDX = 0; keyIDX < (int)KEY::LAST; keyIDX++) {
			//키가 눌렸다면. 
			if (GetAsyncKeyState(g_arrVK[keyIDX]) & 0x8000) {
				if (m_vecKey[keyIDX].bPrevPush) {
					//이전에 눌려있었다. 
					m_vecKey[keyIDX].eState = KEY_STATE::HOLD;
				}
				else {
					//이전에 눌려있지 않았다. 
					m_vecKey[keyIDX].eState = KEY_STATE::TAP;
				}
				m_vecKey[keyIDX].bPrevPush = true;
			}
			else {//현재 키가 눌려있지 않다면. 
				if (m_vecKey[keyIDX].bPrevPush) {
					//이전에 키가 눌려있다면. 
					m_vecKey[keyIDX].eState = KEY_STATE::AWAY;
				}
				else {
					//이전에 키가 눌려있지 않았다면. 
					m_vecKey[keyIDX].eState = KEY_STATE::NONE;
				}
				m_vecKey[keyIDX].bPrevPush = false;
			}
		}

		//Mouse 위치 계산
		POINT ptPos = {};
		GetCursorPos(&ptPos);
		ScreenToClient(CCore::GetInstance()->GetMainHwnd(), &ptPos);

		m_vCurMousePos = Vec2((float)ptPos.x, (float)ptPos.y);
	}
	else {
		//윈도우 포커싱 해제 상태
		for (int keyIDX = 0; keyIDX < (int)KEY::LAST; keyIDX++) {
			m_vecKey[keyIDX].bPrevPush = false;
			if (m_vecKey[keyIDX].eState == KEY_STATE::TAP || m_vecKey[keyIDX].eState == KEY_STATE::HOLD) {
				m_vecKey[keyIDX].eState = KEY_STATE::AWAY;
			}
			else if (m_vecKey[keyIDX].eState == KEY_STATE::AWAY) {
				m_vecKey[keyIDX].eState = KEY_STATE::NONE;
			}
		}
	}
}
