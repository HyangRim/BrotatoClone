#include "pch.h"
#include "CCollisionMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CObject.h"
#include "CCollider.h"

//유니온은 제일 큰 사이즈를 기준으로, 메모리를 공유함. 

CCollisionMgr::CCollisionMgr() 
	:m_arrCheck{}
{
	

}

CCollisionMgr::~CCollisionMgr() {

}

void CCollisionMgr::update()
{

	//표를 확인하면서, 비트값이 ON되어 있다면 충돌하고,
	//OFF라면 충돌하지 않음.

	for (UINT iRow = 0; iRow < (UINT)GROUP_TYPE::END; iRow++) {
		for (UINT iCol = iRow; iCol < (UINT)GROUP_TYPE::END; iCol++) {
			if (m_arrCheck[iRow] & (1 << iCol)) {
				//해당 그룹간의 충돌을 진행한다. 
				//따라서 현재 Scene에서 충돌체를 검사한다. 

				CollisionGroupUpdate((GROUP_TYPE)iRow, (GROUP_TYPE)iCol);
			}
		}
	}
}

void CCollisionMgr::CollisionGroupUpdate(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	CScene* pCurScene = CSceneMgr::GetInstance()->GetCurScene();


	//우리가 하려는 의도랑 조금 어긋나는 부분이 있음.
	//반환타입이 참조이니, 우리가 레퍼런스를 가져온 것. 
	//vecLeft는 주소값의 복사본(지역변수)임. 따라서 &로 받아야함. 
	const vector<CObject*>& vecLeft = pCurScene->GetGroupObject(_eLeft);
	const vector<CObject*>& vecRight = pCurScene->GetGroupObject(_eRight);

	unordered_map<ULONGLONG, bool>::iterator colliderMapIter;


	for (auto leftIDX = 0; leftIDX < vecLeft.size(); leftIDX++) {

		//충돌체를 보유하지 않는 경우 
		if (nullptr == vecLeft[leftIDX]->GetCollider()) continue;

		for (auto rightIDX = 0; rightIDX < vecRight.size(); rightIDX++) { 

			//충돌체를 보유하지 않는 경우. or 자기 자신과의 충돌인 경우. 
			if (nullptr == vecRight[rightIDX]->GetCollider() || vecLeft[leftIDX] == vecRight[rightIDX]) continue;


			CCollider* pLeftCollider = vecLeft[leftIDX]->GetCollider();
			CCollider* pRightCollider = vecRight[rightIDX]->GetCollider();


			//충돌체들 간의 조합 ID
			COOLIDER_ID ID;

			ID.Left_id = pLeftCollider->GetID();
			ID.Right_id = pRightCollider->GetID();

			colliderMapIter = m_mapColInfo.find(ID.ID);

			//충돌 정보가 미 등록 상태인 경우 등록(충돌하지 않았다. 로)
			if (colliderMapIter == m_mapColInfo.end()) {
				//두 조합의 검사를 최초로 했을 경우. 
				m_mapColInfo.insert(make_pair(ID.ID, false));
				colliderMapIter = m_mapColInfo.find(ID.ID);
			}

			//현재 충돌한 경우. 
			if (IsCollision(pLeftCollider, pRightCollider)) {
				if (colliderMapIter->second == false) {
					//이번 프레임에 막 충돌한 경우.
					
					//하필 이제 삭제될 때, 충돌할 때. 그 충돌은 없었던 것이 됨.
					if (!vecLeft[leftIDX]->IsDead() && !vecRight[rightIDX]->IsDead()) {
						pLeftCollider->OnCollisionEnter(vecRight[rightIDX]->GetCollider());
						pRightCollider->OnCollisionEnter(vecLeft[leftIDX]->GetCollider());
						colliderMapIter->second = true;
					}
				}
				else if (colliderMapIter->second == true) {
					//이전 프레임에도 충돌하고 있었을 경우. 

					//이제 죽는 애는 충돌에서 벗어났다고 명시적으로 해줌. 
					if (vecLeft[leftIDX]->IsDead() || vecRight[rightIDX]->IsDead()) {
						pLeftCollider->OnCollisionExit(vecRight[rightIDX]->GetCollider());
						pRightCollider->OnCollisionExit(vecLeft[leftIDX]->GetCollider());
						colliderMapIter->second = false;
					}
					else {
						pLeftCollider->OnCollision(vecRight[rightIDX]->GetCollider());
						pRightCollider->OnCollision(vecLeft[leftIDX]->GetCollider());
					}

				}

			}
			else {
				//충돌하지 않은 경우. 
				if (colliderMapIter->second == true) {
					//막 지금 충돌에서 벗어난 경우.
					pLeftCollider->OnCollisionExit(vecRight[rightIDX]->GetCollider());
					pRightCollider->OnCollisionExit(vecLeft[leftIDX]->GetCollider());
					colliderMapIter->second = false;
				}

			}
		}
	}
}

bool CCollisionMgr::IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol)
{
	Vec2 vLeftPos = _pLeftCol->GetFinalPos();
	Vec2 vLeftScale = _pLeftCol->GetScale();

	Vec2 vRightPos = _pRightCol->GetFinalPos();
	Vec2 vRightScale = _pRightCol->GetScale();

	if ((abs(vRightPos.x - vLeftPos.x) < (vLeftScale.x + vRightScale.x) / 2.f) &&
		(abs(vRightPos.y - vLeftPos.y) < (vLeftScale.y + vRightScale.y) / 2.f)) {
		return true;
	}

	return false;
}

void CCollisionMgr::CheckGroup(GROUP_TYPE _eLeft, GROUP_TYPE _eRight)
{
	//그룹값 중에서 더 작은 타입을 행으로, 더 큰 값을 열(비트)로 사용. 
	//


	UINT iRow = (UINT)_eLeft;
	UINT iCol = (UINT)_eRight;

	if (iCol < iRow) {
		iRow = (UINT)_eRight;
		iCol = (UINT)_eLeft;
	}

	if (m_arrCheck[iRow] & (1 << iCol)) {
		m_arrCheck[iRow] &= ~(1 << iCol);
	}
	else {
		m_arrCheck[iRow] |= (1 << iCol);
	}
}


