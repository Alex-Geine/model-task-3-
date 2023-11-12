#pragma once

#include <gdiplus.h>
#include <vector>
#include <complex>
#include "WaveModel.h"

using namespace std;
using namespace Gdiplus;



class Controller {	
private:	
	ULONG_PTR token;
	WaveModel* mod;

	double	R;		//������ ���
	double dt;		//��� �� �������

	int N;				//���������� ����� �� X
	int IdMax = 1024;	//���������� �������� �������

	double MaxF;		//������������ �������� �� ������� ������
	double MaxFFur;		//������������ �������� �� ������� �������
	double MaxE;		//������������ �������� �� ������� ����������� �������

	complex<double>** F = NULL;				//������ �������� ��������� ������
	complex<double>** FFur = NULL;			//������ �������� �������
	vector <pair<double, int>> Energes;		//������ �� ���������� ����������� ��������

	double* X = NULL;		//������ �������� �� X
	double* f = NULL;		//������ �������� �� f


	//������� ������
	void ClearList();

	//��������� ������
	void FillList();

	//������� ������������ �������� �������
	void FindMaxF();

	//������� ������������ �������� ������� �����
	void FindMaxFFur();

	//������� ������������ �������� ����������� �������
	void FindMaxEn();	
	
public:
	CListBox* listEnerges;		//��������� �� ��������

	int drawId = 0;		//�� ������� �������, � ������� ������ �����
	int drawIdF = 0;	//�� �������, ������� ������
	int drawIdE = 0;	//�� ����������� �������, ������� ������
		
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

	//��������� ��������� 
	void DrawPhase(LPDRAWITEMSTRUCT Item1);

	//��������� ��������� 
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
	
	//�������� ������ �� ������
	void GetData();

	//��������, ���� �� ������ ��� ���������
	bool DataReady();
};