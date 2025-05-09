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


//발쪽으로 애니메이션을 맞춤...
//탑뷰 형태의 게임은 발쪽으로. 따라서 총알또한 그만큼 아래로 내려줌...

CAnimation::~CAnimation()
{

}

void CAnimation::update()
{
	if (m_bFinish) return;
	m_fAccTime += fDT;
	//프레임이 계속 돌아가는데, 만약 내가 한 번 재생만 원한다면?
	if (m_vecFrm[m_iCurFrm].fDuration < m_fAccTime) {

		//일시 정지해서 fDT가 2.344면 프레임 Duration 뺀 값을 넣는게 맞지 않나?
		m_iCurFrm++;
		if (m_vecFrm.size() <= m_iCurFrm) {

			//끝났다는 의미의 -1
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

	vPos += m_vecFrm[m_iCurFrm].vOffset;			// Object Position에 Offset만큼 추가 이동 위치. 
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

void CAnimation::render(Gdiplus::Graphics* _pDGraphics)
{
	if (m_bFinish) return;
	CObject* pObj = m_pAnimator->GetObj();
	Vec2 vPos = pObj->GetPos();

	vPos += m_vecFrm[m_iCurFrm].vOffset;			// Object Position에 Offset만큼 추가 이동 위치. 
	vPos = CCamera::GetInstance()->GetRenderPos(vPos);

	// 대상(화면)에 그릴 사각형: 원래 TransparentBlt의 대상 좌표와 크기와 동일하게 계산
	int destX = (int)(vPos.x - m_vecFrm[m_iCurFrm].vSlice.x / 2.f);
	int destY = (int)(vPos.y - m_vecFrm[m_iCurFrm].vSlice.x / 2.f);  // 원본 코드에서는 vSlice.x가 y 좌표에도 사용됨
	int destW = (int)(m_vecFrm[m_iCurFrm].vSlice.x);
	int destH = (int)(m_vecFrm[m_iCurFrm].vSlice.y);
	Gdiplus::Rect destRect(destX, destY, destW, destH);


	// 원본 이미지에서 가져올 영역 (소스 사각형)
	int srcX = (int)(m_vecFrm[m_iCurFrm].vLT.x);
	int srcY = (int)(m_vecFrm[m_iCurFrm].vLT.y);
	int srcW = destW; // 투명 블리팅 시 width와 height는 동일
	int srcH = destH;
	Gdiplus::Rect srcRect(srcX, srcY, srcW, srcH);

	_pDGraphics->DrawImage(
		m_pTex->GetBitmap(),
		destRect,
		srcRect.X, srcRect.Y,
		srcRect.Width, srcRect.Height,
		Gdiplus::UnitPixel);
}

void CAnimation::render(ID2D1HwndRenderTarget* _pRender)
{

	if (m_bFinish) return;
	CObject* pObj = m_pAnimator->GetObj();
	Vec2 vPos = pObj->GetPos();


	Vec2 vScale = pObj->GetScale();
	Vec2 vWaveScale = pObj->GetRenderScale();
	Vec2 vFinalScale = vWaveScale / vScale;


	vPos += m_vecFrm[m_iCurFrm].vOffset;			// Object Position에 Offset만큼 추가 이동 위치. 
	vPos = CCamera::GetInstance()->GetRenderPos(vPos);



	// 목적지 사각형 계산
	float destLeft = vPos.x - (m_vecFrm[m_iCurFrm].vSlice.x / 2.f) * vFinalScale.x;
	float destTop = vPos.y - (m_vecFrm[m_iCurFrm].vSlice.y / 2.f) * vFinalScale.y;
	float destRight = vPos.x + (m_vecFrm[m_iCurFrm].vSlice.x / 2.f) * vFinalScale.x;
	float destBottom = vPos.y + (m_vecFrm[m_iCurFrm].vSlice.y / 2.f) * vFinalScale.y;
	
	//float destRight = destLeft + m_vecFrm[m_iCurFrm].vSlice.x;
	//float destBottom = destTop + m_vecFrm[m_iCurFrm].vSlice.y;
	D2D1_RECT_F destRect = D2D1::RectF(destLeft, destTop, destRight, destBottom);
	int a = 1213;

	// 소스 사각형 계산 (이미지 내에서 잘라낼 영역)
	float srcLeft = m_vecFrm[m_iCurFrm].vLT.x;
	float srcTop = m_vecFrm[m_iCurFrm].vLT.y;
	float srcRight = srcLeft + m_vecFrm[m_iCurFrm].vSlice.x;
	float srcBottom = srcTop + m_vecFrm[m_iCurFrm].vSlice.y;
	D2D1_RECT_F srcRect = D2D1::RectF(srcLeft, srcTop, srcRight, srcBottom);


	_pRender->DrawBitmap(m_pBitTex, destRect, 1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, srcRect);
}

void CAnimation::Create(CTexture* _pTex, ID2D1Bitmap* _pBit, Vec2 _vLT, Vec2 _vSliceSize, Vec2 _vStep, float _fDuration, UINT iFrameCount)
{
	m_pTex = _pTex;
	m_pBitTex = _pBit;

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

	// Animation이 사용하는 텍스쳐
	SaveWstring(m_pTex->GetKey(), pFile);
	SaveWstring(m_pTex->GetRelativePath(), pFile);

	// 프레임 개수
	size_t iFrameCount = m_vecFrm.size();
	fwrite(&iFrameCount, sizeof(size_t), 1, pFile);

	// 모든 프레임 정보
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

	// 텍스쳐
	wstring strTexKey, strTexPath;
	LoadWstring(strTexKey, pFile);
	LoadWstring(strTexPath, pFile);
	m_pTex = CResMgr::GetInstance()->LoadTexture(strTexKey, strTexPath);

	// 프레임 개수
	size_t iFrameCount = 0;
	fread(&iFrameCount, sizeof(size_t), 1, pFile);

	// 모든 프레임 정보
	m_vecFrm.resize(iFrameCount);
	fread(m_vecFrm.data(), sizeof(tAnimFrm), iFrameCount, pFile);

	fclose(pFile);
}

