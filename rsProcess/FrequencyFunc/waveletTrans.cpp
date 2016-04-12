#include "waveletTrans.h"
//============================================================================
// Name        : waveletTrans.cpp
// Author      : ww
// Version     : 1.0  
// Copyright   : ww
// Description : Ӱ���С���任������任
//============================================================================
//  [4/9/2014 wuwei just you]
// ���Ӷ�Ӱ�����в��ε�С���任��С����任����
// ���Ӷ�Ӱ�����в��ε�Ӱ���ͨ�͸�ͨ�˲�
// [3/23/2015 wuwei just you]
/********************************************************************************
*����������	LPFliter��HPFliter	�����ͼ���С���任							*	
*����������	char* pathSrc       ������Ӱ��·��     								*
*			char* pathDst1		�����Ӱ��·��									*
*			int band			������Ӱ��ĵڼ�������							*
********************************************************************************/
//����С���任�ĸ�ͨ�˲�
void HPFliter(char* pathSrc,char* pathDst,int band)
{
	//����С���任
	WaveletTransTri(pathSrc,"~tempH.tif","~tempV.tif",band);	

	//��ȡ�����˲�����
	GDALAllRegister();
	GDALDatasetH m_dataset;
	m_dataset=GDALOpen("~tempV.tif",GA_Update);
	int width=GDALGetRasterXSize(m_dataset);
	int heigh=GDALGetRasterYSize(m_dataset);
	//const int layer=8;
	float* m_data=new float[width*heigh];
	GDALRasterBandH m_band=GDALGetRasterBand(m_dataset,1);
	GDALRasterIO(m_band,GF_Read,0,0,width,heigh,m_data,width,heigh,GDT_Float32,0,0);

	int iWidth=width>>3;
	int iHeigh=heigh>>3;

	for (int i=0;i<iWidth;i++)
		for (int j=0;j<iHeigh;j++)
			m_data[j*width+i]=0;
	GDALRasterIO(GDALGetRasterBand(m_dataset,1),GF_Write,0,0,width,heigh,m_data,width,heigh,GDT_Float32,0,0);
	GDALClose(m_dataset);
	delete[]m_data;

	//С�����任
	WaveletInvTrans(pathDst,"~tempH.tif","~tempV.tif",3,1);
	std::remove("~tempH.tif");
	std::remove("~tempV.tif");
}
void HPFliter(char* pathSrc,char* pathDst)
{
	//����С���任
	WaveletTransTri(pathSrc,"~tempH.tif","~tempV.tif");	

	//��ȡ�����˲�����
	GDALAllRegister();
	GDALDatasetH m_dataset;
	m_dataset=GDALOpen("~tempV.tif",GA_Update);
	int width=GDALGetRasterXSize(m_dataset);
	int heigh=GDALGetRasterYSize(m_dataset);
	int bands=GDALGetRasterCount(m_dataset);
	//const int layer=8;
	float* m_data=new float[width*heigh];
	for(int k=0;k<bands;k++)
	{

		GDALRasterBandH m_band=GDALGetRasterBand(m_dataset,k+1);
		GDALRasterIO(m_band,GF_Read,0,0,width,heigh,m_data,width,heigh,GDT_Float32,0,0);

		int iWidth=width>>3;
		int iHeigh=heigh>>3;

		for (int i=0;i<iWidth;i++)
			for (int j=0;j<iHeigh;j++)
				m_data[j*width+i]=0;
		GDALRasterIO(GDALGetRasterBand(m_dataset,k+1),GF_Write,0,0,width,heigh,m_data,width,heigh,GDT_Float32,0,0);
	}
	
	GDALClose(m_dataset);
	delete[]m_data;

	//С�����任
	WaveletInvTrans(pathDst,"~tempH.tif","~tempV.tif",3);
	std::remove("~tempH.tif");
	std::remove("~tempV.tif");
}

//����С���任�ĵ�ͨ�˲�
void LPFliter(char* pathSrc,char* pathDst,int band)
{
	//����С���任
	WaveletTransOnce(pathSrc,"~tempH.tif","~tempV.tif",band);	

	//��ȡ�����˲�����
	GDALAllRegister();
	GDALDatasetH m_dataset1,m_dataset2;
	m_dataset1=GDALOpen("~tempV.tif",GA_Update);
	m_dataset2=GDALOpen("~tempH.tif",GA_Update);
	int width1=GDALGetRasterXSize(m_dataset1);
	int heigh1=GDALGetRasterYSize(m_dataset1);
	int width2=GDALGetRasterXSize(m_dataset2);
	int heigh2=GDALGetRasterYSize(m_dataset2);

	//const int layer=8;
	float* m_data1=new float[width1*heigh1];
	float* m_data2=new float[width2*heigh2];
	GDALRasterBandH m_band=GDALGetRasterBand(m_dataset1,1);
	GDALRasterIO(m_band,GF_Read,0,0,width1,heigh1,m_data1,width1,heigh1,GDT_Float32,0,0);

	int iWidth1=width1>>1;
	int iHeigh1=heigh1>>1;
	int iWidth2=width2>>1;
	int iHeigh2=heigh2;
	for (int i=width1-iWidth1;i<width1;i++)
		for (int j=0;j<heigh1;j++)
			m_data1[j*width1+i]=0;
	for (int i=0;i<width1;i++)
		for (int j=heigh1-iHeigh1;j<heigh1;j++)
			m_data1[j*width1+i]=0;
	for (int i=iWidth2;i<width2;i++)
		for(int j=0;j<iHeigh2;j++)
			m_data2[j*width1+i]=0;

	m_band=GDALGetRasterBand(m_dataset1,1);
	GDALRasterIO(m_band,GF_Write,0,0,width1,heigh1,m_data1,width1,heigh1,GDT_Float32,0,0);
	m_band=GDALGetRasterBand(m_dataset2,1);
	GDALRasterIO(m_band,GF_Write,0,0,width2,heigh2,m_data2,width2,heigh2,GDT_Float32,0,0);
	GDALClose(m_dataset1);
	GDALClose(m_dataset2);
	delete[]m_data1;
	delete[]m_data2;

	//С�����任
	WaveletInvTrans(pathDst,"~tempH.tif","~tempV.tif",1);
	std::remove("~tempH.tif");
	std::remove("~tempV.tif");
}
void LPFliter(char* pathSrc,char* pathDst)
{
	//����С���任
	WaveletTransTri(pathSrc,"~tempH.tif","~tempV.tif");	

	//��ȡ�����˲�����
	GDALAllRegister();
	GDALDatasetH m_dataset1,m_dataset2;
	m_dataset1=GDALOpen("~tempV.tif",GA_Update);
	m_dataset2=GDALOpen("~tempH.tif",GA_Update);
	int width1=GDALGetRasterXSize(m_dataset1);
	int heigh1=GDALGetRasterYSize(m_dataset1);
	int width2=GDALGetRasterXSize(m_dataset2);
	int heigh2=GDALGetRasterYSize(m_dataset2);
	int bands =GDALGetRasterCount(m_dataset1);

	//const int layer=8;
	float* m_data1=new float[width1*heigh1];
	float* m_data2=new float[width2*heigh2];

	for(int i=0;i<bands;i++)
	{
		GDALRasterBandH m_band=GDALGetRasterBand(m_dataset1,i+1);
		GDALRasterIO(m_band,GF_Read,0,0,width1,heigh1,m_data1,width1,heigh1,GDT_Float32,0,0);

		int iWidth1=width1>>1;
		int iHeigh1=heigh1>>1;
		int iWidth2=width2>>1;
		int iHeigh2=heigh2;
		for (int i=width1-iWidth1;i<width1;i++)
			for (int j=0;j<heigh1;j++)
				m_data1[j*width1+i]=0;
		for (int i=0;i<width1;i++)
			for (int j=heigh1-iHeigh1;j<heigh1;j++)
				m_data1[j*width1+i]=0;
		for (int i=iWidth2;i<width2;i++)
			for(int j=0;j<iHeigh2;j++)
				m_data2[j*width1+i]=0;

		m_band=GDALGetRasterBand(m_dataset1,1);
		GDALRasterIO(m_band,GF_Write,0,0,width1,heigh1,m_data1,width1,heigh1,GDT_Float32,0,0);
		m_band=GDALGetRasterBand(m_dataset2,1);
		GDALRasterIO(m_band,GF_Write,0,0,width2,heigh2,m_data2,width2,heigh2,GDT_Float32,0,0);	
	}

	GDALClose(m_dataset1);
	GDALClose(m_dataset2);
	delete[]m_data1;
	delete[]m_data2;

	//С�����任
	WaveletInvTrans(pathDst,"~tempH.tif","~tempV.tif",1);
	std::remove("~tempH.tif");
	std::remove("~tempV.tif");
}

