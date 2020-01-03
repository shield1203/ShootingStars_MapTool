// StageTool.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "StageTool.h"
#include <commdlg.h>
#include <CommDlg.h>
#include <commctrl.h>
#include<windowsx.h>
#include <stdio.h>
#include"CFileManager.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    DlgProc(HWND, UINT, WPARAM, LPARAM);
HWND g_hWnd, g_hMenuWnd, g_hDlg;
HDC g_hdc;
CFileManager g_FileManager;

RECT rcBack = { 0, 50, 700, 850 }, rcEnemy;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString(hInstance, IDC_STAGETOOL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_STAGETOOL));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_STAGETOOL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+3);
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_STAGETOOL);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
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

   HWND hWnd;
   g_hWnd = hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

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
//
int FileOpenDilaog(HWND hWnd, char* strFilePath, char* strFileName, char* strFilter, const char* strDefExt)
{
	OPENFILENAME ofn;
	char strFileTitle[MAX_PATH] = { 0, };

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrTitle = "읽을 파일을 선택해 주세요";
	ofn.lpstrFileTitle = strFileName; // global, static 변수 사용
	ofn.lpstrFile = strFilePath; // global, static 변수
	ofn.lpstrFilter = strFilter;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrDefExt = strDefExt;

	if (GetOpenFileName(&ofn) != 0)
	{
		return ofn.nFilterIndex;
	}

	return -1; //선택이 안된 경우
}

