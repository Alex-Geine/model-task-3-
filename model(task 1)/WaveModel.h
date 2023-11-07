#pragma once
#include <iostream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

#define Pi 3.1415926535
#define  NUMBER_IS_2_POW_K(x)   ((!((x)&((x)-1)))&&((x)>1))  // x is pow(2, k), k=1,2, ...
#define  FT_DIRECT        -1    // Direct transform.
#define  FT_INVERSE        1    // Inverse transform.


using namespace std;

class WaveModel {
private:
	
	int Id = 1;		//������� ���� �������

	double
		t = 0,	//����� ����� ������
		dt,		//��� �� �������
		stepX,	//��� �� ������������ 
		R,		//�������, ���������� ������������� � ����� ������
		a,		//����� ������� ���
		b,		//������ ������� ���
		U0,		//������ ���
		f0,		//��������� �������� ������
		aSr,	//������� �������� ��������� �������
		gamma;	//�������������������� ���������� �������, �������� ���� ���������������� �� ���������

	//KOEF
	double
		*AReal,
		*BReal,
		*CReal,
		*DReal,
		*AImagin,
		*BImagin,
		*CImagin,
		*DImagin;


	//DATA
	double		
		* alphaReal,	//�������� �������� ����� ������������� �����
		* alphaImagin,	//�������� ������ ����� ������������� �����
		* bettaReal,	//�������� �������� ����� ������������� �����
		* bettaImagin;	//�������� ������ ����� ������������� �����

	

	double** FFurReal;		//�������� ����� ������� ������� ������
	double** FFurImagin;	//������ ����� ������� ������� ������


	//��������� ������������� ���������
	void InitData();

	//��������� ��������� �����
	double F0(double x);

	//���������� ������������� ABCD
	void FindABCD();

	//������� ������������� �������
	double U(double x);

	//������ ��� ��������
	void ForwardMethod();

	//�������� ��� ��������
	void BackwardMethod();

	//������� �����
	bool  FFT(int Ft_Flag, double* Rdat, double* Idat);

	//���������� ���� ��������
	void FindSpectrum();

public:
	int N;		//���������� ����� �� ��� x
	int IdMax = 1024;	//���������� �������� �� �������

	double
		* X;	//�������� ���������������� �����

	double** FReal;	//�������� �� ���������� �������� ����� ������� ������
	double** FImagin;	//�������� �� ���������� �������� ����� ������� ������

	//���������� ��������� ������ � ��������� ������ �������
	void FindWave();

	//���������� ����������� ������� �������
	void FindFunc();

	//�������� ��������� ������
	void Update(int N, double dt, double R, double a, double b, double U0, double f0, double asr, double gamma);
};