/********************************************************************************
*����������Hori_Transform���һ��ͼ��ˮƽ�����С���任						    *	
*����������float  **spOriginData����άָ�룬ָ��ԭʼ��ͼ������					*
*		   float  **spTransData0��С���任ϵ�������һ��ˮƽ�任���С��ϵ��	*
*		   int   nHeight		  ��ͼ�����Բ�������ֵΪԭʼͼ��ĸ�			*
*		   int	 nWidth_H	      ��ͼ�����Բ�������ֵΪԭʼͼ����ֵ��һ��	*
*		   float fRadius		  ��С���任���ӣ��ڵ���ʱ����ָ����ֵΪ1		*
********************************************************************************/
void Hori_Transform(float ** spOriginData, float ** spTransData0, int nHeight, int nWidth_H, float fRadius)
{
	int Trans_W,				//ͼ��ɨ���߿��ƣ�������
		Trans_H,				//ͼ��ɨ���߿��ƣ�������
		Trans_M,				//ͼ�����ĺ�����
		Trans_N;				//ͼ������������
	int iWidth = nWidth_H * 2;	//ԭʼͼ��Ŀ��ֵ
	float  Trans_Coeff0;		//С���任ϵ��
	float  Trans_Coeff1;
	//��ģ����ɱ任ϵ���ĸ�ֵ����
	for(Trans_H = 0; Trans_H < nHeight; Trans_H ++)            
	{
		for(Trans_N = 0; Trans_N < nWidth_H; Trans_N ++)           
		{
			Trans_W = Trans_N *2;
			if (fRadius == 2)
			{
				spTransData0[Trans_H][Trans_N] = (spOriginData[Trans_H][Trans_W]);
				spTransData0[Trans_H][nWidth_H+Trans_N] = (spOriginData[Trans_H][Trans_W+1]);
			}
			else
			{
				spTransData0[Trans_H][Trans_N] = (spOriginData[Trans_H][Trans_W]-128);				//even
				spTransData0[Trans_H][nWidth_H+Trans_N] = (spOriginData[Trans_H][Trans_W+1]-128);	//odd
			}
		}
	}
	//ͨ��ͼ��Ĳ�֣����С���任
	for(Trans_H=0; Trans_H<nHeight; Trans_H++)
	{
		for(Trans_N=0; Trans_N<nWidth_H-1; Trans_N++)
		{
			//��ż��ֵ�͵�һ��
			Trans_Coeff1 = ((spTransData0[Trans_H][Trans_N]+spTransData0[Trans_H][Trans_N+1])/2);	
			//�߼��ǲ�������ֵ��1
			Trans_Coeff1=-Trans_Coeff1/*~Trans_Coeff1+1*/;	
			//ϵ��Ԥ��
			spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData0[Trans_H][nWidth_H+Trans_N]+Trans_Coeff1;	
		}
		//���һ��żϵ���ı߽紦��
		Trans_Coeff1 = ((spTransData0[Trans_H][nWidth_H-1]+spTransData0[Trans_H][nWidth_H-2])/2);
		Trans_Coeff1=-Trans_Coeff1/*~Trans_Coeff1+1*/;
		spTransData0[Trans_H][iWidth-1] = spTransData0[Trans_H][iWidth-1]+Trans_Coeff1;
		//���һ����ϵ���ı߽紦��
		Trans_Coeff0 = ((spTransData0[Trans_H][nWidth_H]+spTransData0[Trans_H][nWidth_H+1])/4);
		spTransData0[Trans_H][0] = spTransData0[Trans_H][0]+Trans_Coeff0;
		//�����������������ı任
		for(Trans_N=1; Trans_N<nWidth_H; Trans_N++)
		{
			Trans_Coeff0 = ((spTransData0[Trans_H][nWidth_H+Trans_N]+spTransData0[Trans_H][nWidth_H+Trans_N-1])/4);
			spTransData0[Trans_H][Trans_N] = spTransData0[Trans_H][Trans_N]+Trans_Coeff0;
		}

	}
}

//ת��Ϊһάָ��
void Hori_Transform(float *  spOriginData, float *  spTransData0, int nHeight, int nWidth_H, float fRadius)
{
	//�������
	if (spOriginData==NULL||spTransData0==NULL)
		return ;
	if (nHeight<=0||nWidth_H<=0)
		return ;
	int nWidth=2*nWidth_H;

	float** m_spOriginData;
	float** m_spTransData0;

	//ָ��ռ������
	m_spOriginData=new float*[nHeight];
	m_spTransData0=new float*[nHeight];
	for (int i=0;i<nWidth;i++)
	{
		m_spOriginData[i]=new float[nWidth];
		m_spTransData0[i]=new float[nWidth];

		memcpy(m_spOriginData[i],spOriginData+i*nWidth,sizeof(float)*nWidth);
		memcpy(m_spTransData0[i],m_spTransData0+i*nWidth,sizeof(float)*nWidth);
	}
	//���ö�άָ�뺯��
	Hori_Transform(m_spOriginData,m_spTransData0,nHeight,nWidth_H,fRadius);

	for (int i=0;i<nHeight;i++)
		memcpy(m_spTransData0+i*nWidth,m_spTransData0[i],sizeof(float)*nWidth);

	//ָ��ռ���ͷ�
	for (int i=0;i<nHeight;i++)
	{
		delete[]m_spTransData0[i];
		delete[]m_spOriginData[i];
	}
	delete[]m_spTransData0;
	delete[]m_spOriginData;
}


/********************************************************************************
*����������	Vert_Transform���һ��ͼ����ֱ�����С���任						*	
*����������	float  **spOriginData����άָ�룬ָ��ԭʼ��ͼ������					*
*			float  **spTransData1��С���任ϵ�������һ����ֱ�任���С��ϵ��	*
*			int   nHeight_H		��ͼ�����Բ�������ֵΪԭʼͼ��߶�ֵ��һ��		*
*			int	  nWidth		��ͼ�����Բ�������ֵΪԭʼͼ����				*
*			float fRadius		��С���任���ӣ��ڵ���ʱ����ָ����ֵΪ1		*
********************************************************************************/
void Vert_Transform(float ** spOriginData, float ** spTransData1, int nHeight_H, int nWidth, float fRadius)
{
	int Trans_W,				//ͼ��ɨ���߿��ƣ�������
		Trans_H,				//ͼ��ɨ���߿��ƣ�������
		Trans_M,				//ͼ�����ĺ�����
		Trans_N;				//ͼ������������
	int iHeight = nHeight_H * 2;//ԭʼͼ��Ŀ��ֵ
	float  Trans_Coeff0;			//С���任ϵ��
	float  Trans_Coeff1;
	//��ģ����ɱ任ϵ���ĸ�ֵ����
	for(Trans_W = 0; Trans_W < nWidth; Trans_W ++)            
	{
		for(Trans_M = 0; Trans_M < nHeight_H; Trans_M ++)           
		{
			Trans_H = Trans_M * 2;
			if (fRadius == 2)
			{
				spTransData1[Trans_M][Trans_W] = (spOriginData[Trans_H][Trans_W]);
				spTransData1[nHeight_H + Trans_M][Trans_W] = (spOriginData[Trans_H+1][Trans_W]);
			}
			else
			{
				spTransData1[Trans_M][Trans_W] = (spOriginData[Trans_H][Trans_W]-128);		//even
				spTransData1[nHeight_H + Trans_M][Trans_W] = (spOriginData[Trans_H+1][Trans_W]-128);	//odd
			}
		}
	}
	//ͨ��ͼ��Ĳ�֣����С���任
	for(Trans_W=0; Trans_W<nWidth; Trans_W++)
	{
		for(Trans_M=0; Trans_M<nHeight_H-1; Trans_M++)
		{
			//��ż��ֵ�͵�һ��
			Trans_Coeff1 = ((spTransData1[Trans_M][Trans_W]+spTransData1[Trans_M+1][Trans_W])/2);	
			//�߼��ǲ�������ֵ��1
			Trans_Coeff1=-Trans_Coeff1/*~Trans_Coeff1+1*/;	
			//ϵ��Ԥ��
			spTransData1[nHeight_H + Trans_M][Trans_W] = spTransData1[nHeight_H + Trans_M][Trans_W]+Trans_Coeff1;
		}
		//���һ��żϵ���ı߽紦��
		Trans_Coeff1 = ((spTransData1[nHeight_H-1][Trans_W]+spTransData1[nHeight_H-2][Trans_W])/2);
		Trans_Coeff1=-Trans_Coeff1/*~Trans_Coeff1+1*/;
		spTransData1[iHeight-1][Trans_W] = spTransData1[iHeight-1][Trans_W]+Trans_Coeff1;
		//���һ����ϵ���ı߽紦��
		Trans_Coeff0 = ((spTransData1[nHeight_H][Trans_W]+spTransData1[nHeight_H+1][Trans_W])/4);
		spTransData1[0][Trans_W] = spTransData1[0][Trans_W]+Trans_Coeff0;
		//�����������������ı任
		for(Trans_M=1; Trans_M<nHeight_H; Trans_M++)
		{
			Trans_Coeff0 = ((spTransData1[nHeight_H + Trans_M][Trans_W]+spTransData1[nHeight_H + Trans_M -1][Trans_W])/4);
			spTransData1[Trans_M][Trans_W] = spTransData1[Trans_M][Trans_W]+Trans_Coeff0;
		}

	}
}

