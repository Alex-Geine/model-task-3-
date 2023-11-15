#pragma once
#include <iostream>
#include <complex>
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
	int N;		//���������� ����� �� ��� x
	int IdMax = 1024;	//���������� �������� �� �������


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
	complex<double>
		*A = NULL,
		*B = NULL,
		*C = NULL,
		*D = NULL;

	complex<double>		
		* alpha = NULL,	//�������� ������������� �����
		* betta = NULL;	//�������� ������������� �����

	//DATA
	double* X = NULL;					//�������� ���������������� �����
	double* f = NULL;					//�������� ��������� �����
	complex<double>** F = NULL;			//������� ������
	complex<double>** FFur = NULL;		// ������ ������� ������
	vector<pair<double, int>> Energes;	//����������� �������

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
	bool  FFT(int Ft_Flag, complex<double>* data);

	//���������� ���� ��������
	void FindSpectrum();

	//���������� ����������� �� ������� F
	complex<double> DF(int id);

	//������������ �� complex � double
	void ConvertComplex(complex<double>* comp, double** RDat, double** IDat);

	//������������ �� double � complex
	void ConvertDouble(complex<double>** comp, double* RDat, double* IDat);

	//������� ���� � �������������
	void FindPicks();

	//������ ������ ������ �����
	void FindPicks2();

	//������� ������������ ��������, ������� � ���������� id
	void FindMax(int ida, int idb, double&max, int & id);

	//������� ������������ ��������, ������� � ���������� id
	bool FindMax(int ida, int idb, double& max, int& id, double min);

	//������� ������� �� ��������� �����
	void FindId(double min, int& ida, int idb);
public:
	//������ ��������� �� F()
	complex<double>** GetF();

	//������ ��������� �� FFur()
	complex<double>** GetFFur();

	//������ ��������� �� ������ � ���������
	vector<pair<double, int>> GetEnerges();

	//���������� ���������
	void Reset();

	//������ ��������� �� X
	double* GetX();

	//������ ��������� �� f
	double* Getf();

	//���������� ��������� ������ � ��������� ������ �������
	void FindWave();

	//���������� ����������� ������� �������
	void FindFunc();

	//�������� ��������� ������
	void Update(int N, double dt, double R, double a, double b, double U0, double f0, double asr, double gamma);
};