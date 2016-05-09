#include <windows.h>
#include "Lecture.h"
#include <fstream>
using namespace std;
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCWSTR IpszClass = L"2013년 1학기 한국기술교육대 시간표 도우미 v1.0 (예비수강신청용)";
#define VERSION_YEAR 131
HBITMAP hBit;

void WM_Create(HWND hWnd);
void WM_Paint(HDC hdc, const int TT_X_START, const int TT_Y_START, int mode);
void Show_Lecture(HDC hdc, LECTURE* lc, const int TT_X_START, const int TT_Y_START, int mode);
void WM_Command(HWND hWnd, WPARAM wParam);

#define PAINT_WINDOW_MODE	1
#define PAINT_PRINT_MODE	2

HWND MainhWnd;

const wchar_t * DAT_FILE_NAME = L"tt_data_131_0.dat";
LECTURE *Lecture;
SameList *List;
MyLecture *SelectedList;
LECTURE *Selected;

#define SL_MODE_UNDEF	0
int Selected_Mode = SL_MODE_UNDEF;
#define SL_MODE_NEW		1
#define SL_MODE_DEL		2

#define ID_COMBO_MAJOR	100
HWND hCombo_Major;

#define ID_LIST_CT		101
HWND hList_CT;

#define ID_LIST_LECTURE	102
HWND hList_Lecture;

#define ID_LIST_DIVCLS	103
HWND hList_DivCls;

#define ID_BTN_COMMAND	104
HWND hButton_CMD;

#define ID_LIST_SELECTED	105
HWND hList_Selected;

#define ID_BTN_DELCMD	106
HWND hButton_DelCMD;

#define ID_BTN_LOAD		107
HWND hButton_Load;

#define ID_BTN_SAVE		108
HWND hButton_Save;

#define ID_BTN_INIT		109
HWND hButton_Init;

#define ID_EDIT_SEARCH	110
HWND hEdit_Search;

#define ID_BTN_SEARCH	111
HWND hButton_Search;

#define ID_BTN_PRINT	112
HWND hButton_Print;

#define ID_EDIT_NAME_SEARCH	113
HWND hEdit_Name_Search;

#define ID_BTN_NAME_SEARCH	114
HWND hButton_Name_Search;

#define ID_EDIT_PROF_SEARCH	115
HWND hEdit_Prof_Search;

#define ID_BTN_PROF_SEARCH	116
HWND hButton_Prof_Search;


int Selected_Major	= 0;
int Selected_CT		= 0;
int Selected_Lecture = -1;
int Selected_DivCls = 0;

#define NORMAL_MODE			0
#define PROF_SEARCH_MODE	1
int Searching_Prof	= NORMAL_MODE;
wchar_t Searching_Prof_str[10];


#define TT_X_START_W	625	// 윈도우용 x시작좌표
#define TT_Y_START_W	10	// 윈도우용 y시작좌표
int TT_X_START_P;
int TT_Y_START_P;
#define TT_X_CLM_W		63	// 윈도우용 x칸픽셀
#define TT_Y_CLM_W		20	// 윈도우용 y칸픽셀
int TT_X_CLM_P;
int TT_Y_CLM_P;

void Print();

int Score = 0;

WNDPROC OldEditProc;
WNDPROC OldEditProc2;
WNDPROC OldEditProc3;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
					 LPSTR IpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = IpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = 0;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(IpszClass, IpszClass, WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX
						, CW_USEDEFAULT, CW_USEDEFAULT, 1100, 620,
						NULL, (HMENU)NULL, hInstance, NULL);

	MainhWnd = hWnd;
	ShowWindow(hWnd, nCmdShow);

/////////////////////////////////////////////////////
// 시간표 정보 로딩

	Lecture = new LECTURE;
	List = NULL;
	Selected = NULL;
	SelectedList = new MyLecture(NULL);
	if(!Lecture->Open_Lecture(DAT_FILE_NAME, Lecture))
	{
		MessageBox(hWnd, L"시간표 데이터를 찾을 수 없거나 손상되어 종료합니다", L"시간표", MB_OK);
		return 0;
	}
/***************************************************/

	while(GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	delete Lecture;
	if(List != NULL)
		delete List;
	Selected = NULL;// delete Lecture로 지워지므로 지우지 않는다
	delete SelectedList;

	return Message.wParam;
}


LRESULT CALLBACK EditSubProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_KEYDOWN:
		if (wParam==VK_RETURN) 
		{
			SendMessage(MainhWnd, WM_COMMAND, ID_BTN_SEARCH, 0);
		}

		if (wParam==VK_TAB) 
		{
			SetFocus(hButton_Search);
		}
		break;

	case WM_ERASEBKGND:
		return false;
	}
	return CallWindowProc(OldEditProc,hWnd,iMessage,wParam,lParam);
}

LRESULT CALLBACK EditSubProc2(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_KEYDOWN:
		if (wParam==VK_RETURN) 
		{
			SendMessage(MainhWnd, WM_COMMAND, ID_BTN_NAME_SEARCH, 0);
		}

		if (wParam==VK_TAB) 
		{
			SetFocus(hButton_Name_Search);
		}
		break;

	case WM_ERASEBKGND:
		return false;
	}
	return CallWindowProc(OldEditProc2,hWnd,iMessage,wParam,lParam);
}

