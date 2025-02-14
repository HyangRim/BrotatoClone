#pragma once


class CObject;

void CreateObject(CObject* _pObj, GROUP_TYPE _eGroup);
void DeleteObject(CObject* _pObj);

//씬을 바꿀 때, 기존 씬에 있던 포인터나 그런 거. 
//한 프레임은 작업을 마무리하고, Scene을 업데이트 해야만 함. 
//여기서 해줄 건 이벤트 등록. 다음 프레임부터 다음 씬으로 가도록 이벤트를 넣어준다. 
void ChangeScene(SCENE_TYPE _eNext);



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