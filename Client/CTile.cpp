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

void CTile::Save(FILE* _pFile)
{
	fwrite(&m_iImgIdx, sizeof(int), 1, _pFile);
}

void CTile::Load(FILE* _pFile)
{
	fread(&m_iImgIdx, sizeof(int), 1, _pFile);
}