LRESULT CALLBACK EditSubProc3(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_KEYDOWN:
		if (wParam==VK_RETURN) 
		{
			SendMessage(MainhWnd, WM_COMMAND, ID_BTN_PROF_SEARCH, 0);
		}

		if (wParam==VK_TAB) 
		{
			SetFocus(hButton_Prof_Search);
		}
		break;

	case WM_ERASEBKGND:
		return false;
	}
	return CallWindowProc(OldEditProc2,hWnd,iMessage,wParam,lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch(iMessage)
	{
	case WM_CREATE:
		WM_Create(hWnd);

/*
		MessageBox(hWnd, L"2009년 1학기용 시간표 제작기v4.2입니다. 시간표를 제작한 뒤에\n변경과목이 있는지 등을 체크해 주시기 바랍니다!\n"
            L"학사종합서비스→수강정보→-강의시간표 작성해보기로도 확인해 보는걸 추천합니다!\n\n"
			L"v1에서 변경점: 저장/불러오기가 제대로 안되는 것이 수정되었습니다.\nv2에서 변경점: 인쇄기능이 추가되었습니다"
			L"\nv3에서 변경점: 과목으로 검색하기가 추가되었습니다, 과목만 눌러도 각 분반의 시간을 알 수 있습니다.\n"
			L"\t산경의 시간표 변경(학교 공지 7105,7111번 글)내용이 적용되었습니다."
			L"\nv4에서 변경점: 전통->정통 오타 수정, 건축의 시간표 변경(학교 공지7113번글) 적용."
			L"\n\t과목을 누를때 생기는 빨간 선이 인쇄상태에서도 보이는 버그 해결"
			L"\n\t빨간 선 두께 감소"
			L"\nv4.1에서 변경점: 검색에서 가끔 튕기는 문제 해결", L"시간표 제작 도우미", MB_OK);

		MessageBox(hWnd, L"2009년 2학기용 시간표 제작기 v1.0입니다. 시간표를 제작한 뒤에\n변경 과목이 있는지 등을 체크해 주시기 바랍니다!\n"
			, L"시간표 제작 도우미", MB_OK);

		MessageBox(hWnd, L"2010년 1학기용 시간표 제작기 v1.2입니다. 시간표를 제작한 뒤에\n변경 과목이 있는지 등을 체크해 주시기 바랍니다!\n"
			L"v1.1 에서의 변경점: \"영어강의\"인지 * 로 구별 가능하게 수정되었습니다."
			L"\n\t경영정보시스템 2분반 시간표의 잘못을 고쳤습니다."
			L"\nv1.2 에서의 변경점: eBiz 등의 일부 전공이 과목 선택이 불가능했던 점이 수정되었습니다."
			L"\n\t신소재공학과 졸업설계I이 월요일 9교시에서 목요일 9교시로 바뀌었습니다"
			, L"시간표 제작 도우미", MB_OK);

		MessageBox(hWnd, L"2010년 2학기용 시간표 제작기 v1.2입니다. 시간표를 제작한 뒤에\n변경 과목이 있는지 등을 체크해 주시기 바랍니다!\n"
			L"\n\tv1.1 에서의 변경점: 몇몇 과목의 시간이 비정상적으로 표기되는것이 수정되었습니다."
			L"\n\tv1.2 에서의 변경점: 특수하우징공법의 시간이 월11,금6~8B,9B~12A로 수정되었습니다."
			L"\n\t                    공지사항 8875번 글이 반영되었습니다."
			, L"시간표 제작 도우미" , MB_OK);

        MessageBox(hWnd, L"2011년 1학기용 시간표 제작기 v1.0입니다. 시간표를 제작한 뒤에\n변경 과목이 있는지 등을 체크해 주시기 바랍니다!\n"
            L"\n\tv1.1 에서의 변경점: 꽤 많은 과목의 엑셀을 기본으로 한 자료가 한칸씩 밀려서 전공정보, 대상학과/학년 정보, 담당 교수님 정보, 시간표 정보가 모두 어긋나던 문제가 수정되었습니다."
            , L"시간표 제작 도우미", MB_OK);

        MessageBox(hWnd, L"2011년 2학기용 시간표 제작기 v1.0입니다. 시간표를 제작한 뒤에\n변경 과목이 있는지 등을 체크해 주시기 바랍니다!\n"
            , L"시간표 제작 도우미", MB_OK);

        MessageBox(hWnd, L"2012년 1학기용 시간표 제작기 v1.0입니다. 시간표를 제작한 뒤에\n변경 과목이 있는지 등을 체크해 주시기 바랍니다!\n"
            , L"시간표 제작 도우미", MB_OK);

		MessageBox(hWnd, L"2012년 2학기용 시간표 제작기 v1.0  (예비수강신청용) 입니다.\n"
			L"시간표를 제작한 뒤에\n변경 과목이 있는지 등을 체크해 주시기 바랍니다!\n\n"
			L"※ 이 버젼은 예비수강신청용으로, 해당 공지사항의 엑셀파일을\n"
			L"   기초로 작성된 버젼이며,\n"
			L"   정식수강신청용 공지가 올라오면 업데이트될 예정입니다."
            , L"시간표 제작 도우미", MB_OK);
*/
        MessageBox(hWnd, L"2013년 1학기용 시간표 제작기 v1.0 (예비수강신청용) 입니다.\n"
			L"시간표를 제작한 뒤에\n변경 과목이 있는지 등을 체크해 주시기 바랍니다!\n\n"
			L"※ 이 버젼은 예비수강신청용으로, 해당 공지사항의 엑셀파일을\n"
			L"   기초로 작성된 버젼입니다."
            , L"시간표 제작 도우미", MB_OK);
		return 0;

	case WM_PAINT:
		{
			HDC hdc, hMemDC;
			HBITMAP OldBit;
			PAINTSTRUCT ps;
			RECT crt;

			hdc=BeginPaint(hWnd, &ps);
			GetClientRect(hWnd,&crt);
			hMemDC=CreateCompatibleDC(hdc);
			OldBit=(HBITMAP)SelectObject(hMemDC, hBit);
			BitBlt(hdc,170, 50, 300, 85, hMemDC, 170, 50, SRCCOPY);
			BitBlt(hdc,TT_X_START_W,0,crt.right,531,hMemDC,TT_X_START_W,0,SRCCOPY);
			SelectObject(hMemDC, OldBit);
			DeleteDC(hMemDC);
			EndPaint(hWnd, &ps);

		}
		return 0;

	case WM_COMMAND:
		WM_Command(hWnd, wParam);
		
		{
			HDC hdc, hMemDC;
			HBITMAP OldBit;
			RECT crt;

			hdc = GetDC(hWnd);
			GetClientRect(hWnd,&crt);

			if (hBit==NULL) {
				hBit=CreateCompatibleBitmap(hdc,crt.right,crt.bottom);
			}
			hMemDC=CreateCompatibleDC(hdc);
			OldBit=(HBITMAP)SelectObject(hMemDC,hBit);

			FillRect(hMemDC,&crt,GetSysColorBrush(COLOR_WINDOW));

			WM_Paint(hMemDC, TT_X_START_W, TT_Y_START_W, PAINT_WINDOW_MODE);

			SelectObject(hMemDC,OldBit);
			DeleteDC(hMemDC);
			ReleaseDC(hWnd,hdc);
		
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;

	case WM_CLOSE:
		{
			HRESULT rs = MessageBox(hWnd, L"저장하시겠습니까?", L"kut_tt_reader", MB_YESNOCANCEL);
			if(rs == IDYES)
				WM_Command(hWnd, ID_BTN_SAVE);
			if(rs == IDCANCEL)
				return 0;
		}

	case WM_DESTROY:

		SetWindowLong(hEdit_Search,GWL_WNDPROC,(LONG)OldEditProc);
		SetWindowLong(hEdit_Name_Search,GWL_WNDPROC,(LONG)OldEditProc2);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void WM_Create(HWND hWnd)
{
	int i;

	hCombo_Major=CreateWindow(L"combobox",NULL,WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL
		,10,10,200,200,hWnd,(HMENU)ID_COMBO_MAJOR,g_hInst,NULL);

	for (i = 0; i < MAJOR_COUNT; i++)
		SendMessage(hCombo_Major, CB_ADDSTRING, 0, (LPARAM)MajorName[i]);

	hList_CT = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | LBS_NOTIFY
		,10, 50, 150, 200, hWnd, (HMENU)ID_LIST_CT, g_hInst, NULL);

	SendMessage(hList_CT, LB_ADDSTRING, 0, (LPARAM)L"전공을 먼저");
	SendMessage(hList_CT, LB_ADDSTRING, 0, (LPARAM)L"선택해 주세요");
		
	hList_Lecture = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | LBS_NOTIFY
		,10, 250, 270, 200, hWnd, (HMENU)ID_LIST_LECTURE, g_hInst, NULL);

	SendMessage(hList_Lecture, LB_ADDSTRING, 0, (LPARAM)L"이수구분을 먼저 선택해 주세요");

	hList_DivCls = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | LBS_NOTIFY
		,280, 250, 330, 200, hWnd, (HMENU)ID_LIST_DIVCLS, g_hInst, NULL);

	hButton_CMD = CreateWindow(L"button", L"추가", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		280, 230, 70, 19, hWnd, (HMENU)ID_BTN_COMMAND, g_hInst, NULL);
		
	hList_Selected = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | LBS_NOTIFY
		,10, 475, 360, 95, hWnd, (HMENU)ID_LIST_SELECTED, g_hInst, NULL);

	hButton_DelCMD = CreateWindow(L"button", L"삭제", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		280, 455, 70, 19, hWnd, (HMENU)ID_BTN_DELCMD, g_hInst, NULL);

	hButton_Load = CreateWindow(L"button", L"시간표 불러오기", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
		,630, 540, 150, 40, hWnd, (HMENU)ID_BTN_LOAD, g_hInst, NULL);

	hButton_Save = CreateWindow(L"button", L"시간표 저장하기", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
		,800, 540, 150, 40, hWnd, (HMENU)ID_BTN_SAVE, g_hInst, NULL);

	hButton_Print = CreateWindow(L"button", L"프린트하기", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
		,970, 540, 100, 40, hWnd, (HMENU)ID_BTN_PRINT, g_hInst, NULL);

	hButton_Init = CreateWindow(L"button", L"시간표 초기화", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
		, 210, 10, 130, 22, hWnd, (HMENU)ID_BTN_INIT, g_hInst, NULL);

	hEdit_Search = CreateWindow(L"edit", L"코드로 찾기", WS_CHILD | WS_VISIBLE | WS_BORDER
		, 200, 100, 100, 22, hWnd, (HMENU)ID_EDIT_SEARCH, g_hInst, NULL);

	hEdit_Name_Search = CreateWindow(L"edit", L"과목이름으로 찾기", WS_CHILD | WS_VISIBLE | WS_BORDER
		, 200, 130, 200, 22, hWnd, (HMENU)ID_EDIT_NAME_SEARCH, g_hInst, NULL);

	hEdit_Prof_Search = CreateWindow(L"edit", L"교수님 찾기", WS_CHILD | WS_VISIBLE | WS_BORDER
		, 200, 180, 100, 22, hWnd, (HMENU)ID_EDIT_PROF_SEARCH, g_hInst, NULL);

	hButton_Search = CreateWindow(L"button", L"찾기", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
		, 300, 100, 50, 22, hWnd, (HMENU)ID_BTN_SEARCH, g_hInst, NULL);

	hButton_Name_Search = CreateWindow(L"button", L"찾기", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
		, 300, 152, 50, 22, hWnd, (HMENU)ID_BTN_NAME_SEARCH, g_hInst, NULL);

	hButton_Prof_Search = CreateWindow(L"button", L"찾기", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
		, 300, 180, 50, 22, hWnd, (HMENU)ID_BTN_PROF_SEARCH, g_hInst, NULL);

	OldEditProc=(WNDPROC)SetWindowLong(hEdit_Search,GWL_WNDPROC,(LONG)EditSubProc);
	OldEditProc2=(WNDPROC)SetWindowLong(hEdit_Name_Search,GWL_WNDPROC,(LONG)EditSubProc2);
	OldEditProc3=(WNDPROC)SetWindowLong(hEdit_Prof_Search,GWL_WNDPROC,(LONG)EditSubProc3);
}

