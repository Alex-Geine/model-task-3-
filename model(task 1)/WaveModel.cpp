#include "pch.h"
#include "WaveModel.h"

//нахождение волнового пакета в следующий момент времени
void WaveModel::FindWave() {
	InitData();

	for (int i = 1; i < IdMax; i++) {
		FindABCD();
		ForwardMethod();
		BackwardMethod();
		Id++;
		cout << Id << endl;
	}

}

//начальная инициализация алгоритма
void WaveModel::InitData() {

	//создание массивов
	X = new double[N];
	FReal = new double* [IdMax];
	FImagin = new double* [IdMax];
	FFurReal = new double* [N];
	FFurImagin = new double* [N];

	AReal = new double[N - 1];
	AImagin = new double[N - 1];
	BReal = new double[N - 1];
	BImagin = new double[N - 1];
	CReal = new double[N - 1];
	CImagin = new double[N - 1];
	DReal = new double[N - 1];
	DImagin = new double[N - 1];

	alphaReal = new double[N - 1];
	bettaReal = new double[N - 1];
	alphaImagin = new double[N - 1];
	bettaImagin = new double[N - 1];


	//инициализация простраственной стеки
	stepX = 2 * R / N;
	double x0 = -R;

	for (int i = 0; i < N; i++) 
		X[i] = x0 + i * stepX;

	alphaReal[0] = 0;
	bettaReal[0] = 0;
	alphaImagin[0] = 0;
	bettaImagin[0] = 0;
	

	//отсчеты пакета
	for (int i = 0; i < IdMax; i++) {
		FReal[i] = new double[N];
		FImagin[i] = new double[N];
		for (int j = 0; j < N; j++) {
			//волновой пакет в начальный момент времени
			if (i == 0) {
				FReal[i][j] = F0(X[j]);
				FImagin[i][j] = 0;
			}

			//во все остальные моменты времени
			else {
				FReal[i][j] = 0;
				FImagin[i][j] = 0;
			}
		}
	}
}

//начальный волновыой пакет
double WaveModel::F0(double x) {
	if ((x == -R) || (x == R))
		return 0;
	else
		return (f0 * exp(-(x - aSr) * (x - aSr) / (4 * gamma * gamma)));
}

//функция потенциальной энергии
double WaveModel::U(double x) {
	if ((x <= b) && (x >= a))
		return 0;
	else
		return U0;
}

//нахождение коэффициентов ABCD
void WaveModel::FindABCD() {
	for (int i = 1; i < N; i++) {
		AReal[i - 1] = 0;
		AImagin[i - 1] = -0.5 * dt / ((X[i + 1] - X[i - 1]) * (X[i] - X[i - 1]));

		BReal[i - 1] = 0;
		BImagin[i - 1] = -0.5 * dt / ((X[i + 1] - X[i - 1]) * (X[i + 1] - X[i]));

		CReal[i - 1] = 2;
		CImagin[i - 1] = dt * U(X[i]) - BImagin[i - 1] - AImagin[i - 1];

		DReal[i - 1] =
			4 * FReal[Id - 1][i] -
			(AReal[i - 1] * FReal[Id - 1][i - 1] - AImagin[i - 1] * FImagin[Id - 1][i - 1]) -
			(CReal[i - 1] * FReal[Id - 1][i] - CImagin[i - 1] * FImagin[Id - 1][i]) -
			(BReal[i - 1] * FReal[Id - 1][i + 1] - BImagin[i - 1] * FImagin[Id - 1][i + 1]);
		DImagin[i - 1] =
			4 * FImagin[Id - 1][i] -
			(AImagin[i - 1] * FReal[Id - 1][i - 1] - AReal[i - 1] * FImagin[Id - 1][i - 1]) -
			(CImagin[i - 1] * FReal[Id - 1][i] - CReal[i - 1] * FImagin[Id - 1][i]) -
			(BImagin[i - 1] * FReal[Id - 1][i + 1] - BReal[i - 1] * FImagin[Id - 1][i + 1]);
	}
}

//прямой ход прогонки
void WaveModel::ForwardMethod() {

	for (int i = 1; i < N - 1; i++) {
		//buff
		double
			denominatorReal = CReal[i - 1] + AReal[i - 1] * alphaReal[i - 1] - AImagin[i - 1] * alphaImagin[i - 1],
			denominatorImagin = CImagin[i - 1] + AImagin[i - 1] * alphaReal[i - 1] + AReal[i - 1] * alphaImagin[i - 1];
		double
			numeratorAlphaReal = BReal[i - 1],
			numeratorAlphaImagin = BImagin[i - 1],
			numeratorBettaReal = DReal[i - 1] - (AReal[i - 1] * bettaReal[i - 1] - AImagin[i - 1] * bettaImagin[i - 1]),
			numeratorBettaImagin = DImagin[i - 1] - (AImagin[i - 1] * bettaReal[i - 1] + AReal[i - 1] * bettaImagin[i - 1]);

		alphaReal[i] = -(
			(numeratorAlphaReal * denominatorReal + numeratorAlphaImagin * denominatorImagin) /
			(denominatorReal * denominatorReal + denominatorImagin * denominatorImagin));
		alphaImagin[i] = -(
			(denominatorReal * numeratorAlphaImagin - numeratorAlphaReal * denominatorImagin) /
			(denominatorReal * denominatorReal + denominatorImagin * denominatorImagin));

		bettaReal[i] = (
			(numeratorBettaReal * denominatorReal + numeratorBettaImagin * denominatorImagin) /
			(denominatorReal * denominatorReal + denominatorImagin * denominatorImagin));
		bettaImagin[i] = (
			(denominatorReal * numeratorBettaImagin - numeratorBettaReal * denominatorImagin) /
			(denominatorReal * denominatorReal + denominatorImagin * denominatorImagin));

	}
}