//ת��Ϊһάָ��
void Vert_Transform(float *  spOriginData, float *  spTransData1, int nHeight_H, int nWidth, float fRadius)
{
	//�������
	if (spOriginData==NULL||spTransData1==NULL)
		return ;
	if (nHeight_H<=0||nWidth<=0)
		return ;
	int nHeight=2*nHeight_H;

	float** m_spOriginData;
	float** m_spTransData1;

	//ָ��ռ������
	m_spOriginData=new float*[nHeight];
	m_spTransData1=new float*[nHeight];
	for (int i=0;i<nWidth;i++)
	{
		m_spOriginData[i]=new float[nWidth];
		m_spTransData1[i]=new float[nWidth];

		memcpy(m_spOriginData[i],spOriginData+i*nWidth,sizeof(float)*nWidth);
		memcpy(m_spTransData1[i],m_spTransData1+i*nWidth,sizeof(float)*nWidth);
	}
	//���ö�άָ�뺯��
	Vert_Transform(m_spOriginData,m_spTransData1,nHeight_H,nWidth,fRadius);

	for (int i=0;i<nHeight;i++)
		memcpy(m_spTransData1+i*nWidth,m_spTransData1[i],sizeof(float)*nWidth);

	//ָ��ռ���ͷ�
	for (int i=0;i<nHeight;i++)
	{
		delete[]m_spTransData1[i];
		delete[]m_spOriginData[i];
	}
	delete[]m_spTransData1;
	delete[]m_spOriginData;
}


/********************************************************************************
*����������	DWT_Once���һ��ͼ���С���任										*
*����������	float  **spOriginData����άָ�룬ָ��ԭʼ��ͼ������					*
*			float  **spTransData0��С���任ϵ�������һ��ˮƽ�任���С��ϵ��	*
*			float  **spTransData1��С���任ϵ�������һ����ֱ�任���С��ϵ��	*
*			int   nHeight		��ͼ�����Բ�������ֵΪԭʼͼ��ĸ߶�ֵ			*
*			int	  nHeight_H		��ͼ�����Բ�������ֵΪԭʼͼ��߶�ֵ��һ��		*
*			int   nWidth		��ͼ�����Բ�������ֵΪԭʼͼ��Ŀ��ֵ			*
*			int	  nWidth_H		��ͼ�����Բ�������ֵΪԭʼͼ����ֵ��һ��		*
*			int   layer			��С���任�Ĳ�������ֵΪ1��						*
*			float fRadius		��С���任���ӣ��ڵ���ʱ����ָ����ֵΪ1.414		*
********************************************************************************/
void DWT_Once(float ** spOriginData, float ** spTransData0, float ** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	int Trans_W,				//ͼ��ɨ���߿��ƣ�������
		Trans_H,				//ͼ��ɨ���߿��ƣ�������
		Trans_M,				//ͼ�����ĺ�����
		Trans_N;				//ͼ������������
	float  Trans_Coeff0;		//С���任ϵ��
	float  Trans_Coeff1;
	fRadius=1.414f;				//�任�˲�ϵ��
	//��ģ����ɱ任ϵ���ĸ�ֵ����
	//�б任,��һ�Σ�layer=1ʱ��ʱnHeight��Ϊԭʼͼ��ĸ߶�ֵ
	for(Trans_H=0; Trans_H<nHeight; Trans_H++)            
	{
		if(layer == 1)
			//layer=1ʱ��nWidth_HΪԭʼͼ����ֵ��һ��
			for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)          
			{
				Trans_W=Trans_N<<1;
				if (fRadius==2)
				{
					spTransData0[Trans_H][Trans_N] = (spOriginData[Trans_H][Trans_W]);
					spTransData0[Trans_H][nWidth_H+Trans_N] = (spOriginData[Trans_H][Trans_W+1]);
				}
				else
				{
					spTransData0[Trans_H][Trans_N] = (spOriginData[Trans_H][Trans_W]-128);		
					spTransData0[Trans_H][nWidth_H+Trans_N] = (spOriginData[Trans_H][Trans_W+1]-128);	
				}
			}
			//���任��������1,���������Ƶ��С��ϵ��
			if(layer > 1)
				for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
				{
					Trans_W=Trans_N*2;
					spTransData0[Trans_H][Trans_N] = spTransData1[Trans_H][Trans_W];
					spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData1[Trans_H][Trans_W+1];
				}
	}
	for(Trans_H=0; Trans_H<nHeight; Trans_H++)
	{
		for(Trans_N=0; Trans_N<nWidth_H-1; Trans_N++)
		{
			//��ż��ֵ�͵�һ��
			Trans_Coeff1 = ((spTransData0[Trans_H][Trans_N]+spTransData0[Trans_H][Trans_N+1])/2);	
			//�߼��ǲ�������ֵ��1
			Trans_Coeff1=-Trans_Coeff1/*~Trans_Coeff1+1*/;	
			//ϵ��Ԥ��
			spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData0[Trans_H][nWidth_H+Trans_N]+Trans_Coeff1;	
		}
		//���һ��żϵ���ı߽紦��
		Trans_Coeff1 = ((spTransData0[Trans_H][nWidth_H-1]+spTransData0[Trans_H][nWidth_H-2])/2);
		Trans_Coeff1=-Trans_Coeff1/*~Trans_Coeff1+1*/;
		spTransData0[Trans_H][nWidth-1] = spTransData0[Trans_H][nWidth-1]+Trans_Coeff1;
		//���һ����ϵ���ı߽紦��
		Trans_Coeff0 = ((spTransData0[Trans_H][nWidth_H]+spTransData0[Trans_H][nWidth_H+1])/4);
		spTransData0[Trans_H][0] = spTransData0[Trans_H][0]+Trans_Coeff0;
		//�����������������ı任
		for(Trans_N=1; Trans_N<nWidth_H; Trans_N++)
		{
			Trans_Coeff0 = ((spTransData0[Trans_H][nWidth_H+Trans_N]+spTransData0[Trans_H][nWidth_H+Trans_N-1])/4);
			spTransData0[Trans_H][Trans_N] = spTransData0[Trans_H][Trans_N]+Trans_Coeff0;
		}

	}//ˮƽ����ı任����
	//��ֱ����ı任��ʼ������Դδˮƽ�任���С��ϵ��
	for(Trans_M=0; Trans_M<nHeight; Trans_M++)
	{
		for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
		{
			spTransData0[Trans_M][Trans_N]*=fRadius;
			spTransData0[Trans_M][Trans_N+nWidth_H]/=fRadius;
		}
	}
	//���������������spTransData0�У�spTransData0�е�������Ȼ��ż����
	for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
	{
		//�б任
		for(Trans_M=0; Trans_M<nHeight_H; Trans_M++)
		{
			Trans_H =Trans_M<<1;
			//Ƶ��LL����
			spTransData1[Trans_M][Trans_N] = spTransData0[Trans_H][Trans_N];
			//Ƶ��HL����
			spTransData1[nHeight_H+Trans_M][Trans_N] = spTransData0[Trans_H+1][Trans_N];
			//Ƶ��LH����
			spTransData1[Trans_M][nWidth_H+Trans_N] = spTransData0[Trans_H][nWidth_H+Trans_N];	
			//Ƶ��HH����
			spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N] = spTransData0[Trans_H+1][nWidth_H+Trans_N];
		}
		//��һ��������������ϵ��
		for(Trans_M=0; Trans_M<nHeight_H-1; Trans_M++)
		{
			//��ֱ����ı任 
			Trans_Coeff1 = ((spTransData1[Trans_M][Trans_N]+spTransData1[Trans_M+1][Trans_N])/2);
			Trans_Coeff1=-Trans_Coeff1/*~Trans_Coeff1+1*/;
			spTransData1[nHeight_H+Trans_M][Trans_N] = spTransData1[nHeight_H+Trans_M][Trans_N]+Trans_Coeff1;
			Trans_Coeff1 = ((spTransData1[Trans_M][nWidth_H+Trans_N]+spTransData1[Trans_M+1][nWidth_H+Trans_N])/2);
			Trans_Coeff1=-Trans_Coeff1/*~Trans_Coeff1+1*/;
			spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N] = spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N]+Trans_Coeff1;
		}
		Trans_Coeff1 = ((spTransData1[nHeight_H-1][Trans_N]+spTransData1[nHeight_H-2][Trans_N])/2);
		Trans_Coeff1=-Trans_Coeff1/*~Trans_Coeff1+1*/;
		spTransData1[nHeight-1][Trans_N] = spTransData1[nHeight-1][Trans_N]+Trans_Coeff1;
		Trans_Coeff1 = ((spTransData1[nHeight_H-1][nWidth_H+Trans_N]+spTransData1[nHeight_H-2][nWidth_H+Trans_N])/2);
		Trans_Coeff1=-Trans_Coeff1/*~Trans_Coeff1+1*/;
		//�߽紦��
		spTransData1[nHeight-1][nWidth_H+Trans_N] = spTransData1[nHeight-1][nWidth_H+Trans_N]+Trans_Coeff1;

		Trans_Coeff0 = ((spTransData1[nHeight_H][Trans_N]+spTransData1[nHeight_H+1][Trans_N])/4);
		spTransData1[0][Trans_N] = spTransData1[0][Trans_N]+Trans_Coeff0;
		Trans_Coeff0 = ((spTransData1[nHeight_H][nWidth_H+Trans_N]+spTransData1[nHeight_H+1][nWidth_H+Trans_N])/4);
		//�߽紦��
		spTransData1[0][nWidth_H+Trans_N] = spTransData1[0][nWidth_H+Trans_N]+Trans_Coeff0;
		//��һ������ż������ϵ��
		for(Trans_M=1; Trans_M<nHeight_H; Trans_M++)
		{
			Trans_Coeff0 = ((spTransData1[nHeight_H+Trans_M][Trans_N]+spTransData1[nHeight_H+Trans_M-1][Trans_N])/4);
			spTransData1[Trans_M][Trans_N] = spTransData1[Trans_M][Trans_N]+Trans_Coeff0;
			Trans_Coeff0 = ((spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N]+spTransData1[nHeight_H+Trans_M-1][nWidth_H+Trans_N])/4);
			spTransData1[Trans_M][nWidth_H+Trans_N] = spTransData1[Trans_M][nWidth_H+Trans_N]+Trans_Coeff0;
		}
	}
	//���С��ϵ����LLƵ����ϵ�����з�ֵ��ǿ����������ƵƵ����ϵ�����������ֵ
	for(Trans_N=0; Trans_N<nWidth; Trans_N++)
	{
		for(Trans_M=0; Trans_M<nHeight_H; Trans_M++)
		{
			spTransData1[Trans_M][Trans_N]*=fRadius;
			spTransData1[Trans_M+nHeight_H][Trans_N]/=fRadius;
		}
	}
}

