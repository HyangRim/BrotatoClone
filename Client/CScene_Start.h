#pragma once
#include "CScene.h"
class CScene_Start :
    public CScene
{

public:
    virtual void update();
    //부모로부터 상속받은 가상함수인지, 일반 함수인지 보통은 구별 안되서 virtual씀. 
    //virtual을 안적어도 가상함수이긴 함... 그래도 명시적으로. 
    virtual void Enter();
    virtual void Exit();

public:
    CScene_Start();
    ~CScene_Start();
};

