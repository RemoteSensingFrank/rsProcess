#include "spFliter.h"

//============================================================================
// Name        : spFliter.cpp
// Author      : ww
// Version     : 1.0  
// Copyright   : ww
// Description : Ӱ��ռ����˲�
//============================================================================
//  [4/9/2014 wuwei just you]


/*
	Version     : 2.0  	
	1.����Ӱ��ֿ��˲�����
	2.Ӱ��ֿ����̴߳����˲�
	3.���Ӳ��Ժ����������д���
	[2/7/2015 wuwei just you]
*/



/*
	Version     : 2.1  	
	1.����Ӱ����̬ѧ����
		1.1����Ӱ����̬ѧ�˲���
		1.2Ӱ����̬ѧ��ʴ����
		1.3Ӱ����̬ѧ���Ͳ���
		1.4Ӱ�񿪡�������
    [4/11/2015 wuwei just you]
*/

/********************************************************************************
*����������spFliterCore����Ӱ���˲��˴�С���˲�������						    *	
*����������float  **core����άָ�룬���õ��˲���								*
*		   int   xsize		  ���˲��˵�x��С									*
*		   int	 ysize	      ���˲��˵�y��С									*
********************************************************************************/
void spFliter::spFliterCore(float** core,int xsize,int ysize)
{
	if (core==NULL)
		return;

	m_core_xsize=xsize;
	m_core_ysize=ysize;

	m_fliter_core=new float *[ysize];
	for (int i=0;i<ysize;i++)
	{
		m_fliter_core[i]=new float [xsize];
		memcpy(m_fliter_core[i],core[i],sizeof(float)*xsize);
	}
	m_is_core=true;
}

//һάָ������
void spFliter::spFliterCore(float* core,int xsize,int ysize)
{
	if (core==NULL)
		return;

	m_core_xsize=xsize;
	m_core_ysize=ysize;

	m_fliter_core=new float *[ysize];
	for (int i=0;i<ysize;i++)
	{
		m_fliter_core[i]=new float [xsize];
		memcpy(m_fliter_core[i],core+i*xsize,sizeof(float)*xsize);
	}
	m_is_core=true;
}


/********************************************************************************
*����������spitalFliter		  : GDAL��װ��Ӱ������˲�						    *	
*����������char *pathSrc	  ������Ӱ��·��									*
*		   char *pathDst	  : ���Ӱ��·��									*
*		   int   xpos		  ��ģ�崦��λ��x									*
*		   int	 ypos	      ��ģ�崦��λ��y									*
*		   int	 xoffset	  ��Ӱ��x�����ƫ��									*
*		   int	 yoffset	  ��Ӱ��y�����ƫ��									*
********************************************************************************/
void spFliter::spitalFliter(char* pathSrc,char* pathDst,int xpos,int ypos,int xoffset,int yoffset)
{
	//GDAL��װ��Ӱ������˲�
	GDALAllRegister();
	GDALDatasetH m_src_dataset, m_dst_dataset;

	//��ȡ����
	m_src_dataset=GDALOpen(pathSrc,GA_ReadOnly);
	int width=GDALGetRasterXSize(m_src_dataset);
	int heigh=GDALGetRasterYSize(m_src_dataset);
	int bands=GDALGetRasterCount(m_src_dataset);
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	m_dst_dataset=GDALCreate(GDALGetDriverByName("GTiff"),pathDst,width,heigh,bands,GDT_Float32,papszOptions);
	float *m_datasrc=new float[width*heigh];
	float *m_datadst=new float[width*heigh];

	for (int i=0;i<bands;i++)
	{
		GDALRasterBandH m_band;
		m_band=GDALGetRasterBand(m_src_dataset,i+1);
		GDALRasterIO(m_band,GF_Read,0,0,width,heigh,m_datasrc,width,heigh,GDT_Float32,0,0);
		memcpy(m_datadst,m_datasrc,sizeof(float)*width*heigh);
		spFliterOperation(m_datasrc,width,heigh,xpos,ypos,xoffset,yoffset,m_datadst);
		m_band=GDALGetRasterBand(m_dst_dataset,i+1);
		GDALRasterIO(m_band,GF_Write,0,0,width,heigh,m_datadst,width,heigh,GDT_Float32,0,0);
	}

	const char* proj=GDALGetProjectionRef(m_src_dataset);
	GDALSetProjection(m_dst_dataset,proj);
	double adfGeoTrans[6];
	GDALGetGeoTransform(m_src_dataset,adfGeoTrans);
	GDALSetGeoTransform(m_dst_dataset,adfGeoTrans);

	GDALClose(m_src_dataset);
	GDALClose(m_dst_dataset);
	delete[]m_datadst;
	delete[]m_datasrc;

}
void spFliter::spitalFliter(char* pathSrc,char* pathDst)
{
	//GDAL��װ��Ӱ������˲�
	GDALAllRegister();
	GDALDatasetH m_src_dataset, m_dst_dataset;

	//��ȡ����
	m_src_dataset=GDALOpen(pathSrc,GA_ReadOnly);
	int width=GDALGetRasterXSize(m_src_dataset);
	int heigh=GDALGetRasterYSize(m_src_dataset);
	int bands=GDALGetRasterCount(m_src_dataset);
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	m_dst_dataset=GDALCreate(GDALGetDriverByName("GTiff"),pathDst,width,heigh,bands,GDT_Float32,papszOptions);
	float *m_datasrc=new float[width*heigh];
	float *m_datadst=new float[width*heigh];

	for (int i=0;i<bands;i++)
	{
		GDALRasterBandH m_band;
		m_band=GDALGetRasterBand(m_src_dataset,i+1);
		GDALRasterIO(m_band,GF_Read,0,0,width,heigh,m_datasrc,width,heigh,GDT_Float32,0,0);
		memcpy(m_datadst,m_datasrc,sizeof(float)*width*heigh);
		spFliterOperation(m_datasrc,width,heigh,0,0,width,heigh,m_datadst);
		m_band=GDALGetRasterBand(m_dst_dataset,i+1);
		GDALRasterIO(m_band,GF_Write,0,0,width,heigh,m_datadst,width,heigh,GDT_Float32,0,0);
	}

	const char* proj=GDALGetProjectionRef(m_src_dataset);
	GDALSetProjection(m_dst_dataset,proj);
	double adfGeoTrans[6];
	GDALGetGeoTransform(m_src_dataset,adfGeoTrans);
	GDALSetGeoTransform(m_dst_dataset,adfGeoTrans);

	GDALClose(m_src_dataset);
	GDALClose(m_dst_dataset);
	delete[]m_datadst;
	delete[]m_datasrc;
}

