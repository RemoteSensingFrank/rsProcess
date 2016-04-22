#include "HyperSpectural.h"
#include <memory>
#include <fstream>
#include <algorithm>

#include "..\Global.h"
#include "..\gdal/include/gdal_priv.h"
#include "..\matrixOperation.h"
using namespace std;

#pragma comment(lib,"gdal_i.lib")
#pragma warning(disable : 4996)

//============================================================================
// Name        : HyperSpectural.cpp
// Author      : ww
// Version     : 2.0
// Copyright   : ww
// Description : �߹���Ӱ����ͶӰ�ռ�任��
//============================================================================
//[10/18/2014 wuwei just you]
// Version	   :2.0  1.�����˸߹���ͼ��RXD�㷨��2.������MNF�㷨
// Version	   :2.1  1.������FastICA�㷨

// Version	   :2.2  1.�����˿��ٵ������Ԫ��ȡ FastFinder�㷨
//					 2.�����˹��׽Ǿ�����
//					 3.�����˶�Ԫ�������
//  [1/29/2015 wuwei just you]
// Version	   :2.2.1 1.���Ӳ��Ժ����������д���
//  [2/7/2015 wuwei just you]
//  Modified by w.w.Frank
//  [4/2/2015 ygoffice]
// Version	   :2.2.2 1.����ͶӰ�任�Ĵ���
//					  2.���Ӷ�ȡENVI��ROI����Ĵ���
//  Modified by w.w.Frank

//�ļ�����
/*------------------------������д���ļ���------------------------------*/
long Matrix_Export_File(const char* fileName,double **matrix,int size)
{
	int i,j;			//ѭ������
	ofstream outFile;	//�����ļ�
	if(fileName==NULL||matrix==NULL)
		return 1;

	//���ļ��ļ�
	outFile.open(fileName,ios_base::binary);
	if(!outFile.is_open())
		return 3;

	//��һ��Ϊ����Ĵ�С
	/*outFile.write((char*)&size,sizeof(size));*/
	outFile<<size<<"  "<<endl;;

	//������д���ļ�
	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			/*outFile.write((char*)&matrix[i][j],sizeof(matrix[i][j]));*/
			outFile<<matrix[i][j]<<"   ";
		}
		outFile<<endl;
	}
	outFile.close();

	if(!outFile.is_open())
		return 0;
	else
		return 3;
}

/*--------------------------��þ���Ĵ�С------------------------------*/
long Matrix_Get_Size(const char* fileName,int &size)
{
	//int i,j;			//ѭ����������
	ifstream inFile;
	size=0;				//��ʼֵ
	if(fileName==NULL)
		return 1;
	inFile.open(fileName,ios_base::in||ios_base::binary);
	if(!inFile.is_open())
		return 3;
	/*inFile.read((char*)&size,sizeof(int));*/
	inFile>>size;
	if(size>1)
		return 0;
	else
		return 3;

}

/*-----------------------���ļ��ж����任����---------------------------*/
long Matrix_Import_File(const char* fileName,double **matrix)
{

	int i,j;			//ѭ����������
	ifstream inFile;
	int size;
	if(fileName==NULL||matrix==NULL)
		return 1;

	inFile.open(fileName,ios_base::in||ios_base::binary);
	if(!inFile.is_open())
		return 3;
	/*inFile.read((char*)&size,sizeof(int));*/
	inFile>>size;
	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			/*inFile.read((char*)&matrix[i][j],sizeof(double));*/
			inFile>>matrix[i][j];
		}
	}

	inFile.close();
	if(!inFile.is_open())
		return 0;
	else
		return 3;
}

/*--------------------------��Ԫ������ļ���----------------------------*/
long EndMember_Export_File(const char* fileName,double *endmember,int bands,int numbers)
{
	ofstream ofs;
	ofs.open(fileName);
	if(!ofs.is_open())
		return -1;

	//�ļ�ͷ
	time_t curtime;
	curtime=time(NULL);
	ofs<<"ENVI ASCII Plot File"<<"["<<ctime(&curtime)<<"]"<<endl;
	ofs<<"wavelength"<<endl;
	for(int i=0;i<numbers;i++)
		ofs<<"class:"<<i+1<<endl;
	//�������
	for (int i=0;i<bands;i++)
	{
		ofs<<i+1<<" ";
		for (int j=0;j<numbers;j++)
		{
			ofs<<endmember[j*bands+i]<<" ";
		}
		ofs<<endl;
	}
	ofs.close();
	return 0;
}
/*--------------------------���ļ��л�ȡ��Ԫ----------------------------*/
long EndMember_Import_File(const char* fileName,double *endmember,int bands,int numbers)
{
	ifstream ifs;
	ifs.open(fileName,ios_base::in);
	if(ifs.is_open()!=true)
		return NULL;
	char tempchr[2048];
	for(int i=0;i<numbers+2;i++)
		ifs.getline(tempchr,2048);

	for (int i=0;i<bands;i++)
	{
		double temp;
		ifs>>temp;
		for (int j=0;j<numbers;j++)
		{
			ifs>>temp;
			endmember[i*numbers+j]=temp/10;
		}
	}
	return 0;
}

/*----------------------��ȡENVI����ĸ���Ȥ����------------------------*/
long  EnviROINumbers(const char* roiFile,int &roiNumbers)
{
	ifstream ifs(roiFile,ios_base::in);
	char szTemp[2048];memset(szTemp,0,sizeof(char)*2048);
	char sztmp[10];	  memset(sztmp,0,sizeof(char)*10);
	ifs.getline(szTemp,2048);
	memset(szTemp,0,sizeof(char)*2048);
	ifs.getline(szTemp,2048);
	
	for (int i=18,j=0;i<28;i++)
	{
		if(szTemp[i]!='\n')
		{
			sztmp[j]=szTemp[i];
			++j;
		}
	}
	roiNumbers=atoi(sztmp);
	ifs.close();
	return 0;
}

long  EnviROIPts(const char* roiFile,int* roiPts,int roiNmbers)
{
	ifstream ifs(roiFile,ios_base::in);
	char szTemp[2048];memset(szTemp,0,sizeof(char)*2048);
	char sztmp[10];	  memset(sztmp,0,sizeof(char)*10);
	ifs.getline(szTemp,2048);
	ifs.getline(szTemp,2048);
	ifs.getline(szTemp,2048);
	ifs.getline(szTemp,2048);
	memset(szTemp,0,sizeof(char)*2048);

	for (int k=0;k<roiNmbers;k++)
	{
		ifs.getline(szTemp,2048);
		ifs.getline(szTemp,2048);
		memset(szTemp,0,sizeof(char)*2048);
		ifs.getline(szTemp,2048);
		for (int i=12,j=0;i<22;i++)
		{
			if(szTemp[i]!='\n')
			{
				sztmp[j]=szTemp[i];
				++j;
			}
		}
		roiPts[k]=atoi(sztmp);
		memset(sztmp,0,sizeof(char)*10);
		ifs.getline(szTemp,2048);
	}
	return 0;
}

long  EnviROIData(const char* roiFile,int* roiPts,int roiNumbers,int bands,double** roiData)
{
	ifstream ifs(roiFile,ios_base::in);
	char szTemp[2048];memset(szTemp,0,sizeof(char)*2048);
	char sztmp[10];	  memset(sztmp,0,sizeof(char)*10);
	for (int i=0;i<(roiNumbers+1)*4;i++)
		ifs.getline(szTemp,2048);
	int totalnum=0;
	for(int i=0;i<roiNumbers;i++)
		totalnum+=roiPts[i];
	double* data=new double[bands*totalnum];
	for (int i=0,k=0;i<roiNumbers;i++)
	{
		for (int j=0;j<roiPts[i];j++,k++)
		{
			double temp;
			ifs>>temp;ifs>>temp;ifs>>temp;
			for (int l=0;l<bands;l++)
				ifs>>data[k*bands+l];
		}
		ifs.getline(szTemp,2048);
	}
	ifs.close();

	//BSQ�洢��ʽ
	for (int i=0,l=0;i<roiNumbers;i++,l+=roiPts[i])
		for (int k=0;k<bands;k++)
			for (int j=0;j<roiPts[i];j++)
				roiData[i][roiPts[i]*k+j]=data[(l+j)*bands+k];
	delete[]data;
	return 0;

}

/********************************************************************************************************/
/*											Ӱ��ͳ����                                                  */
/********************************************************************************************************/
//������׽Ǿ���
long Spectral_Angle_Distance(double* data1,double* data2,int bands,double &angle)
{
	//�������
	if(data1==NULL||data2==NULL)
		return -1;

	//������׽�
	double tempa1=0,tempa2=0,tempa3=0;
	for(int i=0;i<bands;i++)
	{
		tempa1=data1[i]*data1[i];
		tempa2=data2[i]*data2[i];
		tempa3=data1[i]*data2[i];
	}
	angle=tempa3/sqrt(tempa1)/sqrt(tempa2);

	return 0;
}

long Spectral_Angle_Distance(double* imgBuffer,int xsize,int ysize,int xpos,int ypos,double *data,int bands,double& angle)
{
	//�������
	long lError=0;
	if(imgBuffer==NULL||data==NULL)
		return -1;
	double* data2=new double[bands];
	for (int i=0;i<bands;i++)
	{
		data2[i]=imgBuffer[i*xsize*ysize+ypos*xsize+xpos];
	}
	lError=Spectral_Angle_Distance(data,data2,bands,angle);
	if(lError!=0)
		return lError;
	else
		return 0;
}

//ͼ����������ں�
void Layerstacking(char *pathIn, int begNum,int endNum,char* pathOut)
{
	//�������ݼ�
	GDALAllRegister();
	GDALDatasetH m_datasetIn =GDALOpen(pathIn,GA_ReadOnly);
	if(m_datasetIn==NULL)
		return;

	int x=GDALGetRasterXSize(m_datasetIn);
	int y=GDALGetRasterYSize(m_datasetIn);
	const char* proj=GDALGetProjectionRef(m_datasetIn);
	GDALClose(m_datasetIn);

	//����ļ���
	char driveIn[_MAX_DRIVE];
	char dirIn[_MAX_DIR];
	char fnameIn[_MAX_FNAME];
	char extIn[_MAX_EXT];

	char sName[280];
	char tempIn[10];

	errno_t    err;
	err = _splitpath_s( pathIn, driveIn,_MAX_DRIVE, dirIn, _MAX_DIR, fnameIn,_MAX_FNAME, extIn, _MAX_EXT );
	if (err)
		return ;

	double* data=new double[x*y];
	int bands=0;

	//��ȡ������
	for (int i=begNum;i<=endNum;i++)
	{
		sprintf(tempIn,"%d",i);
		err = _makepath_s(sName, _MAX_PATH, driveIn,dirIn,tempIn,extIn);
		if (err)
			return ;

		//��ȡӰ������
		m_datasetIn=GDALOpen(sName,GA_ReadOnly);
		int tempy=GDALGetRasterYSize(m_datasetIn);
		int tempbands=GDALGetRasterCount(m_datasetIn);
		bands+=tempbands;
		GDALClose(m_datasetIn);
	}
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	GDALDatasetH m_datasetOut=GDALCreate(GDALGetDriverByName("HFA"),pathOut,x,y,bands,GDT_Float64,papszOptions);
	int  bandtempNum=0;
	for (int i=begNum;i<=endNum;i++)
	{
		sprintf(tempIn,"%d",i);
		err = _makepath_s(sName, _MAX_PATH, driveIn,dirIn,tempIn,extIn);
		if (err)
			return ;

		//��ȡӰ������
		m_datasetIn=GDALOpen(sName,GA_ReadOnly);
		int tempx=GDALGetRasterXSize(m_datasetIn);
		int tempy=GDALGetRasterYSize(m_datasetIn);
		int tempband=GDALGetRasterCount(m_datasetIn);

		if(tempx!=x||tempy!=y)
			return;

		memset(data,0,sizeof(double)*x*y);
		for (int j=0;j<tempband;j++)
		{
			GDALRasterIO(GDALGetRasterBand(m_datasetIn,j+1),GF_Read,0,0,x,y,data,x,y,GDT_Float64,0,0);
			GDALRasterIO(GDALGetRasterBand(m_datasetOut,j+bandtempNum+1),GF_Write,0,0,x,y,data,x,y,GDT_Float64,0,0);
		}
		bandtempNum+=tempband;
		GDALClose(m_datasetIn);
	}

	GDALSetProjection(m_datasetOut,proj);
	GDALClose(m_datasetOut);
	delete[]data;
}

//ͳ�ƹ�������Ϊ����Ӱ��ֵ������Чֵ���쳣ֵ��Ҫ��ǰ���д���
/*-----------------------------��ȡӰ�����-----------------------------*/
long GetImageEntropy(double *imgBuffer,int bandSum,int* bandNo,int xsize,
	int ysize,double* entropy)
{
	if (imgBuffer==NULL||entropy==NULL)
		return 1;
	if(xsize<=0||ysize<=0||bandSum<=0)
		return 2;
	memset(entropy,0,sizeof(double)*bandSum);
	//��ȡͼ��������Сֵ
	double maxnum,minnum;
	for (int i=0;i<bandSum;i++)
	{
		maxnum=minnum=imgBuffer[i*xsize*ysize];
		for (int k=0;k<xsize*ysize;k++)
		{
			//��ȡ���ε������Сֵ
			maxnum=max(maxnum,imgBuffer[i*xsize*ysize+k]);
			minnum=max(minnum,imgBuffer[i*xsize*ysize+k]);
		}
		//��ȡ�Ҷȼ���
		int  graylevel=int(maxnum-minnum);
		int* histrogram=new int[graylevel];memset(histrogram,0,sizeof(int)*graylevel);
		double* histroRato=new double[graylevel];memset(histroRato,0,sizeof(double)*graylevel);

		for (int k=0;k<xsize*ysize;k++)
		{
			histrogram[int(imgBuffer[i*xsize*ysize+k])]++;
		}
		for (int k=0;k<graylevel;k++)
		{
			histroRato[k]=double(histrogram[k])/double(xsize)/double(ysize);
			entropy[i]+=histroRato[k]*log(histroRato[k]);
		}
	}
	return 0;
}

