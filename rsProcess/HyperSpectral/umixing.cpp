#include "fusionfunc.h"
#include <fstream>
#include "..\gdal\include\gdal_priv.h"
#include "..\matrixOperation.h"	//��������
#include"..\Global.h"
#include "..\rsImgProcess\SampleFunc.h"			//��������
using namespace std;

#pragma comment(lib,"gdal_i.lib")
//����Ӱ�������С���˽�죨����Լ����
void endmember_unmix_LSE(char* pathEnd,char* pathImg,char* pathRed,int endnumbers)
{
	GDALAllRegister();
	CPLSetConfigOption( "GDAL_FILENAME_IS_UTF8", "NO" );	//����·��

	GDALDatasetH m_dataset;
	m_dataset=GDALOpen(pathImg,GA_ReadOnly);
	int xsize=GDALGetRasterXSize(m_dataset);
	int ysize=GDALGetRasterYSize(m_dataset);
	int bands=GDALGetRasterCount(m_dataset);

	ofstream ofs;
	ofs.open("test.txt");

	if (endnumbers>bands)
	{
		//�޷����
		printf("��Ԫ�����ڲ��θ������޷�������С������⣡\n");
		return;
	}

	float *imgData=new float[xsize*ysize*bands];
	float *endData=new float[bands*endnumbers];

	//��ȡ��Ԫ����
	endData=get_endmenber_spectralf(pathEnd,bands,endnumbers);

	double *dendData=new double[bands*endnumbers];
	//��Ԫ���ݰ�������С
	for (int i=0;i<endnumbers*bands;i++)
		dendData[i]=endData[i]/*/10.0f*/;

	//��ȡӰ������
	for (int i=0;i<bands;i++)
		GDALRasterIO(GDALGetRasterBand(m_dataset,i+1),GF_Read,0,0,xsize,ysize,imgData+i*xsize*ysize,xsize,ysize,GDT_Float32,0,0);
	GDALClose(m_dataset);

	//��С���˽������
	double* data =new double[bands];
	double* datamuti=new double[endnumbers];

	double* res  =new double[endnumbers];
	double* endTrans=new double[endnumbers*bands];
	double* endMuti =new double[endnumbers*endnumbers];
	double* endInv  =new double[endnumbers*endnumbers];
	float* resImgae=new float[endnumbers*xsize*ysize];


	//��������ΪӰ������......
	for (int i=0;i<xsize*ysize;i++)
	{
		for(int j=0;j<bands;j++)
			data[j]=imgData[j*xsize*ysize+i];

		//��������
		MatrixTrans(dendData,bands,endnumbers,endTrans);
		MatrixMuti(endTrans,endnumbers,bands,endnumbers,dendData,endMuti);
		MatrixInverse(endMuti,endnumbers,endInv);
		MatrixMuti(endTrans,endnumbers,bands,1,data,datamuti);
		MatrixMuti(endInv,endnumbers,endnumbers,1,datamuti,res);

		//��������
		printf("\r��С���˼����ȴ������أ�%d/%d",xsize*ysize,i+1);
		
		//for(int j=0;j<endnumbers;j++)
		//	ofs<<res[j]<<"  ";
		//ofs<<endl;

		//������ΪӰ��
		for (int j=0;j<endnumbers;j++)
			resImgae[j*xsize*ysize+i]=res[j];

	}
	printf("\n finished...\n");

	GDALDatasetH m_datasetout;
	m_datasetout=GDALCreate(GDALGetDriverByName("GTiff"),pathRed,xsize,ysize,endnumbers,GDT_Float32,NULL);
	for (int i=0;i<endnumbers;i++)
		GDALRasterIO(GDALGetRasterBand(m_datasetout,i+1),GF_Write,0,0,xsize,ysize,resImgae+i*xsize*ysize,xsize,ysize,GDT_Float32,0,0);
	GDALClose(m_datasetout);

	//�������
	delete[]imgData;imgData=NULL;
	delete[]endData;endData=NULL;

	delete[]data;   data=NULL;
	delete[]res;	res=NULL;
	delete[]endTrans;endTrans=NULL;
	delete[]endMuti;endMuti=NULL;
	delete[]endInv; endInv=NULL;
	delete[]resImgae;resImgae=NULL;
	delete[]dendData;dendData=NULL;
	delete[]datamuti;datamuti=NULL;

	return ;
}

