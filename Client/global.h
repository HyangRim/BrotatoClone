#pragma once


//미리 컴파일된 헤더. 
//이미 제공된 애들 <- 바뀔 여지가 없으니까, 다시 컴파일 할 이유X
//따라서 이걸 사용하면, 컴파일 속도가 빨라짐. 

//그리고 클래스를 만들때 선언해둔 미리 컴파일된 헤더 생김. 

#include <Windows.h>
#include <iostream>

#include <vector>
using std::vector;

#include <list>
using std::list;

#include <map>
using std::map;
using std::make_pair;

#include <unordered_map>
using std::unordered_map;

#include <string>
using std::string;
using std::wstring;

#include <math.h>
#include <assert.h>

#pragma comment(lib, "Msimg32.lib")

#include "define.h"
#include "struct.h"

#include "func.h"