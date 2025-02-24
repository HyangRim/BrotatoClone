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
//Tool Scene�� ���ؼ� Ÿ���� ��ġ�� �ڽ��� �ڵ尡 �ƴ� ������� ��ġ�� �� �־�� �ϴµ�.
//Ÿ���� ���������� �ǵ����, �� Ÿ���� ������ �ٲ� �� �־����. 

//�׸��� ���� & �ε尡 �����ؾ���. 
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

	// �������� ��ġ�� ũ�� ���
	Vec2 vRenderPos = CCamera::GetInstance()->GetRenderPos(GetPos());
	Vec2 vScale = GetScale();

	// �ҽ� �簢��: ��Ʈ�ʿ��� ������ ����
	D2D1_RECT_F srcRect = D2D1::RectF(
		static_cast<FLOAT>(iCurCol * TILE_SIZE),
		static_cast<FLOAT>(iCurRow * TILE_SIZE),
		static_cast<FLOAT>((iCurCol + 1) * TILE_SIZE),
		static_cast<FLOAT>((iCurRow + 1) * TILE_SIZE)
	);

	// ��� �簢��: ȭ�鿡 �׸� ��ġ�� ũ��
	D2D1_RECT_F destRect = D2D1::RectF(
		vRenderPos.x - vScale.x / 2.f,
		vRenderPos.y - vScale.y / 2.f,
		vRenderPos.x + vScale.x / 2.f,
		vRenderPos.y + vScale.y / 2.f
	);

	// ��Ʈ�� �׸���
	_pRender->DrawBitmap(
		m_pTileBit,
		destRect,
		1.0f, // ������
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
