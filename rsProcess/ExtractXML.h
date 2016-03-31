#pragma once

#include"rapidxml\rapidxml.hpp"
#include"rapidxml\rapidxml_iterators.hpp"
#include"rapidxml\rapidxml_print.hpp"
#include"rapidxml\rapidxml_utils.hpp"

#include<vector>
#include<string>
using namespace std;
using namespace rapidxml;

//�����ļ�����
/*
	�����ļ�������ͨ��������ͬ�Ķ����ļ�
	1.����Ԥ�������ļ�����
	2.Ԥ��������Ԫ���ݽ���
	3.0�����ݲ�Ʒ�����ļ�����
	4.0������Ԫ���ݽ���
	5.1�����ݲ�Ʒ�����ļ�����
	6.2�����ݲ�Ʒ�����ļ�����
*/
class ExtractQPDProduct 
{
	/*****************************************************************************
	* @brief : ȫ�׶����ݴ�������Ԥ�������ļ���������
	* @author : W.W.Frank
	* @date : 2016/3/30
	* @version : version 1.1
	* @inparam : pCatalogOF	�����ļ�
	//            pRAWData		ԭʼͼ������
	//			  pDarkData     ����������
	//			  pD0Data		0������
	//			  pD0XML		XMLԪ�ļ�
	//			  pD0JPG		0������ͼ
	//			  bSeg			�Ƿ�־�
	//			  nSLine		�־���ʼ��
	//			  nELine		�־���ֹ��
	//			  nALine		�Զ��־�����
	//			  bQuickView	�Ƿ����ͼ
	//			  bGrey			�Ƿ��ǻҶ�ͼ
	//			  fScale		��������
	//			  nUVBand		������Ӳ���
	//			  nVNIRBand		������Ӳ���
	//			  nSWIRBand		������Ӳ���
	//			  nTIHBand		������Ӳ���
	//			  pProFile		��������ļ�
	*****************************************************************************/
	long ExtractLoadPreProductXML(char *pCatalogOF, vector<string> &pRawData, vector<string> &pDarkData, vector<string> &pD0Data, 
							  vector<string> &pD0Dark, vector<string> &pD0XML, vector<string> &pD0JPG,bool &bSeg, int &nSLine, 
							  int &nELine, int &nALine, bool &bQuickView, bool &bGrey, float &fScale, int nUVBand[], int nVNIRBand[],
							  int nSWIRBand[], int nTIHBand[], string &pProFile);

	/*****************************************************************************
	* @brief : ȫ�׶����ݴ�������Ԥ�������ļ���������
	* @author : W.W.Frank
	* @date : 2016/3/30
	* @version : version 1.1
	* @inparam : pCatalogOF	�����ļ�
	//            pRAWData		ԭʼͼ������
	//			  pDarkData     ����������
	//			  pD0Data		0������
	//			  pD0XML		XMLԪ�ļ�
	//			  pD0JPG		0������ͼ
	//			  bSeg			�Ƿ�־�
	//			  nSLine		�־���ʼ��
	//			  nELine		�־���ֹ��
	//			  nALine		�Զ��־�����
	//			  bQuickView	�Ƿ����ͼ
	//			  bGrey			�Ƿ��ǻҶ�ͼ
	//			  fScale		��������
	//			  nUVBand		������Ӳ���
	//			  nVNIRBand		������Ӳ���
	//			  nSWIRBand		������Ӳ���
	//			  nTIHBand		������Ӳ���
	//			  pProFile		��������ļ�
	*****************************************************************************/
	long ExtractSavePreProductXML(char *pCatalogOF, vector<string> &pRawData, vector<string> &pDarkData, vector<string> &pD0Data, vector<string> &pD0Dark, 
								  vector<string> &pD0XML, vector<string> &pD0JPG,bool &bSeg, int &nSLine, int &nELine, int &nALine, bool &bQuickView, 
								  bool &bGrey, float &fScale, int nUVBand[], int nVNIRBand[], int nSWIRBand[], int nTIHBand[], string &pProFile);

