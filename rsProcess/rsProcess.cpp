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
	GDALProcessBase* m_progress=new GDALConsoleProcess();
	m_lasReader.LidarReader_SetProgress(m_progress);
	m_lasReader.LidarReader_Open("D:\\las�ļ�\\1.las");
	m_lasReader.LidarReader_Read(true);
	m_lasReader.LidarReader_Write("D:\\las�ļ�\\100.las");

	return 0;
}

