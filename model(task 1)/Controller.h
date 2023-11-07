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
	
	
	//вектор со значениями для фазового портрета
	vector<PointF> phasePoints;	

	//паоаметры модели
	int
		n = 0, // количество СФ
		pCount = 100;		//количество точек на втором графике

	double
		k = 1, //коэффициент для потенциальной энергии осциллятора
		stepE = 0.01,  //шаг по энергии
		R = 1,		//полудлинна
		eps = 0.001,	//точность метода половинного деления
		V = 0.1,		//амплитуда купола
		b = 0.1,		//дисперсия купола
		dz = 0;			//отклонение ямы от 0

	double maxE = 0;
	double maxPhase = 0;
	double maxF = 0;
	double maxU = 0;

	int curMod = 0;
	bool Done = false;


	
	int N;

	double MaxF;


	//очищает список
	void ClearList();
	//заполняет список
	void FillList();

	//находит минимальное/максимальное значения функции
	void FindMaxF();
	
	
public:
	int drawIdF = 0;
	CListBox* listModels;
	int drawId = 0;
	double maxf0 = 0, maxdf0 = 0;
	double xst = 0, yst = 0, scalegr = 1;
	double xstTr = 0, ystTr = 0, scalegrTr = 1;
	
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

	//запускает отрисовку главного графика
	void DrawPhase(LPDRAWITEMSTRUCT Item1);

	//запускает отрисовку главного графика
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

};