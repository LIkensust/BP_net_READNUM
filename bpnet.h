#pragma once
#include<vector>
#include<iostream>
#include<cstdlib>
using namespace std;
struct neuron
{
	vector<double>weight;
	vector<double>update_w;
	vector<double>ave_update_w;
	double output;
	double threshold;
	neuron()
		:output(0),
		update_w(0),
		ave_update_w(0)
	{}
};
typedef vector<neuron>layer;

struct read_type
{
	vector<double> weight;
	double threshold;
};
typedef vector<read_type> read_ly;

class bpNet
{
	int train_num;
	static double eta;
	vector<layer>network;
public:
	bpNet()
		:train_num(0)
	{};
	void set(const vector<int> & network_);
	~bpNet()
	{};
	static double sig(double &x) 
	{ 
		return 1.0 / (1.0 + exp(-x));
	}
	void putdata(vector<double> &input_, const vector<int> & size);
	void backchange(const vector<double> & predict, double &error);
	void upweight();
	void show() const;
	void reset_ave_update_w(const vector<double> & network_);
	void upweight_ave();
	void save_network(const string& filename);
	void load_file(const string& filename);
	void read_set(vector<int>& line_num, vector<read_ly>& read_table);
};
double bpNet::eta = 1;

void bpNet::save_network(const string& filename)
{
	FILE* fout = fopen(filename.c_str(),"wb");
	int ly_size[3] = { network[0].size(),network[1].size(),network[2].size() };
	fwrite(ly_size, sizeof(int), 3, fout);

	for (int i = 0; i < 3; i++)
	{
		if (i != 2)
		{
			for (int j = 0; j < ly_size[i]; j++)
			{
				int weight_num = network[i][j].weight.size();
				fwrite(&weight_num, sizeof(int), 1, fout);
				for (int k = 0; k < weight_num; k++)
				{
					double put_weight = network[i][j].weight[k];
					fwrite(&put_weight, sizeof(double), 1, fout);
				}
				double threshold = network[i][j].threshold;
				fwrite(&threshold, sizeof(double), 1, fout);
			}
		}
		if (i == 2)
		{
			for (int j = 0; j < 10; j++)
			{
				double threshold = network[i][j].threshold;
				fwrite(&threshold, sizeof(int), 1, fout);
			}
		}
	}
	cout << "save finished!" << endl;
	fclose(fout);
}


void bpNet::load_file(const string& filename)
{
	FILE* fin = fopen(filename.c_str(), "rb");
	int ly_num[3];
	vector<int> network_;
	network_.resize(3);
	fread(ly_num, sizeof(int), 3, fin);
	for (int i = 0; i < 3; i++)
	{
		network_[i]= ly_num[i];
	}
	vector<read_ly> ly_line;//line
	ly_line.resize(3);
	double read_wight=0;

	for (int i = 0; i < 3; i++)
	{
		ly_line[i].resize(ly_num[i]);
		if (i != 2)
		{
			for (int j = 0; j < ly_num[i]; j++)
			{
				int enu_num;
				fread(&enu_num, sizeof(int), 1, fin);
				ly_line[i][j].weight.resize(enu_num);
				for (int k = 0; k < enu_num; k++)
				{
					fread(&read_wight, sizeof(double), 1, fin);
					ly_line[i][j].weight[k] = read_wight;
				}
				double threshold;
				fread(&threshold, sizeof(double), 1, fin);
				ly_line[i][j].threshold = threshold;
			}
		}
		if (i == 2)
		{
			for (int j = 0; j < 10; j++)
			{
				double threshold;
				fread(&threshold, sizeof(double), 1, fin);
				ly_line[i][j].threshold = threshold;
			}
		}
	}
	read_set(network_, ly_line);
}

void bpNet::read_set(vector<int>& network_, vector<read_ly>& read_table)
{
	int layer_num = network_.size();
	for (int i = 0; i < layer_num; i++)
	{
		network.push_back(layer());
		for (int j = 0; j < network_[i]; j++)
		{
			network.back().push_back(neuron());
			if (i > 0)
			{
				network[i][j].threshold = read_table[i][j].threshold;
			}
			if (i < layer_num - 1)
			{
				for (int k = 0; k < network_[i + 1]; k++)
				{
					network[i][j].weight.push_back(read_table[i][j].weight[k]);
					network[i][j].update_w.push_back(0);
					network[i][j].ave_update_w.push_back(0);
				}
			}
		}
	}
}

