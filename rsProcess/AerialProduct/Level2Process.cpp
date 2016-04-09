
#include"Level2Process.h"
#include"..\AuxiliaryFunction.h"
#include"..\GeoPOSProcess.h"
#include"..\CoordinateTrans.h"

#include"..\gdal\include\gdal_priv.h"
#include"..\gdal\include\ogr_spatialref.h"
#pragma comment(lib,"gdal_i.lib")

//����IGM�ļ�
long Level2Process::Level2Proc_CalculateIGMFile(int nLines, int nImgWidth, int nInterval, THREEDPOINT *pGoundPt, EO *pEOElement, float fFov, float fFocalLen, DPOINT &minPt, DPOINT &maxPt, double dB, double dL, bool bInverse)
{
	long lError = 0;
	int i = 0, j = 0;
	double dAngle = 0, dPixelIFov = 0;
	double dRMatrix[9], dEMMatrix[9];
	float fx = 0, fy = 0;			//ͼ������ϵ����
	int nCorEOLine = 0;
	THREEDPOINT XYZPt;
	CoordinateTrans m_coordinateT;
	//��ͼ����ϵ����������ϵ
	dEMMatrix[0] = -sin(dL);
	dEMMatrix[1] = -sin(dB)*cos(dL);
	dEMMatrix[2] = cos(dB)*cos(dL);
	dEMMatrix[3] = cos(dL);
	dEMMatrix[4] = -sin(dB)*sin(dL);
	dEMMatrix[5] = cos(dB)*sin(dL);
	dEMMatrix[6] = 0;
	dEMMatrix[7] = cos(dB);
	dEMMatrix[8] = sin(dB);
	//��ʼ�������С��
	minPt.dX = MAX_NUM; minPt.dY = MAX_NUM;
	maxPt.dX = MIN_NUM; maxPt.dY = MIN_NUM;
	m_coordinateT.BLHToXYZ(dB, dL, 0, XYZPt);
	//m_coordinateT.XYZToBLH(XYZPt,dB,dL,dH);
	dPixelIFov = fFov*PI / (180.0f*nImgWidth);

	for (i = 0; i<nLines; i++)
	{
		printf("calculate IGM line %d\r", i + 1);
		double dXs, dYs, dZs;
		//��ͬ�׶�����ķֱ��ʲ�ͬ���������Ҳ��ͬ����Ӧ��EO���ڱ�����ϵ
		nCorEOLine = i;
		//��ȡ������ͶӰ���߷��̳������Ĳ���
		dXs = pEOElement[nCorEOLine].m_dX;
		dYs = pEOElement[nCorEOLine].m_dY;
		dZs = pEOElement[nCorEOLine].m_dZ;
		memcpy(dRMatrix, pEOElement[nCorEOLine].m_dRMatrix, sizeof(double) * 9);
		//#pragma omp parallel for
		for (j = 0; j<nImgWidth; j++)
		{
			double dPhotoPt[3], dGroundPt[3], dModelPt[3], dWGSPt[3];
			THREEDPOINT WGSPt;
			double dTempB, dTempL, dTempH;
			double x, y;
			int nZone;
			long lOffset = i*nImgWidth + j;

			//��ָ������ָ����Ϊ������
			if(!bInverse)
				fx = float(nImgWidth / 2 - j);
			else
				fx = float(j - nImgWidth / 2);

			dAngle = fx*dPixelIFov;
			//��ռ�����
			dPhotoPt[0] = fFocalLen*tan(dAngle);
			dPhotoPt[1] = 0;
			dPhotoPt[2] = -fFocalLen;
			//��ռ�����-->��ͼ����ת��			
			MatrixMuti(dRMatrix, 3, 3, 1, dPhotoPt, dModelPt);
			//�����Ĺ��߷��̽���ͼ����Ԫ��Ӧ�ĳ�ͼ����ϵ����
			pGoundPt[lOffset].dX = dXs + (pGoundPt[lOffset].dZ - dZs)*dModelPt[0] / dModelPt[2];	//���߷��̽���Xs��
			pGoundPt[lOffset].dY = dYs + (pGoundPt[lOffset].dZ - dZs)*dModelPt[1] / dModelPt[2]; 	//���߷��̽���Ys��
			dGroundPt[0] = pGoundPt[lOffset].dX;
			dGroundPt[1] = pGoundPt[lOffset].dY;
			dGroundPt[2] = pGoundPt[lOffset].dZ;
			//��ͼ����-->�������꣨WGS84��
			MatrixMuti(dEMMatrix, 3, 3, 1, dGroundPt, dWGSPt);
			WGSPt.dX = XYZPt.dX + dWGSPt[0];
			WGSPt.dY = XYZPt.dY + dWGSPt[1];
			WGSPt.dZ = XYZPt.dZ + dWGSPt[2];
			//��������ת��γ��
			m_coordinateT.XYZToBLH(WGSPt, dTempB, dTempL, dTempH);
			//��γ�ȵ�UTMͶӰ
			tsmLatLongToUTM(dTempB, dTempL, &nZone, &x, &y);
			pGoundPt[lOffset].dX = x;
			pGoundPt[lOffset].dY = y;
		}

		for (j = 0; j<nImgWidth; j++)
		{
			long lOffset = i*nImgWidth + j;
			//Ѱ�������С��
			if (minPt.dX>pGoundPt[lOffset].dX)
				minPt.dX = pGoundPt[lOffset].dX;
			if (minPt.dY>pGoundPt[lOffset].dY)
				minPt.dY = pGoundPt[lOffset].dY;
			if (maxPt.dX<pGoundPt[lOffset].dX)
				maxPt.dX = pGoundPt[lOffset].dX;
			if (maxPt.dY<pGoundPt[lOffset].dY)
				maxPt.dY = pGoundPt[lOffset].dY;
		}
	}
	return lError;
}
//����DEM�������½���IGM�ļ�
long Level2Process::Level2Proc_CalculateIGMFileDEM(int nLines, int nImgWidth, int nInterval, THREEDPOINT *pGoundPt, EO *pEOElement, float fFov, float fFocalLen, DPOINT &minPt, DPOINT &maxPt, double dB, double dL, const char* pathDEM, bool bInverse)
{
	long lError = 0;
	int i = 0, j = 0;
	double dAngle = 0, dPixelIFov = 0;
	double dRMatrix[9], dEMMatrix[9];
	float fx = 0, fy = 0;			//ͼ������ϵ����
	int nCorEOLine = 0;
	long lOffset = 0;
	double dGeoTransform[6] = { 0 };
	double *pDEMPt = NULL;
	int nDEMSamples = 0, nDEMLines = 0;
	int nDEMZone = 0;
	DPOINT originPt;
	double dGSDX = 0, dGSDY = 0;
	int nC = 0, nY = 0;			//����ȡ������
	double fDX = 0, fDY = 0;	//����ȡ��������
	double dElevation = 0;
	int projType = 0;
	DPOINT rangPnt[2];
	CoordinateTrans m_coordinateT;
	//��ͼ����ϵ����������ϵ
	dEMMatrix[0] = -sin(dL);
	dEMMatrix[1] = -sin(dB)*cos(dL);
	dEMMatrix[2] = cos(dB)*cos(dL);
	dEMMatrix[3] = cos(dL);
	dEMMatrix[4] = -sin(dB)*sin(dL);
	dEMMatrix[5] = cos(dB)*sin(dL);
	dEMMatrix[6] = 0;
	dEMMatrix[7] = cos(dB);
	dEMMatrix[8] = sin(dB);

	//��ʼ�������С��
	minPt.dX = MAX_NUM; minPt.dY = MAX_NUM;
	maxPt.dX = MIN_NUM; maxPt.dY = MIN_NUM;
	THREEDPOINT XYZPt;
	m_coordinateT.BLHToXYZ(dB, dL, 0, XYZPt);
	int tmpnZone;
	double tmpx, tmpy;
	tsmLatLongToUTM(dB * 180 / PI, dL * 180 / PI, &tmpnZone, &tmpx, &tmpy);

	//��ȡDEM�ļ�����ȡDEM�ļ��ĵ�������͸߳���Ϣ
	Level2Proc_ReadDEMFile(pathDEM, nDEMSamples, nDEMLines, rangPnt, dGeoTransform, nDEMZone, projType, pDEMPt);
	if (tmpnZone != nDEMZone)
	{
		//DEM�ļ������ͼ������ƥ��
		//�˴����ܴ���6�ȷִ������ȷִ������⣬Ŀǰ��û����
		printf("Ӱ��ͶӰ����DEMͶӰ����һ�£�������DEM��ʽ����IGM\n");
		if (pDEMPt != NULL)
			delete[]pDEMPt;
		return Level2Proc_CalculateIGMFile(nLines, nImgWidth, nInterval, pGoundPt, pEOElement, fFov, fFocalLen, minPt, maxPt, dB, dL,bInverse);
	}
	if (projType != 2)
	{
		printf("Ӱ��ͶӰ��DEMͶӰ��һ�£�������DEM��ʽ����IGM\n");
		if (pDEMPt != NULL)
			delete[]pDEMPt;
		return Level2Proc_CalculateIGMFile(nLines, nImgWidth, nInterval, pGoundPt, pEOElement, fFov, fFocalLen, minPt, maxPt, dB, dL, bInverse);
	}
	//�״μ���Ӱ��λ��
	dPixelIFov = fFov*PI / (180.0f*nImgWidth);
	originPt.dX = dGeoTransform[0];
	originPt.dY = dGeoTransform[3];
	dGSDX = dGeoTransform[1];
	dGSDY = dGeoTransform[5];

	for (i = 0; i<nLines; i++)
	{
		printf("calculate IGM line %d\r", i + 1);
		double dXs = 0, dYs = 0, dZs = 0;
		//��ͬ�׶�����ķֱ��ʲ�ͬ���������Ҳ��ͬ����Ӧ��EO���ڱ�����ϵ
		nCorEOLine = i;
		//��ȡ������ͶӰ���߷��̳������Ĳ���
		dXs = pEOElement[nCorEOLine].m_dX;
		dYs = pEOElement[nCorEOLine].m_dY;
		dZs = pEOElement[nCorEOLine].m_dZ;
		memcpy(dRMatrix, pEOElement[nCorEOLine].m_dRMatrix, sizeof(double) * 9);

		//#pragma omp parallel for
		for (j = 0; j<nImgWidth; j++)
		{
			double dPhotoPt[3], dGroundPt[3], dModelPt[3], dWGSPt[3];
			THREEDPOINT WGSPt;
			double dTempB, dTempL, dTempH;
			double x, y;
			int nZone;
			long lOffset = i*nImgWidth + j;

			//��ָ������ָ����Ϊ������
			if (!bInverse)
				fx = float(nImgWidth / 2 - j);
			else
				fx = float(j - nImgWidth / 2);

			dAngle = fx*dPixelIFov;
			//��ռ�����
			dPhotoPt[0] = fFocalLen*tan(dAngle);
			dPhotoPt[1] = 0;
			dPhotoPt[2] = -fFocalLen;
			//��ռ�����-->��ͼ����ת��			
			MatrixMuti(dRMatrix, 3, 3, 1, dPhotoPt, dModelPt);
			//�����Ĺ��߷��̽���ͼ����Ԫ��Ӧ�ĳ�ͼ����ϵ����
			pGoundPt[lOffset].dX = dXs + (pGoundPt[lOffset].dZ - dZs)*dModelPt[0] / dModelPt[2];	//���߷��̽���Xs��
			pGoundPt[lOffset].dY = dYs + (pGoundPt[lOffset].dZ - dZs)*dModelPt[1] / dModelPt[2]; 	//���߷��̽���Ys��
			dGroundPt[0] = pGoundPt[lOffset].dX;
			dGroundPt[1] = pGoundPt[lOffset].dY;
			dGroundPt[2] = pGoundPt[lOffset].dZ;
			//��ͼ����-->�������꣨WGS84��
			MatrixMuti(dEMMatrix, 3, 3, 1, dGroundPt, dWGSPt);
			WGSPt.dX = XYZPt.dX + dWGSPt[0];
			WGSPt.dY = XYZPt.dY + dWGSPt[1];
			WGSPt.dZ = XYZPt.dZ + dWGSPt[2];
			//��������ת��γ��
			m_coordinateT.XYZToBLH(WGSPt, dTempB, dTempL, dTempH);
			//��γ�ȵ�UTMͶӰ
			tsmLatLongToUTM(dTempB, dTempL, &nZone, &x, &y);
			pGoundPt[lOffset].dX = x;
			pGoundPt[lOffset].dY = y;
		}

		for (j = 0; j<nImgWidth; j++)
		{
			lOffset = i*nImgWidth + j;
			//Ѱ�������С��
			if (minPt.dX>pGoundPt[lOffset].dX)
				minPt.dX = pGoundPt[lOffset].dX;
			if (minPt.dY>pGoundPt[lOffset].dY)
				minPt.dY = pGoundPt[lOffset].dY;
			if (maxPt.dX<pGoundPt[lOffset].dX)
				maxPt.dX = pGoundPt[lOffset].dX;
			if (maxPt.dY<pGoundPt[lOffset].dY)
				maxPt.dY = pGoundPt[lOffset].dY;
		}
	}
	double tmp1 = minPt.dX - rangPnt[0].dX;
	double tmp2 = minPt.dY - rangPnt[0].dY;
	double tmp3 = maxPt.dX - rangPnt[1].dX;
	double tmp4 = maxPt.dY - rangPnt[1].dY;
	if (!(tmp1>0 && tmp2<0 && tmp3<0 && tmp4>0))
	{
		printf("DEM��ΧС��Ӱ��Χ��������DEM��ʽ����IGM\n");
		return 0;
	}
	else
	{
		for (i = 0; i<nLines; i++)
		{
			printf("adjust IGM line %d\r", i + 1);
			//printf("\rprocess line : %d/%d",nLines,i+1);
			double dXs = 0, dYs = 0, dZs = 0;
			//��ͬ�׶�����ķֱ��ʲ�ͬ���������Ҳ��ͬ����Ӧ��EO���ڱ�����ϵ
			nCorEOLine = i;
			//��ȡ������ͶӰ���߷��̳������Ĳ���
			dXs = pEOElement[nCorEOLine].m_dX;
			dYs = pEOElement[nCorEOLine].m_dY;
			dZs = pEOElement[nCorEOLine].m_dZ;
			memcpy(dRMatrix, pEOElement[nCorEOLine].m_dRMatrix, sizeof(double) * 9);
			//#pragma omp parallel for
			for (j = 0; j<nImgWidth; j++)
			{
				double dPhotoPt[3], dGroundPt[3], dModelPt[3], dWGSPt[3];
				THREEDPOINT WGSPt;
				double dTempB, dTempL, dTempH;
				double x, y;
				int nZone;
				long lOffset = i*nImgWidth + j;

				//��ָ������ָ����Ϊ������
				if (!bInverse)
					fx = float(nImgWidth / 2 - j);
				else
					fx = float(j - nImgWidth / 2);
				dAngle = fx*dPixelIFov;
				//��ռ�����
				dPhotoPt[0] = fFocalLen*tan(dAngle);
				dPhotoPt[1] = 0;
				dPhotoPt[2] = -fFocalLen;
				//��ռ�����-->��ͼ����ת��			
				MatrixMuti(dRMatrix, 3, 3, 1, dPhotoPt, dModelPt);
				//�����Ĺ��߷��̽���ͼ����Ԫ��Ӧ�ĳ�ͼ����ϵ����

				DPOINT tmpPnt;
				double xError, yError;
				int iterator_number = 0;
				//�������׼ȷ�ĵ�������
				do
				{
					tmpPnt.dX = pGoundPt[lOffset].dX;
					tmpPnt.dY = pGoundPt[lOffset].dY;

					x = pGoundPt[lOffset].dX;
					y = pGoundPt[lOffset].dY;
					fDX = (x - originPt.dX) / dGSDX;
					fDY = (y - originPt.dY) / dGSDY;
					if (fDX>nDEMSamples - 1 || fDX<0 || fDY>nDEMLines - 1 || fDY<0)
						dElevation = pGoundPt[lOffset].dZ;
					else
					{
						nC = (int)fDX;
						nY = (int)fDY;
						//����ȡ��
						fDX -= nC;
						fDY -= nY;
						//˫���Բ�ֵ
						dElevation = (1 - fDX)*(1 - fDY)*pDEMPt[nY*nDEMSamples + nC] + fDX*(1 - fDY)*pDEMPt[nY*nDEMSamples + nC + 1] + (1 - fDX)*fDY*pDEMPt[(nY + 1)*nDEMSamples + nC] + fDX*fDY*pDEMPt[nY*(nDEMSamples + 1) + nC + 1];
					}

					pGoundPt[lOffset].dZ = dElevation;
					pGoundPt[lOffset].dX = dXs + (pGoundPt[lOffset].dZ - dZs)*dModelPt[0] / dModelPt[2];	//���߷��̽���Xs��
					pGoundPt[lOffset].dY = dYs + (pGoundPt[lOffset].dZ - dZs)*dModelPt[1] / dModelPt[2]; 	//���߷��̽���Ys��
					dGroundPt[0] = pGoundPt[lOffset].dX;
					dGroundPt[1] = pGoundPt[lOffset].dY;
					dGroundPt[2] = pGoundPt[lOffset].dZ;
					//��ͼ����-->�������꣨WGS84��
					MatrixMuti(dEMMatrix, 3, 3, 1, dGroundPt, dWGSPt);
					WGSPt.dX = XYZPt.dX + dWGSPt[0];
					WGSPt.dY = XYZPt.dY + dWGSPt[1];
					WGSPt.dZ = XYZPt.dZ + dWGSPt[2];
					//��������ת��γ��
					m_coordinateT.XYZToBLH(WGSPt, dTempB, dTempL, dTempH);
					//��γ�ȵ�UTMͶӰ
					tsmLatLongToUTM(dTempB, dTempL, &nZone, &x, &y);
					pGoundPt[lOffset].dX = x;
					pGoundPt[lOffset].dY = y;

					xError = pGoundPt[lOffset].dY - tmpPnt.dY;
					yError = pGoundPt[lOffset].dX - tmpPnt.dX;
					iterator_number++;
				} while ((abs(xError)>0.01 || abs(yError)>0.01) && iterator_number<30);
			}
			for (j = 0; j<nImgWidth; j++)
			{
				lOffset = i*nImgWidth + j;
				//Ѱ�������С��
				if (minPt.dX>pGoundPt[lOffset].dX)
					minPt.dX = pGoundPt[lOffset].dX;
				if (minPt.dY>pGoundPt[lOffset].dY)
					minPt.dY = pGoundPt[lOffset].dY;
				if (maxPt.dX<pGoundPt[lOffset].dX)
					maxPt.dX = pGoundPt[lOffset].dX;
				if (maxPt.dY<pGoundPt[lOffset].dY)
					maxPt.dY = pGoundPt[lOffset].dY;
			}

		}
	}
	if (pDEMPt != NULL)
		delete[]pDEMPt;
	return 0;
}