//����ʹ���˲����˲�
//Laplace�����˲�
void spFliter::LaplaceFliter(char* pathSrc,char* pathDst,int xpos,int ypos,int xoffset,int yoffset)
{
	m_is_core=true;
	m_fliter_core=new float *[3];
	for (int i=0;i<3;i++)
		m_fliter_core[i]=new float [3];
	m_fliter_core[0][0]=-1.0f;m_fliter_core[0][1]=-1.0f;m_fliter_core[0][2]=-1.0f;
	m_fliter_core[1][0]=-1.0f;m_fliter_core[1][1]=8.0f; m_fliter_core[1][2]=-1.0f;
	m_fliter_core[2][0]=-1.0f;m_fliter_core[2][1]=-1.0f;m_fliter_core[2][2]=-1.0f;
	m_core_xsize=m_core_ysize=3;
	spitalFliter(pathSrc,pathDst,xpos,ypos,xoffset,yoffset);

}
void spFliter::LaplaceFliter(char* pathSrc,char* pathDst)
{
	m_is_core=true;
	m_fliter_core=new float *[3];
	for (int i=0;i<3;i++)
		m_fliter_core[i]=new float [3];
	m_fliter_core[0][0]=-1.0f;m_fliter_core[0][1]=-1.0f;m_fliter_core[0][2]=-1.0f;
	m_fliter_core[1][0]=-1.0f;m_fliter_core[1][1]=8.0f; m_fliter_core[1][2]=-1.0f;
	m_fliter_core[2][0]=-1.0f;m_fliter_core[2][1]=-1.0f;m_fliter_core[2][2]=-1.0f;
	m_core_xsize=m_core_ysize=3;
	spitalFliter(pathSrc,pathDst);
}

//sobel�����˲�
void spFliter::SobelFliter(char* pathSrc,char* pathDst,int xpos,int ypos,int xoffset,int yoffset)
{
	m_is_core=true;
	m_fliter_core=new float *[3];
	for (int i=0;i<3;i++)
		m_fliter_core[i]=new float [3];
	m_fliter_core[0][0]=1.0f;m_fliter_core[0][1]=0.0f;m_fliter_core[0][2]=-1.0f;
	m_fliter_core[1][0]=2.0f;m_fliter_core[1][1]=0.0f; m_fliter_core[1][2]=-2.0f;
	m_fliter_core[2][0]=1.0f;m_fliter_core[2][1]=0.0f;m_fliter_core[2][2]=-1.0f;
	m_core_xsize=m_core_ysize=3;
	spitalFliter(pathSrc,pathDst,xpos,ypos,xoffset,yoffset);
}
void spFliter::SobelFliter(char* pathSrc,char* pathDst)
{
	m_is_core=true;
	m_fliter_core=new float *[3];
	for (int i=0;i<3;i++)
		m_fliter_core[i]=new float [3];
	m_fliter_core[0][0]=1.0f;m_fliter_core[0][1]=0.0f;m_fliter_core[0][2]=-1.0f;
	m_fliter_core[1][0]=2.0f;m_fliter_core[1][1]=0.0f; m_fliter_core[1][2]=-2.0f;
	m_fliter_core[2][0]=1.0f;m_fliter_core[2][1]=0.0f;m_fliter_core[2][2]=-1.0f;
	m_core_xsize=m_core_ysize=3;
	spitalFliter(pathSrc,pathDst);
}

//��ֵ�˲�
void spFliter::MeanFliter(char* pathSrc,char* pathDst,int xpos,int ypos,int xoffset,int yoffset)
{
	m_is_core=true;
	m_fliter_core=new float *[3];
	for (int i=0;i<3;i++)
		m_fliter_core[i]=new float [3];
	m_fliter_core[0][0]=1.0f/9.0f;m_fliter_core[0][1]=1.0f/9.0f;m_fliter_core[0][2]=1.0f/9.0f;
	m_fliter_core[1][0]=1.0f/9.0f;m_fliter_core[1][1]=1.0f/9.0f;m_fliter_core[1][2]=1.0f/9.0f;
	m_fliter_core[2][0]=1.0f/9.0f;m_fliter_core[2][1]=1.0f/9.0f;m_fliter_core[2][2]=1.0f/9.0f;
	m_core_xsize=m_core_ysize=3;
	spitalFliter(pathSrc,pathDst,xpos,ypos,xoffset,yoffset);
}
void spFliter::MeanFliter(char* pathSrc,char* pathDst)
{
	m_is_core=true;
	m_fliter_core=new float *[3];
	for (int i=0;i<3;i++)
		m_fliter_core[i]=new float [3];
	m_fliter_core[0][0]=1.0f/9.0f;m_fliter_core[0][1]=1.0f/9.0f;m_fliter_core[0][2]=1.0f/9.0f;
	m_fliter_core[1][0]=1.0f/9.0f;m_fliter_core[1][1]=1.0f/9.0f;m_fliter_core[1][2]=1.0f/9.0f;
	m_fliter_core[2][0]=1.0f/9.0f;m_fliter_core[2][1]=1.0f/9.0f;m_fliter_core[2][2]=1.0f/9.0f;
	m_core_xsize=m_core_ysize=3;
	spitalFliter(pathSrc,pathDst);
}

