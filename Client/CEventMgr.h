#pragma once





struct tEvent {
	EVENT_TYPE		eEven;
	DWORD_PTR		lParam;
	DWORD_PTR		wParam;
};

//충돌은 만들었지만, 실제 충돌을 활용하려고 하면. 때리고 삭제시키고가 필요한데.
//EX) Scene변경 -> 렌더링을 이미 다 끝마쳤는데?

//어떤 이벤트가 발생하면, 해당 프레임의 모두가 적용되어야 한다. 
//같은 사건인데도 다르게 받아들여질수가 있음 <- 어떤 사건, 이벤트들을 프레임 단위로 동기화 해줘야 하지 않을까??

//바로 죽이지 않음 -> 지연처리를 통해서 다음 프레임에 모두 적용시킬 것. 

class CEventMgr
{
	SINGLE(CEventMgr)

private:
	vector<tEvent> m_vecEvent;

	vector<CObject*> m_vecDeadScheduled;		//삭제 예정인 오브젝트들. 
public:
	void update();


private:
	void Excute(const tEvent& _eve);


public:
	void AddEvent(const tEvent& _eve) {
		m_vecEvent.push_back(_eve);
	}

	//삭제를 하면, 다음 프레임에서 삭제 적용. 
	//삭제된 오브젝트를 원래 알고있던 오브젝트는 걔가 삭제 됐는지 어떤지 알 방법이 없음. 
	//그래서 저 녀석을 삭제할 거라고 트리거를 on시킴. 그 오브젝트는 Dead상태가 됨. 
	//다음 프레임에서는 Dead상태로 존재해서, 참조하는 오브젝트들이 얘 곧 죽겠구나 알 수 있음. 

	//또 다음 프레임에 삭제 예정인 애들을 삭제시킴. 
};