//ת��Ϊһάָ��
void DWT_Once(float *  spOriginData, float *  spTransData0, float *  spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	//�������
	if (spOriginData==NULL||spTransData0==NULL||spTransData1==NULL)
		return ;
	if (nHeight<=0||nWidth<=0||nHeight_H<=0||nWidth_H<=0)
		return ;

	float** m_spOriginData;
	float** m_spTransData0;
	float** m_spTransData1;

	//ָ��ռ������
	m_spOriginData=new float*[nHeight];
	m_spTransData0=new float*[nHeight];
	m_spTransData1=new float*[nHeight];
	for (int i=0;i<nHeight;i++)
	{
		m_spOriginData[i]=new float [nWidth];
		m_spTransData0[i]=new float [nWidth];
		m_spTransData1[i]=new float [nWidth];

		memcpy(m_spOriginData[i],spOriginData+i*nWidth,sizeof(float)*nWidth);
		memcpy(m_spTransData0[i],spTransData0+i*nWidth,sizeof(float)*nWidth)  ;
		memcpy(m_spTransData1[i],spTransData1+i*nWidth,sizeof(float)*nWidth)  ;
	}

	//���ö�άָ�뺯��
	DWT_Once(m_spOriginData,m_spTransData0,m_spTransData1,nHeight,nHeight_H,nWidth,nWidth_H,layer,fRadius);

	for (int i=0;i<nHeight;i++)
	{
		memcpy(spTransData0+i*nWidth,m_spTransData0[i],sizeof(float)*nWidth);
		memcpy(spTransData1+i*nWidth,m_spTransData1[i],sizeof(float)*nWidth);
	}

	//ָ��ռ���ͷ�
	for (int i=0;i<nHeight;i++)
	{
		delete[]m_spOriginData[i];
		delete[]m_spTransData0[i];
		delete[]m_spTransData1[i];
	}
	delete[]m_spOriginData;
	delete[]m_spTransData0;
	delete[]m_spTransData1;
}


/********************************************************************************
*����������	DWT_TwoLayers�������ͼ���С���任									*	
*����������	float  **spOriginData����άָ�룬ָ��ԭʼ��ͼ������					*
*			float  **spTransData0��С���任ϵ�������һ��ˮƽ�任���С��ϵ��	*
*			float  **spTransData1��С���任ϵ�������һ����ֱ�任���С��ϵ��	*
*			int   nHeight		��ͼ�����Բ�������ֵΪԭʼͼ��ĸ߶�ֵ			*
*			int	  nHeight_H		��ͼ�����Բ�������ֵΪԭʼͼ��߶�ֵ��һ��		*
*			int   nWidth		��ͼ�����Բ�������ֵΪԭʼͼ��Ŀ��ֵ			*
*			int	  nWidth_H		��ͼ�����Բ�������ֵΪԭʼͼ����ֵ��һ��		*
*			int   layer			��С���任�Ĳ�������ֵΪ2��						*
*			float fRadius		��С���任���ӣ��ڵ���ʱ����ָ����ֵΪ1.414		*
********************************************************************************/
void DWT_TwoLayers(float ** spOriginData, float ** spTransData0, float ** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	int i;
	float  **pData, **pTran0, **pTran1;
	//ͼ������Բ���
	int iWidth, iHeight, iWidth_H, iHeight_H;
	float fr = fRadius;
	//������ݿռ��ָ��
	pData = spOriginData;
	pTran0 = spTransData0;
	pTran1 = spTransData1;
	//ͼ�����Բ�����ֵ
	iWidth = nWidth;	iWidth_H = nWidth_H;
	iHeight = nHeight;	iHeight_H = nHeight_H;
	//����ѭ���������С���任
	for(i=1; i<=layer; i++)
	{
		DWT_Once(pData,pTran0,pTran1,iHeight,iHeight_H,iWidth,iWidth_H,i,fr);
		iHeight=iHeight/2;		iWidth=iWidth/2;
		iHeight_H=iHeight/2;	iWidth_H=iWidth/2;
	}
}

//ת��Ϊһάָ��
void DWT_TwoLayers(float *  spOriginData, float *  spTransData0, float *  spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	//�������
	if (spOriginData==NULL||spTransData0==NULL||spTransData1==NULL)
		return ;
	if (nHeight<=0||nWidth<=0||nHeight_H<=0||nWidth_H<=0)
		return ;

	float** m_spOriginData;
	float** m_spTransData0;
	float** m_spTransData1;

	//ָ��ռ������
	m_spOriginData=new float*[nHeight];
	m_spTransData0=new float*[nHeight];
	m_spTransData1=new float*[nHeight];
	for (int i=0;i<nHeight;i++)
	{
		m_spOriginData[i]=new float[nWidth];
		m_spTransData0[i]=new float[nWidth];
		m_spTransData1[i]=new float[nWidth];

		memcpy(m_spOriginData[i],spOriginData+i*nWidth,sizeof(float)*nWidth);
		memcpy(m_spTransData0[i],spTransData0+i*nWidth,sizeof(float)*nWidth);
		memcpy(m_spTransData1[i],spTransData1+i*nWidth,sizeof(float)*nWidth);
	}

	//���ö�άָ�뺯��
	DWT_TwoLayers(m_spOriginData,m_spTransData0,m_spTransData1,nHeight,nHeight_H,nWidth,nWidth_H,layer,fRadius);

	for (int i=0;i<nHeight;i++)
	{
		memcpy(spTransData0+i*nWidth,m_spTransData0[i],sizeof(float)*nWidth);
		memcpy(spTransData1+i*nWidth,m_spTransData1[i],sizeof(float)*nWidth);
	}

	//ָ��ռ���ͷ�
	for (int i=0;i<nHeight;i++)
	{
		delete[]m_spOriginData[i];
		delete[]m_spTransData0[i];
		delete[]m_spTransData1[i];
	}
	delete[]m_spOriginData;
	delete[]m_spTransData0;
	delete[]m_spTransData1;
}


/********************************************************************************
*����������	DWT_TriLayers�������ͼ���С���任									*	
*����������	float  **spOriginData����άָ�룬ָ��ԭʼ��ͼ������					*
*			float  **spTransData0��С���任ϵ�������һ��ˮƽ�任���С��ϵ��	*
*			float  **spTransData1��С���任ϵ�������һ����ֱ�任���С��ϵ��	*
*			int   nHeight		��ͼ�����Բ�������ֵΪԭʼͼ��ĸ߶�ֵ			*
*			int	  nHeight_H		��ͼ�����Բ�������ֵΪԭʼͼ��߶�ֵ��һ��		*
*			int   nWidth		��ͼ�����Բ�������ֵΪԭʼͼ��Ŀ��ֵ			*
*			int	  nWidth_H		��ͼ�����Բ�������ֵΪԭʼͼ����ֵ��һ��		*
*			int   layer			��С���任�Ĳ���,��ֵΪ3��						*
*			float fRadius		��С���任���ӣ��ڵ���ʱ����ָ����ֵΪ1.414		*
********************************************************************************/
void DWT_TriLayers(float ** spOriginData, float ** spTransData0, float ** spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	int i;
	float  **pData, **pTran0, **pTran1;
	//ͼ������Բ���
	int iWidth, iHeight, iWidth_H, iHeight_H;
	float fr = fRadius;
	//������ݿռ��ָ��
	pData = spOriginData;
	pTran0 = spTransData0;
	pTran1 = spTransData1;
	//ͼ�����Բ�����ֵ
	iWidth = nWidth;	iWidth_H = nWidth_H;
	iHeight = nHeight;	iHeight_H = nHeight_H;
	//����ѭ���������С���任
	for(i=1; i<=layer; i++)
	{
		DWT_Once(pData,pTran0,pTran1,iHeight,iHeight_H,iWidth,iWidth_H,i,fr);
		iHeight=iHeight/2;		iWidth=iWidth/2;
		iHeight_H=iHeight/2;	iWidth_H=iWidth/2;
	}
}

