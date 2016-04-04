#include "fusionfunc.h"
#include "..\rsImgProcess\SampleFunc.h"
#include "..\gdal/include/gdal_priv.h"
#include "HyperSpectural.h"
#include "..\FrequencyFunc\waveletTrans.h" 
#pragma comment(lib,"gdal_i.lib")
//���ݻ�ȡ���η�Χ�ķ������
void wavelet_get_spectral_group(float* hywavelen,int hynum,float* mswavelen,int msnum,int *group,bool range)
{
	memset(group,0,sizeof(int)*hynum);
	//�����벨�β����������
	if(hywavelen==NULL&&mswavelen==NULL)
	{
		//ֱ�Ӷ�Ӧ���ֵ ���ղ���ƽ������
		int hmMap=hynum/msnum;
		for (int i=0;i<msnum;i++)
			for (int j=0;j<hmMap;j++)
				group[i*hmMap+j]=i;
		group[hynum-1]=msnum-1;
	}

	//�����벨���������  ���Ȼ�ȡ����ײ��ζ�Ӧ�ĸ߹��ײ��� Ȼ����д���
	if(hywavelen!=NULL&&mswavelen!=NULL)	
	{
		//�Ը߹��׼���������ζ�Ӧ�Ķ���ײ���
		if(range==true)		//��������ǹ��׷�Χ
		{
			for (int i=0;i<hynum;i++)
			{
				for(int j=0;j<msnum;j++)
				{
					if(hywavelen[j]<mswavelen[2*j+1])
					{
						group[j]=j;
						break;
					}
				}
			}
		}
		else
		{
			//��ȡ����Ĺ���
			for(int i=0;i<hynum;i++)//ֱ��������ǹ���
			{
				int indexm=0;
				float mindata=1000;;
				for (int j=0;j<msnum;j++)
				{
					if(mindata>abs(hywavelen[i]-mswavelen[j]))
					{
						mindata=abs(hywavelen[i]-mswavelen[j]);
						indexm=j;
					}
				}
				group[i]=indexm;
			}
		}//�Ը߹������ݷ������
	}
}

//Ӱ���ز���
void wavelet_image_sample(char* pathInHy,char* pathInMs,char* pathOut)
{
	printf("�߹���ͼ���ز���\n");
	GDALAllRegister();
	GDALDatasetH m_datasetHy=GDALOpen(pathInHy,GA_ReadOnly);
	GDALDatasetH m_datasetMs=GDALOpen(pathInMs,GA_ReadOnly);

	int xsizems=GDALGetRasterXSize(m_datasetMs);
	int ysizems=GDALGetRasterYSize(m_datasetMs);

	int xsizehy=GDALGetRasterXSize(m_datasetHy);
	int ysizehy=GDALGetRasterYSize(m_datasetHy);

	CSampleFunc samplefunc;
	samplefunc.cubic_sample(pathInHy,xsizems,ysizems,pathOut);
}

//�Ը߹���Ӱ���ÿһ�����ν���С���任
void wavelet_to_image(char* pathHy,char* pathHyW1,char* pathHyW2,char* pathMs,char* pathMsW1,char* pathMsW2)
{
	WaveletTransTri(pathHy,pathHyW1,pathHyW2);
	WaveletTransTri(pathMs,pathMsW1,pathMsW2);
}

//ͨ�������Ķ�Ӧ���ν���С���任�ں�
void wavelet_fusion(char* pathHyW1,char* pathHyW2,char* pathMsW2,char* pathRecov,int* group)
{
	GDALAllRegister();
	GDALDatasetH m_datasethyw1=GDALOpen(pathHyW1,GA_ReadOnly);
	GDALDatasetH m_datasethyw2=GDALOpen(pathHyW2,GA_Update);
	GDALDatasetH m_datasetmsw1=GDALOpen(pathMsW2,GA_ReadOnly);

	int xsize=GDALGetRasterXSize(m_datasethyw1);
	int ysize=GDALGetRasterYSize(m_datasethyw1);
	int band2=GDALGetRasterCount(m_datasethyw1);
	int band1=GDALGetRasterCount(m_datasetmsw1);

	float* datahy=new float[xsize*ysize];
	float* datams=new float[xsize*ysize];

	for (int i=0;i<band1;i++)
	{
		printf("\r�ںϲ���%d/%d",band1,i+1);
		GDALRasterIO(GDALGetRasterBand(m_datasethyw2,i+1),GF_Read,0,0,xsize,ysize,datahy,xsize,ysize,GDT_Float32,0,0);
		GDALRasterIO(GDALGetRasterBand(m_datasetmsw1,group[i]+1),GF_Read,0,0,xsize,ysize,datams,xsize,ysize,GDT_Float32,0,0);
		
		for (int j=0;j<xsize;j++)
			for(int k=0;k<ysize;k++)
				if(j>xsize/8||i>ysize/8)
					datahy[k*xsize+j]=datams[k*xsize+j];
		GDALRasterIO(GDALGetRasterBand(m_datasethyw2,i+1),GF_Write,0,0,xsize,ysize,datahy,xsize,ysize,GDT_Float32,0,0);
	}
	printf("\n");

	GDALClose(m_datasethyw1);
	GDALClose(m_datasethyw2);
	GDALClose(m_datasetmsw1);
	delete[]datahy;
	delete[]datams;

	printf("С����任\n");
	WaveletInvTrans(pathRecov,pathHyW1,pathHyW2,3);
}