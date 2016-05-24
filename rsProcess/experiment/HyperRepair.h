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

//��ȡӰ���Ե������Ϣ
void get_segment_edge(const char* img_path, vector<Edge_Pixels> &edgeinter);

//��ȡӰ��ָ�������ȱʧ�齻����Ϣ
void get_segment_intersection(const char* img_path, vector<Edge_Pixels> &intersecEdge,int begLine,int endLine);

//��Ե���ز�����ϣ�������Ϸ�����ѡȡ�Ǹ����鷳��
void get_fit_points(vector<Edge_Pixels> edgeinter , Edge_Pixels preIntersectEdge,vector<CPOINT> &fitpoints,int xsize,int ysize);

//�����Ƶ���зֽ�õ�
void split_points(vector<CPOINT> fitpoints, vector<CPOINT> &splitPoint1, vector<CPOINT> &splitPoint2);

//�ҵ���Ϻ�ľ���������ڶ�Ӧ�ߵ�λ�ã�����������ϵĽ���
void get_fit_interesction(vector<vector<CPOINT>> &fitpoints, vector<CPOINT> &fitIntersect,vector<CPOINT> &edgeEntersect, int begLine, int endLine);

//���ڽ�����д���������õ��ϺõĽ��
void process_edge_fit_intersection(vector<CPOINT> &fitIntersect, vector<CPOINT> &edgeEntersect, vector<Edge_Pixels> &intersecEdge, int begLine, int endLine);

//ʵ�鴦����
void experiment_process();