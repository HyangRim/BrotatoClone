#pragma once


//#include 거는 거 불편함. 그래서. 




//순차적이기 때문에, 한 프레임 시간이 지났을 때. DT라는 시간이 지난만큼 각각 변경점을 적용함 -> 그렇게 그린 걸 우리가 봄
//A 물체를 이동시켰을 때, B물체보다 먼저 일어난 건가? 그건 아니지 않나? 그런데 절차지향대로라면 그렇게 됨. 

//이 모든 작업이 완료 됐을 때 모두가 동시에 일어난 사건으로 봐야만 함. 
//만약 누구는 처리 받고, 누구는 처리 받지 못할수도 있음. 
// 
//따라서 KeyManager가 필요한 이유 
//1. 프레임 동기화. 한 프레임에 모두 일어나야 함. 

//2. 윈도우에서 만들어준 함수로는 구체적인 이벤트가 적용되어 있지 않음
//EX) 엔터키가 눌려있다만 궁금한 게 아니라, 이번에 눌렸다(Down), 이번에 뗏다(Up) <- 이런 것도 지정해줘야함. 
#include "define.h"



//키 상태.
enum class KEY_STATE {
	TAP,	//막 누른 시점
	HOLD,	//누르고 있는
	AWAY,	//막 뗀 시점
	NONE,	// 눌리지 않은 상태. 이전에도 눌리지 않은 상태. 
};


//지원해줄 수 있는 키 

enum class KEY {
	LEFT,
	RIGHT,
	UP,
	DOWN,

	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,
	A,
	S,
	D,
	F,
	G,
	H,
	J,
	K,
	L,
	Z,
	X,
	C,
	V,
	B,
	N,
	M,

	ALT,
	CTRL,
	LSHIFT,
	SPACE,
	ENTER,
	ESC,

	LBTN,
	RBTN,
	
	LAST,
};

struct tKeyInfo{
	KEY_STATE		eState;			//키의 상태값. 
	bool			bPrevPush;		//이전에 이 키가 눌렸는지 안 눌렸는지
};
class CkeyMgr
{
	SINGLE(CkeyMgr);

private:
	//벡터의 INDEX값이, 곧 KEY값. 
	vector<tKeyInfo>	m_vecKey;
	Vec2				m_vCurMousePos;

public:
	void init();
	void update();


public:
	KEY_STATE GetKeyState(KEY _eKey) {
		return m_vecKey[(int)_eKey].eState;
	};

	Vec2 GetMousePos() { return m_vCurMousePos; }
};

