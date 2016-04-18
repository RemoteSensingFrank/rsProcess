#include"HyperRepair.h"
#include"..\AuxiliaryFunction.h"
#include<map>

//��ȡ�ָ��ı�Ե������Ϣ
void get_segment_edge(const char* img_path, vector<Edge_Pixels> &edgeinter)
{
	//ʹ��GDAL��Ӱ�񲢻�ȡ����
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");			//����·��
	GDALDatasetH m_dataset = GDALOpen(img_path, GA_ReadOnly);
	
	int xsize = GDALGetRasterXSize(m_dataset);
	int ysize = GDALGetRasterYSize(m_dataset);
	int* imgData = NULL;

	//���ݿռ����������ݶ�ȡ
	try
	{
		imgData = new int[xsize*ysize];
		GDALRasterIO(GDALGetRasterBand(m_dataset, 1), GF_Read, 0, 0, xsize, ysize, imgData, xsize, ysize, GDT_Int32, 0, 0);
	}
	catch (bad_alloc &e)
	{
		printf("%s\n", e.what());
		exit(-1);
	}

	//��ȡ�ָ����
	vector<int> seg;
	for (size_t i = 0; i < xsize*ysize; ++i)
	{
		bool isIn = false;
		if (imgData[i] == 255 || imgData[i] == 0)
			continue;
		for (size_t j = 0; j < seg.size(); ++j)
		{
			if (imgData[i] == seg[j] || imgData[i] == 0 || imgData[i]  == 255)
				isIn = true;
		}
		if (!isIn)
		{
			Edge_Pixels m_pixels;
			m_pixels.m_segType = imgData[i];
			edgeinter.push_back(m_pixels);
			seg.push_back(imgData[i]);
		}
	}
	edgeinter.resize(seg.size());

	//��ȡÿһ���ָ��ı߽�����
	for (size_t i = 1; i < xsize-1; i++)
	{
		for (size_t j = 1; j < ysize-1; j++)
		{
			if (imgData[j*xsize+i] == 0 || imgData[j*xsize + i] == 255)
				continue;
			//��ȡ��
			int idx = 0;
			for (int k = 0; k < seg.size(); ++k)
			{
				if (imgData[j*xsize + i] == seg[k])
					idx = k;
			}
			
			//�ж��ǲ��Ǳ߽�8����
			if (imgData[j*xsize + i] != imgData[j*xsize + i - 1]	   ||
				imgData[j*xsize + i] != imgData[j*xsize + i + 1]	   ||
				imgData[(j - 1)*xsize + i] != imgData[j*xsize + i]	   ||
				imgData[(j + 1)*xsize + i] != imgData[j*xsize + i]	   ||
				imgData[(j + 1)*xsize + i] != imgData[j*xsize + i + 1] ||
				imgData[(j + 1)*xsize + i] != imgData[j*xsize + i - 1] ||
				imgData[(j - 1)*xsize + i] != imgData[j*xsize + i + 1] ||
				imgData[(j - 1)*xsize + i] != imgData[j*xsize - i])
			{
				CPOINT pnt; pnt.x = i; pnt.y = j;
				edgeinter[idx].m_edge_pixels.push_back(pnt);
			}
		}
	}
	if (imgData != NULL)
		delete[]imgData;
	imgData = NULL;
	GDALClose(m_dataset);
}

//��ȡ������Ϣ
void get_segment_intersection(const char* img_path, vector<Edge_Pixels> &intersecEdge, int begLine, int endLine)
{
	//ʹ��GDAL��Ӱ�񲢻�ȡ����
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");			//����·��
	GDALDatasetH m_dataset = GDALOpen(img_path, GA_ReadOnly);

	int xsize = GDALGetRasterXSize(m_dataset);
	int ysize = GDALGetRasterYSize(m_dataset);
	int* imgData = NULL;

	//���ݿռ����������ݶ�ȡ
	try
	{
		imgData = new int[xsize*ysize];
		GDALRasterIO(GDALGetRasterBand(m_dataset, 1), GF_Read, 0, 0, xsize, ysize, imgData, xsize, ysize, GDT_Int32, 0, 0);
	}
	catch (bad_alloc &e)
	{
		printf("%s\n", e.what());
		exit(-1);
	}

	//��ȡ�ָ����
	//��ȡ�ָ����
	vector<int> seg;
	for (size_t i = 0; i < xsize*ysize; ++i)
	{
		bool isIn = false;
		if (imgData[i] == 255 || imgData[i] == 0)
			continue;
		for (size_t j = 0; j < seg.size(); ++j)
		{
			if (imgData[i] == seg[j] || imgData[i] == 0 || imgData[i] == 255)
				isIn = true;
		}
		if (!isIn)
		{
			Edge_Pixels m_pixels;
			m_pixels.m_segType = imgData[i];
			intersecEdge.push_back(m_pixels);
			seg.push_back(imgData[i]);
		}
	}

	//��ȡ�߽�����
	for (size_t i = 0; i < ysize; i++)
	{
		if (imgData[i*xsize + begLine] == 255)
		{
			for (size_t j = 0; j < intersecEdge.size(); j++)
			{
				CPOINT pnt;

				if (i != ysize - 1)
				{
					pnt.x = begLine; pnt.y = (i + 1);
					if (intersecEdge[j].m_segType == imgData[(i + 1)*xsize + begLine])
						intersecEdge[j].m_edge_pixels.push_back(pnt);
				}
				if (i != 0)
				{
					pnt.x = begLine; pnt.y = (i - 1);
					if (intersecEdge[j].m_segType == imgData[(i - 1)*xsize + begLine])
						intersecEdge[j].m_edge_pixels.push_back(pnt);
				}
			}
		}
		if (imgData[i*xsize + endLine] == 255)
		{
			for (size_t j = 0; j < intersecEdge.size(); j++)
			{
				CPOINT pnt; 
				if (i != ysize - 1)
				{
					pnt.x = endLine; pnt.y = (i + 1);
					if (intersecEdge[j].m_segType == imgData[(i + 1)*xsize + endLine])
						intersecEdge[j].m_edge_pixels.push_back(pnt);
				}
				if (i != 0)
				{
					pnt.x = endLine; pnt.y = (i - 1);
					if (intersecEdge[j].m_segType == imgData[(i - 1)*xsize + endLine])
						intersecEdge[j].m_edge_pixels.push_back(pnt);
				}
			}
		}
	}
	
	if (imgData != NULL)
		delete[]imgData;
	imgData = NULL;
	GDALClose(m_dataset);
}