/*-----------------------------��ȡ����Ӱ��-----------------------------*/
double* GetpartImage(double* imgBuffer,int bands,int imgxsize,int imgysize,int xpos,int ypos,int xsize,int ysize)
{
	if(xpos+xsize>imgxsize||ypos+ysize>imgysize)
		return NULL;
	double* imgpart=NULL;
	imgpart=new double[xsize*ysize*bands];
	if(imgpart==NULL)
		return imgpart;
	for(int j=0;j<bands;j++)
	{
		for (int i=0;i<ysize;i++)
		{
			memcpy(imgpart+i*xsize+j*xsize*ysize,imgBuffer+(i+ypos)*imgxsize+xpos+j*imgxsize*imgysize,sizeof(double)*xsize);
		}
	}

	return imgpart;
}

/*-----------------------------����Ӱ�񲨶εľ�ֵ-----------------------*/
long GetImgMean(double *imgBuffer,int bandSum,int* bandNo,int xsize,int ysize,double* meanValue)
{
	int i,j,k;		//��������
	double tempValue=0;

	if (imgBuffer==NULL||meanValue==NULL)
		return 1;
	if(xsize<=0||ysize<=0||bandSum<=0)
		return 2;

	//��ʼ��
	memset(meanValue,0,sizeof(double)*bandSum);
	for (i=0;i<bandSum;i++)
	{
		for (j=0;j<ysize;j++)
		{
			for (k=0;k<xsize;k++)
			{
				tempValue+=imgBuffer[i*xsize*ysize+j*xsize+k];
			}
			meanValue[i]+=tempValue/xsize;
			tempValue=0;
		}
		meanValue[i]/=ysize;
	}

	//������ɷ��سɹ�
	return 0;
}

/*-------------------------------����Ӱ��ı�׼��-----------------------*/
long GetImgStdCovarianve(double *imgBuffer,int bandSum,int* bandNo,
	int xsize,int ysize,double *meanValue,double* stdv)
{
	int i,j,k;		//��������

	if (imgBuffer==NULL||meanValue==NULL||stdv==NULL)
		return 1;
	if(xsize<=0||ysize<=0||bandSum<=0)
		return 2;

	//��ʼ��
	memset(stdv,0,sizeof(double)*bandSum);
	for (i=0;i<bandSum;i++)
	{
		for (j=0;j<ysize;j++)
		{
			for (k=0;k<xsize;k++)
			{
				stdv[i]+=pow(imgBuffer[i*xsize*ysize+j*xsize+k]-meanValue[i],2.0);
			}
		}
		stdv[i]/=(xsize*ysize);
		stdv[i]=sqrt(stdv[i]);
	}

	//������ɷ��سɹ�
	return 0;
}

/*--------------------------���㲨�μ��Э�������----------------------*/
long GetCovarianceMatrix(double *imgBuffer,int bandSum,int* bandNo,
                                int xsize,int ysize,double** covarianceMat)
{
	int i,j,k,l;				//��������
	double tempAvg=0.0f;
	double* avgVector=NULL;		//ÿ�����εľ�ֵ
	double temcov=0.0f;

	if (imgBuffer==NULL||covarianceMat==NULL)
		return 1;
	if(xsize<=0||ysize<=0||bandSum<=0)
		return 2;

	//����ÿ������Ӱ��ľ�ֵ
	if(NULL==(avgVector=new double[bandSum]))
		return 3;
	memset(avgVector,0,sizeof(double)*bandSum);
	for(i=0;i<bandSum;i++)
	{
		for(j=0;j<ysize;j++)
		{
			for(k=0;k<xsize;k++)
				tempAvg+=imgBuffer[i*xsize*ysize+j*xsize+k];
			avgVector[i]+=tempAvg/xsize;
			tempAvg=0;
		}
		avgVector[i]/=ysize;
	}

	for (i=0;i<bandSum;i++)
		memset(covarianceMat[i],0,sizeof(double)*bandSum);	//��ʼ��Э�������

	//����Э�������
	for (i=0;i<bandSum;i++)
	{
		for(j=i;j<bandSum;j++)
		{
			for(k=0;k<ysize;k++)
			{
				//Э�������Ϊ�Գƾ���
				for (l=0;l<xsize;l++)
				{
					temcov+=(imgBuffer[(k*xsize+l)+i*xsize*ysize]-avgVector[i])*
						(imgBuffer[(k*xsize+l)+j*xsize*ysize]-avgVector[j]);
				}
				covarianceMat[i][j]+=temcov/xsize;
				temcov=0.0f;
			}
			covarianceMat[j][i]=covarianceMat[i][j]=covarianceMat[i][j]/ysize;
			//covarianceMat[j][i]=covarianceMat[i][j]=covarianceMat[i][j]*xsize*ysize/(xsize*ysize-1);
		}
	}

	//���ָ��
	if (avgVector==NULL)
		return 3;
	else
		delete []avgVector;avgVector=NULL;

	//�ɹ�ִ�� ����0
	return 0;
}

