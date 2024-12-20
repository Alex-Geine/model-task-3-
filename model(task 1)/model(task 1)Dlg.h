﻿
// model(task 1)Dlg.h: файл заголовка
//

#pragma once
#include "Drawer.h"
#include "Controller.h"
#include "Phase_D.h"
#include "Portret.h"



// Диалоговое окно Cmodeltask1Dlg
class Cmodeltask1Dlg : public CDialogEx
{
// Создание
public:
	Cmodeltask1Dlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODELTASK_1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:	
	Drawer MainGraph;
	UINT_PTR timer;
	MSG msg;
	Controller* control;

	Phase_D* phd;
	Portret* por;	

	afx_msg void OnEnChangeEdit2();	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();

	int n;			//количество точек по оси X
	double dt;		//шаг по времени
	double a;		//левая графница ямы
	double b;		//правая граница ямы
	double R;		//граница ямы
	double f0;		//амплитуда купола
	double U0;		//высота ямы
	double gamma;	//дисперсия ямы
	double asr;		//среднее отклонение
	CListBox listModels;
	afx_msg void OnEnChangeEdit4();
	
	afx_msg void OnBnClickedButton5();
	int idDraw;
};