//0ֵ�˲�
void spFliter::ZeroFliter(char* pathSrc,char* pathDst,int xpos,int ypos,int xoffset,int yoffset)
{
	m_is_core=true;
	m_fliter_core=new float *[3];
	for (int i=0;i<3;i++)
		m_fliter_core[i]=new float [3];
	m_fliter_core[0][0]=0.0f;m_fliter_core[0][1]=0.0f;m_fliter_core[0][2]=0.0f;
	m_fliter_core[1][0]=0.0f;m_fliter_core[1][1]=0.0f;m_fliter_core[1][2]=0.0f;
	m_fliter_core[2][0]=0.0f;m_fliter_core[2][1]=0.0f;m_fliter_core[2][2]=0.0f;
	m_core_xsize=m_core_ysize=3;
	spitalFliter(pathSrc,pathDst,xpos,ypos,xoffset,yoffset);
}
void spFliter::ZeroFliter(char* pathSrc,char* pathDst)
{
	m_is_core=true;
	m_fliter_core=new float *[3];
	for (int i=0;i<3;i++)
		m_fliter_core[i]=new float [3];
	m_fliter_core[0][0]=0.0f;m_fliter_core[0][1]=0.0f;m_fliter_core[0][2]=0.0f;
	m_fliter_core[1][0]=0.0f;m_fliter_core[1][1]=0.0f;m_fliter_core[1][2]=0.0f;
	m_fliter_core[2][0]=0.0f;m_fliter_core[2][1]=0.0f;m_fliter_core[2][2]=0.0f;
	m_core_xsize=m_core_ysize=3;
	spitalFliter(pathSrc,pathDst);
}

/********************************************************************************
*����������MedFliter		  ����ֵ�˲�����								    *	
*����������float  *imgBufferIn������Ӱ������ݿռ�								*
*		   int   width		  ��Ӱ��Ŀ��										*
*		   int	 heigh	      ��Ӱ��ĸ߶�										*
*		   int	 xpos	      ���˲�Ӱ�����ʼxλ��								*
*		   int	 ypos	      ���˲�Ӱ�����ʼyλ��								*
*		   int	 xoffset	  ��Ӱ��x�����ƫ��									*
*		   int	 yoffset	  ��Ӱ��y�����ƫ��									*
*		   float  *imgBufferOut�����Ӱ������ݿռ�								*
********************************************************************************/
float getMedValue(float* imgTemp,int width,int height)							//��ȡ�������ֵ
{
	int i,j;// ѭ������
	float bTemp;
	int iFilterLen=width*height;

	// ��ð�ݷ��������������
	for (j = 0; j < iFilterLen - 1; j ++)
	{
		for (i = 0; i < iFilterLen - j - 1; i ++)
		{
			if (imgTemp[i] > imgTemp[i + 1])
			{
				// ����
				bTemp = imgTemp[i];
				imgTemp[i] = imgTemp[i + 1];
				imgTemp[i + 1] = bTemp;
			}
		}
	}

	// ������ֵ
	if ((iFilterLen & 1) > 0)
	{
		// ������������Ԫ�أ������м�һ��Ԫ��
		bTemp = imgTemp[(iFilterLen + 1) / 2];
	}
	else
	{
		// ������ż����Ԫ�أ������м�����Ԫ��ƽ��ֵ
		bTemp = (imgTemp[iFilterLen / 2] + imgTemp[iFilterLen / 2 + 1]) / 2;
	}

	return bTemp;
}
void spFliter::MedFliter(float*imgBufferIn,int width,int heigh,int xpos,int ypos,int xoffset,int yoffset,int xfsize,int yfsize,float* imgBufferOut)
{
	if (imgBufferIn==NULL||imgBufferOut==NULL)
		return ;
	if (xpos<0||ypos<0||xpos>width||ypos>heigh||xpos+xoffset>width||ypos+yoffset>heigh)
		return ;

	//���ó�ʼֵ
	memcpy(imgBufferOut,imgBufferIn,sizeof(float)*width*heigh);

	//ѭ������
	float* imgTemp=new float[xfsize*yfsize];
	float  med;
	for (int i=xpos+xfsize/2;i<xpos+xoffset-(xfsize/2);i++)
	{
		for (int j=ypos+yfsize/2;j<ypos+yoffset-(yfsize/2);j++)
		{
			for (int k=0;k<yfsize;k++)
				memcpy(imgTemp+k*xfsize,imgBufferIn+i-xfsize/2+j*width,sizeof(float)*xfsize);
			med=getMedValue(imgTemp,xfsize,yfsize);
			imgBufferOut[i+j*width]=med;
		}
	}
	delete[]imgTemp;
}

