#include "pch.h"
#include "CScene.h"
#include "CObject.h"
#include "func.h"
#include "CTile.h"
#include "CTimeMgr.h"
#include "CResMgr.h"
#include "CPathMgr.h"
#include "CCamera.h"
#include "CCore.h"
#include "CTexture.h"
#include "Direct2DMgr.h"
#include "CMonster.h"
#include "CGround.h"
#include "CImage.h"



bool CScene::isPause = false;
const float tile_Devide = 0.15625;

CScene::CScene()

	: m_iTileX(0)
	, m_iTileY(0)
	, m_pPlayer(nullptr)
{
}


CScene::~CScene()
{
	for (UINT typeIDX = 0; typeIDX < (UINT)GROUP_TYPE::END; typeIDX++) {
		for (size_t objIDX = 0; objIDX < m_arrObj[typeIDX].size(); objIDX++) {
			//m_arrObj[�׷�][��ü] ����. 
			delete m_arrObj[typeIDX][objIDX];
		}	
		//���� �������, �� ���� ���͵鵵 �� �����. 
		//STL�� RAII�� �˾Ƽ� �����ϱ� ����. 
	}
}

void CScene::AllDropItemRetrieve()
{
	UINT typeIDX = (UINT)GROUP_TYPE::DROP_ITEM;
	for (size_t objIDX = 0; objIDX < m_arrObj[typeIDX].size(); objIDX++) {

		CMonster* dropItem = static_cast<CMonster*>(m_arrObj[typeIDX][objIDX]);

		if (dropItem->Getinfo().m_eMonType == MON_TYPE::DROP_ITEM) {
			dropItem->SetRecogRange(9999.f);
			dropItem->SetSpeed(500.f);
		}
	}
}

void CScene::start()
{
	for (UINT typeIDX = 0; typeIDX < (UINT)GROUP_TYPE::END; typeIDX++) {
		for (size_t objIDX = 0; objIDX < m_arrObj[typeIDX].size(); objIDX++) {
			m_arrObj[typeIDX][objIDX]->start();
		}
	}
}

void CScene::update()
{
	for (UINT typeIDX = 0; typeIDX < (UINT)GROUP_TYPE::END; typeIDX++) {
		for (size_t objIDX = 0; objIDX < m_arrObj[typeIDX].size(); objIDX++) {
			if (!m_arrObj[typeIDX][objIDX]->IsDead()) {
				m_arrObj[typeIDX][objIDX]->update();
			}
		}
	}
}
//�����̰� �ߴ� ��, ���������� ������Ʈ ��. 
//�浹ü�� �÷��̾� ���󰡰� ��, �浹 ó��. 
void CScene::finalupdate()
{
	for (UINT typeIDX = 0; typeIDX < (UINT)GROUP_TYPE::END; typeIDX++) {
		for (size_t objIDX = 0; objIDX < m_arrObj[typeIDX].size(); objIDX++) {

			//Final Update�� ������. ���������� Component���� ������ �ܰ� ������Ʈ(�浹ó����, ���������)
			m_arrObj[typeIDX][objIDX]->finalupdate();
		}
	}
}

void CScene::render(HDC _dc)
{
	for (UINT typeIDX = 0; typeIDX < (UINT)GROUP_TYPE::END; typeIDX++) {
		if ((UINT)GROUP_TYPE::TILE == typeIDX) {
			render_tile(_dc);
			continue;
		}

		auto ObjVecIter = m_arrObj[typeIDX].begin();

		for (; ObjVecIter != m_arrObj[typeIDX].end();) {
			if (!(*ObjVecIter)->IsDead()) {
				(*ObjVecIter)->render(_dc);
				ObjVecIter++;
			}
			else {
				//Dead������ ��쿣 ���������� �����ϱ�. 
				ObjVecIter = m_arrObj[typeIDX].erase(ObjVecIter);
			}

		}
	}
}

void CScene::render(Gdiplus::Graphics* _pDGraphics)
{
	
	for (UINT typeIDX = 0; typeIDX < (UINT)GROUP_TYPE::END; typeIDX++) {

		auto ObjVecIter = m_arrObj[typeIDX].begin();

		for (; ObjVecIter != m_arrObj[typeIDX].end();) {
			if (!(*ObjVecIter)->IsDead()) {
				(*ObjVecIter)->render(_pDGraphics);
				ObjVecIter++;
			}
			else {
				//Dead������ ��쿣 ���������� �����ϱ�. 
				ObjVecIter = m_arrObj[typeIDX].erase(ObjVecIter);
			}

		}
	}
}

