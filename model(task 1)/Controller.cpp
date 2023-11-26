#include "pch.h"
#include "Controller.h"
#include <algorithm>
#include <string>

using namespace std;

#define _USE_MATH_DEFINES
#include  <math.h>

//�������� ������ �� ������
void Controller::GetData() {
	F = mod->GetF();
	FFur = mod->GetFFur();
	X = mod->GetX();
	f = mod->Getf();
	Energes = mod->GetEnerges();

	FillList();
}

//��������� ������� ����� � �����
void Controller::ShowItemList() {
	int cursel = listEnerges->GetCurSel();
	if (cursel != LB_ERR) {
		CString str;
		listEnerges->GetText(cursel, str);
		double key = _wtof_l(str, 0);
		for (int i = 0; i < Energes.size(); i++) {
			if (round(Energes[i].first * 1000000) / 1000000 == key) {
				drawIdE = Energes[i].second;
				return;
			}
		}		
	}
}

//������� ������
void Controller::ClearList(){
	listEnerges->ResetContent();	
}

//��������� ������
void Controller::FillList(){
	for (int i = 0; i < Energes.size(); i++) {
		const int size = 100;
	
		string st = to_string(Energes[i].first);
		const char* stBuf = st.c_str();   // 1. string to const char *

		size_t sz;                          // save converted string's length + 1
		wchar_t output[size] = L"";          // return data, result is CString data

		mbstowcs_s(&sz, output, size, stBuf, size); // converting function

		CString cst = output;
		listEnerges->AddString((LPCTSTR)cst);		
	}
}

//���������� ����������
void Controller::StartSolve() {
	mod->FindWave();
	mod->FindFunc();	
}

//��������� ��������� �������� �������
void Controller::DrawMainGr(LPDRAWITEMSTRUCT Item1) {
	FindMaxF();
	double ItemWidth = Item1->rcItem.right - Item1->rcItem.left,
		ItemHeight = Item1->rcItem.bottom - Item1->rcItem.top;
	//��������� ������� � ������� ����������
	double top =  MaxF * (1 + 0.2);
	double bottom = 0;
	double left = 0;
	double right = 2 * R * (1 + 0.2);
	
	double steplenY = (top - bottom) / 10,
		steplenX = (right - left) / 10;

	double xScale = ItemWidth / (right - left);
	double yScale = -ItemHeight / (top + top /10 - bottom);



	Bitmap Image(ItemWidth, ItemHeight);
	Graphics gr(&Image);
	gr.Clear(Color::White);
	//�����������
	gr.SetSmoothingMode(SmoothingModeAntiAlias);

	//�������������� ����(������� � ����������� ������ �������
	Matrix matr;
	matr.Scale(xScale, (yScale + 1)); //��������������
	matr.Translate(right / 2, -top); //������� ������ ���������


	gr.SetTransform(&matr);


	Pen BackGroundPen(Color::DarkGray, 0.005);
	Pen pen(Color::Black, 0.006);
	Pen pen2(Color::Blue, 0.006);

	Pen StickPen(Color::CornflowerBlue, 0.02);
	SolidBrush brush(Color::Black);
	SolidBrush PlatformBrush(Color::Crimson);
	Gdiplus::Font font((L"Times New Roman"), 14, FontStyle::FontStyleRegular, UnitPixel);


	//��������
	for (int i = 0; i < 10; i++)
	{
		//��������������
		gr.DrawLine(&BackGroundPen, PointF(-right / 2, top  - i * steplenY), PointF(right / 2, top  - i * steplenY));

		//������������
		gr.DrawLine(&BackGroundPen, PointF(-right / 2 + i * steplenX, top), PointF(-right / 2 + i * steplenX, bottom));
	}

	//������� ���
	//��������������
	gr.DrawLine(&pen, PointF(-right / 2, 0), PointF(right / 2, 0));

	//������������
	gr.DrawLine(&pen, PointF(0, top ), PointF(0, 0));

	//������� ���
	gr.DrawLine(&pen2, PointF(R, top), PointF(R, 0));
	gr.DrawLine(&pen2, PointF(-R, top), PointF(-R, 0));

	gr.ResetTransform();

	//������� � ����
	for (int i = 0; i < 11; i++)
	{
		CString str;

		//�� Y
		str.Format(L"%.2f", top - i * steplenY);
		PointF strPoint(0, top - i * steplenY);
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);

		//�� X
		str.Format(L"%.2f", right / 2 - i * steplenX);
		strPoint.X = right / 2 - i * steplenX;
		strPoint.Y = 0;
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);
	}

	gr.SetTransform(&matr);

	//��������� �������


	Color color(Color::IndianRed);
	Pen StickPen1(color, 0.007);

	PointF p1, p2;
	for (int i = 1; i < N; i++) {
		p1.X = X[i - 1];
		p1.Y = abs(F[drawId][i - 1]);
		p2.X = X[i];
		p2.Y = abs(F[drawId][i]);
		gr.DrawLine(&StickPen1, p1, p2);
	}

	Graphics grnew(Item1->hDC);
	grnew.DrawImage(&Image, 0, 0);
}

