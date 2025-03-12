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
			//m_arrObj[그룹][물체] 삭제. 
			delete m_arrObj[typeIDX][objIDX];
		}	
		//씬이 사라지면, 그 씬의 벡터들도 다 사라짐. 
		//STL의 RAII가 알아서 삭제하기 때문. 
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
//움직이고 했던 걸, 마지막으로 업데이트 함. 
//충돌체가 플레이어 따라가게 함, 충돌 처리. 
void CScene::finalupdate()
{
	for (UINT typeIDX = 0; typeIDX < (UINT)GROUP_TYPE::END; typeIDX++) {
		for (size_t objIDX = 0; objIDX < m_arrObj[typeIDX].size(); objIDX++) {

			//Final Update는 돌려줌. 내부적으로 Component들의 마무리 단계 업데이트(충돌처리나, 참조관계등)
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
				//Dead상태일 경우엔 렌더링에서 삭제하기. 
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
				//Dead상태일 경우엔 렌더링에서 삭제하기. 
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
				//Dead상태일 경우엔 렌더링에서 삭제하기. 
				ObjVecIter = m_arrObj[typeIDX].erase(ObjVecIter);
			}

		}
	}
}

void CScene::render_tile(HDC _dc)
{
	const vector<CObject*> vecTile = GetGroupObject(GROUP_TYPE::TILE);
	//화면 안에 들어오는 애들의 범위를 잡아내어, 들어오는 애들만 렌더링 해준다. 
	Vec2 vCamLook = CCamera::GetInstance()->GetLookAt();
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	Vec2 vLeftTop = vCamLook - vResolution / 2.f;
	Vec2 vRightDown = vCamLook + vResolution / 2.f;

	int iTileSize = TILE_SIZE;

	//Width, HEIGHT로 대체 가능할지도?
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
	//화면 안에 들어오는 애들의 범위를 잡아내어, 들어오는 애들만 렌더링 해준다. 
	Vec2 vCamLook = CCamera::GetInstance()->GetLookAt();
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	Vec2 vLeftTop = vCamLook - vResolution / 2.f;
	Vec2 vRightDown = vCamLook + vResolution / 2.f;

	int iTileSize = TILE_SIZE;

	//Width, HEIGHT로 대체 가능할지도?
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
	//화면 안에 들어오는 애들의 범위를 잡아내어, 들어오는 애들만 렌더링 해준다. 
	Vec2 vCamLook = CCamera::GetInstance()->GetLookAt();
	Vec2 vResolution = CCore::GetInstance()->GetResolution();

	Vec2 vLeftTop = vCamLook - vResolution * 0.5f;
	Vec2 vRightDown = vCamLook + vResolution * 0.5f;

	int iTileSize = TILE_SIZE;

	//Width, HEIGHT로 대체 가능할지도?
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

//추가적인 최적화 방법 -> 커다란 이미지 하나를 두고 그 부분의 일부분만 잘라서 오면 되지 않나?
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

	//타일 생성
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

			//맨윗줄
			else if (tileY == 0 && (tileX >= 1 && tileX <= 34))
			{
				rest = tileX % 7;
				if (rest == 0) rest++;
				tileIdx = rest;
			}
			
			//맨아랫줄
			else if (tileY == 35 && (tileX >= 1 && tileX <= 34))
			{
				rest = tileX % 7;
				if (rest == 0) rest++;
				tileIdx = rest + 56;
			}

			//맨왼쪽줄
			else if (tileX == 0 && (tileY >= 1 && tileY <= 34))
			{
				rest = tileY % 7;
				if (rest == 0) rest++;
				tileIdx = rest * 8;
			}

			//맨 오른쪽
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
	// 각 타일는 원래 MakeTile에서 다음과 같이 배치됨:
	// 위치: (TILE_SIZE/4 + (TILE_SIZE/2 * tileX), TILE_SIZE/4 + (TILE_SIZE/2 * tileY))
	// 크기: TILE_SIZE/2 × TILE_SIZE/2
	// (TILE_SIZE = 64 → offset = 16, tileDrawSize = 32)
	const float offset = 16.0f;
	const float tileDrawSize = 32.0f;

	// 전체 합성 비트맵의 크기 계산: 
	// 좌측 여백 offset에서 시작해 gridCount * tileDrawSize 만큼 확장.
	const UINT compositeWidth = (UINT)(offset + tileDrawSize * gridCount);
	const UINT compositeHeight = compositeWidth; // 정사각형

	// 3. 오프스크린 렌더 타겟(비트맵 렌더 타겟) 생성
	ID2D1Bitmap* pCompositeBitmap = nullptr;
	ComPtr<ID2D1BitmapRenderTarget> pBitmapRT = nullptr;
	HRESULT hr = Direct2DMgr::GetInstance()->GetRenderTarget()->CreateCompatibleRenderTarget(
		D2D1::SizeF((FLOAT)compositeWidth, (FLOAT)compositeHeight),
		&pBitmapRT
	);
	if (FAILED(hr))
		return nullptr;

	pBitmapRT->BeginDraw();
	// 배경색 (예: 흰색)으로 클리어
	pBitmapRT->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	// 4. 그리드 순회: 각 타일의 서브 비트맵 인덱스 결정 후 합성
	for (int tileY = 0; tileY < gridCount; tileY++)
	{
		for (int tileX = 0; tileX < gridCount; tileX++)
		{
			int tileIdx = 0;
			int rest = 0;
			// 모서리 타일 처리
			if (tileY == 0 && tileX == 0)	tileIdx = 0;
			else if (tileY == 0 && tileX == 35)	tileIdx = 7;
			else if (tileY == 35 && tileX == 0)	tileIdx = 56;
			else if (tileY == 35 && tileX == 35)tileIdx = 63;
			// 상단 테두리
			else if (tileY == 0 && (tileX >= 1 && tileX <= 34))
			{
				rest = tileX % 7;
				if (rest == 0) rest++;
				tileIdx = rest;
			}
			// 하단 테두리
			else if (tileY == 35 && (tileX >= 1 && tileX <= 34))
			{
				rest = tileX % 7;
				if (rest == 0) rest++;
				tileIdx = rest + 56;
			}
			// 좌측 테두리
			else if (tileX == 0 && (tileY >= 1 && tileY <= 34))
			{
				rest = tileY % 7;
				if (rest == 0) rest++;
				tileIdx = rest * 8;
			}
			// 우측 테두리
			else if (tileX == 35 && (tileY >= 1 && tileY <= 34))
			{
				rest = tileY % 7;
				if (rest == 0) rest++;
				tileIdx = rest * 8 + 7;
			}
			// 내부 타일: 테두리 제외하고 랜덤하게 (테두리 인덱스 제외)
			else
			{
				int randX = rand() % 7;
				if (randX == 0) randX = 1;
				int randY = rand() % 7;
				if (randY == 0) randY = 1;

				tileIdx = randY * 8 + randX;
			}

			// 5. 타일의 배치 위치 계산
			// MakeTile에서의 위치: X = offset + tileX * tileDrawSize, Y = offset + tileY * tileDrawSize
			float destLeft = offset + tileX * tileDrawSize;
			float destTop = offset + tileY * tileDrawSize;
			D2D1_RECT_F destRect = D2D1::RectF(destLeft, destTop, destLeft + tileDrawSize, destTop + tileDrawSize);

			// 6. 서브 비트맵의 원본 영역: 여기서는 전체 서브 이미지 (크기 TILE_SIZE×TILE_SIZE)
			D2D1_RECT_F srcRect = D2D1::RectF(0, 0, (FLOAT)TILE_SIZE, (FLOAT)TILE_SIZE);

			// 7. 타일 텍스처에서 해당 서브 이미지를 DrawBitmap (타일 이미지가 64x64에서 32x32로 축소됨)
			if (tileIdx >= 0 && tileIdx < (int)splitBitmaps.size())
			{
				pBitmapRT->DrawBitmap(
					splitBitmaps[tileIdx],   // 해당 서브 비트맵
					destRect,                // 합성될 위치 및 크기
					1.0f,                    // 불투명도
					D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
					srcRect                  // 원본 영역
				);
			}
		}
	}

	hr = pBitmapRT->EndDraw();
	if (FAILED(hr))
		return nullptr;

	// 8. 오프스크린 렌더 타겟에서 최종 커다란 비트맵 추출
	hr = pBitmapRT->GetBitmap(&pCompositeBitmap);
	if (FAILED(hr))
		return nullptr;

	return pCompositeBitmap;
}


void CScene::LoadTile(const wstring& _strRelativePath)
{
	wstring strFilePath = CPathMgr::GetInstance()->GetContentPath();
	strFilePath += _strRelativePath;
	//커널 오브젝트
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"rb");

	assert(pFile);

	//데이터 로드.

	UINT xCount = 0;
	UINT yCount = 0;

	fread(&xCount, sizeof(UINT), 1, pFile);
	fread(&yCount, sizeof(UINT), 1, pFile);

	CreateTile(xCount, yCount);

	//모든 타일들을 개별적으로 저장할 데이터를 로드하게 함. 
	const vector<CObject*>& vecTile = GetGroupObject(GROUP_TYPE::TILE);

	for (size_t tileIdx = 0; tileIdx < vecTile.size(); tileIdx++) {
		((CTile*)vecTile[tileIdx])->Load(pFile);
	}

	fclose(pFile);
}

//static 함수. 
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
