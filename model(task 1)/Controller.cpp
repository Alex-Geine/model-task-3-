#include "pch.h"
#include "Controller.h"
#include <algorithm>
#include <string>

using namespace std;

#define _USE_MATH_DEFINES
#include  <math.h>

//показвает текущий айтем в листе
void Controller::ShowItemList() {
	//int cursel = listModels->GetCurSel();
	//if (cursel != LB_ERR) {
	//	Done = false;
	//	//получение id удаляемого элемента		
	//	int id = listModels->GetCurSel();
	//	mod->PointsFunc.clear();
	//	mod->maxF = 0;
	//	mod->FindFunction(id);
	//	maxF = mod->maxF;
	//	Done = true;
	//	MessageBoxW(NULL, L"DONE!", L"DONE!", NULL);
	//	
	//}
}

//очищает список
void Controller::ClearList(){
	listModels->ResetContent();	
}

//заполняет список
void Controller::FillList(){
	//for (int i = 0; i < mod->Roots.size(); i++) {
	//	const int size = 100;
	//	string st = to_string(mod->Roots[i].first);
	//	const char* stBuf = st.c_str();   // 1. string to const char *

	//	size_t sz;                          // save converted string's length + 1
	//	wchar_t output[size] = L"";          // return data, result is CString data

	//	mbstowcs_s(&sz, output, size, stBuf, size); // converting function

	//	CString cst = output;
	//	listModels->AddString((LPCTSTR)cst);		
	//}
}

//запусткает вычисления
void Controller::StartSolve() {
	mod->FindWave();
	mod->FindFunc();
	MessageBoxW(NULL, L"DONE!", L"DONE!", NULL);
	
}

//запускает отрисовку главного графика
void Controller::DrawMainGr(LPDRAWITEMSTRUCT Item1) {
	FindMaxF();
	double ItemWidth = Item1->rcItem.right - Item1->rcItem.left,
		ItemHeight = Item1->rcItem.bottom - Item1->rcItem.top;
	//параметры графика в мировых кооринатах
	double top = 2 * maxF * (1 + 0.2);
	double bottom = 0;
	double left = 0;
	double right = 2 * R * (1 + 0.2);
	
	double steplenY = (top - bottom) / 10,
		steplenX = (right - left) / 10;

	double xScale = ItemWidth / (right - left);
	double yScale = -ItemHeight / (top - bottom);



	Bitmap Image(ItemWidth, ItemHeight);
	Graphics gr(&Image);
	gr.Clear(Color::White);
	//сглаживание
	gr.SetSmoothingMode(SmoothingModeAntiAlias);

	//преобразование мира(переход к координатам самого графика
	Matrix matr;
	matr.Scale(xScale, (yScale + 1)); //маштабирование
	matr.Translate(right / 2, -top / 2); //перенос начала координат


	gr.SetTransform(&matr);


	Pen BackGroundPen(Color::DarkGray, 0.005);
	Pen pen(Color::BlueViolet, 0.006);

	Pen StickPen(Color::CornflowerBlue, 0.02);
	SolidBrush brush(Color::Black);
	SolidBrush PlatformBrush(Color::Crimson);
	Gdiplus::Font font((L"Times New Roman"), 14, FontStyle::FontStyleRegular, UnitPixel);


	//Разметка
	for (int i = 0; i < 10; i++)
	{
		//горизонтальная
		gr.DrawLine(&BackGroundPen, PointF(-right / 2, top / 2 - i * steplenY), PointF(right / 2, top / 2 - i * steplenY));

		//вертикальная
		gr.DrawLine(&BackGroundPen, PointF(-right / 2 + i * steplenX, top / 2), PointF(-right / 2 + i * steplenX, -top / 2));
	}

	//Большие оси
	//горизонтальная
	gr.DrawLine(&pen, PointF(-right / 2, 0), PointF(right / 2, 0));

	//вертикальная
	gr.DrawLine(&pen, PointF(0, top / 2), PointF(0, -top / 2));

	gr.ResetTransform();

	//подписи к осям
	for (int i = 0; i < 11; i++)
	{
		CString str;

		//по Y
		str.Format(L"%.2f", top / 2 - i * steplenY);
		PointF strPoint(0, top / 2 - i * steplenY);
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);

		//по X
		str.Format(L"%.2f", right / 2 - i * steplenX);
		strPoint.X = right / 2 - i * steplenX;
		strPoint.Y = 0;
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);
	}

	gr.SetTransform(&matr);

	//Отрисовка графика


	Color color(Color::IndianRed);
	Pen StickPen1(color, 0.007);

	PointF p1, p2;
	for (int i = 1; i < N; i++) {
		p1.X = mod->X[i - 1];
		p1.Y = sqrt(mod->FReal[drawId][i - 1] * mod->FReal[drawId][i - 1] + mod->FImagin[drawId][i - 1] * mod->FImagin[drawId][i - 1]);
		p2.X = mod->X[i];
		p2.Y = sqrt(mod->FReal[drawId][i] * mod->FReal[drawId][i] + mod->FImagin[drawId][i] * mod->FImagin[drawId][i]);
		gr.DrawLine(&StickPen1, p1, p2);
	}

	Graphics grnew(Item1->hDC);
	grnew.DrawImage(&Image, 0, 0);
}

