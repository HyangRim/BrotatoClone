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
	//���� ��� �� �� ������ Rigidbody�δ�. 
	/*
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player") {
		//pOtherObj->GetGravity()->SetGround(true);

		//�İ� ����� ��쿡 �߽� ��ġ����. 
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

		//�İ� ����� ��쿡 �߽� ��ġ����. 
		Vec2 vObjPos = _pOther->GetFinalPos();
		Vec2 vObjScale = _pOther->GetScale();

		Vec2 vPos = GetCollider()->GetFinalPos();
		Vec2 vScale = GetCollider()->GetScale();



		if (((vPos.x - vScale.x / 2.f) < vObjPos.x )
			&& ((vPos.x + vScale.x / 2.f) > vObjPos.x )
			&&( vObjPos.y < vPos.y)) 
		{
			//Case 1 ĳ���Ͱ� ���� ���� ��. 
			// X�� ������ ���̿� �ְ�, �÷��̾� Y���� Ground���� ���� ���. 
			float fLen = abs(vObjPos.y - vPos.y);
			float fInterpolValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();
			vObjPos.y -= (fInterpolValue);
			pOtherObj->SetPos(vObjPos);
		}
		else if (((vPos.y - vScale.y / 2.f) < vObjPos.y)
			&& ((vPos.y + vScale.y / 2.f) > vObjPos.y)
			&& (vPos.x < vObjPos.x)) {
			//Case 2 ĳ���Ͱ� �����ʿ� ���� ��.
			//Y�� ������ ���̿� �ְ�, �÷��̾� X���� Ground���� ���� ���

			float fLen = abs(vObjPos.x - vPos.x);
			float fInterpolValue = (vObjScale.x / 2.f + vScale.x / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();
			vObjPos.x += (fInterpolValue);
			pOtherObj->SetPos(vObjPos);
			
		}
		else if (((vPos.x - vScale.x / 2.f) < vObjPos.x)
			&& ((vPos.x + vScale.x / 2.f) > vObjPos.x)
			&& (vObjPos.y > vPos.y)) {
			//Case 3 ĳ���Ͱ� �Ʒ��� ���� ��.
			// X�� ������ ���̿� �ְ�, �÷��̾� Y���� Ground���� ���� ���. 
			float fLen = abs(vObjPos.y - vPos.y);
			float fInterpolValue = (vObjScale.y / 2.f + vScale.y / 2.f) - fLen;

			vObjPos = pOtherObj->GetPos();
			vObjPos.y += (fInterpolValue);
			pOtherObj->SetPos(vObjPos);
		}
		else if (((vPos.y - vScale.y / 2.f) < vObjPos.y)
			&& ((vPos.y + vScale.y / 2.f) > vObjPos.y)
			&& (vPos.x > vObjPos.x)) {
			//Case 4 ĳ���Ͱ� ���ʿ� ���� ��. 
			//Y�� ������ ���̿� �ְ�, �÷��̾� X���� Ground���� ���� ���
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

	//Exit���� ��ġ�� ���� Velocity���� �ٲ�����ϳ�? �ε��� �� ��������.
	//�ƴϸ� Enter? 
	CObject* pOtherObj = _pOther->GetObj();
	if (pOtherObj->GetName() == L"Player") {
		//�׶��� üũ ��ü. �츮 ������ �߷� ���� ���..
		//pOtherObj->GetGravity()->SetGround(false);
	}
}
