#include"LidarReader.h"
#include"..\AuxiliaryFunction.h"

//��LAS�ļ���ȡLAS�ļ���ͷ�ļ��ͱ䳤�ֶ��ļ�
long LASLidarReader::LidarReader_Open(const char* pathLidar)
{
	if (m_lasFile != NULL)
		fclose(m_lasFile);
	errno_t err;
	err = fopen_s(&m_lasFile, pathLidar, "rb");
	if (err != 0)
		return -1;

	//��ȡͷ�ļ�
	m_lasHeader.LASHeader_Read(m_lasFile);
	if (m_lasHeader.number_of_variable_length_records == 0)
	{
		//û�б䳤�ֶ�
	}
	else
	{
		//��ȡ�䳤�ֶ�
		m_lasvarHeader = new LASVariableRecord[m_lasHeader.number_of_variable_length_records];
		for (int i = 0; i < m_lasHeader.number_of_variable_length_records; ++i)
		{
			m_lasvarHeader[i].LASVariableRecord_Read(m_lasFile);
		}
	}
	return 0;
	m_isDatasetOpen = true;

}

//��ȡLAS�ļ������Ĳ���Ϊ�Ƿ��������ݶ����ڴ�Ͷ�ȡ���ݲ������
long LASLidarReader::LidarReader_Read(bool inMemory, int readSkip/*=1*/)
{
	if (!m_isDatasetOpen)
	{
		printf("do not open file\n");
		exit(-1);
	}
	int totalLasNumber = 0;
	long long  memoryUsed = 0;
	totalLasNumber = m_lasHeader.number_of_point_records / readSkip;
	memoryUsed = totalLasNumber*m_lasHeader.point_data_record_length;
	if (inMemory&&memoryUsed > LargestMemoryToRead)
	{
		printf("too large memory to allocate\n");
		printf("advice to upper samples\n");
		return -1;
	}
	int *pointsRect			= NULL;
	unsigned char* readOnce = NULL;

	try
	{
		//���Ƚ��зָ�
		//ֱ�Ӹ��������ܶȽ��зֿ飬���ÿ��2048��������
		double width   = m_lasHeader.max_x - m_lasHeader.min_x;
		double height  = m_lasHeader.max_y - m_lasHeader.min_y;
		double scaleWH = width / height;
		//ÿ����Ĵ�С
		double widthPer		= width  / (sqrt(totalLasNumber / scaleWH)) + 0.5;;
		double heightPer    = height / (sqrt(totalLasNumber * scaleWH)) + 0.5;;
		int widthNum  =ceil( width / widthPer);
		int heightNum = ceil( height / heightPer);

		//������ͳ����Ϣ
		//�����ڴ�
		m_lasDataset.LASRect_AllocateMemory(widthNum*heightNum, inMemory);
		//����R���ṹ
		for (int i = 0; i < widthNum; ++i)
		{
			for (int j = 0; j < heightNum; ++j)
			{
				m_lasDataset.m_lasRectangles[j*widthNum + i].m_Rectangle.setRect(i*widthPer + m_lasHeader.min_x, j*heightPer + m_lasHeader.min_y,
					(i + 1)*widthPer + m_lasHeader.min_x, (j + 1)*heightPer + m_lasHeader.min_y);
			}
		}

		//�о����������� Ч���е�ͣ����Ǵ�������Ч�ʱȽϸ�
		m_lasDataset.LASRect_BuildTree();
		pointsRect = new int[widthNum*heightNum];
		memset(pointsRect, 0, sizeof(int)*widthNum*heightNum);
		int alread_read = 0;
		int read_once_max = 2048;
		int read_once = m_lasHeader.number_of_point_records;
		while (alread_read<m_lasHeader.number_of_point_records)
		{
			if (read_once>read_once_max)
				read_once = read_once_max;
			long		bytelen = read_once * m_lasHeader.point_data_record_length;
			readOnce = new unsigned char[bytelen];
			fread(readOnce, bytelen, 1, m_lasFile);
			//�ȶ�ȡ��Ȼ����д���
			for (size_t i = 0; i < read_once; i += readSkip)
			{
				LAS_XYZ vex;
				const unsigned char* data = readOnce + i*m_lasHeader.point_data_record_length;
				int size = sizeof(int);
				int x, y, z;
				memcpy(&x, data, size); memcpy(&y, data + size, size); memcpy(&z, data + size * 2, size);
				vex.x = x * m_lasHeader.x_scale_factor + m_lasHeader.x_offset;
				vex.y = y * m_lasHeader.y_scale_factor + m_lasHeader.y_offset;
				vex.z = z * m_lasHeader.z_scale_factor + m_lasHeader.z_offset;
				//fseek(m_lasFile, m_lasHeader.point_data_record_length - 12, SEEK_CUR);
				int widtnIdx = ceil((vex.x - m_lasHeader.min_x) / widthPer);
				int heighIdx = ceil((vex.y - m_lasHeader.min_y) / heightPer);
				pointsRect[heighIdx*widthNum+ widtnIdx]++;
			}
			alread_read += read_once;
			read_once_max = min(read_once_max, (m_lasHeader.number_of_point_records - alread_read));
		}
		fseek(m_lasFile, sizeof(m_lasHeader), SEEK_SET);

		//�ڶ��������ȡ����
		for (int j = 0; j < widthNum*heightNum; ++j)
		{
			m_lasDataset.m_lasRectangles[j].LASRect_AllocateMemory(pointsRect[j], inMemory, m_lasDataset.m_lasRectangles[j].m_Rectangle);
		}

		if (inMemory)
		{
			alread_read = 0;
			read_once_max = 2048;
			read_once = m_lasHeader.number_of_point_records;
			while (alread_read<m_lasHeader.number_of_point_records)
			{
				if (read_once>read_once_max)
					read_once = read_once_max;
				long		bytelen = read_once * m_lasHeader.point_data_record_length;
				fread(readOnce, bytelen, 1, m_lasFile);
				//�ȶ�ȡ��Ȼ����д���
				int totallasPnts = 0;
				for (size_t i = 0; i < read_once; i += readSkip)
				{
					LASPoint vex;
					const unsigned char* data = readOnce + i*m_lasHeader.point_data_record_length;
					int size = sizeof(int);
					int x, y, z;
					memcpy(&x, data, size); memcpy(&y, data + size, size); memcpy(&z, data + size * 2, size);
					vex.m_vec3d.x = x * m_lasHeader.x_scale_factor + m_lasHeader.x_offset;
					vex.m_vec3d.y = y * m_lasHeader.y_scale_factor + m_lasHeader.y_offset;
					vex.m_vec3d.z = z * m_lasHeader.z_scale_factor + m_lasHeader.z_offset;
					memcpy(&vex + 24, data,m_lasHeader.point_data_record_length - 12);
					
					int widtnIdx=ceil((vex.m_vec3d.x - m_lasHeader.min_x) / widthPer);
					int heighIdx = ceil((vex.m_vec3d.y - m_lasHeader.min_y) / heightPer);
					memcpy(m_lasDataset.m_lasRectangles[heighIdx*widthNum + widtnIdx].m_lasPoints + totallasPnts,&vex,sizeof(LASPoint));
					totallasPnts++;
					m_lasDataset.m_lasRectangles[heighIdx*widthNum + widtnIdx].m_point_index[totallasPnts] = alread_read + i;
				}
				alread_read += read_once;
				read_once_max = min(read_once_max, (m_lasHeader.number_of_point_records - alread_read));
			}
		}
		else
		{
			alread_read = 0;
			read_once_max = 2048;
			read_once = m_lasHeader.number_of_point_records;
			while (alread_read<m_lasHeader.number_of_point_records)
			{
				if (read_once>read_once_max)
					read_once = read_once_max;
				long		bytelen = read_once * m_lasHeader.point_data_record_length;
				fread(readOnce, bytelen, 1, m_lasFile);
				//�ȶ�ȡ��Ȼ����д���
				int totallasPnts = 0;
				for (size_t i = 0; i < read_once; i += readSkip)
				{
					LASPoint vex;
					const unsigned char* data = readOnce + i*m_lasHeader.point_data_record_length;
					int size = sizeof(int);
					int x, y, z;
					memcpy(&x, data, size); memcpy(&y, data + size, size); memcpy(&z, data + size * 2, size);
					vex.m_vec3d.x = x * m_lasHeader.x_scale_factor + m_lasHeader.x_offset;
					vex.m_vec3d.y = y * m_lasHeader.y_scale_factor + m_lasHeader.y_offset;
					vex.m_vec3d.z = z * m_lasHeader.z_scale_factor + m_lasHeader.z_offset;

					int widtnIdx = ceil((vex.m_vec3d.x - m_lasHeader.min_x) / widthPer);
					int heighIdx = ceil((vex.m_vec3d.y - m_lasHeader.min_y) / heightPer);
					totallasPnts++;
					m_lasDataset.m_lasRectangles[heighIdx*widthNum + widtnIdx].m_point_index[totallasPnts] = alread_read + i;
				}
				alread_read += read_once;
				read_once_max = min(read_once_max, (m_lasHeader.number_of_point_records - alread_read));
			}
		}
	}
	catch (bad_alloc &e)
	{
		printf("%s\n", e.what());
		exit(-1);
	}
	if (pointsRect != NULL)
		delete[]pointsRect;
	if (readOnce != NULL)
		delete[]readOnce;
	pointsRect = NULL;
	readOnce = NULL;

	return 0;
}