//��ȡ��ϵ㣬�����ȡ��ϵ�Ƚ��鷳����߽��ж��ٸ��������Ҫ��ȡ���ٸ���ϵ���
void get_fit_points(vector<Edge_Pixels> edgeinter, Edge_Pixels preIntersectEdge, vector<CPOINT> &fitpoints, int xsize, int ysize)
{
	//��ȡ�߽��������Сֵ
	//�����жϱ߽���Ƿ�Ϊ�����㣬��Ϊ�����㣬�򲻲���
	bool isbeg = false, isend = false;
	for (int i = 0; i < preIntersectEdge.m_edge_pixels.size(); ++i)
	{
		if (preIntersectEdge.m_edge_pixels[i].y == 1)
			isbeg = true;
		if (preIntersectEdge.m_edge_pixels[i].y == ysize - 2)
			isend = true;
	}
	if (isbeg&&isend)
	{
		printf("background \n");
		return;
	}

	//��ȡ��ϵ�����һ����
	int idx = 0;
	for (int i = 0;i< edgeinter.size();++i)
	{
		if (preIntersectEdge.m_segType == edgeinter[i].m_segType)
			idx = i;
	}

	int maxpos = edgeinter[idx].m_edge_pixels[0].x;
	int minpos = edgeinter[idx].m_edge_pixels[0].x;
	for (size_t i = 0; i < edgeinter[idx].m_edge_pixels.size(); i++)
	{
		maxpos = max(maxpos, edgeinter[idx].m_edge_pixels[0].x);
		minpos = min(minpos, edgeinter[idx].m_edge_pixels[0].x);
	}

	int interval = (maxpos - minpos) / 5;
	for (size_t i = 0; i < edgeinter[idx].m_edge_pixels.size(); i++)
	{
		//����ֻȥ������ �Ͳ���ѭ���� �鷳
		int pos1 = minpos + interval;
		int pos2 = minpos + 2 * interval;
		int pos3 = minpos + 3 * interval;

		if (edgeinter[idx].m_edge_pixels[i].x == pos1)
			fitpoints.push_back(edgeinter[idx].m_edge_pixels[i]);
		if (edgeinter[idx].m_edge_pixels[i].x == pos2)
			fitpoints.push_back(edgeinter[idx].m_edge_pixels[i]);
		if (edgeinter[idx].m_edge_pixels[i].x == pos3)
			fitpoints.push_back(edgeinter[idx].m_edge_pixels[i]);
	}

}

//�ֽ�õ��㣬����ֽ�����ף�ֱ��ͨ����С���Էֽ����
void split_points(vector<CPOINT> fitpoints, vector<CPOINT> &splitPoint1, vector<CPOINT> &splitPoint2)
{
	//���Ȱ���x����Ĵ�С˳������
	for (size_t i = 0; i < fitpoints.size(); i++)
	{
		for (size_t j = i; j < fitpoints.size(); j++)
		{
			//����
			if (fitpoints[i].x < fitpoints[j].x)
			{
				CPOINT pnt;
				pnt.x = fitpoints[i].x;
				pnt.y = fitpoints[i].y;

				fitpoints[i].x = fitpoints[j].x;
				fitpoints[i].y = fitpoints[j].y;
			}
		}
	}

	//��ȡ
	for (size_t i = 0; i < fitpoints.size(); i+=2)
	{
		if (fitpoints[2 * i + 0].y>fitpoints[2 * i + 1].y)
		{
			splitPoint1.push_back(fitpoints[2 * i + 0]);
			splitPoint2.push_back(fitpoints[2 * i + 1]);
		}
		else
		{
			splitPoint1.push_back(fitpoints[2 * i + 1]);
			splitPoint2.push_back(fitpoints[2 * i + 0]);
		}
	}

}

//ʵ�鴦��
void experiment_process()
{
	vector<Edge_Pixels>  edgeinter;
	vector<Edge_Pixels> intersecEdge;
	int begLine = 417;
	int endLine = 520;
	char* pathimg = "D:\\watershed.bmp";
	get_segment_edge(pathimg, edgeinter);
	get_segment_intersection(pathimg, intersecEdge, begLine, endLine);

	//����������
	for (int i = 0; i < intersecEdge.size(); ++i)
	{
		printf("class type : %d\n", intersecEdge[i].m_segType);
		for (size_t j = 0; j < intersecEdge[i].m_edge_pixels.size(); j++)
		{
			printf("position of edge : %d  %d\n", intersecEdge[i].m_edge_pixels[j].x, intersecEdge[i].m_edge_pixels[j].y);
		}
	}
}