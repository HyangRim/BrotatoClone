#include "pch.h"
#include "CUI.h"

#include "CCamera.h"
#include "CkeyMgr.h"

#include "SelectGDI.h"

CUI::CUI(bool _bCamAff)
	: m_pParentUI(nullptr)
	, m_bCamAffected(_bCamAff)
	, m_bMouseOn(false)
{
}

CUI::CUI(const CUI& _origin)
	: CObject(_origin)
	, m_pParentUI(nullptr)
	, m_bCamAffected(_origin.m_bCamAffected)
	, m_bMouseOn(false)
	, m_bLbtnDown(false)
{
	//여기서 복사 생성자를 만들었어도, 부모에선 기본생성자로 들어가기 때문에.
	//따라서 복사 생성자를 명시적으로 넣어줘야만 함. 
	for (auto& child : _origin.m_vecChildUI) {
		AddChild(child->Clone());
	}
}

CUI::~CUI()
{
	Safe_Delete_Vec(m_vecChildUI);
}


void CUI::update()
{
	//child ui update
	update_child();
}

void CUI::finalupdate()
{
	CObject::finalupdate();
	m_vFinalPos = GetPos();
	//UI의 최종 좌표를 구한다.
	if (GetParent()) {
		Vec2 vParentPos = GetParent()->GetFinalPos();
		m_vFinalPos += vParentPos;
	}

	//UI 마우스 체크
	MouseOnCheck();

	finalupdate_child();
}

void CUI::render(HDC _dc)
{
	Vec2 vPos = GetFinalPos();
	Vec2 vScale = GetScale();

	if (m_bCamAffected) {
		vPos = CCamera::GetInstance()->GetRenderPos(vPos);
	}
	if (m_bLbtnDown) {
		SelectGDI select(_dc, PEN_TYPE::GREEN);
		Rectangle(_dc, (int)(vPos.x), (int)(vPos.y),
			(int)(vPos.x + vScale.x),
			(int)(vPos.y + vScale.y));
	}
	else {
		Rectangle(_dc, (int)(vPos.x), (int)(vPos.y),
			(int)(vPos.x + vScale.x),
			(int)(vPos.y + vScale.y));
	}
	//child ui render
	render_child(_dc);
}

void CUI::update_child()
{
	for (auto& child : m_vecChildUI) {
		child->update();
	}
}

void CUI::finalupdate_child()
{
	for (auto& child : m_vecChildUI) {
		child->finalupdate();
	}
}

void CUI::render_child(HDC _dc)
{
	for (auto& child : m_vecChildUI) {
		child->render(_dc);
	}
}


void CUI::MouseOn()
{
}

void CUI::MouseLbtnDown()
{
}

void CUI::MouseLbtnUp()
{
}

void CUI::MouseLbtnClicked()
{
}


void CUI::MouseOnCheck()
{
	Vec2 vMousePos = MOUSE_POS;
	Vec2 vScale = GetScale();

	if (m_bCamAffected) {
		vMousePos = CCamera::GetInstance()->GetRealPos(vMousePos);
	}
	

	if (m_vFinalPos.x <= vMousePos.x && vMousePos.x <=m_vFinalPos.x + vScale.x
		&& m_vFinalPos.y <= vMousePos.y && vMousePos.y <= m_vFinalPos.y + vScale.y) 
	{
		m_bMouseOn = true;
	}
	else {
		m_bMouseOn = false;
	}
}

//누르는 순간 그 UI로 렌더링 포커싱이 바뀌어야 함. -> 캔버스 UI들의 렌더링 순서를 해야만 함. 