//����IGM�ļ�
long Level2Process::Level2Proc_SaveIGMFile(const char* pIGMFile, THREEDPOINT *pGoundPnt, int nImgWidth, int nImgHeight)
{
	FILE* fIGM = NULL;
	double* imgGeoxsize = NULL;
	double* imgGeoysize = NULL;
	try {
		imgGeoxsize = new double[nImgWidth*nImgHeight];
		imgGeoysize = new double[nImgWidth*nImgHeight];
	}
	catch (bad_alloc)
	{
		printf("allocate memory error\n");
		exit(-1);
	}

	errno_t err = 0;
	err = fopen_s(&fIGM, pIGMFile, "wb");
	if (err != 0)
		exit(-1);

	//д���ݴ�С
	fwrite(&nImgWidth, sizeof(int), 1, fIGM);
	fwrite(&nImgHeight, sizeof(int), 1, fIGM);

	for (size_t i = 0; i < nImgWidth; ++i)
	{
		for (size_t j = 0; j < nImgHeight; ++j)
		{
			imgGeoxsize[j*nImgWidth + i] = pGoundPnt[j*nImgWidth + i].dX;
			imgGeoysize[j*nImgWidth + i] = pGoundPnt[j*nImgWidth + i].dY;
		}
	}

	//д�������ε�����
	fwrite(imgGeoxsize, sizeof(double), nImgWidth*nImgHeight, fIGM);
	fwrite(imgGeoysize, sizeof(double), nImgWidth*nImgHeight, fIGM);
	fflush(fIGM);
	fclose(fIGM);

	if (imgGeoxsize != NULL)
		delete[]imgGeoxsize;
	if (imgGeoysize != NULL)
		delete[]imgGeoysize;
	imgGeoxsize = NULL;
	imgGeoysize = NULL;

	return 0;
}

