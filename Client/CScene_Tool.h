#pragma once
#include "CScene.h"

class CUI;
class CScene_Tool :
    public CScene
{
private:
    CUI* m_pUI;

public:
    virtual void update();
    virtual void Enter();
    virtual void Exit();

public:
    //마우스를 클릭해서 타일의 Index를 변경한다(이미지)
    void SetTileIdx();


    void SaveTileData();
    void SaveTile(const wstring& _strFilePath);


    void LoadTileData();

    
    

public:
    CScene_Tool();
    ~CScene_Tool();
};