void WM_Paint(HDC hdc, const int TT_X_START, const int TT_Y_START, int mode)
{
	HPEN pen, def_pen;
	HFONT font, def_font;
	HBRUSH brush, def_brush;
	RECT rc;
	int i, j;
	wchar_t* temp, *temp2;
	MyLecture* Iterator;
	int TT_X_CLM, TT_Y_CLM;

	if(mode == PAINT_WINDOW_MODE)
	{
		TT_X_CLM = TT_X_CLM_W;
		TT_Y_CLM = TT_Y_CLM_W;
	}
	else if(mode == PAINT_PRINT_MODE)
	{
		TT_X_CLM = TT_X_CLM_P;
		TT_Y_CLM = TT_Y_CLM_P;
	}
	else
	{
		MessageBox(MainhWnd, L"Invalid Paint Mode!", L"Error", MB_OK);
		return;
	}

	pen = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
	def_pen = (HPEN)SelectObject(hdc, pen);

	//////////////////////////////////////////////
	// 사각형 그리기
	for(i = 0; i < DAY_COUNT+1; i++)
	{
		for(j = 0; j <= TI_COUNT; j++)
		{
			Rectangle(hdc, TT_X_START+(i*TT_X_CLM), TT_Y_START+(j*TT_Y_CLM)
						, TT_X_START+((i+1)*TT_X_CLM)+1, TT_Y_START+((j+1)*TT_Y_CLM)+1);
		}
	}

	SelectObject(hdc, def_pen);
	DeleteObject(pen);

	if(mode == PAINT_WINDOW_MODE)
		font = CreateFont(13, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, 0);
	else
		font = CreateFont(TT_Y_CLM_P*4/5, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, 0);
	def_font = (HFONT)SelectObject(hdc, font);
	//////////////////////////////////////////////
	// 요일 그리기

	for(i = 0; i < DAY_COUNT; i++)
	{
		rc.left = TT_X_START+((i+1)*TT_X_CLM)+1;
		rc.right = TT_X_START+((i+2)*TT_X_CLM);
		rc.top = TT_Y_START+4;
		rc.bottom = TT_Y_START+TT_Y_CLM;
		DrawText(hdc, day_inf[i], 1, &rc, DT_CENTER | DT_VCENTER);
	}

	//////////////////////////////////////////////
	// 1A~12B, 13~ 그리기
	int tempSize = 4;
	temp = new wchar_t[tempSize];
	for(i = 1; i <= 12; i++)
	{
		rc.left = TT_X_START+20;
		rc.right = TT_X_START+TT_X_CLM-1;
		rc.top = TT_Y_START+(i*2-1)*TT_Y_CLM+4;
		rc.bottom = TT_Y_START+(i*2)*TT_Y_CLM-1;
		temp[0] = i / 10 + L'0';
		temp[1] = i % 10 + L'0';
		temp[2] = L'A';
		temp[3] = L'\0';
		DrawText(hdc, temp, 3, &rc, DT_LEFT);
		rc.top += TT_Y_CLM;
		rc.bottom += TT_Y_CLM;
		temp[2] = L'B';
		DrawText(hdc, temp, 3, &rc, DT_LEFT);
	}

	rc.top += TT_Y_CLM;
	rc.bottom += TT_Y_CLM;
	wcscpy_s(temp, tempSize, L"13~");
	DrawText(hdc, temp, 3, &rc, DT_LEFT);
	delete[] temp;

	///////////////////////////////////////////
	// 선택학점 표시기
	tempSize = 12;
	temp = new wchar_t[tempSize];
	temp[0] = L'\0';
	if(mode == PAINT_WINDOW_MODE)
		wcscpy_s(temp, tempSize, L"전체 학점 : ");
	else
		wcscpy_s(temp, tempSize, L"학점 : ");
	temp2 = new wchar_t[3];
	temp2[0] = Score/10 + L'0';
	temp2[1] = Score%10 + L'0';
	temp2[2] = L'\0';
	wcscat_s(temp, tempSize, temp2);

	if(mode == PAINT_WINDOW_MODE)
	{
		rc.left = 170;
		rc.right = 300;
		rc.top = 50;
		rc.bottom = 70;
	}
	else
	{
		rc.left = TT_X_CLM*5;
		rc.right = TT_X_CLM*7;
		rc.top = TT_Y_START-TT_Y_CLM;
		rc.bottom = TT_Y_START;
	}

    if(Score > 22)
        SetTextColor(hdc, RGB(255, 0, 0));
    else if(Score == 22)
        SetTextColor(hdc, RGB(196, 196, 0));

	DrawText(hdc, temp, wcslen(temp), &rc, DT_RIGHT);

	SetTextColor(hdc, RGB(0, 0, 0));

	if(mode == PAINT_WINDOW_MODE)
	{
		rc.top += 15;
		rc.bottom += 15;

		if(Selected != NULL && Selected_Mode != SL_MODE_UNDEF)
		{
			wcscpy_s(temp, tempSize, L"선택한 학점 : ");
			temp2[0] = Selected->pou_srt/10 + L'0';
			temp2[1] = Selected->pou_srt%10 + L'0';
			temp2[2] = L'\0';
			wcscat_s(temp, tempSize, temp2);

			DrawText(hdc, temp, 11, &rc, DT_RIGHT);
		}
	}

	SelectObject(hdc, def_font);
	DeleteObject(font);

	delete[] temp;
	delete[] temp2;

	/////////////////////////////////////////////
	// 과목 그리기(상자색칠/글자표기)
	Iterator = SelectedList;
	if(Iterator != NULL)
		Iterator = Iterator->GetNext();
	if(mode == PAINT_WINDOW_MODE)
		font = CreateFont(11, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, 0);
	else
		font = CreateFont(TT_Y_CLM_P*2/5, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, 0);
	def_font = (HFONT)SelectObject(hdc, font);
	brush = CreateSolidBrush(RGB(153, 204, 255));
	def_brush = (HBRUSH)SelectObject(hdc, brush);

	while(Iterator != NULL)
	{
		LECTURE* lc = Iterator->GetLecture();
		Show_Lecture(hdc, lc, TT_X_START, TT_Y_START, mode);

		Iterator = Iterator->GetNext();
	}

	SelectObject(hdc, def_brush);
	DeleteObject(brush);

	if(mode == PAINT_WINDOW_MODE && Selected != NULL && Selected_Mode != SL_MODE_UNDEF)
	{
		if(Selected_Mode == SL_MODE_NEW)
			brush = CreateSolidBrush(RGB(255, 255, 0));
		else if(Selected_Mode == SL_MODE_DEL)
			brush = CreateSolidBrush(RGB(255, 127, 127));
		def_brush = (HBRUSH)SelectObject(hdc, brush);

		Show_Lecture(hdc, Selected, TT_X_START, TT_Y_START, mode);

		SelectObject(hdc, def_brush);
		DeleteObject(brush);
	}

	/////////////////////////////////////////////
	// 빨간 상자 그리기
	if(mode == PAINT_WINDOW_MODE && List != NULL && ((Searching_Prof == PROF_SEARCH_MODE) || (Selected_Mode != SL_MODE_DEL && Selected_Lecture != -1)))
	{
		wchar_t mycode[7];
		if(Searching_Prof != PROF_SEARCH_MODE)
			List->GetCode(Selected_Lecture, mycode);

		if(Searching_Prof == PROF_SEARCH_MODE || (Searching_Prof != PROF_SEARCH_MODE && mycode[0] != L'\0'))
		{
			pen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			def_pen = (HPEN)SelectObject(hdc, pen);
			brush = (HBRUSH)GetStockObject(NULL_BRUSH);
			def_brush = (HBRUSH)SelectObject(hdc, brush);

			LECTURE* link = Lecture;

			class LCNode
			{
			public:
				wchar_t DivCls[20];
				int day;
				int lasttime;
				LCNode *Next;

				LCNode() {Next = NULL; DivCls[0] = L'\0';};
				~LCNode() {if(Next != NULL) delete Next;};
			};

			LCNode* SLNCList;
			LCNode* SLNCSearcher;
			SLNCList = new LCNode;
			SLNCSearcher = NULL;

			while(link != NULL)
			{
				if(link->valid == 0)
				{
					link = link->next;
					continue;
				}

				if(Searching_Prof != PROF_SEARCH_MODE)
				{
					for(i = 0; i < 6; i++)
					{
						if(mycode[i] != link->code[i])
							break;
					}
					if( i != 6)
					{
						link = link->next;
						continue;
					}
				}
				else
				{
					if(wcsstr(link->teacher, Searching_Prof_str) == NULL)
					{
						link = link->next;
						continue;
					}
				}

				for(j = 0; j < 3; j++)
				{
					i = link->time_info[j];
					int k;
					int myday, mylasttime;
					if(IS_DAY_VALIED(i))
					{
						for(k = 0; !(i & (MON >> k)); k++)
						{

						}
						rc.left = TT_X_START + (TT_X_CLM * (k + 1)) + 1;
						rc.right = rc.left + TT_X_CLM - 1;
						myday = k;

						for(k = 0; !(i & (t01A << k)); k++)
						{

						}
						rc.top = TT_Y_START + (TT_Y_CLM * (k + 1)) + 1;

						for(; (i & (t01A << k)); k++)
						{

						}
						rc.bottom = TT_Y_START + (TT_Y_CLM * (k + 1));
						mylasttime = k;

						Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

						SLNCSearcher = SLNCList;
						while(SLNCSearcher->Next != NULL && !(SLNCSearcher->Next->day == myday && SLNCSearcher->Next->lasttime == mylasttime))
						{
							SLNCSearcher = SLNCSearcher->Next;
						}

						if(SLNCSearcher->Next == NULL)
						{
							SLNCSearcher->Next = new LCNode;

							if(Searching_Prof == PROF_SEARCH_MODE)
							{
								wcscpy_s(SLNCSearcher->Next->DivCls, link->name);
								wcscat_s(SLNCSearcher->Next->DivCls, L"-");
							}
						}

						SLNCSearcher = SLNCSearcher->Next;

						temp = new wchar_t[5];
						temp[0] = link->div_class / 10 + L'0';
						temp[1] = link->div_class % 10 + L'0';
						if(link->target_eng == L'Y')
						{
							temp[2] = L'*';
							temp[3] = L',';
							temp[4] = L'\0';
						}
						else
						{
							temp[2] = L',';
							temp[3] = L'\0';
						}

						if(wcsstr(SLNCSearcher->DivCls, temp) == NULL)
							wcscat_s(SLNCSearcher->DivCls, temp);
						delete[] temp;
						SLNCSearcher->day = myday;
						SLNCSearcher->lasttime = mylasttime;
					}
				}

				link = link->next;
			}

			SetBkMode(hdc, OPAQUE);
			SetBkColor(hdc, RGB(255, 255, 0));

			SLNCSearcher = SLNCList;
			SLNCSearcher = SLNCSearcher->Next;
			while(SLNCSearcher != NULL)
			{
				rc.left = TT_X_START + (TT_X_CLM * (SLNCSearcher->day + 1)) + 2;
				rc.right = rc.left + TT_X_CLM - 3;
				rc.bottom = TT_Y_START + (TT_Y_CLM * (SLNCSearcher->lasttime + 1));
				rc.top = rc.bottom - 12;

				DrawText(hdc, SLNCSearcher->DivCls, wcslen(SLNCSearcher->DivCls), &rc, 0);
				SLNCSearcher = SLNCSearcher->Next;
			}

			delete SLNCList;

			SetBkColor(hdc, RGB(255, 255, 255));

			SelectObject(hdc, def_pen);
			DeleteObject(pen);

			SelectObject(hdc, def_brush);
		}
	}

	SelectObject(hdc, def_font);
	DeleteObject(font);
}