//��������� ��������� ������� � ��������
void Controller::DrawPhase(LPDRAWITEMSTRUCT Item1) {
	FindMaxFFur();
	double ItemWidth = Item1->rcItem.right - Item1->rcItem.left,
		ItemHeight = Item1->rcItem.bottom - Item1->rcItem.top;
	//��������� ������� � ������� ����������
	double top = MaxFFur * (1 + 0.2);
	double bottom = 0;
	double left = 0;
	double right =  1 / dt;

	double steplenY = (top - bottom) / 10,
		steplenX = (right - left) / 10;

	double xScale = ItemWidth / (right + right/20 - left);
	double yScale = -ItemHeight / (top  + top / 10- bottom);



	Bitmap Image(ItemWidth, ItemHeight);
	Graphics gr(&Image);
	gr.Clear(Color::White);
	//�����������
	gr.SetSmoothingMode(SmoothingModeAntiAlias);

	//�������������� ����(������� � ����������� ������ �������
	Matrix matr;
	matr.Scale(xScale, (yScale + 1)); //��������������
	matr.Translate(right/20, -top); //������� ������ ���������


	gr.SetTransform(&matr);


	Pen BackGroundPen(Color::DarkGray, 0.005);
	Pen pen(Color::Black, 0.006);

	Pen StickPen(Color::CornflowerBlue, 0.02);
	SolidBrush brush(Color::Black);
	SolidBrush PlatformBrush(Color::Crimson);
	Gdiplus::Font font((L"Times New Roman"), 14, FontStyle::FontStyleRegular, UnitPixel);


	//��������
	for (int i = 0; i < 10; i++)
	{
		//��������������
		gr.DrawLine(&BackGroundPen, PointF(0, top  - i * steplenY), PointF(right, top - i * steplenY));

		//������������
		gr.DrawLine(&BackGroundPen, PointF(i * steplenX, top), PointF( i * steplenX, 0));
	}

	//������� ���
	//��������������
	gr.DrawLine(&pen, PointF(0, 0), PointF(right, 0));

	//������������
	gr.DrawLine(&pen, PointF(0, top), PointF(0, 0));

	gr.ResetTransform();

	//������� � ����
	for (int i = 0; i < 11; i++)
	{
		CString str;
		PointF strPoint;
		if (i != 10) {
			//�� Y
			str.Format(L"%.2f", top - i * steplenY);
			strPoint.X = -right / 25;
			strPoint.Y =  top - i * steplenY;
			matr.TransformPoints(&strPoint);
			gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);
		}		

		//�� X
		str.Format(L"%.2f", right - i * steplenX);
		strPoint.X = right - i * steplenX;
		strPoint.Y = 0;
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);
	}

	gr.SetTransform(&matr);

	//��������� �������


	Color color(Color::IndianRed);
	Pen StickPen1(color, 0.007);

	PointF p1, p2;
	for (int i = 1; i < IdMax; i++) {
		p1.X = f[i - 1];
		p1.Y = abs(FFur[drawIdF][i - 1]);
		p2.X = f[i];
		p2.Y = abs(FFur[drawIdF][i]);
		gr.DrawLine(&StickPen1, p1, p2);
	}

	Graphics grnew(Item1->hDC);
	grnew.DrawImage(&Image, 0, 0);
}

