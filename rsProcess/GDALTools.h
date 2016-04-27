#pragma once
/*
1.դ������ʸ��
2.ͨ��ROI�����ļ�����Ӱ��ü�
3.��ɢ�������񻯣���ɢ�㹹��������
4.SURF���ӽ���ͬ����ƥ��
5.ͨ�����������Ӱ�����ƴ��
*/

#include "gdal/include/gdal_priv.h"
#include "gdal/include/gdalwarper.h" 
#include "gdal/include/gdal_alg_priv.h"
#include "gdal/include/gdal_alg.h"
#include "gdal/include/ogrsf_frmts.h"

#include <iostream>
#include <io.h>
#include <iomanip>
#include <fstream>

#include<vector>
#include<string>

using namespace std;
#pragma comment(lib,"gdal_i.lib")
/*
դ������ʸ����
���ܣ�1.��������դ������ʸ������ʸ�����ķ���Ϊ��ͬһ�Ҷ�ֵ�����ڵ�����ϲ�Ϊһ��ʸ��
2.������Ӱ���ȡ��ʸ�����򣬲�����ʸ�������󲢼�����ȡʸ������ķ���Ϊ���з�0��Ԫת��Ϊһ��ʸ��
3.��Ӱ�����з�0����ת��Ϊʸ������
4.ʸ��������
*/
class RasterToPolygon
{
public:
	/*
	���ܣ���դ������ʸ����
	������1.const char * pszSrcFile ԴӰ���ļ�
	2.const char* pszDstFile  �õ���ʸ������
	3.const char* pszFormat   ʸ�����ݸ�ʽ�磺ESRI Shapefile
	*/
	long GDALTool_RasterToSHP(const char * pszSrcFile, const char* pszDstFile, const char* pszFormat);

	/*
	���ܣ���ƴ��Ӱ��߽�ת��Ϊͬһʸ���ļ�
	������1.vector<string> vStrSrcFiles Ӱ��·��
	2.char* pathDst �ϲ����ʸ��·��
	*/
	long GDALTool_TransImageToPolygon(vector<string> vStrSrcFiles, char* pathDst);

	/*
	���ܣ�������Ӱ���0ֵ������ת��Ϊʸ���ļ�
	������1.const char* pathSrc Ӱ��·��
	2.const char* pathDst ʸ�����·��
	*/
	long GDALTool_ImageNoneZeroToPolygon(const char* pathSrc, const char* pathDst);

	/*
	���ܣ��ϲ�ʸ�����ļ�
	������1.char** path ���ϲ���ʸ�����ļ�·��
	2.char* pathDst ������·��
	3.int number ��Ҫ�ϲ����ļ���Ŀ
	*/
	long GDALTool_ImageMergePolygon(char** path, char* pathDst, int number);
};
//=========================================================================================================================================

/*
Ӱ�����ROI�ü�
ע�⣺ROI�ü�����Ӱ����ʸ�����е��������ҵ���������ƥ��
*/
class ROICut {
public:
	/*
	���ܣ�����Ӱ��AOI����ü�
	������1.const char* pszInFile�����Ӱ���ļ�
	2.const char* pszOutFile ����ü���Ľ��
	3. const char* pszAOIFile�ü������ļ�
	4.const char* pszSQL SQL����ȡ�ü�����
	5.int *pBandInex ָ���ü��Ĳ��Σ�Ĭ��ΪNULL����ʾ�ü����в���
	6.int *piBandCount ָ���ü����εĸ�����ͬ��һ������ͬʱʹ��
	7.int iBufferָ��AOI�ļ�������Χ��Ĭ��Ϊ0����ʾ������
	8.const char* pszFormat��ͼ������ļ��ĸ�ʽ
	*/
	long GDALTool_ROICut(const char* pszInFile, const char* pszOutFile, const char* pszAOIFile, const char* pszSQL,
		int *pBandInex, int *piBandCount, int iBuffer, const char* pszFormat);

private:
	void
		GDALTool_LoadCutline(const char *pszCutlineDSName, const char *pszCLayer,
			const char *pszCWHERE, const char *pszCSQL,
			void **phCutlineRet);