	/*****************************************************************************
	* @brief : ȫ�׶����ݴ���ԭʼ����Ԫ���ݶ�ȡ
	* @author : W.W.Frank
	* @date : 2016/3/30
	* @version : version 1.0
	* @inparam : pP0MetaFile	0������Ԫ�ļ�
	//            nSensorType	����������
	//			  nSensorOrder  ������˳��
	//			  nSamples		ͼ����
	//			  nLines		����
	//			  nBands		������
	//			  nHeadOffset	ͷƫ��
	*****************************************************************************/
	long ExtractLoadPreProductMeta(char *pP0MetaFile, unsigned int &nSensorType, unsigned int &nSensorOrder, unsigned int &nSamples, unsigned int &nLines, unsigned int &nBands, __int64 &nHeadOffset);

	/*****************************************************************************
	* @brief : ȫ�׶����ݴ���ԭʼ����Ԫ����д��
	* @author : W.W.Frank
	* @date : 2016/3/30
	* @version : version 1.1
	* @inparam : pP0MetaFile	0����ƷԪ�ļ�
	//            nSensorType	����������
	//			  nSensorOrder  ������˳��
	//			  nSamples		ͼ����
	//			  nLines		����
	//			  nBands		������
	//			  nHeadOffset	ͷƫ��
	*****************************************************************************/
	long ExtractSavePreProductMeta(char *pD0MetaFile, int nSensorType, int	nSensorOrder, int nWidths, int nSamples, int nLines, int nBands, int nYear, 
														int nMonth,int	nDay, int nSecond, __int64 nHeadOffset, __int64 nEofOffset, bool bDark, int nDarkLine);
	//======================================================================================================================================================================================================

	/*****************************************************************************
	* @brief : ��ȡ0�����ݴ������ļ�
	* @author : W.W.Frank
	* @date : 2016/3/18 19:43
	* @version : version 1.0
	* @inparam : pD0Data		0������
	//			  pD0Dark       ����������
	//			  pD0XML		0������Ԫ�ļ�
	//			  pP0Data		0����Ʒ
	//			  pP0XML		0����ƷԪ�ļ�
	//			  pP0JPG		0����Ʒ����ͼ
	//			  pEvent		Event�ļ�
	//			  bQuickView	�Ƿ����ͼ
	//			  bGrey			�Ƿ��ǻҶ�ͼ
	//			  fScale		��������
	//			  nUVBand		������Ӳ���
	//			  nVNIRBand		������Ӳ���
	//			  nSWIRBand		������Ӳ���
	//			  nTIHBand		������Ӳ���
	//			  pProFile		��������ļ�
	*****************************************************************************/
	long ExtractLoadP0ProductXML(char *pP0OF, vector<string> &pD0Data, vector<string> &pD0XML, vector<string> &pD0Dark,
								vector<string> &pP0Data, vector<string> &pP0XML, vector<string> &pP0JPG, vector<string> &pEvent,bool bQuickView, 
								bool bGrey, float fScale, int nUVBand[], int nVNIRBand[], int nSWIRBand[], int nTIHBand[], string &pProFile);

	/*****************************************************************************
	* @brief : ��0�����ݴ�����д��
	* @author : W.W.Frank
	* @date : 2016/3/18 19:48
	* @version : version 1.0
	* @inparam : pP0OF	�����ļ�
	//			  pD0Data		0������
	//			  pD0Dark       ����������
	//			  pD0XML		0������Ԫ�ļ�
	//			  pP0Data		0����Ʒ
	//			  pP0XML		0����ƷԪ�ļ�
	//			  pP0JPG		0����Ʒ����ͼ
	//			  pEvent		Event�ļ�
	//			  bQuickView	�Ƿ����ͼ
	//			  bGrey			�Ƿ��ǻҶ�ͼ
	//			  fScale		��������
	//			  nUVBand		������Ӳ���
	//			  nVNIRBand		������Ӳ���
	//			  nSWIRBand		������Ӳ���
	//			  nTIHBand		������Ӳ���
	//			  pProFile		��������ļ�
	* @outparam :
	*****************************************************************************/
	long ExtractSaveP0ProductXML(char *pP0OF, vector<string> &pD0Data, vector<string> &pD0XML, vector<string> &pD0Dark,
								vector<string> &pP0Data, vector<string> &pP0XML, vector<string> &pP0JPG, vector<string> &pEvent,bool bQuickView, 
								bool bGrey, float fScale, int nUVBand[], int nVNIRBand[], int nSWIRBand[], int nTIHBand[], string &pProFile);