/********************************************************************************
*����������spFliterOperation����Ӱ���˲��˴�С���˲�������					    *	
*����������float  *imgBufferIn������Ӱ������ݿռ�								*
*		   int   width		  ��Ӱ��Ŀ��										*
*		   int	 heigh	      ��Ӱ��ĸ߶�										*
*		   int	 xpos	      ���˲�Ӱ�����ʼxλ��								*
*		   int	 ypos	      ���˲�Ӱ�����ʼyλ��								*
*		   int	 xoffset	  ��Ӱ��x�����ƫ��									*
*		   int	 yoffset	  ��Ӱ��y�����ƫ��									*
*		   float  *imgBufferOut�����Ӱ������ݿռ�								*
********************************************************************************/
void spFliter::spFliterOperation(float*imgBufferIn,int width,int heigh,int xpos,int ypos,int xoffset,int yoffset,float* imgBufferOut)
{
	m_width=width;
	m_heigh=heigh;
	if (xpos<0||ypos<0||xpos>width||ypos>heigh||xpos+xoffset>width||ypos+yoffset>heigh)
		return;
	if (imgBufferIn==NULL||imgBufferOut==NULL)
		return ;
	for (int i=xpos;i<xpos+xoffset;i++)
		for (int j=ypos;j<ypos+yoffset;j++)
			imgBufferOut[j*width+i]=spFliterOperationCore(imgBufferIn,i,j);
}


/********************************************************************************
*����������spFliterOperation����Ӱ���˲��˴�С���˲�������					    *	
*����������float  *imgBufferIn������Ӱ������ݿռ�								*
*		   int   xpos		  ��ģ�崦��λ��x									*
*		   int	 ypos	      ��ģ�崦��λ��y									*
********************************************************************************/
float __fastcall spFliter::spFliterOperationCore(float*imgBufferIn,int xpos,int ypos)
{
	if (imgBufferIn==NULL)
		return -100000;
	if (xpos<0||ypos<0)
		return -100000;

	int xcoresize=m_core_xsize;
	int ycoresize=m_core_ysize;
	int tempx=0,tempy=0;
	float tempData=0.0f;
	for (int i=-int(xcoresize/2);i<=int(xcoresize/2);i++)
	{
		for (int j=-int(ycoresize/2);j<=int(ycoresize/2);j++)
		{
			tempx=xpos+i;
			tempy=ypos+j;
			if (tempx<0)
				tempx=xpos-i;
			if (tempx>=m_width)
				tempx=xpos-i;
			if (tempy<0)
				tempy=ypos-j;
			if (tempy>=m_heigh)
				tempy=ypos-j;
			tempData+=imgBufferIn[tempy*m_width+tempx]*m_fliter_core[j+int(ycoresize/2)][i+int(xcoresize/2)];
		}
	}
	return abs(tempData);
}

//////////////////////////////////////////////////////////////////////////
//  [4/23/2014 wuwei just you]
//	��Ӹ�˹�˲��Ͳ�ֺ���
/********************************************************************************
*����������SetGaussCore����Ӱ��gauss�˲��˴�С���˲�������					    *	
*����������float  **kernel����άָ�룬���õ��˲���								*
*		   int   xsize		  ���˲��˵�x��С									*
*		   int	 ysize	      ���˲��˵�y��С									*
********************************************************************************/
long SetGaussCore(float **kernel,float sigma, int xsize/* =3 */,int ysize/* =3 */)
{
	//�������
	if (kernel==NULL)
		return -1;

	int center_x = xsize/2,center_y = ysize/2;

	//��¼��ͬ�����Ӧ�Ĳ�ͬ��˹ֵ
	double gaussian;
	double distance;
	float temp;
	float h_sum = 0;
	for(int i=0;i<xsize;i++)
	{
		for(int j=0;j<ysize;j++)
		{
			distance = (center_x - i) * (center_x - i)+ (center_y - j) * (center_y - j);
			gaussian = exp( (0-distance)/(2* sigma * sigma) )/2*PI * sigma * sigma;
			temp = (float)gaussian;
			h_sum += temp;
			kernel[i][j] = temp;
		}
	}

	//��һ��

	for(int i=0;i<xsize;i++)
		for(int j=0; j<ysize; j++)
			kernel[i][j]/=h_sum;

	return 0;
}
long SetGaussCore(float *kernel, float sigma, int xsize/* =3 */,int ysize/* =3 */)
{
	if(kernel==NULL)
		return -1;

	float **kerneldata;
	kerneldata=new float*[ysize];
	for (int i=0;i<ysize;i++)
	{
		kerneldata[i]=new float [xsize];
	}
	SetGaussCore(kerneldata,sigma,xsize,ysize);
	for (int i=0;i<ysize;i++)
		memcpy(kernel+i*xsize,kerneldata[i],sizeof(float)*xsize);
	return 0;
}

