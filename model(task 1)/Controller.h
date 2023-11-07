#pragma once

#include <gdiplus.h>
#include <vector>
#include "WaveModel.h"

using namespace std;
using namespace Gdiplus;



class Controller {	
private:	
	ULONG_PTR token;
	WaveModel* mod;
	
	
	//������ �� ���������� ��� �������� ��������
	vector<PointF> phasePoints;	

	//��������� ������
	int
		n = 0, // ���������� ��
		pCount = 100;		//���������� ����� �� ������ �������

	double
		k = 1, //����������� ��� ������������� ������� �����������
		stepE = 0.01,  //��� �� �������
		R = 1,		//����������
		eps = 0.001,	//�������� ������ ����������� �������
		V = 0.1,		//��������� ������
		b = 0.1,		//��������� ������
		dz = 0;			//���������� ��� �� 0

	double maxE = 0;
	double maxPhase = 0;
	double maxF = 0;
	double maxU = 0;

	int curMod = 0;
	bool Done = false;


	
	int N;

	double MaxF;


	//������� ������
	void ClearList();
	//��������� ������
	void FillList();

	//������� �����������/������������ �������� �������
	void FindMaxF();
	
	
public:
	int drawIdF = 0;
	CListBox* listModels;
	int drawId = 0;
	double maxf0 = 0, maxdf0 = 0;
	double xst = 0, yst = 0, scalegr = 1;
	double xstTr = 0, ystTr = 0, scalegrTr = 1;
	
	void UpdateModel(
		int n,			//���������� ����� �� ��� X
		double dt,		//��� �� �������
		double a,		//����� �������� ���
		double b,		//������ ������� ���
		double R,		//������� ���
		double f0,		//��������� ������
		double U0,		//������ ���
		double gamma,	//��������� ���
		double asr		//������� ����������
	);
		
	//������� ������
	void Clear();
	
	//��������� ��������� �������� �������6
	void DrawMainGr(LPDRAWITEMSTRUCT Item1);

	//��������� ��������� �������� �������
	void DrawPhase(LPDRAWITEMSTRUCT Item1);

	//��������� ��������� �������� �������
	void DrawPhaseTr(LPDRAWITEMSTRUCT Item1);
	
	//���������� ����������
	void StartSolve();	

	Controller():mod(new WaveModel) {
		GdiplusStartupInput si;
		GdiplusStartup(&token, &si, NULL);
	}

	//����������
	~Controller() {
		GdiplusShutdown(token);
	}	

	//��������� ������� ����� � �����
	void ShowItemList();

};