	/*****************************************************************************
	* @brief : ��ȡ0����ƷԪ����
	* @author : W.W.Frank
	* @date : 2016/3/30
	* @version : version 1.1
	* @inparam : pP0MetaFile	0������Ԫ�ļ�
	//            nSensorType	����������
	//			  nSensorOrder  ������˳��
	//			  nSamples		ͼ����
	//			  nLines		����
	//			  nBands		������
	//			  nHeadOffset	ͷƫ��
	*****************************************************************************/
	long ExtractLoadP0ProductMeta(char *pP0MetaFile, unsigned int &nSensorType, unsigned int &nSensorOrder, unsigned int &nSamples, unsigned int &nLines, unsigned int &nBands, __int64 &nHeadOffset);

	/*****************************************************************************
	* @brief : ȫ�׶����ݴ���0����Ʒ�����ļ�����
	* @author : W.W.Frank
	* @date : 2016/3/30
	* @version : version 1.1
	* @inparam : pP0MetaFile	0����ƷԪ�ļ�
	//            nSensorType	����������
	//			  nSensorOrder  ������˳��
	//			  nSamples		ͼ����
	//			  nLines		����
	//			  nBands		������
	//			  nHeadOffset	ͷƫ��
	* @outparam :
	*****************************************************************************/
	long ExtractSaveP0ProductMeta(char *pP0MetaFile, int nSensorType, int nSensorOrder, int nSamples, int nLines, int nBands, __int64 nHeadOffset);

	//======================================================================================================================================================================================================
	/*****************************************************************************
	* @brief : ȫ�׶�Ӱ����1�����ݲ�Ʒ�����ļ�����
	* @author : W.W.Frank
	* @date : 2016/3/30
	* @version : version 1.0
	* @inparam : pP1OF			�����ļ�
	//			  pD0Data		0������
	//			  pD0Dark       ����������
	//			  pD0XML		0������Ԫ�ļ�
	//			  nP0			0����Ʒ��������  0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP1A			1A����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP1B			1B����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP1C			1C����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP1D			1D����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP1E			1E����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  pP0Data		0����Ʒ
	//			  pP0XML		0����ƷԪ�ļ�
	//			  pP0JPG		0����Ʒ����ͼ
	//			  pP1AData		0����Ʒ
	//			  pP1AXML		0����ƷԪ�ļ�
	//			  pP1AJPG		0����Ʒ����ͼ
	//			  pP1BData		0����Ʒ
	//			  pP1BXML		0����ƷԪ�ļ�
	//			  pP1BJPG		0����Ʒ����ͼ
	//			  pP1CData		0����Ʒ
	//			  pP1CXML		0����ƷԪ�ļ�
	//			  pP1CJPG		0����Ʒ����ͼ
	//			  pP1DData		0����Ʒ
	//			  pP1DXML		0����ƷԪ�ļ�
	//			  pP1DJPG		0����Ʒ����ͼ
	//			  pP1EData		0����Ʒ
	//			  pP1EXML		0����ƷԪ�ļ�
	//			  pP1EJPG		0����Ʒ����ͼ
	//			  pReleCof		��Է���У��ϵ��
	//			  pAbCof		���Է���У��ϵ��
	//			  pModtran		�������ļ�
	//			  pWaveLen		ʵ���ҷ��䶨����
	//			  pEvent		Event�ļ�
	//			  fFOLapX		�ӳ��ص�X,8��1
	//			  fFOLapY		�ӳ��ص�Y,8��1
	//			  fSOLapX		�׶��ص�X,3��1
	//			  fSOLapY		�׶��ص�Y,3��1
	//			  bQuickView	�Ƿ����ͼ
	//			  bGrey			�Ƿ��ǻҶ�ͼ
	//			  fScale		��������
	//			  nUVBand		������Ӳ���
	//			  nVNIRBand		������Ӳ���
	//			  nSWIRBand		������Ӳ���
	//			  nTIHBand		������Ӳ���
	//			  pProFile		��������ļ�
	* @outparam :
	*****************************************************************************/
	long ExtractLoadP1ProductXML(char *pP1OF, vector<string> &pD0Data, vector<string> &pD0XML, vector<string> &pD0Dark,
								short &nP0, short &nP1A, short &nP1B, short &nP1C, short &nP1D, short &nP1E,
								vector<string> &pP0Data, vector<string> &pP0XML, vector<string> &pP0JPG, vector<string> &pP1AData, vector<string> &pP1AXML, vector<string> &pP1AJPG,
								vector<string> &pP1BData, vector<string> &pP1BXML, vector<string> &pP1BJPG, vector<string> &pP1CData, vector<string> &pP1CXML, vector<string> &pP1CJPG,
								vector<string> &pP1DData, vector<string> &pP1DXML, vector<string> &pP1DJPG, vector<string> &pP1EData, vector<string> &pP1EXML, vector<string> &pP1EJPG,
								vector<string> &pReleCof, vector<string> &pAbCof, string &pModtran, vector<string> &pWaveLen, vector<string> &pEvent,
								float fFOLapX[], float fFOLapY[], float fSOLapX[], float fSOLapY[],
								bool bQuickView, bool bGrey, float fScale, int nUVBand[], int nVNIRBand[], int nSWIRBand[], int nTIHBand[], string &pProFile);

