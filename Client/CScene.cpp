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
		/*
		if ((UINT)GROUP_TYPE::MONSTER == typeIDX) {
			render_monster(_dc);
			continue;
		}
		*/

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
		/*
		if ((UINT)GROUP_TYPE::TILE == typeIDX && (GetGroupObject(GROUP_TYPE::TILE).size() > 0)) {
			render_tile(_pDGraphics);
			continue;
		}*/
		/*
		if ((UINT)GROUP_TYPE::MONSTER == typeIDX) {
			render_monster(_dc);
			continue;
		}
		*/

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

			vecTile[iIdx]->render(_pDGraphics);
		}
	}
}

//�߰����� ����ȭ ��� -> Ŀ�ٶ� �̹��� �ϳ��� �ΰ� �� �κ��� �Ϻκи� �߶� ���� ���� �ʳ�?
void CScene::render_monster(HDC _dc)
{
}

void CScene::DeleteGroup(GROUP_TYPE _eGroup)
{
	Safe_Delete_Vec<CObject*>(m_arrObj[(UINT)_eGroup]);
}

void CScene::DeleteAll()
{

	for (UINT GroupIdx = 0; GroupIdx < (UINT)GROUP_TYPE::END; GroupIdx++) {
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

void CScene::MakeTile()
{
	DeleteGroup(GROUP_TYPE::TILE);
	//Ÿ�� ����
	CTexture* pTileTex = CResMgr::GetInstance()->LoadTexture(L"Tile Outline", L"texture\\tiles_outline.png");

	m_iTileX = pTileTex->Width() / TILE_SIZE;
	m_iTileY = pTileTex->Height() / TILE_SIZE;

	for (UINT y = 0; y < m_iTileY; ++y)
	{
		for (UINT x = 0; x < m_iTileX; ++x)
		{
			CTile* pTile = new CTile();
			
			pTile->SetTexture(pTileTex);

			// �̹��� �ε��� ���� (�ʿ� �� Ȱ�� ����)
			int imgIdx = y * m_iTileX + x;
			pTile->AddImgIdx(); // imgIdx�� ������Ű�� ������ �ʿ信 ���� ���� ����

			// Ÿ���� ��ġ ���� (��: ���� ��ǥ�迡���� ��ġ)
			pTile->SetPos(Vec2((float)(x * TILE_SIZE), (float)(y * TILE_SIZE)));
			pTile->SetTexture(pTileTex);



			// �׷쿡 Ÿ�� �߰�
			AddObject(pTile, GROUP_TYPE::TILE);
		}
	}
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
