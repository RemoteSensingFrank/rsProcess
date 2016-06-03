#include"KMeans.h"
#include"..\AuxiliaryFunction.h"
#include"..\gdal\include\gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")

void InitKMeans(float* dataImg, int xsize, int ysize, int bands, int classNum, category* m_categories)
{
	for (int i = 0; i < classNum; ++i)
		m_categories[i] = createcategory(bands);

	//��ʼ�ĸ���n����ԪΪ������
	for (int i = 0; i<classNum; ++i)
		for (int k = 0; k<bands; ++k)
			m_categories[i].center[k] = dataImg[k*xsize*ysize + i];

	float* pixelElement = new float[bands];
	float* centerdis = new float[classNum];
	int* objnumbers = new int[classNum];
	memset(objnumbers, 0, sizeof(int)*classNum);

	//��ȡÿһ�����Ԫ��Ŀ
	int idx = 0;
	for (int i = 0; i<xsize*ysize; ++i)
	{
		for (int j = 0; j<bands; ++j)
			pixelElement[j] = dataImg[j*xsize*ysize + i];

		for (int j = 0; j<classNum; ++j)
			centerdis[j] = GetSSD(pixelElement, m_categories[j].center, bands);

		idx = 0;
		for (int j = 1; j<classNum; ++j)
		{
			if (centerdis[idx]>centerdis[j])
				idx = j;
		}
		objnumbers[idx]++;
	}

	//�ռ�����
	for (int i = 0; i < classNum; ++i)
	{
		setcategoryobjs(objnumbers[i], m_categories[i]);
	}


	//ÿһ�����������ĸ���
	memset(objnumbers, 0, sizeof(int)*classNum);
	for (int i = 0; i<xsize*ysize; ++i)
	{
		for (int j = 0; j<bands; ++j)
			pixelElement[j] = dataImg[j*xsize*ysize + i];

		for (int j = 0; j<classNum; ++j)
			centerdis[j] = GetSSD(pixelElement, m_categories[j].center, bands);

		idx = 0;
		for (int j = 1; j<classNum; ++j)
		{
			if (centerdis[idx]>centerdis[j])
				idx = j;
		}
		m_categories[idx].objindex[objnumbers[idx]] = i;
		objnumbers[idx]++;
	}

	delete[]pixelElement;
	delete[]centerdis;
	delete[]objnumbers;
}

void CenterKMeans(float* dataImg, int xsize, int ysize, int bands, int classNum, category* m_categories)
{
	for (int i = 0; i < classNum; ++i)
	{
		for (int j = 0; j <bands; ++j)
		{
			getstatisticcategory(dataImg + j*xsize*ysize, m_categories[i], j);
		}
	}
}

void IteratorKMeans(float* dataImg, int xsize, int ysize, int bands, int classNum, category* m_categories)
{
	//���¼���������¼����������
	//��ȡÿһ�����Ԫ��Ŀ
	float* pixelElement = new float[bands];
	float* centerdis = new float[classNum];
	int* objnumbers = new int[classNum];
	memset(objnumbers, 0, sizeof(int)*classNum);

	//��ȡÿһ�����Ԫ��Ŀ
	int idx = 0;
	for (int i = 0; i < xsize*ysize; ++i)
	{
		for (int j = 0; j < bands; ++j)
			pixelElement[j] = dataImg[j*xsize*ysize + i];

		for (int j = 0; j < classNum; ++j)
			centerdis[j] = GetSSD(pixelElement, m_categories[j].center, bands);

		idx = 0;
		for (int j = 1; j < classNum; ++j)
		{
			if (centerdis[idx] > centerdis[j])
				idx = j;
		}
		objnumbers[idx]++;
	}

	//ɾ����ǰ�Ŀռ�
	for (int i = 0; i < classNum; ++i)
		delete[] m_categories[i].objindex;
	//�ռ�����
	for (int i = 0; i < classNum; ++i)
	{
		setcategoryobjs(objnumbers[i], m_categories[i]);
	}
	//ÿһ�����������ĸ���
	memset(objnumbers, 0, sizeof(int)*classNum);
	for (int i = 0; i<xsize*ysize; ++i)
	{
		for (int j = 0; j<bands; ++j)
			pixelElement[j] = dataImg[j*xsize*ysize + i];

		for (int j = 0; j<classNum; ++j)
			centerdis[j] = GetSSD(pixelElement, m_categories[j].center, bands);

		idx = 0;
		for (int j = 1; j<classNum; ++j)
		{
			if (centerdis[idx]>centerdis[j])
				idx = j;
		}
		m_categories[idx].objindex[objnumbers[idx]] = i;
		objnumbers[idx]++;
	}

	delete[]pixelElement;
	delete[]centerdis;
	delete[]objnumbers;

	//�����������
	CenterKMeans(dataImg, xsize, ysize, bands, classNum, m_categories);
}