	/*****************************************************************************
	* @brief : ȫ�׶����ݴ���1�����ݲ�Ʒ�������ļ�д��
	* @author : W.W.Frank
	* @date : 2016/3/30
	* @version : version 1.0
	* @inparam : pP1OF			�����ļ�
	//			  pD0Data		0������
	//			  pD0Dark       ����������
	//			  pD0XML		0������Ԫ�ļ�
	//			  nP0			0����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP1A			1A����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP1B			1B����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP1C			1C����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP1D			1D����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP1E			1E����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  pP0Data		0����Ʒ
	//			  pP0XML		0����ƷԪ�ļ�
	//			  pP0JPG		0����Ʒ����ͼ
	//			  pP1AData		0����Ʒ
	//			  pP1AXML		0����ƷԪ�ļ�
	//			  pP1AJPG		0����Ʒ����ͼ
	//			  pP1BData		0����Ʒ
	//			  pP1BXML		0����ƷԪ�ļ�
	//			  pP1BJPG		0����Ʒ����ͼ
	//			  pP1CData		0����Ʒ
	//			  pP1CXML		0����ƷԪ�ļ�
	//			  pP1CJPG		0����Ʒ����ͼ
	//			  pP1DData		0����Ʒ
	//			  pP1DXML		0����ƷԪ�ļ�
	//			  pP1DJPG		0����Ʒ����ͼ
	//			  pP1EData		0����Ʒ
	//			  pP1EXML		0����ƷԪ�ļ�
	//			  pP1EJPG		0����Ʒ����ͼ
	//			  pReleCof		��Է���У��ϵ��
	//			  pAbCof		���Է���У��ϵ��
	//			  pModtran		�������ļ�
	//			  pWaveLen		ʵ���ҷ��䶨����
	//			  pEvent		Event�ļ�
	//			  fFOLapX		�ӳ��ص�X,8��1
	//			  fFOLapY		�ӳ��ص�Y,8��1
	//			  fSOLapX		�׶��ص�X,3��1
	//			  fSOLapY		�׶��ص�Y,3��1
	//			  bQuickView	�Ƿ����ͼ
	//			  bGrey			�Ƿ��ǻҶ�ͼ
	//			  fScale		��������
	//			  nUVBand		������Ӳ���
	//			  nVNIRBand		������Ӳ���
	//			  nSWIRBand		������Ӳ���
	//			  nTIHBand		������Ӳ���
	//			  pProFile		��������ļ�
	*****************************************************************************/
	long ExtractSaveP1ProductXML(char *pP1OF, vector<string> pD0Data, vector<string> pD0XML, vector<string> pD0Dark,
								short nP0, short nP1A, short nP1B, short nP1C, short nP1D, short nP1E,
								vector<string> pP0Data, vector<string> pP0XML, vector<string> pP0JPG, vector<string> pP1AData, vector<string> pP1AXML, vector<string> pP1AJPG,
								vector<string> pP1BData, vector<string> pP1BXML, vector<string> pP1BJPG, vector<string> pP1CData, vector<string> pP1CXML, vector<string> pP1CJPG,
								vector<string> pP1DData, vector<string> pP1DXML, vector<string> pP1DJPG, vector<string> pP1EData, vector<string> pP1EXML, vector<string> pP1EJPG,
								vector<string> pReleCof, vector<string> pAbCof, string pModtran, vector<string> pWaveLen, vector<string> pEvent,
								float fFOLapX[], float fFOLapY[], float fSOLapX[], float fSOLapY[],
								bool bQuickView, bool bGrey, float fScale, int nUVBand[], int nVNIRBand[], int nSWIRBand[], int nTIHBand[], string pProFile);