/********************************************************************************
*����������GaussFliter����Ӱ��gauss�˲��˴�С���˲�������					    *	
*����������float  *srcImg	  ��ԴӰ��											*
*          float  *dstImg	  ����˹�˲�Ӱ�����								*
*		   int   xImg		  ��Ӱ���x��С	    								*
*		   int	 yImg	      ��Ӱ���y��С										*
*		   float ** kernel	  ��Ӱ���˲���										*
*		   int   xkernel	  ���˲��˵�x��С									*
*		   int	 ykernel	  ���˲��˵�y��С									*
********************************************************************************/
long GaussFliter(float *srcImg,float *dstImg,int xImg,int yImg,float ** kernel,int xkernel,int ykernel)
{
	//�������
	if(srcImg==NULL||dstImg==NULL||kernel==NULL)
		return -1;
	int number = xkernel*ykernel;			//����ģ�����飬ʹ��һά��
	float *temp_array = new float[number];
	float *temp_img = new float[number];	//����Ҫ�õ�����ص�
	int k = 0;

	for(int i=0;i<ykernel;i++)
		for(int j=0;j<xkernel;j++)
			temp_array[k++] = kernel[i][j];

	int height = yImg;
	int width  = xImg;
	for(int y=0; y<height; y++)
	{
		for(int x=0; x<width; x++)
		{
			int k=0;
			int col_use = ykernel/2;
			int row_use = xkernel/2;
			for(int i=y-row_use; i<=y+row_use; i++)			//���row*column�㴢������
			{
				for(int j=x-col_use; j<=x+col_use; j++)
				{
					/*���Խ�磬��������ĳ���ӽ�����ͼ��������ֵ*/
					int num_r = i;							
					int num_c = j;
					while(num_r<0)
						num_r++;
					while(num_r>=yImg)
						num_r--;
					while(num_c<0)
						num_c++;
					while(num_c>=xImg)
						num_c--;
					/******************************************/
					temp_img[k] = srcImg[num_r*xImg+num_c];
					k++;
				}
			}

			float sum=0;
			for(int i=0;i<number;i++)
				sum += (temp_img[i] * temp_array[i]);
			dstImg[y*xImg+x] = sum;
		}
	}
	delete []temp_array;
	delete []temp_img;

	return 0;
}

//GDAL��װ��˹�˲�
long  GaussFliter(char* pathSrc,char* pathDst,float** kernel, int xkernel, int ykernel)
{
	//�������
	if(pathSrc==NULL||pathDst==NULL||kernel==NULL)
		return -1;
	
	GDALAllRegister();
	GDALDatasetH m_datasetSrc, m_datasetDst;
	m_datasetSrc=GDALOpen(pathSrc,GA_ReadOnly);
	int xImg=GDALGetRasterXSize(m_datasetSrc);
	int yImg=GDALGetRasterYSize(m_datasetSrc);
	int band=GDALGetRasterCount(m_datasetSrc);

	float* datasrc=new float[xImg*yImg];
	float* datadst=new float[xImg*yImg];
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	m_datasetDst=GDALCreate(GDALGetDriverByName("GTiff"),pathDst,xImg,yImg,band,GDT_Float32,papszOptions);
	for (int i=0;i<band;i++)
	{
		GDALRasterBandH hband;
		hband=GDALGetRasterBand(m_datasetSrc,i+1);
		GDALRasterIO(hband,GF_Read,0,0,xImg,yImg,datasrc,xImg,yImg,GDT_Float32,0,0);
		GaussFliter(datasrc,datadst,xImg,yImg,kernel,xkernel,ykernel);
		hband=GDALGetRasterBand(m_datasetDst,i+1);
		GDALRasterIO(hband,GF_Write,0,0,xImg,yImg,datadst,xImg,yImg,GDT_Float32,0,0);
	}

	//ͶӰ
	const char* proj=GDALGetProjectionRef(m_datasetSrc);
	GDALSetProjection(m_datasetDst,proj);
	double adfGeoTrans[6];
	GDALGetGeoTransform(m_datasetSrc,adfGeoTrans);
	GDALSetGeoTransform(m_datasetDst,adfGeoTrans);
	//���ָ��
	GDALClose(m_datasetSrc);
	GDALClose(m_datasetDst);
	delete[]datadst;
	delete[]datasrc;

	return 0;
}

/********************************************************************************
*����������GaussDifference�Ը�˹�˲����Ӱ����в��						    *	
*����������float  *srcImg1	  ���˲���Ӱ��1										*
*		   float  *srcImg2	  ���˲���Ӱ��2										*
*		   float  *dstImg	  ��������Ӱ��									* 
*		   int   xsize		  ��Ӱ���x��С										*
*		   int	 ysize	      ��Ӱ���y��С										*
********************************************************************************/
long GaussDifference(float* srcImg1,float* srcImg2,float* dstImg,int xImg,int yImg)
{
	//�������
	if(srcImg1==NULL||srcImg2==NULL||dstImg==NULL)
		return -1;
	int pixelNum=xImg*yImg;
	for(int i=0;i<pixelNum;i++)
		dstImg[i]=srcImg1[i]-srcImg2[i];

	return 0;
	
}

