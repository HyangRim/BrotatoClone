#include "pch.h"
#include "CCollider.h"

#include "CObject.h"
#include "CCore.h"
#include "CCamera.h"

#include "SelectGDI.h"

UINT CCollider::g_iNextID = 0;


//기본 복사 생성자는 그냥 얕은 복사. m_iID는 복사되면, 그대로 같은 값을 가지게 됨. 
CCollider::CCollider()
	: m_pOwner(nullptr)
	, m_iID(g_iNextID++)
	, m_iCol(0)
{
}

CCollider::CCollider(const CCollider& _origin)
	: m_pOwner(nullptr)
	, m_vOffsetPos(_origin.m_vOffsetPos)
	, m_vScale(_origin.m_vScale)
	, m_iID(g_iNextID++)
{
}

CCollider::~CCollider()
{
}

void CCollider::finalupdate()
{
	//부모 오브젝트의 위치를 따라간다.
	Vec2 vObjectPos = m_pOwner->GetPos();
	m_vFinalPos = vObjectPos + m_vOffsetPos;

	assert(0 <= m_iCol);
}



void CCollider::render(HDC _dc)
{
	PEN_TYPE ePen = PEN_TYPE::GREEN;

	if (m_iCol) ePen = PEN_TYPE::RED;
	//변수 이름 없이 만들면, 이름 없는 임시 객체 만들어짐.
	//여기서 바로 만들자마자 소멸시켜버림.
	/*
	1. 임시 객체는 사용하지 않으면 즉시 소멸한다.
	2. 참조 변수나 일반 변수에 저장하면 생명주기를 연장할 수 있다.
	3. 임시 객체를 참조로 반환하면, 소멸될 가능성이 있으므로 조심해야 한다.
	*/
	SelectGDI greenPen(_dc, ePen);
	SelectGDI hollowBrush(_dc, BRUSH_TYPE::HOLLOW);

	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(m_vFinalPos);

	Rectangle(_dc, (int)(vRenderPos.x - m_vScale.x / 2.f),
		(int)(vRenderPos.y - m_vScale.y / 2.f),
		(int)(vRenderPos.x + m_vScale.x / 2.f),
		(int)(vRenderPos.y + m_vScale.y / 2.f)
	);

}

void CCollider::OnCollision(CCollider* _pOther)
{
	m_pOwner->OnCollision(_pOther);
}

void CCollider::OnCollisionEnter(CCollider* _pOther)
{
	m_pOwner->OnCollisionEnter(_pOther);
	m_iCol++;
}

void CCollider::OnCollisionExit(CCollider* _pOther)
{
	m_pOwner->OnCollisionExit(_pOther);
	m_iCol--;
}




