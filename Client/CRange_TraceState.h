#pragma once
#include "CState.h"
class CRange_TraceState :
    public CState
{
private:


public:
    virtual void update();
    virtual void Enter();
    virtual void Exit();

public:
    CRange_TraceState();
    ~CRange_TraceState();
};

