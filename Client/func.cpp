#include "pch.h"
#include "func.h"
#include "CObject.h"
#include "CEventMgr.h"
#include "CPathMgr.h"

void CreateObject(CObject* _pObj, GROUP_TYPE _eGroup)
{
	tEvent createEvn = {};
	createEvn.eEven = EVENT_TYPE::CREATE_OBJECT;
	createEvn.lParam = (DWORD_PTR)_pObj;
	createEvn.wParam = (DWORD_PTR)_eGroup;

	CEventMgr::GetInstance()->AddEvent(createEvn);
}

void DeleteObject(CObject* _pObj)
{
	tEvent deleteEvn = {};
	deleteEvn.eEven = EVENT_TYPE::DELETE_OBJECT;
	deleteEvn.lParam = (DWORD_PTR)_pObj;

	CEventMgr::GetInstance()->AddEvent(deleteEvn);
}

void ChangeScene(SCENE_TYPE _eNext)
{
	tEvent changeEvn = {};
	changeEvn.eEven = EVENT_TYPE::SCENE_CHANGE;
	changeEvn.lParam = (DWORD_PTR)_eNext;

	CEventMgr::GetInstance()->AddEvent(changeEvn);
}

void ChangeAIState(AI* _pAI, MON_STATE _eNextState)
{
	tEvent changeEvn = {};
	changeEvn.eEven = EVENT_TYPE::CHANGE_AI_STATE;
	changeEvn.lParam = (DWORD_PTR)_pAI;
	changeEvn.wParam = (DWORD_PTR)_eNextState;

	CEventMgr::GetInstance()->AddEvent(changeEvn);
}

void Fscanf(char* _pOutBuff, FILE* _pFile)
{
	int idx = 0;
	while (true) {
		char c = (char)getc(_pFile);
		if (c == '\n') {
			_pOutBuff[idx++] = '\0';
			break;
		}

		_pOutBuff[idx++] = c;
	}

}

void SaveWstring(const wstring& _str, FILE* _pFile)
{
	//Animation의 이름을 저장한다.(데이터 직렬화)
	const wchar_t* pStrName = _str.c_str();
	size_t iLen = _str.length();

	//문장 길이 저장. 
	fwrite(&iLen, sizeof(size_t), 1, _pFile);

	//문자열 저장. 
	fwrite(pStrName, sizeof(wchar_t), iLen, _pFile);
}

void LoadWstring(wstring& _str, FILE* _pFile)
{
	size_t iLen = 0;
	wchar_t szBuffer[256] = {};
	//문장 길이 저장. 
	fread(&iLen, sizeof(size_t), 1, _pFile);

	//문자열 저장. 
	fread(szBuffer, sizeof(wchar_t), iLen, _pFile);

	_str = szBuffer;
}

Vec2 lerp(const Vec2& a, const Vec2& b, float t)
{
	Vec2 result;
	result.x = a.x + t * (b.x - a.x);
	result.y = a.y + t * (b.y - a.y);

	return result;
}

//0~255값을 -> 0~1로 정규화
D2D1::ColorF ColorNormalize(int r, int g, int b) 
{ 
	return D2D1::ColorF(r / 255.f, g / 255.f, b / 255.f);
}

void ChangeScene(DWORD_PTR lParam, DWORD_PTR wParam) {
	//lParam : 변경할신
	//wParam : not use

	SCENE_TYPE Scene_Type = (SCENE_TYPE)(lParam);

	ChangeScene(Scene_Type);
}

// 검정색이 아닌 픽셀인지 확인하는 함수
bool IsNonBlackPixel(Color _color) 
{
	// 검정색에 가까운 픽셀을 제외 (임계값 설정)
	int brightnessThreshold = 50; // 밝기 임계값 (0~255)
	int brightness = (_color.GetR() + _color.GetG() + _color.GetB()) / 3;
	return brightness > brightnessThreshold;
}

// CLSID 가져오기 함수
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid) 
{
	UINT num = 0;          // Image encoders 수량
	UINT size = 0;         // Image encoders 크기

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;         // 실패

	ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;         // 메모리 부족

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j) {
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;      // 성공
		}
	}

	free(pImageCodecInfo);
	return -1;             // 실패
}

