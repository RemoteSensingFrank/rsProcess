#include"stdafx.h"
#include"ProductTest.h"

//ȫ�׶�����Ԥ����Ԫ����
void QPDPreProcessUnitTestFunc()
{
	char* pathRaw1 = "E:\\73\\rawdata\\_0101_20160229_115734783.dat";
	char* pathRaw2 = "E:\\73\\rawdata\\_0301_20160229_115734783.dat";
	char* pathRaw3 = "E:\\73\\rawdata\\_0401_20160229_115734783.dat";
	char* pathRaw4 = "E:\\73\\rawdata\\_0501_20160229_115734783.dat";
	char* pathRaw5 = "E:\\73\\rawdata\\_0601_20160229_115734783.dat";
	char* pathRaw6 = "E:\\73\\rawdata\\_0701_20160229_115734783.dat";

	char* pathRaw1Out = "E:\\73\\test\\Catalog\\D0_0101_20160229_115734783.dat";
	char* pathRaw2Out = "E:\\73\\test\\Catalog\\D0_0301_20160229_115734783.dat";
	char* pathRaw3Out = "E:\\73\\test\\Catalog\\D0_0401_20160229_115734783.dat";
	char* pathRaw4Out = "E:\\73\\test\\Catalog\\D0_0501_20160229_115734783.dat";
	char* pathRaw5Out = "E:\\73\\test\\Catalog\\D0_0601_20160229_115734783.dat";
	char* pathRaw6Out = "E:\\73\\test\\Catalog\\D0_0701_20160229_115734783.dat";

	char* pathLeak1Out = "E:\\73\\test\\Catalog\\D0Leak_0101_20160229_115734783.dat";
	char* pathLeak2Out = "E:\\73\\test\\Catalog\\D0Leak_0301_20160229_115734783.dat";
	char* pathLeak3Out = "E:\\73\\test\\Catalog\\D0Leak_0401_20160229_115734783.dat";
	char* pathLeak4Out = "E:\\73\\test\\Catalog\\D0Leak_0501_20160229_115734783.dat";
	char* pathLeak5Out = "E:\\73\\test\\Catalog\\D0Leak_0601_20160229_115734783.dat";
	char* pathLeak6Out = "E:\\73\\test\\Catalog\\D0Leak_0701_20160229_115734783.dat";

	QPDPreProcess mQPDProcess;
	const int nFixLines = 500;
	DINFO mDataHeader;
	vector<short> nLeakFrameType; vector<int> nLeakFrameSize; vector<__int64> nLeakByteCount; int nLeakFrameCount = 0;
	mQPDProcess.PreProc_GenerateD0Data(pathRaw1, pathRaw1Out, mDataHeader, nLeakFrameType, nLeakFrameSize, nLeakFrameCount, nFixLines);
	mQPDProcess.PrePRoc_WriteLeakInfo(pathLeak1Out, nLeakFrameType, nLeakFrameSize, nLeakFrameCount);

	mQPDProcess.PreProc_GenerateD0Data(pathRaw2, pathRaw2Out, mDataHeader, nLeakFrameType, nLeakFrameSize, nLeakFrameCount, nFixLines);
	mQPDProcess.PrePRoc_WriteLeakInfo(pathLeak2Out, nLeakFrameType, nLeakFrameSize, nLeakFrameCount);

	mQPDProcess.PreProc_GenerateD0Data(pathRaw3, pathRaw3Out, mDataHeader, nLeakFrameType, nLeakFrameSize, nLeakFrameCount, nFixLines);
	mQPDProcess.PrePRoc_WriteLeakInfo(pathLeak3Out, nLeakFrameType, nLeakFrameSize, nLeakFrameCount);

	mQPDProcess.PreProc_GenerateD0Data(pathRaw4, pathRaw4Out, mDataHeader, nLeakFrameType, nLeakFrameSize, nLeakFrameCount, nFixLines);
	mQPDProcess.PrePRoc_WriteLeakInfo(pathLeak4Out, nLeakFrameType, nLeakFrameSize, nLeakFrameCount);

	mQPDProcess.PreProc_GenerateD0Data(pathRaw5, pathRaw5Out, mDataHeader, nLeakFrameType, nLeakFrameSize, nLeakFrameCount, nFixLines);
	mQPDProcess.PrePRoc_WriteLeakInfo(pathLeak5Out, nLeakFrameType, nLeakFrameSize, nLeakFrameCount);

	mQPDProcess.PreProc_GenerateD0Data(pathRaw6, pathRaw6Out, mDataHeader, nLeakFrameType, nLeakFrameSize, nLeakFrameCount, nFixLines);
	mQPDProcess.PrePRoc_WriteLeakInfo(pathLeak6Out, nLeakFrameType, nLeakFrameSize, nLeakFrameCount);
}

