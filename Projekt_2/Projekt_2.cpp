// Projekt_2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Projekt_2.h"
#include <vector>
#include <cstring>
#include <string>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <cmath>
using namespace std;


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

vector<float> Wszystko;
vector<float> PositionX;
vector<float> PositionY;
vector<float> PositionZ;
int Amplituda=1;
int Czas=1;
int przesuw=10;
bool WykresX=false;
bool WykresY=false;
bool WykresZ=false;
RECT Odswiez = {0,0,1300,820};
int b = Wszystko.size();

// buttons
HWND hwndButton;

void uzupelnijWektory(){
	fstream plik;
	plik.open("tekst.txt",ios::in );
	while((!plik.eof()))
	{
		float b;
		plik >> b;
		Wszystko.push_back(b);
	}
	int b = Wszystko.size();
	for (int i=1203; i < b ; i+=12){
		PositionX.push_back((Wszystko[i]*50));
		PositionY.push_back((Wszystko[i+1]*50));
		PositionZ.push_back(((Wszystko[i+2]+1)*50));
	}

	plik.close();
}

void PiszMenu(HDC hdcMem)
{
	TextOut(hdcMem,10, 840,L"Amplituda:",12);
	TextOut(hdcMem,10, 870,L"Przesuniecie:",13);
	TextOut(hdcMem,10, 900,L"Przedzia³ka czasu:",19);
	TextOut(hdcMem,900, 860,L"Czas postoju:",13);
	TextOut(hdcMem,900, 840,L"Czas Ruchu:",11);
	TextOut(hdcMem,1100, 860,L"Iloœæ postojów:",15);
	TextOut(hdcMem,1100, 840,L"Iloœæ ruchów:",13);
}

void PiszPrzedzial(int pozycja,HDC hdcMem){
	Graphics graphics(hdcMem);
	Pen pen1(Color(255, 255, 0, 0));
			TextOut(hdcMem, 15, (pozycja - 25*Amplituda),L"5" , 2);
			TextOut(hdcMem, 15, (pozycja + 25*Amplituda), L"-5", 3);
			TextOut(hdcMem, 15, (pozycja), L"0", 1);
			graphics.DrawLine(&pen1, 20, (pozycja), 2000, (pozycja));
			graphics.DrawLine(&pen1, 20, (pozycja - 25*Amplituda), 2000, (pozycja - 25*Amplituda));
			graphics.DrawLine(&pen1, 20, (pozycja + 25*Amplituda), 2000, (pozycja + 25*Amplituda));
}

void MyOnPaint(HDC hdc,HWND hWnd)
{
	// DOUBLE BUFFER
	RECT rcClient;
	GetClientRect(hWnd, &rcClient);
 
	HDC hdcMem = CreateCompatibleDC(hdc); // ekran w pamieci
	const int nMemDC = SaveDC(hdcMem); // zapisanie danych
 
	HBITMAP hBitmap = CreateCompatibleBitmap(hdc, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top); // stworzenie bitmapy
	SelectObject(hdcMem, hBitmap); //z ekranu w pamieci na bitmape
 
	Graphics graphics(hdcMem);
	SolidBrush back(Color(255, 255, 255)); //biale tlo
	SolidBrush points(Color(255, 0, 0));
 
	//wypelnienie tla
	graphics.FillRectangle(&back, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)); //SM_CXSCREEN - szerokosc ekranu, SM_CYSCREEN - wysokosc ekranu

	Pen pen(Color(255, 100, 100, 100));
	Pen pen1(Color(255, 255, 0, 0));
	 PiszMenu(hdcMem);



	int a = PositionX.size();
	for (int i =10; i < a; i++){
		if(WykresX)graphics.DrawLine(&pen,przesuw+(i-1)*Czas,70-(int)(Amplituda*PositionX[i-1]),przesuw+i*Czas,70-(int)(Amplituda*PositionX[i]));
		if(WykresY)graphics.DrawLine(&pen,przesuw+(i-1)*Czas,400-(int)(Amplituda*PositionY[i-1]), przesuw+i*Czas,400-(int)(Amplituda*PositionY[i]));
		if(WykresZ)graphics.DrawLine(&pen,przesuw+(i-1)*Czas,700-(int)(Amplituda*PositionZ[i-1]),przesuw+i*Czas,700-(int)(Amplituda*PositionZ[i]));
	}
	graphics.DrawLine(&pen, 10, 800, 1300, 800);
	if(przesuw==10)graphics.DrawLine(&pen, 10, 20, 10, 800); //linia pionowa
		 for (int i=0; i < a; i += 25) { // pomiarka pozioma
	TCHAR buf[256];
	swprintf_s(buf, 256, L"%i", i/25);
	TextOut(hdcMem,przesuw+i*Czas, 805, buf, _tcslen(buf));
	if(i==0) continue;
		graphics.DrawLine(&pen, i+12, 800, i+12, 785);
}
		 //Przedzia³ka dla wartosci
		  if(WykresX) PiszPrzedzial( 70, hdcMem);
		  if(WykresY) PiszPrzedzial( 400, hdcMem);
		  if(WykresZ) PiszPrzedzial( 700, hdcMem);
			
		 

	//Dalsza czesc double buffera
	  RECT rcClip;
		GetClipBox(hdc, &rcClip); // obszar przycinania
		//kopiowanie z pamieci na nasz ekran
		BitBlt(hdc, rcClip.left, rcClip.top, rcClip.right - rcClip.left, rcClip.bottom - rcClip.top, hdcMem, rcClip.left, rcClip.top, SRCCOPY);
		//kasowanie
		RestoreDC(hdcMem, nMemDC);
		DeleteObject(hBitmap);
		DeleteObject(hdcMem);
}