//запускает отрисовку графика с собственными функциями
void Controller::DrawPhase(LPDRAWITEMSTRUCT Item1) {
	if (Done) {
		double ItemWidth = Item1->rcItem.right - Item1->rcItem.left,
			ItemHeight = Item1->rcItem.bottom - Item1->rcItem.top;
		//параметры графика в мировых кооринатах
		double top = 2 * (fabs(maxF)) * (1 + 0.2);
		double bottom = 0;
		double left = 0;
		double right = 2 * R * (1 + 0.2);

		//xPadding = (right - left) / 10;
		//yPadding = (top - bottom) / 10;

		double steplenY = (top - bottom) / 10,
			steplenX = (right - left) / 10;

		double xScale = ItemWidth / (right - left);
		double yScale = -ItemHeight / (top - bottom);



		Bitmap Image(ItemWidth, ItemHeight);
		Graphics gr(&Image);
		gr.Clear(Color::White);
		//сглаживание
		gr.SetSmoothingMode(SmoothingModeAntiAlias);

		//преобразование мира(переход к координатам самого графика
		Matrix matr;
		matr.Scale(xScale, (yScale + 1)); //маштабирование
		matr.Translate(right / 2, -top / 2); //перенос начала координат


		gr.SetTransform(&matr);


		Pen BackGroundPen(Color::DarkGray, 0.005);
		Pen pen(Color::BlueViolet, 0.006);

		Pen StickPen(Color::CornflowerBlue, 0.02);
		SolidBrush brush(Color::Black);
		SolidBrush PlatformBrush(Color::Crimson);
		Gdiplus::Font font((L"Times New Roman"), 14, FontStyle::FontStyleRegular, UnitPixel);


		//Разметка
		for (int i = 0; i < 10; i++)
		{
			//горизонтальная
			gr.DrawLine(&BackGroundPen, PointF(-right / 2, top / 2 - i * steplenY), PointF(right / 2, top / 2 - i * steplenY));

			//вертикальная
			gr.DrawLine(&BackGroundPen, PointF(-right / 2 + i * steplenX, top / 2), PointF(-right / 2 + i * steplenX, -top / 2));
		}

		//Большие оси
		//горизонтальная
		gr.DrawLine(&pen, PointF(-right / 2, 0), PointF(right / 2, 0));

		//вертикальная
		gr.DrawLine(&pen, PointF(0, top / 2), PointF(0, -top / 2));

		gr.ResetTransform();

		//подписи к осям
		for (int i = 0; i < 11; i++)
		{
			CString str;

			//по Y
			str.Format(L"%.2f", top / 2 - i * steplenY);
			PointF strPoint(0, top / 2 - i * steplenY);
			matr.TransformPoints(&strPoint);
			gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);

			//по X
			str.Format(L"%.2f", right / 2 - i * steplenX);
			strPoint.X = right / 2 - i * steplenX;
			strPoint.Y = 0;
			matr.TransformPoints(&strPoint);
			gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);
		}

		gr.SetTransform(&matr);

		//Отрисовка графика


		Color color(Color::IndianRed);
		Pen StickPen1(color, 0.01);

		/*PointF p1, p2;
		for (int i = 1; i < mod->PointsFunc.size(); i++) {
			p1.X = mod->PointsFunc[i - 1].first;
			p1.Y = mod->PointsFunc[i - 1].second;
			p2.X = mod->PointsFunc[i].first;
			p2.Y = mod->PointsFunc[i].second;
			gr.DrawLine(&StickPen1, p1, p2);
		}*/

		Graphics grnew(Item1->hDC);
		grnew.DrawImage(&Image, 0, 0);
	}
}

