#pragma once

//ÿһ�������Ϣ
struct category {
	float* center;
	int*   objindex;
	int    demension;
	int    objnumbers;
	float*  objdervation;
};

//����һ�����
category createcategory(int demension);

//��������ڶ�����Ŀ
void setcategoryobjs(int objnumbers, category &m_categ);

//�����ڶ���ľ�ֵ�ͷ���
//���������������ݶ����룬�������������Ĵ�С
void getstatisticcategory(float* data, category &m_categ,int demensionIdx);

//��������
void splitcategory(category m_categ, category &m_categ1, category &m_categ2);

//��ϲ�����
void combinecategory(category m_categ1, category m_categ2, category &m_categ);

//�ͷ���ռ�
void freecategory(category &m_categ);
