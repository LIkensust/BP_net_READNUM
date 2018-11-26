#pragma once
#include "bpnet.h"
#define SIZE 15
#include "stdafx.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include "cv.h"
#include <vector>
#include "bpnet.h"
#define TOUTLE 100
using namespace std;
using namespace cv;

class Train
{
public:
	void train();
	void read();
	void save();
	void get_train_data();
	void read_num();
	void load_train();

private:
	bpNet bpnet;
	vector<vector<double>> data_in; 
	vector<vector<double>> data_out;
};


void Train::get_train_data()
{
	char group;
	group = '2';
	string filename;
	for (int i = 0; i < 10; i++)
	{
		filename = "train_data\\";
		filename += i + '0';
		filename += ".";
		filename += group;
		filename += ".bmp";

		vector<double> table;
		table.resize(SIZE*SIZE);
		Mat image = imread(filename.c_str(), 0);
		Mat newima;
		resize(image, newima, Size(SIZE, SIZE));
		imwrite("tmp.bmp", newima);
		int count = 0;
		IplImage* src = cvLoadImage("tmp.bmp", 0);//导入图片
		int width = src->width;//图片宽度
		int height = src->height;//图片高度
		for (size_t row = 0; row<height; row++)
		{
			uchar* ptr = (uchar*)src->imageData + row * src->width;//获得灰度值数据指针
			for (size_t cols = 0; cols<width; cols++)
			{
				int intensity = ptr[cols];
				table[count] = intensity;
				count++;
			}
		}

		data_in.push_back(table);
		vector<double> out_data;
		out_data.resize(10);
		for (int j= 0; j < 10; j++)
		{
			out_data[j] = 0;
		}
		out_data[i] = 1;

		data_out.push_back(out_data);
	}
}

void Train::read_num() 
{
	string filename;
	filename = "read_num\\read.bmp";
	vector<double> table;
	table.resize(SIZE*SIZE);
	Mat image = imread(filename.c_str(), 0);
	Mat newima;
	resize(image, newima, Size(SIZE, SIZE));
	imwrite("tmp.bmp", newima);
	int count = 0;
	IplImage* src = cvLoadImage("tmp.bmp", 0);//导入图片
	int width = src->width;//图片宽度
	int height = src->height;//图片高度
	for (size_t row = 0; row<height; row++)
	{
		uchar* ptr = (uchar*)src->imageData + row * src->width;//获得灰度值数据指针
		for (size_t cols = 0; cols<width; cols++)
		{
			int intensity = ptr[cols];
			table[count] = intensity;
			count++;
		}
	}
	vector<int>ly{ SIZE*SIZE,50,10 };
	bpnet.putdata(table, ly);
	bpnet.show();
}

void Train::read()
{
	string filename;
	filename = "netdata.save";
	bpnet.load_file(filename);
}

void Train::save()
{
	string filename;
	filename = "netdata.save";
	bpnet.save_network(filename);
}

void Train::train()
{
	int input_num = data_in.size();
	int output_num = data_out.size();
	if (input_num != output_num)
	{
		cout << "参数错误" << endl;
		return;
	}
	double error = 1;
	vector<int>ly{ SIZE*SIZE,50,10 };
	bpnet.set(ly);
	int toutle = TOUTLE;
	for (int times = 0; times < toutle; times++)
	{
		for (int i = 0; i < input_num; i++)
		{
			bpnet.putdata(data_in[i], ly);
			bpnet.backchange(data_out[i], error);
			bpnet.upweight_ave();
				//upweight();
		}
		//bpnet.upweight_ave();
		cout << (times + 1)*100/toutle<< "%" << endl;
		if( ((times + 1) * 100 / toutle)%5==0 )
		{
			for (int i = 0; i < input_num; i++)
			{
			cout << "======================================" << endl;
			cout << "for" << i << endl;
			bpnet.putdata(data_in[i], ly);
			bpnet.show();
			cout << endl;
			}
			this->save();
		}
	}

	for (int i = 0; i < input_num; i++)
	{
		cout << "======================================" << endl;
		cout << "for" << i << endl;
		bpnet.putdata(data_in[i], ly);
		bpnet.show();
		cout << endl;
	}

	this->save();
}

void Train::load_train()
{
	int input_num = data_in.size();
	int output_num = data_out.size();
	if (input_num != output_num)
	{
		cout << "参数错误" << endl;
		return;
	}
	double error = 1;
	vector<int>ly{ SIZE*SIZE,50,10 };
	//bpnet.set(ly);
	this->read();

	int toutle = TOUTLE;
	for (int times = 0; times < toutle; times++)
	{
		for (int i = 0; i < input_num; i++)
		{
			bpnet.putdata(data_in[i], ly);
			bpnet.backchange(data_out[i], error);
			bpnet.upweight();
		}
		//bpnet.upweight_ave();
		cout << (times + 1) * 100 / toutle << "%" << endl;
		if (((times + 1) * 100 / toutle) % 5 == 0)
		{
			for (int i = 0; i < input_num; i++)
			{
				cout << "======================================" << endl;
				cout << "for" << i << endl;
				bpnet.putdata(data_in[i], ly);
				bpnet.show();
				cout << endl;
			}
			this->save();
		}
	}

	for (int i = 0; i < input_num; i++)
	{
		cout << "======================================" << endl;
		cout << "for" << i << endl;
		bpnet.putdata(data_in[i], ly);
		bpnet.show();
		cout << endl;
	}

	this->save();
}