	void
		GDALTool_TransformCutlineToSource(GDALDatasetH hSrcDS, void *hCutline,
			char ***ppapszWarpOptions, char **papszTO_In);


	//Ӱ�������ͶӰ����֮���ת��
	bool GDALTool_Projection2ImageRowCol(double *adfGeoTransform, double dProjX, double dProjY, int &iCol, int &iRow);

	bool GDALTool_ImageRowCol2Projection(double *adfGeoTransform, int iCol, int iRow, double &dProjX, double &dProjY);
};
//=========================================================================================================================================

/*
��ɢ������
���ܣ�1.��ɢ�㹹�ɹ����������Ӱ����ʽ��ʾ
2.��ɢ�㹹�������������ļ���ʽ�洢
*/
class GridTrans {
public:
	/*
	���ܣ���ɢ���ݵ㹹�ɹ������
	������1.GDALGridAlgorithm gridAlg ��ֵ�㷨
	typedef enum {
	GGA_InverseDistanceToAPower = 1,
	GGA_MovingAverage = 2,
	GGA_NearestNeighbor = 3,
	GGA_MetricMinimum = 4,
	GGA_MetricMaximum = 5,
	GGA_MetricRange = 6,
	GGA_MetricCount = 7,
	GGA_MetricAverageDistance = 8,
	GGA_MetricAverageDistancePts = 9,
	GGA_Linear = 10,
	GGA_InverseDistanceToAPowerNearestNeighbor = 11
	} GDALGridAlgorithm;
	2.const void *poOptions ��ֵ������
	// �����ֵ�㷨���������ò�ֵ����ֵ
	GDALGridInverseDistanceToAPowerOptions *poOptions = new GDALGridInverseDistanceToAPowerOptions();
	poOptions->dfPower = 2;				���ݷ���ϵ��
	poOptions->dfRadius1 = 20;			��������뾶
	poOptions->dfRadius2 = 15;			��С�����뾶

	GDALGridMovingAverageOptions *poOptions = new GDALGridMovingAverageOptions();
	poOptions->dfRadius1 = 20;
	poOptions->dfRadius2 = 15;

	GDALGridNearestNeighborOptions  *poOptions = new GDALGridNearestNeighborOptions();
	poOptions->dfRadius1 = 20;
	poOptions->dfRadius2 = 15;

	3.double* points ��ɢ���ݵ㣬���ݵ㹹��Ϊ x��y��value
	4.int pointNumber ��ɢ���ݵ����
	5.double pixelResolution ��Ԫ�ķֱ���
	6.const char* pathDst ������Ӱ��
	*/
	long GDALTool_Grid(GDALGridAlgorithm gridAlg, const void *poOptions, double* points, int pointNumber, double pixelResolution, const char* pathDst);
	long GDALGridCreate(GDALGridAlgorithm eAlgorithm, const void *poOptions, GUInt32 nPoints,
		const double *padfX, const double *padfY, const double *padfZ,
		double dfXMin, double dfXMax, double dfYMin, double dfYMax,
		GUInt32 nXSize, GUInt32 nYSize, GDALDataType eType, void *pData);

	/*
	���ܣ�����Ӱ��ľ�γ��ת��Ϊʸ��������
	������1.const char* pathLatLong ��γ���ļ�����γ�ȵ��ļ���ʽΪ
	pointID		Longitude	  Latitude
	dataItem:
	2.const char* pathShp ���ɵ�ʸ�����ļ�
	*/
	long GDALLatLongToSHP(const char* pathLatLong, const char* pathShp);

	/*
	���ܣ�����Delaunay������
	������1.double* points ��ɢ��
	2.int pointNumber ��ɢ��ĸ���
	3.const char* pathDst ����ļ�·��
	*/
	long GDALTool_TriangleDelaunay(double* points, int pointNumber, const char* pathDst);

