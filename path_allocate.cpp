#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;

class Point // Point class用來儲存各點資料
{
public:
	int x_coordinate; // x值
	int y_coordinate; // y值
	int type; // 表示該點的性質
	Point();
	Point(int x, int y, int t);
	double operator*(Point input);
	bool operator=(Point input);
};

Point::Point()
{
	x_coordinate = 0;
	y_coordinate = 0;
	type = 0;
}
Point::Point(int x, int y, int t)
{
	x_coordinate = x;
	y_coordinate = y;
	type = t;
}
double Point::operator*(Point input) // 用來計算距離
{
	double temp = (x_coordinate - input.x_coordinate) * (x_coordinate - input.x_coordinate) + 
	(y_coordinate - input.y_coordinate) * (y_coordinate - input.y_coordinate);
	//cout << x_coordinate << " " << input.x_coordinate << "\n" << y_coordinate << " " << input.y_coordinate;
	//return ((int)(sqrt(temp) * 1000 + .5)) / 1000.;
	return sqrt(temp);
}
bool Point::operator=(Point input) // 用來assign
{
	x_coordinate = input.x_coordinate;
	y_coordinate = input.y_coordinate;
	type = input.type;
	return true;
}

class Set // 用來儲存路徑上的點的順序
{
private:
	int n;
public:
	int numOfSet; // set裡面的數量
	int* setData; // 資料儲存於此
	Set(int n);
	void add(int node); // 在set裡新增點
	void insert(int front, int data); // 在set中insert點
	void print(); // 印出set
};

Set::Set(int num)
{
	n = num;
	numOfSet = 0;
	setData = new int[n];
	for (int i = 0; i < n; i++)
	{
		setData[i] = -1;
	}
}
void Set::add(int node)
{
	if(numOfSet < n)
	{
		setData[numOfSet] = node;
		numOfSet++;
	}
}
void Set::insert(int front, int data)
{
	int index = -1;
	for(int i = 0; i < numOfSet; i++)
	{
		if (setData[i] == front)
		{
			index = i;
		}
	}
	if (index != -1 && numOfSet < n)
	{
		for (int i = numOfSet; i > index; i--)
		{
			setData[i] = setData[i - 1];
		}
		setData[index + 1] = data;
		numOfSet++;
	}
}
void Set::print()
{
	for (int i = 0; i < numOfSet; i++)
	{
		cout << setData[i] << " ";
	}
	cout << "\n";
}

double placeCount(int left, int right, int mid, Point data[]) // 計算插入的點與鄰近的點的距離
{
	double sum = 0;
	sum = sum + data[left] * data [mid] + data [mid] * data [right];
	return sum;
}

double lengthCount(Set pass1, Set pass2, Point data[]) // 計算全部的路徑長度
{
	double sum = 0;
	for (int i = 1; i < pass1.numOfSet; i++)
	{
		sum += data[pass1.setData[i]] * data[pass1.setData[i - 1]];
	}
	for (int i = 1; i < pass2.numOfSet; i++)
	{
		sum += data[pass2.setData[i]] * data[pass2.setData[i - 1]];
	}
	return sum;
}

