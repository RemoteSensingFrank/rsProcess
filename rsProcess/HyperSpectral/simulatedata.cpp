#include "fusionfunc.h"
#include <fstream>
#include "..\gdal\include\gdal_priv.h"
#include "..\matrixOperation.h"	//��������
#include "..\rsImgProcess\SampleFunc.h"			//��������
using namespace std;

#pragma warning(disable : 4996)
#pragma comment(lib,"gdal_i.lib")

//��ȡӰ���Ԫ����
float*  get_endmenber_spectralf(char* pathEnd,int bands,int endnumbers)
{
	float *endMemberData=new float[bands*endnumbers];
	ifstream ifs;
	ifs.open(pathEnd,ios_base::in);
	if(ifs.is_open()!=true)
		return NULL;
	char tempchr[2048];
	for(int i=0;i<endnumbers+2;i++)
		ifs.getline(tempchr,2048);

	printf("��ȡ���׿�...\n");
	for (int i=0;i<bands;i++)
	{
		float temp;
		ifs>>temp;
		for (int j=0;j<endnumbers;j++)
		{
			ifs>>temp;
			endMemberData[i*endnumbers+j]=temp;
		}
	}

	return endMemberData;
}
double* get_endmenber_spectrald(char* pathEnd,int bands,int endnumbers)
{
	double *endMemberData=new double[bands*endnumbers];
	ifstream ifs;
	ifs.open(pathEnd,ios_base::in);
	if(ifs.is_open()!=true)
		return NULL;
	char tempchr[2048];
	for(int i=0;i<endnumbers+2;i++)
		ifs.getline(tempchr,2048);

	printf("��ȡ���׿�...\n");
	for (int i=0;i<bands;i++)
	{
		double temp;
		ifs>>temp;
		for (int j=0;j<endnumbers;j++)
		{
			ifs>>temp;
			endMemberData[i*endnumbers+j]=temp;
		}
	}

	return endMemberData;
}

//������Ԫ����
void  export_endmenber_spectral(char* path,double *enddata,int bands,int endnumbers)
{
	ofstream ofs(path,ios_base::out);
	if(ofs.fail()==true)
	{
		printf("������ļ�ʧ�ܣ�");
		return ;
	}

	//���
	time_t curtime;
	curtime=time(NULL);
	ofs<<"ENVI ASCII Plot File:";
	ofs<<ctime(&curtime);
	ofs<<"Column 1 Wavelength"<<endl;
	for (int i=0;i<endnumbers;i++)
		ofs<<"Column "<<i+1<<": Unknown "<<endl;

	//�������
	for (int i=0;i<bands;i++)
	{
		ofs<<i+1<<" ";
		for (int j=0;j<endnumbers;j++)
		{
			ofs<<enddata[i*endnumbers+j]<<" ";
		}
		ofs<<endl;
	}

	ofs.close();

}
void  export_endmenber_spectral(char* path,float *enddata,int bands,int endnumbers)
{
	ofstream ofs(path,ios_base::out);
	if(ofs.fail()==true)
	{
		printf("������ļ�ʧ�ܣ�");
		return ;
	}

	//���
	time_t curtime;
	curtime=time(NULL);
	ofs<<"ENVI ASCII Plot File:";
	ofs<<ctime(&curtime);
	ofs<<"Column 1 Wavelength"<<endl;
	for (int i=0;i<endnumbers;i++)
		ofs<<"Column "<<i+1<<": Unknown "<<endl;

	//�������
	for (int i=0;i<bands;i++)
	{
		ofs<<i+1<<" ";
		for (int j=0;j<endnumbers;j++)
		{
			ofs<<enddata[i*endnumbers+j]<<" ";
		}
		ofs<<endl;
	}

	ofs.close();

}

