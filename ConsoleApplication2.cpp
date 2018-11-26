// ConsoleApplication1.cpp: 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include"highgui.h"
#include"cv.h"
#include <vector>
#include "bpnet.h"
#include "training.h"
#define SIZE 15
using namespace std;
using namespace cv;

void test()
{
	Train train;
	train.get_train_data();
	train.train();
	train.save();
}

int main()
{
	test();
	system("pause");
	return 0;
}

//int main(int argc, char** argv)
//{
//	vector<double> table;
//	table.resize(SIZE*SIZE);
//	Mat image = imread("1.bmp",0);
//	Mat newima;
//	resize(image, newima, Size(SIZE,SIZE));
//	imwrite("1tmp.bmp", newima);
//		//imshow("show", newima);
//		//waitKey(0);
//	int count = 0;
//	IplImage* src = cvLoadImage("1tmp.bmp", 0);//导入图片
//	int width = src->width;//图片宽度
//	int height = src->height;//图片高度
//	for (size_t row = 0; row<height; row++)
//	{
//		uchar* ptr = (uchar*)src->imageData + row * src->width;//获得灰度值数据指针
//		for (size_t cols = 0; cols<width; cols++)
//		{
//			int intensity = ptr[cols];
//			table[count] = intensity;
//			count ++;
//		}
//	}
//
//	vector<double> test1;
//	
//	test1.resize(SIZE*SIZE);
//	Mat image1 = imread("test1.bmp", 0);
//	Mat newima1;
//	resize(image1, newima1, Size(SIZE, SIZE));
//	imwrite("test1tmp.bmp", newima1);
//	//imshow("show", newima);
//	//waitKey(0);
//	IplImage* src1 = cvLoadImage("test1tmp.bmp", 0);//导入图片
//	int width1 = src1->width;//图片宽度
//	int height1 = src1->height;//图片高度
//	count = 0;
//	for (size_t row = 0; row<height; row++)
//	{
//		uchar* ptr = (uchar*)src1->imageData + row * src1->width;//获得灰度值数据指针
//		for (size_t cols = 0; cols<width; cols++)
//		{
//			int intensity = ptr[cols];
//			test1[count] = intensity;
//			count ++;
//		}
//	}
//
//	BPNet bpnetwork;
//	vector<int>ly{ SIZE*SIZE,50,10 };
//	vector<double>input = table;
//	vector<double>output{1,0,0,0,0,0,0,0,0,0 };
//	bpnetwork.set(ly);
//	double error;
//	for (int i = 0; i < 100; i++)
//	{
//		bpnetwork.putdata(input, ly);
//		bpnetwork.backchange(output,error);
//		bpnetwork.upweight();
//		bpnetwork.show();
//		cout << "trying" << i << '%' << endl;
//	}
//	bpnetwork.show();
//
//	cout << endl;
//	bpnetwork.putdata(input, ly);
//	bpnetwork.show();
//	cout << endl;
//	bpnetwork.putdata(test1, ly);
//	bpnetwork.show();
//	cout << endl;
//
//
//	system("pause");
//	return 0;
//}

//int main()
//{
//	Mat image = imread("C:\\Users\\Administrator\\source\\repos\\ConsoleApplication1\\ConsoleApplication1\\1.png");
//	//cout<<image.channels()<<endl;
//	imshow("show", image);
//	Mat newima;
//	resize(image, newima, Size(10, 10));
//
//	imshow("show", newima);
//	waitKey(0);
//	system("pause");
//	return 0;
//}