//��ȡIGM�ļ�
long Level2Process::Level2Proc_ReadIGMFile(const char* pIGMFile, THREEDPOINT *pGoundPnt, int &nImgWidth, int &nImgHeight)
{
	FILE* fIGM = NULL;
	double* imgGeoxsize = NULL;
	double* imgGeoysize = NULL;

	errno_t err = 0;
	err = fopen_s(&fIGM, pIGMFile, "wb");
	if (err != 0)
		exit(-1);

	//д���ݴ�С
	fread(&nImgWidth, sizeof(int), 1, fIGM);
	fread(&nImgHeight, sizeof(int), 1, fIGM);
	try {
		imgGeoxsize = new double[nImgWidth*nImgHeight];
		imgGeoysize = new double[nImgWidth*nImgHeight];
	}
	catch (bad_alloc)
	{
		printf("allocate memory error\n");
		exit(-1);
	}
	//д�������ε�����
	fwrite(imgGeoxsize, sizeof(double), nImgWidth*nImgHeight, fIGM);
	fwrite(imgGeoysize, sizeof(double), nImgWidth*nImgHeight, fIGM);

	for (size_t i = 0; i < nImgWidth; ++i)
	{
		for (size_t j = 0; j < nImgHeight; ++j)
		{
			pGoundPnt[j*nImgWidth + i].dX = imgGeoxsize[j*nImgWidth + i];
			pGoundPnt[j*nImgWidth + i].dY = imgGeoysize[j*nImgWidth + i];
		}
	}

	fflush(fIGM);
	fclose(fIGM);

	if (imgGeoxsize != NULL)
		delete[]imgGeoxsize;
	if (imgGeoysize != NULL)
		delete[]imgGeoysize;
	imgGeoxsize = NULL;
	imgGeoysize = NULL;
	return 0;
}

