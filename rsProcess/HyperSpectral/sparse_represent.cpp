#include "fusionfunc.h"
#include <vector>
#include "..\matrixOperation.h"
#include "..\rsImgProcess\SampleFunc.h"
#include "..\gdal\include\gdal_priv.h"
using namespace std;
#pragma comment(lib,"gdal_i.lib")
//�ڶ�Ԫ��Ŀ���ڲ�����Ŀ�������ʹ��Ӱ���ϡ���ʾ���

//��ȡϵ����ʾ��Ŀ
int sparse_represent_number(int endnumber,int saprse)
{
	int number=1;
	//��ϵĹ�ʽ һ���������
	for (int i=0;i<saprse;i++)
		number*=(endnumber-i);
	for (int i=1;i<=saprse;i++)
		number/=i;
	return number;
}
//����в�
double sparse_represent_residual(double* repdata,double* pixel,int numbands)
{
	double temp=0;
	for (int i=0;i<numbands;i++)
		temp+=abs(repdata[i]-pixel[i]);
	return temp;
}

//��ȡ���е�ϡ�����
//�����㷨�����ѵ� ��ȡϡ�����
void _combination(int* comb,int numsel,vector<int> &combvec,vector<int> &out)
{
	if(numsel==0)
	{
		for (int i=0;i<int(combvec.size());i++)
			out.push_back(combvec[i]);
		return ;
	}
	if((*comb)==-1)
	{
		return ;
	}
	int temp=*comb;
	combvec.push_back(temp);
	_combination(comb+1,numsel-1,combvec,out);
	combvec.pop_back();
	_combination(comb+1,numsel,combvec,out);
}
void combination(int* comb,int numsel,vector<int> &out)
{
	vector<int> combvec;
	/*numsel--;*/	//��0��ʼ ���Լ�һ
	_combination(comb,numsel,combvec,out);
	combvec.clear();
}

void sparse_represent_combine(int numsparse,int selsparse,int endnumber,int* selindex)
{
	int* endmemberIndex=new int[endnumber+1];
	for (int i=0;i<endnumber;i++)
		endmemberIndex[i]=i;
	endmemberIndex[endnumber]=-1;

	//��ȡ�������
	vector<int> comb;
	combination(endmemberIndex,selsparse,comb);

	for (int i=0;i<numsparse*selsparse;i++)
		selindex[i]=comb[i];

	delete[]endmemberIndex;
	comb.clear();
	return ;
	
}