//GDAL��װ��˹���
long GaussDifference(char* pathsrc1,char* pathsrc2,char* pathdst)
{
	//�����������
	if(pathsrc1==NULL||pathsrc2==NULL||pathdst==NULL)
		return -1;

	GDALAllRegister();
	GDALDatasetH m_dataset1,m_dataset2,m_datasetdst;

	m_dataset1=GDALOpen(pathsrc1,GA_ReadOnly);
	m_dataset2=GDALOpen(pathsrc2,GA_ReadOnly);
	int xsrc1=GDALGetRasterXSize(m_dataset1);
	int ysrc1=GDALGetRasterYSize(m_dataset1);
	int xsrc2=GDALGetRasterXSize(m_dataset2);
	int ysrc2=GDALGetRasterYSize(m_dataset2);
	int band1=GDALGetRasterCount(m_dataset1);
	int band2=GDALGetRasterCount(m_dataset2);
	if (xsrc1!=xsrc2||ysrc1!=ysrc2||band1!=band2)
		return -1;

	float* datasrc1=new float [xsrc1*ysrc1];
	float* datasrc2=new float [xsrc2*ysrc2];
	float* datadst =new float [xsrc1*ysrc1];
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	m_datasetdst=GDALCreate(GDALGetDriverByName("GTiff"),pathdst,xsrc1,ysrc1,band1,GDT_Float32,papszOptions);
	for (int i=0;i<band1;i++)
	{
		GDALRasterBandH hBand;
		hBand=GDALGetRasterBand(m_dataset1,i+1);
		GDALRasterIO(hBand,GF_Read,0,0,xsrc1,ysrc1,datasrc1,xsrc1,ysrc1,GDT_Float32,0,0);
		hBand=GDALGetRasterBand(m_dataset2,i+1);
		GDALRasterIO(hBand,GF_Read,0,0,xsrc2,ysrc2,datasrc2,xsrc2,ysrc2,GDT_Float32,0,0);
		hBand=GDALGetRasterBand(m_datasetdst,i+1);
		GaussDifference(datasrc1,datasrc2,datadst,xsrc2,ysrc2);
		GDALRasterIO(hBand,GF_Write,0,0,xsrc1,ysrc2,datadst,xsrc1,ysrc1,GDT_Float32,0,0);
	}

	//ͶӰ
	const char* proj=GDALGetProjectionRef(m_dataset1);
	GDALSetProjection(m_datasetdst,proj);
	double adfGeoTrans[6];
	GDALGetGeoTransform(m_dataset1,adfGeoTrans);
	GDALSetGeoTransform(m_datasetdst,adfGeoTrans);

	//�������
	GDALClose(m_dataset1);
	GDALClose(m_dataset2);
	GDALClose(m_datasetdst);

	delete[]datadst;
	delete[]datasrc1;
	delete[]datasrc2;

	return 0;

}

//////////////////////////////////////////////////////////////////////////////////////////////
//Ӱ������˲����߳����ݽṹ��
struct spFliterThreadStruct 
{
	GDALDatasetH dataset_src;	//�������ݼ�
	GDALDatasetH dataset_dst;	//������ݼ�
	float* fliter_core;			//�˲���
	int corexsize;				//�˲��˴�С
	int coreysize;
	BlockLabel block;			//Ӱ���
	int image_band;				//�������Ӱ�񲨶�
};

//Ӱ��ֿ�����˲�
long ImageSpaitalBlockFliter(char* pathsrc,char* pathdst,float* fliterCore,int xcoresize,int ycoresize)
{
	GDALAllRegister();
	GDALDatasetH m_dataset_src=GDALOpen(pathsrc,GA_ReadOnly);
	
	int xsize=GDALGetRasterXSize(m_dataset_src);
	int ysize=GDALGetRasterYSize(m_dataset_src);
	int bands=GDALGetRasterCount(m_dataset_src);
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	GDALDatasetH m_dataset_dst=GDALCreate(GDALGetDriverByName("GTiff"),pathdst,xsize,ysize,bands,GDT_Float32,papszOptions);
	//GDALDatasetH m_dataset_dst=GDALCreateCopy(GDALGetDriverByName("GTiff"),pathdst,m_dataset_src,)

	//��ȡӰ��ֿ���Ϣ
	int xblock=GetImageXBlockNum(m_dataset_src);
	int yblock=GetImageYBlockNum(m_dataset_src);
	BlockLabel *blocks=new BlockLabel[xblock*yblock];
	int buffer=max(xcoresize,ycoresize)+10;
	GetImageBlock(m_dataset_src,blocks,xblock,yblock,buffer);

	//�ֿ��˲���д���ļ���
	spFliter spatialfliter;spatialfliter.spFliterCore(fliterCore,xcoresize,ycoresize);
	for (int i=0;i<xblock*yblock;i++)
	{
		printf("\r����Ӱ���%d/%d",xblock*yblock,i+1);
		for (int j=0;j<bands;j++)
		{
			int readWidth =blocks[i].m_edgeH[1]-blocks[i].m_edgeH[0];
			int readHeight=blocks[i].m_edgeV[1]-blocks[i].m_edgeV[0];

			float* dataIn =new float[readWidth*readHeight];memset(dataIn,0,sizeof(float)*readHeight*readWidth);
			float* dataOut=new float[readWidth*readHeight];memset(dataOut,0,sizeof(float)*readHeight*readWidth);
			ReadBlockImageData(m_dataset_src,blocks[i],dataIn ,j+1);
			spatialfliter.spFliterOperation(dataIn,readWidth,readHeight,0,0,readWidth,readHeight,dataOut);
			WriteBlockImageData(m_dataset_dst,blocks[i],dataOut,j+1);

			delete[]dataIn;
			delete[]dataOut;
		}
	}
	printf("\n");

	GDALClose(m_dataset_dst);
	GDALClose(m_dataset_src);
	delete[]blocks;

	return 0;
}