	/*
	���ܣ����ļ��л�ȡDelaunary������
	������1.const char* pathTri �ļ�·��
	2.double* pdfTriVex   �����ļ�
	3.int number			�������
	*/
	GDALTriangulation* GDALTool_LoadTriangle(const char* pathTri, double* pdfTriVex, int &number);
};
//=========================================================================================================================================

/*
ͨ�����������Ӱ�����ƴ��
���ܣ�1.��ȡȥ����׺���ļ���
2.��������ļ�
3.ת��AOI��Դ�ļ�֮������к�
4.������Ƕ��
5.Ӱ����Ƕ
*/
class GeoMosaic {
private:

	//ͼ��Χ
	double	       dfMinX, dfMinY, dfMaxX, dfMaxY;
	//ͼ��ֱ���
	double	       dfXRes, dfYRes;
	//ͼ����
	int             nForcePixels, nForceLines;
	//ͼ���Ƿ����Alphaͨ��
	int             bEnableDstAlpha, bEnableSrcAlpha;

public:
	GeoMosaic() :dfMinX(0.0), dfMinY(0.0), dfMaxX(0.0), dfMaxY(0.0), dfXRes(0.0), dfYRes(0.0),
		nForceLines(0), nForcePixels(0), bEnableDstAlpha(FALSE), bEnableSrcAlpha(FALSE)
	{

	}

	/**
	ת��ͶӰ���굽ͼ�����к�����
	����ͼ������У�����AOI������ʱת��AOI���굽ͼ�����к�����
	**/
	class CutlineTransformer : public OGRCoordinateTransformation
	{
	public:

		/**
		* @brief Դͼ��ת������
		*/
		void *hSrcImageTransformer;

		/**
		* @brief ��ȡԴ���ݿռ�ο�����ϵ
		* @return ����Դ���ݿռ�ο�����ϵ
		*/
		virtual OGRSpatialReference *GetSourceCS() { return NULL; }

		/**
		* @brief ��ȡĿ�����ݿռ�ο�����ϵ
		* @return ����Ŀ�����ݿռ�ο�����ϵ
		*/
		virtual OGRSpatialReference *GetTargetCS() { return NULL; }

		/**
		* @brief ����ת��ͶӰ���굽���к�
		* @param nCount		��������
		* @param x			x���괮
		* @param y			y���괮
		* @param z			z���괮
		* @return �����Ƿ�ִ����ȷ
		*/
		virtual int Transform(int nCount, double *x, double *y, double *z = NULL)
		{
			int nResult;

			int *pabSuccess = (int *)CPLCalloc(sizeof(int), nCount);
			nResult = TransformEx(nCount, x, y, z, pabSuccess);
			CPLFree(pabSuccess);

			return nResult;
		}

		/**
		* @brief ����ת��ͶӰ���굽���к�
		* @param nCount		��������
		* @param x			x���괮
		* @param y			y���괮
		* @param z			z���괮
		* @param pabSuccess	����ִ�гɹ�������
		* @return �����Ƿ�ִ����ȷ
		*/
		virtual int TransformEx(int nCount, double *x, double *y,
			double *z = NULL, int *pabSuccess = NULL)
		{
			return GDALGenImgProjTransform(hSrcImageTransformer, TRUE,
				nCount, x, y, z, pabSuccess);
		}
	};


	/*
	���ܣ���ȡ�ļ�����ȥ��׺
	������1.const char* pszFile������Ӱ������
	*/
	inline string GDALTool_GetFileName(const char* pszFile);


	/*
	���ܣ���������ļ�
	������1.char **papszSrcFiles�������ļ��б�
	2.const char *pszFilename  ������ļ�·��
	3.const char *pszFormat    : ����ļ���ʽ
	4.char **papszTO		    : ת��ѡ��
	5.char ***ppapszCreateOptions: �����ļ�ѡ��
	6.GDALDataType eDT			�������ļ���������
	*/
	GDALDatasetH GDALTool_GDALWarpCreateOutput(char **papszSrcFiles, const char *pszFilename,
		const char *pszFormat, char **papszTO,
		char ***ppapszCreateOptions, GDALDataType eDT);