//��ÿһ������������ϡ���ʾ��ѡȡһ������ʵĻ�ȡ������
void sparse_represent_fitness(float* endmemberdata,double* pixeldata,int* spareIdx,float* result,
														int numspare,int selsparse,int bands,int endnum)
{
	//���ݿռ�����
	double* enddata=new double[selsparse*bands];
	double* fitness=new double[selsparse];
	double* enddataT=new double[bands*selsparse];
	double* enddataM=new double[selsparse*selsparse];
	double* enddataI=new double[selsparse*selsparse];
	double* endfitness=new double[selsparse];

	double* pixelM  =new double[selsparse];
	double* pixelR  =new double[bands];
	double* pixeltemp=new double[bands];				//ѡȡ����ʵ�ϡ�����
	
	memset(enddata,0,sizeof(double)*selsparse*bands);
	memset(pixeltemp,0,sizeof(double)*bands);

	double fitresi=0;
	int intfitness=0;

	for (int i=0;i<numspare;i++)
	{
		//ѡȡ���
		for (int j=0;j<bands;j++)
			for (int k=0;k<selsparse;k++)
				enddata[j*selsparse+k]=endmemberdata[j*endnum+spareIdx[i*selsparse+k]];
		
		MatrixTrans(enddata,bands,selsparse,enddataT);
		MatrixMuti(enddataT,selsparse,bands,selsparse,enddata,enddataM);
		MatrixMuti(enddataT,selsparse,bands,1,pixeldata,pixelM);
		MatrixInverse(enddataM,selsparse,enddataI);
		MatrixMuti(enddataI,selsparse,selsparse,1,pixelM,endfitness);

		//����Ӱ��
		MatrixMuti(enddata,bands,selsparse,1,endfitness,pixelR);

		//������в�
		fitresi=sparse_represent_residual(pixeldata,pixelR,bands);
		//�������ԭʼ���
		if(fitresi<sparse_represent_residual(pixeldata,pixeltemp,bands))
		{
			memcpy(fitness,endfitness,sizeof(double)*selsparse);
			memcpy(pixeltemp,pixelR,sizeof(double)*bands);
			intfitness=i;
		}
	}


	//���ս��
	memset(result,0,sizeof(float)*endnum);
	for (int i=0;i<selsparse;i++)
		result[spareIdx[intfitness*selsparse+i]]=fitness[i];

	//ɾ���������ݿռ�
	delete[]enddata;enddata=NULL;
	delete[]fitness;fitness=NULL;
	delete[]enddataT;enddataT=NULL;
	delete[]enddataM;enddataM=NULL;
	delete[]enddataI;enddataI=NULL;
	delete[]endfitness;endfitness=NULL;
	delete[]pixelM;pixelM=NULL;
	delete[]pixelR;pixelR=NULL;
	delete[] pixeltemp;pixeltemp=NULL;

	return ;

}
//���������С����
void sparse_GeneralizedInverseLES(char* pathEnd, char* pathImg, char* pathRed, int endnumber, int bands)
{
	//��ȡ��Ԫ
	float* endMembers = new float[endnumber*bands];
	float* endMemberT = new float[endnumber*bands];
	float* endInverse = new float[endnumber*bands];
	endMembers = get_endmenber_spectralf(pathEnd, bands, endnumber);

	//������
	MatrixTrans(endMembers,  endnumber, bands, endMemberT);
	Matrix_GenInverse(endMemberT, bands, endnumber, endInverse);

	//��ȡͼ��
	GDALAllRegister();
	GDALDatasetH m_dataset = GDALOpen(pathImg, GA_ReadOnly);
	int xsize = GDALGetRasterXSize(m_dataset);
	int ysize = GDALGetRasterYSize(m_dataset);
	//int bands = GDALGetRasterCount(m_dataset);

	//�����ڴ�����Ҫtryһ�µ� ���ø��......@_@
	float* dataimg = new float[xsize*ysize*bands];
	float* reduimg = new float[xsize*ysize*endnumber];
	for (int i = 0; i < bands; ++i)
		GDALRasterIO(GDALGetRasterBand(m_dataset, i + 1), GF_Read, 0, 0, xsize, ysize, dataimg + i*xsize*ysize, xsize, ysize, GDT_Float32, 0, 0);

	//���
	float* tempdata1 = new float[bands];
	float* tempdata2 = new float[endnumber];
	for (int i = 0; i < xsize*ysize; ++i)
	{
		printf("process pixel %d\r", i + 1);
		for (int j = 0; j < bands; ++j)
			tempdata1[j] = dataimg[j*xsize*ysize + i];
		MatrixMuti(endInverse, endnumber, bands, 1, tempdata1, tempdata2);
		for (int j = 0; j < endnumber; ++j)
			reduimg[j*xsize*ysize + i] = tempdata2[j];
	}
	printf("\n");
	//���ͼ��
	GDALDatasetH m_datasetDst = GDALCreate(GDALGetDriverByName("GTiff"), pathRed, xsize, ysize, endnumber, GDT_Float32, NULL);
	for (int i = 0; i < endnumber; ++i)
		GDALRasterIO(GDALGetRasterBand(m_datasetDst, i + 1), GF_Write, 0, 0, xsize, ysize, reduimg + i*xsize*ysize, xsize, ysize, GDT_Float32, 0, 0);
	GDALClose(m_dataset);
	GDALClose(m_datasetDst);

	delete[]endMembers;
	delete[]endMemberT;
	delete[]endInverse;
	delete[]dataimg;
	delete[]reduimg;
	delete[]tempdata1;
	delete[]tempdata2;
}