//обратный ход прогонки
void WaveModel::BackwardMethod() {
	for (int i = N - 2; i > 0; i--) {
		FReal[Id][i] = bettaReal[i] + alphaReal[i] * FReal[Id][i + 1] - alphaImagin[i] * FImagin[Id][i + 1];
		FImagin[Id][i] = bettaImagin[i] + alphaImagin[i] * FReal[Id][i + 1] + alphaReal[i] * FImagin[Id][i + 1];
	}
}

//апдейтит параметры модели
void WaveModel::Update(int N, double dt, double R, double a, double b, double U0, double f0, double asr, double gamma) {
	this->N = N;
	this->dt = dt;
	this->R = R;
	this->a = a;
	this->b = b;
	this->U0 = U0;
	this->f0 = f0;
	this->aSr = asr;
	this->gamma = gamma;
}

//Быстрое Фурье
bool  WaveModel::FFT(int Ft_Flag, double* Rdat, double* Idat)
{
	int LogN = log2(IdMax);
	int Nn = IdMax;

	// parameters error check:
	if ((Rdat == NULL) || (Idat == NULL))                  return false;
	if ((Nn > 16384) || (Nn < 1))                            return false;
	if (!NUMBER_IS_2_POW_K(Nn))                             return false;
	if ((LogN < 2) || (LogN > 14))                         return false;
	if ((Ft_Flag != FT_DIRECT) && (Ft_Flag != FT_INVERSE)) return false;

	register int  i, j, n, k, io, ie, in, nn;
	float         ru, iu, rtp, itp, rtq, itq, rw, iw, sr;

	static const float Rcoef[14] =
	{ -1.0000000000000000F,  0.0000000000000000F,  0.7071067811865475F,
		0.9238795325112867F,  0.9807852804032304F,  0.9951847266721969F,
		0.9987954562051724F,  0.9996988186962042F,  0.9999247018391445F,
		0.9999811752826011F,  0.9999952938095761F,  0.9999988234517018F,
		0.9999997058628822F,  0.9999999264657178F
	};
	static const float Icoef[14] =
	{ 0.0000000000000000F, -1.0000000000000000F, -0.7071067811865474F,
	   -0.3826834323650897F, -0.1950903220161282F, -0.0980171403295606F,
	   -0.0490676743274180F, -0.0245412285229122F, -0.0122715382857199F,
	   -0.0061358846491544F, -0.0030679567629659F, -0.0015339801862847F,
	   -0.0007669903187427F, -0.0003834951875714F
	};

	nn = (int)Nn >> 1;
	ie = Nn;
	for (n = 1; n <= LogN; n++)
	{
		rw = Rcoef[LogN - n];
		iw = Icoef[LogN - n];
		if (Ft_Flag == FT_INVERSE) iw = -iw;
		in = ie >> 1;
		ru = 1.0F;
		iu = 0.0F;
		for (j = 0; j < in; j++)
		{
			for (i = j; i < Nn; i += ie)
			{
				io = i + in;
				rtp = Rdat[i] + Rdat[io];
				itp = Idat[i] + Idat[io];
				rtq = Rdat[i] - Rdat[io];
				itq = Idat[i] - Idat[io];
				Rdat[io] = rtq * ru - itq * iu;
				Idat[io] = itq * ru + rtq * iu;
				Rdat[i] = rtp;
				Idat[i] = itp;
			}

			sr = ru;
			ru = ru * rw - iu * iw;
			iu = iu * rw + sr * iw;
		}

		ie >>= 1;
	}

	for (j = i = 1; i < Nn; i++)
	{
		if (i < j)
		{
			io = i - 1;
			in = j - 1;
			rtp = Rdat[in];
			itp = Idat[in];
			Rdat[in] = Rdat[io];
			Idat[in] = Idat[io];
			Rdat[io] = rtp;
			Idat[io] = itp;
		}

		k = nn;

		while (k < j)
		{
			j = j - k;
			k >>= 1;
		}

		j = j + k;
	}

	if (Ft_Flag == FT_DIRECT) return true;

	rw = 1.0F / Nn;

	for (i = 0; i < Nn; i++)
	{
		Rdat[i] *= rw;
		Idat[i] *= rw;
	}

	return true;
}

//нахождение всех спектров
void WaveModel::FindSpectrum() {
	double* realBuf, * imaginBuf;

	for (int i = 0; i < N; i++) {
		realBuf = new double[IdMax];
		imaginBuf = new double[IdMax];

		cout << "Before " << endl;
		for (int j = 0; j < IdMax; j++) {
			realBuf[j] = FReal[j][i];
			imaginBuf[j] = FImagin[j][i];
			cout << "Real: " << realBuf[j] << "Imagin: " << imaginBuf[j] << endl;
		}
		cout << "After " << endl;

		//берем фурье
		bool res = FFT(FT_DIRECT, realBuf, imaginBuf);
		if (!res)
			cout << "Фурье обосралось" << endl;

		for (int j = 0; j < IdMax; j++) {
			cout << "Real: " << realBuf[j] << "Imagin: " << imaginBuf[j] << endl;

		}

		FFurReal[i] = realBuf;
		FFurImagin[i] = imaginBuf;
	}
}

//нахождение собственных функций частицы
void WaveModel::FindFunc() {
	//находим спектр
	FindSpectrum();
}