#pragma once
#pragma comment(lib,"gdal_i.lib")
#include "..\gdal\include\gdal_priv.h"

//�ֿ��СΪ512*512
//���÷ֿ��СΪ2�ı�����ҪĿ������ͨ����λ����ӿ�ֿ��ٶ�
//Ӱ��ֿ鲢��ͨ������Ӱ����зֿ飬����ͨ���ֿ��ǩ��ȡÿһ����Ӱ���е�λ�ã��Լ�ÿһ�黺������λ��

//Ӱ��ֿ��ǩ
struct  BlockLabel 
{
	//ÿһ�����Ͻǵ�Ӱ�������
	int m_xIndex;
	int m_yIndex;
	
	//Ӱ���Ŀ�Ⱥ͸߶�
	int m_BlockWidth;
	int m_BlockHeight;

	//Ӱ���߽�λ��
	int m_edgeV[2];		//���±߽�
	int m_edgeH[2];		//���ұ߽�

	void operator =(BlockLabel block)
	{
		m_xIndex=block.m_xIndex;
		m_yIndex=block.m_yIndex;
		m_BlockHeight=block.m_BlockHeight;
		m_BlockWidth=block.m_BlockWidth;
		m_edgeH[0]=block.m_edgeH[0];m_edgeH[1]=block.m_edgeH[1];
		m_edgeV[0]=block.m_edgeV[0];m_edgeV[1]=block.m_edgeV[1];
	}
};

//Ӱ��ֿ���Ŀ
int  GetImageXBlockNum(GDALDatasetH dataset);	//��ȡӰ��x�᷽��ֿ���Ŀ

int  GetImageYBlockNum(GDALDatasetH dataset);	//��ȡӰ��y�᷽��ֿ���Ŀ

/*
	��ȡӰ��ֿ���Ϣ
	������
	dataset				��Ӱ�����ݼ�
	blocks				���ֿ���ȡ��Ӱ���
	blockXNum blockYNum	��Ӱ������Ŀ
	buffer				���������Ĵ�С�����ܳ���Ӱ���Ĵ�С��
*/
long  GetImageBlock(GDALDatasetH dataset,BlockLabel* blocks,int blockXNum,int blockYNum,int buffer);

/*
	��д�ֿ�Ӱ������
	������
	dataset				��Ӱ�����ݼ�
	block				������ȡ���ݵ�Ӱ���
	imgBuffer			����ȡ��Ӱ������
	band				����ȡ���ݵĲ���
*/
//float������Ӱ��
long  ReadBlockImageData(GDALDatasetH dataset, BlockLabel block,float* imgBuffer,int band=1); 
long  WriteBlockImageData(GDALDatasetH dataset,BlockLabel block,float* imgBuffer,int band=1);

//int������Ӱ��
long  ReadBlockImageData(GDALDatasetH dataset, BlockLabel block,int* imgBuffer,int band=1);
long  WriteBlockImageData(GDALDatasetH dataset,BlockLabel block,int* imgBuffer,int band=1);

//BYTE������Ӱ��
long  ReadBlockImageData(GDALDatasetH dataset, BlockLabel block,unsigned char* imgBuffer,int band=1);
long  WriteBlockImageData(GDALDatasetH dataset,BlockLabel block,unsigned char* imgBuffer,int band=1);