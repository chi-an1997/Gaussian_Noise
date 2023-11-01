#include <cmath>
#include <limits>
#include <cstdlib>
#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

using namespace cv;
using namespace std;

double generateGaussianNoise(double m, double sigma);
Mat addGaussianNoise(Mat& srcImag);

int main()
{
	Mat srcImage = imread("flower.jpg");
	if (!srcImage.data)
	{
		cout << "讀入圖片錯誤！" << endl;
		system("pause");
		return -1;
	}
	imshow("原圖像", srcImage);
	Mat dstImage = addGaussianNoise(srcImage);
	imshow("添加高斯雜訊後的圖像", dstImage);
	waitKey();
	return 0;
}

//生成高斯雜訊
double generateGaussianNoise(double mu, double sigma)
{
	//定義小值
	const double epsilon = numeric_limits<double>::min();
	static double z0, z1;
	static bool flag = false;
	flag = !flag;
	//flag為假構造高斯隨機變數X
	if (!flag)
		return z1 * sigma + mu;
	double u1, u2;
	//構造隨機變數
	do
	{
		u1 = rand() * (1.0 / RAND_MAX);
		u2 = rand() * (1.0 / RAND_MAX);
	} while (u1 <= epsilon);
	//flag為真構造高斯隨機變數
	z0 = sqrt(-2.0 * log(u1)) * cos(2 * CV_PI * u2);
	z1 = sqrt(-2.0 * log(u1)) * sin(2 * CV_PI * u2);
	return z0 * sigma + mu;
}

//為圖像添加高斯雜訊
Mat addGaussianNoise(Mat& srcImag)
{
	Mat dstImage = srcImag.clone();
	int channels = dstImage.channels();
	int rowsNumber = dstImage.rows;
	int colsNumber = dstImage.cols * channels;
	//判斷圖像的連續性
	if (dstImage.isContinuous())
	{
		colsNumber *= rowsNumber;
		rowsNumber = 1;
	}
	for (int i = 1; i < rowsNumber; i++)
	{
		for (int j = 1; j < colsNumber; j++)
		{
			//添加高斯雜訊
			int val = dstImage.ptr<uchar>(i)[j] +
				generateGaussianNoise(2, 0.8)*32;
				//generateGaussianNoise(0, 0.1) * 255;
			if (val < 0)
				val = 0;
			if (val > 255)
				val = 255;
			dstImage.ptr<uchar>(i)[j] = (uchar)val;
		}
	}
	return dstImage;
}