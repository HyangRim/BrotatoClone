#pragma once


class CTimeMgr
{
	SINGLE(CTimeMgr);
private:
	//이동량을 프레임으로 쪼개서 이동을 시키면, 이번 프레임에 움직이는 양이 나옴.

	//이동량 * 1프레임에 걸리는 시간(delta Time).
	LARGE_INTEGER m_llCurCount;
	LARGE_INTEGER m_llPrevCount;
	LARGE_INTEGER m_llFrequency;


	double			m_dDT;			//프레임 간의 시간값. 
	double			m_dAcc;			//1초 체크를 위한 누적 시간. 
	UINT			m_iCallCount;	//1초당 호출 횟수. 
	UINT			m_iFPS;			//1초당 프레임. 
public:
	//GetTick은 1초당 1000이라서 정확도가 조금...
	//게다가 오차도 있어서 훨씬 더 정밀한 함수가
	//QueryPerformanceCounter(); -> 1000만 단위.
	void init();
	void update();
	void render();

public:
	double GetDT() { return m_dDT; }
	float GetfDT() { return (float)m_dDT; }
};

