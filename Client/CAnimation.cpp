#include "pch.h"
#include "CAnimation.h"
#include "CAnimator.h"
#include "CTimeMgr.h"
#include "CTexture.h"
#include "CObject.h"
#include "CCamera.h"
#include "CPathMgr.h"
#include "CResMgr.h"


CAnimation::CAnimation()
	:m_pAnimator(nullptr)
	,m_pTex(nullptr)
	,m_iCurFrm(0)
	,m_fAccTime(0.f)
	,m_bFinish(false)
	,m_iFrameCount(0)
{

}


//�������� �ִϸ��̼��� ����...
//ž�� ������ ������ ��������. ���� �Ѿ˶��� �׸�ŭ �Ʒ��� ������...

CAnimation::~CAnimation()
{

}

void CAnimation::update()
{
	if (m_bFinish) return;
	m_fAccTime += fDT;
	//�������� ��� ���ư��µ�, ���� ���� �� �� ����� ���Ѵٸ�?
	if (m_vecFrm[m_iCurFrm].fDuration < m_fAccTime) {

		//�Ͻ� �����ؼ� fDT�� 2.344�� ������ Duration �� ���� �ִ°� ���� �ʳ�?
		m_iCurFrm++;
		if (m_vecFrm.size() <= m_iCurFrm) {

			//�����ٴ� �ǹ��� -1
			m_iCurFrm = -1;
			m_bFinish = true;
			m_fAccTime = 0.f;
			return;
		}
		m_fAccTime -= m_vecFrm[m_iCurFrm].fDuration;
	}

}

void CAnimation::render(HDC _dc)
{

	if (m_bFinish) return;
	CObject* pObj = m_pAnimator->GetObj();
	Vec2 vPos = pObj->GetPos();

	vPos += m_vecFrm[m_iCurFrm].vOffset;			// Object Position�� Offset��ŭ �߰� �̵� ��ġ. 
	vPos = CCamera::GetInstance()->GetRenderPos(vPos);
	TransparentBlt(_dc,
			(int)(vPos.x - m_vecFrm[m_iCurFrm].vSlice.x / 2.f),
			(int)(vPos.y - m_vecFrm[m_iCurFrm].vSlice.x / 2.f),
			(int)(m_vecFrm[m_iCurFrm].vSlice.x),
			(int)(m_vecFrm[m_iCurFrm].vSlice.y),
			m_pTex->GetDC(),
			(int)(m_vecFrm[m_iCurFrm].vLT.x),
			(int)(m_vecFrm[m_iCurFrm].vLT.y),
			(int)(m_vecFrm[m_iCurFrm].vSlice.x),
			(int)(m_vecFrm[m_iCurFrm].vSlice.y),
			RGB(255,0,255)
		);
}

void CAnimation::Create(CTexture* _pTex, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT iFrameCount)
{
	m_pTex = _pTex;
	tAnimFrm frm = {};
	for (UINT frameIDX = 0; frameIDX < iFrameCount; frameIDX++) {
		frm.fDuration = _fDuration;
		frm.vLT = _vLT + _vStep * (float)frameIDX;
		frm.vSlice = _vSliceSize;

		m_vecFrm.push_back(frm);
	}

}

void CAnimation::Save(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInstance()->GetContentPath();
	strFilePath += _strRelativePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	assert(pFile);
	SaveWstring(m_strName, pFile);

	// Animation�� ����ϴ� �ؽ���
	SaveWstring(m_pTex->GetKey(), pFile);
	SaveWstring(m_pTex->GetRelativePath(), pFile);

	// ������ ����
	size_t iFrameCount = m_vecFrm.size();
	fwrite(&iFrameCount, sizeof(size_t), 1, pFile);

	// ��� ������ ����
	fwrite(m_vecFrm.data(), sizeof(tAnimFrm), iFrameCount, pFile);


	fclose(pFile);
}

void CAnimation::Load(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInstance()->GetContentPath();
	strFilePath += _strRelativePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");
	assert(pFile);

	LoadWstring(m_strName, pFile);

	// �ؽ���
	wstring strTexKey, strTexPath;
	LoadWstring(strTexKey, pFile);
	LoadWstring(strTexPath, pFile);
	m_pTex = CResMgr::GetInstance()->LoadTexture(strTexKey, strTexPath);

	// ������ ����
	size_t iFrameCount = 0;
	fread(&iFrameCount, sizeof(size_t), 1, pFile);

	// ��� ������ ����
	m_vecFrm.resize(iFrameCount);
	fread(m_vecFrm.data(), sizeof(tAnimFrm), iFrameCount, pFile);

	fclose(pFile);
}