	/*
	���ܣ�ת��AOI��Դ�ļ�֮������к�
	������1.GDALDatasetH hSrcDS �������ļ�GDAL���ݼ����
	2.void *hCutline      �����еļ�����״
	3.char ***ppapszWarpOptionst: ת��ѡ��������ò��в���
	4.char **papszTO		     : ת��ѡ��
	5.char **papszTO_In		 : ת��ѡ��
	*/
	void GDALTool_TransformCutlineToSource(GDALDatasetH hSrcDS, void *hCutline,
		char ***ppapszWarpOptions, char **papszTO_In);


	/*
	���ܣ�������Ƕ��
	������1.const char *pszCutlineDSName ����Ƕ���ļ�
	2.void *hCutline       �����еļ�����״
	3.const char *pszCLayer: ��Ƕ��ͼ�����ƣ�����ΪNULL
	4.const char *pszCWHERE: ��Ƕ������ֶ�
	5.const char *pszCSQL	: ��Ƕ��SQL�����ֶ�
	6.void **phCutlineRet : ���ص���Ƕ������ָ��
	*/
	long GDALTool_LoadCutline(const char *pszCutlineDSName, const char *pszCLayer,
		const char *pszCWHERE, const char *pszCSQL,
		void **phCutlineRet);

	/*
	���ܣ�Ӱ����Ƕ
	* ע��������ô˺���֮ǰ�����ȶ�Ӱ����м��ξ�����ͬһͶӰ����ϵͳ�£��ֱ��ʿ��Բ�ͬ��
	* ����ͶӰ��Ϣ�Լ�����ĸ������ݵĲ��θ�������һ�£��������ֲ������������Ƕ������
	* ͼ��ķֱ��ʻᰴ������Ӱ��ĵ�һ��Ӱ���ȡ������ͶӰ����Ϣ					*
	* ����������vector<string> vStrSrcFiles �������ļ��Լ���Ƕ��·�����飬			*
	Ĭ�ϵ�һ��Ϊ�ο�Ӱ�񣬵�һ��ͼ�������²㣬�������������				*
	*          const char* pszCutLineFile  ��������Ƕ���ļ��ļ�·������				*
	*		   const char* pszOutFile: ����ļ�·��									*
	*		   GDALResampleAlg eResampleMethod: �ز�����ʽ							*
	*		   const char *pszCSQL	: ��Ƕ��SQL�����ֶ�								*
	*		   const char *pszFormat:����ļ���ʽ����ϸ�ο�GDAL֧����������			*
	*/
	long GDALTool_ImageMosaicing(vector<string> vStrSrcFiles, const char* pszCutLineFile, const char* pszOutFile,
		GDALResampleAlg eResampleMethod, const char *pszFormat);

	//��װΪӰ����ֹ��ŵĸ�ʽ
	long GDALTool_ImageMosaicing(char* path, int numBeg, int numEnd, const char* pszCutLineFile, const char* pszOutFile,
		GDALResampleAlg eResampleMethod, const char *pszFormat);
};
//=========================================================================================================================================

/*
����Ӱ����ļ�����shp��ʽ�������ļ�
*/
struct GdPos
{
	double x;
	double y;
};
class CAreaFileGet
{
public:
	/*
	���ܣ����õ�����
	��������ɷ������ĵ������
	*/
	long setPolygon(vector<GdPos> polygon);

	/*
	���ܣ����ļ�ת��Ϊ���ļ�
	������1.const char* projRef ͶӰ��Ϣ
	2.const char* pathOut ���shp�ļ�·��
	*/
	long transTOpolygon(const char* projRef, const char* pathOut);

private:
	vector<vector<GdPos>> m_polygons;
};
//=========================================================================================================================================