//��ȡDEM�ļ�
long Level2Process::Level2Proc_ReadDEMFile(const char *pDEMFile, int &nSamples, int &nLines, DPOINT rangePnt[], double dGeoTransform[], int &nZone, int &projType, double *&pDEMPt)
{
	long  lError = 0;
	int i = 0;
	GDALDataset *pDEMDS = NULL;
	OGRSpatialReference oSRS;
	char *pszWKT = NULL;
	int nBands = 0, nImgSize = 0;
	GDALDataType eDataType = GDT_Unknown;
	GDALRasterBand *pBand = NULL;
	const char *pszGeoCS = NULL;//
	const char *pszProjection = NULL;//

	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");	//����·��
	GDALAllRegister();
	pDEMDS = (GDALDataset*)GDALOpen(pDEMFile, GA_ReadOnly);
	if (!pDEMDS)
	{
		lError = -1;
		goto ErrEnd;
	}
	nSamples = pDEMDS->GetRasterXSize();
	if (nSamples <= 0)
	{
		lError = -1;
		goto ErrEnd;
	}
	nLines = pDEMDS->GetRasterYSize();
	if (nLines <= 0)
	{
		lError = -1;
		goto ErrEnd;
	}
	nBands = pDEMDS->GetRasterCount();
	if (nBands != 1)
	{
		lError = -1;
		goto ErrEnd;
	}
	nImgSize = nSamples*nLines;
	//��ȡͶӰ��Ϣ

	pszWKT = const_cast<char*>(pDEMDS->GetProjectionRef());
	if (!pszWKT)
	{
		lError = -1;
		goto ErrEnd;
	}

	projType = 0;
	oSRS.importFromWkt(&pszWKT);
	pszGeoCS = oSRS.GetAttrValue("GEOGCS");
	if (pszGeoCS == NULL)
		projType = 1;
	else if (!strcmp(pszGeoCS, "WGS 84"))
		projType = 1;
	pszProjection = oSRS.GetAttrValue("PROJECTION");
	if (!strcmp(pszProjection, "Transverse_Mercator") && projType == 1)
		projType = 2;
	nZone = oSRS.GetUTMZone();
	if (nZone<0 || nZone>60)
	{
		lError = -1;
		goto ErrEnd;
	}
	pDEMDS->GetGeoTransform(dGeoTransform);
	//��ȡ���Ͻǵ�����½ǵ�����
	rangePnt[0].dX = dGeoTransform[0];
	rangePnt[0].dY = dGeoTransform[3];
	rangePnt[1].dX = dGeoTransform[0] + nSamples*dGeoTransform[1] + nLines*dGeoTransform[2];
	rangePnt[1].dY = dGeoTransform[3] + nSamples*dGeoTransform[4] + nLines*dGeoTransform[5];

	try
	{
		pDEMPt = new double[nImgSize];
	}
	catch (bad_alloc)
	{
		lError = -1;
		goto ErrEnd;
	}
	memset(pDEMPt, 0, sizeof(double)*nImgSize);
	eDataType = GDALGetRasterDataType(GDALGetRasterBand(pDEMDS, 1));
	if (eDataType == GDT_Unknown)
	{
		lError = -1;
		goto ErrEnd;
	}
	else if (eDataType == GDT_Int16)
	{
		short *pDEM = NULL;
		try
		{
			pDEM = new short[nImgSize];
		}
		catch (bad_alloc)
		{
			lError = -1;
			goto ErrEnd;
		}
		memset(pDEM, 0, sizeof(short)*nImgSize);
		pBand = pDEMDS->GetRasterBand(1);
		pBand->RasterIO(GF_Read, 0, 0, nSamples, nLines, pDEM, nSamples, nLines, GDT_Int16, 0, 0);
		for (i = 0; i<nImgSize; i++)
		{
			pDEMPt[i] = pDEM[i];
		}
		delete[]pDEM;
	}
	else if (eDataType == GDT_Int32)
	{
		int *pDEM = NULL;
		try
		{
			pDEM = new int[nImgSize];
		}
		catch (bad_alloc)
		{
			lError = -1;
			goto ErrEnd;
		}
		memset(pDEM, 0, sizeof(int)*nImgSize);
		pBand = pDEMDS->GetRasterBand(1);
		pBand->RasterIO(GF_Read, 0, 0, nSamples, nLines, pDEM, nSamples, nLines, GDT_Int32, 0, 0);
		for (i = 0; i<nImgSize; i++)
		{
			pDEMPt[i] = pDEM[i];
		}
		delete[]pDEM;
	}
	else if (eDataType == GDT_Float32)
	{
		float *pDEM = NULL;
		try
		{
			pDEM = new float[nImgSize];
		}
		catch (bad_alloc)
		{
			lError = -1;
			goto ErrEnd;
		}
		memset(pDEM, 0, sizeof(float)*nImgSize);
		pBand = pDEMDS->GetRasterBand(1);
		pBand->RasterIO(GF_Read, 0, 0, nSamples, nLines, pDEM, nSamples, nLines, GDT_Float32, 0, 0);
		for (i = 0; i<nImgSize; i++)
		{
			pDEMPt[i] = pDEM[i];
		}
		delete[]pDEM;
	}
	else if (eDataType == GDT_Float64)
	{
		pBand = pDEMDS->GetRasterBand(1);
		pBand->RasterIO(GF_Read, 0, 0, nSamples, nLines, pDEMPt, nSamples, nLines, GDT_Float64, 0, 0);
	}
	else
	{
		lError = -1;
		goto ErrEnd;
	}
ErrEnd:
	if (pDEMDS != NULL)
		GDALClose(pDEMDS);
	return lError;
}


