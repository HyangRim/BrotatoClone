#include "pch.h"
#include "AI.h"

#include "CState.h"

AI::AI()
	: m_pCurState(nullptr)
	, m_pOwner(nullptr)
{
}

AI::~AI()
{
	Safe_Delete_Map(m_mapstate);
}

void AI::update()
{
	m_pCurState->update();
}

void AI::AddState(CState* _pState)
{
	CState* pState = GetState(_pState->GetType());
	assert(!pState);


	m_mapstate.insert(make_pair(_pState->GetType(), _pState));
	_pState->m_pAI = this;
}

CState* AI::GetState(MON_STATE _eState)
{
	auto mapIter = m_mapstate.find(_eState);

	if (mapIter == m_mapstate.end()) {
		return nullptr;
	}

	return mapIter->second;
}

void AI::ChangeState(MON_STATE _eNextState)
{
	CState* pNextState = GetState(_eNextState);

	assert(m_pCurState != pNextState);
	m_pCurState->Exit();
	m_pCurState = pNextState;
	m_pCurState->Enter();

}

void AI::SetCurState(MON_STATE _eState)
{
	m_pCurState = GetState(_eState);
	assert(m_pCurState);
}