	//======================================================================================================================================================================================================
	/*****************************************************************************
	* @brief : ȫ�׶����ݴ���2�����ݲ�Ʒ�����ļ�����
	* @author : W.W.Frank
	* @date : 2016/3/30
	* @version : version 1.1
	* @inparam : pP1OF			�����ļ�
	//			  pD0Data		0������
	//			  pD0Dark       ����������
	//			  pD0XML		0������Ԫ�ļ�
	//			  nP0			0����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP1A			1A����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP1B			1B����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP1C			1C����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP1D			1D����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP1E			1E����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP2A			2A����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP2B			2B����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP2C			2C����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  pP0Data		0����Ʒ
	//			  pP0XML		0����ƷԪ�ļ�
	//			  pP0JPG		0����Ʒ����ͼ
	//			  pP1AData		1A����Ʒ
	//			  pP1AXML		1A����ƷԪ�ļ�
	//			  pP1AJPG		1A����Ʒ����ͼ
	//			  pP1BData		1B����Ʒ
	//			  pP1BXML		1B����ƷԪ�ļ�
	//			  pP1BJPG		1B����Ʒ����ͼ
	//			  pP1CData		1C����Ʒ
	//			  pP1CXML		1C����ƷԪ�ļ�
	//			  pP1CJPG		1C����Ʒ����ͼ
	//			  pP1DData		1D����Ʒ
	//			  pP1DXML		1D����ƷԪ�ļ�
	//			  pP1DJPG		1D����Ʒ����ͼ
	//			  pP1EData		1E����Ʒ
	//			  pP1EXML		1E����ƷԪ�ļ�
	//			  pP1EJPG		1E����Ʒ����ͼ
	//			  pP2AData		2A����Ʒ
	//			  pP2AXML		2A����ƷԪ�ļ�
	//			  pP2AJPG		2A����Ʒ����ͼ
	//			  pP2BData		2B����Ʒ
	//			  pP2BXML		2B����ƷԪ�ļ�
	//			  pP2BJPG		2B����Ʒ����ͼ
	//			  pP2CData		2C����Ʒ
	//			  pP2CXML		2C����ƷԪ�ļ�
	//			  pP2CJPG		2C����Ʒ����ͼ
	//			  pReleCof		��Է���У��ϵ��
	//			  pAbCof		���Է���У��ϵ��
	//			  pModtran		�������ļ�
	//			  pWaveLen		ʵ���ҷ��䶨����
	//			  pEvent		Event�ļ�
	//			  pSbet			SBET�ļ�
	//			  pPOS			POS�ļ�
	//			  pEOF			EOF�ļ�
	//			  pDEM			DEM�ļ�
	//			  fFOLapX		�ӳ��ص�X,8��1
	//			  fFOLapY		�ӳ��ص�Y,8��1
	//			  fSOLapX		�׶��ص�X,3��1
	//			  fSOLapY		�׶��ص�Y,3��1
	//			  fFov			���ӳ�,4��1
	//			  fIFov			˲ʱ�ӳ�,8��1
	//			  fFocalLen		����,4��1
	//			  dBoresightMis	��׼��ƫ��,3��1
	//			  dGNSSOffset	λ��ƫ��,3��1
	//			  dXYZOffset	����λ��ƫ��,3��1
	//			  nBand			�ز����Ĳ�����,4��1
	//			  nSamples		�ز������׶���,4��1
	//			  bQuickView	�Ƿ����ͼ
	//			  bGrey			�Ƿ��ǻҶ�ͼ
	//			  fScale		��������
	//			  nUVBand		������Ӳ���
	//			  nVNIRBand		������Ӳ���
	//			  nSWIRBand		������Ӳ���
	//			  nTIHBand		������Ӳ���
	//			  pProFile		��������ļ�
	*****************************************************************************/
	long ExtractLoadP2ProductXML(char *pP2OF, vector<string> &pD0Data, vector<string> &pD0XML, vector<string> &pD0Dark,
								short &nP0, short &nP1A, short &nP1B, short &nP1C, short &nP1D, short &nP1E, short &nP2A, short &nP2B, short &nP2C,
								vector<string> &pP0Data, vector<string> &pP0XML, vector<string> &pP0JPG, vector<string> &pP1AData, vector<string> &pP1AXML, vector<string> &pP1AJPG,
								vector<string> &pP1BData, vector<string> &pP1BXML, vector<string> &pP1BJPG, vector<string> &pP1CData, vector<string> &pP1CXML, vector<string> &pP1CJPG,
								vector<string> &pP1DData, vector<string> &pP1DXML, vector<string> &pP1DJPG, vector<string> &pP1EData, vector<string> &pP1EXML, vector<string> &pP1EJPG,
								vector<string> &pP2AData, vector<string> &pP2AXML, vector<string> &pP2AJPG,
								vector<string> &pP2BData, vector<string> &pP2BXML, vector<string> &pP2BJPG, vector<string> &pP2CData, vector<string> &pP2CXML, vector<string> &pP2CJPG,
								vector<string> &pReleCof, vector<string> &pAbCof, string &pModtran, vector<string> &pWaveLen, vector<string> &pEvent,
								string &pSbet, vector<string> &pPOS, vector<string> &pEOF, string &pDEM,
								float fFOLapX[], float fFOLapY[], float fSOLapX[], float fSOLapY[],
								float fFov[], float fIFov[], float fFocalLen[],
								double dBoresightMis[], double dGNSSOffset[], double dXYZOffset[], int nBand[], int nSamples[],
								bool bQuickView, bool bGrey, float fScale, int nUVBand[], int nVNIRBand[], int nSWIRBand[], int nTIHBand[], string &pProFile);