/*
ʹ�ÿ��Ƶ���м���У��
���ܣ�1.ʹ�ÿ��Ƶ���ж���ʽ����У��
2.ʹ�ÿ��Ƶ����TPS����У��
3.ʹ�ÿ��Ƶ����Triangle�ָ��У��
4.ʹ��RPC��������У��
5.����GeoLoc����У��
*/
class GeoGCPProcess
{
public:
	/*
	���ܣ�ʹ�ÿ��Ƶ���м���У��
	������1.const char *pszSrcFilename ��У����Ӱ��
	2.const char *pszDstFilename У�����Ӱ��
	3.int nReqOrder ����ʽ����
	4.int bReversed ���ò���
	5.const char *pszReferenceWKT ͶӰϵ
	6.double *dGroundSize Ӱ��ֱ��ʴ�С
	7.double* GCPs,int gcpNum ���Ƶ�ԺͿ��Ƶ����Ŀ
	*/
	long GeoProc_GCPWarpOrder(const char *pszSrcFilename, const char *pszDstFilename, int nReqOrder, int bReversed, const char *pszReferenceWKT, double *dGroundSize, double* GCPs, int gcpNum);

	/*
	���ܣ�ʹ��TPS�������м���У��
	������1.const char *pszSrcFilename ��У����Ӱ��
	2.const char *pszDstFilename У�����Ӱ��
	3.const char* pszDstWkt ͶӰ����ϵ
	4.double* dGroundSize Ӱ��ֱ���
	5.GDALResampleAlg eResampleMethod �ز�������
	6.,double* GCPs,int gcpNum ���Ƶ�ԺͿ��Ƶ����Ŀ
	*/
	long GeoProc_GCPWarpTPS(const char* pszSrcFilename, const char* pszDstFilename, const char* pszDstWkt, double* dGroundSize, GDALResampleAlg eResampleMethod, double* GCPs, int gcpNum);

	/*
	���ܣ�ʹ��RPC������Ӱ����м���У��
	������1.const char * pszSrcFile �����ļ�·��
	2.const char * pszDstFile ����ļ�·��
	3.GDALRPCInfo *psRPC  RPC��Ϣ
	4.double dfPixErrThreshold RPC���귴�������ֵ
	5.char **papszOptions RPCУ���߳����ò���
	6.GDALResampleAlg eResampleMethod �ز�������
	7.const char * pszFormat ����ļ���ʽ����
	*/
	long GeoProc_GCPWarpRPC(const char * pszSrcFile, const char * pszDstFile, GDALRPCInfo *psRPC, double dfPixErrThreshold, char **papszOptions,
		GDALResampleAlg eResampleMethod, const char * pszFormat);

	/*
	���ܣ�ʹ������������Ӱ���������м���У�������Ƚ����Ƶ��������������������������ݽ���У��
	������1.const char* pathSrcImg ԴӰ��·��
	2.const char* pathDstImg ���Ӱ��·��
	3.const char* pathTri	����������ļ�
	4.double* GCPs,int gcpNum ���Ƶ�Ϳ��Ƶ���Ŀ
	5.UTMZone   UTMͶӰ����
	6.GroundSize ����ֱ���
	*/
	long GeoProc_GCPTriangle(char* pathSrcImg, char* pathDstImg, char* pathTri, double* GCPs, int gcpNum, int UTMZone, double GroundSize);

	/*
	���ܣ����HDFӰ��ʹ�õ�GeoLocӰ��У������
	������1.const char * pszSrcFile ����Ӱ��·��
	2.const char * pszDstFile ���Ӱ��·��
	3. char **papszGeoLocOptions  GeoLoc��Ϣ
	4.GDALResampleAlg eResampleMethod �ز�������
	5. const char * pszFormat ���ݸ�ʽ����
	*/
	long GeoProc_GCPWarpGeoLoc(const char * pszSrcFile, const char * pszDstFile, char **papszGeoLocOptions, GDALResampleAlg eResampleMethod, const char * pszFormat);

	/*
	���ܣ���ȡGCP���ݲ���
	������1.const char* pathGCP �����Ƶ��ļ�
	2.double *gcps        ���õ��Ŀ��Ƶ�
	3.int &num			�����Ƶ���Ŀ
	4.int headline=0		��ͷ�ļ�����
	*/
	long GeoProc_GetENVIGcp(const char* pathGCP, double *gcps, int num, int headline = 0);
};

;