void CScene::render(ID2D1HwndRenderTarget* _pRender)
{
	for (UINT typeIDX = 0; typeIDX < (UINT)GROUP_TYPE::END; typeIDX++) {
		auto ObjVecIter = m_arrObj[typeIDX].begin();

		for (; ObjVecIter != m_arrObj[typeIDX].end();) {
			if (!(*ObjVecIter)->IsDead()) {
				(*ObjVecIter)->render(_pRender);
				ObjVecIter++;
			}
			else {
				//Dead������ ��쿣 ���������� �����ϱ�. 
				ObjVecIter = m_arrObj[typeIDX].erase(ObjVecIter);
			}

		}
	}
}

void CScene::render_tile(HDC _dc)
{
	const vector<CObject*> vecTile = GetGroupObject(GROUP_TYPE::TILE);
	//ȭ�� �ȿ� ������ �ֵ��� ������ ��Ƴ���, ������ �ֵ鸸 ������ ���ش�. 
	Vec2 vCamLook = CCamera::GetInstance()->GetLookAt();
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	Vec2 vLeftTop = vCamLook - vResolution / 2.f;
	Vec2 vRightDown = vCamLook + vResolution / 2.f;

	int iTileSize = TILE_SIZE;

	//Width, HEIGHT�� ��ü ����������?
	int iLTCol = (int)vLeftTop.x / iTileSize;
	int iLTRow = (int)vLeftTop.y / iTileSize;

	int iLTIdx = (m_iTileX * iLTRow) + iLTCol;

	int iClientWidth = (int)vResolution.x / iTileSize + 1;
	int iClientHeight = (int)vResolution.y / iTileSize + 1;

	for (int iCurRow = iLTRow; iCurRow < (iLTRow + iClientHeight); iCurRow++) {

		for (int iCurcol = iLTCol; iCurcol < (iLTCol + iClientWidth); iCurcol++) {
			if (iCurcol < 0 || m_iTileX <= (UINT)iCurcol ||
				iCurRow < 0 || m_iTileY <= (UINT)iCurRow) continue;

			int iIdx = (m_iTileX * iCurRow) + iCurcol;

			vecTile[iIdx]->render(_dc);
		}

	}
}

void CScene::render_tile(Gdiplus::Graphics* _pDGraphics)
{
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);
	//ȭ�� �ȿ� ������ �ֵ��� ������ ��Ƴ���, ������ �ֵ鸸 ������ ���ش�. 
	Vec2 vCamLook = CCamera::GetInstance()->GetLookAt();
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	Vec2 vLeftTop = vCamLook - vResolution / 2.f;
	Vec2 vRightDown = vCamLook + vResolution / 2.f;

	int iTileSize = TILE_SIZE;

	//Width, HEIGHT�� ��ü ����������?
	int iLTCol = (int)vLeftTop.x / iTileSize;
	int iLTRow = (int)vLeftTop.y / iTileSize;

	int iLTIdx = (m_iTileX * iLTRow) + iLTCol;

	int iClientWidth = (int)vResolution.x / iTileSize + 1;
	int iClientHeight = (int)vResolution.y / iTileSize + 1;

	for (int iCurRow = iLTRow; iCurRow < (iLTRow + iClientHeight); iCurRow++) {

		for (int iCurcol = iLTCol; iCurcol < (iLTCol + iClientWidth); iCurcol++) {
			
			if (iCurcol < 0 || m_iTileX <= (UINT)iCurcol ||
				iCurRow < 0 || m_iTileY <= (UINT)iCurRow) continue;
				
			int iIdx = (m_iTileX * iCurRow) + iCurcol;

			vecTile[iIdx]->render(_pDGraphics);
		}
	}
}

void CScene::render_tile(ID2D1HwndRenderTarget* _pRender)
{
	const vector<CObject*> vecTile = GetGroupObject(GROUP_TYPE::TILE);
	//ȭ�� �ȿ� ������ �ֵ��� ������ ��Ƴ���, ������ �ֵ鸸 ������ ���ش�. 
	Vec2 vCamLook = CCamera::GetInstance()->GetLookAt();
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	Vec2 vLeftTop = vCamLook - vResolution * 0.5f;
	Vec2 vRightDown = vCamLook + vResolution * 0.5f;

	int iTileSize = TILE_SIZE;

	//Width, HEIGHT�� ��ü ����������?
	int iLTCol = (int)vLeftTop.x / iTileSize;
	int iLTRow = (int)vLeftTop.y / iTileSize;

	
	int iLTIdx = (m_iTileX * iLTRow) + iLTCol;

	int iClientWidth = (int)vResolution.x / iTileSize + 1;
	int iClientHeight = (int)vResolution.y / iTileSize + 1;

	for (int iCurRow = iLTRow; iCurRow < (iLTRow + iClientHeight); iCurRow++) {

		for (int iCurcol = iLTCol; iCurcol < (iLTCol + iClientWidth); iCurcol++) {
			if (iCurcol < 0 || m_iTileX <= (UINT)iCurcol ||
				iCurRow < 0 || m_iTileY <= (UINT)iCurRow) continue;

			int iIdx = (m_iTileX * iCurRow) + iCurcol;

			vecTile[iIdx]->render(_pRender);
		}
	}
}