// GDI+ 초기화
void InitializeGDIPlus(ULONG_PTR& token) {
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&token, &gdiplusStartupInput, NULL);
}

// GDI+ 종료
void ShutdownGDIPlus(ULONG_PTR token) {
	GdiplusShutdown(token);
}

void MakeMapTile(const WCHAR* _tileOutlinePath, const WCHAR* _tilesPath, const WCHAR* _outputDir
	, int _blankTileWeight, int _otherTileWeight)
{
	// GDI+ 초기화
	ULONG_PTR gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	const WCHAR* header = CPathMgr::GetInstance()->GetContentPath();

	// 이미지 파일 경로
	wstring tileOutlinePath = (wstring)header + _tileOutlinePath;
	wstring tilesPath = (wstring)header + _tilesPath;

	Bitmap tileOutline(tileOutlinePath.c_str());
	if (tileOutline.GetLastStatus() != Ok) {
		std::cerr << "Failed to load tile_outline.png" << std::endl;
		GdiplusShutdown(gdiplusToken);
		return;
	}

	Bitmap tiles(tilesPath.c_str());
	if (tiles.GetLastStatus() != Ok) {
		std::cerr << "Failed to load tiles_1.png" << std::endl;
		GdiplusShutdown(gdiplusToken);
		return;
	}

	// 타일 크기 정의
	const int tileSize = TILE_SIZE / 2;

	// tileOutline과 tiles의 크기 가져오기
	int outlineWidth = tileOutline.GetWidth();
	int outlineHeight = tileOutline.GetHeight();
	int tilesWidth = tiles.GetWidth();
	int tilesHeight = tiles.GetHeight();

	// 각 이미지의 조각 개수 계산
	int outlineCols = outlineWidth / tileSize;
	int outlineRows = outlineHeight / tileSize;
	int tilesCols = tilesWidth / tileSize;
	int tilesRows = tilesHeight / tileSize;

	// 결과 이미지 저장 경로 기본값
	std::wstring outputDir = (wstring)header + _outputDir;

	// CLSID 가져오기
	CLSID pngClsid;
	GetEncoderClsid(L"image/png", &pngClsid);

	// 가중치 설정 (6번째 조각의 확률을 높임)
	std::vector<int> weights(tilesCols * tilesRows, _otherTileWeight); // 기본 가중치: 1
	weights[11] = _blankTileWeight; // 예: 6번째 조각의 가중치를 10으로 설정

	// 가중치를 기반으로 한 분포 생성
	std::discrete_distribution<int> randomTile(weights.begin(), weights.end());

	// 조합된 이미지 생성 및 저장
	int resultIndex = 0;

	for (int oy = 0; oy < outlineRows; ++oy) {
		for (int ox = 0; ox < outlineCols; ++ox) {
			// 새로운 빈 비트맵 생성
			Bitmap result(tileSize, tileSize);

			Graphics graphics(&result);

			bool hasNonBlackPixel = false;

			// 랜덤 타일 선택 (가중치 기반)
			int randomTileIndex = randomTile(rng);
			int randomTx = randomTileIndex % tilesCols; // 열 인덱스 계산
			int randomTy = randomTileIndex / tilesCols; // 행 인덱스 계산

			for (int y = 0; y < tileSize; ++y) {
				for (int x = 0; x < tileSize; ++x) {
					Color pixelColor;
					tileOutline.GetPixel(ox * tileSize + x, oy * tileSize + y, &pixelColor);

					if (IsNonBlackPixel(pixelColor)) {
						hasNonBlackPixel = true;

						Color tilePixelColor;
						tiles.GetPixel(randomTx * tileSize + x % tileSize, randomTy * tileSize + y % tileSize, &tilePixelColor);

						result.SetPixel(x, y, tilePixelColor);
					}
					else {
						result.SetPixel(x, y, pixelColor); // 검은색은 그대로 유지
					}
				}
			}

			if (hasNonBlackPixel) {
				// 결과 파일 저장 경로 설정
				std::wstring resultPath = outputDir + L"mapTile_" + std::to_wstring(resultIndex++) + L".png";

				// 결과 이미지 저장
				result.Save(resultPath.c_str(), &pngClsid, NULL);
			}
		}
	}
	// GDI+ 종료
	GdiplusShutdown(gdiplusToken);
}