//ת��Ϊһάָ��
void DWT_TriLayers(float *  spOriginData, float *  spTransData0, float *  spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	//�������
	if (spOriginData==NULL||spTransData0==NULL||spTransData1==NULL)
		return ;
	if (nHeight<=0||nWidth<=0||nHeight_H<=0||nWidth_H<=0)
		return ;

	float** m_spOriginData;
	float** m_spTransData0;
	float** m_spTransData1;

	//ָ��ռ������
	m_spOriginData=new float*[nHeight];
	m_spTransData0=new float*[nHeight];
	m_spTransData1=new float*[nHeight];
	for (int i=0;i<nHeight;i++)
	{
		m_spOriginData[i]=new float[nWidth];
		m_spTransData0[i]=new float[nWidth];
		m_spTransData1[i]=new float[nWidth];

		memcpy(m_spOriginData[i],spOriginData+i*nWidth,sizeof(float)*nWidth);
		memcpy(m_spTransData0[i],spTransData0+i*nWidth,sizeof(float)*nWidth);
		memcpy(m_spTransData1[i],spTransData1+i*nWidth,sizeof(float)*nWidth);
	}

	//���ö�άָ�뺯��
	DWT_TriLayers(m_spOriginData,m_spTransData0,m_spTransData1,nHeight,nHeight_H,nWidth,nWidth_H,layer,fRadius);

	for (int i=0;i<nHeight;i++)
	{
		memcpy(spTransData0+i*nWidth,m_spTransData0[i],sizeof(float)*nWidth);
		memcpy(spTransData1+i*nWidth,m_spTransData1[i],sizeof(float)*nWidth);
	}

	//ָ��ռ���ͷ�
	for (int i=0;i<nHeight;i++)
	{
		delete[]m_spOriginData[i];
		delete[]m_spTransData0[i];
		delete[]m_spTransData1[i];
	}
	delete[]m_spOriginData;
	delete[]m_spTransData0;
	delete[]m_spTransData1;
}


/********************************************************************************
*����������	DWTi_Once���һ��ͼ��С���任����任								*	
*����������	float  **spData		����άָ�룬�����ݿռ���С����任���ͼ������*
*			float  **spTransData0��С���任ϵ�������һ��ˮƽ�任���С��ϵ��	*
*			float  **spTransData1��С���任ϵ�������һ����ֱ�任���С��ϵ��	*
*			int   nHeight		��ͼ�����Բ�������ֵΪԭʼͼ��ĸ߶�ֵ			*
*			int	  nHeight_H		��ͼ�����Բ�������ֵΪԭʼͼ��߶�ֵ��һ��		*
*			int   nWidth		��ͼ�����Բ�������ֵΪԭʼͼ��Ŀ��ֵ			*
*			int	  nWidth_H		��ͼ�����Բ�������ֵΪԭʼͼ����ֵ��һ��		*
*			int   layer			��С���任�Ĳ���,��ֵΪ3��						*
*			float fRadius		��С���任���ӣ��ڵ���ʱ����ָ����ֵΪ1.414		*
********************************************************************************/
void DWTi_Once(float  **spData, float  **spTransData0, float  **spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	int Trans_W,				//ͼ��ɨ���߿��ƣ�������
		Trans_H,				//ͼ��ɨ���߿��ƣ�������
		Trans_M,				//ͼ�����ĺ�����
		Trans_N;				//ͼ������������

	float  WvltCoeff0;			//С���任ϵ��
	float  WvltCoeff1;

	for(Trans_N=0; Trans_N<nWidth; Trans_N++)
	{
		for(Trans_M=0; Trans_M<nHeight_H; Trans_M++)
		{
			spTransData1[Trans_M][Trans_N]/=fRadius;
			spTransData1[Trans_M+nHeight_H][Trans_N]*=fRadius;
		}
	}
	//��任��һ������̣������ִ���ֱ����ʼ
	//��ֱ�������任����ʱ��Ȼ��ż�������У���Trans_NΪͼ������������
	for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
	{
		//ż�������С��ϵ����任ʱ�ı߽紦��
		WvltCoeff0 = ((spTransData1[nHeight_H][Trans_N]+spTransData1[nHeight_H+1][Trans_N])/4);
		spTransData1[0][Trans_N] = spTransData1[0][Trans_N]-WvltCoeff0;
		WvltCoeff0 = ((spTransData1[nHeight_H][nWidth_H+Trans_N]+spTransData1[nHeight_H+1][nWidth_H+Trans_N])/4);
		spTransData1[0][nWidth_H+Trans_N] = spTransData1[0][nWidth_H+Trans_N]-WvltCoeff0;
		//��ֱ������任�ĵڶ���ż������С��ϵ������任
		for(Trans_M=1; Trans_M<nHeight_H; Trans_M++)
		{
			WvltCoeff0 = ((spTransData1[nHeight_H+Trans_M][Trans_N]+spTransData1[nHeight_H+Trans_M-1][Trans_N])/4);
			spTransData1[Trans_M][Trans_N] = spTransData1[Trans_M][Trans_N]-WvltCoeff0;
			WvltCoeff0 = ((spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N]+spTransData1[nHeight_H+Trans_M-1][nWidth_H+Trans_N])/4);
			spTransData1[Trans_M][nWidth_H+Trans_N] = spTransData1[Trans_M][nWidth_H+Trans_N]-WvltCoeff0;
		}
		//�ڶ�����������С��ϵ������任
		for(Trans_M=0; Trans_M<nHeight_H-1; Trans_M++)
		{
			WvltCoeff1 = ((spTransData1[Trans_M][Trans_N]+spTransData1[Trans_M+1][Trans_N])/2);
			WvltCoeff1=-WvltCoeff1/*~WvltCoeff1+1*/;
			spTransData1[nHeight_H+Trans_M][Trans_N] = spTransData1[nHeight_H+Trans_M][Trans_N]-WvltCoeff1;
			WvltCoeff1 = ((spTransData1[Trans_M][nWidth_H+Trans_N]+spTransData1[Trans_M+1][nWidth_H+Trans_N])/2);
			WvltCoeff1=-WvltCoeff1/*~WvltCoeff1+1*/;
			spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N] = spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N]-WvltCoeff1;
		}
		//��������С��ϵ����任ʱ�ı߽紦��
		WvltCoeff1 = ((spTransData1[nHeight_H-1][Trans_N]+spTransData1[nHeight_H-2][Trans_N])/2);
		WvltCoeff1=-WvltCoeff1/*~WvltCoeff1+1*/;
		spTransData1[nHeight-1][Trans_N] = spTransData1[nHeight-1][Trans_N]-WvltCoeff1;
		WvltCoeff1 = ((spTransData1[nHeight_H-1][nWidth_H+Trans_N]+spTransData1[nHeight_H-2][nWidth_H+Trans_N])/2);
		WvltCoeff1=-WvltCoeff1/*~WvltCoeff1+1*/;
		spTransData1[nHeight-1][nWidth_H+Trans_N] = spTransData1[nHeight-1][nWidth_H+Trans_N]-WvltCoeff1;
		
		//��ģ����ɱ任ϵ���ĸ�ֵ�����������
		for(Trans_M=0; Trans_M<nHeight_H; Trans_M++)
		{
			Trans_H =Trans_M<<1;
			spTransData0[Trans_H][Trans_N] = spTransData1[Trans_M][Trans_N];
			spTransData0[Trans_H+1][Trans_N] = spTransData1[nHeight_H+Trans_M][Trans_N];
			spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData1[Trans_M][nWidth_H+Trans_N];
			spTransData0[Trans_H+1][nWidth_H+Trans_N]= spTransData1[nHeight_H+Trans_M][nWidth_H+Trans_N];
		}
	}

	//ȥ��С���任��Ƶ��ϵ�����˲�Ӱ��
	for(Trans_M=0; Trans_M<nHeight; Trans_M++)
	{
		for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
		{
			spTransData0[Trans_M][Trans_N]/=fRadius;
			spTransData0[Trans_M][Trans_N+nWidth_H]*=fRadius;
		}
	}
	//�������ˮƽ�������任
	//ˮƽ�������任
	for(Trans_H=0; Trans_H<nHeight; Trans_H++)
	{
		//ż������С��ϵ����任ʱ�ı߽紦��
		WvltCoeff0 = ((spTransData0[Trans_H][nWidth_H]+spTransData0[Trans_H][nWidth_H+1])/4);
		spTransData0[Trans_H][0] = spTransData0[Trans_H][0]-WvltCoeff0;
		//�ڶ���ż������С��ϵ������任
		for(Trans_N=1; Trans_N<nWidth_H; Trans_N++)
		{
			WvltCoeff0 = ((spTransData0[Trans_H][nWidth_H+Trans_N]+spTransData0[Trans_H][nWidth_H+Trans_N-1])/4);
			spTransData0[Trans_H][Trans_N] = spTransData0[Trans_H][Trans_N]-WvltCoeff0;
		}
		//�ڶ�����������С��ϵ������任
		for(Trans_N=0; Trans_N<nWidth_H-1; Trans_N++)
		{
			WvltCoeff1 = ((spTransData0[Trans_H][Trans_N]+spTransData0[Trans_H][Trans_N+1])/2);
			WvltCoeff1=-WvltCoeff1/*~WvltCoeff1+1*/;
			spTransData0[Trans_H][nWidth_H+Trans_N] = spTransData0[Trans_H][nWidth_H+Trans_N]-WvltCoeff1;
		}
		//��������С��ϵ����任ʱ�ı߽紦��
		WvltCoeff1 = ((spTransData0[Trans_H][nWidth_H-1]+spTransData0[Trans_H][nWidth_H-2])/2);
		WvltCoeff1=-WvltCoeff1/*~WvltCoeff1+1*/;
		spTransData0[Trans_H][nWidth-1] = spTransData0[Trans_H][nWidth-1]-WvltCoeff1;

		if(layer > 1)
		{
			for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
			{
				Trans_W =Trans_N<<1;
				spTransData1[Trans_H][Trans_W] = spTransData0[Trans_H][Trans_N];
				spTransData1[Trans_H][Trans_W+1] = spTransData0[Trans_H][nWidth_H+Trans_N];
			}
		}
		if(layer == 1)
		{
			for(Trans_N=0; Trans_N<nWidth_H; Trans_N++)
			{
				Trans_W =Trans_N<<1;
				if(fRadius!=2)
				{
					spTransData0[Trans_H][Trans_N]=spTransData0[Trans_H][Trans_N]+128;
					spTransData0[Trans_H][nWidth_H+Trans_N]=spTransData0[Trans_H][nWidth_H+Trans_N]+128;
				}
				////if(spTransData0[Trans_H][Trans_N]>255) 
				////	spTransData0[Trans_H][Trans_N]=255;
				////if(spTransData0[Trans_H][Trans_N]<0)   
				////	spTransData0[Trans_H][Trans_N]=0;
				////if(spTransData0[Trans_H][nWidth_H+Trans_N]>255) 
				////	spTransData0[Trans_H][nWidth_H+Trans_N]=255;
				////if(spTransData0[Trans_H][nWidth_H+Trans_N]<0)   
				////	spTransData0[Trans_H][nWidth_H+Trans_N]=0;
				//spData[Trans_H][Trans_W] = (un char)spTransData0[Trans_H][Trans_N];
				spData[Trans_H][Trans_W] = spTransData0[Trans_H][Trans_N];
				//spData[Trans_H][Trans_W+1] = (un char)spTransData0[Trans_H][nWidth_H+Trans_N];
				spData[Trans_H][Trans_W+1] =spTransData0[Trans_H][nWidth_H+Trans_N];
			}
		}

	}

}

