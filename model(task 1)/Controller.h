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

	double	R;		//ширина ямы
	double dt;		//шаг по времени

	int N;				//количество точек по X
	int IdMax = 1024;	//количество отсчетов времени

	double MaxF;		//максимальное значение на графике пакета
	double MaxFFur;		//максимальное значение на графике спектра
	double MaxE;		//максимальное значение на графике собственной функции

	complex<double>** F = NULL;				//массив значений волнового пакета
	complex<double>** FFur = NULL;			//массив значений спектра
	vector <pair<double, int>> Energes;		//вектор со значениями собственных значений

	double* X = NULL;		//вектор значений по X
	double* f = NULL;		//вектор значений по f


	//очищает список
	void ClearList();

	//заполняет список
	void FillList();

	//находит максимальное значения функции
	void FindMaxF();

	//находит максимальное значения функции фурье
	void FindMaxFFur();

	//находит максимальное значение собственной функции
	void FindMaxEn();	
	
public:
	CListBox* listEnerges;		//указатель на листбокс

	int drawId = 0;		//ид отсчета времени, в который рисуем пакет
	int drawIdF = 0;	//ид спектра, который рисуем
	int drawIdE = 0;	//ид собственной функции, которую рисуем
		
	void UpdateModel(
		int n,			//количество точек по оси X
		double dt,		//шаг по времени
		double a,		//левая графница ямы
		double b,		//правая граница ямы
		double R,		//граница ямы
		double f0,		//амплитуда купола
		double U0,		//высота ямы
		double gamma,	//дисперсия ямы
		double asr		//среднее отклонение
	);
		
	//очищает данные
	void Clear();
	
	//запускает отрисовку главного графика6
	void DrawMainGr(LPDRAWITEMSTRUCT Item1);

	//запускает отрисовку 
	void DrawPhase(LPDRAWITEMSTRUCT Item1);

	//запускает отрисовку 
	void DrawPhaseTr(LPDRAWITEMSTRUCT Item1);
	
	//запусткает вычисления
	void StartSolve();	

	Controller():mod(new WaveModel) {
		GdiplusStartupInput si;
		GdiplusStartup(&token, &si, NULL);
	}

	//деструктор
	~Controller() {
		GdiplusShutdown(token);
	}	

	//показвает текущий айтем в листе
	void ShowItemList();
	
	//забирает данные из модели
	void GetData();

	//отвечает, есть ли данные для отрисовки
	bool DataReady();
};