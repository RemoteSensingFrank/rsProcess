// rsProcess.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include"lidar\LidarAlgorithm.h"
#include"lidar\LidarReader.h"
#include"RenderProcess.h"
#include"UAV\UAVGeoCorrection.h"
#include"UAV\UAVMosaicFast.h"
#include"AuxiliaryFunction.h"
#include"experiment\HyperRepair.h"
#include"experiment\BSCB.h"
#include"experiment\ExemplarBased.h"
float main()
{

	//UAVMosaicFast m_mosaic_fast;
	//ImgFeaturesTools m_img_tools;
	//vector<vector<Point2f>> m_pnts;
	//vector<string> pszPath;
	//int width, height;
	//GetImageList("D:\\my_doc\\2015.12.18�������˻�����\\DCIM\\100MSDCF\\test.txt", pszPath);
	//m_mosaic_fast.UAVMosaicFast_Mosaic(pszPath, "D:\\img.tif");

	//m_mosaic_fast.UAVMosaicFast_AffineTrans(pszPath);
	//m_mosaic_fast.UAVMosaicFast_GetMosaicRange(pszPath,width, height);
	//m_mosaic_fast.UAVMosaicFast_AffineTrans(pszPath);
	//
	//LidarReader *reader = new LASLidarReader();
	//reader->LidarReader_Open("D:\\las�ļ�\\cc_000341.las");
	//reader->LidarReader_Read(true, 10);
	////LidarTinCreateLocal(reader->m_lasDataset);
	//GDALTriangulation *lasTriangle = NULL;
	//lasTriangle=LidarTinCreateGlobal(reader->m_lasDataset);
	//LidarTriangleRenderGlobal(&reader->m_lasDataset, lasTriangle);

	//QPDPreProcessUnitTestFunc();
	//QPDLevel0ProcessUnitTestFunc();
	//QPDLevel1ProcessUnitTestFunc();
	//QPDLevel2ProcessUnitTestFunc();
	//
	//UAVGeoCorrectionTest();

	//ImageSegmentTools tools;
	//tools.ImgSegmentTools_WaterShed("C:\\Users\\Public\\Pictures\\Sample Pictures\\repairTest.bmp", "D:\\watershed.bmp");
	//tools.ImgSegmentTools_Canny("C:\\Users\\Public\\Pictures\\Sample Pictures\\badimg.bmp", "D:\\cannyedge.bmp");

	//BezierCurve curve;
	//curve.BezierCurve_BezierDraw();
	//
	//experiment_process();

	//ImageInpaint m_inPaint;
	//m_inPaint.ImageInpaint_Inpaint("D:\\1.bmp","test.jpg");
	//UAVGeoCorrection m_uav_correct;
	//m_uav_correct.UAVGeoCorrection_GeoCorrect("D:\\my_doc\\2015.12.18�������˻�����\\DCIM\\100MSDCF", "D:", "D:\\my_doc\\2015.12.18�������˻�����\\�½��ı��ĵ�.txt", NULL, 6, 0, 1, 0.035, 0.2, 0);

	//PhotogrammetryToolsTest();

	//correct_non_nonhomogeneity("C:\\Users\\Public\\Pictures\\Sample Pictures\\hyperspectralRGB.tif","C:\\Users\\Public\\Pictures\\Sample Pictures\\hyperspectralCorrectRGB.tif");
	
	CPOINT pnt1, pnt2;
	pnt1.x = 228; pnt1.y = 60;
	pnt2.x = 268; pnt2.y = 100;
	set_mask_region("D:\\mask.bmp", "D:\\mask.tif", pnt1, pnt2);
	set_mask_region("D:\\can_tmr.bmp", "D:\\can_tmr.tif", pnt1, pnt2);
	//vector<Edge_Pixels> edgeinter;
	//get_segment_edge("D:\\ImgCutSimulate1", edgeinter);

	ExemplarBased m_eb;
	m_eb.ExemplarBased_InpaintTexture("D:\\can_tmr.tif", "D:\\mask.tif","D:\\texture.tif", "D:\\Repair.tif");
	//BSCB m_BSCB;
	//m_BSCB.BSCBImageInpaintingProcess("D:\\can_tmr.tif", "D:\\mask.tif", "D:\\RepairD.tif");
	return 0;
}
