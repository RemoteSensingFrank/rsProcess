#pragma once
#include"..\Global.h"
#include"..\gdal\include\gdal_priv.h"
#include<vector>
using namespace std;

#pragma comment(lib,"gdal_i.lib");


//��ȡ�ָ�Ӱ������飬���õ������������ȱʧ��
//ÿ���ָ�ı�Ե����
struct Edge_Pixels
{
	int m_segType;
	vector<CPOINT> m_edge_pixels;
};
//ÿ���ָ�������ȱʧ�㽻������
struct Edge_Intersection
{
	int m_segType;
	vector<CPOINT> m_edge_intersction;
};

//��ȡӰ���Ե������Ϣ
void get_segment_edge(const char* img_path, vector<Edge_Pixels> &edgeinter);

//��ȡӰ��ָ�������ȱʧ�齻����Ϣ
void get_segment_intersection(const char* img_path, vector<Edge_Pixels> &edgeinter,int begLine,int endLine);