//ϡ���ʾ��Ӱ����С�������
void sparse_represent_LES(char* pathEnd,char* pathImg,char* pathRed,int endnumbers,int sparsesel)
{
	////
	int numsparse=sparse_represent_number(endnumbers,sparsesel);
	int *sparseIndex=new int[numsparse*sparsesel];
	sparse_represent_combine(numsparse,sparsesel,endnumbers,sparseIndex);

	//��Ӱ��
	GDALAllRegister();
	GDALDatasetH m_dataset=GDALOpen(pathImg,GA_ReadOnly);
	int xsize=GDALGetRasterXSize(m_dataset);
	int ysize=GDALGetRasterYSize(m_dataset);
	int bands=GDALGetRasterCount(m_dataset);

	float *dataimg=new float[xsize*ysize*bands];
	float *datared=new float[xsize*ysize*endnumbers];
	memset(datared,0,sizeof(float)*xsize*ysize*endnumbers);
	for(int i=0;i<bands;i++)
		GDALRasterIO(GDALGetRasterBand(m_dataset,i+1),GF_Read,0,0,xsize,ysize,dataimg+i*xsize*ysize,xsize,ysize,GDT_Float32,0,0);
	GDALClose(m_dataset);

	double *pixels=new double[bands];
	float* enddata=new float[endnumbers*bands];
	float* fitness=new float[endnumbers];

	enddata=get_endmenber_spectralf(pathEnd,bands,endnumbers);

	for (int i=0;i<xsize*ysize;i++)
	{
		printf("\rϡ���ʾ���������أ�%d/%d",xsize*ysize,i+1);
		for (int j=0;j<bands;j++)
			pixels[j]=dataimg[j*xsize*ysize+i];
		sparse_represent_fitness(enddata,pixels,sparseIndex,fitness,numsparse,sparsesel,bands,endnumbers);
		for (int j=0;j<endnumbers;j++)
			datared[j*xsize*ysize+i]=fitness[j];
	}
	printf("\n");
	//���������
	GDALDatasetH m_datasetout=GDALCreate(GDALGetDriverByName("GTiff"),pathRed,xsize,ysize,endnumbers,GDT_Float32,NULL);
	for (int i=0;i<endnumbers;i++)
		GDALRasterIO(GDALGetRasterBand(m_datasetout,i+1),GF_Write,0,0,xsize,ysize,datared+i*xsize*ysize,xsize,ysize,GDT_Float32,0,0);
	GDALClose(m_datasetout);

	//���ָ��
	delete[]dataimg;dataimg=NULL;
	delete[]datared;datared=NULL;
	delete[]pixels;pixels=NULL;
	delete[]fitness;fitness=NULL;
	delete[]enddata;enddata=NULL;
	delete[]sparseIndex;sparseIndex=NULL;
}

//��Ԫ�����ڿռ��Ͻ��и�˹�ز���
void sparse_represent_sample(char* pathred,char* pathredSmp)
{
	GDALAllRegister();
	GDALDatasetH m_dataset;
	m_dataset=GDALOpen(pathred,GA_ReadOnly);

	//�ռ���ӦΪ��˹����  ��˹�˵Ĵ�СΪ����
	CSampleFunc samplefunc;
	samplefunc.gauss_down_sample(pathred,3,3,pathredSmp);

	return ;
}

//����Ӱ��Ķ�Ԫ����
void sparse_represent_modify(char* pathred,char* pathimg,char* pathmodify)
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

//ͨ��ϡ���ʾ��Ӱ��ḻ����Ӱ��ָ�
void sparse_represent_recovery(char* pathred,char* pathEnd,char* pathout,int recbands)
{
	GDALAllRegister();
	GDALDatasetH m_datasetin=GDALOpen(pathred,GA_ReadOnly);
	
	int xsize=GDALGetRasterXSize(m_datasetin);
	int ysize=GDALGetRasterYSize(m_datasetin);
	int endnum=GDALGetRasterCount(m_datasetin);

	float* dataend=new float[recbands*endnum];
	float* datarec=new float[xsize*ysize*recbands];
	float* datared=new float[xsize*ysize*endnum];
	memset(datarec,0,sizeof(float)*xsize*ysize*recbands);

	for (int i=0;i<endnum;i++)
		GDALRasterIO(GDALGetRasterBand(m_datasetin,i+1),GF_Read,0,0,xsize,ysize,datared+i*xsize*ysize,xsize,ysize,GDT_Float32,0,0);
	GDALClose(m_datasetin);
	dataend=get_endmenber_spectralf(pathEnd,recbands,endnum);

	//����ÿһ���ָ�������
	for (int i=0;i<xsize*ysize;i++)
	{
		printf("\r�ָ�Ӱ�����أ�%d/%d",xsize*ysize,i+1);
		for (int j=0;j<recbands;j++)
			for (int k=0;k<endnum;k++)
				datarec[j*xsize*ysize+i]+=datared[k*xsize*ysize+i]*dataend[j*endnum+k];
	}
	printf("\n");
	//���������
	GDALDatasetH m_datasetout=GDALCreate(GDALGetDriverByName("GTiff"),pathout,xsize,ysize,recbands,GDT_Float32,NULL);
	for (int i=0;i<recbands;i++)
		GDALRasterIO(GDALGetRasterBand(m_datasetout,i+1),GF_Write,0,0,xsize,ysize,datarec+i*xsize*ysize,xsize,ysize,GDT_Float32,0,0);
	GDALClose(m_datasetout);

	delete[]dataend;
	delete[]datarec;
	delete[]datared;
}

//����ָ����׵Ĺ��ײв�
void sparse_represent_residual_spAngle(char* pathrecovery,char* pathOri,char* pathresidual)
{
	residual_spAngle(pathrecovery,pathOri,pathresidual);
}