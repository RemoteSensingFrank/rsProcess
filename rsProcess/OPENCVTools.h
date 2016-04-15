#pragma once

#include<vector>
#include<string>
#include<Windows.h>
using namespace std;

#include"ThreadPool.h"

////////////////////////////////////////////////////////////////////////////////////////////////

//OPENCV���ߺ���
#include"opencv\opencv2\stitching\detail\camera.hpp"
#include"opencv\opencv2\opencv.hpp"
#pragma comment(lib,"opencv_world310d.lib")
using namespace cv;

//Ӱ��ƥ���̲߳���
struct ImgFeaturesThreadSt
{
	string img1;
	string img2;
	vector<Point2f> pts1;
	vector<Point2f> pts2;
	string descriptorMethod;
	string matchMethod;
};
//�����������̺߳���
DWORD WINAPI ImgFeaturesTools_ExtractMatchThread(LPVOID lpParameters);

//Ӱ����������ȡ��ƥ��Ĺ�����
/*
descriptor method
	("AKAZE-DESCRIPTOR_KAZE_UPRIGHT");    // see http://docs.opencv.org/trunk/d8/d30/classcv_1_1AKAZE.html
	("AKAZE");							  // see http://docs.opencv.org/trunk/d8/d30/classcv_1_1AKAZE.html
	("ORB");							  // see http://docs.opencv.org/trunk/de/dbf/classcv_1_1BRISK.html
	("BRISK");							  // see http://docs.opencv.org/trunk/db/d95/classcv_1_1ORB.html
match algorithm
	see http://docs.opencv.org/trunk/db/d39/classcv_1_1DescriptorMatcher.html#ab5dc5036569ecc8d47565007fa518257
	("BruteForce");
	("BruteForce-L1");
	("BruteForce-Hamming");
	("BruteForce-Hamming(2)");
*/
class ImgFeaturesTools
{
public:
	//1.����ȡ��ƥ�������㱣��ΪENVI��ʽ
	long ImgFeaturesTools_SaveENVIMatches(const char* pathDst, const char* pathSrc1, const char* pathSrc2, vector<Point2f> pts1, vector<Point2f> pts2);

	//2.��ȡENVI��ʽ��ƥ���
	long ImgFeaturesTools_ReadENVIMatches(const char* pathPoints, vector<Point2f> &pts1, vector<Point2f> &pts2);

	//3.����SURFƥ��������<���ڵ���ͼ��>
	long ImgFeaturesTools_ExtractMatch(const char* pathImage1, vector<Point2f> &pts1, const char* pathImage2, vector<Point2f> &pts2, string descriptorMethod, string matchMethod);
	static long ImgFeaturesTools_ExtractMatchThread(ImgFeaturesThreadSt &featureExtractThreadParam);

	//4.����SURFƥ������<��������ͼ��>
	long ImgFeaturesTools_ExtracMatches(vector<string> pathList, vector<vector<Point2f>> &pts, bool* ismatchpair, string descriptorMethod, string matchMethod);
private:
	//5.��ȡSURFƥ�����������������
	long ImgFeatruesTools_ExtractFeatures(const char* pathImage, Mat& descriptor, vector<Point2f> &keypoints,string descriptorMethod);

	//6.ƥ���������Ż�
	long ImgFeaturesTools_MatchOptimize(vector<Point2f> &pts1, vector<Point2f> &pts2);
};

//��Ӱ�����Ĺ�����
class PhotogrammetryTools
{
public:
	//1.�ռ�󷽽��ᣬͨ������������Ӱ����������Ӱ����ⷽλԪ��
	long PhotogrammetryTools_Resection(vector<Point3f> ptsCoordinate, vector<Point2f> ptsImg, double* cameraMat, double* rotationMat, double* translateMat, bool isCameraGuess = true);

	//2.��ȡ��������x1Fx2=0
	long PhotogrammetryTools_FundamentalMat(vector<Point2f> ptsImg1, vector<Point2f> ptsImg2, Mat &fundmentalMat);

	//3.��ȡ��������
	long PhotogrammetryTools_EssitialMat(vector<Point2f> ptsImg1, vector<Point2f> ptsImg2, Mat &essitialMat,double fLen,Point2d principalPnt);

	//4.��ȡ��Զ���Ԫ�ص���ת��ƽ�ƾ���
	long PhotogrammetryTools_ExtractRT(Mat essitialMat, vector<Point2f> ptsImg1, vector<Point2f> ptsImg2, Mat &rotMatrix, Mat &translateMatrix, double fLen, Point2d principalPnt);

	//5.��ȡ��Ӧ����
	long PhotogrammetryTools_Homography(vector<Point2f> ptsImg1, vector<Point2f> ptsImg2, Mat &homographyMat);
};


//Ӱ��ָ����
//��ˮ���㷨Ԥ���幤��
class ImageSegmentTools
{
public:
	//1.��ˮ���㷨����ˮ���㷨��Ҫѡȡ��ǣ������Ҫ��ȡ�����
	long ImgSegmentTools_WaterShed(const char* path,const char* pathOutput);

	//2.ʹ��canny���ӽ��зָ�
	long ImgSegmentTools_Canny(const char* path, const char* pathOutput);

public:

};

