#pragma once


class CObject;
class AI;

void CreateObject(CObject* _pObj, GROUP_TYPE _eGroup);
void DeleteObject(CObject* _pObj);

//씬을 바꿀 때, 기존 씬에 있던 포인터나 그런 거. 
//한 프레임은 작업을 마무리하고, Scene을 업데이트 해야만 함. 
//여기서 해줄 건 이벤트 등록. 다음 프레임부터 다음 씬으로 가도록 이벤트를 넣어준다. 
void ChangeScene(SCENE_TYPE _eNext);
void ChangeAIState(AI* _pAI, MON_STATE _eNextState);

D2D1::ColorF ColorNormalize(int r, int g, int b);

bool IsNonBlackPixel(Color _color);
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
void InitializeGDIPlus(ULONG_PTR& token);
void ShutdownGDIPlus(ULONG_PTR token);
void MakeMapTile(const WCHAR* _tileOutlinePath, const WCHAR* _tilesPath, const WCHAR* _outputDir
	, int _blankTileWeight, int _otherTileWeight);


void ShowCharacterInfo(DWORD_PTR lParam, DWORD_PTR wParam);
void ShowWeaponInfo(DWORD_PTR lParam, DWORD_PTR wParam);
void SelectCharacter(DWORD_PTR lParam, DWORD_PTR wParam);
void SelectWeapon(DWORD_PTR lParam, DWORD_PTR wParam);
void ChangeScene(DWORD_PTR, DWORD_PTR);

void InitializeGDIPlus(ULONG_PTR& token);
void ShutdownGDIPlus(ULONG_PTR token);

template<typename T>
void Safe_Delete_Vec(vector<T>& _vec);


//템플릿은 헤더에 구현해야만 함. 
template<typename T>
inline void Safe_Delete_Vec(vector<T>& _vec)
{
	for (auto& Obj : _vec) {
		if (Obj != nullptr) {
			delete Obj;
			Obj = nullptr;
		}
	}
	_vec.clear();
}


template<typename T1, typename T2>
void Safe_Delete_Map(map<T1, T2>& _map) {
	typename map<T1, T2>::iterator iter = _map.begin();

	for (; iter != _map.end(); ++iter) {
		if (iter->second != nullptr) {
			delete iter->second;
			iter->second = nullptr;
		}
	}
	_map.clear();
}

template<typename T1, typename T2>
void Safe_Delete_UnordedMap(unordered_map<T1, T2>& _map) {
	typename unordered_map<T1, T2>::iterator iter = _map.begin();

	for (; iter != _map.end(); ++iter) {
		if (iter->second != nullptr) {
			delete iter->second;
			iter->second = nullptr;
		}
	}
	_map.clear();
}

void Fscanf(char* _pOutBuff, FILE* _pFile);

void SaveWstring(const wstring& _str, FILE* _pFile);
void LoadWstring(wstring& _str, FILE* _pFile);

float flerp(const float a, const float b, float t);
Vec2 lerp(const Vec2& a, const Vec2& b, float t);