void bpNet::set(const vector<int> & network_)
{
	int layer_num = network_.size();
	for (int i = 0; i < layer_num; i++)
	{
		network.push_back(layer());
		for (int j = 0; j < network_[i]; j++)
		{
			network.back().push_back(neuron());
			if (i > 0)
			{
				network[i][j].threshold = 0.5;
			}
			if (i < layer_num - 1)
			{
				for (int k = 0; k < network_[i + 1]; k++)
				{
					network[i][j].weight.push_back(rand()*(rand() % 2 ? 1 : -1)*1.0 / RAND_MAX);
					network[i][j].update_w.push_back(0);
					network[i][j].ave_update_w.push_back(0);
				}
			}
		}
	}
}

void bpNet::putdata(vector<double> &in, const vector<int> & size)
{
	for (int t = 0; t < size[0]; t++)
	{
		network[0][t].output = in[t];
	}
	for (int i = 1; i < size.size(); i++)
	{
		for (int j = 0; j < size[i]; j++)
		{
			network[i][j].output = 0;
			for (int k = 0; k < size[i - 1]; k++)
			{
				network[i][j].output += network[i - 1][k].output * network[i - 1][k].weight[j];
			}
			network[i][j].output += network[i][j].threshold;
			network[i][j].output = sig(network[i][j].output);
		}
	}
}

void bpNet::reset_ave_update_w(const vector<double> & network_)
{
	int layer_num = network_.size();
	for (int i = 0; i < layer_num; i++)
	{
		for (int j = 0; j < network_[i]; j++)
		{
			if (i < layer_num - 1)
			{
				for (int k = 0; k < network_[i + 1]; k++)
				{
					network[i][j].ave_update_w[k] = 0;
				}
			}
		}
	}
}

void bpNet::backchange(const vector<double> & predict, double & error)
{
	double delta_total = 0.0;
	double delta = 0.0;
	double sum;
	for (int i = 0; i < network[1].size(); i++)
	{
		for (int j = 0; j < network[2].size(); j++)
		{
			delta = -(predict[j] - network[2][j].output)*network[2][j].output*(1 - network[2][j].output)*network[1][i].output;//一阶偏导数
			network[1][i].update_w[j] = /*network[1][i].weight[j] */- eta * delta;
			if (train_num == 1 || train_num == 0)
			{
				network[1][i].ave_update_w[j] = 0;
			}
			network[1][i].ave_update_w[j] += network[1][i].update_w[j];
		}
	}
	delta = 0.0;
	for (int i = 0; i < network[0].size(); i++)
	{
		for (int j = 0; j < network[1].size(); j++)
		{
			sum = 0.0;
			delta = network[1][j].output*(1 - network[1][j].output)*network[0][i].output;
			for (int k = 0; k < network[2].size(); k++)
			{
				sum += -(predict[k] - network[2][k].output)*network[2][k].output*(1 - network[2][k].output)*network[1][j].weight[k];//神经网络第一层的偏导数
			}
			delta *= sum;
			network[0][i].update_w[j] = /*network[0][i].weight[j]*/ - eta * delta;
			if (train_num == 0||train_num==1)
			{
				network[0][i].ave_update_w[j] = 0;
			}
			network[0][i].ave_update_w[j] += network[0][i].update_w[j] /*+ network[0][i].ave_update_w[j]) / 2*/;
		}
	}
	train_num++;
}

void bpNet::upweight()
{
	for (int i = 0; i < network.size() - 1; i++)
	{
		for (int j = 0; j < network[i].size(); j++)
		{
			for(int k=0;k < network[i][j].weight.size();k++)
			network[i][j].weight[k] += network[i][j].update_w[k];
		}
	}
}

void bpNet::upweight_ave()
{
	for (int i = 0; i < network.size() - 1; i++)
	{
		for (int j = 0; j < network[i].size(); j++)
		{
			for (int k = 0;k < network[i][j].weight.size(); k++)
			network[i][j].weight[k] += network[i][j].ave_update_w[k]/train_num;
		}
	}
	train_num = 0;
}

void bpNet::show() const
{
	for (int i = 0; i < network[2].size(); i++)
	{
		std::cout << "output" << i + 1 << "=" << network[2][i].output << std::endl;
	}
}