//����άָ��ת��Ϊһάָ��
void DWTi_Once(float*   spData, float*   spTransData0, float*  spTransData1, int nHeight, int nHeight_H, int nWidth, int nWidth_H, int layer, float fRadius)
{
	//�������
	if (spData==NULL||spTransData0==NULL||spTransData1==NULL)
		return ;
	if (nHeight<=0||nWidth<=0||nHeight_H<=0||nWidth_H<=0)
		return ;

	float** m_spData;
	float** m_spTransData0;
	float** m_spTransData1;

	//ָ��ռ������
	m_spData=new float*[nHeight];
	m_spTransData0=new float*[nHeight];
	m_spTransData1=new float*[nHeight];
	for (int i=0;i<nWidth;i++)
	{
		m_spData[i]=new float [nWidth];
		m_spTransData0[i]=new float[nWidth];
		m_spTransData1[i]=new float[nWidth];

		memcpy(m_spData[i],spData+i*nWidth,sizeof(float)*nWidth);
		memcpy(m_spTransData0[i],spTransData0+i*nWidth,sizeof(float)*nWidth);
		memcpy(m_spTransData1[i],spTransData1+i*nWidth,sizeof(float)*nWidth);
	}

	//���ö�άָ�뺯��
	DWTi_Once(m_spData,m_spTransData0,m_spTransData1,nHeight,nHeight_H,nWidth,nWidth_H,layer,fRadius);

	for (int i=0;i<nWidth;i++)
	{
		memcpy(spTransData0+i*nWidth,m_spTransData0[i],sizeof(float)*nWidth);
		memcpy(spTransData1+i*nWidth,m_spTransData1[i],sizeof(float)*nWidth);
		memcpy(spData+i*nWidth,m_spData[i],sizeof(float)*nWidth);
	}

	//ָ��ռ���ͷ�
	for (int i=0;i<nHeight;i++)
	{
		delete[]m_spData[i];
		delete[]m_spTransData0[i];
		delete[]m_spTransData1[i];
	}
	delete[]m_spData;
	delete[]m_spTransData0;
	delete[]m_spTransData1;
}


//////////////////////////////////////////////////////////////////////////
/********************************************************************************
*����������	WaveletTransOnce,WaveletTransTwice,WaveletTransTri���ͼ���С���任*	
*����������	char* pathSrc       ������С���任Ӱ���·��     					*
*			char* pathDst1		��ˮƽ����С���任ϵ�����·��					*
*			char* pathDst2		����ֱ����С���任ϵ�����·��					*
*			int band			������Ӱ��ĵڼ�������							*
********************************************************************************/
void WaveletTransOnce(char* pathSrc,char* pathDst1,char* pathDst2,int band)
{
	GDALAllRegister();
	GDALDatasetH m_datasetsrc,m_datasetdst1,m_datasetdst2;
	m_datasetsrc=GDALOpen(pathSrc,GA_ReadOnly);
	if(!m_datasetsrc)
		return ;

	//��ȡ���ݺ�Ӱ����Ϣ
	int width=GDALGetRasterBandXSize(m_datasetsrc);
	int heigh=GDALGetRasterBandYSize(m_datasetsrc);
	int bands=GDALGetRasterCount(m_datasetsrc);
	if (band>bands)
		return ;
	
	float* m_Indata =new float[width*heigh];
	float* m_Outdata1=new float[width*heigh];
	float* m_Outdata2=new float[width*heigh];

	GDALRasterBandH m_band;
	//������
	m_band=GDALGetRasterBand(m_datasetsrc,band);
	GDALRasterIO(m_band,GF_Read,0,0,width,heigh,m_Indata,width,heigh,GDT_Float32,0,0);
	const char* proj=GDALGetProjectionRef(m_datasetsrc);
	DWT_Once(m_Indata,m_Outdata1,m_Outdata2,heigh,heigh/2,width,width/2,1,1.414f);
	//д����
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	m_datasetdst1=GDALCreate(GDALGetDriverByName("GTiff"),pathDst1,width,heigh,1,GDT_Float32,papszOptions);
	GDALSetProjection(m_datasetdst1,proj);
	m_band=GDALGetRasterBand(m_datasetdst1,1);
	GDALRasterIO(m_band,GF_Write,0,0,width,heigh,m_Outdata1,width,heigh,GDT_Float32,0,0);

	//д����
	m_datasetdst2=GDALCreate(GDALGetDriverByName("GTiff"),pathDst2,width,heigh,1,GDT_Float32,papszOptions);
	GDALSetProjection(m_datasetdst2,proj);
	m_band=GDALGetRasterBand(m_datasetdst2,1);
	GDALRasterIO(m_band,GF_Write,0,0,width,heigh,m_Outdata2,width,heigh,GDT_Float32,0,0);


	GDALClose(m_datasetdst1);
	GDALClose(m_datasetdst2);
	GDALClose(m_datasetsrc);
	delete[]m_Indata;
	delete[]m_Outdata1;
	delete[]m_Outdata2;
}
void WaveletTransOnce(char* pathSrc,char* pathDst1,char* pathDst2)
{
	GDALAllRegister();
	GDALDatasetH m_datasetsrc,m_datasetdst1,m_datasetdst2;
	m_datasetsrc=GDALOpen(pathSrc,GA_ReadOnly);
	if(!m_datasetsrc)
		return ;

	//��ȡ���ݺ�Ӱ����Ϣ
	int width=GDALGetRasterBandXSize(m_datasetsrc);
	int heigh=GDALGetRasterBandYSize(m_datasetsrc);
	int bands=GDALGetRasterCount(m_datasetsrc);

	float* m_Indata =new float[width*heigh];
	float* m_Outdata1=new float[width*heigh];
	float* m_Outdata2=new float[width*heigh];

	const char* proj=GDALGetProjectionRef(m_datasetsrc);
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	m_datasetdst1=GDALCreate(GDALGetDriverByName("GTiff"),pathDst1,width,heigh,bands,GDT_Float32,papszOptions);
	GDALSetProjection(m_datasetdst1,proj);
	m_datasetdst2=GDALCreate(GDALGetDriverByName("GTiff"),pathDst2,width,heigh,bands,GDT_Float32,papszOptions);
	GDALSetProjection(m_datasetdst2,proj);

	GDALRasterBandH m_band;
	for (int i=0;i<bands;i++)
	{
		m_band=GDALGetRasterBand(m_datasetsrc,i+1);
		GDALRasterIO(m_band,GF_Read,0,0,width,heigh,m_Indata,width,heigh,GDT_Float32,0,0);
		DWT_Once(m_Indata,m_Outdata1,m_Outdata2,heigh,heigh/2,width,width/2,1,1.414f);

		m_band=GDALGetRasterBand(m_datasetdst1,i+1);
		GDALRasterIO(m_band,GF_Write,0,0,width,heigh,m_Outdata1,width,heigh,GDT_Float32,0,0);

		m_band=GDALGetRasterBand(m_datasetdst2,i+1);
		GDALRasterIO(m_band,GF_Write,0,0,width,heigh,m_Outdata2,width,heigh,GDT_Float32,0,0);
	}

	GDALClose(m_datasetdst1);
	GDALClose(m_datasetdst2);
	GDALClose(m_datasetsrc);
	delete[]m_Indata;
	delete[]m_Outdata1;
	delete[]m_Outdata2;
}

