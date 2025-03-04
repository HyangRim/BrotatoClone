#pragma once


//미리 컴파일된 헤더. 
//이미 제공된 애들 <- 바뀔 여지가 없으니까, 다시 컴파일 할 이유X
//따라서 이걸 사용하면, 컴파일 속도가 빨라짐. 

//그리고 클래스를 만들때 선언해둔 미리 컴파일된 헤더 생김. 

#include <Windows.h>
#include <ctime>
#include <iostream>
#include <d2d1.h>
#include <d2d1_1.h>
#include <dwrite.h>
#pragma comment(lib, "Dwrite.lib")
#include <wincodec.h>
#include <stdio.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "windowscodecs.lib")

#include <vector>
using std::vector;

#include <list>
using std::list;

#include <set>
#include <unordered_set>
using std::set;
using std::unordered_set;

#include <map>
using std::map;
using std::make_pair;

#include <unordered_map>
using std::unordered_map;

#include <string>
using std::string;
using std::wstring;

#include <random>
std::random_device random_generator;
std::uniform_int_distribution<int> random_distribution(0, 100);

#include <math.h>
#include <assert.h>

#pragma comment(lib, "Msimg32.lib")

#include <codecvt>
#include <locale>

#include "fmod.hpp"
#include "fmod_errors.h"
#pragma comment(lib, "fmod_vc.lib")

#include <gdiplus.h>
#include <gdiplusheaders.h>
#include <gdiplusflat.h>
#pragma comment(lib, "gdiplus")
using namespace Gdiplus;

#include <utility>


#include "define.h"
#include "struct.h"
#include "func.h"
#include "item_string.h"


std::random_device rd;
std::mt19937 rng(rd());
std::uniform_real_distribution<float> float_distribution(-0.03f, 0.03f);
std::uniform_int_distribution<int> distribution(0, 100);

const wstring dropItemKey = L"drop_item";