#pragma once
#include"classify.h"

//��ʼ������
void InitKMeans(float* dataImg, int xsize, int ysize, int bands, int classNum, category* m_categories);

//���ݷ����������ݼ���������
void CenterKMeans(float* dataImg, int xsize, int ysize, int bands, int classNum, category* m_categories);

//����k��ֵ�ĵ���
void IteratorKMeans(float* dataImg, int xsize, int ysize, int bands, int classNum, category* m_categories);

//K��ֵ����
void KMeansClassify(const char* pathImgIn, const char* pathImgOut, int classNum);

//ISOData����
void ISODataClassify(const char* pathImgIn, const char* pathImgOut, int classNum,float maxDev,float minDis);