//ȫ�׶����ݴ���0�����ݲ�Ʒ��������
void QPDLevel0ProcessUnitTestFunc()
{
	char* pathRaw1Out = "E:\\73\\test\\Catalog\\D0_0101_20160229_115734783.dat";
	char* pathRaw2Out = "E:\\73\\test\\Catalog\\D0_0301_20160229_115734783.dat";
	char* pathRaw3Out = "E:\\73\\test\\Catalog\\D0_0401_20160229_115734783.dat";
	char* pathRaw4Out = "E:\\73\\test\\Catalog\\D0_0501_20160229_115734783.dat";
	char* pathRaw5Out = "E:\\73\\test\\Catalog\\D0_0601_20160229_115734783.dat";
	char* pathRaw6Out = "E:\\73\\test\\Catalog\\D0_0701_20160229_115734783.dat";

	char* pathEventOut1 = "E:\\73\\test\\Event_POS\\EVENT_0101_20160229_115734783.txt";
	char* pathEventOut2 = "E:\\73\\test\\Event_POS\\EVENT_0301_20160229_115734783.txt";
	char* pathEventOut3 = "E:\\73\\test\\Event_POS\\EVENT_0401_20160229_115734783.txt";
	char* pathEventOut4 = "E:\\73\\test\\Event_POS\\EVENT_0501_20160229_115734783.txt";
	char* pathEventOut5 = "E:\\73\\test\\Event_POS\\EVENT_0601_20160229_115734783.txt";
	char* pathEventOut6 = "E:\\73\\test\\Event_POS\\EVENT_0701_20160229_115734783.txt";
	char* pathModifyEvent1 = "E:\\73\\test\\Event_POS\\EVENTModify_0101_20160229_115734783.txt";
	char* pathModifyEvent2 = "E:\\73\\test\\Event_POS\\EVENTModify_0101_20160229_115734783.txt";

	char* pathBSQ1 = "E:\\73\\test\\Level0Product\\P0\\P0_0101_20160229_115734783.dat";
	char* pathBSQ2 = "E:\\73\\test\\Level0Product\\P0\\P0_0301_20160229_115734783.dat";
	char* pathBSQ3 = "E:\\73\\test\\Level0Product\\P0\\P0_0401_20160229_115734783.dat";
	char* pathBSQ4 = "E:\\73\\test\\Level0Product\\P0\\P0_0501_20160229_115734783.dat";
	char* pathBSQ5 = "E:\\73\\test\\Level0Product\\P0\\P0_0601_20160229_115734783.dat";
	char* pathBSQ6 = "E:\\73\\test\\Level0Product\\P0\\P0_0701_20160229_115734783.dat";

	char* pathBSQUseful1 = "E:\\73\\test\\Level0Product\\P0\\P01_0101_20160229_115734783.dat";
	char* pathBSQUseful2 = "E:\\73\\test\\Level0Product\\P0\\P01_0301_20160229_115734783.dat";
	char* pathBSQUseful3 = "E:\\73\\test\\Level0Product\\P0\\P01_0401_20160229_115734783.dat";
	char* pathBSQUseful4 = "E:\\73\\test\\Level0Product\\P0\\P01_0501_20160229_115734783.dat";
	char* pathBSQUseful5 = "E:\\73\\test\\Level0Product\\P0\\P01_0601_20160229_115734783.dat";
	char* pathBSQUseful6 = "E:\\73\\test\\Level0Product\\P0\\P01_0701_20160229_115734783.dat";

	char* pathBSQNonuniform1 = "E:\\73\\test\\Level0Product\\P0Noneuniform\\P01_0101_20160229_115734783.dat";
	char* pathBSQNonuniform2 = "E:\\73\\test\\Level0Product\\P0Noneuniform\\P01_0301_20160229_115734783.dat";
	char* pathBSQNonuniform3 = "E:\\73\\test\\Level0Product\\P0Noneuniform\\P01_0401_20160229_115734783.dat";
	char* pathBSQNonuniform4 = "E:\\73\\test\\Level0Product\\P0Noneuniform\\P01_0501_20160229_115734783.dat";
	char* pathBSQNonuniform5 = "E:\\73\\test\\Level0Product\\P0Noneuniform\\P01_0601_20160229_115734783.dat";
	char* pathBSQNonuniform6 = "E:\\73\\test\\Level0Product\\P0Noneuniform\\P01_0701_20160229_115734783.dat";

	//char* pathBSQNonuniform1 = "E:\\73\\test\\Level0Product\\P0Noneuniform\\P01_0101_20160229_115734783.dat";
	//char* pathBSQNonuniform2 = "E:\\73\\test\\Level0Product\\P0Noneuniform\\P01_0301_20160229_115734783.dat";
	//char* pathBSQNonuniform3 = "E:\\73\\test\\Level0Product\\P0Noneuniform\\P01_0401_20160229_115734783.dat";
	//char* pathBSQNonuniform4 = "E:\\73\\test\\Level0Product\\P0Noneuniform\\P01_0501_20160229_115734783.dat";
	//char* pathBSQNonuniform5 = "E:\\73\\test\\Level0Product\\P0Noneuniform\\P01_0601_20160229_115734783.dat";
	//char* pathBSQNonuniform6 = "E:\\73\\test\\Level0Product\\P0Noneuniform\\P01_0701_20160229_115734783.dat";

	QPDLevel0Process m_level0Proc;
	//����EVNET�ļ�
	m_level0Proc.Level0Proc_ExtractEvent(pathRaw1Out, pathEventOut1, 18);
	m_level0Proc.Level0Proc_ExtractEvent(pathRaw2Out, pathEventOut2, 18);
	m_level0Proc.Level0Proc_ExtractEvent(pathRaw3Out, pathEventOut3, 18);
	m_level0Proc.Level0Proc_ExtractEvent(pathRaw4Out, pathEventOut4, 18);
	m_level0Proc.Level0Proc_ExtractEvent(pathRaw5Out, pathEventOut5, 18);
	m_level0Proc.Level0Proc_ExtractEvent(pathRaw6Out, pathEventOut6, 18);
	//m_level0Proc.Level0Proc_ModifySWIREvent(pathModifyEvent1,);
	//m_level0Proc.Level0Proc_ModifySWIREvent(pathModifyEvent2,);

	//BILת��ΪBSQ
	//m_level0Proc.Level0Proc_RawToBSQ(pathRaw1Out, pathBSQ1);
	//m_level0Proc.Level0Proc_RawToBSQ(pathRaw2Out, pathBSQ2);
	//m_level0Proc.Level0Proc_RawToBSQ(pathRaw3Out, pathBSQ3);
	//m_level0Proc.Level0Proc_RawToBSQ(pathRaw4Out, pathBSQ4);
	//m_level0Proc.Level0Proc_RawToBSQ(pathRaw5Out, pathBSQ5);
	//m_level0Proc.Level0Proc_RawToBSQ(pathRaw6Out, pathBSQ6);

	////��ȥ��������
	//vector<int> bandSWIR;
	//for (int i = 2; i <= 513; ++i)
	//	bandSWIR.push_back(i);
	//m_level0Proc.Level0Proc_GetUsefulData(pathBSQ1, pathBSQUseful1, 8, 0, bandSWIR);
	//m_level0Proc.Level0Proc_GetUsefulData(pathBSQ2, pathBSQUseful2, 8, 0, bandSWIR);
	//vector<int> bandVNirTir;
	//for (int i = 2; i <= 257; ++i)
	//	bandVNirTir.push_back(i);
	//m_level0Proc.Level0Proc_GetUsefulData(pathBSQ3, pathBSQUseful3, 8, 0, bandVNirTir);
	//m_level0Proc.Level0Proc_GetUsefulData(pathBSQ4, pathBSQUseful4, 8, 0, bandVNirTir);
	//m_level0Proc.Level0Proc_GetUsefulData(pathBSQ5, pathBSQUseful5, 8, 0, bandVNirTir);
	//m_level0Proc.Level0Proc_GetUsefulData(pathBSQ6, pathBSQUseful6, 8, 0, bandVNirTir);

	////�Ǿ�����У��
	//m_level0Proc.Level0Proc_Nonuniform(pathBSQUseful1, pathBSQNonuniform1);
	//m_level0Proc.Level0Proc_Nonuniform(pathBSQUseful2, pathBSQNonuniform2);
	//m_level0Proc.Level0Proc_Nonuniform(pathBSQUseful3, pathBSQNonuniform3);
	//m_level0Proc.Level0Proc_Nonuniform(pathBSQUseful4, pathBSQNonuniform4);
	//m_level0Proc.Level0Proc_Nonuniform(pathBSQUseful5, pathBSQNonuniform5);
	//m_level0Proc.Level0Proc_Nonuniform(pathBSQUseful6, pathBSQNonuniform6);
}

