﻿
// model(task 1)Dlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "model(task 1).h"
#include "model(task 1)Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_PAINT()

END_MESSAGE_MAP()


// Диалоговое окно Cmodeltask1Dlg



Cmodeltask1Dlg::Cmodeltask1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MODELTASK_1_DIALOG, pParent)

	, n(100)
	, dt(0.02)	
	, R(2)
	, f0(0.01)
	, gamma(0.1)
	, asr(0)
	, idDraw()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cmodeltask1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_MAINGRAPH, MainGraph);

	DDX_Text(pDX, IDC_EDIT1, n);
	DDX_Text(pDX, IDC_EDIT2, dt);	
	DDX_Text(pDX, IDC_EDIT5, R);
	DDX_Text(pDX, IDC_EDIT7, f0);
	DDX_Text(pDX, IDC_EDIT8, gamma);
	DDX_Control(pDX, IDC_LIST2, listModels);
	DDX_Text(pDX, IDC_EDIT9, asr);
	DDX_Text(pDX, IDC_EDIT10, idDraw);
	DDX_Control(pDX, IDC_MAINGRAPH3, Functions);
	DDX_Control(pDX, IDC_MAINGRAPH2, Spectr);
}

BEGIN_MESSAGE_MAP(Cmodeltask1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()	
	ON_BN_CLICKED(IDC_BUTTON4, &Cmodeltask1Dlg::OnBnClickedButton4)	
	ON_EN_CHANGE(IDC_EDIT4, &Cmodeltask1Dlg::OnEnChangeEdit4)
	ON_BN_CLICKED(IDC_BUTTON5, &Cmodeltask1Dlg::OnBnClickedButton5)
	ON_LBN_SELCHANGE(IDC_LIST2, &Cmodeltask1Dlg::OnLbnSelchangeList2)
	ON_EN_CHANGE(IDC_EDIT10, &Cmodeltask1Dlg::OnEnChangeEdit10)
END_MESSAGE_MAP()


// Обработчики сообщений Cmodeltask1Dlg

BOOL Cmodeltask1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	control = new Controller();
	MainGraph.GetContr(control);
	MainGraph.drawerID = 1;

	CWnd* m_Parent;
	m_Parent = GetDesktopWindow();
	
	Functions.drawerID = 3;
	Functions.control = control;

	Spectr.drawerID = 2;
	Spectr.control = control;
	

	control->listEnerges = &listModels;
	
	
	
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void Cmodeltask1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void Cmodeltask1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR Cmodeltask1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Cmodeltask1Dlg::OnEnChangeEdit2()
{
}


void Cmodeltask1Dlg::OnTimer(UINT_PTR nIDEvent)
{
	if (ID < 1024) {	

		control->drawId = ID;
		MainGraph.draw = 1;
		MainGraph.Invalidate(false);

		while (PeekMessage(&msg, 0, WM_PAINT, WM_PAINT, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		ID++;
	}
	else {
		KillTimer(timer);
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CAboutDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}

//Посчитать значения
void Cmodeltask1Dlg::OnBnClickedButton4()
{
	UpdateData();
	KillTimer(timer);
	control->Clear();
	control->UpdateModel(n, dt,  R, f0, gamma, asr);
	control->StartSolve();
	control->GetData();
}

void Cmodeltask1Dlg::OnEnChangeEdit4()
{
}

//нарисовать
void Cmodeltask1Dlg::OnBnClickedButton5()
{
	if (!control->DataReady()) {
		MessageBox(L"Нет данных!", L"Ошибка!", NULL);
		return;
	}
	ID = 0;
	timer = SetTimer(1, 10, 0);
}

//собственные функции
void Cmodeltask1Dlg::OnLbnSelchangeList2()
{
	if (!control->DataReady()) {
		MessageBox(L"Нет данных!", L"Ошибка!", NULL);
		return;
	}
	control->ShowItemList();

	Functions.draw = 1;
	Functions.Invalidate(false);

	while (PeekMessage(&msg, 0, WM_PAINT, WM_PAINT, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

//спектр
void Cmodeltask1Dlg::OnEnChangeEdit10()
{
	if (!control->DataReady()) {
		MessageBox(L"Нет данных!", L"Ошибка!", NULL);
		return;
	}

	UpdateData();

	if ((idDraw < 0) || (idDraw >= n)) {
		MessageBox(L"Id вышло за предел количества точек по X", L"Ошибка!", NULL);
		return;
	}

	control->drawIdF = idDraw;
	Spectr.draw = 1;
	Spectr.Invalidate(false);


	while (PeekMessage(&msg, 0, WM_PAINT, WM_PAINT, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	control->GetSF(idDraw);
}