//����С���任
void WaveletTransTwice(char* pathSrc,char* pathDst1,char* pathDst2,int band)
{
	GDALAllRegister();
	GDALDatasetH m_datasetsrc,m_datasetdst1,m_datasetdst2;
	m_datasetsrc=GDALOpen(pathSrc,GA_ReadOnly);
	if(!m_datasetsrc)
		return ;

	//��ȡ���ݺ�Ӱ����Ϣ
	int width=GDALGetRasterBandXSize(m_datasetsrc);
	int heigh=GDALGetRasterBandYSize(m_datasetsrc);
	int bands=GDALGetRasterCount(m_datasetsrc);
	float* m_Indata =new float[width*heigh];
	if (band>bands)
		return ;

	float* m_Outdata1=new float[width*heigh];
	float* m_Outdata2=new float[width*heigh];

	GDALRasterBandH m_band;
	//������
	m_band=GDALGetRasterBand(m_datasetsrc,band);
	GDALRasterIO(m_band,GF_Read,0,0,width,heigh,m_Indata,width,heigh,GDT_Float32,0,0);
	const char* proj=GDALGetProjectionRef(m_datasetsrc);
	//DWT_Once(m_Indata,m_Outdata1,m_Outdata2,heigh,heigh/2,width,width/2,1,1.414);
	DWT_TwoLayers(m_Indata,m_Outdata1,m_Outdata2,heigh,heigh/2,width,width/2,2,1.414f);
	//д����
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	m_datasetdst1=GDALCreate(GDALGetDriverByName("GTiff"),pathDst1,width,heigh,1,GDT_Float32,papszOptions);
	GDALSetProjection(m_datasetdst1,proj);
	m_band=GDALGetRasterBand(m_datasetdst1,1);
	GDALRasterIO(m_band,GF_Write,0,0,width,heigh,m_Outdata1,width,heigh,GDT_Float32,0,0);

	//д����
	m_datasetdst2=GDALCreate(GDALGetDriverByName("GTiff"),pathDst2,width,heigh,1,GDT_Float32,papszOptions);
	GDALSetProjection(m_datasetdst2,proj);
	m_band=GDALGetRasterBand(m_datasetdst2,1);
	GDALRasterIO(m_band,GF_Write,0,0,width,heigh,m_Outdata2,width,heigh,GDT_Float32,0,0);


	GDALClose(m_datasetdst1);
	GDALClose(m_datasetdst2);
	GDALClose(m_datasetsrc);
	delete[]m_Indata;
	delete[]m_Outdata1;
	delete[]m_Outdata2;

}
void WaveletTransTwice(char* pathSrc,char* pathDst1,char* pathDst2)
{
	GDALAllRegister();
	GDALDatasetH m_datasetsrc,m_datasetdst1,m_datasetdst2;
	m_datasetsrc=GDALOpen(pathSrc,GA_ReadOnly);
	if(!m_datasetsrc)
		return ;

	//��ȡ���ݺ�Ӱ����Ϣ
	int width=GDALGetRasterBandXSize(m_datasetsrc);
	int heigh=GDALGetRasterBandYSize(m_datasetsrc);
	int bands=GDALGetRasterCount(m_datasetsrc);

	float* m_Indata =new float[width*heigh];
	float* m_Outdata1=new float[width*heigh];
	float* m_Outdata2=new float[width*heigh];

	const char* proj=GDALGetProjectionRef(m_datasetsrc);
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	m_datasetdst1=GDALCreate(GDALGetDriverByName("GTiff"),pathDst1,width,heigh,bands,GDT_Float32,papszOptions);
	GDALSetProjection(m_datasetdst1,proj);
	m_datasetdst2=GDALCreate(GDALGetDriverByName("GTiff"),pathDst2,width,heigh,bands,GDT_Float32,papszOptions);
	GDALSetProjection(m_datasetdst2,proj);

	GDALRasterBandH m_band;
	for (int i=0;i<bands;i++)
	{
		m_band=GDALGetRasterBand(m_datasetsrc,i+1);
		GDALRasterIO(m_band,GF_Read,0,0,width,heigh,m_Indata,width,heigh,GDT_Float32,0,0);
		DWT_TwoLayers(m_Indata,m_Outdata1,m_Outdata2,heigh,heigh/2,width,width/2,2,1.414f);

		m_band=GDALGetRasterBand(m_datasetdst1,i+1);
		GDALRasterIO(m_band,GF_Write,0,0,width,heigh,m_Outdata1,width,heigh,GDT_Float32,0,0);

		m_band=GDALGetRasterBand(m_datasetdst2,i+1);
		GDALRasterIO(m_band,GF_Write,0,0,width,heigh,m_Outdata2,width,heigh,GDT_Float32,0,0);
	}

	GDALClose(m_datasetdst1);
	GDALClose(m_datasetdst2);
	GDALClose(m_datasetsrc);
	delete[]m_Indata;
	delete[]m_Outdata1;
	delete[]m_Outdata2;
}

//����С���任
void WaveletTransTri(char* pathSrc,char* pathDst1,char* pathDst2,int band)
{
	GDALAllRegister();
	GDALDatasetH m_datasetsrc,m_datasetdst1,m_datasetdst2;
	m_datasetsrc=GDALOpen(pathSrc,GA_ReadOnly);
	if(!m_datasetsrc)
		return ;

	//��ȡ���ݺ�Ӱ����Ϣ
	int width=GDALGetRasterBandXSize(m_datasetsrc);
	int heigh=GDALGetRasterBandYSize(m_datasetsrc);
	int bands=GDALGetRasterCount(m_datasetsrc);
	if (band>bands)
		return ;

	float* m_Indata =new float[width*heigh];
	float* m_Outdata1=new float[width*heigh];
	float* m_Outdata2=new float[width*heigh];

	GDALRasterBandH m_band;
	//������
	m_band=GDALGetRasterBand(m_datasetsrc,band);
	GDALRasterIO(m_band,GF_Read,0,0,width,heigh,m_Indata,width,heigh,GDT_Float32,0,0);
	const char* proj=GDALGetProjectionRef(m_datasetsrc);
	//DWT_Once(m_Indata,m_Outdata1,m_Outdata2,heigh,heigh/2,width,width/2,1,1.414);
	//DWT_TwoLayers(m_Indata,m_Outdata1,m_Outdata2,heigh,heigh/2,width,width/2,2,1.414);
	DWT_TriLayers(m_Indata,m_Outdata1,m_Outdata2,heigh,heigh/2,width,width/2,3,1.414f);
	
	//д����
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	m_datasetdst1=GDALCreate(GDALGetDriverByName("GTiff"),pathDst1,width,heigh,1,GDT_Float32,papszOptions);
	GDALSetProjection(m_datasetdst1,proj);
	m_band=GDALGetRasterBand(m_datasetdst1,1);
	GDALRasterIO(m_band,GF_Write,0,0,width,heigh,m_Outdata1,width,heigh,GDT_Float32,0,0);

	//д����
	m_datasetdst2=GDALCreate(GDALGetDriverByName("GTiff"),pathDst2,width,heigh,1,GDT_Float32,papszOptions);
	GDALSetProjection(m_datasetdst2,proj);
	m_band=GDALGetRasterBand(m_datasetdst2,1);
	GDALRasterIO(m_band,GF_Write,0,0,width,heigh,m_Outdata2,width,heigh,GDT_Float32,0,0);


	GDALClose(m_datasetdst1);
	GDALClose(m_datasetdst2);
	GDALClose(m_datasetsrc);
	delete[]m_Indata;
	delete[]m_Outdata1;
	delete[]m_Outdata2;
}
void WaveletTransTri(char* pathSrc,char* pathDst1,char* pathDst2)
{
	GDALAllRegister();
	GDALDatasetH m_datasetsrc,m_datasetdst1,m_datasetdst2;
	m_datasetsrc=GDALOpen(pathSrc,GA_ReadOnly);
	if(!m_datasetsrc)
		return ;

	//��ȡ���ݺ�Ӱ����Ϣ
	int width=GDALGetRasterBandXSize(m_datasetsrc);
	int heigh=GDALGetRasterBandYSize(m_datasetsrc);
	int bands=GDALGetRasterCount(m_datasetsrc);

	float* m_Indata =new float[width*heigh];
	float* m_Outdata1=new float[width*heigh];
	float* m_Outdata2=new float[width*heigh];

	const char* proj=GDALGetProjectionRef(m_datasetsrc);
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	m_datasetdst1=GDALCreate(GDALGetDriverByName("GTiff"),pathDst1,width,heigh,bands,GDT_Float32,papszOptions);
	GDALSetProjection(m_datasetdst1,proj);
	m_datasetdst2=GDALCreate(GDALGetDriverByName("GTiff"),pathDst2,width,heigh,bands,GDT_Float32,papszOptions);
	GDALSetProjection(m_datasetdst2,proj);

	GDALRasterBandH m_band;
	for (int i=0;i<bands;i++)
	{
		m_band=GDALGetRasterBand(m_datasetsrc,i+1);
		GDALRasterIO(m_band,GF_Read,0,0,width,heigh,m_Indata,width,heigh,GDT_Float32,0,0);
		DWT_TriLayers(m_Indata,m_Outdata1,m_Outdata2,heigh,heigh/2,width,width/2,3,1.414f);

		m_band=GDALGetRasterBand(m_datasetdst1,i+1);
		GDALRasterIO(m_band,GF_Write,0,0,width,heigh,m_Outdata1,width,heigh,GDT_Float32,0,0);

		m_band=GDALGetRasterBand(m_datasetdst2,i+1);
		GDALRasterIO(m_band,GF_Write,0,0,width,heigh,m_Outdata2,width,heigh,GDT_Float32,0,0);
	}

	GDALClose(m_datasetdst1);
	GDALClose(m_datasetdst2);
	GDALClose(m_datasetsrc);
	delete[]m_Indata;
	delete[]m_Outdata1;
	delete[]m_Outdata2;
}

