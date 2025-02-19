#include "pch.h"
#include "CGround.h"
#include "CCollider.h"
#include "CGravity.h"


CGround::CGround()
{
	CreateCollider();
}

CGround::~CGround()
{
}

void CGround::start()
{
	GetCollider()->SetScale(GetScale());
}

void CGround::update()
{

}

void CGround::OnCollisionEnter(CCollider* _pOther)
{
	//굳이 없어도 될 거 같은데 Rigidbody로는. 
	/*
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player") {
		//pOtherObj->GetGravity()->SetGround(true);

		//파고 들었을 경우에 중심 위치에서. 
		Vec2 vObjPos = _pOther->GetFinalPos();
		Vec2 vObjScale = _pOther->GetScale();

		Vec2 vPos = GetCollider()->GetFinalPos();
		Vec2 vScale = GetCollider()->GetScale();

		float fLen = abs(vObjPos.y - vPos.y);
		float fInterpolValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

		vObjPos = pOtherObj->GetPos();
		vObjPos.y -= fInterpolValue;

		pOtherObj->SetPos(vObjPos);
	}
	*/
}

void CGround::OnCollision(CCollider* _pOther)
{

	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player") {
		//pOtherObj->GetGravity()->SetGround(true);

		//파고 들었을 경우에 중심 위치에서. 
		Vec2 vObjPos = _pOther->GetFinalPos();
		Vec2 vObjScale = _pOther->GetScale();

		Vec2 vPos = GetCollider()->GetFinalPos();
		Vec2 vScale = GetCollider()->GetScale();



		if (((vPos.x - vScale.x / 2.f) < vObjPos.x )
			&& ((vPos.x + vScale.x / 2.f) > vObjPos.x )
			&&( vObjPos.y < vPos.y)) 
		{
			//Case 1 캐릭터가 위에 있을 때. 
			// X축 값으로 사이에 있고, 플레이어 Y값이 Ground보다 낮을 경우. 
			float fLen = abs(vObjPos.y - vPos.y);
			float fInterpolValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();
			vObjPos.y -= (fInterpolValue);
			pOtherObj->SetPos(vObjPos);
		}
		else if (((vPos.y - vScale.y / 2.f) < vObjPos.y)
			&& ((vPos.y + vScale.y / 2.f) > vObjPos.y)
			&& (vPos.x < vObjPos.x)) {
			//Case 2 캐릭터가 오른쪽에 있을 때.
			//Y축 값으로 사이에 있고, 플레이어 X값이 Ground보다 높을 경우

			float fLen = abs(vObjPos.x - vPos.x);
			float fInterpolValue = (vObjScale.x / 2.f + vScale.x / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();
			vObjPos.x += (fInterpolValue);
			pOtherObj->SetPos(vObjPos);
			
		}
		else if (((vPos.x - vScale.x / 2.f) < vObjPos.x)
			&& ((vPos.x + vScale.x / 2.f) > vObjPos.x)
			&& (vObjPos.y > vPos.y)) {
			//Case 3 캐릭터가 아래에 있을 때.
			// X축 값으로 사이에 있고, 플레이어 Y값이 Ground보다 높을 경우. 
			float fLen = abs(vObjPos.y - vPos.y);
			float fInterpolValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();
			vObjPos.y += (fInterpolValue);
			pOtherObj->SetPos(vObjPos);
		}
		else if (((vPos.y - vScale.y / 2.f) < vObjPos.y)
			&& ((vPos.y + vScale.y / 2.f) > vObjPos.y)
			&& (vPos.x > vObjPos.x)) {
			//Case 4 캐릭터가 왼쪽에 있을 때. 
			//Y축 값으로 사이에 있고, 플레이어 X값이 Ground보다 낮을 경우
			float fLen = abs(vObjPos.x - vPos.x);
			float fInterpolValue = (vObjScale.x / 2.f + vScale.x / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();
			vObjPos.x -= (fInterpolValue);
			pOtherObj->SetPos(vObjPos);
		}
 
	}
}

void CGround::OnCollisionExit(CCollider* _pOther)
{

	//Exit에서 위치에 따라 Velocity값을 바꿔줘야하나? 부딪힌 곳 기준으로.
	//아니면 Enter? 
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player") {
		//그라운드 체크 해체. 우리 게임은 중력 설정 없어서..
		//pOtherObj->GetGravity()->SetGround(false);
	}
}