void KMeansClassify(const char* pathImgIn, const char* pathImgOut, int classNum)
{
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");	//����·��
	GDALDatasetH m_datasetin = GDALOpen(pathImgIn, GA_ReadOnly);

	int xsize = GDALGetRasterXSize(m_datasetin);
	int ysize = GDALGetRasterYSize(m_datasetin);
	int bands = GDALGetRasterCount(m_datasetin);

	char* classrs = new char[xsize*ysize];
	float* centersPre = new float[classNum*bands];
	float* centersLast = new float[classNum*bands];
	float* dataimg = new float[xsize*ysize*bands];
	category* m_categroies = new category[classNum];

	for (int i = 0; i<bands; i++)
		GDALRasterIO(GDALGetRasterBand(m_datasetin, i + 1), GF_Read, 0, 0, xsize, ysize, dataimg + i*xsize*ysize, xsize, ysize, GDT_Float32, 0, 0);

	//��ʼ��������
	InitKMeans(dataimg, xsize, ysize, bands, classNum, m_categroies);

	//��������������
	CenterKMeans(dataimg, xsize, ysize, bands,classNum, m_categroies);
	for (int i = 0; i < classNum; ++i)
	{
		memcpy(centersPre, m_categroies[i].center, sizeof(float)*bands);
	}

	int iterator_i = 0;
	do
	{
		printf("����������%d\r", iterator_i);
		//���������������������
		IteratorKMeans(dataimg, xsize, ysize, bands, classNum, m_categroies);
		for (int i = 0; i < classNum; ++i)
		{
			memcpy(centersLast + i*bands, m_categroies[i].center, sizeof(float)*bands);
		}

		//�������������
		float disCenter = 0;
		for (int i = 0; i<classNum*bands; ++i)
			disCenter += fabs(centersLast[i] - centersPre[i]);
		if (disCenter<1)
			break;
		else
			memcpy(centersPre, centersLast, sizeof(float)*bands*classNum);
		++iterator_i;
	} while (iterator_i<30);
	printf("\n");

	//�������ͼ
	for (int i = 0; i < classNum; ++i)
	{
		for (int j = 0; j < m_categroies[i].objnumbers; ++j)
			classrs[m_categroies[i].objindex[j]] = char(i);
	}

	GDALDatasetH m_datasetout = GDALCreate(GDALGetDriverByName("GTiff"), pathImgOut, xsize, ysize, 1, GDT_Byte, NULL);
	GDALRasterIO(GDALGetRasterBand(m_datasetout, 1), GF_Write, 0, 0, xsize, ysize, classrs, xsize, ysize, GDT_Byte, 0, 0);

	const char* proj = GDALGetProjectionRef(m_datasetin);
	GDALSetProjection(m_datasetout, proj);
	double adfGeoTrans[6];
	GDALGetGeoTransform(m_datasetin, adfGeoTrans);
	GDALSetGeoTransform(m_datasetout, adfGeoTrans);

	//�����ڴ�
	if (classrs != NULL)
		delete[]classrs;
	if (dataimg != NULL)
		delete[]dataimg;
	if (centersPre != NULL)
		delete[]centersPre;
	if (centersLast != NULL)
		delete[]centersLast;

	classrs = NULL;
	dataimg = NULL;
	centersPre = centersLast = NULL;

	GDALClose(m_datasetin);
	GDALClose(m_datasetout);

	for (int i = 0; i < classNum; ++i)
		freecategory(m_categroies[i]);
	delete[]m_categroies;
}

