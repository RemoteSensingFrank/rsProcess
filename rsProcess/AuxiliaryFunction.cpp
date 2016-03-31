#include"stdafx.h"
#include"AuxiliaryFunction.h"

void WriteENVIHeader(const char* pathHeader, ENVIHeader mImgHeader)
{
	FILE* fpHead = NULL;
	errno_t err = fopen_s(&fpHead, pathHeader, "w");
	if (err != 0)
		exit(-1);

	if (!fpHead)
		exit(-1);

	fprintf(fpHead, "%s\n%s\n%s\n", "ENVI", "description = {", "File Imported into ENVI.}");
	fprintf(fpHead, "samples = %d\n", mImgHeader.imgWidth);					//д������Ԫ��
	fprintf(fpHead, "lines = %d\n", mImgHeader.imgHeight);				    //д������
	fprintf(fpHead, "bands = %d\n", mImgHeader.imgBands);					//д�벨����
	fprintf(fpHead, "file type = %s\n", "ENVI Standard");				    //д���ʾ
	fprintf(fpHead, "data type = %d\n", mImgHeader.datatype);
	fprintf(fpHead, "interleave = %s\n", mImgHeader.interleave.c_str());
	fprintf(fpHead, "sensor type = %s\n", "Unknown");
	fprintf(fpHead, "byte order = %d\n", mImgHeader.byteorder);
	fprintf(fpHead, "wavelength units = %s\n", "Unknown");

	//ͶӰ������ϵͳ
	if (mImgHeader.bimgGeoProjection)
	{
		fprintf(fpHead, "map info = { %s,", mImgHeader.mapInfo.projection.c_str());
		fprintf(fpHead, "%lf,", mImgHeader.mapInfo.adfGeotransform[1]);
		fprintf(fpHead, "%lf,", mImgHeader.mapInfo.adfGeotransform[5]);
		fprintf(fpHead, "%lf,", mImgHeader.mapInfo.adfGeotransform[0]);
		fprintf(fpHead, "%lf,", mImgHeader.mapInfo.adfGeotransform[3]);
		fprintf(fpHead, "%lf,", mImgHeader.mapInfo.adfGeotransform[2]);
		fprintf(fpHead, "%lf,", mImgHeader.mapInfo.adfGeotransform[4]);
		fprintf(fpHead, "%d,", mImgHeader.mapInfo.zone);
		fprintf(fpHead, "%s\n", mImgHeader.mapInfo.directions.c_str());
		fprintf(fpHead, "%s\n", mImgHeader.coordianteSys.c_str());
	}

	//����
	if (mImgHeader.bimgWaveLength)
	{
		fprintf(fpHead, "wavelength = {");
		for (int i = 0; i < mImgHeader.imgWaveLength.size(); ++i)
		{
			for (int j = 0; j + i < mImgHeader.imgWaveLength.size(); ++j)
				fprintf(fpHead, "%lf", mImgHeader.imgWaveLength[j + i]);
			fprintf(fpHead, "\n");
		}
		fprintf(fpHead, "}\n");
	}

	//�벨��
	if (mImgHeader.bimgFWHM)
	{
		fprintf(fpHead, "fwhm = {");
		for (int i = 0; i < mImgHeader.imgFWHM.size(); ++i)
		{
			for (int j = 0; j + i < mImgHeader.imgFWHM.size(); ++j)
				fprintf(fpHead, "%lf", mImgHeader.imgFWHM[j + i]);
			fprintf(fpHead, "\n");
		}
		fprintf(fpHead, "}\n");
	}

	//������
	if (mImgHeader.bimgBandNames)
	{
		fprintf(fpHead, "band names = {");
		for (int i = 0; i < mImgHeader.imgBandNnames.size(); ++i)
		{
			for (int j = 0; j + i < mImgHeader.imgBandNnames.size(); ++j)
				fprintf(fpHead, "%s", mImgHeader.imgBandNnames[j + i].c_str());
			fprintf(fpHead, "\n");
		}
		fprintf(fpHead, "}\n");
	}

	fclose(fpHead);		//�ر��ļ�
}