	/*****************************************************************************
	* @brief : ȫ�׶����ݴ���2�����ݲ�Ʒ�����ļ�����
	* @author : W.W.Frank
	* @date : 2016/3/30
	* @version : version 1.1
	* @inparam : pP1OF			�����ļ�
	//			  pD0Data		0������
	//			  pD0Dark       ����������
	//			  pD0XML		0������Ԫ�ļ�
	//			  nP0			0����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP1A			1A����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP1B			1B����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP1C			1C����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP1D			1D����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP1E			1E����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP2A			2A����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP2B			2B����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  nP2C			2C����Ʒ�������� 0�������д���1�����д������������2�������������,3���������������ɿ���ͼ
	//			  pP0Data		0����Ʒ
	//			  pP0XML		0����ƷԪ�ļ�
	//			  pP0JPG		0����Ʒ����ͼ
	//			  pP1AData		1A����Ʒ
	//			  pP1AXML		1A����ƷԪ�ļ�
	//			  pP1AJPG		1A����Ʒ����ͼ
	//			  pP1BData		1B����Ʒ
	//			  pP1BXML		1B����ƷԪ�ļ�
	//			  pP1BJPG		1B����Ʒ����ͼ
	//			  pP1CData		1C����Ʒ
	//			  pP1CXML		1C����ƷԪ�ļ�
	//			  pP1CJPG		1C����Ʒ����ͼ
	//			  pP1DData		1D����Ʒ
	//			  pP1DXML		1D����ƷԪ�ļ�
	//			  pP1DJPG		1D����Ʒ����ͼ
	//			  pP1EData		1E����Ʒ
	//			  pP1EXML		1E����ƷԪ�ļ�
	//			  pP1EJPG		1E����Ʒ����ͼ
	//			  pP2AData		2A����Ʒ
	//			  pP2AXML		2A����ƷԪ�ļ�
	//			  pP2AJPG		2A����Ʒ����ͼ
	//			  pP2BData		2B����Ʒ
	//			  pP2BXML		2B����ƷԪ�ļ�
	//			  pP2BJPG		2B����Ʒ����ͼ
	//			  pP2CData		2C����Ʒ
	//			  pP2CXML		2C����ƷԪ�ļ�
	//			  pP2CJPG		2C����Ʒ����ͼ
	//			  pReleCof		��Է���У��ϵ��
	//			  pAbCof		���Է���У��ϵ��
	//			  pModtran		�������ļ�
	//			  pWaveLen		ʵ���ҷ��䶨����
	//			  pEvent		Event�ļ�
	//			  pSbet			SBET�ļ�
	//			  pPOS			POS�ļ�
	//			  pEOF			EOF�ļ�
	//			  pDEM			DEM�ļ�
	//			  fFOLapX		�ӳ��ص�X,8��1
	//			  fFOLapY		�ӳ��ص�Y,8��1
	//			  fSOLapX		�׶��ص�X,3��1
	//			  fSOLapY		�׶��ص�Y,3��1
	//			  fFov			���ӳ�,4��1
	//			  fIFov			˲ʱ�ӳ�,8��1
	//			  fFocalLen		����,4��1
	//			  dBoresightMis	��׼��ƫ��,3��1
	//			  dGNSSOffset	λ��ƫ��,3��1
	//			  dXYZOffset	����λ��ƫ��,3��1
	//			  nBand			�ز����Ĳ�����,4��1
	//			  nSamples		�ز������׶���,4��1
	//			  bQuickView	�Ƿ����ͼ
	//			  bGrey			�Ƿ��ǻҶ�ͼ
	//			  fScale		��������
	//			  nUVBand		������Ӳ���
	//			  nVNIRBand		������Ӳ���
	//			  nSWIRBand		������Ӳ���
	//			  nTIHBand		������Ӳ���
	//			  pProFile		��������ļ�
	*****************************************************************************/
	long ExtractSaveP2ProductXML(char *pP2OF, vector<string> &pD0Data, vector<string> &pD0XML, vector<string> &pD0Dark,
		short &nP0, short &nP1A, short &nP1B, short &nP1C, short &nP1D, short &nP1E, short &nP2A, short &nP2B, short &nP2C,
		vector<string> &pP0Data, vector<string> &pP0XML, vector<string> &pP0JPG, vector<string> &pP1AData, vector<string> &pP1AXML, vector<string> &pP1AJPG,
		vector<string> &pP1BData, vector<string> &pP1BXML, vector<string> &pP1BJPG, vector<string> &pP1CData, vector<string> &pP1CXML, vector<string> &pP1CJPG,
		vector<string> &pP1DData, vector<string> &pP1DXML, vector<string> &pP1DJPG, vector<string> &pP1EData, vector<string> &pP1EXML, vector<string> &pP1EJPG,
		vector<string> &pP2AData, vector<string> &pP2AXML, vector<string> &pP2AJPG,
		vector<string> &pP2BData, vector<string> &pP2BXML, vector<string> &pP2BJPG, vector<string> &pP2CData, vector<string> &pP2CXML, vector<string> &pP2CJPG,
		vector<string> &pReleCof, vector<string> &pAbCof, string &pModtran, vector<string> &pWaveLen, vector<string> &pEvent,
		string &pSbet, vector<string> &pPOS, vector<string> &pEOF, string &pDEM,
		float fFOLapX[], float fFOLapY[], float fSOLapX[], float fSOLapY[],
		float fFov[], float fIFov[], float fFocalLen[],
		double dBoresightMis[], double dGNSSOffset[], double dXYZOffset[], int nBand[], int nSamples[],
		bool bQuickView, bool bGrey, float fScale, int nUVBand[], int nVNIRBand[], int nSWIRBand[], int nTIHBand[], string &pProFile);
};