void ISODataClassify(const char* pathImgIn, const char* pathImgOut, int classNum, float maxDev, float minDis)
{
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");	//����·��
	GDALDatasetH m_datasetin = GDALOpen(pathImgIn, GA_ReadOnly);

	int xsize = GDALGetRasterXSize(m_datasetin);
	int ysize = GDALGetRasterYSize(m_datasetin);
	int bands = GDALGetRasterCount(m_datasetin);

	char* classrs = new char[xsize*ysize];
	float* dataimg = new float[xsize*ysize*bands];
	category* m_categroies = new category[classNum];

	for (int i = 0; i<bands; i++)
		GDALRasterIO(GDALGetRasterBand(m_datasetin, i + 1), GF_Read, 0, 0, xsize, ysize, dataimg + i*xsize*ysize, xsize, ysize, GDT_Float32, 0, 0);

	//��ʼ��������
	InitKMeans(dataimg, xsize, ysize, bands, classNum, m_categroies);
	//��������������
	CenterKMeans(dataimg, xsize, ysize, bands, classNum, m_categroies);

	int preclassnum = classNum, lastclassnum = classNum;

	do
	{
		float* tempdev = new float[classNum*bands];
		float* centersPre = new float[classNum*bands];
		float* centersLast = new float[classNum*bands];
		for (int i = 0; i < classNum; ++i)
		{
			memcpy(centersPre, m_categroies[i].center, sizeof(float)*bands);
		}

		int iterator_i = 0;
		do
		{
			printf("����������%d\r", iterator_i);
			//���������������������
			IteratorKMeans(dataimg, xsize, ysize, bands, classNum, m_categroies);
			for (int i = 0; i < classNum; ++i)
				memcpy(centersLast+i*bands, m_categroies[i].center, sizeof(float)*bands);
			for (int i = 0; i < classNum; ++i)
				memcpy(tempdev + i*bands, m_categroies[i].objdervation, sizeof(float)*bands);

			//�������������
			float disCenter = 0;
			for (int i = 0; i<classNum*bands; ++i)
				disCenter += fabs(centersLast[i] - centersPre[i]);
			if (disCenter<1)
				break;
			else
				memcpy(centersPre, centersLast, sizeof(float)*bands*classNum);
			++iterator_i;
		} while (iterator_i<100);
		printf("\n");

		//�жϷ���֮�����ĸ���
		for (int i = 0; i < classNum; ++i)
		{
			float meandev=0;
			for (int j = 0; j < bands; ++j)
				meandev += m_categroies[i].objdervation[j];
			if (meandev / bands > maxDev)
				lastclassnum++;
		}
		if (lastclassnum == classNum)
			break;
		else
		{

			//�������������
			for (int i = 0; i < classNum; ++i)
				freecategory(m_categroies[i]);
			delete[]m_categroies;

			//��������ռ�
			m_categroies = new category[lastclassnum];
			for (int i = 0; i < lastclassnum; ++i)
			{
				m_categroies[i] = createcategory(bands);
			}

			//����
			//��ʼֵ
			int precategoryidx = 0;
			int lastcategoryidx = 0;//�����ڼ��������
			for (int i = 0; i < classNum; ++i)
			{
				float meandev = 0;
				for (int j = 0; j < bands; ++j)
					meandev += tempdev[i*bands+j];

				//������ֵ�����
				if (meandev / bands > maxDev)
				{
					for (int j = 0; j < bands; ++j)
					{
						m_categroies[lastcategoryidx].center[j] = abs(centersLast[precategoryidx*bands + j]- 0.5*tempdev[precategoryidx*bands + j]);
						m_categroies[lastcategoryidx+1].center[j] = abs(centersLast[precategoryidx*bands + j]+ 0.5*tempdev[precategoryidx*bands + j]);
					}
					lastcategoryidx += 2;
					precategoryidx++;
				}
				else
				{
					for (int j = 0; j < bands; ++j)
					{
						m_categroies[lastcategoryidx].center[j] = centersLast[precategoryidx*bands + j];
					}
					lastcategoryidx++;
					precategoryidx++;
				}
			}

			classNum = lastclassnum;
		}
		delete[]tempdev;
		delete[]centersLast;
		delete[]centersPre;
	} while (true);
	printf("\n");

	//�������ͼ
	for (int i = 0; i < classNum; ++i)
	{
		for (int j = 0; j < m_categroies[i].objnumbers; ++j)
			classrs[m_categroies[i].objindex[j]] = char(i);
	}

	GDALDatasetH m_datasetout = GDALCreate(GDALGetDriverByName("GTiff"), pathImgOut, xsize, ysize, 1, GDT_Byte, NULL);
	GDALRasterIO(GDALGetRasterBand(m_datasetout, 1), GF_Write, 0, 0, xsize, ysize, classrs, xsize, ysize, GDT_Byte, 0, 0);

	const char* proj = GDALGetProjectionRef(m_datasetin);
	GDALSetProjection(m_datasetout, proj);
	double adfGeoTrans[6];
	GDALGetGeoTransform(m_datasetin, adfGeoTrans);
	GDALSetGeoTransform(m_datasetout, adfGeoTrans);

	//�����ڴ�
	if (classrs != NULL)
		delete[]classrs;
	if (dataimg != NULL)
		delete[]dataimg;

	classrs = NULL;
	dataimg = NULL;

	GDALClose(m_datasetin);
	GDALClose(m_datasetout);

	for (int i = 0; i < classNum; ++i)
		freecategory(m_categroies[i]);
	delete[]m_categroies;
}