//�߰����� ����ȭ ��� -> Ŀ�ٶ� �̹��� �ϳ��� �ΰ� �� �κ��� �Ϻκи� �߶� ���� ���� �ʳ�?
void CScene::render_monster(HDC _dc)
{
}

void CScene::render_monster(Gdiplus::Graphics* _pDGraphics)
{
}

void CScene::render_monster(ID2D1HwndRenderTarget* _pRender)
{
}

void CScene::DeleteGroup(GROUP_TYPE _eGroup)
{
	Safe_Delete_Vec<CObject*>(m_arrObj[(UINT)_eGroup]);
}

void CScene::DeleteAll()
{
	for (UINT GroupIdx = 0; GroupIdx < (UINT)GROUP_TYPE::END; GroupIdx++) {
		if (GroupIdx == (UINT)GROUP_TYPE::PLAYER)
		{
			m_arrObj[(UINT)GROUP_TYPE::PLAYER].clear();
			continue;
		}
		else if (GroupIdx == (UINT)GROUP_TYPE::WEAPON) {
			m_arrObj[(UINT)GROUP_TYPE::WEAPON].clear();
		}
		DeleteGroup((GROUP_TYPE)GroupIdx);
	}
}

void CScene::CreateTile(UINT _IXCount, UINT _IYCount)
{
	DeleteGroup(GROUP_TYPE::TILE);

	m_iTileX = _IXCount;
	m_iTileY = _IYCount;

	//Ÿ�� ����
	//CTexture* pTileTex = CResMgr::GetInstance()->LoadTexture(L"Tile", L"texture\\tera2.bmp");
	CTexture* pTileTex = CResMgr::GetInstance()->LoadTexture(L"Tile Outline", L"texture\\result2.png");
	for (UINT tileIDX = 0; tileIDX < _IYCount; tileIDX++) {
		for (UINT tileJDX = 0; tileJDX < _IXCount; tileJDX++) {
			CTile* pTile = new CTile();

			pTile->SetPos(Vec2((float)(tileJDX * TILE_SIZE), (float)(tileIDX * TILE_SIZE)));
			pTile->SetTexture(pTileTex);

			if (tileIDX > 0 && tileIDX < m_iTileX - 1 && tileJDX > 0 && tileJDX < m_iTileY - 1)
			{
				int tmp = rand() % 64;
				int col = tmp / 8;
				int row = tmp % 8;

				while (col == 0 || col == 7 || 
					row == 0 || row == 7)
				{
					tmp = rand() % 64;
					col = tmp / 8;
					row = tmp % 8;
				}

				pTile->SetImgIdx(tmp);
			}

			if (tileIDX == 0 && tileJDX == 0) pTile->SetImgIdx(0);
			if (tileIDX == 0 && tileJDX == 17) pTile->SetImgIdx(7);
			if (tileIDX == 17 && tileJDX == 0) pTile->SetImgIdx(56);
			if (tileIDX == 17 && tileJDX == 17) pTile->SetImgIdx(63);

			if (tileIDX == 0 && tileJDX != 0 && tileJDX != 17)
			{
				int tmp = tileJDX % 7;
				if (tmp == 0) tmp++;
				pTile->SetImgIdx(tmp);
			}
			if (tileJDX == 0 && tileIDX != 0 && tileIDX != 17)
			{
				int tmp = tileIDX % 7;
				if (tmp == 0) tmp++;
				pTile->SetImgIdx(tmp * 8);
			}
			if (tileIDX == 17 && tileJDX != 0 && tileJDX != 17)
			{
				int tmp = tileJDX % 7;
				if (tmp == 0) tmp++;
				pTile->SetImgIdx(tmp + 56);
			}
			if (tileJDX == 17 && tileIDX != 0 && tileIDX != 17)
			{
				int tmp = tileIDX % 7;
				if (tmp == 0) tmp++;
				pTile->SetImgIdx(tmp * 8 + 7 );
			}

			AddObject(pTile, GROUP_TYPE::TILE);
		}
	}
}

