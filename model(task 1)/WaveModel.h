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
	
	int Id = 1;		//текущий айди времени

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
		* alphaReal,	//значения реальной части кожффициентов альфа
		* alphaImagin,	//значения мнимой части кожффициентов альфа
		* bettaReal,	//значения реальной части коэффициентов бетта
		* bettaImagin;	//значения мнимой части коэффициентов бетта

	

	double** FFurReal;		//реальная часть спектра функции пакета
	double** FFurImagin;	//мнимая часть спектра функции пакета


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
	bool  FFT(int Ft_Flag, double* Rdat, double* Idat);

	//нахождение всех спектров
	void FindSpectrum();

public:
	int N;		//количество точек по оси x
	int IdMax = 1024;	//количество отсчетов по времени

	double
		* X;	//значения пространственной сетки

	double** FReal;	//значения со значениями реальной части функции пакета
	double** FImagin;	//значения со значениями реальной части функции пакета

	//нахождение волнового пакета в следующий момент времени
	void FindWave();

	//нахождение собственных функций частицы
	void FindFunc();

	//апдейтит параметры модели
	void Update(int N, double dt, double R, double a, double b, double U0, double f0, double asr, double gamma);
};