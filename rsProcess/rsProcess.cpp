// rsProcess.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include"AerialProduct\ProductTest.h"
#include"UAV\UAVUnityTest.h"
#include"lidar\LidarReader.h"

float main()
{
	//QPDPreProcessUnitTestFunc();
	//QPDLevel0ProcessUnitTestFunc();
	//QPDLevel1ProcessUnitTestFunc();
	//QPDLevel2ProcessUnitTestFunc();
	//UAVGeoCorrectionTest();
	LASLidarReader m_lasReader;
	XYZLidarReader m_xyzReader;
	GDALProcessBase* m_progress=new GDALConsoleProcess();
	m_lasReader.LidarReader_SetProgress(m_progress);
	m_xyzReader.LidarReader_SetProgress(m_progress);
	m_lasReader.LidarReader_Open("D:\\las�ļ�\\1.las");
	m_lasReader.LidarReader_Read(true);
	m_xyzReader.LidarReader_Open("D:\\las�ļ�\\las.txt");
	m_xyzReader.LidarReader_Read(true);
	m_xyzReader.LidarReader_Write("D:\\las�ļ�\\200.las");
	m_xyzReader.LidarReader_WriteXYZ("D:\\las�ļ�\\300.txt");
	return 0;
}