//�Խ���Ƚ��и�˹����ɢ��������
//�õ�������߹��׵ķ��
void endmember_sample(char* pathRed,char* pathSample)
{
	GDALAllRegister();
	GDALDatasetH m_dataset;
	m_dataset=GDALOpen(pathRed,GA_ReadOnly);
	
	//�ռ���ӦΪ��˹����
	CSampleFunc samplefunc;
	samplefunc.gauss_down_sample(pathRed,3,3,pathSample);

	return ;
}

//�������Ĳв� �������һ������Ϊ�ܲв�
void endmember_residual(char* pathimg,char* pathend,char* pathred,char* pathres,int endnumbers)
{
	GDALAllRegister();
	GDALDatasetH m_dataset   =GDALOpen(pathred,GA_ReadOnly);
	GDALDatasetH m_datasetimg=GDALOpen(pathimg,GA_ReadOnly);
	int xsize=GDALGetRasterXSize(m_dataset);
	int ysize=GDALGetRasterYSize(m_dataset);
	int bands=GDALGetRasterCount(m_dataset);

	float* datared=new float[xsize*ysize*bands];
	float* dataimg=new float[xsize*ysize*bands];
	float* dataend=new float[bands*endnumbers];
	float* dataout=new float[(bands+1)*xsize*ysize];
	memset(dataout,0,sizeof(float)*xsize*ysize*(bands+1));

	for(int i=0;i<bands;i++)
		GDALRasterIO(GDALGetRasterBand(m_dataset,i+1),GF_Read,0,0,xsize,ysize,datared+i*xsize*ysize,xsize,ysize,GDT_Float32,0,0);
	for(int i=0;i<bands;i++)
		GDALRasterIO(GDALGetRasterBand(m_datasetimg,i+1),GF_Read,0,0,xsize,ysize,dataimg+i*xsize*ysize,xsize,ysize,GDT_Float32,0,0);
	dataend=get_endmenber_spectralf(pathend,bands,endnumbers);
	GDALClose(m_dataset);
	GDALClose(m_datasetimg);

	for (int i=0;i<xsize*ysize;i++)
	{
		for (int j=0;j<bands;j++)
			for(int k=0;k<endnumbers;k++)
				dataout[j*xsize*ysize+i]+=datared[k*xsize*ysize+i]*dataend[j*endnumbers+k];

		//���һ������Ϊ�ܵĲв�
		for (int j=0;j<bands;j++)
			dataout[(bands)*xsize*ysize+i]+=dataout[j*xsize*ysize+i]-dataimg[j*xsize*ysize+i];
	}
	
	GDALDatasetH m_datasetout=GDALCreate(GDALGetDriverByName("GTiff"),pathres,xsize,ysize,bands+1,GDT_Float32,NULL);
	for(int i=0;i<bands+1;i++)
		GDALRasterIO(GDALGetRasterBand(m_datasetout,i+1),GF_Write,0,0,xsize,ysize,dataout+i*xsize*ysize,xsize,ysize,GDT_Float32,0,0);
	GDALClose(m_datasetout);

	delete[]dataend;
	delete[]dataimg;
	delete[]dataout;
	delete[]datared;
}