int FileSaveDilaog(HWND hWnd, char* strFilePath, char* strFileName, char* strFilter)
{
	OPENFILENAME ofn;
	char strFileTitle[MAX_PATH] = { 0, };

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrTitle = "저장할 파일을 선택해 주세요";
	ofn.lpstrFileTitle = strFileName; // global, static 변수 사용
	ofn.lpstrFile = strFilePath; // global, static 변수
	ofn.lpstrFilter = strFilter;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrDefExt = "xml";

	if (GetSaveFileName(&ofn) != 0)
	{
		return ofn.nFilterIndex;
	}

	return -1; //선택이 안된 경우
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RECT rtWnd = { 0, 0, 750, 900 }, rtDlg;
	static SIZE szWndSize, szDlgSize;
	OPENFILENAME ofn;
	static char strFilePath[MAX_PATH], strFileName[100];
	static char strFilter[] = "(*.xml)\0*.xml\0";
	char buffer[100];

	static int nXPos, nYPos;

    switch (message)
    {
	case WM_CREATE:
	{
		g_hMenuWnd = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DlgProc);

		AdjustWindowRect(&rtWnd, WS_OVERLAPPEDWINDOW, TRUE);
		GetWindowRect(g_hMenuWnd, &rtDlg);
		szWndSize.cx = rtWnd.right - rtWnd.left;
		szWndSize.cy = rtWnd.bottom - rtWnd.top;
		szDlgSize.cx = rtDlg.right - rtDlg.left;
		szDlgSize.cy = rtDlg.bottom - rtDlg.top;
		MoveWindow(hWnd, 10, 10, szWndSize.cx, szWndSize.cy, TRUE);
		MoveWindow(g_hMenuWnd, 10 + szWndSize.cx, 10, szDlgSize.cx, szDlgSize.cy, TRUE);
		break;
	}
	case WM_SIZE:
	{
		GetWindowRect(hWnd, &rtWnd);
		MoveWindow(hWnd, rtWnd.left, rtWnd.top, szWndSize.cx, szWndSize.cy, TRUE);
		break;
	}
	case WM_MOVE:
	{
		GetWindowRect(hWnd, &rtWnd);
		MoveWindow(hWnd, rtWnd.left, rtWnd.top, szWndSize.cx, szWndSize.cy, TRUE);
		MoveWindow(g_hMenuWnd, rtWnd.right, rtWnd.top, szDlgSize.cx, szDlgSize.cy, TRUE);
		break;
	}
	case WM_LBUTTONDOWN:
	{
		nXPos = LOWORD(lParam);
		nYPos = HIWORD(lParam);

		if (nXPos > 700) {
			nXPos = 700;
		}

		if (nYPos < 50) {
			nYPos = -50;
		}
		else if (nYPos > 850) {
			nYPos = 850;
		}

		rcEnemy = { nXPos, nYPos, nXPos + 100, nYPos + 100 };

		g_hdc = GetDC(g_hWnd);
		PatBlt(g_hdc, 0, 0, 750, 900, BLACKNESS);
		FillRect(g_hdc, &rcBack, CreateSolidBrush(RGB(0, 100, 255)));
		FrameRect(g_hdc, &rcEnemy, CreateSolidBrush(RGB(255, 255, 255)));
		
		nXPos += 1000;
		nYPos -= 50;
		SetDlgItemInt(g_hMenuWnd, IDC_EDIT3, nXPos, FALSE);
		SetDlgItemInt(g_hMenuWnd, IDC_EDIT4, nYPos, TRUE);

		break;
	}
    case WM_COMMAND:
        {
			int wmId = LOWORD(wParam);
			int wmEvent = HIWORD(wParam);

            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
			case ID_32771: // Open
			{
				if (FileOpenDilaog(hWnd, strFilePath, strFileName, strFilter, "xml") != -1)
				{
					g_FileManager.LoadFile(strFilePath);

					// 읽은 데이터를 컨트롤에 설정하는 부분

					ListView_DeleteAllItems(g_hDlg);

					for (int i = 0; i < g_FileManager.m_EnemyInfo.size(); i++)
					{
						LVITEM ITEM;
						ITEM.mask = LVIF_TEXT;
						ITEM.state = 0;
						ITEM.stateMask = 0;
						ITEM.iSubItem = 0;
						ITEM.iItem = i; // Note: 추가 아이템 등록 인덱스 

						sprintf(buffer, "%d", i);
						ITEM.pszText = buffer;
						ListView_InsertItem(g_hDlg, &ITEM);
						
						sprintf(buffer, "%s", g_FileManager.m_EnemyInfo[i]->strEnemyType.c_str());
						ListView_SetItemText(g_hDlg, i, 1, buffer);

						sprintf(buffer, "%d", g_FileManager.m_EnemyInfo[i]->nXPos);
						ListView_SetItemText(g_hDlg, i, 2, buffer);

						sprintf(buffer, "%d", g_FileManager.m_EnemyInfo[i]->nYPos);
						ListView_SetItemText(g_hDlg, i, 3, buffer);

						sprintf(buffer, "%s", g_FileManager.m_EnemyInfo[i]->strMovePattern.c_str());
						ListView_SetItemText(g_hDlg, i, 4, buffer);

						sprintf(buffer, "%s", g_FileManager.m_EnemyInfo[i]->strAttackPattern.c_str());
						ListView_SetItemText(g_hDlg, i, 5, buffer);

						sprintf(buffer, "%d", g_FileManager.m_EnemyInfo[i]->nHP);
						ListView_SetItemText(g_hDlg, i, 6, buffer);

						sprintf(buffer, "%d", g_FileManager.m_EnemyInfo[i]->dwTime);
						ListView_SetItemText(g_hDlg, i, 7, buffer);
					}
				}
				break;
			}
			case ID_32772: // Save
			{
				if (FileSaveDilaog(hWnd, strFilePath, strFileName, strFilter) != -1)
				{	// 저장파일 형식이 한 가지이므로 파일 선택 인덱스를 조사할 필요가 없다.				
					int nCount = ListView_GetItemCount(g_hDlg);
					
					g_FileManager.m_EnemyInfo.clear();

					for (int i = 0; i < nCount; i++)
					{
						ENEMY_INFO* addEnemy = new ENEMY_INFO();

						ListView_GetItemText(g_hDlg, i, 1, buffer, 40);
						addEnemy->strEnemyType = buffer;
						ListView_GetItemText(g_hDlg, i, 2, buffer, 10);
						addEnemy->nXPos = atoi(buffer);
						ListView_GetItemText(g_hDlg, i, 3, buffer, 10);
						addEnemy->nYPos = atoi(buffer);
						ListView_GetItemText(g_hDlg, i, 4, buffer, 40);
						addEnemy->strMovePattern = buffer;
						ListView_GetItemText(g_hDlg, i, 5, buffer, 40);
						addEnemy->strAttackPattern = buffer;
						ListView_GetItemText(g_hDlg, i, 6, buffer, 10);
						addEnemy->nHP = atoi(buffer);
						ListView_GetItemText(g_hDlg, i, 7, buffer, 10);
						addEnemy->dwTime = atoi(buffer);

						g_FileManager.m_EnemyInfo.push_back(addEnemy);
					}

					g_FileManager.SaveFile(strFilePath);
				}
				break;
			}
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
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
            g_hdc = BeginPaint(hWnd, &ps);

			PatBlt(g_hdc, 0, 0, 750, 900, BLACKNESS);
			FillRect(g_hdc, &rcBack, CreateSolidBrush(RGB(0, 100, 255)));
			FrameRect(g_hdc, &rcEnemy, CreateSolidBrush(RGB(255, 255, 255)));
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		DestroyWindow(g_hMenuWnd);
        PostQuitMessage(0);
        break;
    default:
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
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK DlgProc   (HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	LVCOLUMN COL;
	LVITEM ITEM;
	int idx, nCount;
	char buffer[100];

	static const char* strEnemyName[] = { "NOMAL_NECO", "GRAY_NECO", "MECA_GIRL" };
	static const char* strMovePattern[] = { "MOVE_R_L", "MOVE_R_L_FS", "MOVE_R_L_ST", "MOVE_U_D", "MOVE_U_D_FS", "MOVE_U_D_ST",
		"MOVE_D_U", "MOVE_D_U_FS", "MOVE_D_U_ST", "MOVE_RU_LD", "MOVE_C_R_L" };
	static const char* strAttackPattern[] = { "TYPE_1", "TYPE_2", "TYPE_3", "TYPE_4", "TYPE_5", "TYPE_6", "TYPE_7", "TYPE_8", "TYPE_9" };

	switch (message)
	{
	case WM_INITDIALOG:
	{
		g_hDlg = GetDlgItem(hDlg, IDC_COMBO1);
		for (int i = 0; i < 3; i++) {
			ComboBox_AddString(g_hDlg, strEnemyName[i]); // #include<windowsx.h>
		}

		g_hDlg = GetDlgItem(hDlg, IDC_COMBO2);
		for (int i = 0; i < 11; i++) {
			ComboBox_AddString(g_hDlg, strMovePattern[i]);
		}

		g_hDlg = GetDlgItem(hDlg, IDC_COMBO3);
		for (int i = 0; i < 9; i++) {
			ComboBox_AddString(g_hDlg, strAttackPattern[i]);
		}

		g_hDlg = GetDlgItem(hDlg, IDC_LIST1);
		ListView_SetExtendedListViewStyle(g_hDlg, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

		COL.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		COL.fmt = LVCFMT_CENTER;

		// Note: 기초 상태 리스트 박스 셋팅
		COL.cx = 40;
		COL.pszText = (LPSTR)("순서");
		COL.iSubItem = 0;
		SendMessage(g_hDlg, LVM_INSERTCOLUMN, 0, (LPARAM)& COL);

		COL.cx = 70;
		COL.pszText = (LPSTR)("적유닛명 ");
		COL.iSubItem = 1;
		SendMessage(g_hDlg, LVM_INSERTCOLUMN, 1, (LPARAM)& COL);

		COL.cx = 40;
		COL.pszText = (LPSTR)("X");
		COL.iSubItem = 2;
		SendMessage(g_hDlg, LVM_INSERTCOLUMN, 2, (LPARAM)& COL);

		COL.cx = 40;
		COL.pszText = (LPSTR)("Y");
		COL.iSubItem = 3;
		SendMessage(g_hDlg, LVM_INSERTCOLUMN, 3, (LPARAM)& COL);

		COL.cx = 110;
		COL.pszText = (LPSTR)("이동패턴");
		COL.iSubItem = 4;
		SendMessage(g_hDlg, LVM_INSERTCOLUMN, 4, (LPARAM)& COL);

		COL.cx = 70;
		COL.pszText = (LPSTR)("공격패턴");
		COL.iSubItem = 5;
		SendMessage(g_hDlg, LVM_INSERTCOLUMN, 5, (LPARAM)& COL);

		COL.cx = 40;
		COL.pszText = (LPSTR)("체력");
		COL.iSubItem = 6;
		SendMessage(g_hDlg, LVM_INSERTCOLUMN, 6, (LPARAM)& COL);

		COL.cx = 60;
		COL.pszText = (LPSTR)("시간차");
		COL.iSubItem = 7;
		SendMessage(g_hDlg, LVM_INSERTCOLUMN, 7, (LPARAM)& COL);
		return (INT_PTR)TRUE;
	}
	case WM_NOTIFY: // 리스트뷰가 선택되면
	{
		nCount = ListView_GetItemCount(g_hDlg);
		if (nCount > 0)
		{
			int nListIndex = ListView_GetNextItem(g_hDlg, -1, LVNI_ALL | LVNI_SELECTED);
			if (nListIndex != -1)
			{
				ListView_GetItemText(g_hDlg, nListIndex, 2, buffer, 10);
				rcEnemy.left = atoi(buffer) - 1000;
				rcEnemy.right = atoi(buffer) - 900;

				ListView_GetItemText(g_hDlg, nListIndex, 3, buffer, 10);
				rcEnemy.top = atoi(buffer) + 50;
				rcEnemy.bottom = atoi(buffer) + 150;

				g_hdc = GetDC(g_hWnd);
				PatBlt(g_hdc, 0, 0, 750, 900, BLACKNESS);
				FillRect(g_hdc, &rcBack, CreateSolidBrush(RGB(0, 100, 255)));
				FrameRect(g_hdc, &rcEnemy, CreateSolidBrush(RGB(255, 255, 255)));
			}
		}
		return TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			case IDC_BUTTON1: // 등록버튼
			{
				ITEM.mask = LVIF_TEXT;
				ITEM.state = 0;
				ITEM.stateMask = 0;
				ITEM.iSubItem = 0;

				idx = ListView_GetItemCount(g_hDlg);
				ITEM.iItem = idx;

				sprintf(buffer, "%d", idx);
				ITEM.pszText = buffer;
				ListView_InsertItem(g_hDlg, &ITEM);

				GetDlgItemText(hDlg, IDC_COMBO1, buffer, 40);
				ListView_SetItemText(g_hDlg, idx, 1, buffer);

				GetDlgItemText(hDlg, IDC_EDIT3, buffer, 10);
				ListView_SetItemText(g_hDlg, idx, 2, buffer);

				GetDlgItemText(hDlg, IDC_EDIT4, buffer, 10);
				ListView_SetItemText(g_hDlg, idx, 3, buffer);

				GetDlgItemText(hDlg, IDC_COMBO2, buffer, 40);
				ListView_SetItemText(g_hDlg, idx, 4, buffer);

				GetDlgItemText(hDlg, IDC_COMBO3, buffer, 40);
				ListView_SetItemText(g_hDlg, idx, 5, buffer);

				GetDlgItemText(hDlg, IDC_EDIT2, buffer, 10);
				ListView_SetItemText(g_hDlg, idx, 6, buffer);

				GetDlgItemText(hDlg, IDC_EDIT5, buffer, 10);
				ListView_SetItemText(g_hDlg, idx, 7, buffer);

				return (INT_PTR)TRUE;
			}
			case IDC_BUTTON2: // 중간에 삽입
			{
				idx = ListView_GetNextItem(g_hDlg, -1, LVNI_ALL | LVNI_SELECTED);

				if (idx == -1)
					MessageBox(hDlg, "등록부터 해주세요", "주의", MB_OK);
				else
				{
					ITEM.mask = LVIF_TEXT;
					ITEM.state = 0;
					ITEM.stateMask = 0;
					ITEM.iSubItem = 0;
					ITEM.iItem = idx; // Note: 추가 아이템 등록 인덱스 

					sprintf(buffer, "%d", idx);
					ITEM.pszText = buffer;
					ListView_InsertItem(g_hDlg, &ITEM);

					GetDlgItemText(hDlg, IDC_COMBO1, buffer, 40);
					ListView_SetItemText(g_hDlg, idx, 1, buffer);

					GetDlgItemText(hDlg, IDC_EDIT3, buffer, 10);
					ListView_SetItemText(g_hDlg, idx, 2, buffer);

					GetDlgItemText(hDlg, IDC_EDIT4, buffer, 10);
					ListView_SetItemText(g_hDlg, idx, 3, buffer);

					GetDlgItemText(hDlg, IDC_COMBO2, buffer, 40);
					ListView_SetItemText(g_hDlg, idx, 4, buffer);

					GetDlgItemText(hDlg, IDC_COMBO3, buffer, 40);
					ListView_SetItemText(g_hDlg, idx, 5, buffer);

					GetDlgItemText(hDlg, IDC_EDIT2, buffer, 10);
					ListView_SetItemText(g_hDlg, idx, 6, buffer);

					GetDlgItemText(hDlg, IDC_EDIT5, buffer, 10);
					ListView_SetItemText(g_hDlg, idx, 7, buffer);

					nCount = ListView_GetItemCount(g_hDlg);
					for (int i = idx + 1; i < nCount; i++)
					{
						sprintf(buffer, "%d", i);
						ListView_SetItemText(g_hDlg, i, 0, buffer);
					}
				}
				return (INT_PTR)TRUE;
			}
			case IDC_BUTTON3: // 삭제
			{
				idx = ListView_GetNextItem(g_hDlg, -1, LVNI_ALL | LVNI_SELECTED);

				if (idx == -1)
					MessageBox(hDlg, "삭제할 항목을 먼저 선택해 주세요", "주의", MB_OK);
				else
				{
					ListView_DeleteItem(g_hDlg, idx);
					nCount = ListView_GetItemCount(g_hDlg);
					for (int i = idx; i < nCount; i++)
					{
						sprintf(buffer, "%d", i);
						ListView_SetItemText(g_hDlg, i, 0, buffer);
					}
				}
				return (INT_PTR)TRUE;
			}
			case IDC_BUTTON4: // 수정
			{
				idx = ListView_GetNextItem(g_hDlg, -1, LVNI_ALL | LVNI_SELECTED);

				if (idx == -1)
					MessageBox(hDlg, "삭제할 항목을 먼저 선택해 주세요", "주의", MB_OK);
				else
				{
					GetDlgItemText(hDlg, IDC_COMBO1, buffer, 40);
					ListView_SetItemText(g_hDlg, idx, 1, buffer);

					GetDlgItemText(hDlg, IDC_EDIT3, buffer, 10);
					ListView_SetItemText(g_hDlg, idx, 2, buffer);

					GetDlgItemText(hDlg, IDC_EDIT4, buffer, 10);
					ListView_SetItemText(g_hDlg, idx, 3, buffer);

					GetDlgItemText(hDlg, IDC_COMBO2, buffer, 40);
					ListView_SetItemText(g_hDlg, idx, 4, buffer);

					GetDlgItemText(hDlg, IDC_COMBO3, buffer, 40);
					ListView_SetItemText(g_hDlg, idx, 5, buffer);

					GetDlgItemText(hDlg, IDC_EDIT2, buffer, 10);
					ListView_SetItemText(g_hDlg, idx, 6, buffer);

					GetDlgItemText(hDlg, IDC_EDIT5, buffer, 10);
					ListView_SetItemText(g_hDlg, idx, 7, buffer);
				}

				return (INT_PTR)TRUE;
			}
		}
		
		break;
	}
	return (INT_PTR)FALSE;
}
