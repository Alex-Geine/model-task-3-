#include "pch.h"
#include "WaveModel.h"

//���������� ��������� ������ � ��������� ������ �������
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

//��������� ������������� ���������
void WaveModel::InitData() {

	//�������� ��������
	X = new double[N];
	f = new double[IdMax];
	F = new complex<double>* [IdMax];	
	FFur = new complex<double>* [N];
	

	A = new complex<double>[N - 1];	
	B = new complex<double>[N - 1];	
	C = new complex<double>[N - 1];	
	D = new complex<double>[N - 1];
	

	alpha = new complex<double>[N - 1];
	betta = new complex<double>[N - 1];	


	//������������� ��������������� �����
	stepX = 2 * R / N;
	double x0 = -R;

	for (int i = 0; i < N; i++) 
		X[i] = x0 + i * stepX;

	//������������� ��������� �����
	double stepF = 1 / (dt * IdMax);

	for (int i = 0; i < IdMax; i++)
		f[i] = i * stepF;

	complex<double> buf(0., 0.);
	alpha[0] = buf;
	betta[0] = buf;	

	//������� ������
	for (int i = 0; i < IdMax; i++) {
		F[i] = new complex<double>[N];

		for (int j = 0; j < N; j++) {
			//�������� ����� � ��������� ������ �������
			if (i == 0) {
				complex<double> buf(F0(X[j]), 0.);
				F[i][j] = buf;				
			}

			//�� ��� ��������� ������� �������
			else {
				complex<double> buf(0., 0.);
				F[i][j] = buf;
			}
		}
	}
}

//��������� ��������� �����
double WaveModel::F0(double x) {
	if ((x == -R) || (x == R))
		return 0;
	else
		return (f0 * exp(-(x - aSr) * (x - aSr) / (4 * gamma * gamma)));
}

//������� ������������� �������
double WaveModel::U(double x) {
	if ((x <= b) && (x >= a))
		return 0;
	else
		return U0;
}

//���������� ����������� �� ������� F
complex<double> WaveModel::DF(int id) {
	return (2. / (X[id + 1] - X[id - 1]) * ((F[Id - 1][id + 1] - F[Id - 1][id]) / (X[id + 1] - X[id]) - (F[Id - 1][id] - F[Id - 1][id - 1]) / (X[id] - X[id - 1])));
}

//���������� ������������� ABCD
void WaveModel::FindABCD() {
	
	for (int i = 1; i < N - 1; i++) {
		A[i - 1] = complex<double>( 0., -dt / ((X[i + 1] - X[i - 1]) * (X[i] - X[i - 1])));
		B[i - 1] = complex<double>( 0., -dt / ((X[i + 1] - X[i - 1]) * (X[i + 1] - X[i])) );
		C[i - 1] = complex<double>( 1., dt * U(X[i]) / 2 + dt / (X[i + 1] - X[i - 1]) * ((1 / (X[i + 1] - X[i])) + (1 / (X[i] - X[i - 1]))) );
		D[i - 1] = F[Id - 1][i] + complex<double>(0., dt / 2.) * (DF(i) - U(X[i]) * F[Id - 1][i]);
	}
}

//������ ��� ��������
void WaveModel::ForwardMethod() {

	for (int i = 1; i < N - 1; i++) {
		alpha[i] = -1. * (B[i - 1]) / (C[i - 1] + A[i - 1] * alpha[i - 1]);
		betta[i] = (D[i - 1] - A[i - 1] * betta[i - 1]) / (C[i - 1] + A[i - 1] * alpha[i - 1]);		
	}
}

//�������� ��� ��������
void WaveModel::BackwardMethod() {
	for (int i = N - 2; i > 0; i--) 
		F[Id][i] = alpha[i] * F[Id][i + 1] + betta[i];
}

//�������� ��������� ������
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

//������� �����
bool  WaveModel::FFT(int Ft_Flag,complex<double>* data)
{
	double* Rdat = NULL, * Idat = NULL;
	ConvertComplex(data, &Rdat, &Idat);

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

	if (Ft_Flag == FT_DIRECT) {
		ConvertDouble(&data, Rdat, Idat);
		delete Rdat, delete Idat;
		return true;
	}

	rw = 1.0F / Nn;

	for (i = 0; i < Nn; i++)
	{
		Rdat[i] *= rw;
		Idat[i] *= rw;
	}

	ConvertDouble(&data, Rdat, Idat);
	delete Rdat, delete Idat;
	return true;
}