//��������� ��������� ������� ������� ����������
void Controller::DrawPhaseTr(LPDRAWITEMSTRUCT Item1) {
	FindMaxEn();
	double ItemWidth = Item1->rcItem.right - Item1->rcItem.left,
		ItemHeight = Item1->rcItem.bottom - Item1->rcItem.top;
	//��������� ������� � ������� ����������
	double top =  MaxE * (1 + 0.2);
	double bottom = 0;
	double left = 0;
	double right = 2 * R * (1 + 0.2);

	double steplenY = (top - bottom) / 10,
		steplenX = (right - left) / 10;

	double xScale = ItemWidth / (right - left);
	double yScale = -ItemHeight / (top + top / 10 - bottom);



	Bitmap Image(ItemWidth, ItemHeight);
	Graphics gr(&Image);
	gr.Clear(Color::White);
	//�����������
	gr.SetSmoothingMode(SmoothingModeAntiAlias);

	//�������������� ����(������� � ����������� ������ �������
	Matrix matr;
	matr.Scale(xScale, (yScale + 1)); //��������������
	matr.Translate(right / 2, -top); //������� ������ ���������


	gr.SetTransform(&matr);


	Pen BackGroundPen(Color::DarkGray, 0.005);
	Pen pen(Color::Black, 0.006);
	Pen pen2(Color::Blue, 0.006);

	Pen StickPen(Color::CornflowerBlue, 0.02);
	SolidBrush brush(Color::Black);
	SolidBrush PlatformBrush(Color::Crimson);
	Gdiplus::Font font((L"Times New Roman"), 14, FontStyle::FontStyleRegular, UnitPixel);


	//��������
	for (int i = 0; i < 10; i++)
	{
		//��������������
		gr.DrawLine(&BackGroundPen, PointF(-right / 2, top - i * steplenY), PointF(right / 2, top - i * steplenY));

		//������������
		gr.DrawLine(&BackGroundPen, PointF(-right / 2 + i * steplenX, top), PointF(-right / 2 + i * steplenX, bottom));
	}

	//������� ���
	//��������������
	gr.DrawLine(&pen, PointF(-right / 2, 0), PointF(right / 2, 0));

	//������������
	gr.DrawLine(&pen, PointF(0, top), PointF(0, 0));

	//������� ���
	gr.DrawLine(&pen2, PointF(R, top), PointF(R, 0));
	gr.DrawLine(&pen2, PointF(-R, top), PointF(-R, 0));

	gr.ResetTransform();

	//������� � ����
	for (int i = 0; i < 11; i++)
	{
		CString str;

		//�� Y
		str.Format(L"%.2f", top - i * steplenY);
		PointF strPoint(0, top - i * steplenY);
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);

		//�� X
		str.Format(L"%.2f", right / 2 - i * steplenX);
		strPoint.X = right / 2 - i * steplenX;
		strPoint.Y = 0;
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);
	}

	gr.SetTransform(&matr);

	//��������� �������


	Color color(Color::IndianRed);
	Pen StickPen1(color, 0.007);

	PointF p1, p2;
	for (int i = 1; i < N; i++) {
		p1.X = X[i - 1];
		p1.Y = abs(FFur[i - 1][drawIdE]);
		p2.X = X[i];
		p2.Y = abs(FFur[i][drawIdE]);
		gr.DrawLine(&StickPen1, p1, p2);
	}

	Graphics grnew(Item1->hDC);
	grnew.DrawImage(&Image, 0, 0);
}

//������� ������
void Controller::Clear() {
	X = nullptr;
	f = nullptr;
	F = nullptr;
	FFur = nullptr;
	Energes.clear();
	
	mod->Reset();
	ClearList();
}

void Controller::UpdateModel(
	int n,			//���������� ����� �� ��� X
	double dt,		//��� �� �������
	
	double R,		//������� ���
	double f0,		//��������� ������
	
	double gamma,	//��������� ���
	double asr		//������� ����������
	) 
{
	mod->Update(n, dt, R,  f0, asr, gamma);
	this->R = R;
	this->N = n;
	this->dt = dt;
}

//������� �����������/������������ �������� �������
void Controller::FindMaxF() {
	double max = abs(F[drawId][0]);

	for (int i = 1; i < N; i++) {
		double buf = abs(F[drawId][i]);
		if (max < buf)
			max = buf;
	}
		
	MaxF = max;
}

//������� ������������ �������� ������� �����
void Controller::FindMaxFFur() {
	double max = abs(FFur[drawIdF][0]);

	for (int i = 1; i < IdMax; i++) {
		double buf = abs(FFur[drawIdF][i]);
		if (max < buf)
			max = buf;
	}

	MaxFFur = max;
}
//������� ������������ �������� ����������� �������
void Controller::FindMaxEn() {
	double max = abs(FFur[0][drawIdE]);

	for (int i = 1; i < N; i++) {
		double buf = abs(FFur[i][drawIdE]);
		if (max < buf)
			max = buf;
	}

	MaxE = max;
}

//��������, ���� �� ������ ��� ���������
bool Controller::DataReady() {
	if (X)
		return true;
	else
		return false;
}

//������� ����������� ������� �� ������� ������� ����������
void Controller::GetSF(int id) {
	Energes.clear();
	ClearList();

	//���������� � ������
	mod->FindSF(id);
	Energes = mod->GetEnerges();

	FillList();
}