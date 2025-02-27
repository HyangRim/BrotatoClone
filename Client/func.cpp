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
	//Animation�� �̸��� �����Ѵ�.(������ ����ȭ)
	const wchar_t* pStrName = _str.c_str();
	size_t iLen = _str.length();

	//���� ���� ����. 
	fwrite(&iLen, sizeof(size_t), 1, _pFile);

	//���ڿ� ����. 
	fwrite(pStrName, sizeof(wchar_t), iLen, _pFile);
}

void LoadWstring(wstring& _str, FILE* _pFile)
{
	size_t iLen = 0;
	wchar_t szBuffer[256] = {};
	//���� ���� ����. 
	fread(&iLen, sizeof(size_t), 1, _pFile);

	//���ڿ� ����. 
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

//0~255���� -> 0~1�� ����ȭ
D2D1::ColorF ColorNormalize(int r, int g, int b) 
{ 
	return D2D1::ColorF(r / 255.f, g / 255.f, b / 255.f);
}

void ChangeScene(DWORD_PTR lParam, DWORD_PTR wParam) {
	//lParam : �����ҽ�
	//wParam : not use

	SCENE_TYPE Scene_Type = (SCENE_TYPE)(lParam);

	ChangeScene(Scene_Type);
}

// �������� �ƴ� �ȼ����� Ȯ���ϴ� �Լ�
bool IsNonBlackPixel(Color _color) 
{
	// �������� ����� �ȼ��� ���� (�Ӱ谪 ����)
	int brightnessThreshold = 50; // ��� �Ӱ谪 (0~255)
	int brightness = (_color.GetR() + _color.GetG() + _color.GetB()) / 3;
	return brightness > brightnessThreshold;
}

// CLSID �������� �Լ�
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid) 
{
	UINT num = 0;          // Image encoders ����
	UINT size = 0;         // Image encoders ũ��

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;         // ����

	ImageCodecInfo* pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;         // �޸� ����

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j) {
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;      // ����
		}
	}

	free(pImageCodecInfo);
	return -1;             // ����
}

// GDI+ �ʱ�ȭ
void InitializeGDIPlus(ULONG_PTR& token) {
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&token, &gdiplusStartupInput, NULL);
}

// GDI+ ����
void ShutdownGDIPlus(ULONG_PTR token) {
	GdiplusShutdown(token);
}

void MakeMapTile(const WCHAR* _tileOutlinePath, const WCHAR* _tilesPath, const WCHAR* _outputDir
	, int _blankTileWeight, int _otherTileWeight)
{
	// GDI+ �ʱ�ȭ
	ULONG_PTR gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	const WCHAR* header = CPathMgr::GetInstance()->GetContentPath();

	// �̹��� ���� ���
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

	// Ÿ�� ũ�� ����
	const int tileSize = TILE_SIZE / 2;

	// tileOutline�� tiles�� ũ�� ��������
	int outlineWidth = tileOutline.GetWidth();
	int outlineHeight = tileOutline.GetHeight();
	int tilesWidth = tiles.GetWidth();
	int tilesHeight = tiles.GetHeight();

	// �� �̹����� ���� ���� ���
	int outlineCols = outlineWidth / tileSize;
	int outlineRows = outlineHeight / tileSize;
	int tilesCols = tilesWidth / tileSize;
	int tilesRows = tilesHeight / tileSize;

	// ��� �̹��� ���� ��� �⺻��
	std::wstring outputDir = (wstring)header + _outputDir;

	// CLSID ��������
	CLSID pngClsid;
	GetEncoderClsid(L"image/png", &pngClsid);

	// ����ġ ���� (6��° ������ Ȯ���� ����)
	std::vector<int> weights(tilesCols * tilesRows, _otherTileWeight); // �⺻ ����ġ: 1
	weights[11] = _blankTileWeight; // ��: 6��° ������ ����ġ�� 10���� ����

	// ����ġ�� ������� �� ���� ����
	std::discrete_distribution<int> randomTile(weights.begin(), weights.end());

	// ���յ� �̹��� ���� �� ����
	int resultIndex = 0;

	for (int oy = 0; oy < outlineRows; ++oy) {
		for (int ox = 0; ox < outlineCols; ++ox) {
			// ���ο� �� ��Ʈ�� ����
			Bitmap result(tileSize, tileSize);

			Graphics graphics(&result);

			bool hasNonBlackPixel = false;

			// ���� Ÿ�� ���� (����ġ ���)
			int randomTileIndex = randomTile(rng);
			int randomTx = randomTileIndex % tilesCols; // �� �ε��� ���
			int randomTy = randomTileIndex / tilesCols; // �� �ε��� ���

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
						result.SetPixel(x, y, pixelColor); // �������� �״�� ����
					}
				}
			}

			if (hasNonBlackPixel) {
				// ��� ���� ���� ��� ����
				std::wstring resultPath = outputDir + L"mapTile_" + std::to_wstring(resultIndex++) + L".png";

				// ��� �̹��� ����
				result.Save(resultPath.c_str(), &pngClsid, NULL);
			}
		}
	}
	// GDI+ ����
	GdiplusShutdown(gdiplusToken);
}