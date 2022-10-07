#pragma once
#include<algorithm>
#include"fftw3.h"
#pragma comment(lib, "fftw3.lib")


class FindDelay
{

public:
	void GetCorr(double*corr, double* xin, double* yin, int xlen, int ylen ,int maxlag);

	void fdelay(double* xin, double* yin, int xlen, int ylen, int maxlag);


};

void FindDelay::GetCorr(double* corr, double* x, double* y, int xlen, int ylen, int maxlag)
{
	int M = std::max<int>(xlen, ylen);

	int Y = 2 * M  - 1;

	double Z = log10(Y) / log10(2.0);

	int FFTLEN = int(ceilf(Z));
	FFTLEN = 1 << FFTLEN;

	fftw_complex *xin, *xout, *Xout;
	fftw_plan fft, ifft;
	/*
	* 申请内存
	*/
	for (int i = 0; i < xlen && i < FFTLEN; i++)
	{
		xin[i][0] = x[i];
		xin[i][1] = 0;

	}

	fft = fftw_plan_dft_1d(FFTLEN, xin, xout, FFTW_FORWARD, FFTW_ESTIMATE);

	fftw_execute(fft);
	
	//拷贝xout到 Xout

	for (int i = 0; i < ylen && i < FFTLEN; i++)
	{
		xin[i][0] = y[i];
		xin[i][1] = 0;

	}
	fftw_execute(fft);
	/*
	* 销毁fft内存
	*/

	// Z= xout * conj(yout)

	for (int i = 0; i < FFTLEN; i++)
	{
		xin[i][0] = xout[i][0] * Xout[i][0] + xout[i][1] * Xout[i][1];  //实部
		xin[i][1] = -xout[i][0]*Xout[i][1] + xout[i][1] * Xout[i][0];    //虚部

	}

	ifft = fftw_plan_dft_1d(FFTLEN, xin, xout, FFTW_BACKWARD, FFTW_ESTIMATE);  //逆变换

	fftw_execute(fft);


	int inx = 0;
	corr[inx++] = 0;
	for (int i = FFTLEN - (maxlag - 2) - 1; i < FFTLEN; i++)
	{
		corr[inx++] = xout[i][0] / FFTLEN;
	}

	for (int i = 0; i < maxlag; i++)
	{
		corr[inx++] = xout[i][0] / FFTLEN;
	}

	corr[inx++] = 0;



}


void FindDelay::fdelay(double* xin, double* yin, int xlen, int ylen, int maxlag)
{

	int M = maxlag == -1 ? (xlen > ylen ? xlen - 1 : ylen - 1) : maxlag;
	double Cx00 = 0, Cy00 = 0;

	for (int i = 0; i < xlen; i++)
	{
		Cx00 += xin[i] * xin[i];
	}


	for (int i = 0; i < ylen; i++)
	{
		Cx00 += yin[i] * yin[i];
	}

	double* corr = new double[2 * M - 1];

	GetCorr(corr, xin, yin, xlen, ylen, maxlag);
	for (int i = 0; i < 2 * M - 1;i++)
	{
		corr[i] = corr[i] / sqrt(Cx00 * Cy00);
	}

	int imp, ime;
	double mcp = -1, mce = -1;
	for (int i = M; i < 2 * M + 1; i++)
	{
		if (mcp < corr[i])
		{
			mcp = corr[i];
			imp = i + 1 - M;
		}
	}

	for (int i = M - 1; i > 0; i--)
	{
		if (mce < corr[i])
		{
			mce = corr[i];
			ime = M-i;
		}
	}
	double mc;
	int im;
	if (ime == -1)
	{
		im = M + imp;
		mc = mcp;
	}
	else
	{
		if (mcp > mce)
		{
			im = M + imp;
			mc = mcp;
		}
		else if (mcp < mce)
		{
			im = M + 1 - ime;
			mc = mce;
		}
		else
		{
			if (mcp < mce)
			{
				im = M + imp;
				mc = mcp;
			}
			else
			{
				im = M + 1 - ime;
				mc = mce;
			}
		}

	}

	int d = M + 1 - im;
	if (mc < 1e-8)
	{
		d = 0;
		//失败
	}


}