void Show_Lecture(HDC hdc, LECTURE* lc, const int TT_X_START, const int TT_Y_START, int mode)
{
	wchar_t* temp;
	wchar_t* temp2;
	int i;
	RECT rc;

	int TT_X_CLM, TT_Y_CLM;
	int TT_Y_SEPERATED;

	if(mode == PAINT_WINDOW_MODE)
	{
		TT_X_CLM = TT_X_CLM_W;
		TT_Y_CLM = TT_Y_CLM_W;
		TT_Y_SEPERATED = 12;
	}
	else if(mode == PAINT_PRINT_MODE)
	{
		TT_X_CLM = TT_X_CLM_P;
		TT_Y_CLM = TT_Y_CLM_P;
		TT_Y_SEPERATED = TT_Y_CLM * 7 / 10;
	}
	else
	{
		MessageBox(MainhWnd, L"Invalid Paint Mode!", L"Error", MB_OK);
		return;
	}

	for(int j = 0; j < 3; j++)
	{
		i = lc->time_info[j];
		int k;
		if(IS_DAY_VALIED(i))
		{
			for(k = 0; !(i & (MON >> k)); k++)
			{
					
			}
			rc.left = TT_X_START + (TT_X_CLM * (k + 1)) + 1;
			rc.right = rc.left + TT_X_CLM - 1;
	
			for(k = 0; !(i & (t01A << k)); k++)
			{
	
			}
			rc.top = TT_Y_START + (TT_Y_CLM * (k + 1)) + 1;
	
			for(; (i & (t01A << k)); k++)
			{
				
			}
			rc.bottom = TT_Y_START + (TT_Y_CLM * (k + 1));
	
			Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
		
			rc.left++;
			rc.top++;

			SetBkMode(hdc,TRANSPARENT);

			int tempSize = 30;
			temp = new wchar_t[tempSize];
			wcscpy_s(temp, tempSize, lc->code);
			wcscat_s(temp, tempSize, L"-");
			temp2 = new wchar_t[3];
			temp2[0] = lc->div_class / 10 + L'0';
			temp2[1] = lc->div_class % 10 + L'0';
			temp2[2] = L'\0';
			wcscat_s(temp, tempSize, temp2);
			DrawText(hdc, temp, 9, &rc, 0);
			rc.top+= TT_Y_SEPERATED;
			rc.bottom += TT_Y_SEPERATED;
			if(lc->target_eng == L'Y')
				wcscpy_s(temp, tempSize, L"*");
			else
				wcscpy_s(temp, tempSize, L"");
			wcscat_s(temp, tempSize, lc->name);
			DrawText(hdc, temp, wcslen(temp), &rc, 0);

			rc.top += TT_Y_SEPERATED;
			rc.bottom += TT_Y_SEPERATED;

			wcscpy_s(temp, tempSize, L"교수:");
			wcscat_s(temp, tempSize, lc->teacher);
			DrawText(hdc, temp, wcslen(temp), &rc, 0);

			delete[] temp;
			delete[] temp2;
	
			for(; !(i & (t01A << k)); k++)
			{
				if(k > 25)
					break;
			}

			if(k <= 25)
			{
				rc.top = TT_Y_START + (TT_Y_CLM * (k + 1)) + 1;
				for(; (i & (t01A << k)); k++)
				{
			
				}
				rc.bottom = TT_Y_START + (TT_Y_CLM * (k + 1));
				rc.left--;
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
				rc.left++;
				rc.top++;
				tempSize = 30;
				temp = new wchar_t[tempSize];
				wcscpy_s(temp, tempSize, lc->code);
				wcscat_s(temp, tempSize, L"-");
				temp2 = new wchar_t[3];
				temp2[0] = lc->div_class / 10 + '0';
				temp2[1] = lc->div_class % 10 + '0';
				temp2[2] = '\0';
				wcscat_s(temp, tempSize, temp2);
				DrawText(hdc, temp, 9, &rc, 0);

				rc.top += TT_Y_SEPERATED;
				rc.bottom += TT_Y_SEPERATED;
				wcscpy_s(temp, tempSize, lc->name);
				DrawText(hdc, temp, wcslen(temp), &rc, 0);

				rc.top += TT_Y_SEPERATED;
				rc.bottom += TT_Y_SEPERATED;

				wcscpy_s(temp, tempSize, L"교수:");
				wcscat_s(temp, tempSize, lc->teacher);
				DrawText(hdc, temp, wcslen(temp), &rc, 0);

				delete[] temp;
				delete[] temp2;
			}

		}
	}
}