/*-------------------------------��������Ӱ��----------------------------*/
long GetNoiseImg(double *imgBuffer,int bandSum,int* bandNo, int sampleX,
                                int sampleY,int xsize,int ysize,double* imgNoise)
{
	int i,j,k;				//��������

	if(imgBuffer==NULL||imgNoise==NULL)
		return 1;
	if (bandSum<=0||sampleX<=0||sampleY<=0||xsize<=0||ysize<=0)
		return 2;

	//ÿ�����μ��������
	for (i =0;i<bandSum;i++)
	{
		for (j=0;j<ysize;j++)
		{
			for (k=0;k<xsize;k++)
			{
				//�жϼ��ϲ��������Ƿ񳬹�Ӱ��Ĵ�С
				//������Ӱ���С��ȥ�ԳƷ����ϵ�����
				if (j+sampleY>=ysize&&k+sampleX<xsize)
				{
					imgNoise[i*xsize*ysize+j*xsize+k]=abs(imgBuffer[i*xsize*ysize+j*xsize+k]-
						imgBuffer[i*xsize*ysize+(j-sampleY)*xsize+k+sampleX]);
				}
				else if(k+sampleX>=xsize&&j+sampleY<ysize)
				{
					imgNoise[i*xsize*ysize+j*xsize+k]=abs(imgBuffer[i*xsize*ysize+j*xsize+k]-
						imgBuffer[i*xsize*ysize+(j+sampleY)*xsize+k-sampleX]);
				}
				else if(k+sampleX>=xsize&&j+sampleY>=ysize)
				{
					imgNoise[i*xsize*ysize+j*xsize+k]=abs(imgBuffer[i*xsize*ysize+j*xsize+k]-
						imgBuffer[i*xsize*ysize+(j-sampleY)*xsize+k-sampleX]);
				}
				else
				{
					imgNoise[i*xsize*ysize+j*xsize+k]=abs(imgBuffer[i*xsize*ysize+j*xsize+k]-
						imgBuffer[i*xsize*ysize+(j+sampleY)*xsize+k+sampleX]);
				}
			}
		}
	}

	//������ɷ��سɹ�
	return 0;
}
/*-------------------------------ĳһ�к�����Ӱ��------------------------*/
long  GetCutLineImg(const char*pathIn,int lineIdx,const char* pathOut)
{
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO" );	//����·��
	GDALDatasetH m_datasetIn;
	m_datasetIn=GDALOpen(pathIn,GA_ReadOnly);
	int xsize=GDALGetRasterXSize(m_datasetIn);
	int ysize=GDALGetRasterYSize(m_datasetIn);
	int bands=GDALGetRasterCount(m_datasetIn);
	if(lineIdx>ysize)
		return 1;
	GDALDatasetH m_datasetOut;
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	m_datasetOut=GDALCreate(GDALGetDriverByName("GTiff"),pathOut,xsize,bands,1,GDT_Float32,papszOptions);
	float *data=new float[xsize*bands];
	for (int i=0;i<bands;i++)
		GDALRasterIO(GDALGetRasterBand(m_datasetIn,i+1),GF_Read,0,lineIdx,xsize,1,data+i*xsize,xsize,1,GDT_Float32,0,0);
	GDALClose(m_datasetIn);
	GDALRasterIO(GDALGetRasterBand(m_datasetOut,1),GF_Write,0,0,xsize,bands,data,xsize,bands,GDT_Float32,0,0);
	GDALClose(m_datasetOut);
	delete[]data;
	return 0;
}
long  GetCutLineImg(const char*pathIn,const char* path_plant_coeff,int lineIdx,const char* pathOut)
{
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO" );	//����·��
	GDALDatasetH m_datasetIn;
	m_datasetIn=GDALOpen(pathIn,GA_ReadOnly);
	int xsize=GDALGetRasterXSize(m_datasetIn);
	int ysize=GDALGetRasterYSize(m_datasetIn);
	int bands=GDALGetRasterCount(m_datasetIn);
	if(lineIdx>ysize)
		return 1;
	GDALDatasetH m_datasetOut;
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	m_datasetOut=GDALCreate(GDALGetDriverByName("GTiff"),pathOut,xsize,bands,1,GDT_Float32,papszOptions);

	float *data=new float[xsize*bands];
	float* plankData=new float[2*bands];
	char tmpchr[2048];

	ifstream ifs;
	ifs.open(path_plant_coeff);
	if(!ifs.is_open())
		return 2;
	ifs.getline(tmpchr,2048);
	for (int i=0;i<bands;i++)
	{
		ifs.getline(tmpchr,2048);
		sscanf(tmpchr,"%f%f",&plankData[2*i+0],&plankData[2*i+1]);
	}
	ifs.close();

	for (int i=0;i<bands;i++)
	{
		GDALRasterIO(GDALGetRasterBand(m_datasetIn,i+1),GF_Read,0,lineIdx,xsize,1,data+i*xsize,xsize,1,GDT_Float32,0,0);
		for (int j=0;j<xsize;j++)
		{
			data[i*xsize+j]=data[i*xsize+j]*plankData[2*i+1]+plankData[2*i+0];
		}
	}
	GDALClose(m_datasetIn);
	GDALRasterIO(GDALGetRasterBand(m_datasetOut,1),GF_Write,0,0,xsize,bands,data,xsize,bands,GDT_Float32,0,0);
	GDALClose(m_datasetOut);
	delete[]data;
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************************************************/
/*								  �����򻯱任                                                          */
/********************************************************************************************************/
/*-----------------------------����ȥ���Ļ�-----------------------------*/
long Decentered(double *imgBufferIn,int bandSum,int *bandNo,int xsize,int ysize,double *imgBufferOut)
{
	int i,j,k;
	double *meanvalue=NULL;
	double **covmatrix=NULL;
	long rs;

	if(imgBufferIn==NULL||imgBufferOut==NULL)
		return 1;

	meanvalue=new double[bandSum];
	covmatrix=new double*[bandSum];
	for (int i=0;i<bandSum;i++)
		covmatrix[i]=new double [bandSum];

	//rs=GetCovarianceMatrix(imgBufferIn,bandSum,bandNo,xsize,ysize,covmatrix);
	//if(0!=rs)
	//	goto ERROROUT;

	rs=GetImgMean(imgBufferIn,bandSum,bandNo,xsize,ysize,meanvalue);
	if(0!=rs)
		goto ERROROUT;

	for (i=0;i<bandSum;i++)
	{
		for(j=0;j<xsize*ysize;j++)
			imgBufferOut[j+i*xsize*ysize]=(imgBufferIn[j+i*xsize*ysize]-meanvalue[i])/*/sqrt(covmatrix[i][i])*/;
	}

ERROROUT:
	if(meanvalue==NULL)
		return 3;
	delete[]meanvalue;
	for (int i=0;i<bandSum;i++)
		delete[]covmatrix[i];
	delete[]covmatrix;
	return 0;
}

/*-----------------------------����ȥ�����-----------------------------*/
long Derelated(double *imgBufferIn,int bandSum,int *bandNo,int xsize,int ysize,double *imgBufferOut)
{
	int i,j,k;
	long rs;
	double **covImg=NULL;
	double *eigValue=NULL;
	double **eigMat =NULL;
	double **eigMatTemp=NULL;
	double **eigTemp=NULL;
	double **temp   =NULL;
	double **matTran=NULL;

	//�����ļ�
	ofstream ofs("test.txt");

	if(NULL==imgBufferIn||NULL==imgBufferOut)
		return 1;
	memset(imgBufferOut,0,sizeof(double)*bandSum*xsize*ysize);

	//���ݿռ������
	covImg=new double *[bandSum];
	eigMat=new double *[bandSum];
	eigMatTemp=new double *[bandSum];
	eigTemp=new double *[bandSum];
	temp   =new double *[bandSum];
	matTran=new double *[bandSum];
	eigValue=new double[bandSum];

	for (i=0;i<bandSum;i++)
	{
		covImg[i]=new double  [bandSum];
		eigMat[i]=new double  [bandSum];
		eigMatTemp[i]=new double [bandSum];
		eigTemp[i]=new double [bandSum];
		temp[i]  =new double [bandSum];
		matTran[i]=new double [bandSum];
		memset(temp[i],0,sizeof(double)*bandSum);
		memset(eigTemp[i],0,sizeof(double)*bandSum);
		memset(eigMatTemp[i],0,sizeof(double)*bandSum);
	}

	//��ȡ�׻�����
	rs=GetCovarianceMatrix(imgBufferIn,bandSum,bandNo,xsize,ysize,covImg);
	if(rs!=0)
		goto ERROROUT;

	//////////////////////////////////////////////////////////////////////////
	//�������

	for (int i=0;i<bandSum;i++)
	{
		for (int j=0;j<bandSum;j++)
		{
			ofs<<covImg[i][j]<<"  ";
		}
		ofs<<endl;
	}
	ofs.close();
	//////////////////////////////////////////////////////////////////////////

	rs=MatrixEigen_value_vec_Jccob(covImg,eigValue,bandSum,eigMat);
	if(rs!=0)
		goto ERROROUT;

	//����ֵ��С��������
	for (i=0;i<bandSum;i++)
	{
		for (j=0;j<int(bandSum/2);j++)
		{
			swap(eigMat[i][j],eigMat[i][bandSum-1-j]);
		}
	}


	for(i=0;i<bandSum;i++)
		eigTemp[i][i]=eigValue[i];

	//rs=Matrix_Cholesky(eigTemp,temp,bandSum);
	//if(rs!=0)
	//	goto ERROROUT;
	for (int i=0;i<bandSum;i++)
	{
		temp[i][i]=1/sqrt(eigTemp[bandSum-1-i][bandSum-1-i]);
	}
	/////////////////////////////////////////////////
	//��������
	//if (ofs.is_open())
	//{
	//	for (int i=0;i<bandSum;i++)
	//	{
	//		for (int j=0;j<bandSum;j++)
	//		{
	//			ofs<<covImg[i][j]<<"    ";
	//		}
	//		ofs<<endl;
	//	}
	//	ofs.close();
	//}

	///////////////////////////////////////////////////

	rs=MatrixMuti(eigMat,bandSum,bandSum,bandSum,temp,matTran);
	if(rs!=0)
		goto ERROROUT;

	//ͨ���׻���������ݽ��д���
	for (i=0;i<xsize*ysize;i++)
	{
		for (j=0;j<bandSum;j++)
		{
			for (k=0;k<bandSum;k++)
			{
				imgBufferOut[i+j*xsize*ysize]+=imgBufferIn[i+k*xsize*ysize]*matTran[k][j];
			}
		}
	}


ERROROUT:
	if(NULL==covImg||NULL==eigMat||NULL==eigTemp
		||NULL==temp||NULL==matTran||NULL==eigValue||NULL==eigMatTemp)
		return 3;

	for (i=0;i<bandSum;i++)
	{
		delete []covImg[i];
		delete []eigMat[i];
		delete []eigTemp[i];
		delete []temp[i];
		delete []matTran[i];
		delete []eigMatTemp[i];
	}
	delete []covImg;
	delete []eigMat;
	delete []eigTemp;
	delete []temp ;
	delete []matTran;
	delete []eigValue;
	delete []eigMatTemp;

	return 0;
}

/*-------------------------------������-------------------------------*/
void SphereTrans(double *imgBufferIn,int bandSum,int *bandNo,int xsize,int ysize,double *imgBufferOut)
{
	long lError=0;
	double* m_deData =new double[xsize*ysize*bandSum];
	memset(m_deData,0,xsize*ysize*bandSum*sizeof(double));
	lError=Decentered(imgBufferIn,bandSum,bandNo,xsize,ysize,m_deData);
	if (lError!=0)
	{
		delete[]m_deData;
		printf("Decentered Error!");
		return;
	}
	lError=Derelated(m_deData,bandSum,bandNo,xsize,ysize,imgBufferOut);
	if (lError!=0)
	{
		delete[]m_deData;
		printf("Derelated Error!");
		return;
	}

	delete[]m_deData;
	return ;
}

/*----------------------------������GDAL��װ--------------------------*/
void SphereTrans(char* pathSrc,char* pathDst,char* pathDecenter,int* bandNo,int num)
{
	GDALAllRegister();
	GDALDatasetH m_datasetsrc,m_datasetdst;
	m_datasetsrc=GDALOpen(pathSrc,GA_ReadOnly);
	if(!m_datasetsrc)
		return ;

	//��ȡ���ݺ�Ӱ����Ϣ
	int width=GDALGetRasterBandXSize(m_datasetsrc);
	int heigh=GDALGetRasterBandYSize(m_datasetsrc);
	int bands=GDALGetRasterCount(m_datasetsrc);

	//��ȡʵ�ʴ�������
	int realBand=0;
	if(bandNo==NULL)
		realBand=bands;
	else
	{
		for (int i=0;i<num;i++)
		{
			if(bandNo[i]!=0)
				realBand++;
		}
	}

	//bands=realBand;
	int *band=new int[realBand];
	double* m_Indata =new double[width*heigh*realBand];
	double* m_deData =new double[width*heigh*realBand];
	double* m_Outdata=new double[width*heigh*realBand];
	memset(m_deData,0,sizeof(double)*width*heigh*realBand);
	memset(m_Outdata,0,sizeof(double)*width*heigh*realBand);

	GDALRasterBandH m_band;

	//������  ����ѡȡ�Ĳ��ζ�ȡ���ݽ��������򻯱任
	if(bandNo==NULL)
	{
		for (int i=0;i<bands;i++)
		{
			band[i]=i+1;
			m_band=GDALGetRasterBand(m_datasetsrc,i+1);
			GDALRasterIO(m_band,GF_Read,0,0,width,heigh,m_Indata+i*width*heigh,width,heigh,GDT_Float64,0,0);
		}
		Decentered(m_Indata,realBand,bandNo,width,heigh,m_deData);
		Derelated(m_deData,realBand,bandNo,width,heigh,m_Outdata);
	}
	else
	{
		for (int i=0;i<bands;i++)
		{
			if(bandNo[i]!=0)
			{

				band[i]=i+1;
				m_band=GDALGetRasterBand(m_datasetsrc,i+1);
				GDALRasterIO(m_band,GF_Read,0,0,width,heigh,m_Indata+i*width*heigh,width,heigh,GDT_Float64,0,0);
			}
		}
		Decentered(m_Indata,realBand,bandNo,width,heigh,m_deData);
		Derelated(m_deData,realBand,bandNo,width,heigh,m_Outdata);
	}
	const char* proj=GDALGetProjectionRef(m_datasetsrc);
	//д���ݽ�PCA�任�������д���ļ���
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	m_datasetdst=GDALCreate(GDALGetDriverByName("GTiff"),pathDst,width,heigh,realBand,GDT_Float64,papszOptions);
	for (int i=0;i<realBand;i++)
	{
		m_band=GDALGetRasterBand(m_datasetdst,i+1);
		GDALRasterIO(m_band,GF_Write,0,0,width,heigh,m_Outdata+i*width*heigh,width,heigh,GDT_Float64,0,0);
	}
	GDALSetProjection(m_datasetdst,proj);
	GDALClose(m_datasetdst);
	GDALClose(m_datasetsrc);

	if (pathDecenter!=NULL)
	{
		GDALDatasetH dedataset=GDALCreate(GDALGetDriverByName("GTiff"),pathDecenter,width,heigh,realBand,GDT_Float64,papszOptions);

		for (int i=0;i<realBand;i++)
		{
			m_band=GDALGetRasterBand(dedataset,i+1);
			GDALRasterIO(m_band,GF_Write,0,0,width,heigh,m_deData+i*width*heigh,width,heigh,GDT_Float64,0,0);
		}
		GDALClose(dedataset);
	}

	delete[]m_Indata;
	delete[]m_Outdata;
	delete[]m_deData;
	delete[]band;
}


/********************************************************************************************************/
/*								          MNF�任                                                       */
/********************************************************************************************************/

/*-------------------------------��ȡMNF����----------------------------*/
long GetMNFOperator(double **covmatNoise,double **covmatImg,double** mnfOperator,int size)
{
	int i;
	int rs;				// ����ֵ
	double **invData;
	double **temp;
	double **tempF;
	double **tempCovImg;
	double **transTempF;
	double *eigValue;
	double **eigMat;
	double **tempMat;
	ofstream ofs("test.txt");
	//�������
	if (covmatNoise==NULL||covmatNoise==NULL||mnfOperator==NULL)
		return 1;
	if(size<1)
		return 2;

	if(NULL==(invData=new double *[size])||NULL==(temp=new double *[size])
		||NULL==(tempCovImg=new double *[size])||NULL==(tempF=new double *[size])
		||NULL==(transTempF=new double *[size])||NULL==(eigMat=new double *[size])
		||NULL==(tempMat=new double*[size]))
		return 3;
	for(i=0;i<size;i++)
	{
		invData[i]=new double [size];
		temp[i]=new double [size];
		tempCovImg[i]=new double [size];
		tempF[i]=new double [size];
		transTempF[i]=new double [size];
		eigMat[i]  =new double[size];
		tempMat[i]=new double [size];
		memset(tempMat[i],0,sizeof(double)*size);
	}
	if (NULL==(eigValue=new double [size]))
		return 3;

	rs=MatrixEigen_value_vec_Jccob(covmatNoise,eigValue,size,eigMat);

	///////////////////////////////////////////////////////////////////////////
	//����
	//ofstream ofile("temp.txt");
	//for (i=0;i<size;i++)
	//{
	//	for ( int j=0;j<size;j++)
	//	{
	//		ofile<<covmatNoise[i][j]<<"    ";
	//	}
	//	ofile<<endl;
	//}
	//ofile.close();
	//////////////////////////////////////////////////////////////////////////

	for (i=0;i<size;i++)
	{
		tempMat[i][i]=eigValue[i];
	}

	rs=MatrixInverse(tempMat,size,invData);
	if(rs!=0)
		goto ERROROUT;
	rs=Matrix_Cholesky(invData,tempF,size);
	if(rs!=0)
		goto ERROROUT;

	//rs=MatrixTrans(tempF,size,size,transTempF);
	rs=MatrixMuti(eigMat,size,size,size,tempF,transTempF);
	if(rs!=0)
		goto ERROROUT;

	rs=MatrixMuti(transTempF,size,size,size,covmatImg,temp);
	if(rs!=0)
		goto ERROROUT;
	rs=MatrixTrans(transTempF,size,size,eigMat);
	if(rs!=0)
		goto ERROROUT;
	rs=MatrixMuti(temp,size,size,size,eigMat,tempCovImg);
	if(rs!=0)
		goto ERROROUT;

	//���tempCovImg������ֵ����������
	rs=MatrixEigen_value_vec_Jccob(tempCovImg,eigValue,size,eigMat);
	if(rs!=0)
		goto ERROROUT;

	rs=MatrixMuti(transTempF,size,size,size,eigMat,mnfOperator);
	if(rs!=0)
		goto ERROROUT;

	//rs=MatrixEigen_value_vec_InvPower(temp,eigValue,size,mnfOperator);
	//if(rs!=0)
	//	return rs;

	//���ָ��
ERROROUT:
	if (invData==NULL||temp==NULL||eigValue==NULL||tempF==NULL
		||tempCovImg==NULL||transTempF==NULL||eigMat==NULL||tempMat==NULL)
		return 3;
	for (i=0;i<size;i++)
	{
		delete[]invData[i];
		delete[]temp[i];
		delete[]tempF[i];
		delete[]tempCovImg[i];
		delete[]transTempF[i];
		delete[]eigMat[i];
		delete[]tempMat[i];
	}

	delete[]invData;
	delete[]temp;
	delete[]tempF;
	delete[]tempCovImg;
	delete[]transTempF;
	delete[]eigMat;
	delete[]eigValue;
	delete[]tempMat;

	return 0;
}

/*-------------------------------����MNF�任----------------------------*/
long  MNF(double *imgBufferIn,int bandSum,int *bandNo,
									int xsize,int ysize,double *imgBufferOut,char* path)
{
	int i,j,k;
	int rs;
	/*double *imgNoise;*/	//����Ӱ��
	double *imgDeal;			//�����Ӱ�񲨶�
	double **covNoise;
	double **covImg;
	double **mnfOperator;
	double *imgNoise;

	//�������
	if(imgBufferIn==NULL||imgBufferOut==NULL)
		return 1;
	if (bandSum<=1||xsize<=1||ysize<=1)
		return 2;

	//����ռ�
	if(NULL==(imgNoise=new double[xsize*ysize*bandSum])||NULL==(imgDeal=new double[xsize*ysize*bandSum]))
		return 3;
	if (NULL==(covNoise=new double*[bandSum])||NULL==(covImg=new double*[bandSum])||NULL==(mnfOperator=new double*[bandSum]))
		return 3;

	for (i=0;i<bandSum;i++)
	{
		covNoise[i]=new double [bandSum];
		covImg[i]=new double [bandSum];
		mnfOperator[i]=new double [bandSum];
	}
	memset(imgBufferOut,0,sizeof(double)*xsize*ysize*bandSum);

	//��ȡ����Ӱ��
	//�������Ӱ��
	memcpy(imgDeal,imgBufferIn,sizeof(double)*xsize*ysize*bandSum);

	/*����ƺ��е����� �Ⱦ�������*/
	rs=GetNoiseImg(imgDeal,bandSum,bandNo,1,1,xsize,ysize,imgNoise);
	if(rs!=0)
		return rs;

	//��ȡ������Ӱ���Э�������
	//Ӱ�����ݰ׻�
	SphereTrans(imgBufferIn,bandSum,bandNo,xsize,ysize,imgDeal);

	rs=GetCovarianceMatrix(imgDeal,bandSum,bandNo,xsize,ysize,covImg);
	if(rs!=0)
		return rs;
	rs=GetCovarianceMatrix(imgNoise,bandSum,bandNo,xsize,ysize,covNoise);
	if(rs!=0)
		return rs;

	//��ȡmnf�任����
	rs=GetMNFOperator(covNoise,covImg,mnfOperator,bandSum);
	if(rs!=0)
		return rs;

	//����MNF�任
	for (i=0;i<xsize*ysize;i++)
	{
		for (j=0;j<bandSum;j++)
		{
			for (k=0;k<bandSum;k++)
			{
				imgBufferOut[i+j*xsize*ysize]+=imgBufferIn[i+k*xsize*ysize]*mnfOperator[k][j];
			}
		}
	}

	//����任�����ļ���
	Matrix_Export_File(path,mnfOperator,bandSum);

	//���ָ�벢����
	if (imgNoise==NULL||imgDeal==NULL||covImg==NULL||covNoise==NULL||mnfOperator==NULL)
		return 3;
	delete[]imgDeal;
	//delete[]imgNoise;
	for (i=0;i<bandSum;i++)
	{
		delete[]covImg[i];
		delete[]covNoise[i];
		delete[]mnfOperator[i];
	}
	delete[]imgNoise;
	delete[]covImg;
	delete[]covNoise;
	delete[]mnfOperator;
	return 0;
}

/*-------------------------------����MNF���任--------------------------*/
long InvMNF(double* imgBufferIn,int bandSum,int *bandNo,
					int xsize,int ysize,double *imgBufferOut,char *path)
{
	int i,j,k;
	int rs;
	int size;
	int bandNum=0;
	double **matTrans=NULL;
	double **InvmnfOperator=NULL;
	double *data;

	//�������
	if(NULL==imgBufferIn||NULL==imgBufferOut||NULL==path)
		return 1;
	if(bandSum<=1||xsize<=1||ysize<=1)
		return 2;

	//��ʼֵ
	memset(imgBufferOut,0,sizeof(double)*xsize*ysize*bandSum);
	//��ñ任����Ĵ�С
	if(0!=(rs=Matrix_Get_Size(path,size)))
		return rs;
	//����ռ�
	matTrans=new double *[size];
	for(i=0;i<bandSum;i++)
		matTrans[i]=new double [size];
	if(NULL==matTrans)
		return 3;

	//��ñ任��������
	if(0!=(rs=Matrix_Import_File(path,matTrans)))
		return rs;

	//���bandNoΪ�������в��ζ�δ���޳�
	if (bandNo==NULL)
	{
		//����ռ�
		InvmnfOperator=new double *[size];
		for(i=0;i<size;i++)
			InvmnfOperator[i]=new double [size];
		//�����ת��
		double **tmatTrans=new double *[size];
		for(i=0;i<size;i++)
		{
			tmatTrans[i]=new double [size];
			memcpy(tmatTrans[i],matTrans[i],sizeof(double)*size);
		}
		//MatrixTrans(matTrans,size,size,tmatTrans);
		MatrixInverse(tmatTrans,size,InvmnfOperator);

		for (i=0;i<xsize*ysize;i++)
		{
			for (j=0;j<bandSum;j++)
			{
				for (k=0;k<bandSum;k++)
				{
					imgBufferOut[i+j*xsize*ysize]+=imgBufferIn[i+k*xsize*ysize]*InvmnfOperator[k][j];
				}
			}
		}

		//ɾ��ָ��
		//delete[]data;
		for (i=0;i<bandSum;i++)
		{
			delete[]matTrans[i];
			delete[]InvmnfOperator[i];
		}
		delete[]matTrans;
		delete[]InvmnfOperator;
	}
	else
	{
		//����ռ�
		double **tempOperator=new double *[size];
		double **tmatTrans=new double *[size];
		for(i=0;i<size;i++)
		{
			tempOperator[i]=new double [size];
			tmatTrans[i]=new double [size];
			memcpy(tmatTrans[i],matTrans[i],sizeof(double)*size);
		}
		//MatrixTrans(matTrans,size,size,tmatTrans);
		MatrixInverse(tmatTrans,size,tempOperator);

		for (i=0;i<size;i++)
			memcpy(matTrans[i],tmatTrans[i],sizeof(double)*size);

		//��ȡδ�޳��Ĳ�����Ŀ
		for(i=0;i<bandSum;i++)
		{
			if(bandNo[i]!=0)
				bandNum++;
		}
		//����ռ�
		InvmnfOperator=new double *[bandNum];
		for(i=0;i<bandNum;i++)
			InvmnfOperator[i]=new double [size];
		for(i=0;i<size;i++)
		{
			k=0;
			for(j=0;j<size;j++)
			{
				if (bandNo[j]!=0)
				{
					InvmnfOperator[k][i]=tempOperator[i][j];
					k++;
				}
			}
		}
		//��ȡ������Ĳ�������
		data=new double [xsize*ysize*bandNum];
		for(i=0,k=0;i<bandSum;i++)
		{
			if(bandNo[i]!=0)
			{
				memcpy(data+k*xsize*ysize,imgBufferIn+i*xsize*ysize,sizeof(double)*xsize*ysize);
				k++;
			}
		}

		//�����ݽ��д���
		for (i=0;i<xsize*ysize;i++)
		{
			for (j=0;j<bandSum;j++)
			{
				for (k=0;k<bandNum;k++)
				{
					imgBufferOut[i+j*xsize*ysize]+=data[i+k*xsize*ysize]*InvmnfOperator[k][j];
				}
			}
		}

		delete[]data;
		for(i=0;i<bandSum;i++)
		{
			delete[]matTrans[i];
			delete[]tmatTrans[i];
		}
		delete[]tmatTrans;
		delete[]matTrans;
		for(i=0;i<bandNum;i++)
			delete[]InvmnfOperator[i];
		delete[]InvmnfOperator;
		for (int i=0;i<size;i++)
			delete[]tempOperator[i];
		delete[]tempOperator;
	}

	return 0;
}

/*------------------------GDAL��װMNF�任��MNF���任--------------------*/
void MNFTrans(char* pathSrc,char* pathDst,int* bandNo,int num,char* pathMartix)
{
	GDALAllRegister();
	GDALDatasetH m_datasetsrc,m_datasetdst;
	m_datasetsrc=GDALOpen(pathSrc,GA_ReadOnly);
	if(!m_datasetsrc)
		return ;

	//��ȡ���ݺ�Ӱ����Ϣ
	int width=GDALGetRasterBandXSize(m_datasetsrc);
	int heigh=GDALGetRasterBandYSize(m_datasetsrc);
	int bands=GDALGetRasterCount(m_datasetsrc);

	//��ȡʵ�ʴ�������
	int realBand=0;
	if(bandNo==NULL)
		realBand=bands;
	else
	{
		for (int i=0;i<num;i++)
		{
			if(bandNo[i]!=0)
				realBand++;
		}
	}
	//bands=realBand;
	int *band=new int[realBand];
	double* m_Indata =new double[width*heigh*realBand];
	double* m_Outdata=new double[width*heigh*realBand];
	//double* m_Whiten_Data=new double[width*heigh*realBand];

	GDALRasterBandH m_band;
	//������  ����ѡȡ�Ĳ��ζ�ȡ���ݽ���PCA�任
	if(bandNo==NULL)
	{
		for (int i=0;i<bands;i++)
		{
			band[i]=i+1;
			m_band=GDALGetRasterBand(m_datasetsrc,i+1);
			GDALRasterIO(m_band,GF_Read,0,0,width,heigh,m_Indata+i*width*heigh,width,heigh,GDT_Float64,0,0);
		}
		MNF(m_Indata,bands,band,width,heigh,m_Outdata,pathMartix);
	}
	else
	{
		for (int i=0;i<bands;i++)
		{
			if(bandNo[i]!=0)
			{

				band[i]=i+1;
				m_band=GDALGetRasterBand(m_datasetsrc,i+1);
				GDALRasterIO(m_band,GF_Read,0,0,width,heigh,m_Indata+i*width*heigh,width,heigh,GDT_Float64,0,0);
			}
		}
		const char* proj=GDALGetProjectionRef(m_datasetsrc);
		MNF(m_Indata,bands,band,width,heigh,m_Outdata,pathMartix);
	}
	const char* proj=GDALGetProjectionRef(m_datasetsrc);
	//д���ݽ�PCA�任�������д���ļ���
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	m_datasetdst=GDALCreate(GDALGetDriverByName("GTiff"),pathDst,width,heigh,realBand,GDT_Float64,papszOptions);
	for (int i=0;i<realBand;i++)
	{
		m_band=GDALGetRasterBand(m_datasetdst,i+1);
		GDALRasterIO(m_band,GF_Write,0,0,width,heigh,m_Outdata+i*width*heigh,width,heigh,GDT_Float64,0,0);
	}
	GDALSetProjection(m_datasetdst,proj);
	GDALClose(m_datasetdst);
	GDALClose(m_datasetsrc);

	delete[]m_Indata;
	delete[]m_Outdata;
	delete[]band;
}

void InvMNFTrans(char* pathSrc,char* pathDst,int* bandNo,int num,char* pathMartix)
{

	GDALAllRegister();
	GDALDatasetH m_datasetsrc,m_datasetdst;
	m_datasetsrc=GDALOpen(pathSrc,GA_ReadOnly);
	if(!m_datasetsrc)
		return ;

	//��ȡ���ݺ�Ӱ����Ϣ
	int width=GDALGetRasterBandXSize(m_datasetsrc);
	int heigh=GDALGetRasterBandYSize(m_datasetsrc);
	int bands=GDALGetRasterCount(m_datasetsrc);
	int *band=new int[bands];
	double* m_Indata =new double[width*heigh*bands];
	double* m_Outdata=new double[width*heigh*bands];
	GDALRasterBandH m_band;

	//������
	for (int i=0;i<bands;i++)
	{
		band[i]=i+1;
		m_band=GDALGetRasterBand(m_datasetsrc,i+1);
		GDALRasterIO(m_band,GF_Read,0,0,width,heigh,m_Indata+i*width*heigh,width,heigh,GDT_Float64,0,0);
	}
	const char* proj=GDALGetProjectionRef(m_datasetsrc);
	//PCA(m_Indata,bands,band,width,heigh,m_Outdata,pathMartix);
	if(bandNo==NULL)
		InvMNF(m_Indata,bands,bandNo,width,heigh,m_Outdata,pathMartix);
	else
		InvMNF(m_Indata,bands,bandNo,width,heigh,m_Outdata,pathMartix);

	//д����
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	m_datasetdst=GDALCreate(GDALGetDriverByName("GTiff"),pathDst,width,heigh,bands,GDT_Float64,papszOptions);
	GDALSetProjection(m_datasetdst,proj);
	for (int i=0;i<bands;i++)
	{
		m_band=GDALGetRasterBand(m_datasetdst,i+1);
		GDALRasterIO(m_band,GF_Write,0,0,width,heigh,m_Outdata+i*width*heigh,width,heigh,GDT_Float64,0,0);
	}

	GDALClose(m_datasetdst);
	GDALClose(m_datasetsrc);
	delete[]m_Indata;
	delete[]m_Outdata;
	delete[]band;
}

/********************************************************************************************************/
/*								  PCA�任                                                               */
/********************************************************************************************************/
/*-------------------------------���PCA�任����------------------------*/
long GetPCAOperator(double **covmatImg,double **pcaOperator,int size)
{
	int rs;				// ����ֵ
	double *eigValue;
	//�������
	if (covmatImg==NULL||pcaOperator==NULL)
		return 1;
	if(size<1)
		return 2;

	if(NULL==(eigValue=new double [size]))
		return 3;
	rs=MatrixEigen_value_vec_Jccob(covmatImg,eigValue,size,pcaOperator);
	if(rs!=0)
		return rs;

	//���ָ��
	if (eigValue==NULL)
		return 3;
	delete[]eigValue;
	return 0;
}


/*---------------------------------����PCA�任--------------------------*/
long PCA(double *imgBufferIn,int bandSum,int *bandNo,
					int xsize,int ysize,double *imgBufferOut,char* path)
{
	int i,j,k;
	int rs;
	double *imgDeal;		//�����Ӱ�񲨶�
	double **covImg;
	double **pcaOperator;

	//�������
	if(imgBufferIn==NULL||imgBufferOut==NULL)
		return 1;
	if (bandSum<=1||xsize<1||ysize<1)
		return 2;

	//����ռ�
	if(NULL==(imgDeal=new double[xsize*ysize*bandSum]))
		return 3;
	if (NULL==(covImg=new double*[bandSum])||NULL==(pcaOperator=new double*[bandSum]))
		return 3;
	for (i=0;i<bandSum;i++)
	{
		covImg[i]=new double [bandSum];
		pcaOperator[i]=new double [bandSum];
	}
	memset(imgBufferOut,0,sizeof(double)*xsize*ysize*bandSum);

	//��ȡ����Ӱ��
	//�������Ӱ��
	memcpy(imgDeal,imgBufferIn,sizeof(double)*xsize*ysize*bandSum);

	//Ӱ���Э�������
	rs=GetCovarianceMatrix(imgDeal,bandSum,bandNo,xsize,ysize,covImg);
	if(rs!=0)
		return rs;

	//��ȡPCA�任����
	rs=GetPCAOperator(covImg,pcaOperator,bandSum);
	if(rs!=0)
		return rs;

	//����PCA�任
	for (i=0;i<xsize*ysize;i++)
	{
		for (j=0;j<bandSum;j++)
		{
			for (k=0;k<bandSum;k++)
			{
				imgBufferOut[i+j*xsize*ysize]+=imgBufferIn[i+k*xsize*ysize]*pcaOperator[k][j];
			}
		}
	}

	//��MNF�任�������
	if(path!=NULL)
		Matrix_Export_File(path,pcaOperator,bandSum);
	else
		printf("ע�⣺δȡ��ת������!\n");

	//���ָ�벢����
	if (imgDeal==NULL||covImg==NULL||pcaOperator==NULL)
		return 3;
	delete[]imgDeal;
	for (i=0;i<bandSum;i++)
	{
		delete[]covImg[i];
		delete[]pcaOperator[i];
	}
	delete[]covImg;
	delete[]pcaOperator;
	return 0;
}


/*--------------------------------����PCA���任-------------------------*/
long InvPCA(double *imgBufferIn,int bandSum,int *bandNo,
						int xsize,int ysize,double *imgBufferOut,char* path)
{
	int i,j,k;
	int rs;
	int size;
	int bandNum=0;
	double **matTrans=NULL;
	double **InvpcaOperator=NULL;
	double *data;

	//�������
	if(NULL==imgBufferIn||NULL==imgBufferOut||NULL==path)
		return 1;
	if(bandSum<=1||xsize<=1||ysize<=1)
		return 2;

	//��ʼֵ
	memset(imgBufferOut,0,sizeof(double)*xsize*ysize*bandSum);
	//��ñ任����Ĵ�С
	if(0!=(rs=Matrix_Get_Size(path,size)))
		return rs;
	//����ռ�
	matTrans=new double *[size];
	for(i=0;i<bandSum;i++)
		matTrans[i]=new double [size];
	if(NULL==matTrans)
		return 3;

	//��ñ任��������
	if(0!=(rs=Matrix_Import_File(path,matTrans)))
		return rs;

	//���bandNoΪ�������в��ζ�δ���޳�
	if (bandNo==NULL)
	{
		//����ռ�
		InvpcaOperator=new double *[size];
		for(i=0;i<size;i++)
			InvpcaOperator[i]=new double [size];
		//�����ת��
		double **tmatTrans=new double *[size];
		for(i=0;i<size;i++)
		{
			tmatTrans[i]=new double [size];
		}
		MatrixTrans(matTrans,size,size,tmatTrans);
		MatrixInverse(tmatTrans,size,InvpcaOperator);

		for (i=0;i<xsize*ysize;i++)
		{
			for (j=0;j<bandSum;j++)
			{
				for (k=0;k<bandSum;k++)
				{
					imgBufferOut[i+j*xsize*ysize]+=imgBufferIn[i+k*xsize*ysize]*InvpcaOperator[k][j];
				}
			}
		}

		//ɾ��ָ��
		//delete[]data;
		for (i=0;i<bandSum;i++)
		{
			delete[]matTrans[i];
			delete[]InvpcaOperator[i];
		}
		delete[]matTrans;
		delete[]InvpcaOperator;
	}
	else
	{
		//����ռ�
		double **tempOperator=new double *[size];
		double **tmatTrans   =new double *[size];
		for(i=0;i<size;i++)
		{
			tempOperator[i]=new double [size];
			tmatTrans[i]   =new double [size];
		}
		MatrixTrans(matTrans,size,size,tmatTrans);
		MatrixInverse(tmatTrans,size,tempOperator);

		for (i=0;i<size;i++)
			memcpy(matTrans[i],tmatTrans[i],sizeof(double)*size);

		//��ȡδ�޳��Ĳ�����Ŀ
		for(i=0;i<bandSum;i++)
		{
			if(bandNo[i]!=0)
				bandNum++;
		}
		//����ռ�
		InvpcaOperator=new double *[bandNum];
		for(i=0;i<bandNum;i++)
			InvpcaOperator[i]=new double [size];
		for(i=0;i<size;i++)
		{
			k=0;
			for(j=0;j<size;j++)
			{
				if (bandNo[j]!=0)
				{
					InvpcaOperator[k][i]=tempOperator[i][j];
					k++;
				}
			}
		}
		//��ȡ������Ĳ�������
		data=new double [xsize*ysize*bandNum];
		for(i=0,k=0;i<bandSum;i++)
		{
			if(bandNo[i]!=0)
			{
				memcpy(data+k*xsize*ysize,imgBufferIn+i*xsize*ysize,sizeof(double)*xsize*ysize);
				k++;
			}
		}

		//�����ݽ��д���
		for (i=0;i<xsize*ysize;i++)
		{
			for (j=0;j<bandSum;j++)
			{
				for (k=0;k<bandNum;k++)
				{
					imgBufferOut[i+j*xsize*ysize]+=data[i+k*xsize*ysize]*InvpcaOperator[k][j];
				}
			}
		}

		delete[]data;
		for(i=0;i<bandSum;i++)
		{
			delete[]matTrans[i];
			delete[]tmatTrans[i];
		}
		delete[]tmatTrans;
		delete[]matTrans;
		for(i=0;i<bandNum;i++)
			delete[]InvpcaOperator[i];
		delete[]InvpcaOperator;
		for (int i=0;i<size;i++)
			delete[]tempOperator[i];
		delete[]tempOperator;
	}

	return 0;

}


/*------------------------GDAL��װPCA�任��PCA���任--------------------*/
void PCATrans(char* pathSrc,char* pathDst,int* bandNo,int num,char* pathMartix)
 {
 	GDALAllRegister();
 	GDALDatasetH m_datasetsrc,m_datasetdst;
 	m_datasetsrc=GDALOpen(pathSrc,GA_ReadOnly);
 	if(!m_datasetsrc)
 		return ;

 	//��ȡ���ݺ�Ӱ����Ϣ
 	int width=GDALGetRasterBandXSize(m_datasetsrc);
 	int heigh=GDALGetRasterBandYSize(m_datasetsrc);
 	int bands=GDALGetRasterCount(m_datasetsrc);

	//��ȡʵ�ʴ�������
	int realBand=0;
	if(bandNo==NULL)
		realBand=bands;
	else
	{
		for (int i=0;i<num;i++)
		{
			if(bandNo[i]!=0)
				realBand++;
		}
	}
	//bands=realBand;
 	int *band=new int[realBand];
 	double* m_Indata =new double[width*heigh*realBand];
 	double* m_Outdata=new double[width*heigh*realBand];
 	GDALRasterBandH m_band;

	//������  ����ѡȡ�Ĳ��ζ�ȡ���ݽ���PCA�任
	if(bandNo==NULL)
	{
		for (int i=0;i<bands;i++)
		{
			band[i]=i+1;
			m_band=GDALGetRasterBand(m_datasetsrc,i+1);
			GDALRasterIO(m_band,GF_Read,0,0,width,heigh,m_Indata+i*width*heigh,width,heigh,GDT_Float64,0,0);
		}
		PCA(m_Indata,bands,band,width,heigh,m_Outdata,pathMartix);
	}
	else
	{
		for (int i=0;i<bands;i++)
		{
			if(bandNo[i]!=0)
			{

				band[i]=i+1;
				m_band=GDALGetRasterBand(m_datasetsrc,i+1);
				GDALRasterIO(m_band,GF_Read,0,0,width,heigh,m_Indata+i*width*heigh,width,heigh,GDT_Float64,0,0);
			}
		}
		const char* proj=GDALGetProjectionRef(m_datasetsrc);
		PCA(m_Indata,realBand,band,width,heigh,m_Outdata,pathMartix);
	}
	const char* proj=GDALGetProjectionRef(m_datasetsrc);
 	//д���ݽ�PCA�任�������д���ļ���
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
 	m_datasetdst=GDALCreate(GDALGetDriverByName("GTiff"),pathDst,width,heigh,realBand,GDT_Float64,papszOptions);
 	for (int i=0;i<realBand;i++)
 	{
 		m_band=GDALGetRasterBand(m_datasetdst,i+1);
 		GDALRasterIO(m_band,GF_Write,0,0,width,heigh,m_Outdata+i*width*heigh,width,heigh,GDT_Float64,0,0);
 	}
 	GDALSetProjection(m_datasetdst,proj);
 	GDALClose(m_datasetdst);
 	GDALClose(m_datasetsrc);

 	delete[]m_Indata;
 	delete[]m_Outdata;
 	delete[]band;
 }

void InvPCATrans(char* pathSrc,char* pathDst,int* bandNo,int num,char* pathMartix)
{

	GDALAllRegister();
	GDALDatasetH m_datasetsrc,m_datasetdst;
	m_datasetsrc=GDALOpen(pathSrc,GA_ReadOnly);
	if(!m_datasetsrc)
	 	return ;

	//��ȡ���ݺ�Ӱ����Ϣ
	int width=GDALGetRasterBandXSize(m_datasetsrc);
	int heigh=GDALGetRasterBandYSize(m_datasetsrc);
	int bands=GDALGetRasterCount(m_datasetsrc);
	int *band=new int[bands];
	double* m_Indata =new double[width*heigh*bands];
	double* m_Outdata=new double[width*heigh*bands];
	GDALRasterBandH m_band;

	//������
	for (int i=0;i<bands;i++)
	{
	 	band[i]=i+1;
	 	m_band=GDALGetRasterBand(m_datasetsrc,i+1);
	 	GDALRasterIO(m_band,GF_Read,0,0,width,heigh,m_Indata+i*width*heigh,width,heigh,GDT_Float64,0,0);
	}
	const char* proj=GDALGetProjectionRef(m_datasetsrc);
	//PCA(m_Indata,bands,band,width,heigh,m_Outdata,pathMartix);
	if(bandNo==NULL)
		InvPCA(m_Indata,bands,bandNo,width,heigh,m_Outdata,pathMartix);
	else
		InvPCA(m_Indata,bands,bandNo,width,heigh,m_Outdata,pathMartix);

	//д����
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	m_datasetdst=GDALCreate(GDALGetDriverByName("GTiff"),pathDst,width,heigh,bands,GDT_Float64,papszOptions);
	GDALSetProjection(m_datasetdst,proj);
	for (int i=0;i<bands;i++)
	{
	 	m_band=GDALGetRasterBand(m_datasetdst,i+1);
	 	GDALRasterIO(m_band,GF_Write,0,0,width,heigh,m_Outdata+i*width*heigh,width,heigh,GDT_Float64,0,0);
	}

	GDALClose(m_datasetdst);
	GDALClose(m_datasetsrc);
	delete[]m_Indata;
	delete[]m_Outdata;
	delete[]band;
}


/********************************************************************************************************/
/*						                        RXD�㷨                                                 */
/********************************************************************************************************/
void RXD(double *imgBufferIn,double* imgBufferOut,int xsize,int ysize,int bands,int bgWndsize,int tWndsize)
{
	int xtwnd,ytwnd;	//Ŀ�괰�ڵ�λ�� ����ڱ�������
	int xbwnd,ybwnd;	//�������ڵ�λ�� �����Ӱ��
	double thresthold;	//��ֵ

	/*
		���ݿռ������
	*/
	double* bwndmean=NULL;	//�������ھ�ֵ
	double* twndmean=NULL;	//Ŀ�괰�ڵľ�ֵ
	double* bgwndPixels=NULL;//����ȡ��������

	double** bcovMat=NULL;	//�������ڵ�Э�������
	double** bInvcovMat=NULL;//����Э�������������
	double** eigMat=NULL;	//������������
	double* eigValue=NULL;	//����ֵ

	bwndmean   =new double[bands];memset(bwndmean,0,sizeof(double)*bands);
	twndmean   =new double[bands];memset(twndmean,0,sizeof(double)*bands);
	bgwndPixels=new double[bgWndsize*bgWndsize*bands];
	memset(imgBufferOut,0,sizeof(double)*xsize*ysize);

	bcovMat    =new double*[bands];
	bInvcovMat =new double*[bands];
	eigMat     =new double*[bands];
	for (int i =0;i<bands;i++)
	{
		bcovMat[i]   =new double[bands];
		bInvcovMat[i]=new double[bands];
		eigMat[i]    =new double[bands];
	}
	eigValue  =new double[bands];
	if(bcovMat==NULL||bInvcovMat==NULL||eigMat==NULL||eigValue==NULL||bgwndPixels==NULL)
		return ;
	//��ȡӰ��ÿһ������
	int xwndsize,ywndsize;
	for (xbwnd=0;xbwnd<xsize;xbwnd+=bgWndsize-tWndsize)
	{
		for (ybwnd=0;ybwnd<ysize;ybwnd+=bgWndsize-tWndsize)
		{
			xwndsize=xsize-xbwnd;
			ywndsize=ysize-ybwnd;

			//�ж��Ƿ�Ϊ��Ե ���ڱ�Ե���⴦��
			if(xwndsize<bgWndsize&&ywndsize<bgWndsize)
			{
				int tempbgxWndsize=xwndsize;
				int tempbgyWndsize=ywndsize;
				double *tempPixels =NULL;
				tempPixels=new double[tempbgxWndsize*tempbgyWndsize];
				tempPixels=GetpartImage(imgBufferIn,bands,xsize,ysize,xbwnd,ybwnd,tempbgxWndsize,tempbgyWndsize);

				//��ȡ��һ���������� ����㱳��������������
				bgwndPixels=GetpartImage(imgBufferIn,bands,xsize,ysize,xbwnd,ybwnd,bgWndsize,bgWndsize);
				if(bgwndPixels==NULL)
					return;

				long lError=0;
				//���㱳��Ӱ���Э�������ֵ������������ֵ
				lError=GetImgMean(bgwndPixels,bands,NULL,bgWndsize,bgWndsize,bwndmean);
				if(lError!=0)
					return;
				lError=GetCovarianceMatrix(bgwndPixels,bands,NULL,bgWndsize,bgWndsize,bcovMat);
				if(lError!=0)
					return;
				lError=MatrixInverse(bcovMat,bands,bInvcovMat);
				if(lError!=0)
					return;
				lError=MatrixEigen_value_vec_Jccob(bInvcovMat,eigValue,bands,eigMat);
				if(lError!=0)
					return;


				//��ȡ�������صľ�ֵ
				//Ŀ�괰���ڱ������ڵ�λ�õ��ƶ�
				for (xtwnd=0;xtwnd<tempbgxWndsize-tWndsize-1;xtwnd++)
				{
					for (ytwnd=0;ytwnd<tempbgyWndsize-tWndsize-1;ytwnd++)
					{
						//����Ŀ�괰�ڵľ�ֵ
						for (int i=0;i<tWndsize;i++)
						{
							for (int j=0;j<tWndsize;j++)
							{
								//�����в���
								for (int k=0;k<bands;k++)
								{
									twndmean[k]+=bgwndPixels[(ytwnd+j)*bgWndsize+xtwnd+j+k*bgWndsize*bgWndsize]/double(tWndsize)/double(tWndsize);
								}
							}
						}

						//���������
						for (int i=0;i<bands;i++)
						{
							//��ȥ�����ľ�ֵ
							imgBufferOut[(ybwnd+ytwnd+tWndsize/2)*xsize+xbwnd+xtwnd+tWndsize/2]+=(twndmean[i]-bwndmean[i])*eigMat[i][0];
						}
						memset(twndmean,0,sizeof(double)*bands);

					}
				}//����������������������

				memset(bwndmean,0,sizeof(double)*bands);
				delete[]tempPixels;tempPixels=NULL;
			}
			else if (xwndsize>bgWndsize&&ywndsize<bgWndsize)
			{
				int tempbgxWndsize=bgWndsize;
				int tempbgyWndsize=ywndsize;
				double *tempPixels =NULL;
				tempPixels=new double[tempbgxWndsize*tempbgyWndsize];
				tempPixels=GetpartImage(imgBufferIn,bands,xsize,ysize,xbwnd,ybwnd,tempbgxWndsize,tempbgyWndsize);
				if(tempPixels==NULL)
					return;

				long lError=0;
				//���㱳��Ӱ���Э�������ֵ������������ֵ
				lError=GetImgMean(bgwndPixels,bands,NULL,bgWndsize,bgWndsize,bwndmean);
				if(lError!=0)
					return;
				lError=GetCovarianceMatrix(bgwndPixels,bands,NULL,bgWndsize,bgWndsize,bcovMat);
				if(lError!=0)
					return;
				lError=MatrixInverse(bcovMat,bands,bInvcovMat);
				if(lError!=0)
					return;
				lError=MatrixEigen_value_vec_Jccob(bInvcovMat,eigValue,bands,eigMat);
				if(lError!=0)
					return;


				//��ȡ�������صľ�ֵ
				//Ŀ�괰���ڱ������ڵ�λ�õ��ƶ�
				for (xtwnd=0;xtwnd<tempbgxWndsize-tWndsize;xtwnd++)
				{
					for (ytwnd=0;ytwnd<tempbgyWndsize-tWndsize;ytwnd++)
					{
						//����Ŀ�괰�ڵľ�ֵ
						for (int i=0;i<tWndsize;i++)
						{
							for (int j=0;j<tWndsize;j++)
							{
								//�����в���
								for (int k=0;k<bands;k++)
								{
									twndmean[k]+=bgwndPixels[(ytwnd+j)*bgWndsize+xtwnd+j+k*bgWndsize*bgWndsize]/double(tWndsize)/double(tWndsize);
								}
							}
						}

						//���������
						for (int i=0;i<bands;i++)
						{
							//û�м�ȥ�����ľ�ֵ
							imgBufferOut[(ybwnd+ytwnd+tWndsize/2)*xsize+xbwnd+xtwnd+tWndsize/2]+=(twndmean[i]-bwndmean[i])*eigMat[i][0];
						}
						memset(twndmean,0,sizeof(double)*bands);

					}
				}//����������������������

				memset(bwndmean,0,sizeof(double)*bands);
				delete[]tempPixels;tempPixels=NULL;
			}
			else if (xwndsize<bgWndsize&&ywndsize>bgWndsize)
			{
				int tempbgxWndsize=xwndsize;
				int tempbgyWndsize=bgWndsize;
				double *tempPixels =NULL;
				tempPixels=new double[tempbgxWndsize*tempbgyWndsize];
				tempPixels=GetpartImage(imgBufferIn,bands,xsize,ysize,xbwnd,ybwnd,tempbgxWndsize,tempbgyWndsize);

				//��ȡ��һ���������� ����㱳��������������
				if(tempPixels==NULL)
					return;

				long lError=0;
				//���㱳��Ӱ���Э�������ֵ������������ֵ
				lError=GetImgMean(bgwndPixels,bands,NULL,bgWndsize,bgWndsize,bwndmean);
				if(lError!=0)
					return;
				lError=GetCovarianceMatrix(bgwndPixels,bands,NULL,bgWndsize,bgWndsize,bcovMat);
				if(lError!=0)
					return;
				lError=MatrixInverse(bcovMat,bands,bInvcovMat);
				if(lError!=0)
					return;
				lError=MatrixEigen_value_vec_Jccob(bInvcovMat,eigValue,bands,eigMat);
				if(lError!=0)
					return;


				//��ȡ�������صľ�ֵ
				//Ŀ�괰���ڱ������ڵ�λ�õ��ƶ�
				for (xtwnd=0;xtwnd<tempbgxWndsize-tWndsize;xtwnd++)
				{
					for (ytwnd=0;ytwnd<tempbgyWndsize-tWndsize;ytwnd++)
					{
						//����Ŀ�괰�ڵľ�ֵ
						for (int i=0;i<tWndsize;i++)
						{
							for (int j=0;j<tWndsize;j++)
							{
								//�����в���
								for (int k=0;k<bands;k++)
								{
									twndmean[k]+=bgwndPixels[(ytwnd+j)*bgWndsize+xtwnd+j+k*bgWndsize*bgWndsize]/double(tWndsize)/double(tWndsize);
								}
							}
						}

						//���������
						for (int i=0;i<bands;i++)
						{
							//û�м�ȥ�����ľ�ֵ
							imgBufferOut[(ybwnd+ytwnd+tWndsize/2)*xsize+xbwnd+xtwnd+tWndsize/2]+=(twndmean[i]-bwndmean[i])*eigMat[i][0];
						}
						memset(twndmean,0,sizeof(double)*bands);

					}
				}//����������������������

				memset(bwndmean,0,sizeof(double)*bands);
				delete[]tempPixels;tempPixels=NULL;
			}
			else
			{
				//��ȡ��һ���������� ����㱳��������������
				bgwndPixels=GetpartImage(imgBufferIn,bands,xsize,ysize,xbwnd,ybwnd,bgWndsize,bgWndsize);
				if(bgwndPixels==NULL)
					return;

				long lError=0;
				//���㱳��Ӱ���Э�������ֵ������������ֵ
				lError=GetImgMean(bgwndPixels,bands,NULL,bgWndsize,bgWndsize,bwndmean);
				if(lError!=0)
					return;
				lError=GetCovarianceMatrix(bgwndPixels,bands,NULL,bgWndsize,bgWndsize,bcovMat);
				if(lError!=0)
					return;
				//ofstream ofs("test.txt");
				//for (int i=0;i<bands;i++)
				//{
				//	for (int j=0;j<bands;j++)
				//	{
				//		ofs<<bcovMat[i][j]<<"  ";
				//	}
				//	ofs<<endl;
				//}
				//ofs.close();
				lError=MatrixInverse(bcovMat,bands,bInvcovMat);
				if(lError!=0)
					return;
				lError=MatrixEigen_value_vec_Jccob(bInvcovMat,eigValue,bands,eigMat);
				if(lError!=0)
					return;

				//��ȡ�������صľ�ֵ
				//Ŀ�괰���ڱ������ڵ�λ�õ��ƶ�
				for (xtwnd=0;xtwnd<bgWndsize-tWndsize;xtwnd++)
				{
					for (ytwnd=0;ytwnd<bgWndsize-tWndsize;ytwnd++)
					{
						//����Ŀ�괰�ڵľ�ֵ
						for (int i=0;i<tWndsize;i++)
						{
							for (int j=0;j<tWndsize;j++)
							{
								//�����в���
								for (int k=0;k<bands;k++)
								{
									twndmean[k]+=bgwndPixels[(ytwnd+j)*bgWndsize+xtwnd+j+k*bgWndsize*bgWndsize]/double(tWndsize)/double(tWndsize);
								}
							}
						}

						//���������
						for (int i=0;i<bands;i++)
						{
							//û�м�ȥ�����ľ�ֵ
							imgBufferOut[(ybwnd+ytwnd+tWndsize/2)*xsize+xbwnd+xtwnd+tWndsize/2]+=(twndmean[i]-bwndmean[i])*eigMat[i][0];
						}
						memset(twndmean,0,sizeof(double)*bands);

					}
				}//����������������������

				memset(bwndmean,0,sizeof(double)*bands);
			}
		}
	}//���б�����������

	//������б���
	delete[]bwndmean;bwndmean=NULL;
	delete[]twndmean;twndmean=NULL;
	delete[]bgwndPixels;bgwndPixels=NULL;
	delete[]eigValue;eigValue=NULL;

	for (int i =0;i<bands;i++)
	{
		delete[]bcovMat[i];
		delete[]bInvcovMat[i];
		delete[]eigMat[i];
	}
	delete[]bcovMat;
	delete[]bInvcovMat;
	delete[]eigMat;

}

/*----------------------------GDAL��װ----------------------------------*/
void RXD(char* pathSrc,char* pathDst,int bgWndsize,int tWndsize)
{
	GDALAllRegister();
	GDALDatasetH m_datasetIn;
	GDALDatasetH m_datasetOut;

	m_datasetIn=GDALOpen(pathSrc,GA_ReadOnly);
	int bands=GDALGetRasterCount(m_datasetIn);
	int xsize=GDALGetRasterXSize(m_datasetIn);
	int ysize=GDALGetRasterYSize(m_datasetIn);

	double *dataIn=new double[xsize*ysize*bands];
	double *dataOut=new double[xsize*ysize];
	for (int i=0;i<bands;i++)
	{
		GDALRasterIO(GDALGetRasterBand(m_datasetIn,i+1),GF_Read,0,0,xsize,ysize,dataIn+i*xsize*ysize,xsize,ysize,GDT_Float64,0,0);
	}

	RXD(dataIn,dataOut,xsize,ysize,bands,bgWndsize,tWndsize);

	const char* proj=GDALGetProjectionRef(m_datasetIn);
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	m_datasetOut=GDALCreate(GDALGetDriverByName("HFA"),pathDst,xsize,ysize,1,GDT_Float64,papszOptions);
	GDALRasterIO(GDALGetRasterBand(m_datasetOut,1),GF_Write,0,0,xsize,ysize,dataOut,xsize,ysize,GDT_Float64,0,0);
	GDALSetProjection(m_datasetOut,proj);
	GDALClose(m_datasetIn);
	GDALClose(m_datasetOut);
	delete[]dataIn;
	delete[]dataOut;

}

/********************************************************************************************************/
/*						                        FastICA                                                 */
/********************************************************************************************************/
//tan������ת��
long g_tanhFunction(double* w,double* x_imput,int bands,int xsize,int ysize,double* x_output)
{
	double temp=0;
	if(x_output==NULL)
		return -1;
	for (int i=0;i<xsize*ysize;i++)
	{
		temp=0;
		for (int j=0;j<bands;j++)
		{
			temp+=w[j]*x_imput[j*xsize*ysize+i];
		}
		x_output[i]=tanh(temp);
	}
	return 0;
}
long g_derivative_tanh(double* w,double* x_imput,int bands,int xsize,int ysize,double* x_output)
{
	double *wtemp=NULL;
	double temp=0;
	wtemp   =new double [bands];
	if(x_output==NULL)
		return -1;
	for (int i=0;i<bands;i++)
	{
		wtemp[i]=tanh(w[i]);
	}
	for (int i=0;i<xsize*ysize;i++)
	{
		temp=0;
		for (int j=0;j<bands;j++)
		{
			temp+=wtemp[j]*x_imput[j*xsize*ysize+i];
		}
		x_output[i]=1-temp*temp;
	}

	delete[]wtemp;
	return 0;
}

//�ݺ�����ת��
long g_power3Function(double* w,double* x_imput,int bands,int xsize,int ysize,double* x_output)
{
	if(x_output==NULL)
		return -1;
	for (int i=0;i<xsize*ysize;i++)
		x_output[i]=x_imput[i]*x_imput[i]*x_imput[i];
	return 0;
}
long g_derivative_power3(double* w,double* x_imput,int bands,int xsize,int ysize,double* x_output)
{
	if(x_output==NULL)
		return -1;
	for (int i=0;i<xsize*ysize;i++)
		x_output[i]=3.0f*x_imput[i]*x_imput[i];
	return 0;
}

/*-----------------------------FastICA�㷨-------------------------*/
long FastICA(double* imgBufferIn,int xsize,int ysize,int bandSum,int IpNum,double* imgBufferOut)
{
	if(IpNum>bandSum)
		return -1;
	if(imgBufferIn==NULL||imgBufferOut==NULL)
		return -2;

	int iteratorNum;
	int maxIterator=100;
	double sumline=0;

	//���ݰ׻�
	double* imgBufferWhiten=new double[xsize*ysize*bandSum];
	//memcpy(imgBufferWhiten,imgBufferIn,sizeof(double)*xsize*ysize*bandSum);
	SphereTrans(imgBufferIn,bandSum,NULL,xsize,ysize,imgBufferWhiten);

	//Ȩ�ؾ���
	double* weighMat=new double[bandSum*IpNum];
	double* weighp  =new double[bandSum];memset(weighp,0,sizeof(double)*bandSum);
	double* weighpp =new double[bandSum];memset(weighpp,0,sizeof(double)*bandSum);
	double* lastwp  =new double[bandSum];memset(lastwp,0,sizeof(double)*bandSum);

	double* tempadd  =new double[bandSum];
	double* tempminus=new double[bandSum];

	double *temp1=new double[xsize*ysize];memset(temp1,0,sizeof(double)*xsize*ysize);
	double *temp2=new double[xsize*ysize];memset(temp2,0,sizeof(double)*xsize*ysize);

	//���Ȩ�غ���
	double temp=(double)rand();
	for (int i=0;i<IpNum*bandSum;i++)
		weighMat[i]= (double)rand() / RAND_MAX ;

	for (int i=0;i<IpNum;i++)
	{
		sumline=0;
		iteratorNum=0;
		memset(lastwp,0,sizeof(double)*bandSum);
		memset(weighp,0,sizeof(double)*bandSum);
		memset(weighpp,0,sizeof(double)*bandSum);

		//��ȡ��i��Ȩ��
		for (int j=0;j<bandSum;j++)
		{
			weighp[j]=weighMat[j*IpNum+i];
			sumline +=weighMat[j*IpNum+i]*weighMat[j*IpNum+i];
		}

		//��һ����һ��
		sumline=sqrt(sumline);
		for (int j=0;j<bandSum;j++)
			weighMat[j*IpNum+i]/=sumline;

		double maxadd=0;
		double maxminu=0;

		do
		{
		    printf("\r����%d��",iteratorNum);
			memset(tempadd,0,sizeof(bandSum));
			memset(tempminus,0,sizeof(bandSum));

			iteratorNum++;
			memcpy(lastwp,weighp,sizeof(double)*bandSum);

			for (int j=0;j<bandSum;j++)
			{
				double mean1=0.0f,mean2=0.0f;

				memset(temp1,0,sizeof(double)*xsize*ysize);
				memset(temp2,0,sizeof(double)*xsize*ysize);

				g_tanhFunction(lastwp,imgBufferWhiten,bandSum,xsize,ysize,temp1);
				for (int k=0;k<xsize*ysize;k++)
				{
					temp1[k]=temp1[k]*imgBufferWhiten[k+j*xsize*ysize];
				}
				g_derivative_tanh(lastwp,imgBufferWhiten,bandSum,xsize,ysize,temp2);

				GetImgMean(temp1,1,NULL,xsize,ysize,&mean1);
				GetImgMean(temp2,1,NULL,xsize,ysize,&mean2);
				mean2*=lastwp[j];
				weighp[j]=mean1-mean2;
			}

			memset(weighpp,0,sizeof(double)*bandSum);
			for (int j=0;j<=i-1;j++)
			{
				double temp=0;
				for (int l=0;l<bandSum;l++)
				{
					temp+=weighp[l]*weighMat[l*IpNum+j];
				}

				for (int l=0;l<bandSum;l++)
				{
					weighpp[l]+=temp*weighMat[l*IpNum+j];
				}
			}

			sumline=0;
			for (int j=0;j<bandSum;j++)
			{
				weighp[j]=weighp[j]-weighpp[j];
				sumline+=weighp[j]*weighp[j];
			}
			for(int j=0;j<bandSum;j++)
				weighp[j]/=sqrt(sumline);
			for (int j=0;j<bandSum;j++)
			{
				tempadd[j]  =lastwp[j]+weighp[j];
				tempminus[j]=lastwp[j]-weighp[j];
			}

			maxadd =*max_element(tempadd,tempadd+bandSum);
			maxminu=*max_element(tempminus,tempminus+bandSum);

		} while (abs(maxminu)>0.01/*&&abs(maxadd)>0.0001*/&&iteratorNum<maxIterator);

		for (int j=0;j<bandSum;j++)
		{
			weighMat[j*IpNum+i]=weighp[j];
		}
	}
    printf("\n");

	//����ת��
	memset(imgBufferOut,0,sizeof(double)*xsize*ysize*IpNum);
	for (int j=0;j<xsize*ysize;j++)
	{
		for (int k=0;k<IpNum;k++)
		{
			for (int i=0;i<bandSum;i++)
			{
				imgBufferOut[k*xsize*ysize+j]+=imgBufferWhiten[i*xsize*ysize+j]*weighMat[i*IpNum+k];
			}
		}
	}

	//���ָ��
	delete[]weighMat;	weighMat=NULL;
	delete[]weighp;		weighp =NULL;
	delete[]weighpp;	weighpp=NULL;
	delete[]lastwp;		lastwp=NULL;
	delete[]imgBufferWhiten;imgBufferWhiten=NULL;

	delete[]tempadd;	tempadd=NULL;
	delete[]tempminus;	tempminus=NULL;

	delete[]temp1;temp1=NULL;
	delete[]temp2;temp2=NULL;

	return 0;

}

/*---------------------FastICA�㷨GDAL��װ-------------------------*/
void FastICA(char* pathSrc,char* pathDst,int IpNum)
{
	GDALAllRegister();
	GDALDatasetH m_datasetIn;
	GDALDatasetH m_datasetOut;

	m_datasetIn=GDALOpen(pathSrc,GA_ReadOnly);
	int bands=GDALGetRasterCount(m_datasetIn);
	int xsize=GDALGetRasterXSize(m_datasetIn);
	int ysize=GDALGetRasterYSize(m_datasetIn);

	if (IpNum>bands)
		return ;

	double *dataIn=new double[xsize*ysize*bands];
	double *dataOut=new double[xsize*ysize*IpNum];
	for (int i=0;i<bands;i++)
	{
		GDALRasterIO(GDALGetRasterBand(m_datasetIn,i+1),GF_Read,0,0,xsize,ysize,dataIn+i*xsize*ysize,xsize,ysize,GDT_Float64,0,0);
	}
	FastICA(dataIn,xsize,ysize,bands,IpNum,dataOut);

	const char* proj=GDALGetProjectionRef(m_datasetIn);
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	m_datasetOut=GDALCreate(GDALGetDriverByName("HFA"),pathDst,xsize,ysize,IpNum,GDT_Float64,papszOptions);
	for (int i=0;i<IpNum;i++)
	{
		GDALRasterIO(GDALGetRasterBand(m_datasetOut,i+1),GF_Write,0,0,xsize,ysize,dataOut+i*xsize*ysize,xsize,ysize,GDT_Float64,0,0);
	}
	GDALSetProjection(m_datasetOut,proj);

	GDALClose(m_datasetIn);
	GDALClose(m_datasetOut);
}


/********************************************************************************************************/
/*						                   FastFinder�㷨                                               */
/********************************************************************************************************/
//���㵥�������
long SingleVloumeFunc(double* data,double*dataCal,double* dataTra,double* dataMul,int bands,int number,double&det)
{
	//���㷨 ���������ݽ������� ��ֹ����Խ��
	if(data==NULL||dataCal==NULL||dataTra==NULL||dataMul==NULL)
		return -1;

	det     =0;		//��������ʽ

	//��ԭʼ���ݽ��й�һ��
	double* tempdata=new double[bands*number];
	for (int i=0;i<number*bands;i++)
	{
		tempdata[i]=data[i]/10.0f;
	}

	double total=0.0f;
	long lError   =0;

	for (int i=0;i<number-1;i++)
	{
		for (int j=0;j<bands;j++)
		{
			dataCal[i*bands+j]=tempdata[(i+1)*bands+j]-tempdata[j];
		}
	}

	//������
 	lError=MatrixTrans(dataCal,bands,number-1,dataTra);
 	lError=MatrixMuti(dataTra,number-1,bands,number-1,dataCal,dataMul);

 	//��һ������ʹ���Ƚ��ȶ�
//  	for (int i=0;i<(number-1)*(number-1);i++)
//  		total+=dataMul[i];
//  	for (int i=0;i<(number-1)*(number-1);i++)
//  		dataMul[i]=dataMul[i]*100*(number-1)*(number-1)/total;

 	lError=MatrixDet(dataMul,number-1,det);
	delete[]tempdata;
	return 0;
}

//fast finder�㷨���ж�Ԫ��ȡ
long FastFinder(double* imgBuffer,double* endmember,int xsize,int ysize,int bandSum,int endMemberNum)
{
	long LError=0;
	if (imgBuffer==NULL||endmember==NULL)
		return -1;

	//���������Ԫ
	double det=0;			//���������
	double iterdet=0;		//�������������

	double* randEndmember=NULL;
	double* tempEnd=NULL;
	double* temp=NULL;

	double *dataCal=NULL;	//����
	double *dataTra=NULL;	//����ת��
	double *dataMul=NULL;	//����˻�

	if((dataTra=new double[bandSum*(endMemberNum-1)])==NULL)
		return -1;
	if((dataMul=new double[(endMemberNum-1)*(endMemberNum-1)])==NULL)
		return -1;
	if((dataCal=new double[bandSum*(endMemberNum-1)])==NULL)
		return -1;
	tempEnd=new double[bandSum];
	temp=new double[bandSum];
	randEndmember=new double[bandSum*endMemberNum];

	rand();
	//�����ȡλ�����ݼ����Ԫ����
	for (int i=0;i<endMemberNum;i++)
	{
		int pos=rand()%xsize*ysize;
		for (int j=0;j<bandSum;j++)
		{
			randEndmember[i*bandSum+j]=imgBuffer[j*xsize*ysize+pos];
		}
	}

	//���㵥�������
	LError=SingleVloumeFunc(randEndmember,dataCal,dataTra,dataMul,bandSum,endMemberNum,det);
	if(LError!=0)
		return -2;

	//��������Ӱ���ȡ����
	int maxMember=0;
	for (int i=0;i<xsize*ysize;i++)
	{
		printf("\r��������%d/%d...",xsize*ysize,i+1);
		//��ȡ�õ�Ӱ�����
		for (int j=0;j<bandSum;j++)
			tempEnd[j]=imgBuffer[j*xsize*ysize+i];

		//�ֱ��������Ӱ�񲨶λ�ȡ������
		iterdet=det;
		maxMember=-1;
		for (int j=0;j<endMemberNum;j++)
		{
// 			for(int k=0;k<bandSum;k++)
// 			{
// 				temp[k]=randEndmember[j*bandSum+k];
// 				randEndmember[j*bandSum+k]=tempEnd[k];
// 			}
			memcpy(temp,randEndmember+j*bandSum,sizeof(double)*bandSum);
			memcpy(randEndmember+j*bandSum,tempEnd,sizeof(double)*bandSum);
			LError=SingleVloumeFunc(randEndmember,dataCal,dataTra,dataMul,bandSum,endMemberNum,det);
			if(LError!=0)
				return -2;
			if(det>iterdet)
			{
				maxMember=j;
			}
			iterdet = det;

			//for(int k=0;k<bandSum;k++)
			//	randEndmember[j*bandSum+k]=temp[k];
			memcpy(randEndmember+j*bandSum,temp,sizeof(double)*bandSum);
		}

		//������ֵ�����������Ķ�Ԫ
		if(maxMember!=-1)
		{
			for(int k=0;k<bandSum;k++)
				randEndmember[maxMember*bandSum+k]=tempEnd[k];
		}

	}
	printf("\n");

	//��ȡ�����
	memcpy(endmember,randEndmember,sizeof(double)*bandSum*endMemberNum);

	//�������
	delete[]randEndmember;randEndmember=NULL;
	delete[]tempEnd;tempEnd=NULL;
	delete[]temp;temp=NULL;
	delete[]dataCal;dataCal=NULL;
	delete[]dataMul;dataMul=NULL;
	delete[]dataTra;dataTra=NULL;

	return 0;
}

//GDAL��װ
void FastFinder(char* pathImg,int endMemberNum,char* pathOut)
{
	GDALAllRegister();

	GDALDatasetH m_dataset=GDALOpen(pathImg,GA_ReadOnly);
	int xsize=GDALGetRasterXSize(m_dataset);
	int ysize=GDALGetRasterYSize(m_dataset);
	int bands=GDALGetRasterCount(m_dataset);

	//���ݿռ������
	try
	{
		double *testdata=NULL;
		testdata=new double[xsize*ysize*bands];
		if(testdata==NULL)
			throw(1);
		else
		{
			delete[]testdata;
			testdata=NULL;
		}
	}
	catch (int)
	{
		printf("memory limited��");
		exit(0);
	}

	//�ڴ��㹻
	double* imgBuffer=new double[xsize*ysize*bands];
	double* endmember=new double[bands*endMemberNum];
	//��ȡ����
	for (int i=0;i<bands;i++)
	{
		printf("\r��ȡ������%d/%d...",bands,i+1);
		GDALRasterIO(GDALGetRasterBand(m_dataset,i+1),GF_Read,0,0,xsize,ysize,imgBuffer+i*xsize*ysize,xsize,ysize,GDT_Float64,0,0);
	}
	printf("\n");
	printf("������...\n");

	FastFinder(imgBuffer,endmember,xsize,ysize,bands,endMemberNum);

	//�����Ԫ����
	EndMember_Export_File(pathOut,endmember,bands,endMemberNum);
}

/********************************************************************************************************/
/*						                   SVDӰ��ֽ��㷨                                              */
/********************************************************************************************************/
long SVD(double *dataIn,int bands,int xsize,int ysize,char* pathS,char* pathV,char* pathD)
{
	if(dataIn==NULL)
		return -1;
	GDALAllRegister();
	double* datas =new double[xsize*xsize];
	double* datav =new double[xsize*ysize];
	double* datad =new double[ysize*ysize];
	double* datat =new double[xsize*ysize];
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	GDALDatasetH m_datasets=GDALCreate(GDALGetDriverByName("GTiff"),pathS,xsize,xsize,bands,GDT_Float64,papszOptions);
	GDALDatasetH m_datasetv=GDALCreate(GDALGetDriverByName("GTiff"),pathV,xsize,xsize,bands,GDT_Float64,papszOptions);
	GDALDatasetH m_datasetd=GDALCreate(GDALGetDriverByName("GTiff"),pathD,xsize,xsize,bands,GDT_Float64,papszOptions);

	for (int i=0;i<bands;i++)
	{
		printf("\r������%d/%d",bands,i+1);
		memset(datas,0,sizeof(double)*xsize*xsize);
		memset(datav,0,sizeof(double)*xsize*ysize);
		memset(datad,0,sizeof(double)*ysize*ysize);
		memcpy(datat,dataIn+i*xsize*ysize,sizeof(double)*xsize*ysize);

		//���зֽ�
		Matrix_SVD(dataIn,datas,datav,datad,xsize,ysize);
		
		//������
		GDALRasterIO(GDALGetRasterBand(m_datasets,i+1),GF_Read,0,0,xsize,xsize,datas,xsize,xsize,GDT_Float64,0,0);
		GDALRasterIO(GDALGetRasterBand(m_datasetv,i+1),GF_Read,0,0,xsize,xsize,datav,xsize,ysize,GDT_Float64,0,0);
		GDALRasterIO(GDALGetRasterBand(m_datasetd,i+1),GF_Read,0,0,ysize,ysize,datad,ysize,ysize,GDT_Float64,0,0);
	}
	printf("\n");
	GDALClose(m_datasets);
	GDALClose(m_datasetv);
	GDALClose(m_datasetd);

	delete[]datas;
	delete[]datav;
	delete[]datad;
	delete[]datat;

	return 0;
}

long SVDTrans(char* pathIn,int *bandNo,char* pathS,char* pathV,char* pathD)
{
	GDALAllRegister();
	CPLSetConfigOption( "GDAL_FILENAME_IS_UTF8", "NO" );	//����·��

	//ѡȡ���в���
	if (bandNo==NULL)
	{
		GDALDatasetH m_dataset=GDALOpen(pathIn,GA_ReadOnly);
		int xsize=GDALGetRasterXSize(m_dataset);
		int ysize=GDALGetRasterYSize(m_dataset);
		int bands=GDALGetRasterCount(m_dataset);

		double *datain=new double[xsize*ysize*bands];
		for(int i=0;i<bands;i++)
			GDALRasterIO(GDALGetRasterBand(m_dataset,i+1),GF_Read,0,0,xsize,ysize,datain,xsize,ysize,GDT_Float64,0,0);
		GDALClose(m_dataset);
		SVD(datain,bands,xsize,ysize,pathS,pathV,pathD);
		delete[]datain;
	}
	else
	{
		GDALDatasetH m_dataset=GDALOpen(pathIn,GA_ReadOnly);
		int xsize=GDALGetRasterXSize(m_dataset);
		int ysize=GDALGetRasterYSize(m_dataset);
		int bands=GDALGetRasterCount(m_dataset);

		int numbands=0;
		for (int i=0;i<bands;i++)
			if(bandNo[i]!=0)
				numbands++;

		double *datain=new double[xsize*ysize*numbands];
		for (int i=0,j=0;i<bands;i++)
		{
			if(bandNo[i]!=0)
			{
				GDALRasterIO(GDALGetRasterBand(m_dataset,i+1),GF_Read,0,0,xsize,ysize,datain+j*xsize*ysize,xsize,ysize,GDT_Float64,0,0);
				j++;
			}
		}
		GDALClose(m_dataset);
		SVD(datain,numbands,xsize,ysize,pathS,pathV,pathD);
		delete[]datain;

	}
	return 0;
}
/********************************************************************************************************/
/*						                   OSPӰ��ͶӰ����                                              */
/********************************************************************************************************/
long  OSP(double* dataIn,int xsize,int ysize,int bands,double* transMat,int matx,int maty,double* dataOut)
{
	//�������
	if(dataIn==NULL||transMat==NULL||dataOut==NULL)
		return -1;
	//�������
	MatrixMuti(transMat,matx,bands,xsize*ysize,dataIn,dataOut);
	return 0;
}

long  OSPTrans(char* pathIn,double* transMat,int matx,int maty,char* pathOut)
{
	//�������
	if(pathIn==NULL||pathOut==NULL)
		return -1;

	GDALAllRegister();
	GDALDatasetH m_dataset=GDALOpen(pathIn,GA_ReadOnly);
	int xsize=GDALGetRasterXSize(m_dataset);
	int ysize=GDALGetRasterYSize(m_dataset);
	int bands=GDALGetRasterCount(m_dataset);

	//���ݿռ�����
	double* dataIn =new double[xsize*ysize*bands];
	double* dataOut=new double[xsize*ysize*matx];
	for(int i=0;i<bands;i++)
		GDALRasterIO(GDALGetRasterBand(m_dataset,i+1),GF_Read,0,0,xsize,ysize,dataIn+i*xsize*ysize,xsize,ysize,GDT_Float64,0,0);
	GDALClose(m_dataset);
	OSP(dataIn,xsize,ysize,bands,transMat,matx,maty,dataOut);
	
	//����������ݼ�
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	GDALDatasetH m_dataout=GDALCreate(GDALGetDriverByName("GTiff"),pathOut,xsize,ysize,matx,GDT_Float64,papszOptions);
	for(int i=0;i<matx;i++)
		GDALRasterIO(GDALGetRasterBand(m_dataout,i+1),GF_Write,0,0,xsize,ysize,dataOut+i*xsize*ysize,xsize,ysize,GDT_Float64,0,0);
	GDALClose(m_dataout);

	delete[]dataIn;
	delete[]dataOut;

	return 0;
}

//endMat��BIP��ʽ�洢
long  GetProjMat(double* endMat,int endNumber,int bands,int selEndIdx,double* selEnd,double* p)
{
	double* endMat_1;
	
	//��ȡ��ȡ���Ķ�Ԫ
	for (int i=0;i<bands;++i)
		selEnd[i]=endMat[i*endNumber+selEndIdx];
	
	//��ȡ�����޳��Ķ�Ԫ֮��Ķ�Ԫ
	endMat_1=new double[bands*(endNumber-1)];
	memset(endMat_1,0,sizeof(double)*bands*(endNumber-1));

	for (int i=0;i<bands;++i)
	{
		int iterator=0;
		for (int j=0;j<endNumber;++j)
		{
			if(j!=selEndIdx)
			{
				endMat_1[i*(endNumber-1)+iterator]=endMat[i*endNumber+j];
				iterator++;
			}
		}
	}

	//������α��
	double *endMat_1T  =new double[bands*(endNumber-1)];
	double *endMat_1T1 =new double[(endNumber-1)*(endNumber-1)];
	double *endMat_inv =new double[(endNumber-1)*(endNumber-1)];
	double *endMat_pinv=new double[(endNumber-1)*(bands)];

	MatrixTrans(endMat_1,bands,endNumber-1,endMat_1T);
	MatrixMuti(endMat_1T,endNumber-1,bands,endNumber-1,endMat_1,endMat_1T1);
	MatrixInverse(endMat_1T1,endNumber-1,endMat_inv);

	//���Դ���
	//double *endMatTest =new double[(endNumber-1)*(endNumber-1)];
	//MatrixMuti(endMat_inv,endNumber-1,endNumber-1,endNumber-1,endMat_1T1,endMatTest);
	//delete[]endMatTest;

	MatrixMuti(endMat_inv,endNumber-1,endNumber-1,bands,endMat_1T,endMat_pinv);
	MatrixMuti(endMat_1,bands,endNumber-1,bands,endMat_pinv,p);
	for (int i=0;i<bands;++i)
	{
		for (int j=0;j<bands;++j)
		{
			if(i==j)
				p[i*bands+j]=1-p[i*bands+j];
			else
				p[i*bands+j]=-p[i*bands+j];
		}
	}

	if(endMat_1T!=NULL)
		delete[]endMat_1T;
	if(endMat_1T1!=NULL)
		delete[]endMat_1T1;
	if(endMat_inv!=NULL)
		delete[]endMat_inv;
	if(endMat_pinv!=NULL)
		delete[]endMat_pinv;
	if(endMat_1!=NULL)
		delete[]endMat_1;

	return 0;
}
long  GetProjMatTilt(double* endMat,int endNumber,int bands,int selEndIdx,double* selEnd,double* p)
{
	double* endMat_1;

	//��ȡ��ȡ���Ķ�Ԫ
	for (int i=0;i<bands;++i)
		selEnd[i]=endMat[i*endNumber+selEndIdx];

	//��ȡ�����޳��Ķ�Ԫ֮��Ķ�Ԫ
	double* endMat_tmp=new double[bands*(endNumber-1)];
	endMat_1=new double[bands*(endNumber-1)];
	memset(endMat_1,0,sizeof(double)*bands*(endNumber-1));

	for (int i=0;i<bands;++i)
	{
		int iterator=0;
		for (int j=0;j<endNumber;++j)
		{
			if(j!=selEndIdx)
			{
				endMat_1[i*(endNumber-1)+iterator]=endMat[i*endNumber+j];
				iterator++;
			}
		}
	}
	MatrixTrans(endMat_1,bands,endNumber-1,endMat_tmp);
	memcpy(endMat_1,endMat_tmp,sizeof(double)*(endNumber-1)*bands);
	if(endMat_tmp!=NULL)
		delete[]endMat_tmp;


	//������α��
	double *endMat_1T  =new double[bands*(endNumber-1)];
	double *endMat_1T1 =new double[(endNumber-1)*(endNumber-1)];
	double *endMat_inv =new double[(endNumber-1)*(endNumber-1)];
	double *endMat_pinv=new double[(endNumber-1)*(bands)];

	MatrixTrans(endMat_1,endNumber-1,bands,endMat_1T);
	MatrixMuti(endMat_1,endNumber-1,bands,endNumber-1,endMat_1T,endMat_1T1);
	MatrixInverse(endMat_1T1,endNumber-1,endMat_inv);

	MatrixMuti(endMat_inv,endNumber-1,endNumber-1,bands,endMat_1,endMat_pinv);
	MatrixMuti(endMat_1T,bands,endNumber-1,bands,endMat_pinv,p);
	for (int i=0;i<bands;++i)
	{
		for (int j=0;j<bands;++j)
		{
			if(i==j)
				p[i*bands+j]=1-p[i*bands+j];
			else
				p[i*bands+j]=-p[i*bands+j];
		}
	}

	if(endMat_1T!=NULL)
		delete[]endMat_1T;
	if(endMat_1T1!=NULL)
		delete[]endMat_1T1;
	if(endMat_inv!=NULL)
		delete[]endMat_inv;
	if(endMat_pinv!=NULL)
		delete[]endMat_pinv;
	if(endMat_1!=NULL)
		delete[]endMat_1;

	return 0;
}
//������Ԫ�Ͷ�Ԫ����ݷ��
double  GetAbundance(double* selEnd,double* p,double *pixelElement,int bands)
{
	double *dtp		=new double[bands];
	double dtpe;
	double belta;

	MatrixMuti(selEnd,1,bands,bands,p,dtp);
	MatrixMuti(dtp,1,bands,1,selEnd,&belta);
	belta=1.0/belta;
	MatrixMuti(dtp,1,bands,1,pixelElement,&dtpe);

	if(dtp!=NULL)
		delete[]dtp;

	return belta*dtpe;
}
double  GetAbundanceTilt(double* selEnd,double* p,double *pixelElement,int bands)
{
	double *ap=new double[bands];
	double *cp=new double[bands];
	double *cpInv=new double[bands];
	double rs=0;
	MatrixMuti(pixelElement,1,bands,bands,p,ap);
	MatrixMuti(selEnd,1,bands,bands,p,cp);
	Matrix_GenInverse(cp,1,bands,cpInv);
	MatrixMuti(ap,1,bands,1,cpInv,&rs);

	if(ap!=NULL)
		delete[]ap;
	if(cp!=NULL)
		delete[]cp;
	if(cpInv!=NULL)
		delete[]cpInv;
	
	return rs;
}

long  OSPUnmix(double* dataIn,int xsize,int ysize,int bands,double *endMat,int endNumber,double *dataout)
{
	double *p=new double[bands*bands];
	double *selEnd=new double[bands];
	double *pixelData=new double[bands];

	//�Ҿ�����BIP��ʽ�洢�߹������ݸ��������ڸ߹������ݵĴ���
	for (int i=0;i<endNumber;++i)
	{
		//GetProjMatTilt(endMat,endNumber,bands,i,selEnd,p);
		GetProjMat(endMat,endNumber,bands,i,selEnd,p);
		for (int j=0;j<xsize*ysize;++j)
		{
			printf("process band %d, pixel %d\r",i,j);
			for(int k=0;k<bands;++k)
				pixelData[k]=dataIn[k*xsize*ysize+j];
			//dataout[i*xsize*ys5ize+j]=GetAbundanceTilt(selEnd,p,pixelData,bands);
			dataout[i*xsize*ysize+j]=GetAbundance(selEnd,p,pixelData,bands);
			int  temp=0;
		}
	}
	printf("\n");
	if(p!=NULL)
		delete[]p;
	if(selEnd!=NULL)
		delete[]selEnd;
	if(pixelData!=NULL)
		delete[]pixelData;
	return 0;
}
long  OSPUnmix(const char* pathImgIn,const char* pathImgOut,const char* endFile,int endnumber,int bands)
{
	//��ȡӰ������
	GDALAllRegister();
	CPLSetConfigOption( "GDAL_FILENAME_IS_UTF8", "NO" );	//����·��
	GDALDatasetH m_datasetIn=GDALOpen(pathImgIn,GA_ReadOnly);
	int xsize =GDALGetRasterXSize(m_datasetIn);
	int ysize =GDALGetRasterYSize(m_datasetIn);
	int ibands=GDALGetRasterCount(m_datasetIn);
	if(bands!=ibands)
		return -1;		

	double* dataImg=new double[xsize*ysize*bands];
	for (int i=0;i<bands;++i)
		GDALRasterIO(GDALGetRasterBand(m_datasetIn,i+1),GF_Read,0,0,xsize,ysize,dataImg+i*xsize*ysize,xsize,ysize,GDT_Float64,0,0);
	for (int i=0;i<xsize*ysize*bands;++i)
		dataImg[i]=dataImg[i]/10;
	double* endMat=new double[endnumber*bands];
	EndMember_Import_File(endFile,endMat,bands,endnumber);
	double* dataout=new double[xsize*ysize*endnumber];

	OSPUnmix(dataImg,xsize,ysize,bands,endMat,endnumber,dataout);

	GDALDatasetH m_datasetOut=GDALCreate(GDALGetDriverByName("GTiff"),pathImgOut,xsize,ysize,endnumber,GDT_Float64,NULL);
	for (int i=0;i<endnumber;++i)
		GDALRasterIO(GDALGetRasterBand(m_datasetOut,i+1),GF_Write,0,0,xsize,ysize,dataout+i*xsize*ysize,xsize,ysize,GDT_Float64,0,0);
	GDALClose(m_datasetIn);
	GDALClose(m_datasetOut);

	if(dataImg!=NULL)
		delete[]dataImg;
	if(dataout!=NULL)
		delete[]dataout;
	if(endMat!=NULL)
		delete[]endMat;
	return 0;
}