int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);



	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PROJEKT_2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJEKT_2));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	GdiplusShutdown(gdiplusToken);
	PositionX.clear();
	PositionY.clear();
	PositionZ.clear();
	Wszystko.clear();

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJEKT_2));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PROJEKT_2);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 1500, 1500, NULL, NULL, hInstance, NULL);
   	uzupelnijWektory();

   hwndButton = CreateWindow(TEXT("button"), TEXT("Zmniejsz"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		200, 840, 100, 30, hWnd, (HMENU)ID_RBUTTON1, GetModuleHandle(NULL), NULL);
    hwndButton = CreateWindow(TEXT("button"), TEXT("Zwieksz"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		300, 840, 100, 30, hWnd, (HMENU)ID_RBUTTON2, GetModuleHandle(NULL), NULL);
	 hwndButton = CreateWindow(TEXT("button"), TEXT("-->"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		300, 870, 100, 30, hWnd, (HMENU)ID_RBUTTON4, GetModuleHandle(NULL), NULL);
	 hwndButton = CreateWindow(TEXT("button"), TEXT("<--"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		200, 870, 100, 30, hWnd, (HMENU)ID_RBUTTON5, GetModuleHandle(NULL), NULL);
	   hwndButton = CreateWindow(TEXT("button"), TEXT("Zmniejsz"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		200, 900, 100, 30, hWnd, (HMENU)ID_RBUTTON6, GetModuleHandle(NULL), NULL);
	   hwndButton = CreateWindow(TEXT("button"), TEXT("Zwieksz"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		300, 900, 100, 30, hWnd, (HMENU)ID_RBUTTON3, GetModuleHandle(NULL), NULL);
	   hwndButton = CreateWindow(TEXT("button"), TEXT("Zwieksz"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		300, 900, 100, 30, hWnd, (HMENU)ID_RBUTTON3, GetModuleHandle(NULL), NULL);
	   hwndButton = CreateWindow(TEXT("button"), TEXT("WykresX"),
		WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
		600, 840, 100, 30, hWnd, (HMENU)ID_BUTTON1, GetModuleHandle(NULL), NULL);
	   hwndButton = CreateWindow(TEXT("button"), TEXT("WykresY"),
		WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
		700, 840, 100, 30, hWnd, (HMENU)ID_BUTTON2, GetModuleHandle(NULL), NULL);
	   hwndButton = CreateWindow(TEXT("button"), TEXT("WykresZ"),
		WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
		800, 840, 100, 30, hWnd, (HMENU)ID_BUTTON3, GetModuleHandle(NULL), NULL);

   if (!hWnd)
   {
      return FALSE;
   }
SetTimer( hWnd, ID_TIMER, 200, NULL );

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_RBUTTON1:{
		if(Amplituda>1)Amplituda -=1;
		break;
					 }
	case ID_RBUTTON2:{
		if(Amplituda < 3)Amplituda +=1;
		break;
					 }
	case ID_RBUTTON3:{
		if(Czas<3)Czas+=1;
		break;
		}
	case ID_RBUTTON6:{
		if(Czas>1)Czas-=1;
		break;
		}
	case ID_RBUTTON4:{
		przesuw-=Czas*25;
		break;
		}
		case ID_RBUTTON5:{
		if(przesuw<10)przesuw+=Czas*25;
		break;
		}
	case ID_BUTTON1:
		WykresX=!WykresX;
		break;
	case ID_BUTTON2:
		WykresY=!WykresY;
		break;
	case ID_BUTTON3:
		WykresZ=!WykresZ;
		break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
		case WM_LBUTTONDOWN:
		{
			int iPosX = LOWORD(lParam);
			int iPosY = HIWORD(lParam);
			wchar_t waCoord[20];
			wsprintf(waCoord, _T(" (%i, %i) "), iPosX, iPosY);
			::MessageBox(hWnd, waCoord, _T("LMB"), MB_OK);
			break;
		}
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		MyOnPaint(hdc,hWnd);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_TIMER:{
		InvalidateRect(hWnd, &Odswiez, FALSE);
		break;
		}
	
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
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
