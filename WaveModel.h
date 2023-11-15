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
	
	int Id = 1;		//текущий айди времени
	int N;		//количество точек по оси x
	int IdMax = 1024;	//количество отсчетов по времени


	double
		t = 0,	//время нашей модели
		dt,		//шаг по времени
		stepX,	//шаг по пространству 
		R,		//граница, заменяющая бесконечность в нашей модели
		a,		//левая граница ямы
		b,		//правая граница ямы
		U0,		//высота ямы
		f0,		//амплитуда гауссого купола
		aSr,	//среднее занчение положения частицы
		gamma;	//среднеквадратическое отклонение частицы, задающее меру неопределенности ее положения

	//KOEF
	complex<double>
		*A = NULL,
		*B = NULL,
		*C = NULL,
		*D = NULL;

	complex<double>		
		* alpha = NULL,	//значения коэффициентов альфа
		* betta = NULL;	//значения коэффициентов бетта

	//DATA
	double* X = NULL;					//значения пространственной сетки
	double* f = NULL;					//значения частотной сетки
	complex<double>** F = NULL;			//функция пакета
	complex<double>** FFur = NULL;		// спектр функции пакета
	vector<pair<double, int>> Energes;	//собственные энергии

	//начальная инициализация алгоритма
	void InitData();

	//начальный волновыой пакет
	double F0(double x);

	//нахождение коэффициентов ABCD
	void FindABCD();

	//функция потенциальной энергии
	double U(double x);

	//прямой ход прогонки
	void ForwardMethod();

	//обратный ход прогонки
	void BackwardMethod();

	//Быстрое Фурье
	bool  FFT(int Ft_Flag, complex<double>* data);

	//нахождение всех спектров
	void FindSpectrum();

	//нахождение производной от функции F
	complex<double> DF(int id);

	//конвертирует из complex в double
	void ConvertComplex(complex<double>* comp, double** RDat, double** IDat);

	//конвертирует из double в complex
	void ConvertDouble(complex<double>** comp, double* RDat, double* IDat);

	//находит пики в спектрограмме
	void FindPicks();

	//вторая версия поиска пиков
	void FindPicks2();

	//находит максимальное значение, начиная с некоторого id
	void FindMax(int ida, int idb, double&max, int & id);

	//находит максимальное значение, начиная с некоторого id
	bool FindMax(int ida, int idb, double& max, int& id, double min);

	//находит отрезок со следующим пиком
	void FindId(double min, int& ida, int idb);
public:
	//Отдает указатель на F()
	complex<double>** GetF();

	//Отдает указатель на FFur()
	complex<double>** GetFFur();

	//Отдает указатель на вектор с энергиями
	vector<pair<double, int>> GetEnerges();

	//сбрасывает настройки
	void Reset();

	//отдает указатель на X
	double* GetX();

	//отдает указатель на f
	double* Getf();

	//нахождение волнового пакета в следующий момент времени
	void FindWave();

	//нахождение собственных функций частицы
	void FindFunc();

	//апдейтит параметры модели
	void Update(int N, double dt, double R, double a, double b, double U0, double f0, double asr, double gamma);
};