//запускает отрисовку графика фазовой траектории
void Controller::DrawPhaseTr(LPDRAWITEMSTRUCT Item1) {
	if (Done) {
		double ItemWidth = Item1->rcItem.right - Item1->rcItem.left,
			ItemHeight = Item1->rcItem.bottom - Item1->rcItem.top;
		//параметры графика в мировых кооринатах
		double top = 2 * (fabs(maxU)) * (1 + 0.2);
		double bottom = 0;
		double left = 0;
		double right = 2 * R * (1 + 0.2);

		//xPadding = (right - left) / 10;
		//yPadding = (top - bottom) / 10;

		double steplenY = (top - bottom) / 10,
			steplenX = (right - left) / 10;

		double xScale = ItemWidth / (right - left);
		double yScale = -ItemHeight / (top - bottom);



		Bitmap Image(ItemWidth, ItemHeight);
		Graphics gr(&Image);
		gr.Clear(Color::White);
		//сглаживание
		gr.SetSmoothingMode(SmoothingModeAntiAlias);

		//преобразование мира(переход к координатам самого графика
		Matrix matr;
		matr.Scale(xScale, (yScale + 1)); //маштабирование
		matr.Translate(right / 2, -top / 2); //перенос начала координат


		gr.SetTransform(&matr);


		Pen BackGroundPen(Color::DarkGray, 0.005);
		Pen pen(Color::BlueViolet, 0.006);

		Pen StickPen(Color::CornflowerBlue, 0.02);
		SolidBrush brush(Color::Black);
		SolidBrush PlatformBrush(Color::Crimson);
		Gdiplus::Font font((L"Times New Roman"), 14, FontStyle::FontStyleRegular, UnitPixel);


		//Разметка
		for (int i = 0; i < 10; i++)
		{
			//горизонтальная
			gr.DrawLine(&BackGroundPen, PointF(-right / 2, top / 2 - i * steplenY), PointF(right / 2, top / 2 - i * steplenY));

			//вертикальная
			gr.DrawLine(&BackGroundPen, PointF(-right / 2 + i * steplenX, top / 2), PointF(-right / 2 + i * steplenX, -top / 2));
		}

		//Большие оси
		//горизонтальная
		gr.DrawLine(&pen, PointF(-right / 2, 0), PointF(right / 2, 0));

		//вертикальная
		gr.DrawLine(&pen, PointF(0, top / 2), PointF(0, -top / 2));

		gr.ResetTransform();

		//подписи к осям
		for (int i = 0; i < 11; i++)
		{
			CString str;

			//по Y
			str.Format(L"%.2f", top / 2 - i * steplenY);
			PointF strPoint(0, top / 2 - i * steplenY);
			matr.TransformPoints(&strPoint);
			gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);

			//по X
			str.Format(L"%.2f", right / 2 - i * steplenX);
			strPoint.X = right / 2 - i * steplenX;
			strPoint.Y = 0;
			matr.TransformPoints(&strPoint);
			gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);
		}

		gr.SetTransform(&matr);

		//Отрисовка графика


		Color color(Color::IndianRed);
		Pen StickPen1(color, 0.01);
		PointF p1, p2;	
		/*for (int i = 1; i < pCount; i++) {
			p1.X = mod->PointsFunc[i-1].first;
			p2.X = mod->PointsFunc[i].first;
			p1.Y = mod->UPoints[i -1];
			p2.Y = mod->UPoints[i];
			gr.DrawLine(&StickPen1, p1, p2);
		}	*/	

		Graphics grnew(Item1->hDC);
		grnew.DrawImage(&Image, 0, 0);
	}
}

//очищает данные
void Controller::Clear() {
	ClearList();
	//mod->Clear();
	//maxf0 = 0; maxdf0 = 0; maxla = 0;
	xst = 0; yst = 0; scalegr = 1;
	xstTr = 0; ystTr = 0; scalegrTr = 1;
}



void Controller::UpdateModel(
	int n,			//количество точек по оси X
	double dt,		//шаг по времени
	double a,		//левая графница ямы
	double b,		//правая граница ямы
	double R,		//граница ямы
	double f0,		//амплитуда купола
	double U0,		//высота ямы
	double gamma,	//дисперсия ямы
	double asr		//среднее отклонение
	) 
{
	mod->Update(n, dt, R, a, b, U0, f0, asr, gamma);

	this->R = R;
	this->N = n;
}

//находит минимальное/максимальное значения функции
void Controller::FindMaxF() {
	double max = sqrt(mod->FReal[drawId][0] * mod->FReal[drawId][0] + mod->FImagin[drawId][0] * mod->FImagin[drawId][0]);

	for (int i = 1; i < mod->N; i++) {
		double buf = sqrt(mod->FReal[drawId][i] * mod->FReal[drawId][i] + mod->FImagin[drawId][i] * mod->FImagin[drawId][i]);
		if (max < buf)
			max = buf;
	}
		
	maxF = max;
}
