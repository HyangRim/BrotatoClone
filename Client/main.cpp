// Client.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//


#include "pch.h"
#include "framework.h"
#include "Client.h"
#include "CCore.h"

#include <crtdbg.h>

#define MAX_LOADSTRING 100
//WCHAR은 2바이트, wchar_t형임. (16비트)
//그거 이외엔 다른 게 없음(유니코드형을 위한 것)
// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
//메인함수가 처음 시작할 때 받아오는 값. 

HWND g_hWnd; //메인 윈도우 핸들. 
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
//100길이로 그냥 선언해놓은 것. 

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
//함수들은 전방 선언해놓은 것.(함수 프로토타입)


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    //메모리 릭(누수) 체크
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    //문제가 생겼을 때, 중단점 걸어주는 기능. 
    //_crtBreakAlloc();


    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    //딱히 신경쓸 건 없음. 얘내 둘은 딱히 쓰이지 않는다... 라는 걸 선언한 느낌. 
    //매크로임. 
// TODO: 여기에 코드를 입력합니다.


    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);


    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    // 여기서 윈도우 창을 생성함. 유저와 프로그램 사이의 인터페이스, 접점이라는 의미도 됨.
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    //CCore 초기화
    //이게 메인 윈도우 입니다. 이게 해상도입니다. 
    if (FAILED(CCore::GetInstance()->init(g_hWnd, POINT{1280, 768}))) {
        MessageBox(nullptr, L"Core 객체 초기화 실패", L"ERROR", MB_OK);
        return FALSE;
    }

    //단축키 정보. 
    //리소스 뷰 -> Accelerator -> table 
    //특정 키와 조합했을 때, 키값. 
    //테이블 정보 불러와서. 
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

    MSG msg;


    while (true)
    {
        //무한 반복 -> 메시지가 없으면 if문 바깥으로 무시. 
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            //메시지를 처리하기 전에, 

            if (WM_QUIT == msg.message) {
                break;
            }


            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        //메시지가 발생하지 않는 대부분의 시간. 
        else {
            //디자인 패턴(설계 유형)
            //싱글톤 패턴


            //게임 속에서 렌더링이라는 건, 매 순간순간 전부 다 삭제 후, 전부 그리기. 
            CCore::GetInstance()->progress();
        }
    }
    //두 번재 매개변수가 ID.

    return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//  창 클래스를 우리가 정의하고, 등록하는 절차는 윈도우 OS에서 제공하는 함수. 
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    //구조체 채워서 함수에 전달하기. 
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    //WndProc이란?  전방 선언 되어 있는 위 함수. 
    //함수 포인터. 
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    //처음 생성되는 메뉴바 null 값을 넣으면 메뉴바 사라짐. 
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CLIENT);

    //이거의 Key값을 szWindowClass로 사용함. 이 MyRegisterClass의 Key값임. 
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    //이건 윈도우 OS에서 제공하는 함수임... 
    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
    //szTitle 대신 "한재훈~" 넣어도 됨. 2번째 변수는 이름 말하는 거임. 
    //szWindowClass 키 값대로 만들어달라. 
    g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!g_hWnd)
    {
        return FALSE;
    }

    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

    return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
// 메시지가 발생한 윈도우 id값, 메시지등등이 같이 들어옴.
//이런 함수가 있다. 전방 선언. 
INT_PTR __stdcall TileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND://왠만한 메시지(단축키)는 이렇게 여기에서 처리함. 
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다:
        switch (wmId)
        {
        case IDM_ABOUT:
            //여기가 도움말. 모달 방식(도움말에서 전부 주목(입력)을 다 가져감. -> 모달에서 리턴이 안되기 때문. 
            //Visual Studio에도 리소스라는 것이 있음. About은 함수 포인터. 
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;

        case ID_MENU_TILE:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_TILE_COUNT), hWnd, TileCountProc);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {

        PAINTSTRUCT ps;
        //Begin에서 End까지 마무리를 할 때, 우리 윈도우에는 무효화 영역이 없다. 
        //End안하면, 계속해서 무한히 무효화 영역이라고 생각함. 

        //메시지가 있던 없던 그림을 그릴거다. 그럴때 사용하는게 GetDC
        HDC hdc = BeginPaint(hWnd, &ps); //Devicde Context.(그리기)



        //Rectangle(hdc, 1180, 668, 1280, 768);

        //그리기 종료. 
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_LBUTTONDOWN:
        break;
    case WM_MOUSEMOVE:
        break;

    case WM_LBUTTONUP:
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
    {
    }
    break;
    default: //따로 정의되지 않은 수많은 단축키들. 
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)// ok나 X창 누르면
        {
            EndDialog(hDlg, LOWORD(wParam));    //Dialog 끝내기. 
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