//ȫ�׶����ݴ���1�����ݲ�Ʒ��������
void QPDLevel1ProcessUnitTestFunc()
{
	char* pathBSQNonuniform1 = "E:\\73\\test\\Level0Product\\P0\\P0_0101_20160229_115734783";
	char* pathBSQNonuniform2 = "E:\\73\\test\\Level0Product\\P0\\P0_0301_20160229_115734783";
	char* pathBSQNonuniform3 = "E:\\73\\test\\Level0Product\\P0\\P0_0401_20160229_115734783";
	char* pathBSQNonuniform4 = "E:\\73\\test\\Level0Product\\P0\\P0_0501_20160229_115734783";
	char* pathBSQNonuniform5 = "E:\\73\\test\\Level0Product\\P0\\P0_0601_20160229_115734783";
	char* pathBSQNonuniform6 = "E:\\73\\test\\Level0Product\\P0\\P0_0701_20160229_115734783";

	char* pathRelativeCalib1 = "E:\\�ⳡ����ϵ��\\RC_0101_20160225.txt";
	char* pathRelativeCalib2 = "E:\\�ⳡ����ϵ��\\RC_0301_20160225.txt";
	char* pathRelativeCalib3 = "E:\\�ⳡ����ϵ��\\RC_0401_20160225.txt";
	char* pathRelativeCalib4 = "E:\\�ⳡ����ϵ��\\RC_0501_20160225.txt";
	char* pathRelativeCalib5 = "E:\\�ⳡ����ϵ��\\RC_0601_20160225.txt";
	char* pathRelativeCalib6 = "E:\\�ⳡ����ϵ��\\RC_0701_20160225.txt";

	char* pathRelativeRad1 = "D:\\P1_0101_20160229_115734783";
	char* pathRelativeRad2 = "D:\\P1_0301_20160229_115734783";
	char* pathRelativeRad3 = "E:\\73\\test\\Level1Product\\P1_0401_20160229_115734783";
	char* pathRelativeRad4 = "E:\\73\\test\\Level1Product\\P1_0501_20160229_115734783";
	char* pathRelativeRad5 = "E:\\73\\test\\Level1Product\\P1_0601_20160229_115734783";
	char* pathRelativeRad6 = "E:\\73\\test\\Level1Product\\P1_0701_20160229_115734783";

	QPDLevel1Process m_level1Proc;
	//��Է���У��
	//m_level1Proc.Level1Proc_RadiationRelative(pathBSQNonuniform1, pathRelativeRad1, pathRelativeCalib1);
	//m_level1Proc.Level1Proc_RadiationRelative(pathBSQNonuniform2, pathRelativeRad2, pathRelativeCalib2);
	//m_level1Proc.Level1Proc_RadiationRelative(pathBSQNonuniform3, pathRelativeRad3, pathRelativeCalib3);
	//m_level1Proc.Level1Proc_RadiationRelative(pathBSQNonuniform4, pathRelativeRad4, pathRelativeCalib4);
	//m_level1Proc.Level1Proc_RadiationRelative(pathBSQNonuniform5, pathRelativeRad5, pathRelativeCalib5);
	//m_level1Proc.Level1Proc_RadiationRelative(pathBSQNonuniform6, pathRelativeRad6, pathRelativeCalib6);

	//�ӳ�ƴ�������ɫ�ƺ�Ҳû��ʲô�ܺõİ취
	//m_level1Proc.Level1Proc_ViewJointTwo( "D:\\SWIR3","D:\\SWIR1", 204, 83, "D:\\Joint.tif");
}