//opencv���������߹���
const int WW_MAX_MARK_COUNT = 40; //���40�����Ƶ�
static int mark_count = 4;
static int conner_pt_index = -1;
static CvPoint3D32f Control_pts[WW_MAX_MARK_COUNT];
static IplImage *image = NULL; //ԭʼͼ��
static bool is_showControlLines = true;

class BezierCurve
{
private:

	// ����������ӣ�p=p+q
	static CvPoint3D32f BezierCurve_PointAdd(CvPoint3D32f p, CvPoint3D32f q) {
		p.x += q.x;		p.y += q.y;		p.z += q.z;
		return p;
	}

	// �����ͱ������p=c*p
	static CvPoint3D32f BezierCurve_PointTimes(float c, CvPoint3D32f p) {
		p.x *= c;	p.y *= c;	p.z *= c;
		return p;
	}

	// ���㱴�������̵�ֵ
	// ����u�ķ�Χ��0-1֮��
	//P1*t^3 + P2*3*t^2*(1-t) + P3*3*t*(1-t)^2 + P4*(1-t)^3 = Pnew 
	static CvPoint3D32f BezierCurve_Bernstein(float u, CvPoint3D32f *p) {
		CvPoint3D32f	a, b, c, d, r;

		a = BezierCurve_PointTimes(pow(u, 3), p[0]);
		b = BezierCurve_PointTimes(3 * pow(u, 2)*(1 - u), p[1]);
		c = BezierCurve_PointTimes(3 * u*pow((1 - u), 2), p[2]);
		d = BezierCurve_PointTimes(pow((1 - u), 3), p[3]);

		r = BezierCurve_PointAdd(BezierCurve_PointAdd(a, b), BezierCurve_PointAdd(c, d));

		return r;
	}

	//��������
	static void BezierCurve_DrawControlLine(CvPoint3D32f *p) {

		CvPoint pc[4];
		for (int i = 0; i<4; i++)
		{
			pc[i].x = (int)p[i].x;
			pc[i].y = (int)p[i].y;
		}

		cvLine(image, pc[0], pc[1], CV_RGB(0, 0, 255), 1, CV_AA, 0);
		cvLine(image, pc[2], pc[3], CV_RGB(0, 0, 255), 1, CV_AA, 0);
	}

	//�õ����Control_pts��index
	static int BezierCurve_GetNearPointIndex(CvPoint mouse_pt)
	{
		CvPoint pt;
		for (int i = 0; i<mark_count; i++)
		{
			pt.x = mouse_pt.x - (int)Control_pts[i].x;
			pt.y = mouse_pt.y - (int)Control_pts[i].y;
			float distance = sqrt((float)(pt.x*pt.x + pt.y*pt.y));
			if (distance<10) return i;

		}
		return -1;
	}

	static void BezierCurve_On_mouse(int event, int x, int y, int flags, void *param)
	{
		if (event == CV_EVENT_LBUTTONDOWN)
		{
			CvPoint pt = cvPoint(x, y);
			//cout<<x<<","<<y<<endl;

			if (conner_pt_index >-1)
				conner_pt_index = -1;
			else
			{
				conner_pt_index = BezierCurve_GetNearPointIndex(pt);
				//����µĿ��Ƶ�
				if (conner_pt_index == -1)
				{
					if (mark_count <= (WW_MAX_MARK_COUNT - 1))
					{
						Control_pts[mark_count].x = (float)pt.x;
						Control_pts[mark_count].y = (float)pt.y;
						Control_pts[mark_count].z = 0;
						mark_count++;
					}
				}
			}
		}
		else if (event == CV_EVENT_MOUSEMOVE) //�޸Ŀ��Ƶ�����
		{
			if (conner_pt_index >-1)
			{
				Control_pts[conner_pt_index].x = (float)x;
				Control_pts[conner_pt_index].y = (float)y;
			}
		}

	};

public:
	//���Ʊ���������
	void BezierCurve_BezierDraw();
};

//opencv Ӱ���޸�����
static Mat ImageInpaint_img, ImageInpaint_inpaintMask;
static Point ImageInpaint_prevPt(-1, -1);
class ImageInpaint
{
private:
	static void ImageInpaint_onMouse(int event, int x, int y, int flags, void*)
	{
		if (event == CV_EVENT_LBUTTONUP || !(flags & CV_EVENT_FLAG_LBUTTON))
			ImageInpaint_prevPt = Point(-1, -1);
		else if (event == CV_EVENT_LBUTTONDOWN)
			ImageInpaint_prevPt = Point(x, y);
		else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))
		{
			Point pt(x, y);
			if (ImageInpaint_prevPt.x < 0)
				ImageInpaint_prevPt = pt;
			line(ImageInpaint_inpaintMask, ImageInpaint_prevPt, pt, Scalar::all(255), 5, 8, 0);
			line(ImageInpaint_img, ImageInpaint_prevPt, pt, Scalar::all(255), 5, 8, 0);
			ImageInpaint_prevPt = pt;
			imshow("image", ImageInpaint_img);
		}
	}

public:
	void ImageInpaint_Inpaint(const char* pathImg,const char* pathDst);
};