//���߳̿����˲��̺߳���
DWORD WINAPI thread_spatial_Fliter_core(LPVOID dataStruct) 
{
	UserPoolData* poolData = (UserPoolData*)dataStruct;
	spFliterThreadStruct* sampleSt=(spFliterThreadStruct*)poolData->pData;
	spFliter spatialfliter;
	//����
	BlockLabel m_block=sampleSt->block;
	GDALDatasetH m_dataset_src=sampleSt->dataset_src;
	GDALDatasetH m_dataset_dst=sampleSt->dataset_dst;
	int m_corexsize=sampleSt->corexsize;
	int m_coreysize=sampleSt->coreysize;
	float* coredata=sampleSt->fliter_core;
	int band=sampleSt->image_band;

	int readWidth =m_block.m_edgeH[1]-m_block.m_edgeH[0];
	int readHeight=m_block.m_edgeV[1]-m_block.m_edgeV[0];

	float* dataIn =new float[readWidth*readHeight];memset(dataIn,0,sizeof(float)*readHeight*readWidth);
	float* dataOut=new float[readWidth*readHeight];memset(dataOut,0,sizeof(float)*readHeight*readWidth);
	spatialfliter.spFliterCore(coredata,m_corexsize,m_coreysize);
	ReadBlockImageData(m_dataset_src,m_block,dataIn ,band);
	spatialfliter.spFliterOperation(dataIn,readWidth,readHeight,0,0,readWidth,readHeight,dataOut);
	WriteBlockImageData(m_dataset_dst,m_block,dataOut,band);

	delete[]dataIn;
	delete[]dataOut;

	return 0;
}

//���߳̿����˲�
long ThreadImageSpaitalFliter(char* pathsrc,char* pathdst,float* fliterCore,int xcoresize,int ycoresize)
{
	GDALAllRegister();
	GDALDatasetH m_dataset_src;
	GDALDatasetH m_dataset_dst;
	spFliterThreadStruct* threadSt;

	m_dataset_src=GDALOpen(pathsrc,GA_ReadOnly);
	int xsize=GDALGetRasterXSize(m_dataset_src);
	int ysize=GDALGetRasterYSize(m_dataset_src);
	int bands=GDALGetRasterCount(m_dataset_src);
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	m_dataset_dst=GDALCreate(GDALGetDriverByName("GTiff"),pathdst,xsize,ysize,bands,GDT_Float32,papszOptions);

	//��ȡӰ��ֿ���Ϣ
	int xblock=GetImageXBlockNum(m_dataset_src);
	int yblock=GetImageYBlockNum(m_dataset_src);
	BlockLabel *blocks=new BlockLabel[xblock*yblock];
	GetImageBlock(m_dataset_src,blocks,xblock,yblock,max(xcoresize,ycoresize));
	threadSt=new spFliterThreadStruct[xblock*yblock*bands];
	//���߳�
	CThreadPool m_threadpool;
	for (int i=0;i<xblock*yblock;i++)
	{
		for (int j=0;j<bands;j++)
		{
			threadSt[j*xblock*yblock+i].block=blocks[i];
			threadSt[j*xblock*yblock+i].corexsize=xcoresize;
			threadSt[j*xblock*yblock+i].coreysize=ycoresize;
			threadSt[j*xblock*yblock+i].dataset_dst=m_dataset_dst;
			threadSt[j*xblock*yblock+i].dataset_src=m_dataset_src;
			threadSt[j*xblock*yblock+i].fliter_core=fliterCore;
			threadSt[j*xblock*yblock+i].image_band=j+1;
			m_threadpool.Run(thread_spatial_Fliter_core,&threadSt[j*xblock*yblock+i],Low);
		}
	}
	
	do 
	{
		Sleep(5);
	} while (m_threadpool.GetWorkingThreadCount()!=0);

	GDALClose(m_dataset_src);
	GDALClose(m_dataset_dst);
	delete[]blocks;
	delete[]threadSt;
	m_threadpool.Destroy();

	return 0;
}

/********************************************************************************
*����Ϣ������				 Ӱ����̬ѧ�˲�����								    *	
*							1.����Ӱ���˲��ˣ�Ŀ��λ��Ϊ1����λ��Ϊ0				*
*							2.Ӱ����̬ѧ��ʴ����									*
*							3.Ӱ����̬ѧ���Ͳ���									*
*							4.Ӱ�񿪱�����										*
********************************************************************************/
//�����˲���
void MorphologyFliter::MorphologyCore(float** core,int xsize,int ysize,int centerx,int centery)
{
	if (core==NULL)
		return;

	m_core_xsize=xsize;
	m_core_ysize=ysize;

	m_fliter_core=new float *[ysize];
	for (int i=0;i<ysize;i++)
	{
		m_fliter_core[i]=new float [xsize];
		memcpy(m_fliter_core[i],core[i],sizeof(float)*xsize);
	}
	m_is_core=true;
}

void MorphologyFliter::MorphologyCore(float* core,int xsize,int ysize,int centerx,int centery)
{
	if (core==NULL)
		return;

	m_core_xsize=xsize;
	m_core_ysize=ysize;

	m_fliter_core=new float *[ysize];
	for (int i=0;i<ysize;i++)
	{
		m_fliter_core[i]=new float [xsize];
		memcpy(m_fliter_core[i],core+i*xsize,sizeof(float)*xsize);
	}
	m_is_core=true;
}