void CScene::MakeTile(const wstring &tag)
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	auto splitBitmaps = Direct2DMgr::GetInstance()->GetSplitBitmaps(tag);

	int tileIdx = 0;
	int rest = 0;
	for (int tileY = 0; tileY < 36; tileY++)
	{
		for (int tileX = 0; tileX < 36; tileX++)
		{
			CObject* pObj = new CTile;
			pObj->SetPos(Vec2((float)TILE_SIZE / 4.f + ((float)TILE_SIZE / 2.f * (float)(tileX))
				, (float)TILE_SIZE / 4.f + ((float)TILE_SIZE / 2.f * (float)(tileY))));
			pObj->SetScale(Vec2((float)TILE_SIZE / 2.f, (float)TILE_SIZE / 2.f));
			pObj->SetName(L"TILE");
			pObj->CreateImage();

			if (tileY == 0 && tileX == 0) tileIdx = 0;
			else if (tileY == 0 && tileX == 35) tileIdx = 7;
			else if (tileY == 35 && tileX == 0) tileIdx = 56;
			else if (tileY == 35 && tileX == 35) tileIdx = 63;

			//������
			else if (tileY == 0 && (tileX >= 1 && tileX <= 34))
			{
				rest = tileX % 7;
				if (rest == 0) rest++;
				tileIdx = rest;
			}
			
			//�ǾƷ���
			else if (tileY == 35 && (tileX >= 1 && tileX <= 34))
			{
				rest = tileX % 7;
				if (rest == 0) rest++;
				tileIdx = rest + 56;
			}

			//�ǿ�����
			else if (tileX == 0 && (tileY >= 1 && tileY <= 34))
			{
				rest = tileY % 7;
				if (rest == 0) rest++;
				tileIdx = rest * 8;
			}

			//�� ������
			else if (tileX == 35 && (tileY >= 1 && tileY <= 34))
			{
				rest = tileY % 7;
				if (rest == 0) rest++;
				tileIdx = rest * 8 + 7;
			}

			else
			{
				int randX = rand() % 7;
				if (randX == 0) randX = 1;
				int randY = rand() % 7;
				if (randY == 0) randY = 1;

				tileIdx = randY * 8 + randX;
			}
		
			pObj->AddImage(splitBitmaps[tileIdx]);
			AddObject(pObj, GROUP_TYPE::TILE);
		}
	}
}