//=========================================================================================================================================================================================================
//ȫ�׶�����2����Ʒ����
long QPDLevel2Process::Level2Proc_Product2A(const char *pFile, const char *pCFile, const char *pEOFile, float fGSDX, float fGSDY, double fElevation, int nEOOffset,
	float fFov, float fIFov, float fFocalLen, bool bIGM, const char *pIGMFile, bool bInverse /*= false*/)
{
	long lError = 0;
	int i = 0, j = 0, k = 0;
	int nEoLines = 0;						//EO�ļ�������
	int nEoActLines = 0;					//����EOƫ���У�ʵ�ʵ�EO����
	double dB = 0, dL = 0, dH = 0;				//����γ�Ⱦ��Ⱥͺ���
	int nPOSSamples = 0;
	int nSamples = 0, nLines = 0, nBands = 0;		//Ӱ�����в�����
	int nActualLines = 0;
	int nImgSize = 0, nReImgSize = 0;
	int nReSamples = 0, nReLines = 0;			//�ز��������в�����
	int nZone = 0;
	double x = 0, y = 0;
	THREEDPOINT *pGoundPt = NULL;
	DPOINT minPt, maxPt;
	GDALDataset *pCorDS = NULL, *pRegDS = NULL;
	GDALDataType eDataType = GDT_Unknown;
	unsigned short *pnImgBuffer = NULL, *pnRegBuffer = NULL;
	float *pfImgBuffer = NULL, *pfRegBuffer = NULL;
	GDALDriver *pRegDriver = NULL;
	GDALRasterBand *pCorBand = NULL, *pRegBand = NULL;
	const char *pFormat = "GTiff";
	double dGeoTransform[6] = { 0 };		//����任����

	OGRSpatialReference oSRS;			//ͶӰ
	char *pszWKT = NULL;

	QPDGeoPOSProcess m_pos_proc;
	//��ȡӰ�����
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");	//����·��
	GDALAllRegister();
	pCorDS = (GDALDataset*)GDALOpen(pFile, GA_ReadOnly);
	if (!pCorDS)
	{
		lError = -1;
		goto ErrEnd;
	}
	nSamples = pCorDS->GetRasterXSize();
	if (nSamples <= 0)
	{
		lError = -1;
		goto ErrEnd;
	}
	nLines = pCorDS->GetRasterYSize();
	//	nLines = 20;
	if (nLines <= 0)
	{
		lError = -1;
		goto ErrEnd;
	}
	nBands = pCorDS->GetRasterCount();
	if (nBands <= 0)
	{
		lError = -1;
		goto ErrEnd;
	}
	eDataType = GDALGetRasterDataType(GDALGetRasterBand(pCorDS, 1));
	if (eDataType == GDT_Unknown)
	{
		lError = -1;
		goto ErrEnd;
	}
	//��ȡEO������
	//��Բ�ͬ�׶ε�ͼ����г���ʱ��ͼ��������Ƿ��EO����ͬʱ������
	lError = m_pos_proc.GeoPOSProc_ReadEOFile(pEOFile, dB, dL, dH);
	if (lError != 0)
		goto ErrEnd;
	lError = m_pos_proc.GeoPOSProc_GetEOLines(pEOFile, nEoLines);
	if (lError != 0)
		goto ErrEnd;

	//��EO����С��Ӱ������ʱ��ȡ��С�н��м���У��
	nActualLines = nLines;
	if (nEoLines<nActualLines)
		nLines = nEoLines;
	nImgSize = nSamples*nLines;
	//����Image GeoLocation Map��IGM����Ϣ����ÿ�����ص�Ĵ������
	tsmLatLongToUTM(dB * 180 / PI, dL * 180 / PI, &nZone, &x, &y);
	try
	{
		pGoundPt = new THREEDPOINT[nImgSize];
	}
	catch (bad_alloc)
	{
		lError = -1;
		goto ErrEnd;
	}
	for (i = 0; i<nLines; i++)
	{
		for (j = 0; j<nSamples; j++)
		{
			pGoundPt[i*nSamples + j].dZ = fElevation;
		}
	}

	lError = Level2Proc_CalculateIGMFile(nLines, nSamples, nPOSSamples, pGoundPt, m_pos_proc.m_geo_EO, fFov, fFocalLen, minPt, maxPt, dB, dL,bInverse);
	if (lError != 0)
		goto ErrEnd;
	//����IGM�ļ�
	if (bIGM)
		Level2Proc_SaveIGMFile(pIGMFile, pGoundPt, nSamples, nLines);

	//����У��
	nReSamples = int((maxPt.dX - minPt.dX) / fGSDX + 0.5);
	nReLines = int((maxPt.dY - minPt.dY) / fGSDY + 0.5);
	nReImgSize = nReSamples*nReLines;
	if (eDataType == GDT_UInt16)
	{
		//2�ֽ�unsigned short��������У��
		try
		{
			pnImgBuffer = new unsigned short[nImgSize];
		}
		catch (bad_alloc )
		{
			lError = -1;
			goto ErrEnd;
		}
		try
		{
			pnRegBuffer = new unsigned short[nReImgSize];
		}
		catch (bad_alloc)
		{
			lError = -1;
			goto ErrEnd;
		}
		memset(pnImgBuffer, 0, sizeof(unsigned short)*nImgSize);
		memset(pnRegBuffer, 0, sizeof(unsigned short)*nReImgSize);
		//��ȡ�ļ���׺
		const char * pszFormat = "GTiff";
		pRegDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
		if (!pRegDriver)
		{
			lError = -1;
			goto ErrEnd;
		}
		//����BSQ��ʽ�ļ�//������ֻ����һ��������Ϊ�����ĵ�
		char **papszOptions = NULL;
		papszOptions = CSLSetNameValue(papszOptions, "INTERLEAVE", "BAND");
		pRegDS = pRegDriver->Create(pCFile, nReSamples, nReLines, 1, GDT_UInt16, papszOptions);
		if (!pRegDS)
		{
			lError = -1;
			goto ErrEnd;
		}
		//����У��
		for (i = 1; i <= 1; i++)
		{
			printf("\rprocess band %d", i);
			pCorBand = pCorDS->GetRasterBand(128);
			pCorBand->RasterIO(GF_Read, 0, 0, nSamples, nLines, pnImgBuffer, nSamples, nLines, GDT_UInt16, 0, 0);
			GetImgSample(pnImgBuffer, minPt, maxPt, pGoundPt, fGSDX, fGSDY, nSamples, nLines, nReSamples, nReLines, pnRegBuffer);
			pRegBand = pRegDS->GetRasterBand(i);
			pRegBand->RasterIO(GF_Write, 0, 0, nReSamples, nReLines, pnRegBuffer, nReSamples, nReLines, GDT_UInt16, 0, 0);
		}
	}
	else if (eDataType == GDT_Float32)
	{
		//4�ֽ�float��������У��
		try
		{
			pfImgBuffer = new float[nImgSize];
		}
		catch (bad_alloc)
		{
			lError = -1;
			goto ErrEnd;
		}
		try
		{
			pfRegBuffer = new float[nReImgSize];
		}
		catch (bad_alloc)
		{
			lError = -1;
			goto ErrEnd;
		}
		memset(pfImgBuffer, 0, sizeof(float)*nImgSize);
		memset(pfRegBuffer, 0, sizeof(float)*nReImgSize);
		//��ȡ�ļ���׺
		const char * pszFormat = "GTiff";
		pRegDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
		if (!pRegDriver)
		{
			lError = -1;
			goto ErrEnd;
		}
		char **papszOptions = NULL;
		papszOptions = CSLSetNameValue(papszOptions, "INTERLEAVE", "BAND");
		pRegDS = pRegDriver->Create(pCFile, nReSamples, nReLines, nBands, GDT_Float32, papszOptions);
		if (!pRegDS)
		{
			lError = -1;
			goto ErrEnd;
		}
		//����У��
		for (i = 1; i <= nBands; i++)
		{
			printf("\rprocess band %d", i);
			pCorBand = pCorDS->GetRasterBand(i);
			pCorBand->RasterIO(GF_Read, 0, 0, nSamples, nLines, pfImgBuffer, nSamples, nLines, GDT_Float32, 0, 0);
			GetImgSample(pfImgBuffer, minPt, maxPt, pGoundPt, fGSDX, fGSDY, nSamples, nLines, nReSamples, nReLines, pfRegBuffer);
			pRegBand = pRegDS->GetRasterBand(i);
			pRegBand->RasterIO(GF_Write, 0, 0, nReSamples, nReLines, pfRegBuffer, nReSamples, nReLines, GDT_Float32, 0, 0);
		}
	}
	printf("\n");

	//���÷���任����
	dGeoTransform[0] = minPt.dX;
	dGeoTransform[1] = fGSDX;
	dGeoTransform[2] = 0;
	dGeoTransform[3] = maxPt.dY;
	dGeoTransform[4] = 0;
	dGeoTransform[5] = fGSDY;
	//���õ�ͼͶӰ��Ϣ
	oSRS.SetProjCS("UTM (WGS84) in northern hemisphere");
	oSRS.SetWellKnownGeogCS("WGS84");
	oSRS.SetUTM(nZone, TRUE);
	oSRS.exportToWkt(&pszWKT);
	//д��ͶӰ��Ϣ�ͷ���任����
	pRegDS->SetProjection(pszWKT);
	pRegDS->SetGeoTransform(dGeoTransform);
ErrEnd:
	if (!pnImgBuffer)
	{
		delete[]pnImgBuffer;
		pnImgBuffer = NULL;
	}
	if (!pnRegBuffer)
	{
		delete[]pnRegBuffer;
		pnRegBuffer = NULL;
	}
	if (!pfImgBuffer)
	{
		delete[]pfImgBuffer;
		pfImgBuffer = NULL;
	}
	if (!pfRegBuffer)
	{
		delete[]pfRegBuffer;
		pfRegBuffer = NULL;
	}
	if (pCorDS!=NULL)
	{
		GDALClose(pCorDS);
	}
	if (pRegDS != NULL)
	{
		GDALClose(pRegDS);
	}
	return lError;
}