void WM_Command(HWND hWnd, WPARAM wParam)
{
	int i, j = 0;
	LECTURE *link;
	wchar_t *temp;
	wchar_t mycode[7];
	wchar_t *ret;
	MyLecture *Iterator;

	switch (LOWORD(wParam)) 
	{
	case ID_COMBO_MAJOR:
		switch (HIWORD(wParam)) 
		{
		case CBN_SELCHANGE:
			while(SendMessage(hList_CT, LB_DELETESTRING, 0, 0) > 0);
			while(SendMessage(hList_Lecture, LB_DELETESTRING, 0, 0) > 0);
			while(SendMessage(hList_DivCls, LB_DELETESTRING, 0, 0) > 0);
			Selected = NULL;
			Selected_Mode = SL_MODE_UNDEF;
			Searching_Prof = NORMAL_MODE;

			Selected_Major=SendMessage(hCombo_Major, CB_GETCURSEL,0,0);
			for (i = 0; i < CT_COUNT; i++)
			{
				SendMessage(hList_CT, LB_ADDSTRING, 0, (LPARAM)CTName[i]);
			}
			break;
		}
		break;
	case ID_LIST_CT:
		switch (HIWORD(wParam))
		{
		case LBN_SELCHANGE:
			while(SendMessage(hList_Lecture, LB_DELETESTRING, 0, 0) > 0);
			while(SendMessage(hList_DivCls, LB_DELETESTRING, 0, 0) > 0);
			Selected = NULL;
			Selected_Mode = SL_MODE_UNDEF;
			Selected_Lecture = -1;
			Searching_Prof = NORMAL_MODE;

			if(List != NULL)
			{
				delete List;
				List = NULL;
			}

			if(List == NULL)
				List = new SameList(L"\0");

			Selected_CT = SendMessage(hList_CT, LB_GETCURSEL, 0, 0);

			link = Lecture;
			while(link != NULL)
			{
				if(link->valid == 0)
				{
					link = link->next;
					continue;
				}
				if(link->major[Selected_Major] == Selected_CT)
				{
					if(List->AddList(link->code) == 1)
					{
						int tempSize = 40;
						temp = new wchar_t[tempSize];
						wcscpy_s(temp, tempSize, link->code);
						wcscat_s(temp, tempSize, L" ");
						wcscat_s(temp, tempSize, link->name);
						SendMessage(hList_Lecture, LB_ADDSTRING, 0, (LPARAM)temp);

						delete[] temp;
					}
				}
				link = link->next;
			}

			break;
		}
		break;

	case ID_BTN_PROF_SEARCH:
		j++;		// j==2 일때 교수님검색

	case ID_BTN_NAME_SEARCH:
		j++;		// j==1 일때 이름검색

	case ID_BTN_SEARCH:
		{			// j==0 일때 코드검색
			while(SendMessage(hList_Lecture, LB_DELETESTRING, 0, 0) > 0);
			while(SendMessage(hList_DivCls, LB_DELETESTRING, 0, 0) > 0);
			Selected = NULL;
			Selected_Mode = SL_MODE_UNDEF;
			Selected_Lecture = -1;
			 
			if(j != 2)
				Searching_Prof = NORMAL_MODE;
			else
				Searching_Prof = PROF_SEARCH_MODE;

			if(List != NULL)
			{
				delete List;
				List = NULL;
			}
	
			List = new SameList(L"\0");
	
			wchar_t* temp2 = new wchar_t[30];

			if(j == 0)
				GetWindowText(hEdit_Search, temp2, 7);
			else if(j == 1)
				GetWindowText(hEdit_Name_Search, temp2, 29);
			else
			{
				GetWindowText(hEdit_Prof_Search, temp2, 9);

				if(wcslen(temp2) <= 1)	// 교수님 이름 검색에서는 글자수 1개 이하일때 검색거부
					break;
			}
	
			for(i = 0; temp2[i] != L'\0'; i++)
			{
				if(temp2[i] >= 'a' && temp2[i] <= 'z')
					temp2[i] -= ('a' - 'A');
			}

			if(j == 2)
				wcscpy_s(Searching_Prof_str, temp2);

			link = Lecture;
			while(link != NULL)
			{
				if(link->valid == 0)
				{
					link = link->next;
					continue;
				}

				if(j == 0)
					ret = wcsstr(link->code, temp2);
				else if(j == 1)
					ret = wcsstr(link->name, temp2);
				else
					ret = wcsstr(link->teacher, temp2);

				if(ret == NULL)
				{
					link = link->next;
					continue;
				}
	
				if(List->AddList(link->code) == 1)
				{
					int tempSize = 40;
					temp = new wchar_t[tempSize];
					wcscpy_s(temp, tempSize, link->code);
					wcscat_s(temp, tempSize, L" ");
					wcscat_s(temp, tempSize, link->name);
					SendMessage(hList_Lecture, LB_ADDSTRING, 0, (LPARAM)temp);
	
					delete[] temp;
				}
					
				link = link->next;
			}
	
			delete[] temp2;
		}

		break;

	case ID_LIST_LECTURE:
		switch(HIWORD(wParam))
		{
		case LBN_SELCHANGE:
			while(SendMessage(hList_DivCls, LB_DELETESTRING, 0, 0) > 0);
			Selected = NULL;
			Selected_Mode = SL_MODE_UNDEF;

			if(List == NULL)
				break;
			Selected_Lecture = SendMessage(hList_Lecture, LB_GETCURSEL, 0, 0);

			List->GetCode(Selected_Lecture, mycode);

			link = Lecture;
			while(link != NULL)
			{
				if(link->valid == 0)
				{
					link = link->next;
					continue;
				}
				for(i = 0; i < 7; i++)
				{
					if(mycode[i] != link->code[i])
						break;
				}

				if(i != 7)
				{
					link = link->next;
					continue;
				}

				if(Searching_Prof == PROF_SEARCH_MODE && wcsstr(link->teacher, Searching_Prof_str) == NULL)
				{
					link = link->next;
					continue;
				}

				int tempSize = 150;
				temp = new wchar_t[tempSize];
				temp[0] = link->div_class / 10 + '0';
				temp[1] = link->div_class % 10 + '0';
				if(link->target_eng == L'Y')
				{
					temp[2] = L'*';
					temp[3] = L' ';
					temp[4] = L'\0';
				}
				else
				{
					temp[2] = ' ';
					temp[3] = '\0';
				}

				wcscat_s(temp, tempSize, link->target_dep_year);

				wcscat_s(temp, tempSize, L" ");

				wcscat_s(temp, tempSize, link->teacher);

				SendMessage(hList_DivCls, LB_ADDSTRING, 0, (LPARAM)temp);

				delete[] temp;
				link = link->next;
			}
		}

		break;

	case ID_LIST_DIVCLS:
		switch(HIWORD(wParam))
		{
		case LBN_SELCHANGE:
			Selected_DivCls = SendMessage(hList_DivCls, LB_GETCURSEL, 0, 0);
// 이때 Selected_DivCls는 임시 색인값
			List->GetCode(Selected_Lecture, mycode);

			link = Lecture;
			j = 0;
			while(link != NULL)
			{
				if(link->valid == 0)
				{
					link = link->next;
					continue;
				}
				for(i = 0; i < 6; i++)
				{
					if(mycode[i] != link->code[i])
						break;
				}
				if( i != 6)
				{
					link = link->next;
					continue;
				}

				if(Searching_Prof == PROF_SEARCH_MODE && wcsstr(link->teacher, Searching_Prof_str) == NULL)
				{
					link = link->next;
					continue;
				}

				if(j == Selected_DivCls)
				{
					Selected_DivCls = link->div_class;
					Selected = link;
					Selected_Mode = SL_MODE_NEW;
					break;
					// Selected_DivCls는 이때 제대로 된 분반 정보를 가짐
				}
				j++;

				link = link->next;
			}
		}

		break;

	case ID_LIST_SELECTED:
		switch(HIWORD(wParam))
		{
		case LBN_SELCHANGE:
			int temptarget;
			MyLecture* Iterator;
			while(SendMessage(hList_DivCls, LB_DELETESTRING, 0, 0) > 0);
			Selected_Lecture = -1;
			temptarget = SendMessage(hList_Selected, LB_GETCURSEL, 0, 0);

			i = 0;
			Iterator = SelectedList->GetNext();
			while(i < temptarget)
			{
				i++;
				Iterator = Iterator->GetNext();
			}

			Selected = Iterator->GetLecture();
			Selected_Mode = SL_MODE_DEL;
		}
		break;

	case ID_BTN_SAVE:
		{
			OPENFILENAME OFN;
			TCHAR str[300];
			TCHAR lpstrFile[MAX_PATH]=L"";
			wofstream fout;

			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner=hWnd;
			OFN.lpstrFilter=TEXT("한기대 시간표 파일(*.kut)\0*.kut\0");
			OFN.lpstrFile=lpstrFile;
			OFN.nMaxFile=MAX_PATH;

			if (GetSaveFileName(&OFN)!=0) 
			{
				wsprintf(str,TEXT("%s"),OFN.lpstrFile);
				i = 0;
				while(str[i] != L'\0')
				{
					i++;
				}

				if(str[i-4] == L'.' && 
					(str[i-3] == L'k' || str[i-3] == L'K') && 
					(str[i-2] == L'u' || str[i-2] == L'U') && 
					(str[i-1] == L't' || str[i-1] == L'T'))
				{

				}
				else
				{
					wcscat_s(str, L".kut");
				}

				fout.open(str);
				fout.imbue(locale("kor"));

				fout << VERSION_YEAR << endl;
				Iterator = SelectedList;
				Iterator = Iterator->GetNext();
				while(Iterator != NULL)
				{
					LECTURE* lc = Iterator->GetLecture();
					fout << lc->code;
					fout << L'\t';
					fout << wchar_t(lc->div_class / 10 + L'0');
					fout << wchar_t(lc->div_class % 10 + L'0');
					fout << L'\t';
					fout << lc->teacher;
					fout << L'\t';
					fout << lc->name;
					fout << L'\n';

					Iterator = Iterator->GetNext();
				}

				fout << endl;

				fout.close();
			}				
		}

		break;

	case ID_BTN_LOAD:
		{
			OPENFILENAME OFN;
			TCHAR str[300];
			TCHAR lpstrFile[MAX_PATH]=L"";
			wifstream fin;
			wchar_t buffer[7];
			wchar_t ch;
			wchar_t ch2;
			int buf_divcls;

			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner=hWnd;
			OFN.lpstrFilter=TEXT("한기대 시간표 파일(*.kut)\0*.kut\0");
			OFN.lpstrFile=lpstrFile;
			OFN.nMaxFile=MAX_PATH;

			if (GetOpenFileName(&OFN)!=0) 
			{
				if(SelectedList != NULL)
				{
					delete SelectedList;
					SelectedList = new MyLecture(NULL);
				}

				while(SendMessage(hList_Selected, LB_DELETESTRING, 0, 0) > 0);

				Score = 0;

				Selected = NULL;
				Selected_Mode = SL_MODE_UNDEF;
				Searching_Prof = NORMAL_MODE;
				Selected_Lecture = -1;
				while(SendMessage(hList_CT, LB_DELETESTRING, 0, 0) > 0);
				while(SendMessage(hList_Lecture, LB_DELETESTRING, 0, 0) > 0);
				while(SendMessage(hList_DivCls, LB_DELETESTRING, 0, 0) > 0);

				wsprintf(str,TEXT("%s"),OFN.lpstrFile);
				fin.open(str);

				fin >> i;
				fin.get(ch); // Enter Remover
				if(i != VERSION_YEAR)
				{
					MessageBox(hWnd, L"시간표파일 버젼이 다릅니다!", L"시간표 도우미", MB_OK);
				}

				else
				{

					while(true)
					{
						for(i = 0; i < 6; i++)
							fin.get(buffer[i]);
						buffer[i] = L'\0';
						fin.get(ch);	// Tab Remover
						fin.get(ch);
						fin.get(ch2);

						if(fin.eof())
							break;

						buf_divcls = tonumber(ch, ch2);

						link = Lecture;
						while(link != NULL)
						{
							if(link->valid == 0)
							{
								link = link->next;
								continue;
							}
							for(i = 0; i < 6; i++)
							{
								if(buffer[i] != link->code[i])
									break;
							}

							if(i != 6)
							{
								link = link->next;
								continue;
							}

							if(buf_divcls == link->div_class)
							{
								if(SelectedList->AddLecture(link) == 0)
								{
									MessageBox(hWnd, L"시간이 겹치는 과목이 있습니다.", L"kut", MB_OK);
									break;
								}

								wchar_t* temp2;

								int tempSize = 100;
								temp = new wchar_t[tempSize];
								temp2 = new wchar_t[3];

								temp[0] = L'\0';
								wcscat_s(temp, tempSize, link->code);
								wcscat_s(temp, tempSize, L"-");
								temp2[0] = link->div_class / 10 + '0';
								temp2[1] = link->div_class % 10 + '0';
								temp2[2] = L'\0';
								wcscat_s(temp, tempSize, temp2);
								if(link->target_eng == L'Y')
									wcscat_s(temp, tempSize, L"*");
								wcscat_s(temp, tempSize, L" ");
								wcscat_s(temp, tempSize, link->teacher);
								wcscat_s(temp, tempSize, L" ");
								wcscat_s(temp, tempSize, link->name);

								SendMessage(hList_Selected, LB_ADDSTRING, 0, (LPARAM)temp);

								Score += link->pou_srt;
								break;
							}

							link = link->next;
						}

						do
						{
							fin.get(ch);
							if(fin.eof())
								break;
						}while(ch != L'\n');
						if(fin.eof())
							break;
					}

				}

				fin.close();
			}
		}

		break;

	case ID_BTN_INIT:
		{
			HRESULT rs;
			rs = MessageBox(hWnd, L"정말 초기화하시겠습니까?", L"kut_tt_maker", MB_YESNO);

			if(rs == IDNO)
				break;
		}

		if(SelectedList != NULL)
		{
			delete SelectedList;
			SelectedList = new MyLecture(NULL);
		}
		while(SendMessage(hList_Selected, LB_DELETESTRING, 0, 0) > 0);

		Score = 0;

		Selected = NULL;
		Selected_Mode = SL_MODE_UNDEF;
		Searching_Prof = NORMAL_MODE;
		Selected_Lecture = -1;
		while(SendMessage(hList_CT, LB_DELETESTRING, 0, 0) > 0);
		while(SendMessage(hList_Lecture, LB_DELETESTRING, 0, 0) > 0);
		while(SendMessage(hList_DivCls, LB_DELETESTRING, 0, 0) > 0);

		break;

	case ID_BTN_COMMAND:
		{
			if (Selected == NULL || Selected_Mode != SL_MODE_NEW)
			{
				MessageBox(hWnd, L"먼저 과목과 분반을 선택해 주십시오", L"kut", MB_OK);
				break;
			}
			if (SelectedList->CheckLecture(Selected) == 1)
			{
				MessageBox(hWnd, L"똑같은 과목을 고르셨습니다", L"kut", MB_OK);
				break;
			}
			if (SelectedList->AddLecture(Selected) == 0)
			{
				MessageBox(hWnd, L"시간이 겹치는 과목이 있습니다.", L"kut", MB_OK);
				break;
			}

			wchar_t* temp2;

			int tempSize = 100;
			temp = new wchar_t[tempSize];
			temp2 = new wchar_t[3];

			temp[0] = L'\0';
			wcscat_s(temp, tempSize, Selected->code);
			wcscat_s(temp, tempSize, L"-");
			temp2[0] = Selected->div_class / 10 + L'0';
			temp2[1] = Selected->div_class % 10 + L'0';
			temp2[2] = L'\0';
			wcscat_s(temp, tempSize, temp2);
			wcscat_s(temp, tempSize, L" ");
			wcscat_s(temp, tempSize, Selected->teacher);
			wcscat_s(temp, tempSize, L" ");
			wcscat_s(temp, tempSize, Selected->name);

			SendMessage(hList_Selected, LB_ADDSTRING, 0, (LPARAM)temp);

			Score += Selected->pou_srt;

			Selected = NULL;
			Selected_Mode = SL_MODE_UNDEF;
			delete[] temp;
			delete[] temp2;
		}

		break;

	case ID_BTN_DELCMD:
		if(Selected == NULL || Selected_Mode != SL_MODE_DEL)
		{
			MessageBox(hWnd, L"먼저 삭제할 과목을 선택해 주세요.", L"kut", MB_OK);
			break;
		}

		SelectedList->DelLecture(Selected);
		SendMessage(hList_Selected, LB_DELETESTRING, SendMessage(hList_Selected, LB_GETCURSEL, 0, 0), 0);

		Score -= Selected->pou_srt;

		Selected = NULL;
		Selected_Mode = SL_MODE_UNDEF;

		break;

	case ID_BTN_PRINT:
		Print();
		
		break;

	}
}