ID2D1Bitmap* CScene::CreateCompositeMapBitmap(const wstring &tag)
{
	Direct2DMgr* pD2DMgr = Direct2DMgr::GetInstance();
	auto splitBitmaps = Direct2DMgr::GetInstance()->GetSplitBitmaps(tag);

	const int gridCount = 36;
	// �� Ÿ�ϴ� ���� MakeTile���� ������ ���� ��ġ��:
	// ��ġ: (TILE_SIZE/4 + (TILE_SIZE/2 * tileX), TILE_SIZE/4 + (TILE_SIZE/2 * tileY))
	// ũ��: TILE_SIZE/2 �� TILE_SIZE/2
	// (TILE_SIZE = 64 �� offset = 16, tileDrawSize = 32)
	const float offset = 16.0f;
	const float tileDrawSize = 32.0f;

	// ��ü �ռ� ��Ʈ���� ũ�� ���: 
	// ���� ���� offset���� ������ gridCount * tileDrawSize ��ŭ Ȯ��.
	const UINT compositeWidth = (UINT)(offset + tileDrawSize * gridCount);
	const UINT compositeHeight = compositeWidth; // ���簢��

	// 3. ������ũ�� ���� Ÿ��(��Ʈ�� ���� Ÿ��) ����
	ID2D1Bitmap* pCompositeBitmap = nullptr;
	ComPtr<ID2D1BitmapRenderTarget> pBitmapRT = nullptr;
	HRESULT hr = Direct2DMgr::GetInstance()->GetRenderTarget()->CreateCompatibleRenderTarget(
		D2D1::SizeF((FLOAT)compositeWidth, (FLOAT)compositeHeight),
		&pBitmapRT
	);
	if (FAILED(hr))
		return nullptr;

	pBitmapRT->BeginDraw();
	// ���� (��: ���)���� Ŭ����
	pBitmapRT->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	// 4. �׸��� ��ȸ: �� Ÿ���� ���� ��Ʈ�� �ε��� ���� �� �ռ�
	for (int tileY = 0; tileY < gridCount; tileY++)
	{
		for (int tileX = 0; tileX < gridCount; tileX++)
		{
			int tileIdx = 0;
			int rest = 0;
			// �𼭸� Ÿ�� ó��
			if (tileY == 0 && tileX == 0)	tileIdx = 0;
			else if (tileY == 0 && tileX == 35)	tileIdx = 7;
			else if (tileY == 35 && tileX == 0)	tileIdx = 56;
			else if (tileY == 35 && tileX == 35)tileIdx = 63;
			// ��� �׵θ�
			else if (tileY == 0 && (tileX >= 1 && tileX <= 34))
			{
				rest = tileX % 7;
				if (rest == 0) rest++;
				tileIdx = rest;
			}
			// �ϴ� �׵θ�
			else if (tileY == 35 && (tileX >= 1 && tileX <= 34))
			{
				rest = tileX % 7;
				if (rest == 0) rest++;
				tileIdx = rest + 56;
			}
			// ���� �׵θ�
			else if (tileX == 0 && (tileY >= 1 && tileY <= 34))
			{
				rest = tileY % 7;
				if (rest == 0) rest++;
				tileIdx = rest * 8;
			}
			// ���� �׵θ�
			else if (tileX == 35 && (tileY >= 1 && tileY <= 34))
			{
				rest = tileY % 7;
				if (rest == 0) rest++;
				tileIdx = rest * 8 + 7;
			}
			// ���� Ÿ��: �׵θ� �����ϰ� �����ϰ� (�׵θ� �ε��� ����)
			else
			{
				int randX = rand() % 7;
				if (randX == 0) randX = 1;
				int randY = rand() % 7;
				if (randY == 0) randY = 1;

				tileIdx = randY * 8 + randX;
			}

			// 5. Ÿ���� ��ġ ��ġ ���
			// MakeTile������ ��ġ: X = offset + tileX * tileDrawSize, Y = offset + tileY * tileDrawSize
			float destLeft = offset + tileX * tileDrawSize;
			float destTop = offset + tileY * tileDrawSize;
			D2D1_RECT_F destRect = D2D1::RectF(destLeft, destTop, destLeft + tileDrawSize, destTop + tileDrawSize);

			// 6. ���� ��Ʈ���� ���� ����: ���⼭�� ��ü ���� �̹��� (ũ�� TILE_SIZE��TILE_SIZE)
			D2D1_RECT_F srcRect = D2D1::RectF(0, 0, (FLOAT)TILE_SIZE, (FLOAT)TILE_SIZE);

			// 7. Ÿ�� �ؽ�ó���� �ش� ���� �̹����� DrawBitmap (Ÿ�� �̹����� 64x64���� 32x32�� ��ҵ�)
			if (tileIdx >= 0 && tileIdx < (int)splitBitmaps.size())
			{
				pBitmapRT->DrawBitmap(
					splitBitmaps[tileIdx],   // �ش� ���� ��Ʈ��
					destRect,                // �ռ��� ��ġ �� ũ��
					1.0f,                    // ������
					D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
					srcRect                  // ���� ����
				);
			}
		}
	}

	hr = pBitmapRT->EndDraw();
	if (FAILED(hr))
		return nullptr;

	// 8. ������ũ�� ���� Ÿ�ٿ��� ���� Ŀ�ٶ� ��Ʈ�� ����
	hr = pBitmapRT->GetBitmap(&pCompositeBitmap);
	if (FAILED(hr))
		return nullptr;

	return pCompositeBitmap;
}


void CScene::LoadTile(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInstance()->GetContentPath();
	strFilePath += _strRelativePath;
	//Ŀ�� ������Ʈ
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	assert(pFile);

	//������ �ε�.

	UINT xCount = 0;
	UINT yCount = 0;

	fread(&xCount, sizeof(UINT), 1, pFile);
	fread(&yCount, sizeof(UINT), 1, pFile);

	CreateTile(xCount, yCount);

	//��� Ÿ�ϵ��� ���������� ������ �����͸� �ε��ϰ� ��. 
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	for (size_t tileIdx = 0; tileIdx < vecTile.size(); tileIdx++) {
		((CTile*)vecTile[tileIdx])->Load(pFile);
	}

	fclose(pFile);
}

//static �Լ�. 
void CScene::ChangePause(bool _bPause)
{
	if (true == _bPause) {
		CScene::isPause = _bPause;
		CTimeMgr::GetInstance()->SetTimeScale(0.f);
	}
	else if (false == _bPause) {
		CScene::isPause = _bPause;
		CTimeMgr::GetInstance()->SetTimeScale(1.f);
	}
}