//ȫ�׶�2�����ݲ�Ʒ��������
void QPDLevel2ProcessUnitTestFunc()
{
	char* pathSBET = "";

	char* pathBSQNonuniform1 = "E:\\73\\test\\Level0Product\\P0Noneuniform\\P01_0101_20160229_115734783.dat";
	char* pathBSQNonuniform2 = "E:\\73\\test\\Level0Product\\P0Noneuniform\\P01_0301_20160229_115734783.dat";
	char* pathBSQNonuniform3 = "E:\\73\\test\\Level0Product\\P0Noneuniform\\P01_0401_20160229_115734783.dat";
	char* pathBSQNonuniform4 = "E:\\73\\test\\Level0Product\\P0Noneuniform\\P01_0501_20160229_115734783.dat";
	char* pathBSQNonuniform5 = "E:\\73\\test\\Level0Product\\P0Noneuniform\\P01_0601_20160229_115734783.dat";
	char* pathBSQNonuniform6 = "E:\\73\\test\\Level0Product\\P0Noneuniform\\P01_0701_20160229_115734783.dat";

	char* pathEventOut1 = "E:\\73\\test\\Catalog\\EVENTModify_0101_20160229_115734783.txt";
	char* pathEventOut2 = "E:\\73\\test\\Catalog\\EVENTModify_0101_20160229_115734783.txt";
	char* pathEventOut3 = "E:\\73\\test\\Catalog\\EVENT_0401_20160229_115734783.txt";
	char* pathEventOut4 = "E:\\73\\test\\Catalog\\EVENT_0501_20160229_115734783.txt";
	char* pathEventOut5 = "E:\\73\\test\\Catalog\\EVENT_0601_20160229_115734783.txt";
	char* pathEventOut6 = "E:\\73\\test\\Catalog\\EVENT_0701_20160229_115734783.txt";

	char* pathPOS1 = "E:\\73\\test\\P2\\POS_0101_20160229_115734783.txt";
	char* pathPOS2 = "E:\\73\\test\\P2\\POS_0101_20160229_115734783.txt";
	char* pathPOS3 = "E:\\73\\test\\P2\\POS_0401_20160229_115734783.txt";
	char* pathPOS4 = "E:\\73\\test\\P2\\POS_0501_20160229_115734783.txt";
	char* pathPOS5 = "E:\\73\\test\\P2\\POS_0601_20160229_115734783.txt";
	char* pathPOS6 = "E:\\73\\test\\P2\\POS_0701_20160229_115734783.txt";


	char* pathEO1 = "E:\\73\\test\\P2\\EO_0101_20160229_115734783.txt";
	char* pathEO2 = "E:\\73\\test\\P2\\EO_0101_20160229_115734783.txt";
	char* pathEO3 = "E:\\73\\test\\P2\\EO_0401_20160229_115734783.txt";
	char* pathEO4 = "E:\\73\\test\\P2\\EO_0501_20160229_115734783.txt";
	char* pathEO5 = "E:\\73\\test\\P2\\EO_0601_20160229_115734783.txt";
	char* pathEO6 = "E:\\73\\test\\P2\\EO_0701_20160229_115734783.txt";
	QPDGeoPOSProcess m_QPDPOSProc;

	//����POS����
	m_QPDPOSProc.GeoPOSProc_ExtractSBET(pathSBET, pathEventOut1, pathPOS1, 19);
	m_QPDPOSProc.GeoPOSProc_ExtractSBET(pathSBET, pathEventOut2, pathPOS2, 19);
	m_QPDPOSProc.GeoPOSProc_ExtractSBET(pathSBET, pathEventOut3, pathPOS3, 19);
	m_QPDPOSProc.GeoPOSProc_ExtractSBET(pathSBET, pathEventOut4, pathPOS4, 19);
	m_QPDPOSProc.GeoPOSProc_ExtractSBET(pathSBET, pathEventOut5, pathPOS5, 19);
	m_QPDPOSProc.GeoPOSProc_ExtractSBET(pathSBET, pathEventOut6, pathPOS6, 19);

	//����EO����
	THREEDPOINT setupVec;setupVec.dX= setupVec.dY=setupVec.dZ=0; float setupAngle[] = { 0,0,0 };
	m_QPDPOSProc.GeoPOSProc_ExtractEO(pathPOS1, 0, pathEO1, setupVec, setupAngle, 1);
	m_QPDPOSProc.GeoPOSProc_ExtractEO(pathPOS2, 0, pathEO2, setupVec, setupAngle, 1);
	m_QPDPOSProc.GeoPOSProc_ExtractEO(pathPOS3, 0, pathEO3, setupVec, setupAngle, 1);
	m_QPDPOSProc.GeoPOSProc_ExtractEO(pathPOS4, 0, pathEO4, setupVec, setupAngle, 1);
	m_QPDPOSProc.GeoPOSProc_ExtractEO(pathPOS5, 0, pathEO5, setupVec, setupAngle, 1);
	m_QPDPOSProc.GeoPOSProc_ExtractEO(pathPOS6, 0, pathEO6, setupVec, setupAngle, 1);

	char* pathLevel21 = "E:\\73\\test\\P2\\P2_0101_20160229_115734783.tif";
	char* pathLevel22 = "E:\\73\\test\\P2\\P2_0101_20160229_115734783.tif";
	char* pathLevel23 = "E:\\73\\test\\P2\\P2_0401_20160229_115734783.tif";
	char* pathLevel24 = "E:\\73\\test\\P2\\P2_0501_20160229_115734783.tif";
	char* pathLevel25 = "E:\\73\\test\\P2\\P2_0601_20160229_115734783.tif";
	char* pathLevel26 = "E:\\73\\test\\P2\\P2_0701_20160229_115734783.tif";
	QPDLevel2Process m_QDPLevel2Proc;
	float fGSDX = 2, fGSDY = 2;
	double fElevation = 0;
	int nEoOffset = 0;
	float fFov = 14, fIFov = 1,fFocalLen = 40;
	bool bDEM = 0,bIGM = 1;
	int nInterval = 1;
	//A�����ݲ�Ʒ����
	m_QDPLevel2Proc.Level2Proc_Product2A(pathBSQNonuniform1, pathLevel21, pathEO1, fGSDX, fGSDY, fElevation, nEoOffset, fFov, fIFov, fFocalLen, false, NULL,true);
	m_QDPLevel2Proc.Level2Proc_Product2A(pathBSQNonuniform2, pathLevel22, pathEO2, fGSDX, fGSDY, fElevation, nEoOffset, fFov, fIFov, fFocalLen, false, NULL, true);
	m_QDPLevel2Proc.Level2Proc_Product2A(pathBSQNonuniform3, pathLevel23, pathEO3, fGSDX, fGSDY, fElevation, nEoOffset, fFov, fIFov, fFocalLen, false, NULL, true);
	m_QDPLevel2Proc.Level2Proc_Product2A(pathBSQNonuniform4, pathLevel24, pathEO4, fGSDX, fGSDY, fElevation, nEoOffset, fFov, fIFov, fFocalLen, false, NULL, true);
	m_QDPLevel2Proc.Level2Proc_Product2A(pathBSQNonuniform5, pathLevel25, pathEO5, fGSDX, fGSDY, fElevation, nEoOffset, fFov, fIFov, fFocalLen, false, NULL, true);
	m_QDPLevel2Proc.Level2Proc_Product2A(pathBSQNonuniform6, pathLevel26, pathEO6, fGSDX, fGSDY, fElevation, nEoOffset, fFov, fIFov, fFocalLen, false, NULL, true);

	//B�����ݲ�Ʒ����
	char* pathDEM = "";
	m_QDPLevel2Proc.Level2Proc_Product2B(pathBSQNonuniform1, pathLevel21, pathEO1, fGSDX, fGSDY, fElevation, nEoOffset, fFov, fIFov, fFocalLen, pathDEM,false,  NULL, true);
	m_QDPLevel2Proc.Level2Proc_Product2B(pathBSQNonuniform2, pathLevel22, pathEO2, fGSDX, fGSDY, fElevation, nEoOffset, fFov, fIFov, fFocalLen, pathDEM, false, NULL, true);
	m_QDPLevel2Proc.Level2Proc_Product2B(pathBSQNonuniform3, pathLevel23, pathEO3, fGSDX, fGSDY, fElevation, nEoOffset, fFov, fIFov, fFocalLen, pathDEM, false, NULL, true);
	m_QDPLevel2Proc.Level2Proc_Product2B(pathBSQNonuniform4, pathLevel24, pathEO4, fGSDX, fGSDY, fElevation, nEoOffset, fFov, fIFov, fFocalLen, pathDEM, false, NULL, true);
	m_QDPLevel2Proc.Level2Proc_Product2B(pathBSQNonuniform5, pathLevel25, pathEO5, fGSDX, fGSDY, fElevation, nEoOffset, fFov, fIFov, fFocalLen, pathDEM, false, NULL, true);
	m_QDPLevel2Proc.Level2Proc_Product2B(pathBSQNonuniform6, pathLevel26, pathEO6, fGSDX, fGSDY, fElevation, nEoOffset, fFov, fIFov, fFocalLen, pathDEM, false, NULL, true);

}