//������Ԫ����
void endmember_modify(char* pathred,char* pathimg,char* pathmodify)
{
	//ͨ����� ԭʼӰ���ԭʼ��Ԫ��ȡ�¶�Ԫ
	GDALAllRegister();
	GDALDatasetH m_datared=GDALOpen(pathred,GA_ReadOnly);
	GDALDatasetH m_dataimg=GDALOpen(pathimg,GA_ReadOnly);

	int xsizered=GDALGetRasterXSize(m_datared);
	int ysizered=GDALGetRasterYSize(m_datared);
	int bandsred=GDALGetRasterCount(m_datared);

	int xsizeimg=GDALGetRasterXSize(m_dataimg);
	int ysizeimg=GDALGetRasterYSize(m_dataimg);
	int bandsimg=GDALGetRasterCount(m_dataimg);

	//��ȡ����
	double *datared=new double[xsizered*ysizered*bandsred];
	double *dataimg=new double[xsizeimg*ysizeimg*bandsimg];
	for(int i=0;i<bandsimg;i++)
		GDALRasterIO(GDALGetRasterBand(m_dataimg,i+1),GF_Read,0,0,xsizeimg,ysizeimg,dataimg+i*xsizeimg*ysizeimg,xsizeimg,ysizeimg,GDT_Float64,0,0);
	for(int i=0;i<bandsred;i++)
		GDALRasterIO(GDALGetRasterBand(m_datared,i+1),GF_Read,0,0,xsizered,ysizered,datared+i*xsizered*ysizered,xsizered,ysizered,GDT_Float64,0,0);

	//��������
	double *dataend=new double[bandsred*bandsimg];
	memset(dataend,0,sizeof(double)*bandsred*bandsimg);

	double *dataredt=new double[xsizered*ysizered*bandsred];
	double *dataredm=new double[bandsred*bandsred];
	double *dataredI=new double[bandsred*bandsred];
	double *dataimgm=new double[bandsimg*bandsred];

	//������
	printf("�����������Ԫ����\n");
	MatrixTrans(datared,bandsred,xsizered*ysizered,dataredt);
	MatrixMuti(datared,bandsred,xsizered*ysizered,bandsred,dataredt,dataredm);
	MatrixMuti(dataimg,bandsimg,xsizered*ysizered,bandsred,dataredt,dataimgm);
	MatrixInverse(dataredm,bandsred,dataredI);
	MatrixMuti(dataimgm,bandsimg,bandsred,bandsred,dataredI,dataend);
	printf("finished...\n");

	//��Ԫ����
	export_endmenber_spectral(pathmodify,dataend,bandsimg,bandsred);

	//�����ڴ�ռ�
	delete[]dataend;
	delete[]dataredt;
	delete[]dataredm;
	delete[]dataredI;
	delete[]dataimgm;

	delete[]datared;
	delete[]dataimg;
}

//���ݷ����Ϣ��������Ķ�Ԫ��Ϣ����Ӱ���ؽ�
void endmember_recovery_image(char* pathred,char* pathmodend,char* pathout,int hyspectral)
{
	GDALAllRegister();
	GDALDatasetH m_datasetred=GDALOpen(pathred,GA_ReadOnly);

	int xsize=GDALGetRasterXSize(m_datasetred);
	int ysize=GDALGetRasterYSize(m_datasetred);
	int bands=GDALGetRasterCount(m_datasetred);

	//Ӱ�����ݿռ������
	float* datared=new float[xsize*ysize*bands];
	float* datamodend=new float[bands*hyspectral];
	float* datarecov =new float[xsize*ysize*hyspectral];
	memset(datarecov,0,sizeof(float)*xsize*ysize*hyspectral);

	for(int i=0;i<bands;i++)
		GDALRasterIO(GDALGetRasterBand(m_datasetred,i+1),GF_Read,0,0,xsize,ysize,datared+i*xsize*ysize,xsize,ysize,GDT_Float32,0,0);
	datamodend=get_endmenber_spectralf(pathmodend,hyspectral,bands);
	GDALClose(m_datasetred);


	//���ÿһ������
	for (int i=0;i<xsize*ysize;i++)
	{
		printf("\rԭʼͼ��ָ����أ�%d/%d",xsize*ysize,i+1);
		for (int j=0;j<hyspectral;j++)
		{
			for (int k=0;k<bands;k++)
			{
				datarecov[j*xsize*ysize+i]+=datared[k*xsize*ysize+i]*datamodend[j*bands+k];
			}
		}
	}
	printf("\n");

	//������
	GDALDatasetH m_dataset=GDALCreate(GDALGetDriverByName("GTiff"),pathout,xsize,ysize,hyspectral,GDT_Float32,NULL);
	for (int i=0;i<hyspectral;i++)
		GDALRasterIO(GDALGetRasterBand(m_dataset,i+1),GF_Write,0,0,xsize,ysize,datarecov+i*xsize*ysize,xsize,ysize,GDT_Float32,0,0);
	GDALClose(m_dataset);

	//�������
	delete[]datarecov;
	delete[]datared;
	delete[]datamodend;
}

//����Ӱ����׽ǲв�;���ֵ��
void endmember_residual_spAngle(char* pathrecovery,char* pathOri,char* pathresidual)
{
	residual_spAngle(pathrecovery,pathOri,pathresidual);
}