//���������� ���� ��������
void WaveModel::FindSpectrum() {
	for (int i = 0; i < N; i++) {
		FFur[i] = new complex<double>[IdMax];
		for (int j = 0; j < IdMax; j++)
			FFur[i][j] = F[IdMax - j - 1][i];

		//����� �����
		FFT(FT_DIRECT, FFur[i]);

		for (int j = 0; j < IdMax; j++)
			cout << FFur[i][j] << endl;
	}
}

//������� ���� � �������������
void WaveModel::FindPicks() {
	//������� ������������ ���
	int id;
	double max;
	FindMax(0, IdMax, max, id);
	Energes.push_back({ max, id });

	//������� ��������� ����
	double min = max * 0.25;
	int ida = id;
	int idb = IdMax;
	bool res = true;
	while (res) {
		FindId(min, ida, idb);
		res = FindMax(ida, idb, max, id, min);
		
		if (res) {
			Energes.push_back({ max, id }); 
			ida = id;
		}
	}

}

//������� ������������ ��������, ������� � ���������� id
void WaveModel::FindMax(int ida, int idb, double& max, int& id) {
	max = abs(FFur[int(N / 2)][ida]);
	for (int i = ida + 1; i < idb; i++) {
		if (max < abs(FFur[int(N / 2)][i])) {
			max = abs(FFur[int(N / 2)][i]);
			id = i;
		}
	}
}
//������� ������������ ��������, ������� � ���������� id
bool WaveModel::FindMax(int ida, int idb, double& max, int& id, double min) {
	max = abs(FFur[int(N / 2)][ida]);
	bool res = false;
	for (int i = ida + 1; i < idb; i++) {
		if ((max < abs(FFur[int(N / 2)][i])) && (abs(FFur[int(N / 2)][i]) > min)) {
			max = abs(FFur[int(N / 2)][i]);
			id = i;
			res = true;
		}
	}
	return res;
}
//������� ������� �� ��������� �����
void WaveModel::FindId(double min, int& ida, int idb) {
	for (int i = ida; i < idb; i++) {
		if (abs(FFur[int(N / 2)][i]) <= min) {
			ida = i;
			return;
		}		
	}
}

//���������� ����������� ������� �������
void WaveModel::FindFunc() {
	//������� ������
	FindSpectrum();
	FindPicks();
}

//������ ��������� �� F(id)
complex<double>** WaveModel::GetF() {
	return F;
}

//������ ��������� �� FFur(id)
complex<double>** WaveModel::GetFFur() {
	return FFur;
}

//������ ��������� �� X
double* WaveModel::GetX() {
	return X;
}

//������ ��������� �� f
double* WaveModel::Getf() {
	return f;
}

//���������� ���������
void WaveModel::Reset()
{
	if ((F != NULL)) {
		delete [] X;
		delete [] A;
		delete [] B;
		delete [] C;
		delete [] D;
		delete [] alpha;
		delete [] betta;

		for (int i = 0; i < IdMax; i++)
			delete [] F[i];
		for (int i = 0; i < N; i++)
			delete [] FFur[i];

		delete [] F;
		delete [] FFur;
		Id = 1;
		Energes.clear();
	}	
}

//������������ �� complex � double
void WaveModel::ConvertComplex(complex<double>* comp, double** RDat, double** IDat) {
	*RDat = new double[IdMax];
	*IDat = new double[IdMax];
	for (int i = 0; i < IdMax; i++) {
		(*RDat)[i] = comp[i].real();
		(*IDat)[i] = comp[i].imag();
	}
}

//������������ �� double � complex
void WaveModel::ConvertDouble(complex<double>** comp, double* RDat, double* IDat) {
	for (int i = 0; i < IdMax; i++)
		(*comp)[i] = complex<double>(RDat[i], IDat[i]);
}

//������ ������ �� ������ � ���������
vector<pair<double, int>> WaveModel::GetEnerges(){
	return Energes;
}