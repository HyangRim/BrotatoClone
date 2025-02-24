#include "pch.h"
#include "CTile.h"

#include "CTexture.h"


CTile::CTile()
	: m_pTileTex(nullptr)
	, m_iImgIdx(0)
{
	SetScale(Vec2(TILE_SIZE, TILE_SIZE));
}

CTile::~CTile()
{
	
}
//Tool Scene을 통해서 타일의 배치를 자신이 코드가 아닌 방식으로 배치할 수 있어야 하는데.
//타일을 개별적으로 건드려서, 각 타일의 모양새를 바꿀 수 있어야함. 

//그리고 저장 & 로드가 가능해야함. 
void CTile::update()
{

}

void CTile::render(HDC _dc)
{
	if (m_pTileTex == nullptr || -1 == m_iImgIdx) {
		return;
	}

	UINT iWidth = m_pTileTex->Width();
	UINT iHeight = m_pTileTex->Height();

	UINT IMaxCol = iWidth / TILE_SIZE;
	UINT IMaxRow = iHeight / TILE_SIZE;

	UINT iCurRow = (UINT)(m_iImgIdx / IMaxCol);
	UINT iCurCol = (UINT)(m_iImgIdx % IMaxCol);

	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(GetPos());
	Vec2 vScale = GetScale();

	if (IMaxRow <= iCurRow) {
		assert(nullptr);
	}
	
	BitBlt(_dc,
		(int)(vRenderPos.x),
		(int)(vRenderPos.y),
		(int)(vScale.x),
		(int)(vScale.y),
		m_pTileTex->GetDC(),
		iCurRow * TILE_SIZE,
		iCurCol * TILE_SIZE,
		SRCCOPY);
}

void CTile::render(Gdiplus::Graphics* _pDGraphics)
{
	if (m_pTileTex == nullptr || -1 == m_iImgIdx) {
		return;
	}

	UINT iWidth = m_pTileTex->Width();
	UINT iHeight = m_pTileTex->Height();

	UINT IMaxCol = iWidth / TILE_SIZE;
	UINT IMaxRow = iHeight / TILE_SIZE;

	UINT iCurRow = (UINT)(m_iImgIdx / IMaxCol);
	UINT iCurCol = (UINT)(m_iImgIdx % IMaxCol);

	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(GetPos());
	Vec2 vScale = GetScale();

	if (IMaxRow <= iCurRow) {
		assert(nullptr);
	}
	Gdiplus::Rect srcRect(iCurCol * TILE_SIZE, iCurRow * TILE_SIZE, TILE_SIZE, TILE_SIZE);
	Gdiplus::Rect destRect((int)vRenderPos.x, (int)vRenderPos.y, (int)vScale.x, (int)vScale.y);

	_pDGraphics->DrawImage(
		m_pTileTex->GetBitmap(),
		destRect,
		srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height,
		Gdiplus::UnitPixel
	);
}

void CTile::render(ID2D1HwndRenderTarget* _pRender)
{
	component_render(_pRender);
	/*
	if (m_pTileTex == nullptr || -1 == m_iImgIdx) {
		return;
	}

	D2D1_SIZE_F bitmapSize = m_pTileBit->GetSize();

	UINT iWidth = static_cast<UINT>(bitmapSize.width);
	UINT iHeight = static_cast<UINT>(bitmapSize.height);

	UINT IMaxCol = iWidth / TILE_SIZE;
	UINT IMaxRow = iHeight / TILE_SIZE;

	UINT iCurRow = m_iImgIdx / IMaxCol;
	UINT iCurCol = m_iImgIdx % IMaxCol;

	if (iCurRow >= IMaxRow) {
		assert(false);
		return;
	}

	// 렌더링할 위치와 크기 계산
	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(GetPos());
	Vec2 vScale = GetScale();

	// 소스 사각형: 비트맵에서 가져올 영역
	D2D1_RECT_F srcRect = D2D1::RectF(
		static_cast<FLOAT>(iCurCol * TILE_SIZE),
		static_cast<FLOAT>(iCurRow * TILE_SIZE),
		static_cast<FLOAT>((iCurCol + 1) * TILE_SIZE),
		static_cast<FLOAT>((iCurRow + 1) * TILE_SIZE)
	);

	// 대상 사각형: 화면에 그릴 위치와 크기
	D2D1_RECT_F destRect = D2D1::RectF(
		vRenderPos.x - vScale.x / 2.f,
		vRenderPos.y - vScale.y / 2.f,
		vRenderPos.x + vScale.x / 2.f,
		vRenderPos.y + vScale.y / 2.f
	);

	// 비트맵 그리기
	_pRender->DrawBitmap(
		m_pTileBit,
		destRect,
		1.0f, // 불투명도
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		srcRect
	);
	*/
}

void CTile::Save(FILE* _pFile)
{
	fwrite(&m_iImgIdx, sizeof(int), 1, _pFile);
}

void CTile::Load(FILE* _pFile)
{
	fread(&m_iImgIdx, sizeof(int), 1, _pFile);
}
