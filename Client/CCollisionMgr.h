#pragma once

class CCollider;



union COOLIDER_ID {
	struct {
		UINT Left_id;
		UINT Right_id;
	};
	ULONGLONG ID;
};

class CCollisionMgr
{
	SINGLE(CCollisionMgr)
private:
	//무수한 갯수 * 무수한 갯수이고, 또한 빠르게 찾아올 수 있어야함. 
	// 자료구조는 따라서 HashTable을 해야함. (탐색 특화) ->
	// 검색 조건은 두 충돌체들의 Key값이 있어야함. -> 충돌체 간의 고유한 ID값 필요. 

	unordered_map<ULONGLONG, bool>	m_mapColInfo;	//충돌체 간의 이전 프레임 충돌 정보. 

	UINT m_arrCheck[(UINT)GROUP_TYPE::END];			//그룹간의 충돌 체크 매트릭스


public:
	void update();
	void CheckGroup(GROUP_TYPE eLeft, GROUP_TYPE e_Right);
	void Reset() {
		memset(m_arrCheck, 0, sizeof(UINT) * (UINT)GROUP_TYPE::END);
		m_mapColInfo.clear();
	}


private:
	void CollisionGroupUpdate(GROUP_TYPE _eLeft, GROUP_TYPE _eRight);
	bool IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol);
};