long QPDLevel2Process::Level2Proc_Product2B(const char *pFile, const char *pCFile, const char *pEOFile, float fGSDX, float fGSDY, double fElevation, int nEOOffset,
	float fFov, float fIFov, float fFocalLen, const char* pDEMFile, bool bIGM, const char *pIGMFile, bool bInverse /*= false*/)
{
	long lError = 0;
	int i = 0, j = 0, k = 0;
	int nEoLines = 0;						//EO�ļ�������
	int nEoActLines = 0;					//����EOƫ���У�ʵ�ʵ�EO����
	double dB = 0, dL = 0, dH = 0;				//����γ�Ⱦ��Ⱥͺ���
	int nPOSSamples = 0;
	int nSamples = 0, nLines = 0, nBands = 0;		//Ӱ�����в�����
	int nActualLines = 0;
	int nImgSize = 0, nReImgSize = 0;
	int nReSamples = 0, nReLines = 0;			//�ز��������в�����
	int nZone = 0;
	double x = 0, y = 0;
	THREEDPOINT *pGoundPt = NULL;
	DPOINT minPt, maxPt;
	GDALDataset *pCorDS = NULL, *pRegDS = NULL;
	GDALDataType eDataType = GDT_Unknown;
	unsigned short *pnImgBuffer = NULL, *pnRegBuffer = NULL;
	float *pfImgBuffer = NULL, *pfRegBuffer = NULL;
	GDALDriver *pRegDriver = NULL;
	GDALRasterBand *pCorBand = NULL, *pRegBand = NULL;
	const char *pFormat = "GTiff";
	double dGeoTransform[6] = { 0 };		//����任����

	OGRSpatialReference oSRS;			//ͶӰ
	char *pszWKT = NULL;

	QPDGeoPOSProcess m_pos_proc;
	//��ȡӰ�����
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");	//����·��
	GDALAllRegister();
	pCorDS = (GDALDataset*)GDALOpen(pFile, GA_ReadOnly);
	if (!pCorDS)
	{
		lError = -1;
		goto ErrEnd;
	}
	nSamples = pCorDS->GetRasterXSize();
	if (nSamples <= 0)
	{
		lError = -1;
		goto ErrEnd;
	}
	nLines = pCorDS->GetRasterYSize();
	//	nLines = 20;
	if (nLines <= 0)
	{
		lError = -1;
		goto ErrEnd;
	}
	nBands = pCorDS->GetRasterCount();
	if (nBands <= 0)
	{
		lError = -1;
		goto ErrEnd;
	}
	eDataType = GDALGetRasterDataType(GDALGetRasterBand(pCorDS, 1));
	if (eDataType == GDT_Unknown)
	{
		lError = -1;
		goto ErrEnd;
	}
	//��ȡEO������
	//��Բ�ͬ�׶ε�ͼ����г���ʱ��ͼ��������Ƿ��EO����ͬʱ������
	lError = m_pos_proc.GeoPOSProc_ReadEOFile(pEOFile, dB, dL, dH);
	if (lError != 0)
		goto ErrEnd;
	lError = m_pos_proc.GeoPOSProc_GetEOLines(pEOFile, nEoLines);
	if (lError != 0)
		goto ErrEnd;

	//��EO����С��Ӱ������ʱ��ȡ��С�н��м���У��
	nActualLines = nLines;
	if (nEoLines<nActualLines)
		nLines = nEoLines;
	nImgSize = nSamples*nLines;
	//����Image GeoLocation Map��IGM����Ϣ����ÿ�����ص�Ĵ������
	tsmLatLongToUTM(dB * 180 / PI, dL * 180 / PI, &nZone, &x, &y);
	try
	{
		pGoundPt = new THREEDPOINT[nImgSize];
	}
	catch (bad_alloc)
	{
		lError = -1;
		goto ErrEnd;
	}
	for (i = 0; i<nLines; i++)
	{
		for (j = 0; j<nSamples; j++)
		{
			pGoundPt[i*nSamples + j].dZ = fElevation;
		}
	}
	lError = Level2Proc_CalculateIGMFileDEM(nLines, nSamples, nPOSSamples, pGoundPt, m_pos_proc.m_geo_EO, fFov, fFocalLen, minPt, maxPt, dB, dL, pDEMFile,bInverse);
	if (lError != 0)
		goto ErrEnd;
	//����IGM�ļ�
	if (bIGM)
		Level2Proc_SaveIGMFile(pIGMFile, pGoundPt, nSamples, nLines);
	//����У��
	nReSamples = int((maxPt.dX - minPt.dX) / fGSDX + 0.5);
	nReLines = int((maxPt.dY - minPt.dY) / fGSDY + 0.5);
	nReImgSize = nReSamples*nReLines;
	if (eDataType == GDT_UInt16)
	{
		//2�ֽ�unsigned short��������У��
		try
		{
			pnImgBuffer = new unsigned short[nImgSize];
		}
		catch (bad_alloc )
		{
			lError = -1;
			goto ErrEnd;
		}
		try
		{
			pnRegBuffer = new unsigned short[nReImgSize];
		}
		catch (bad_alloc )
		{
			lError = -1;
			goto ErrEnd;
		}
		memset(pnImgBuffer, 0, sizeof(unsigned short)*nImgSize);
		memset(pnRegBuffer, 0, sizeof(unsigned short)*nReImgSize);
		//��ȡ�ļ���׺
		const char * pszFormat = "GTiff";
		pRegDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
		if (!pRegDriver)
		{
			lError = -1;
			goto ErrEnd;
		}
		char **papszOptions = NULL;
		papszOptions = CSLSetNameValue(papszOptions, "INTERLEAVE", "BAND");
		pRegDS = pRegDriver->Create(pCFile, nReSamples, nReLines, nBands, GDT_UInt16, papszOptions);
		if (!pRegDS)
		{
			lError = -1;
			goto ErrEnd;
		}
		//����У��
		for (i = 1; i <= nBands; i++)
		{
			printf("\rprocess band %d", i);
			pCorBand = pCorDS->GetRasterBand(i);
			pCorBand->RasterIO(GF_Read, 0, 0, nSamples, nLines, pnImgBuffer, nSamples, nLines, GDT_UInt16, 0, 0);
			GetImgSample(pnImgBuffer, minPt, maxPt, pGoundPt, fGSDX, fGSDY, nSamples, nLines, nReSamples, nReLines, pnRegBuffer);
			pRegBand = pRegDS->GetRasterBand(i);
			pRegBand->RasterIO(GF_Write, 0, 0, nReSamples, nReLines, pnRegBuffer, nReSamples, nReLines, GDT_UInt16, 0, 0);
		}
	}
	else if (eDataType == GDT_Float32)
	{
		//4�ֽ�float��������У��
		try
		{
			pfImgBuffer = new float[nImgSize];
		}
		catch (bad_alloc )
		{
			lError = -1;
			goto ErrEnd;
		}
		try
		{
			pfRegBuffer = new float[nReImgSize];
		}
		catch (bad_alloc )
		{
			lError = -1;
			goto ErrEnd;
		}
		memset(pfImgBuffer, 0, sizeof(float)*nImgSize);
		memset(pfRegBuffer, 0, sizeof(float)*nReImgSize);
		//��ȡ�ļ���׺
		const char * pszFormat = "GTiff";
		pRegDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
		if (!pRegDriver)
		{
			lError = -1;
			goto ErrEnd;
		}
		char **papszOptions = NULL;
		papszOptions = CSLSetNameValue(papszOptions, "INTERLEAVE", "BAND");
		pRegDS = pRegDriver->Create(pCFile, nReSamples, nReLines, nBands, GDT_Float32, papszOptions);
		if (!pRegDS)
		{
			lError = -1;
			goto ErrEnd;
		}
		//����У��
		for (i = 1; i <= nBands; i++)
		{
			pCorBand = pCorDS->GetRasterBand(i);
			pCorBand->RasterIO(GF_Read, 0, 0, nSamples, nLines, pfImgBuffer, nSamples, nLines, GDT_Float32, 0, 0);
			GetImgSample(pfImgBuffer, minPt, maxPt, pGoundPt, fGSDX, fGSDY, nSamples, nLines, nReSamples, nReLines, pfRegBuffer);
			pRegBand = pRegDS->GetRasterBand(i);
			pRegBand->RasterIO(GF_Write, 0, 0, nReSamples, nReLines, pfRegBuffer, nReSamples, nReLines, GDT_Float32, 0, 0);
		}
	}

	//���÷���任����
	dGeoTransform[0] = minPt.dX;
	dGeoTransform[1] = fGSDX;
	dGeoTransform[2] = 0;
	dGeoTransform[3] = maxPt.dY;
	dGeoTransform[4] = 0;
	dGeoTransform[5] = fGSDY;
	//���õ�ͼͶӰ��Ϣ
	oSRS.SetProjCS("UTM (WGS84) in northern hemisphere");
	oSRS.SetWellKnownGeogCS("WGS84");
	oSRS.SetUTM(nZone, TRUE);
	oSRS.exportToWkt(&pszWKT);
	//д��ͶӰ��Ϣ�ͷ���任����
	pRegDS->SetProjection(pszWKT);
	pRegDS->SetGeoTransform(dGeoTransform);
ErrEnd:
	if (!pnImgBuffer)
	{
		delete[]pnImgBuffer;
		pnImgBuffer = NULL;
	}
	if (!pnRegBuffer)
	{
		delete[]pnRegBuffer;
		pnRegBuffer = NULL;
	}
	if (!pfImgBuffer)
	{
		delete[]pfImgBuffer;
		pfImgBuffer = NULL;
	}
	if (!pfRegBuffer)
	{
		delete[]pfRegBuffer;
		pfRegBuffer = NULL;
	}
	if (!pCorDS)
	{
		GDALClose(pCorDS);
	}
	if (!pRegDS)
	{
		GDALClose(pRegDS);
	}
	return lError;
}