//��̬ѧ��ʴ����
void MorphologyFliter::MorphologyCorrode(float* dataIn,int sizex,int sizey,int bands,float* dataOut)
{
	if(dataIn==NULL||dataOut==NULL)
		return ;
	if(!this->m_is_core)
	{
		printf("do not set fliter core!\n");
		return ;
	}
	//��ʴ�����˲��˴�������С������
	memcpy(dataOut,dataIn,sizeof(float)*sizex*sizey);
	for (int i=0;i<sizex;i++)
	{
		for (int j=0;j<sizey;j++)
		{
			if(i+this->m_core_xsize>=sizex||j+this->m_core_ysize>=sizey)
				continue;

			vector<float> m_tempData;
			for (int l=0;l<m_core_xsize;l++)
			{
				for (int m=0;m<m_core_ysize;m++)
				{
					if(this->m_fliter_core[m*this->m_core_xsize+l]!=0)
						m_tempData.push_back(dataIn[(j+m)*sizex+(l+i)]);
				}
			}
			float mindata=0;//min_element(m_tempData.begin(),m_tempData.end());
			dataOut[(j+this->m_centery)*sizex+(i+this->m_centerx)]=mindata;
			m_tempData.clear();
		}
	}
}
void MorphologyFliter::MorphologyDilation(float* dataIn,int sizex,int sizey,int bands,float* dataOut)
{
	if(dataIn==NULL||dataOut==NULL)
		return ;
	if(!this->m_is_core)
	{
		printf("do not set fliter core!\n");
		return ;
	}
	//��ʴ�����˲��˴�������С������
	memcpy(dataOut,dataIn,sizeof(float)*sizex*sizey);
	for (int i=0;i<sizex;i++)
	{
		for (int j=0;j<sizey;j++)
		{
			if(i+this->m_core_xsize>=sizex||j+this->m_core_ysize>=sizey)
				continue;

			vector<float> m_tempData;
			for (int l=0;l<m_core_xsize;l++)
			{
				for (int m=0;m<m_core_ysize;m++)
				{
					if(this->m_fliter_core[m*this->m_core_xsize+l]!=0)
						m_tempData.push_back(dataIn[(j+m)*sizex+(l+i)]);
				}
			}
			float maxdata=0;//max_element(m_tempData.begin(),m_tempData.end());
			dataOut[(j+this->m_centery)*sizex+(i+this->m_centerx)]=maxdata;
			m_tempData.clear();
		}
	}
}

//GDAL��װ����̬ѧ��ʴ������
//��̬ѧ��ʴ
void MorphologyFliter::MorphologyCorrode(char* pathIn,char* pathOut)
{
	if(!this->m_is_core)
	{
		printf("do not set fliter core!\n");
		return ;
	}

	GDALAllRegister();
	GDALDatasetH m_dataset=GDALOpen(pathIn,GA_ReadOnly);
	
	int xsize=GDALGetRasterXSize(m_dataset);
	int ysize=GDALGetRasterYSize(m_dataset);
	int bands=GDALGetRasterCount(m_dataset);
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	GDALDatasetH m_datasetDS=GDALCreate(GDALGetDriverByName("GTiff"),pathOut,xsize,ysize,bands,GDT_Float32,papszOptions);
	float *dataIn=NULL,*dataOut=NULL;
	dataIn =new float[xsize*ysize];
	dataOut=new float[xsize*ysize];
	for(int i=0;i<bands;i++)
	{
		GDALRasterIO(GDALGetRasterBand(m_dataset,i+1),GF_Read,0,0,xsize,ysize,dataIn,xsize,ysize,GDT_Float32,0,0);
		MorphologyCorrode(dataIn,xsize,ysize,1,dataOut);
		GDALRasterIO(GDALGetRasterBand(m_datasetDS,i+1),GF_Write,0,0,xsize,ysize,dataOut,xsize,ysize,GDT_Float32,0,0);
	}
	//ͶӰ
	const char* proj=GDALGetProjectionRef(m_dataset);
	GDALSetProjection(m_datasetDS,proj);
	double adfGeoTrans[6];
	GDALGetGeoTransform(m_dataset,adfGeoTrans);
	GDALSetGeoTransform(m_datasetDS,adfGeoTrans);

	GDALClose(m_datasetDS);
	GDALClose(m_dataset);
	delete[]dataIn;
	delete[]dataOut;
}

//��̬ѧ����
void MorphologyFliter::MorphologyDilation(char* pathIn,char* pathOut)
{
	if(!this->m_is_core)
	{
		printf("do not set fliter core!\n");
		return ;
	}

	GDALAllRegister();
	GDALDatasetH m_dataset=GDALOpen(pathIn,GA_ReadOnly);

	int xsize=GDALGetRasterXSize(m_dataset);
	int ysize=GDALGetRasterYSize(m_dataset);
	int bands=GDALGetRasterCount(m_dataset);
	char **papszOptions = NULL;  
	papszOptions = CSLSetNameValue( papszOptions, "INTERLEAVE", "BAND" );
	GDALDatasetH m_datasetDS=GDALCreate(GDALGetDriverByName("GTiff"),pathOut,xsize,ysize,bands,GDT_Float32,papszOptions);
	float *dataIn=NULL,*dataOut=NULL;
	dataIn =new float[xsize*ysize];
	dataOut=new float[xsize*ysize];
	for(int i=0;i<bands;i++)
	{
		GDALRasterIO(GDALGetRasterBand(m_dataset,i+1),GF_Read,0,0,xsize,ysize,dataIn,xsize,ysize,GDT_Float32,0,0);
		MorphologyDilation(dataIn,xsize,ysize,1,dataOut);
		GDALRasterIO(GDALGetRasterBand(m_datasetDS,i+1),GF_Write,0,0,xsize,ysize,dataOut,xsize,ysize,GDT_Float32,0,0);
	}

	//ͶӰ
	const char* proj=GDALGetProjectionRef(m_dataset);
	GDALSetProjection(m_datasetDS,proj);
	double adfGeoTrans[6];
	GDALGetGeoTransform(m_dataset,adfGeoTrans);
	GDALSetGeoTransform(m_datasetDS,adfGeoTrans);

	GDALClose(m_datasetDS);
	GDALClose(m_dataset);
	delete[]dataIn;
	delete[]dataOut;
}