void findShortestPath (Point data[], int n, int b1, int b2, int output[]) // 執行找到最短路徑的演算法
{
	bool *isAssign = new bool[n]; // 用來辨別哪些點已經被分配到路徑上
	for (int i = 0; i < n; i++)
	{
		isAssign[i] = false; // 先全部初始化為false
	}
	Set pass1(n); // Set pass1用來儲存去程或回程其中一條路徑
	Set pass2(n); // Set pass2用來儲存去程或回程其中一條路徑
	pass1.add(0); // 先加入起始點
	pass1.add(n - 1); // 在加入終點
	pass2.add(0); // 先加入起始點
	pass2.add(n - 1); // 在加入終點
	isAssign[0] = true; // 接著把起點和終點的點設為以分配
	isAssign[n - 1] = true;
	pass1.insert(0,b1); // 將加油站插入其中一條路徑
	pass2.insert(0,b2); // 將另一個加油站插入另一條路徑
	isAssign[b1] = true; // 將加油站設為已分配路
	isAssign[b2] = true;

	for (int i = 0; i < n; i++) // 接下來依序分配還沒分配過的點到路徑中
	{
		if (isAssign[i] == false) // 如果是還沒分配過的點
		{
			int indexNum = -1;
			int passNum = -1;
			double misDis = 100000000;
			for (int j = 1; j < pass1.numOfSet; j++) // 則先搜尋pass1中可插入點的位置
			{
				double temp = placeCount(pass1.setData[j - 1], pass1.setData[j], i, data) - data[pass1.setData[j - 1]] * data[pass1.setData[j]];
				// 找插入點後使路徑長度改變最小的地方插入
				if ((data[pass1.setData[j - 1]].x_coordinate >= data[i].x_coordinate && data[i].x_coordinate >= data[pass1.setData[j]].x_coordinate)
					|| (data[pass1.setData[j - 1]].x_coordinate <= data[i].x_coordinate && data[i].x_coordinate <= data[pass1.setData[j]].x_coordinate))
				{
					if (temp < misDis)
					{
						misDis = temp;
						indexNum = pass1.setData[j - 1];
						passNum = 1;
					}
				}
			}
			for (int j = 1; j < pass2.numOfSet; j++)
			{
				double temp = placeCount(pass2.setData[j - 1], pass2.setData[j], i, data) - data[pass2.setData[j - 1]] * data[pass2.setData[j]];
				// 找插入點後使路徑長度改變最小的地方插入
				if ((data[pass2.setData[j - 1]].x_coordinate >= data[i].x_coordinate && data[i].x_coordinate >= data[pass2.setData[j]].x_coordinate)
					|| (data[pass2.setData[j - 1]].x_coordinate <= data[i].x_coordinate && data[i].x_coordinate <= data[pass2.setData[j]].x_coordinate))
				{
					if (temp < misDis)
					{
						misDis = temp;
						indexNum = pass2.setData[j - 1];
						passNum = 2;
					}
				}
			}
			if (passNum == 1) // 選擇將點插入pass1還是pass2
			{
				pass1.insert(indexNum, i);
			}
			else if(passNum == 2)
			{
				pass2.insert(indexNum, i);
			}
		}
	}

	int findOne = -1;
	for (int i = 0; i < pass1.numOfSet; i++) //找哪一條路徑是起始路徑
	{
		if (pass1.setData[i] == 1)
		{
			findOne = 1;
		}
	}
	if (findOne == -1)
	{
		for (int i = 0; i < pass2.numOfSet; i++)
		{
			if (pass2.setData[i] == 1)
			{
				findOne = 2;
			}
		}
	}
	if (findOne == 1) // 輸出啟始路徑到陣列output中
	{
		int pointer = 0;
		output[pointer] = 0;
		pointer++;
		for (int i = 1; i < pass1.numOfSet - 1; i++)
		{
			output[pointer] = pass1.setData[i];
			pointer++;
		}
		output[pointer] = n - 1;
		pointer++;
		for (int i = pass2.numOfSet - 2; i > 0; i--)
		{
			output[pointer] = pass2.setData[i];
			pointer++;
		}
		output[pointer] = 0;
	}
	else if (findOne == 2)
	{
		int pointer = 0;
		output[pointer] = 0;
		pointer++;
		for (int i = 1; i < pass2.numOfSet - 1; i++)
		{
			output[pointer] = pass2.setData[i];
			pointer++;
		}
		output[pointer] = n - 1;
		pointer++;
		for (int i = pass1.numOfSet - 2; i > 0; i--)
		{
			output[pointer] = pass1.setData[i];
			pointer++;
		}
		output[pointer] = 0;
	}
	cout  << fixed  <<  setprecision(2) << ((int)(lengthCount(pass1, pass2, data) * 100 + .5)) / 100. << ", "; // 印出總路徑長
}

int main(int argc, char const *argv[])
{
	char str[255];
	char b[5] = ".txt";
    strcat(str,argv[1]);
    strcat(str,b);
	freopen(str, "rt", stdin); // 讀入字串並開啟相應檔案
	int n = 0, b1 = 0, b2 = 0; // 儲存輸入數值
	cin >> n >> b1 >> b2;
	Point* data = new Point[n]; // 用來儲存所有點資料的陣列
	for (int i = 0; i < n; i++)
	{
		cin >> data[i].x_coordinate >> data[i].y_coordinate; // 讀入個點的資料
	}
	data[0].type = 2; // 設定個點的型態
	data[b1].type = 1;
	data[b2].type = 1;
	data[n - 1].type = 2;

	int *output = new int[n + 1]; // output陣列用來儲存輸出的路徑
	for (int i = 0; i < n + 1; i++)
	{
		output[i] = -1; // 初始化
	}
	findShortestPath(data, n, b1, b2, output); // 執行找最短的路徑
	for (int i = 0; i < n + 1; i++)
	{
		if (i + 1 == n + 1)
		{
			cout << output[i];
		}
		else
		{
			cout << output[i] << " "; // 印出路徑
		}
	}
	return 0;
}