/********************************************************************************
*����������	WaveletInvTrans���һ��ͼ��任����任								*	
*����������	char* pathDst		�������任��Ӱ��·��							*
*			char* pathSrc1		��ˮƽ������С���任ϵ��Ӱ��·��				*
*			char* pathSrc2		����ֱ������С���任ϵ��Ӱ��·��				*
*			int layer			��С����任�Ĳ�������С�����任������ͬ��ԭ��	*
*			int band			��С���任����Ĳ�����Ϣ��������ʱָ��Ϊ1		*
********************************************************************************/
//С����任
void WaveletInvTrans(char* pathDst,char* pathSrc1,char* pathSrc2,int layer,int band/*=1*/)
{
	GDALAllRegister();
	GDALDatasetH m_datasetdst,m_datasetsrc1,m_datasetsrc2;

	m_datasetsrc1=GDALOpen(pathSrc1,GA_ReadOnly);
	m_datasetsrc2=GDALOpen(pathSrc2,GA_ReadOnly);
	if(m_datasetsrc1==NULL||m_datasetsrc2==NULL)
		return;

	//��ȡӰ������
	int width=GDALGetRasterXSize(m_datasetsrc1);
	int heigh=GDALGetRasterYSize(m_datasetsrc2);
	int width_h=width/2;
	int heigh_h=heigh/2;
	int iHeigh =/*heigh/4*/heigh/int(pow((double)2,layer-1));
	int iWidth=/*width/4*/width/int(pow((double)2,layer-1));
	int iHeigh_h=/*iHeigh/2*/heigh_h/int(pow((double)2,layer-1));
	int iWidth_h=/*iWidth/2*/width_h/int(pow((double)2,layer-1));

	//�ռ�����
	float** m_datasrc1=new float*[heigh];
	float** m_datasrc2=new float*[heigh];
	float** m_datadst=new float*[heigh];
	float* datasrc1=new float[heigh*width];
	float* datasrc2=new float[heigh*width];
	float* datadst =new float[heigh*width];

	if (m_datasrc1==NULL||m_datasrc2==NULL||m_datadst==NULL)
		return;
	for (int i=0;i<heigh;i++)
	{
		m_datasrc1[i]=new float[width];
		m_datasrc2[i]=new float[width];
		m_datadst[i] =new float[width];
	}

	GDALRasterBandH m_bandH;
	m_bandH=GDALGetRasterBand(m_datasetsrc1,band);
	GDALRasterIO(m_bandH,GF_Read,0,0,width,heigh,datasrc1,width,heigh,GDT_Float32,0,0);
	m_bandH=GDALGetRasterBand(m_datasetsrc2,band);
	GDALRasterIO(m_bandH,GF_Read,0,0,width,heigh,datasrc2,width,heigh,GDT_Float32,0,0);

	for (int i=0;i<heigh;i++)
	{
		memcpy(m_datasrc1[i],datasrc1+i*width,sizeof(float)*width);
		memcpy(m_datasrc2[i],datasrc2+i*width,sizeof(float)*width);
	}

	//�������ݼ�
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	m_datasetdst=GDALCreate(GDALGetDriverByName("GTiff"),pathDst,width,heigh,1,GDT_Float32,papszOptions);
	if(m_datasetdst==NULL)
		return;

	for(int i=layer;i>=1;i--)
	{
		DWTi_Once(m_datadst,m_datasrc1,m_datasrc2,iHeigh,iHeigh_h,iWidth,iWidth_h,i,1.414f/*2.0f*//*1.731*/);
		iHeigh*=2;		  iWidth*=2;
		iHeigh_h=iHeigh_h*2;iWidth_h=iWidth_h*2;
	}

	for (int i=0;i<heigh;i++)
		memcpy(datadst+i*width,m_datadst[i],sizeof(float)*width);

	m_bandH=GDALGetRasterBand(m_datasetdst,band);
	GDALRasterIO(m_bandH,GF_Write,0,0,width,heigh,datadst,width,heigh,GDT_Float32,0,0);
	//ɾ��ָ��

	GDALClose(m_datasetdst );
	GDALClose(m_datasetsrc1);
	GDALClose(m_datasetsrc2);

	for (int i=0;i<heigh;i++)
	{
		delete[]m_datadst[i] ;
		delete[]m_datasrc1[i];
		delete[]m_datasrc2[i];
	}
	delete[]m_datadst ;
	delete[]m_datasrc1;
	delete[]m_datasrc2;

	delete[]datadst;
	delete[]datasrc1;
	delete[]datasrc2;
}
void WaveletInvTrans(char* pathDst,char* pathSrc1,char* pathSrc2,int layer)
{
	GDALAllRegister();
	GDALDatasetH m_datasetdst,m_datasetsrc1,m_datasetsrc2;

	m_datasetsrc1=GDALOpen(pathSrc1,GA_ReadOnly);
	m_datasetsrc2=GDALOpen(pathSrc2,GA_ReadOnly);
	if(m_datasetsrc1==NULL||m_datasetsrc2==NULL)
		return;

	//��ȡӰ������
	int width=GDALGetRasterXSize(m_datasetsrc1);
	int heigh=GDALGetRasterYSize(m_datasetsrc2);
	int bands1=GDALGetRasterCount(m_datasetsrc1);
	int bands2=GDALGetRasterCount(m_datasetsrc2);
	if(bands2!=bands1)
		return ;

	int width_h=width/2;
	int heigh_h=heigh/2;
	int iHeigh =/*heigh/4*/heigh/int(pow((double)2,layer-1));
	int iWidth=/*width/4*/width/int(pow((double)2,layer-1));
	int iHeigh_h=/*iHeigh/2*/heigh_h/int(pow((double)2,layer-1));
	int iWidth_h=/*iWidth/2*/width_h/int(pow((double)2,layer-1));

	//�ռ�����
	float** m_datasrc1=new float*[heigh];
	float** m_datasrc2=new float*[heigh];
	float** m_datadst=new float*[heigh];
	float* datasrc1=new float[heigh*width];
	float* datasrc2=new float[heigh*width];
	float* datadst =new float[heigh*width];

	if (m_datasrc1==NULL||m_datasrc2==NULL||m_datadst==NULL)
		return;
	for (int i=0;i<heigh;i++)
	{
		m_datasrc1[i]=new float[width];
		m_datasrc2[i]=new float[width];
		m_datadst[i] =new float[width];
	}

	//�������ݼ�
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	m_datasetdst=GDALCreate(GDALGetDriverByName("GTiff"),pathDst,width,heigh,bands1,GDT_Float32,papszOptions);
	if(m_datasetdst==NULL)
		return;

	GDALRasterBandH m_bandH;

	for (int i=0;i<bands1;i++)
	{
		m_bandH=GDALGetRasterBand(m_datasetsrc1,i+1);
		GDALRasterIO(m_bandH,GF_Read,0,0,width,heigh,datasrc1,width,heigh,GDT_Float32,0,0);
		m_bandH=GDALGetRasterBand(m_datasetsrc2,i+1);
		GDALRasterIO(m_bandH,GF_Read,0,0,width,heigh,datasrc2,width,heigh,GDT_Float32,0,0);

		for (int j=0;j<heigh;j++)
		{
			memcpy(m_datasrc1[j],datasrc1+j*width,sizeof(float)*width);
			memcpy(m_datasrc2[j],datasrc2+j*width,sizeof(float)*width);
		}

		for(int j=layer;j>=1;j--)
		{
			DWTi_Once(m_datadst,m_datasrc1,m_datasrc2,iHeigh,iHeigh_h,iWidth,iWidth_h,j,1.414f/*2.0f*//*1.731*/);
			iHeigh*=2;		  iWidth*=2;
			iHeigh_h=iHeigh_h*2;iWidth_h=iWidth_h*2;
		}

		for (int j=0;j<heigh;j++)
			memcpy(datadst+j*width,m_datadst[j],sizeof(float)*width);

		m_bandH=GDALGetRasterBand(m_datasetdst,i+1);
		GDALRasterIO(m_bandH,GF_Write,0,0,width,heigh,datadst,width,heigh,GDT_Float32,0,0);

		for(int j=layer;j>=1;j--)
		{
			iHeigh/=2;		  iWidth/=2;
			iHeigh_h=iHeigh_h/2;iWidth_h=iWidth_h/2;
		}
	}

	GDALClose(m_datasetdst );
	GDALClose(m_datasetsrc1);
	GDALClose(m_datasetsrc2);

	for (int i=0;i<heigh;i++)
	{
		delete[]m_datadst[i] ;
		delete[]m_datasrc1[i];
		delete[]m_datasrc2[i];
	}
	delete[]m_datadst ;
	delete[]m_datasrc1;
	delete[]m_datasrc2;

	delete[]datadst;
	delete[]datasrc1;
	delete[]datasrc2;

}