void Print()
{
	PRINTDLG pd;
	HDC hPrtdc;
	DEVMODE *dm;
	int xpage, ypage;
	int dpiX, dpiY;
	DOCINFO doc;
	int Result;
	wchar_t str[]=L"강의 시간표";

	memset(&pd,0,sizeof(PRINTDLG));
	pd.lStructSize=sizeof(PRINTDLG);
	pd.Flags=PD_RETURNDC;
	pd.hwndOwner=MainhWnd;
	pd.nFromPage=1;
	pd.nToPage=1;
	pd.nMinPage=1;
	pd.nMaxPage=1;
	pd.nCopies=1;

	pd.hDevMode=GlobalAlloc(GHND,sizeof(DEVMODE));
	dm=(DEVMODE *)GlobalLock(pd.hDevMode);
	dm->dmSize=sizeof(DEVMODE);
	dm->dmFields=DM_ORIENTATION;
	dm->dmOrientation=DMORIENT_PORTRAIT;
	GlobalUnlock(pd.hDevMode);

	PrintDlg(&pd);
	hPrtdc=pd.hDC;
	if (hPrtdc == NULL) 
		return;

	// 인쇄 작업을 시작한다.
	doc.cbSize=sizeof(DOCINFO);
	doc.lpszDocName=L"강의 시간표";
	doc.lpszOutput=NULL;
	doc.lpszDatatype=NULL;
	doc.fwType=0;
	Result=StartDoc(hPrtdc, &doc);

	if (Result > 0)
	{
		Result = StartPage(hPrtdc);

		if (Result > 0)
		{

			// 프린터의 해상도를 구한다.
			dpiX = GetDeviceCaps(hPrtdc, LOGPIXELSX);
			dpiY = GetDeviceCaps(hPrtdc, LOGPIXELSY);
			xpage = GetDeviceCaps(hPrtdc, HORZRES);
			ypage = GetDeviceCaps(hPrtdc, VERTRES);

			TT_X_START_P = xpage / 14;
			TT_Y_START_P = ypage / 30;
			TT_X_CLM_P = xpage / 8;
			TT_Y_CLM_P = ypage / 30;

			WM_Paint(hPrtdc, TT_X_START_P, TT_Y_START_P, PAINT_PRINT_MODE);


			// 출력을 종료한다.
			Result = EndPage(hPrtdc);
			if (Result > 0)
			{

				// 인쇄 작업을 끝낸다.
				Result = EndDoc(hPrtdc);
			}
		}
	}

	if (pd.hDevMode)
		GlobalFree(pd.hDevMode);
	if (pd.hDevNames)
		GlobalFree(pd.hDevNames);

	DeleteDC(hPrtdc);
	SetCursor(LoadCursor(NULL,IDC_ARROW));

	return;
}