//������Ӧ����
float CalculateR(float wavelength,float cntlength,float stddev)
{
	float r=0;
	r=(1/(stddev*sqrt(2*PI)))*exp(-((wavelength-cntlength)*(wavelength-cntlength))/(2*stddev*stddev));
	return r;
}
void gauss_response(float* dataHy,float *hyLength,float mLength,float &dataM,int num)
{
	float stddev=0.05f;
	float r=0;
	float updata=0;
	float rdown=0;
	float tempdata=0;
	for (int j=0;j<num;j++)
	{
		r=CalculateR(hyLength[j],mLength,stddev);
		updata+=dataHy[j]*r;
		rdown+=r;
	}
	dataM=updata/rdown;
}
void spectral_response_endmenber(char* pathHy,char* pathM,float* hysp,int bandsHy,float* msp,int bandm,int endnumber,bool range)
{
	//�߹��׺Ͷ���׶�Ԫ�Ŀռ�����
	float *endHyData=new float[bandsHy*endnumber];
	float *endMsData=new float[bandm*endnumber];
	memset(endHyData,0,sizeof(float)*bandsHy*endnumber);
	memset(endMsData,0,sizeof(float)*bandm*endnumber);

	endHyData=get_endmenber_spectralf(pathHy,bandsHy,endnumber);

	//�����벨�β����������
	if(hysp==NULL&&msp==NULL)
	{
		//ֱ�Ӷ�Ӧ���ֵ ���ղ���ƽ������
		int hmMap=bandsHy/bandm;
		for (int i=0;i<bandm;i++)
		{
			for (int k=0;k<endnumber;k++)
			{
				for (int j=0;j<hmMap;j++)
					endMsData[i*endnumber+k]+=endHyData[(i*hmMap+j)*endnumber+k];
				endMsData[i*endnumber+k]/=float(hmMap);
			}
		}
	}

	//�����벨���������  ���Ȼ�ȡ����ײ��ζ�Ӧ�ĸ߹��ײ��� Ȼ����д���
	if(hysp!=NULL&&msp!=NULL)	
	{
		//��ȡ�߹��׶�Ӧ����ײ��εķ���
		int *group=new int[bandsHy];	//�������
		memset(group,0,sizeof(int)*bandsHy);

		int *groupNum=new int[bandm];	//�����һ�����ζ�Ӧ�߹��ײ��θ���

		//�Ը߹��׼���������ζ�Ӧ�Ķ���ײ���
		if(range==true)		//��������ǹ��׷�Χ
		{
			for (int i=0;i<bandsHy;i++)
			{
				for(int j=0;j<bandm;j++)
				{
					if(hysp[j]<msp[2*j+1])
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
			for(int i=0;i<bandsHy;i++)//ֱ��������ǹ���
			{
				int indexm=0;
				float mindata=1000;;
				for (int j=0;j<bandm;j++)
				{
					if(mindata>abs(hysp[i]-msp[j]))
					{
						mindata=abs(hysp[i]-msp[j]);
						indexm=j;
					}
				}
				group[i]=indexm;
			}
		}//�Ը߹������ݷ������

		//ÿ��߹��ײ��ν��м���
		memset(groupNum,0,sizeof(int)*bandm);
		for (int i=0;i<bandsHy;i++)
			groupNum[group[i]]++;

		//��ÿһ����д���õ�����׶�Ԫ
		//��ÿ����Ԫ���д���
		int total=0;
		for (int l=0;l<endnumber;l++)
		{
			for (int i=0;i<bandm;i++)
			{
				float *datahy =new float[groupNum[i]];
				float *datalen=new float[groupNum[i]];
				for (int j=0,k=0;j<bandsHy;j++)
				{
					if(group[j]==i)
					{
						datahy[k]=endHyData[j*endnumber+l];
						datalen[k]=hysp[j];
						k++;
					}
				}
				gauss_response(datahy,datalen,msp[i],endMsData[i*endnumber+l],groupNum[i]);
				delete[]datahy;datahy=NULL;
				delete[]datalen;datalen=NULL;
			}
		}
		delete[]group;group=NULL;
		delete[]groupNum;groupNum=NULL;
	}//�����벨������������

	//��������Ķ�Ԫ
	export_endmenber_spectral(pathM,endMsData,bandm,endnumber);

	//���ָ��
	delete[]endMsData;endMsData=NULL;
	delete[]endHyData;endHyData=NULL;

	return ;
}
void spectral_response_image(char* pathHyImage,char* pathMImage,float* hysp,int bandsHy,float* msp,int bandm,int endnumber,bool range)
{
	//��ȡӰ��
	GDALAllRegister();
	GDALDatasetH m_dataet=GDALOpen(pathHyImage,GA_ReadOnly);
	int xsize=GDALGetRasterXSize(m_dataet);
	int ysize=GDALGetRasterYSize(m_dataet);
	int bands=GDALGetRasterCount(m_dataet);

	//�������Ӱ�����ݼ�
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" ); 
	GDALDatasetH m_datasetout=GDALCreate(GDALGetDriverByName("GTiff"),pathMImage,xsize,ysize,bandm,GDT_Float32,papszOptions);

	//��ȡӰ������
	float* dataHy=new float[xsize*ysize*bands];
	float* dataM =new float[xsize*ysize*bandm];
	for (int i=0;i<bands;i++)
		GDALRasterIO(GDALGetRasterBand(m_dataet,i+1),GF_Read,0,0,xsize,ysize,dataHy+i*xsize*ysize,xsize,ysize,GDT_Float32,0,0);
	GDALClose(m_dataet);

	//�����벨�β����������
	if(hysp==NULL&&msp==NULL)
	{
		//ֱ�Ӷ�Ӧ���ֵ ���ղ���ƽ������
		int hmMap=bandsHy/bandm;
		for (int i=0;i<bandm;i++)
		{
			for (int k=0;k<xsize*ysize;k++)
			{
				for (int j=0;j<hmMap;j++)
					dataHy[i*xsize*ysize+k]+=dataHy[(i*hmMap+j)*xsize*ysize+k];
				dataM[i*xsize*ysize+k]/=float(hmMap);
			}
		}
		//Ӱ�����
		for (int i=0;i<bandm;i++)
			GDALRasterIO(GDALGetRasterBand(m_datasetout,i+1),GF_Write,0,0,xsize,ysize,dataM+i*xsize*ysize,xsize,ysize,GDT_Float32,0,0);
		GDALClose(m_datasetout);

		delete[]dataHy;
		delete[]dataM;
	}

	//�����벨���������  ���Ȼ�ȡ����ײ��ζ�Ӧ�ĸ߹��ײ��� Ȼ����д���
	if(hysp!=NULL&&msp!=NULL)	
	{
		//��ȡ�߹��׶�Ӧ����ײ��εķ���
		int *group=new int[bandsHy];	//�������
		memset(group,0,sizeof(int)*bandsHy);

		int *groupNum=new int[bandm];	//�����һ�����ζ�Ӧ�߹��ײ��θ���

		//�Ը߹��׼���������ζ�Ӧ�Ķ���ײ���
		if(range==true)		//��������ǹ��׷�Χ
		{
			for (int i=0;i<bandsHy;i++)
			{
				for(int j=0;j<bandm;j++)
				{
					if(hysp[j]<msp[2*j+1])
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
			for(int i=0;i<bandsHy;i++)//ֱ��������ǹ���
			{
				int indexm=0;
				float mindata=1000;;
				for (int j=0;j<bandm;j++)
				{
					if(mindata>abs(hysp[i]-msp[j]))
					{
						mindata=abs(hysp[i]-msp[j]);
						indexm=j;
					}
				}
				group[i]=indexm;
			}
		}//�Ը߹������ݷ������

		//ÿ��߹��ײ��ν��м���
		memset(groupNum,0,sizeof(int)*bandm);
		for (int i=0;i<bandsHy;i++)
			groupNum[group[i]]++;

		//��ÿ�����ؽ��д���
		int total=0;
		for (int l=0;l<xsize*ysize;l++)
		{
			printf("\r�߹��׹�����Ӧ�������أ�%d/%d",xsize*ysize,l+1);
			for (int i=0;i<bandm;i++)
			{
				float *datathy=new float[groupNum[i]];
				float *datalen=new float[groupNum[i]];
				for (int j=0,k=0;j<bandsHy;j++)
				{
					if(group[j]==i)
					{
						datathy[k]=dataHy[j*xsize*ysize+l];
						datalen[k]=hysp[j];
						k++;
					}
				}
				gauss_response(datathy,datalen,msp[i],dataM[i*xsize*ysize+l],groupNum[i]);
				delete[]datathy;datathy=NULL;
				delete[]datalen;datalen=NULL;
			}
		}
		printf("\n");


		delete[]group;group=NULL;
		delete[]groupNum;groupNum=NULL;
	}//�����벨������������



	//Ӱ�����
	for (int i=0;i<bandm;i++)
		GDALRasterIO(GDALGetRasterBand(m_datasetout,i+1),GF_Write,0,0,xsize,ysize,dataM+i*xsize*ysize,xsize,ysize,GDT_Float32,0,0);
	GDALClose(m_datasetout);

	delete[]dataHy;
	delete[]dataM;

}

//�ռ����ɢ����������
void spatial_spread_function(char* pathImg,char* pathSample)
{
	CSampleFunc samplefunc;
	samplefunc.gauss_down_sample(pathImg,3,3,pathSample);
	return ;
}

//���ݻָ�Ӱ���ԭʼӰ�������׽����;������
float spectral_angle(float* data1,float* data2,int sizedata,int bands,int pos)
{
	//������׽�
	double absa=0,absb=0,ab=0;
	for (int i=0;i<bands;i++)
	{
		ab+=data1[i*sizedata+pos]*data2[i*sizedata+pos];
		absa+=data1[i*sizedata+pos]*data1[i*sizedata+pos];
		absb+=data1[i*sizedata+pos]*data1[i*sizedata+pos];
	}
	float result=ab/sqrt(absa)/sqrt(absb);

	return acos(result);
}

//������׽Ǻ;���ֵ�в�
void residual_spAngle(char* pathrecovery,char* pathOri,char* pathresidual)
{
	GDALAllRegister();
	GDALDatasetH datasetrec=GDALOpen(pathrecovery,GA_ReadOnly);
	GDALDatasetH datasetori=GDALOpen(pathOri,GA_ReadOnly);

	int xsize=GDALGetRasterXSize(datasetrec);
	int ysize=GDALGetRasterYSize(datasetrec);
	int bands=GDALGetRasterCount(datasetrec);

	//���ݿռ������
	float *datarec=new float[xsize*ysize*bands];
	float *dataori=new float[xsize*ysize*bands];
	float *dataresi=new float[xsize*ysize*2];
	memset(dataresi,0,sizeof(float)*xsize*ysize*2);

	//��ȡ����
	for (int i=0;i<bands;i++)
	{
		GDALRasterIO(GDALGetRasterBand(datasetrec,i+1),GF_Read,0,0,xsize,ysize,datarec+i*xsize*ysize,xsize,ysize,GDT_Float32,0,0);
		GDALRasterIO(GDALGetRasterBand(datasetori,i+1),GF_Read,0,0,xsize,ysize,dataori+i*xsize*ysize,xsize,ysize,GDT_Float32,0,0);
	}
	GDALClose(datasetori);
	GDALClose(datasetrec);

	for (int i=0;i<xsize*ysize;i++)
	{
		printf("\r����ָ��в����أ�%d/%d",xsize*ysize,i+1);
		dataresi[i]=spectral_angle(datarec,dataori,xsize*ysize,bands,i);
		for (int j=0;j<bands;j++)
		{
			dataresi[xsize*ysize+i]+=fabs(datarec[j*xsize*ysize+i]-dataori[j*xsize*ysize+i]);
		}
	}
	printf("\n");

	//���Ӱ��
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" ); 
	GDALDatasetH m_dataresi=GDALCreate(GDALGetDriverByName("GTiff"),pathresidual,xsize,ysize,2,GDT_Float32,papszOptions);
	for (int i=0;i<2;i++)
		GDALRasterIO(GDALGetRasterBand(m_dataresi,i+1),GF_Write,0,0,xsize,ysize,dataresi+i*xsize*ysize,xsize,ysize,GDT_Float32,0,0);
	GDALClose(m_dataresi);

	delete[]datarec;
	delete[]dataresi;
	delete[]dataori;
}

//������
void reduance_total(char* pathRed,char* pathTotal)
{
	GDALAllRegister();
	GDALDatasetH datasetred=GDALOpen(pathRed,GA_ReadOnly);

	int xsize=GDALGetRasterXSize(datasetred);
	int ysize=GDALGetRasterYSize(datasetred);
	int bands=GDALGetRasterCount(datasetred);
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" ); 
	GDALDatasetH datasettotal=GDALCreate(GDALGetDriverByName("GTiff"),pathTotal,xsize,ysize,1,GDT_Float32,papszOptions);
	//���ݿռ������
	float *datared  =new float[xsize*ysize];memset(datared  ,0,sizeof(float)*xsize*ysize);
	float *datatotal=new float[xsize*ysize];memset(datatotal,0,sizeof(float)*xsize*ysize);

	for (int i=0;i<bands;i++)
	{
		printf("\r�������%d",i+1);
		GDALRasterIO(GDALGetRasterBand(datasetred,i+1),GF_Read,0,0,xsize,ysize,datared,xsize,ysize,GDT_Float32,0,0);
		for(int j=0;j<xsize*ysize;j++)
			datatotal[j]+=datared[j];
	}
	printf("\n");
	GDALRasterIO(GDALGetRasterBand(datasettotal,1),GF_Write,0,0,xsize,ysize,datatotal,xsize,ysize,GDT_Float32,0,0);
	GDALClose(datasetred);
	GDALClose(datasettotal);
	delete[]datared;
	delete[]datatotal;
}

//ֱ�Ӹ��ݳ���ģ�͵������ԭʼ�߷ָ߹���Ӱ��
//������ȥ ���Ǿ���Ч��̫���� ��̫�ʺ�
void get_image_iterator(char* pathHy,char* pathMs,char* pathout)
{
	GDALAllRegister();
	GDALDatasetH m_datasethy=GDALOpen(pathHy,GA_ReadOnly);
	GDALDatasetH m_datasetms=GDALOpen(pathMs,GA_ReadOnly);

	int xsizems=GDALGetRasterXSize(m_datasetms);
	int ysizems=GDALGetRasterYSize(m_datasetms);
	int bandsms=GDALGetRasterCount(m_datasetms);

	int xsizehy=GDALGetRasterXSize(m_datasethy);
	int ysizehy=GDALGetRasterYSize(m_datasethy);
	int bandshy=GDALGetRasterCount(m_datasethy);

	//Ӱ��ռ�����
	float *datahy=new float[xsizehy*ysizehy*bandshy];
	float *datams=new float[xsizems*ysizems*bandsms];
	float *datare=new float[xsizems*ysizems*bandshy];

	//��ȡ����
	for (int i=0;i<bandsms;i++)
		GDALRasterIO(GDALGetRasterBand(m_datasetms,i+1),GF_Read,0,0,xsizems,ysizems,datams+i*xsizems*ysizems,xsizems,ysizems,GDT_Float32,0,0);
	for (int i=0;i<bandshy;i++)
		GDALRasterIO(GDALGetRasterBand(m_datasethy,i+1),GF_Read,0,0,xsizehy,ysizehy,datahy+i*xsizehy*ysizems,xsizehy,ysizehy,GDT_Float32,0,0);
	
	GDALClose(m_datasethy);
	GDALClose(m_datasetms);

	//��ʼֵ���ݸ߹��������ز�����ȡ
	CSampleFunc samlefunc;
	for (int i=0;i<bandshy;i++)
	{
		printf("\r��ʼֵ�ز������Σ�%d/%d",bandshy,i+1);
		samlefunc.cubic_sample(xsizehy,ysizehy,datahy+i*xsizehy*ysizehy,xsizems,ysizems,datare+i*xsizems*ysizems);
	}
	printf("\n");



	delete[]datahy;
	delete[]datams;
	delete[]datare;

}


//���ݶ�Ԫ������ֵı�������ģ��
//xsize ysize:ģ��Ӱ��Ĵ�С
//oriPos	:��ʼ����Ԫ��λ�ã�x��y��
//endtype	:ÿ��λ�õĶ�Ԫ���
//propotion :����λ�õĶ�Ԫ��ȱ���BSQ�洢
//oriPosNum	:��ʼ����Ԫ����
//endnumbers:��Ԫ����
void ratio_simulated(int xsize,int ysize,int* oriPos,int* endtype,float* propotion,int oriPosNum,int endnumbers)
{
	//�ж�����
	if((oriPos==NULL&&endtype!=NULL)||(oriPos!=NULL&&endtype==NULL))
	{
		printf("���������ƥ�䣡\n");
		return;
	}
	float maxdis=sqrt(float(xsize*xsize)+float(ysize*ysize))*2.0f/3.0f;
	float *ratioSet=new float[oriPosNum];
	float *ratiorand=new float[endnumbers];
	float *ratioinit=new float[endnumbers];
	memset(ratioSet,0,sizeof(float)*oriPosNum);
	memset(propotion,0,sizeof(float)*xsize*ysize*endnumbers);
	//û�г�ʼ��λ��
	if(oriPos==NULL&&endtype==NULL)
	{
		printf("û�г�ʼ��λ�ã���ʱ������...\n");
		return ;
	}
	else //��ʼ����λ��
	{
		//��ʼ��λ�ö�Ԫ��������
		for (int i=0;i<oriPosNum;i++)
			propotion[oriPos[2*i+1]*xsize+oriPos[2*i+0]/*λ��*/+endtype[i]*xsize*ysize/*��Ԫ���*/]=1.0f;
		
		//���ݾ�������Ӱ������ ����ԽԶӰ������ԽС ����Ϊ����Ӱ��Խ��ߵ�һ�����ʾ��Ӱ��
		for (int j=0;j<xsize;j++)
		{
			for (int k=0;k<ysize;k++)
			{
				printf("\r��ȡ����(%d,%d)�ı���",j,k);
				float distemp=0;
				memset(ratioSet,0,sizeof(float)*oriPosNum);

				//��ȡ������ʼ��Ԫ��Ŀ�����ص�Ӱ�����
				for(int i=0;i<oriPosNum;i++)
				{
					distemp=sqrt(float(oriPos[2*i+0]-j)*float(oriPos[2*i+0]-j)+float(oriPos[2*i+1]-k)*float(oriPos[2*i+1]-k));
					if(distemp==0)	//Ϊ��Ԫλ��
						break;
					else if(distemp>maxdis)	//������� ����Ϊ��Ӱ��
						ratioSet[i]=0;
					else
						ratioSet[i]=1/(distemp+1);
				}
				//Ϊ��Ԫλ�� ����Ҫ�����κβ���
				if(distemp==0)
					continue;
				else	
				{
					memset(ratioinit,0,sizeof(float)*endnumbers);
					//�����ʼ��������Ԫ����
					srand((unsigned)time(0));
					rand();
					for (int i=0;i<endnumbers;i++)
						ratiorand[i]=(float)rand() / RAND_MAX; 
					//���������Ԫ��ʼ����
					for (int i=0;i<oriPosNum;i++)
						ratioinit[endtype[i]]+=ratioSet[i];
					float total=0;
					for (int i=0;i<endnumbers;i++)
					{
						ratioinit[i]=ratioinit[i]*ratiorand[i];
						total+=ratioinit[i];
					}
					//��һ��
					for (int i=0;i<endnumbers;i++)
						propotion[i*xsize*ysize+k*xsize+j]=ratioinit[i]/total;
				}

			}//Ӱ��ÿ�����ش����ѭ��
		}
	}
	printf("\n");
	delete[]ratioSet;
	delete[]ratiorand;
	delete[]ratioinit;
}

//ͨ����Ԫ�ͳ�ʼλ��ģ��Ӱ��
void simulate_image_data(char* pathEnd,int bands,int endnumbers,int xsize,int ysize,int* oriPos,int* endtype,int oriPosNum,char* pathsimulateImage)
{
	//��Ԫ��������
	float* dataendmemeber=new float[bands*endnumbers];
	dataendmemeber=get_endmenber_spectralf(pathEnd,bands,endnumbers);
	
	float* propotion=new float[endnumbers*xsize*ysize];
	float* imagedata=new float[xsize*ysize];
	ratio_simulated(xsize,ysize,oriPos,endtype,propotion,6,endnumbers);

	//����Ӱ������
	GDALAllRegister();
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" ); 
	GDALDatasetH m_datasetout=GDALCreate(GDALGetDriverByName("GTiff"),pathsimulateImage,xsize,ysize,bands,GDT_Float32,papszOptions);
	for (int i=0;i<bands;i++)
	{
		memset(imagedata,0,sizeof(float)*xsize*ysize);
		printf("\r���ɲ���%d/%d",bands,i+1);
		for (int j=0;j<xsize;j++)
			for (int k=0;k<ysize;k++)
				for (int l=0;l<endnumbers;l++)
					imagedata[k*xsize+j]+=dataendmemeber[i*endnumbers+l]*propotion[l*xsize*ysize+k*xsize+j];
		GDALRasterIO(GDALGetRasterBand(m_datasetout,i+1),GF_Write,0,0,xsize,ysize,imagedata,xsize,ysize,GDT_Float32,0,0);
	}
	printf("\n");
	GDALClose(m_datasetout);
	delete[]dataendmemeber;
	delete[]propotion;
	delete[]imagedata;
}

//�������10�����ݽ����������
void simulateTest(char* pathEnd,int bands,int endnumbers,char* pathOri,char* pathRatio,char* pathoutS,char* pathRecov)
{
	double* dataendmemeber=new double[bands*endnumbers];
	dataendmemeber=get_endmenber_spectrald(pathEnd,bands,endnumbers);
	int number=100;
	double* dataout=new double[number*bands];
	memset(dataout,0,sizeof(double)*number*bands);

	ofstream ofs;	
	double* propotion=new double[endnumbers*number];
	srand(time(0));
	rand();
	for (int i=0;i<endnumbers*number;i++)
		propotion[i]=(double)rand() / RAND_MAX; 
	
	//��һ��
	ofs.open(pathRatio);
	for (int i=0;i<number;i++)
	{
		double total=0;
		for(int j=0;j<endnumbers;j++)
			total+=propotion[i*endnumbers+j];
		for(int j=0;j<endnumbers;j++)
		{
			propotion[i*endnumbers+j]/=total;
			ofs<<propotion[i*endnumbers+j]<<" ";
		}
		ofs<<endl;
	}
	ofs.close();

	//������
	ofs.open(pathOri);
	for (int i=0;i<number;i++)
	{
		for (int j=0;j<bands;j++)
		{
			for (int k=0;k<endnumbers;k++)
				dataout[j*number+i]+=dataendmemeber[j*endnumbers+k]*propotion[i*endnumbers+k];
		}
	}
	for (int i=0;i<number;i++)
	{
		for (int j=0;j<bands;j++)
			ofs<<dataout[j*bands+i]<<" ";
		ofs<<endl;
	}
	ofs.close();

	//�ֽ�
	double* dataT=new double[number*bands];
	MatrixTrans(dataout,bands,number,dataT);
	double* dataA=new double[(endnumbers-1)*number];
	double* dataS=new double[(endnumbers-1)*bands];
	int rank=0;
	Matrix_Rank(dataT,number,bands,rank);
	Matrix_AS(dataT,dataA,dataS,number,bands,endnumbers-1);

	//���
	ofs.open(pathoutS);
	for (int i=0;i<bands;i++)
	{
		for (int j=0;j<endnumbers-1;j++)
			ofs<<dataS[j*bands+i]<<" ";
		ofs<<endl;
	}
	ofs.close();

	MatrixMuti(dataA,number,endnumbers-1,bands,dataS,dataT);
	//���
	ofs.open(pathRecov);
	for (int i=0;i<number;i++)
	{
		for (int j=0;j<bands;j++)
			ofs<<dataT[i*bands+j]<<" ";
		ofs<<endl;
	}
	ofs.close();

	delete[]dataS;
	delete[]dataT;
	delete[]dataout;
	delete[]dataendmemeber;
	delete[]dataA;
	delete[]propotion;
}