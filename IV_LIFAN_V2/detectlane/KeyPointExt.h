#ifndef KEYPOINTEXT_H
#define KEYPOINTEXT_H

#pragma once
#include<iostream>
#include<vector>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include"curvefitting.h"

using namespace std;
using namespace cv; 

void rightPointsExt(Mat &BW,vector<Point2i> &scanPoints,int boundary,vector<Point2f> &last_solid_points,vector<Point2f> &last_dash_points,int frame_count,bool isDoubleLane)   //,bool &thresholdFlag
{

    //int candi_num=0;			//候选像素点的个数
    //int blank_num=0;			//候选点后面背景像素点的个数
	vector<int> rowcandi;		//存放每一行的初始候选点
    unsigned int width_min,width_max;	//每一行扫描宽度限制
    unsigned int dep_left,dep_right;		//根据上一帧是否包含双车道确定点滤除的范围
	if(isDoubleLane)			//如果一侧存在两条车道线，则滤除范围较小
	{
		dep_left=-10;
		dep_right=10;
	}
	else						//如果不存在双线，则滤除范围较大，因为如果范围一直较小会找不到双线
	{
		dep_left=-60;
		dep_right=10;
	}
	for(int i=0;i<BW.rows;i++)
	{
		if(i<80)				//远视场的宽度限制
		{
			width_min=2;
			width_max=15;
		}
		else					//近视场的宽度限制
		{
			width_min=10;
			width_max=30;
		}
		uchar* data=BW.ptr<uchar>(i);
		for(int j=boundary;j<BW.cols;j++)			//提取出白色像素点
		{
			if(data[j]==255)
			{
				rowcandi.push_back(j);
			}
		}
        unsigned int startIndex=0,stopIndex=0;				//每一段的起始索引
		size_t size=rowcandi.size();
		//if( i == 1,size > 50)                       //判断阈值分割是否合适，true则表示需要切换阈值模式
		//{
		//	thresholdFlag = true;
		//}
		if(size>3)									//白色像素点间隔大于阈值才进行分段，间隔过小可能由于车道缺失，灰尘等造成
		{
			for(size_t k=0;k<size-1;k++)
			{
				if(rowcandi[k+1]-rowcandi[k]>3) 
				{
					stopIndex=k;
					if(stopIndex-startIndex<=width_max&&stopIndex-startIndex>=width_min)			//宽度符合条件，则根据是否跟上一帧偏差过大确定是否保留
					{
						if(frame_count==1)
						{
							scanPoints.push_back(Point2f(i,rowcandi[startIndex]));			//第一帧由于没有上一帧信息，直接保留
							stopIndex=0;
							break;
						}
						else																//后续帧若与上一帧偏差过大的点则不保留，否则保留
						{
							if(last_solid_points.size()>0 && last_dash_points.size()>0)
							{
								if(((rowcandi[startIndex]-last_solid_points[i].x)<dep_right)&&((rowcandi[startIndex]-last_dash_points[i].x)>dep_left))
								{
									scanPoints.push_back(Point2f(i,rowcandi[startIndex]));
									stopIndex=0;
									break;
								}
							}
						}
					}
					startIndex=k+1;
				}
				else
				{
					stopIndex=k+1;
				}
			}
			if(stopIndex==rowcandi.size()-1)			//最后一段采用与上面相同的方法进行处理
			{
				if(stopIndex-startIndex<=width_max&&stopIndex-startIndex>=width_min)
				{
					if(frame_count==1)
					{
						scanPoints.push_back(Point2f(i,rowcandi[startIndex]));
					}
					else
					{
						if(last_solid_points.size()>0 && last_dash_points.size()>0)
						{
							if(((rowcandi[startIndex]-last_solid_points[i].x)<dep_right)&&((rowcandi[startIndex]-last_dash_points[i].x)>dep_left))
							{
								scanPoints.push_back(Point2f(i,rowcandi[startIndex]));
							}
						}
					}
				}
			}
		}

		rowcandi.clear();
	}
}

void leftPointsExt(Mat &BW,vector<Point2i> &leftscanPoints,int boundary,vector<Point2f> &last_left_points,vector<Point2f> &last_dash_points,int frame_count,bool isDoubleLane) //bool &left_thresholdFlag
{
	vector<int> rowcandi;		//存放每一行的初始候选点
	int width_min,width_max;	//每一行扫描宽度限制
	int dep_left,dep_right;		//根据上一帧是否包含双车道确定点滤除的范围
	if(isDoubleLane)			//如果一侧存在两条车道线，则滤除范围较小
	{
		dep_left=-10;
		dep_right=10;
	}
	else						//如果不存在双线，则滤除范围较大，因为如果范围一直较小会找不到双线
	{
		dep_left=-60;
		dep_right=10;
	}

	for(int i=0;i<BW.rows;i++)
	{
		if(i<BW.rows/2)			//远视场的宽度限制
		{
			width_min=2;
			width_max=15;
		}
		else					//近视场的宽度限制
		{
			width_min=8;
			width_max=30;
		}
		uchar* data=BW.ptr<uchar>(i);
		for(int j=0;j<boundary;j++)		//提取出白色像素点
		{
			if(data[j]==255)
			{
				rowcandi.push_back(j);
			}
		}
		size_t size=rowcandi.size();
		//if( i == 1,size > 30)                       //判断阈值分割是否合适，true则表示需要切换阈值模式
		//{
		//	left_thresholdFlag = true;
		//}
		int startIndex=size-1,stopIndex=size-1;		//每一段的起始索引
		if(size>3)
		{
			for(size_t k=size-1;k>0;k--)
			{
				if(rowcandi[k]-rowcandi[k-1]>3)		//白色像素点间隔大于阈值才进行分段，间隔过小可能由于车道缺失，灰尘等造成
				{
					stopIndex=k;
					if(startIndex-stopIndex<=width_max&&startIndex-stopIndex>=width_min)		//宽度符合条件，则根据是否跟上一帧偏差过大确定是否保留
					{
						if(frame_count==1)														//第一帧由于没有上一帧信息，直接保留
						{
							leftscanPoints.push_back(Point2f(i,rowcandi[startIndex]));
							stopIndex=size-1;
							break;
						}
						else
						{
							if(last_dash_points.size()>0 && last_left_points.size()>0)
							{
								if(((rowcandi[startIndex]-last_dash_points[i].x)<dep_right)&&((rowcandi[startIndex]-last_left_points[i].x)>dep_left)) //通过将本帧获得点的列与上一帧点的列比较，超出指定范围筛除干扰点
								{
									leftscanPoints.push_back(Point2f(i,rowcandi[startIndex]));		//后续帧若与上一帧偏差过大的点则不保留，否则保留
									stopIndex=size-1;
									break;
								}
							}
						}
					}
					startIndex=k-1;
				}
				else
				{
					stopIndex=k-1;
				}
			}
			if(stopIndex==0)
			{
				if(startIndex-stopIndex<=width_max&&startIndex-stopIndex>=width_min)			//最后一段采用与上面相同的方法进行处理
				{
					if(frame_count==1)
					{
						leftscanPoints.push_back(Point2f(i,rowcandi[startIndex]));
					}
					else
					{
						if(last_dash_points.size()>0 && last_left_points.size()>0)
						{
							if(((rowcandi[startIndex]-last_dash_points[i].x)<dep_right)&&((rowcandi[startIndex]-last_left_points[i].x)>dep_left))
							{
								leftscanPoints.push_back(Point2f(i,rowcandi[startIndex]));
							}
						}
					}
				}
			}
		}
		rowcandi.clear();
	}
}

void left_scanPointsFilter(vector<Point2i> &scanPoints,vector<Point2f> &leftLanePoints)
{
	vector<Point2i>::iterator it;						//清除扫描得到的无效点
	vector<Point2f> lanePoints;
	size_t size=scanPoints.size();
	int upperIndex=size-1;						//记录每一段的上沿的纵坐标
	int belowIndex=size-1;						//记录每一段的下沿的纵坐标
	int temp=0;
	Mat polyFactor;								//记录其中的一段的直线拟合参数
	double p0=0,p1=0;							//上述参数ihou
	for(size_t i=size-1;i>0;i--)
	{
		temp=scanPoints[i-1].y-scanPoints[i].y;
		if(temp>3||temp<-2)						//相邻行的横坐标差值过大时进行处理
		{
			upperIndex=i;
			if((belowIndex-upperIndex<5))		//这一段的点过少
			{
				for (int j=upperIndex; j<=belowIndex; j++)
				{
					scanPoints[j].x=0;
				}
				if(p1!=0&&abs(scanPoints[i-1].x*p1+p0-scanPoints[i-1].y)>20)			//对突变的上面点进行判断，以确定如何对索引进行设置
				{
					belowIndex=i-1;
					upperIndex=i;
					scanPoints[i-1].x=0;
				}
				else
					belowIndex=upperIndex=i-1;
			}
			else
			{
				lanePoints.clear();
				for (int j=upperIndex; j<=belowIndex; j++)
				{
					lanePoints.push_back(scanPoints[j]);
				}
				polynomial_curve_fit(lanePoints, 1, polyFactor);			//连续点数量足够时，通过对此段的拟合，确定突变处上面点是否为噪点
				p0 = polyFactor.at<double>(0,0);
				p1 = polyFactor.at<double>(1,0);
				double result = p0 + p1*scanPoints[i-1].x;
				if (abs(scanPoints[i-1].y-result) >20)
				{
					scanPoints[i-1].x=0;
				}
				else
				{
					belowIndex=upperIndex=i-1;
				}
			}
			belowIndex=i-1;
		}
		else															//坐标没有突变时的处理方法
		{
			if((upperIndex!=belowIndex))								//不是正常点时，每一个都进行判断
			{
				if(abs(scanPoints[i-1].x*p1+p0-scanPoints[i-1].y)>20)
				{
					scanPoints[i].x=0;
					belowIndex=i-1;
				}
				else
				{
					belowIndex=i-1;
					upperIndex=i;
				}
			}
		}
	}
	if(belowIndex==0)
	{
		upperIndex=0;
	}
	if(belowIndex==upperIndex)
	{
		upperIndex=0;
	}
	if((belowIndex-upperIndex<5))		//最后一段的点过少
	{
		for (int j=upperIndex; j<=belowIndex; j++)
		{
			scanPoints[j].x=0;
		}
	}
	it = scanPoints.begin();			//清楚不符合要求的点
	while (it != scanPoints.end())
	{
		if ((*it).x == 0)
		{
			it = scanPoints.erase(it);
		}
		else
		{
			it ++;
		}
	}
	for(size_t i=0;i<scanPoints.size();i++)
	{
		leftLanePoints.push_back(scanPoints[i]);
	}
}

void Filter1(vector<Point2i> &scanPoints)				//右侧车道线点滤波第一步	
{//去除横坐标值变化较小的噪点

    unsigned int startIndex = 0;							//每一段的起始索引
    unsigned int stopIndex = 0;							//每一段的终止索引
    int temp = 0;
    int temp1=0;
    unsigned int seq_min=0;								//最短的点序列的长度，过短时直接滤除，近视场设置为8，远视场设置为4
	size_t size = scanPoints.size();

	for (size_t i=0; i<size-1; i++)
	{
		if(i<size/2)
		{
			seq_min=4;
		}
		else
		{
			seq_min=8;
		}
		temp = scanPoints[i+1].y - scanPoints[i].y;
		if (temp < -2)
		{
			stopIndex = i;
			if (stopIndex-startIndex < seq_min)						//针对车道内的车辆的噪点，当连续点的数量少时，滤除
			{
                for (unsigned int j=startIndex; j<=stopIndex; j++)
				{
					scanPoints[j].y = 0;
				}
			}
			startIndex = i+1;
		}
		else if(temp > 2)										//针对车道缺失
		{
			stopIndex = i;
			if (stopIndex-startIndex < seq_min)
			{
                for (unsigned int j=startIndex; j<=stopIndex; j++)
				{
					scanPoints[j].y = 0;
				}
			}
			startIndex = i+1;
		}
		else
		{
			stopIndex = i+1;
		}
	}
	if(startIndex==size-1)
		startIndex=stopIndex=size-1;
	if (stopIndex-startIndex < 5)							//如果最后一段点过少时，滤除，针对绿化带等的影响
	{
        for (unsigned int j=startIndex; j<=stopIndex; j++)
		{
			scanPoints[j].y = 0;
		}
	}

	vector<Point2i>::iterator it;				//滤除噪点
	it = scanPoints.begin();
	while (it != scanPoints.end())
	{
		if ((*it).y == 0)
		{
			it = scanPoints.erase(it);
		}
		else
		{
			it ++;
		}
	}

	size = scanPoints.size();
	if(size>3)
	{
		for (size_t i=0; i<size-3; i++)
		{
			temp = scanPoints[i+1].y - scanPoints[i].y;
			temp1 = scanPoints[i+1].x - scanPoints[i].x;
			if(temp>2&&temp1<2)							//判断车道线是否出现缺失
			{
				if(scanPoints[i+3].y - scanPoints[i+1].y<-2)	
				{
					scanPoints[i+1].y=0;
					scanPoints[i+2].y=0;
					i+=2;
				}
			}
		}
		it = scanPoints.begin();
		while (it != scanPoints.end())
		{
			if ((*it).y == 0)
			{
				it = scanPoints.erase(it);
			}
			else
			{
				it ++;
			}
		}
	}

	return;
}

void leftFilter1(vector<Point2i> &scanPoints)				//左侧车道线点滤波第一步	
{//去除横坐标值变化较小的噪点

    unsigned int startIndex = 0;							//每一段的起始索引
    unsigned int stopIndex = 0;							//每一段的终止索引
	int temp = 0;
	int temp1=0;
    unsigned int seq_min=0;								//最短的点序列的长度，过短时直接滤除，近视场设置为8，远视场设置为4
	size_t size = scanPoints.size();

	for (size_t i=0; i<size-1; i++)
	{
		if(i<size/2)
		{
			seq_min=4;
		}
		else
		{
			seq_min=8;
		}
		temp = scanPoints[i+1].y - scanPoints[i].y;
		if (temp < -2)
		{
			stopIndex = i;
			if (stopIndex-startIndex < seq_min)						//针对车道内的车辆的噪点，当连续点的数量少时，滤除
			{
                for (unsigned int j=startIndex; j<=stopIndex; j++)
				{
					scanPoints[j].y = 0;
				}
			}
			startIndex = i+1;
		}
		else if(temp > 2)										//针对车道缺失
		{
			stopIndex = i;
			if (stopIndex-startIndex < seq_min)
			{
                for (unsigned int j=startIndex; j<=stopIndex; j++)
				{
					scanPoints[j].y = 0;
				}
			}
			startIndex = i+1;
		}
		else
		{
			stopIndex = i+1;
		}
	}
	if(startIndex==size-1)
		startIndex=stopIndex=size-1;
	if (stopIndex-startIndex < 5)							//如果最后一段点过少时，滤除，针对绿化带等的影响
	{
        for (unsigned int j=startIndex; j<=stopIndex; j++)
		{
			scanPoints[j].y = 0;
		}
	}

	vector<Point2i>::iterator it;				//滤除噪点
	it = scanPoints.begin();
	while (it != scanPoints.end())
	{
		if ((*it).y == 0)
		{
			it = scanPoints.erase(it);
		}
		else
		{
			it ++;
		}
	}

	size = scanPoints.size();
	if(size>3)
	{
		for (size_t i=0; i<size-3; i++)
		{
			temp = scanPoints[i+1].y - scanPoints[i].y;
			temp1 = scanPoints[i+1].x - scanPoints[i].x;
			if(temp<-2&&temp1<2)							//判断车道线是否出现缺失
			{
				if(scanPoints[i+3].y - scanPoints[i+1].y>2)	
				{
					scanPoints[i+1].y=0;
					scanPoints[i+2].y=0;
					i+=2;
				}
			}
		}
		it = scanPoints.begin();
		while (it != scanPoints.end())
		{
			if ((*it).y == 0)
			{
				it = scanPoints.erase(it);
			}
			else
			{
				it ++;
			}
		}
	}

	return;
}

void Round1Filter(vector<Point2i> &scanPoints)				//右侧车道线点滤波第一步
	{//去除掉横坐标值变化较大的孤立噪点
		int temp1 = scanPoints[0].y;
		size_t size = scanPoints.size();

		for (size_t i=0; i<size-1; i++)
		{
			if ((scanPoints[i+1].y-temp1) <0)				//去掉横坐标变小的点
			{
				temp1 = scanPoints[i+1].y;
				scanPoints[i+1].y = 0;						//噪点横坐标置0
			}
			else if ((scanPoints[i+1].y - temp1) > 3)		//去掉横坐标变大太多的点
			{
				temp1 = scanPoints[i+1].y;
				scanPoints[i+1].y = 0;
			}
			else 
			{
				temp1 = scanPoints[i+1].y;
			}
		}

		vector<Point2i>::iterator it;						//清除噪点
		it = scanPoints.begin();
		while (it != scanPoints.end())
		{
			if ((*it).y == 0)
			{
				it = scanPoints.erase(it);
			}
			else
			{
				it ++;
			}
		}
		return;
	}

void Round2Filter(vector<Point2i> &scanPoints)				//右侧车道线点滤波第二步	
{//去除横坐标值变化较小的噪点

    unsigned int startIndex = 0;							//每一段的起始索引
    unsigned int stopIndex = 0;							//每一段的终止索引
	int temp = 0;
    unsigned int seq_min=0;								//最短的点序列的长度，过短时直接滤除，近视场设置为8，远视场设置为4
	size_t size = scanPoints.size();

	for (size_t i=0; i<size-1; i++)
	{
		if(i<size/2)
		{
			seq_min=4;
		}
		else
		{
			seq_min=8;
		}
		temp = scanPoints[i+1].y - scanPoints[i].y;
		if (temp < -3)                                   //进入该循环说明扫描点不连续
		{
			stopIndex = i;
			if (stopIndex-startIndex < seq_min)						//针对车道内的车辆的噪点，当连续点的数量少时，滤除
			{
                for (unsigned int j=startIndex; j<=stopIndex; j++)
				{
					scanPoints[j].y = 0;
				}
			}
			startIndex = i+1;
		}
		else if(temp > 3)										//针对车道缺失，即车道线不连续
		{
			stopIndex = i;
			if (stopIndex-startIndex < seq_min)
			{
                for (unsigned int j=startIndex; j<=stopIndex; j++)
				{
					scanPoints[j].y = 0;
				}
			}
			startIndex = i+1;
		}
		else
		{
			stopIndex = i+1;
		}
	}
	if(startIndex==size-1)
		startIndex=stopIndex=size-1;
	if (stopIndex-startIndex < 5)							//如果最后一段点过少时，滤除，针对绿化带等的影响
	{
        for (unsigned int j=startIndex; j<=stopIndex; j++)
		{
			scanPoints[j].y = 0;
		}
	}

	vector<Point2i>::iterator it;				//滤除噪点
	it = scanPoints.begin();
	while (it != scanPoints.end())
	{
		if ((*it).y == 0)
		{
			it = scanPoints.erase(it);
		}
		else
		{
			it ++;
		}
	}
	return;
}

bool Round3Filter(vector<Point2i> &scanPoints,int &doubleLaneNum,bool &isDoubleLaneInLastFrame,vector<Point2f> &dashLanePoints,vector<Point2f> &solidLanePoints,Mat &BW)
{//消除连续的噪点群

	int num = 0;   //记录连续的噪点群的数量
	int startIndex = 0;
	int stopIndex = 0;
	bool flag = false;				//最后一段是否是从突变小处开始的标记位
	int temp;
	vector<Point2f> lanePoints;		// 临时的容器，存放虚线上的点，用于虚实线分离
	Mat polyFactor;
	size_t size = scanPoints.size();
	int seq_min=0;								//最短的点序列的长度，过短时直接滤除，近视场设置为8，远视场设置为4

	for (size_t i=0; i<size-1; i++)
	{
		if(i<size/2)
		{
			seq_min=4;
		}
		else
		{
			seq_min=8;
		}
		temp = scanPoints[i+1].y - scanPoints[i].y;
		if (temp<=-3)									//与步骤二类似，加上对每一段的分类标签，若是噪点，则滤除，若是有效点，则分类
		{
			stopIndex = i;
			if (stopIndex-startIndex < seq_min)
			{
				for (int j=startIndex; j<=stopIndex; j++)			//滤除短连续噪点
				{
					scanPoints[j].y = 0;
				}
			}
			else
			{
				flag = true;							//突变小，置1
			}

			startIndex = i+1;							//索引更替
		}
		else if(temp > 4)
		{
			stopIndex = i;
			//flag = false;								//横坐标突变大时不一定将flag置为false，这也有可能是因为虚线段的缺失导致的
			if (stopIndex-startIndex < seq_min)
			{
				for (int j=startIndex; j<=stopIndex; j++)			//滤除短连续噪点
				{
					scanPoints[j].y = 0;
				}
				startIndex = i+1;
				flag = false;
			}
			else 
			{
				if(scanPoints[i+1].x-scanPoints[i].x>2)
				{
					for (int j=startIndex; j<=stopIndex; j++)
					{
						lanePoints.push_back(scanPoints[j]);
					}

					Mat polyFactor;                                         //拟合曲线的参数矩阵
					polynomial_curve_fit(lanePoints, 1, polyFactor);
					double p0 = polyFactor.at<double>(0,0);
					double p1 = polyFactor.at<double>(1,0);
					double result = p0 + p1*scanPoints[i+1].x;

					if (((scanPoints[i+1].x-scanPoints[i].x<30) && (scanPoints[i+1].y-result>6)) || ((scanPoints[i+1].x-scanPoints[i].x>30) && (scanPoints[i+1].y-result>30)))
					{
						num += 1;								//右侧左边候选部分加1，候选部分大于2，则认为存在双线
						startIndex = i+1;
						flag = false;							//在判断为是由双车道引起的坐标突变时，将flag置为false
					}
					else if (((scanPoints[i+1].x-scanPoints[i].x<30) && (scanPoints[i+1].y-result<-6)) || ((scanPoints[i+1].x-scanPoints[i].x>30) && (scanPoints[i+1].y-result<-30)))
					{
						startIndex = i+1;
						flag = true;							//在判断为是由双车道引起的坐标突变时，将flag置为false（原为true）
					}
					else
					{
						stopIndex = i+1;						//若前一段拟合结果和后一段的接近，此时认为是虚线或者是实线由于某种原因导致中间缺失
					}
					lanePoints.clear();
				}
				else
				{
					num += 1;								//右侧左边候选部分加1，候选部分大于2，则认为存在双线
					startIndex = i+1;
					flag = false;							//在判断为是由双车道引起的坐标突变时，将flag置为false
				}
			}	
		}
		else 
		{
			if (scanPoints[i+1].x-scanPoints[i].x > 10)				//若横坐标变化在容许范围之内，要针对车道是否确实的情况对上一段进行判断
			{
				stopIndex = i;
				for (int j=startIndex; j<=stopIndex; j++)
				{
					lanePoints.push_back(scanPoints[j]);
				}

				//cout<<lanePoints.size()<<endl;
				Mat polyFactor;
				polynomial_curve_fit(lanePoints, 1, polyFactor);
				double p0 = polyFactor.at<double>(0,0);
				double p1 = polyFactor.at<double>(1,0);
				double result = p0 + p1*scanPoints[i+1].x;

				if ((scanPoints[i+1].y-result) < -6)						//上一段和下一段之间因为实线的缺损导致实线和虚线之间的横坐标差较小
				{															//需根据上一段和下一段之间的距离进行判断
					flag = true;
					startIndex = i+1;
				}
				else
				{
					stopIndex=i+1;
				} 
				lanePoints.clear();
			}
			else 
			{
				stopIndex = i+1;
			}
		}
	}

	if (flag)
	{					 //如果最后一段是从突变小处开始的，则认为此段也是虚线段
		num += 1;
	}

	vector<Point2i>::iterator it;			//滤除噪点
	it = scanPoints.begin();
	while (it != scanPoints.end())
	{
		if ((*it).y == 0)
		{
			it = scanPoints.erase(it);
		}
		else
		{
			it ++;
		}
	}
		
	bool isDoubleFlag = false;			//是否存在双线的标志
	startIndex = 0;						//本段与上一段完全一致，只是添加了点分类存储
	stopIndex = 0;
	flag = false;
	temp = 0;
		//d_temp[]
	lanePoints.clear();	
	size_t size1 = scanPoints.size();	
		
	for (size_t i=0; i<size1-1; i++)
	{
		if(i<size/2)
		{
			seq_min=8;                            //原是4
		}
		else
		{
			seq_min=8;
		}
		temp = scanPoints[i+1].y - scanPoints[i].y;
		if (temp <= -3)
		{
			stopIndex = i;
			if (stopIndex - startIndex < seq_min)
			{
				for (int j=startIndex; j<=stopIndex; j++)
				{
					scanPoints[j].y = 0;
				}
			}

			flag = true;
			startIndex = i+1;
		}
		else if(temp > 4)
		{
			stopIndex = i;
			if(scanPoints[i+1].x-scanPoints[i].x > 2)
			{
				for (int j=startIndex; j<=stopIndex; j++)
				{
					lanePoints.push_back(scanPoints[j]);
				}
				Mat polyFactor;
				polynomial_curve_fit(lanePoints, 1, polyFactor);
				double p0 = polyFactor.at<double>(0,0);
				double p1 = polyFactor.at<double>(1,0);
				double result = p0 + p1*scanPoints[i+1].x;

				if (((scanPoints[i+1].x-scanPoints[i].x<30) && (scanPoints[i+1].y-result>6)) || ((scanPoints[i+1].x-scanPoints[i].x>30) && (scanPoints[i+1].y-result>30)))
				{
					if (num > 0)
					{
						int j = 0;
						for (j=startIndex; j<=stopIndex; j++)
						{
							dashLanePoints.push_back(scanPoints[j]);			//左侧候选点,存入临时容器
						}
						for (j=startIndex; j<=stopIndex; j++)
						{
							int k;
							uchar* data=BW.ptr<uchar>(scanPoints[j].x);
							int r_boundary=scanPoints[j].y+(scanPoints[j].x/10)+20;
							if(r_boundary>BW.cols-1)
							{
								r_boundary=BW.cols-1;
							}
							for(k=scanPoints[j].y+(scanPoints[j].x/10);k<r_boundary;k++)							//k=scanPoints[j].y+10这里加10是加的车道线的宽度，防止车道线破损时这里只加1导致错误
							{
								if(data[k+1]-data[k]==255)
								{
									scanPoints[j].y = k;
									break;
								}
							}
							if(k==r_boundary)
								scanPoints[j].y=0;
							//scanPoints[j].y = 0;
						}
					}
					else 
					{
						for (int j=startIndex; j<=stopIndex; j++)
						{
							scanPoints[j].y = 0;
						}
					}
					startIndex = i+1;
					flag = false;
				}
				else if (((scanPoints[i+1].x-scanPoints[i].x<30) && (scanPoints[i+1].y-result<-6)) || ((scanPoints[i+1].x-scanPoints[i].x>30) && (scanPoints[i+1].y-result<-30)))
				{
					startIndex = i+1;
					flag = true;							//在判断为是由双车道引起的坐标突变时，将flag置为true
				}
				else
				{
					stopIndex = i+1;
				}
				lanePoints.clear();	
			}
			else
			{
				if (num > 0)
				{
					int j = 0;
					for (j=startIndex; j<=stopIndex; j++)
					{
						dashLanePoints.push_back(scanPoints[j]);			//左侧候选点,存入临时容器
					}
					for (j=startIndex; j<=stopIndex; j++)
					{
						int k;
						uchar* data=BW.ptr<uchar>(scanPoints[j].x);
						int r_boundary=scanPoints[j].y+(scanPoints[j].x/10)+20;
						if(r_boundary>BW.cols-1)
						{
							r_boundary=BW.cols-1;
						}
						for(k=scanPoints[j].y+(scanPoints[j].x/10);k<r_boundary;k++)							//k=scanPoints[j].y+10这里加10是加的车道线的宽度，防止车道线破损时这里只加1导致错误
						{
							if(data[k+1]-data[k]==255)
							{
								scanPoints[j].y = k;
								break;
							}
						}
						if(k==r_boundary)
							scanPoints[j].y=0;
						//scanPoints[j].y = 0;
					}
				}
				else 
				{
					for (int j=startIndex; j<=stopIndex; j++)
					{
						scanPoints[j].y = 0;
					}
				}
				startIndex = i+1;
				flag = false;
			}
		}
		else 
		{
			if (scanPoints[i+1].x-scanPoints[i].x > 10)
			{
				stopIndex = i;
				for (int j=startIndex; j<=stopIndex; j++)
				{
					lanePoints.push_back(scanPoints[j]);
				}
				Mat polyFactor;
				polynomial_curve_fit(lanePoints, 1, polyFactor);
				double p0 = polyFactor.at<double>(0,0);
				double p1 = polyFactor.at<double>(1,0);
				double result = p0 + p1*scanPoints[i+1].x;
				if (scanPoints[i+1].y-result < -6)
				{
					flag = true;
					startIndex = i+1;
				}
				else
				{
					stopIndex = i+1;
				}

					
			}
			else 
			{
				stopIndex = i+1;
			}
		}
	}

	if (flag)						//如果flag为1，说明最后一段是从突变小处开始的，也将其添加到内侧点
	{
		int j = 0;
		for (j=startIndex; j<=stopIndex; j++)
		{
			dashLanePoints.push_back(scanPoints[j]);
		}
		for (j=startIndex; j<=stopIndex; j++)
		{
			//scanPoints[j].y = 0;
			int k;
			uchar* data=BW.ptr<uchar>(scanPoints[j].x);
			int r_boundary=scanPoints[j].y+(scanPoints[j].x/10)+20;
			if(r_boundary>BW.cols-1)
			{
				r_boundary=BW.cols-1;
			}
			for(k=scanPoints[j].y+10;k<r_boundary;k++)			//内侧有点，则继续往外进行搜索，对点进行补充
			{
				if(data[k+1]-data[k]==255)
				{
					scanPoints[j].y = k+1;
					break;
				}
			}
			if(k==r_boundary)
				scanPoints[j].y=0;
		}
	}

		
	//判断是否存在双线
	if (num>1 || (isDoubleLaneInLastFrame && num==1))			//对是否存在双线进行判断
	{
		/*float dashMid = (dashLanePoints[0].y + dashLanePoints[dashLanePoints.size()-1].y) / 2;				//此部分是防止检测到别的车道，还需调整
		float solidMid = (scanPoints[0].y + scanPoints[scanPoints.size()-1].y) / 2;*/
		//Mat polyFactor,polyFactor1;
		//polynomial_curve_fit(dashLanePoints, 1, polyFactor);
		//double a0 = polyFactor.at<double>(0,0);
		//double a1 = polyFactor.at<double>(1,0);
		//vector<Point2f> temp_solidPoints;
		//for(size_t i=0;i<scanPoints.size();i++)
		//	temp_solidPoints.push_back(scanPoints[i]);
		//polynomial_curve_fit(temp_solidPoints, 1, polyFactor1);
		//double b0 = polyFactor1.at<double>(0,0);
		//double b1 = polyFactor1.at<double>(1,0);

		//if (abs(a0-b0+(a1-b1)*ROI_HEIGH/2)>40)		//如果虚实线间距过大，则说明外侧实线不是本车道
		//{
		//	isDoubleFlag = false;

		//	it = scanPoints.begin();						//删除solidPoints
		//	while (it != scanPoints.end())
		//	{
		//		it = scanPoints.erase(it);
		//	}	

		//}
		//else
		//{
		//	isDoubleFlag = true;
		//    doubleLaneNum += 1;
		//}
		isDoubleFlag = true;
		doubleLaneNum += 1;
	}			

	it = scanPoints.begin();						//滤除噪点
	while (it != scanPoints.end())
	{
		if ((*it).y == 0)
		{
			it = scanPoints.erase(it);
		}
		else
		{
			it ++;
		}
	}
	
	if(scanPoints.size()>1)				//因为又对外侧点进行了补充，所以再进行两次简单滤波
	{ 
		Round1Filter(scanPoints);
	}
	if(scanPoints.size()>1)
	{ 
		Round2Filter(scanPoints);
	}	
	if(scanPoints.size()>0)						//将点分类保存
	{
		for(size_t i=0;i<scanPoints.size();i++)
			solidLanePoints.push_back(scanPoints[i]);
	}
	else
	{
		for(size_t i=0;i<dashLanePoints.size();i++)
			solidLanePoints.push_back(dashLanePoints[i]);
	}
	return isDoubleFlag;
}

bool FeatureExtraction(vector<Point2i> &scanPoints,int &doubleLaneNum,bool &isDoubleLaneInLastFrame,vector<Point2f> &dashLanePoints,vector<Point2f> &solidLanePoints,Mat &BW)
{
	bool isDoubleLane=false;					//对步骤1-3进行整合
	Round1Filter(scanPoints);
	if(scanPoints.size()>1)
	{ 
		Round2Filter(scanPoints);
	}
		
	if(scanPoints.size()>5)
	{
		isDoubleLane=Round3Filter(scanPoints,doubleLaneNum,isDoubleLaneInLastFrame,dashLanePoints,solidLanePoints,BW);
	}
		
	//cout<<isDoubleLane<<endl;
	return isDoubleLane;
}

//bool LeftFeaturePointExt(vector<Point2i> &scanPoints,vector<Point2f> &leftPointsMatched,vector<Point2f> &farPoints,vector<Point2f> &closePoints,vector<Point2f> &lastLeftPoints,bool predict_label)
//{
//	int startIndex = 0;
//	int stopIndex = 0;
//	int temp = 0;
//	int temp1;
//	size_t size = scanPoints.size();
//	/******************************************/
//	/*对横坐标变大或者变小太多的单个点进行去除*/
//	/******************************************/
//	temp1 = scanPoints[0].y;

//	for (size_t i=0; i<size-1; i++)
//	{
//		if ((scanPoints[i+1].y!=0)&&((scanPoints[i+1].y-temp1) > 2))
//		{
//			temp1 = scanPoints[i+1].y;
//			scanPoints[i+1].y = 0;
//		}
//		else if ((scanPoints[i+1].y - temp1) < -5)
//		{
//			temp1 = scanPoints[i+1].y;
//			scanPoints[i+1].y = 0;
//		}
//		else
//		{
//			temp1 = scanPoints[i+1].y;
//		}
//	}
//	/********************************************/
//	/*          对小段内的噪点进行去除          */
//	/********************************************/
//	startIndex = 0;
//	stopIndex = 0;
//	temp = 0;

//	for (size_t i=0; i<size-1; i++)
//	{
//		temp = scanPoints[i+1].y - scanPoints[i].y;
//		if (temp > 2)
//		{
//			stopIndex = i;
//			if (stopIndex-startIndex < 4)
//			{
//				for (int j=startIndex; j<=stopIndex; j++)
//				{
//					scanPoints[j].y = 0;
//				}
//			}
//			startIndex = i+1;
//		}
//		else if(temp <- 3)
//		{
//			stopIndex = i;
//			if (stopIndex-startIndex < 4)
//			{
//				for (int j=startIndex; j<=stopIndex; j++)
//				{
//					scanPoints[j].y = 0;
//				}
//			}
//			startIndex = i+1;
//		}
//		else
//		{
//			stopIndex = i+1;
//		}
//	}
//	if(startIndex==size)
//		stopIndex=size;
//	if (stopIndex-startIndex < 3)
//	{
//		for (int j=startIndex; j<=stopIndex; j++)
//		{
//			scanPoints[j].y = 0;
//		}
//	}
		
//	vector<Point2i>::iterator it;
//	it = scanPoints.begin();
//	while (it != scanPoints.end())
//	{
//		if ((*it).y == 0)
//		{
//			it = scanPoints.erase(it);
//		}
//		else
//		{
//			it ++;
//		}
//	}

//	/*****************************************************/
//	/*对每段长度进行判断，去除长度小于6的（测试是否需要）*/
//	/*****************************************************/
//	startIndex = 0;
//	stopIndex = 0;
//	temp = 0;
//	size_t size1 = scanPoints.size();
//	if(size1==0)
//		return false;
//	for (size_t i=0; i<size1-1; i++)
//	{
//		temp = scanPoints[i+1].x - scanPoints[i].x;
//		if(temp>5)
//		{
//			stopIndex=i;
//			/*if (stopIndex-startIndex < 6)
//			{
//				for (int j=startIndex; j<=stopIndex; j++)
//				{
//					scanPoints[j].y = 0;
//				}
//			}*/
//			if (stopIndex-startIndex > 3)
//			{
//				if(scanPoints[startIndex].x<10)
//				{
//					for (int j=startIndex; j<=stopIndex; j++)
//				{
//					scanPoints[j].y = 0;
//				}
//				}
//			}
//			startIndex=i+1;
//		}
//		else
//		{
//			if((scanPoints[i+1].y - scanPoints[i].y)<-3)
//			{
//				stopIndex=i;
//				for (int j=startIndex; j<=stopIndex; j++)
//				{
//					scanPoints[j].y = 0;
//				}
//				startIndex=i+1;
//			}
//			else
//			{
//				stopIndex=i+1;
//			}
//		}
			
//	}
//	vector<Point2i>::iterator it1;
//	it1 = scanPoints.begin();
//	while (it1 != scanPoints.end())
//	{
//		if ((*it1).y == 0)
//		{
//			it1 = scanPoints.erase(it1);
//		}
//		else
//		{
//			it1 ++;
//		}
//	}

//	vector<Point2i> temp_scanPoints;				//因最后需返回1：160的结果，因此用一个临时容器存放（0，1：160）
//	for(size_t i=0;i<size;i++)
//	{
//		if(i<0)
//		{
//			if(predict_label)						//若需预测，则在顶部加上宽度匹配的控制点，控制整体趋势
//				temp_scanPoints.push_back(Point(leftPointsMatched[i].y,leftPointsMatched[i].x-8));
//			else
//				temp_scanPoints.push_back(Point2i(i,0));
//		}
//		else
//		{
//			temp_scanPoints.push_back(Point2i(i,0));
//		}
//		//temp_scanPoints.push_back(Point2i(i,0));
//	}
//	for(size_t i=0;i<scanPoints.size();i++)
//	{
//		int temp_y=scanPoints[i].x;
//		temp_scanPoints[temp_y].y=scanPoints[i].y;
//	}
//	scanPoints=temp_scanPoints;


//	vector<Point2f> upperPoints;			//按纵坐标将点分为近视场和远视场
//	vector<Point2f> belowPoints;
//	//for(int i=0;i<size/2;i++)
//	//{
//	//	upperPoints.push_back(scanPoints[i]);
//	//	//belowPoints.push_back(scanPoints[i+size/2]);
//	//	if(scanPoints[i+size/2].y==0)
//	//		belowPoints.push_back(Point2i(scanPoints[i+size/2].x,leftPointsMatched[i+size/2].x));
//	//	else
//	//		belowPoints.push_back(Point2i(scanPoints[i+size/2].x,(scanPoints[i+size/2].y+leftPointsMatched[i+size/2].x)/2));
//	//}
//	//for(int i=0;i<40;i++)
//	//{
//	//	upperPoints.push_back(belowPoints[i]);
//	//	closePoints.push_back(belowPoints[i+40]);
//	//}
//	//for(size_t i=0;i<upperPoints.size();i++)
//	//{
//	//	if(upperPoints[i].y!=0)
//	//		farPoints.push_back(upperPoints[i]);
//	//}
//	for(int i=0;i<120;i++)
//	{
//		upperPoints.push_back(scanPoints[i]);
//	}
//	for(int i=120;i<160;i++)
//	{
//		//if(scanPoints[i].y==0)						//无效点，不添加
//		//	belowPoints.push_back(Point2i(scanPoints[i].x,leftPointsMatched[i].x));
//		//else
//		//	belowPoints.push_back(Point2i(scanPoints[i].x,(scanPoints[i].y+leftPointsMatched[i].x)/2));
//		belowPoints.push_back(scanPoints[i]);
//	}
//	for(int i=0;i<10;i++)
//		upperPoints.push_back(belowPoints[i]);
//	for(size_t i=0;i<upperPoints.size();i++)
//	{
//		if(upperPoints[i].y!=0)						//无效点，不添加
//			farPoints.push_back(upperPoints[i]);
//	}
//	for(int i=0;i<30;i++)
//	{
//		if(belowPoints[i+10].y!=0)
//			closePoints.push_back(belowPoints[i+10]);
//	}
			
		
//	/*for(int i=0;i<40;i++)
//	{
//		upperPoints.push_back(scanPoints[i]);
//	}
//	for(int i=40;i<80;i++)
//	{
//		if(scanPoints[i].y==0)
//			upperPoints.push_back(Point2i(scanPoints[i].x,leftPointsMatched[i].x));
//		else
//			upperPoints.push_back(Point2i(scanPoints[i].x,(scanPoints[i].y+leftPointsMatched[i].x)/2));
//	}
//	for(int i=80;i<120;i++)
//	{
//		belowPoints.push_back(scanPoints[i]);
//	}
//	for(int i=120;i<160;i++)
//	{
//		if(scanPoints[i].y==0)
//			belowPoints.push_back(Point2i(scanPoints[i].x,leftPointsMatched[i].x));
//		else
//			belowPoints.push_back(Point2i(scanPoints[i].x,(scanPoints[i].y+leftPointsMatched[i].x)/2));
//	}
//	for(size_t i=0;i<upperPoints.size();i++)
//	{
//		if(upperPoints[i].y!=0)
//			farPoints.push_back(upperPoints[i]);
//	}
//	for(size_t i=0;i<belowPoints.size();i++)
//	{
//		if(belowPoints[i].y!=0)
//			closePoints.push_back(belowPoints[i]);
//	}
//	*/
		
//	return true;
//}

//void ScanPointsInGrayImage(Mat &gray, vector<Point2i> &scanPoints, int leftOrRight, int scanInitPoint, int frameNum)
//{
//	int width = gray.cols;
//	int height = gray.rows;
//	int midPoint = width/2 - 1;
//	bool swapFlag = false;        //跳变标志
//	int times = 0;               //记录高亮像素是否满足车道宽度
//	int temp = 0;                    //连续两个像素的像素差
//	int frontPoint = 0;

//	if (frameNum == 1)
//	{
//		if (leftOrRight == RIGHT)               //扫描右侧目标点
//		{
//			for (int i=0; i<height/2; i++)                 //先对ROI上半部扫描
//			{
//				for (int j=midPoint+1; j<width-1; j++)
//				{
//					temp = gray.at<uchar>(i,j) - gray.at<uchar>(i,j-1);
//					if (temp>FRONT_LANE_PIXEL_SWAP_IN_ZONE1 && !swapFlag)                       //出现像素跳变
//					{
//						frontPoint = j;
//						swapFlag = true;
//						times = 1;
//					}
//					else if (temp<REAR_LANE_PIXEL_SWAP_IN_ZONE1 && swapFlag)                 //车道另外一边的跳变
//					{
//						if (times>=ZONE1_MIN_LANE_WIDTH && times<=ZONE1_MAX_LANE_WIDTH)
//						{
//							scanPoints.push_back(Point2i(i, frontPoint));        //x与y调换位置，方便拟合
//						}
//						swapFlag = false;
//						times = 0;

//						break;
//					}
//					else
//					{
//						times ++;
//					}
//				}
//			}

//			for (int i=height/2; i<height; i++)
//			{
//				for (int j=midPoint+1; j<width-1; j++)
//				{
//					temp = gray.at<uchar>(i,j) - gray.at<uchar>(i,j-1);
//					if (temp>FRONT_LANE_PIXEL_SWAP_IN_ZONE2 && !swapFlag)
//					{
//						frontPoint = j;
//						swapFlag = true;
//						times = 1;
//					}
//					else if (temp<REAR_LANE_PIXEL_SWAP_IN_ZONE2 &&swapFlag)
//					{
//						if (times>=ZONE2_MIN_LANE_WIDTH && times<=ZONE2_MAX_LANE_WIDTH)
//						{
//							scanPoints.push_back(Point2i(i, frontPoint));
//						}
//						swapFlag = false;
//						times = 0;

//						break;
//					}
//					else
//					{
//						times ++;
//					}
//				}
//			}
//		}

//		if (leftOrRight == LEFT)                   //对左边进行扫描
//		{
//			for (int i=0; i<height/2; i++)
//			{
//				for (int j=midPoint-1; j>0; j--)
//				{
//					temp = gray.at<uchar>(i,j) - gray.at<uchar>(i,j+1);
//					if (temp>FRONT_LANE_PIXEL_SWAP_IN_ZONE1 && !swapFlag)
//					{
//						frontPoint = j;
//						swapFlag = true;
//						times = 1;
//					}
//					else if (temp<REAR_LANE_PIXEL_SWAP_IN_ZONE1 &&swapFlag)
//					{
//						if (times>=ZONE1_MIN_LANE_WIDTH && times<=ZONE1_MAX_LANE_WIDTH)
//						{
//							scanPoints.push_back(Point2i(i, frontPoint));
//						}
//						swapFlag = false;
//						times = 0;

//						break;
//					}
//					else
//					{
//						times ++;
//					}
//				}
//			}

//			for (int i=height/2; i<height; i++)
//			{
//				for (int j=midPoint-1; j>0; j--)
//				{
//					temp = gray.at<uchar>(i,j) - gray.at<uchar>(i,j+1);
//					if (temp>FRONT_LANE_PIXEL_SWAP_IN_ZONE2 && !swapFlag)
//					{
//						frontPoint = j;
//						swapFlag = true;
//						times = 1;
//					}
//					else if (temp<REAR_LANE_PIXEL_SWAP_IN_ZONE2 &&swapFlag)
//					{
//						if (times>=ZONE2_MIN_LANE_WIDTH && times<=ZONE2_MAX_LANE_WIDTH)
//						{
//							scanPoints.push_back(Point2i(i, frontPoint));
//						}
//						swapFlag = false;
//						times = 0;

//						break;
//					}
//					else
//					{
//						times ++;
//					}
//				}
//			}
//		}
//	}
//	else                    //不是第一帧
//	{
//		if (leftOrRight == RIGHT)               //扫描右侧目标点
//		{
//			for (int i=0; i<height/2; i++)                 //先对ROI上半部扫描
//			{
//				for (int j=scanInitPoint+5; j<width-1; j++)
//				{
//					temp = gray.at<uchar>(i,j) - gray.at<uchar>(i,j-1);
//					if (temp>FRONT_LANE_PIXEL_SWAP_IN_ZONE1 && !swapFlag)                       //出现像素跳变
//					{
//						frontPoint = j;
//						swapFlag = true;
//						times = 1;
//					}
//					else if (temp<REAR_LANE_PIXEL_SWAP_IN_ZONE1 && swapFlag)                 //车道另外一边的跳变
//					{
//						if (times>=ZONE1_MIN_LANE_WIDTH && times<=ZONE1_MAX_LANE_WIDTH)
//						{
//							scanPoints.push_back(Point2i(i, frontPoint));        //x与y调换位置，方便拟合
//						}
//						swapFlag = false;
//						times = 0;

//						break;
//					}
//					else
//					{
//						times ++;
//					}
//				}
//			}

//			for (int i=height/2; i<height; i++)
//			{
//				for (int j=scanInitPoint+5; j<width-1; j++)
//				{
//					temp = gray.at<uchar>(i,j) - gray.at<uchar>(i,j-1);
//					if (temp>FRONT_LANE_PIXEL_SWAP_IN_ZONE2 && !swapFlag)
//					{
//						frontPoint = j;
//						swapFlag = true;
//						times = 1;
//					}
//					else if (temp<REAR_LANE_PIXEL_SWAP_IN_ZONE2 && swapFlag)
//					{
//						if (times>=ZONE2_MIN_LANE_WIDTH && times<=ZONE2_MAX_LANE_WIDTH)
//						{
//							scanPoints.push_back(Point2i(i, frontPoint));
//						}
//						swapFlag = false;
//						times = 0;

//						break;
//					}
//					else
//					{
//						times ++;
//					}
//				}
//			}
//		}

//		if (leftOrRight == LEFT)                   //对左边进行扫描
//		{
//			for (int i=0; i<height/2; i++)
//			{
//				for (int j=scanInitPoint-5; j>0; j--)
//				{
//					temp = gray.at<uchar>(i,j) - gray.at<uchar>(i,j+1);
//					if (temp>FRONT_LANE_PIXEL_SWAP_IN_ZONE1 && !swapFlag)
//					{
//						frontPoint = j;
//						swapFlag = true;
//						times = 1;
//					}
//					else if (temp<REAR_LANE_PIXEL_SWAP_IN_ZONE1 &&swapFlag)
//					{
//						if (times>=ZONE1_MIN_LANE_WIDTH && times<=ZONE1_MAX_LANE_WIDTH)
//						{
//							scanPoints.push_back(Point2i(i, frontPoint));
//						}
//						swapFlag = false;
//						times = 0;

//						break;
//					}
//					else
//					{
//						times ++;
//					}
//				}
//			}

//			for (int i=height/2; i<height; i++)
//			{
//				for (int j=scanInitPoint-5; j>0; j--)
//				{
//					temp = gray.at<uchar>(i,j) - gray.at<uchar>(i,j+1);
//					if (temp>FRONT_LANE_PIXEL_SWAP_IN_ZONE2 && !swapFlag)
//					{
//						frontPoint = j;
//						swapFlag = true;
//						times = 1;
//					}
//					else if (temp<REAR_LANE_PIXEL_SWAP_IN_ZONE2 &&swapFlag)
//					{
//						if (times>=ZONE2_MIN_LANE_WIDTH && times<=ZONE2_MAX_LANE_WIDTH)
//						{
//							scanPoints.push_back(Point2i(i, frontPoint));
//						}
//						swapFlag = false;
//						times = 0;

//						break;
//					}
//					else
//					{
//						times ++;
//					}
//				}
//			}
//		}
//        }
//}
bool Filter2(vector<Point2i> &scanPoints,bool &isDoubleLaneInLastFrame,vector<Point2f> &dashLanePoints,
			 vector<Point2f> &solidLanePoints,vector<Point2f> &last_dash_point,Mat &bw)
{
	int num = 0;   //记录连续的噪点群的数量
	int startIndex = 0;
	int stopIndex = 0;
	bool flag = false;				//最后一段是否是从突变小处开始的标记
	int temp;						//相邻两点的横坐标差
	int temp1;						//相邻两张的纵坐标差
	vector<Point2f> lanePoints;		// 临时的容器，存放虚线上的点，用于虚实线分离
	Mat polyFactor;
	size_t size = scanPoints.size();
    //unsigned int seq_min=0;

	for (size_t i=0; i<size-1; i++)
	{
		temp = scanPoints[i+1].y - scanPoints[i].y;
		if(temp>3)
		{
			stopIndex=i;
			temp1=scanPoints[i+1].x-scanPoints[i].x;
			if(temp1<2)
			{
				startIndex=i+1;
				flag=false;
				num+=1;
			}
			else
			{
				for (int j=startIndex; j<=stopIndex; j++)
				{
					lanePoints.push_back(scanPoints[j]);
				}
				Mat polyFactor;
				polynomial_curve_fit(lanePoints, 1, polyFactor);
				double p0 = polyFactor.at<double>(0,0);
				double p1 = polyFactor.at<double>(1,0);
				double result = p0 + p1*scanPoints[i+1].x;
				if(scanPoints[i+1].y-result>4)
				{
					startIndex=i+1;
					flag=false;
					num+=1;
				}
				else if(scanPoints[i+1].y-result<-4)
				{
					startIndex=i+1;
					flag=true;
				}
				else
				{
					stopIndex=i+1;
				}
				lanePoints.clear();
			}
		}
		else if(temp<-3)
		{
			stopIndex=i;
			temp1=scanPoints[i+1].x-scanPoints[i].x;
			if(temp1<2)
			{
				startIndex=i+1;
				flag=true;
			}
			else
			{
				for (int j=startIndex; j<=stopIndex; j++)
				{
					lanePoints.push_back(scanPoints[j]);
				}
				Mat polyFactor;
				polynomial_curve_fit(lanePoints, 1, polyFactor);
				double p0 = polyFactor.at<double>(0,0);
				double p1 = polyFactor.at<double>(1,0);
				double result = p0 + p1*scanPoints[i+1].x;
				if(scanPoints[i+1].y-result>4)
				{
					startIndex=i+1;
					flag=false;
					num+=1;
				}
				else if(scanPoints[i+1].y-result<-4)
				{
					startIndex=i+1;
					flag=true;
				}
				else
				{
					stopIndex=i+1;
				}
				lanePoints.clear();
			}
		}
		else
		{
			temp1=scanPoints[i+1].x-scanPoints[i].x;
			if(temp1>10)
			{
				stopIndex=i;
				for (int j=startIndex; j<=stopIndex; j++)
				{
					lanePoints.push_back(scanPoints[j]);
				}
				Mat polyFactor;
				polynomial_curve_fit(lanePoints, 1, polyFactor);
				double p0 = polyFactor.at<double>(0,0);
				double p1 = polyFactor.at<double>(1,0);
				double result = p0 + p1*scanPoints[i+1].x;
				if(scanPoints[i+1].y-result>4)
				{
					startIndex=i+1;
					flag=false;
					num+=1;
				}
				else if(scanPoints[i+1].y-result<-4)
				{
					startIndex=i+1;
					flag=true;
				}
				else
				{
					stopIndex=i+1;
				}
				lanePoints.clear();
			}
			else
			{
				stopIndex=i+1;
			}
		}
	}
	if (flag)
	{					 //如果最后一段是从突变小处开始的，则认为此段也是虚线段
		num += 1;
	}

	bool isDoubleFlag = false;			//是否存在双线的标志
	startIndex = 0;						//本段与上一段完全一致，只是添加了点分类存储
	stopIndex = 0;
	flag = false;
	temp = 0;
	lanePoints.clear();	

	for (size_t i=0; i<size-1; i++)
	{
		temp = scanPoints[i+1].y - scanPoints[i].y;
		if(temp>3)
		{
			stopIndex=i;
			temp1=scanPoints[i+1].x-scanPoints[i].x;
			if(temp1<2)
			{
				if (num > 0)
				{
					int j = 0;
					for (j=startIndex; j<=stopIndex; j++)
					{
						dashLanePoints.push_back(scanPoints[j]);			//左侧候选点,存入临时容器
					}
					for (j=startIndex; j<=stopIndex; j++)
					{
						int k;
						uchar* data=bw.ptr<uchar>(scanPoints[j].x);
						int r_boundary=scanPoints[j].y+((int)(last_dash_point[scanPoints[j].x].x)+10)+(scanPoints[j].x/10)+20;
						if(r_boundary>bw.cols-1)
						{
							r_boundary=bw.cols-1;
						}
						for(k=scanPoints[j].y+((int)(last_dash_point[scanPoints[j].x].x)+10)+(scanPoints[j].x/10);k<r_boundary;k++)							//k=scanPoints[j].y+10这里加10是加的车道线的宽度，防止车道线破损时这里只加1导致错误
						{
							if(data[k+1]-data[k]==255)
							{
								scanPoints[j].y = k+1-((int)(last_dash_point[scanPoints[j].x].x)+10);
								break;
							}
						}
						if(k==r_boundary)
							scanPoints[j].y=0;
					}
				}
				else 
				{
					for (int j=startIndex; j<=stopIndex; j++)
					{
						scanPoints[j].y = 0;
					}
				}
				startIndex = i+1;
				flag = false;
			}
			else
			{
				for (int j=startIndex; j<=stopIndex; j++)
				{
					lanePoints.push_back(scanPoints[j]);
				}
				Mat polyFactor;
				polynomial_curve_fit(lanePoints, 1, polyFactor);
				double p0 = polyFactor.at<double>(0,0);
				double p1 = polyFactor.at<double>(1,0);
				double result = p0 + p1*scanPoints[i+1].x;
				if(scanPoints[i+1].y-result>4)
				{
					if (num > 0)
					{
						int j = 0;
						for (j=startIndex; j<=stopIndex; j++)
						{
							dashLanePoints.push_back(scanPoints[j]);			//左侧候选点,存入临时容器
						}
						for (j=startIndex; j<=stopIndex; j++)
						{
							int k;
							uchar* data=bw.ptr<uchar>(scanPoints[j].x);
							int r_boundary=scanPoints[j].y+((int)(last_dash_point[scanPoints[j].x].x)+10)+(scanPoints[j].x/10)+20;
							if(r_boundary>bw.cols-1)
							{
								r_boundary=bw.cols-1;
							}
							for(k=scanPoints[j].y+((int)(last_dash_point[scanPoints[j].x].x)+10)+(scanPoints[j].x/10);k<r_boundary;k++)							//k=scanPoints[j].y+10这里加10是加的车道线的宽度，防止车道线破损时这里只加1导致错误
							{
								if(data[k+1]-data[k]==255)
								{
									scanPoints[j].y = k+1-((int)(last_dash_point[scanPoints[j].x].x)+10);
									break;
								}
							}
							if(k==r_boundary)
								scanPoints[j].y=0;
						}
					}
					else 
					{
						for (int j=startIndex; j<=stopIndex; j++)
						{
							scanPoints[j].y = 0;
						}
					}
					startIndex = i+1;
					flag = false;
				}
				else if(scanPoints[i+1].y-result<-4)
				{
					startIndex=i+1;
					flag=true;
				}
				else
				{
					stopIndex=i+1;
				}
				lanePoints.clear();
			}
		}
		else if(temp<-3)
		{
			stopIndex=i;
			temp1=scanPoints[i+1].x-scanPoints[i].x;
			if(temp1<2)
			{
				startIndex=i+1;
				flag=true;
			}
			else
			{
				for (int j=startIndex; j<=stopIndex; j++)
				{
					lanePoints.push_back(scanPoints[j]);
				}
				Mat polyFactor;
				polynomial_curve_fit(lanePoints, 1, polyFactor);
				double p0 = polyFactor.at<double>(0,0);
				double p1 = polyFactor.at<double>(1,0);
				double result = p0 + p1*scanPoints[i+1].x;
				if(scanPoints[i+1].y-result>4)
				{
					if (num > 0)
					{
						int j = 0;
						for (j=startIndex; j<=stopIndex; j++)
						{
							dashLanePoints.push_back(scanPoints[j]);			//左侧候选点,存入临时容器
						}
						for (j=startIndex; j<=stopIndex; j++)
						{
							int k;
							uchar* data=bw.ptr<uchar>(scanPoints[j].x);
							int r_boundary=scanPoints[j].y+((int)(last_dash_point[scanPoints[j].x].x)+10)+(scanPoints[j].x/10)+20;
							if(r_boundary>bw.cols-1)
							{
								r_boundary=bw.cols-1;
							}
							for(k=scanPoints[j].y+((int)(last_dash_point[scanPoints[j].x].x)+10)+(scanPoints[j].x/10);k<r_boundary;k++)							//k=scanPoints[j].y+10这里加10是加的车道线的宽度，防止车道线破损时这里只加1导致错误
							{
								if(data[k+1]-data[k]==255)
								{
									scanPoints[j].y = k+1-((int)(last_dash_point[scanPoints[j].x].x)+10);
									break;
								}
							}
							if(k==r_boundary)
								scanPoints[j].y=0;
						}
					}
					else 
					{
						for (int j=startIndex; j<=stopIndex; j++)
						{
							scanPoints[j].y = 0;
						}
					}
					startIndex = i+1;
					flag = false;
				}
				else if(scanPoints[i+1].y-result<-4)
				{
					startIndex=i+1;
					flag=true;
				}
				else
				{
					stopIndex=i+1;
				}
				lanePoints.clear();
			}
		}
		else
		{
			temp1=scanPoints[i+1].x-scanPoints[i].x;
			if(temp1>10)
			{
				stopIndex=i;
				for (int j=startIndex; j<=stopIndex; j++)
				{
					lanePoints.push_back(scanPoints[j]);
				}
				Mat polyFactor;
				polynomial_curve_fit(lanePoints, 1, polyFactor);
				double p0 = polyFactor.at<double>(0,0);
				double p1 = polyFactor.at<double>(1,0);
				double result = p0 + p1*scanPoints[i+1].x;
				if(scanPoints[i+1].y-result>4)
				{
					if (num > 0)
					{
						int j = 0;
						for (j=startIndex; j<=stopIndex; j++)
						{
							dashLanePoints.push_back(scanPoints[j]);			//左侧候选点,存入临时容器
						}
						for (j=startIndex; j<=stopIndex; j++)
						{
							int k;
							uchar* data=bw.ptr<uchar>(scanPoints[j].x);
							int r_boundary=scanPoints[j].y+((int)(last_dash_point[scanPoints[j].x].x)+10)+(scanPoints[j].x/10)+20;
							if(r_boundary>bw.cols-1)
							{
								r_boundary=bw.cols-1;
							}
							for(k=scanPoints[j].y+((int)(last_dash_point[scanPoints[j].x].x)+10)+(scanPoints[j].x/10);k<r_boundary;k++)							//k=scanPoints[j].y+10这里加10是加的车道线的宽度，防止车道线破损时这里只加1导致错误
							{
								if(data[k+1]-data[k]==255)
								{
									scanPoints[j].y = k+1-((int)(last_dash_point[scanPoints[j].x].x)+10);
									break;
								}
							}
							if(k==r_boundary)
								scanPoints[j].y=0;
						}
					}
					else 
					{
						for (int j=startIndex; j<=stopIndex; j++)
						{
							scanPoints[j].y = 0;
						}
					}
					startIndex = i+1;
					flag = false;
				}
				else if(scanPoints[i+1].y-result<-4)
				{
					startIndex=i+1;
					flag=true;
				}
				else
				{
					stopIndex=i+1;
				}
				lanePoints.clear();
			}
			else
			{
				stopIndex=i+1;
			}
		}
	}
	if (flag)						//如果flag为1，说明最后一段是从突变小处开始的，也将其添加到内侧点
	{
		int j = 0;
		for (j=startIndex; j<=stopIndex; j++)
		{
			dashLanePoints.push_back(scanPoints[j]);
		}
		for (j=startIndex; j<=stopIndex; j++)
		{
			//scanPoints[j].y = 0;
			int k;
			uchar* data=bw.ptr<uchar>(scanPoints[j].x);
			int r_boundary=scanPoints[j].y+((int)(last_dash_point[scanPoints[j].x].x)+10)+(scanPoints[j].x/10)+20;
			if(r_boundary>bw.cols-1)
			{
				r_boundary=bw.cols-1;
			}
			for(k=scanPoints[j].y+((int)(last_dash_point[scanPoints[j].x].x)+10)+(scanPoints[j].x/10);k<r_boundary;k++)							//k=scanPoints[j].y+10这里加10是加的车道线的宽度，防止车道线破损时这里只加1导致错误
			{
				if(data[k+1]-data[k]==255)
				{
					scanPoints[j].y = k+1-((int)(last_dash_point[scanPoints[j].x].x)+10);
					break;
				}
			}
			if(k==r_boundary)
				scanPoints[j].y=0;
		}
	}
	//判断是否存在双线
	if (num>1 || (isDoubleLaneInLastFrame && num==1))			//对是否存在双线进行判断
	{
		isDoubleFlag = true;
		//doubleLaneNum += 1;
	}			

	vector<Point2i>::iterator it;						//清除噪点
	it = scanPoints.begin();
	while (it != scanPoints.end())
	{
		if ((*it).y == 0)
		{
			it = scanPoints.erase(it);
		}
		else
		{
			it ++;
		}
	}
	
	if(scanPoints.size()>1)				//因为又对外侧点进行了补充，所以再进行两次简单滤波
	{ 
		Round1Filter(scanPoints);
	}
	if(scanPoints.size()>1)
	{ 
		Round2Filter(scanPoints);
	}	
	if(scanPoints.size()>0)						//将点分类保存
	{
		for(size_t i=0;i<scanPoints.size();i++)
			solidLanePoints.push_back(scanPoints[i]);
	}
	else
	{
		for(size_t i=0;i<dashLanePoints.size();i++)
			solidLanePoints.push_back(dashLanePoints[i]);
	}
	return isDoubleFlag;
}

bool leftFilter2(vector<Point2i> &scanPoints,bool &isDoubleLaneInLastFrame,vector<Point2f> &dashLanePoints,
                 vector<Point2f> &solidLanePoints,vector<Point2f> &left_last_solid_point,Mat &bw)
{
    int num = 0;   //记录连续的噪点群的数量
    int startIndex = 0;
    int stopIndex = 0;
    bool flag = false;				//最后一段是否是从突变大处开始的标记
    int temp;						//相邻两点的横坐标差
    int temp1;						//相邻两张的纵坐标差
    vector<Point2f> lanePoints;		// 临时的容器，存放虚线上的点，用于虚实线分离
    Mat polyFactor;
    size_t size = scanPoints.size();
    //int seq_min=0;

    for (size_t i=0; i<size-1; i++)
    {
        temp = scanPoints[i+1].y - scanPoints[i].y;
        if(temp>3)
        {
            stopIndex=i;
            temp1=scanPoints[i+1].x-scanPoints[i].x;
            if(temp1<2)
            {
                startIndex=i+1;
                flag=true;
            }
            else
            {
                for (int j=startIndex; j<=stopIndex; j++)
                {
                    lanePoints.push_back(scanPoints[j]);
                }
                Mat polyFactor;
                polynomial_curve_fit(lanePoints, 1, polyFactor);
                double p0 = polyFactor.at<double>(0,0);
                double p1 = polyFactor.at<double>(1,0);
                double result = p0 + p1*scanPoints[i+1].x;
                if(scanPoints[i+1].y-result>4)
                {
                    startIndex=i+1;
                    flag=true;
                }
                else if(scanPoints[i+1].y-result<-4)
                {
                    startIndex=i+1;
                    flag=false;
                    num+=1;
                }
                else
                {
                    stopIndex=i+1;
                }
                lanePoints.clear();
            }
        }
        else if(temp<-3)
        {
            stopIndex=i;
            temp1=scanPoints[i+1].x-scanPoints[i].x;
            if(temp1<2)
            {
                startIndex=i+1;
                flag=false;
                num+=1;
            }
            else
            {
                for (int j=startIndex; j<=stopIndex; j++)
                {
                    lanePoints.push_back(scanPoints[j]);
                }
                Mat polyFactor;
                polynomial_curve_fit(lanePoints, 1, polyFactor);
                double p0 = polyFactor.at<double>(0,0);
                double p1 = polyFactor.at<double>(1,0);
                double result = p0 + p1*scanPoints[i+1].x;
                if(scanPoints[i+1].y-result>4)
                {
                    startIndex=i+1;
                    flag=true;
                }
                else if(scanPoints[i+1].y-result<-4)
                {
                    startIndex=i+1;
                    flag=false;
                    num+=1;
                }
                else
                {
                    stopIndex=i+1;
                }
                lanePoints.clear();
            }
        }
        else
        {
            stopIndex=i+1;
        }
    }
    if (flag)
    {
        num += 1;
    }

    bool isDoubleFlag = false;			//是否存在双线的标志
    startIndex = 0;						//本段与上一段完全一致，只是添加了点分类存储
    stopIndex = 0;
    flag = false;
    temp = 0;
    lanePoints.clear();

    for (size_t i=0; i<size-1; i++)
    {
        temp = scanPoints[i+1].y - scanPoints[i].y;
        if(temp<-3)
        {
            stopIndex=i;
            temp1=scanPoints[i+1].x-scanPoints[i].x;
            if(temp1<2)
            {
                if (num > 0)
                {
                    int j = 0;
                    for (j=startIndex; j<=stopIndex; j++)
                    {
                        dashLanePoints.push_back(scanPoints[j]);			//内侧候选点,存入临时容器
                    }
                    for (j=startIndex; j<=stopIndex; j++)
                    {
                        int k;
                        uchar* data=bw.ptr<uchar>(scanPoints[j].x);
                        int r_boundary=scanPoints[j].y-(scanPoints[j].x/10)-20+((int)(left_last_solid_point[scanPoints[j].x].x)+10);
                        if(r_boundary<0)
                        {
                            r_boundary=0;
                        }
                        for(k=scanPoints[j].y-(scanPoints[j].x/10)+((int)(left_last_solid_point[scanPoints[j].x].x)+10);k>r_boundary;k--)							//k=scanPoints[j].y+10这里加10是加的车道线的宽度，防止车道线破损时这里只加1导致错误
                        {
                            if(data[k-1]-data[k]==255)
                            {
                                scanPoints[j].y = k-1-((int)(left_last_solid_point[scanPoints[j].x].x)+10);
                                break;
                            }
                        }
                        if(k==r_boundary)
                            scanPoints[j].y=0;
                    }
                }
                else
                {
                    for (int j=startIndex; j<=stopIndex; j++)
                    {
                        scanPoints[j].y = 0;
                    }
                }
                startIndex = i+1;
                flag = false;
            }
            else
            {
                for (int j=startIndex; j<=stopIndex; j++)
                {
                    lanePoints.push_back(scanPoints[j]);
                }
                Mat polyFactor;
                polynomial_curve_fit(lanePoints, 1, polyFactor);
                double p0 = polyFactor.at<double>(0,0);
                double p1 = polyFactor.at<double>(1,0);
                double result = p0 + p1*scanPoints[i+1].x;
                if(scanPoints[i+1].y-result<-4)
                {
                    if (num > 0)
                    {
                        int j = 0;
                        for (j=startIndex; j<=stopIndex; j++)
                        {
                            dashLanePoints.push_back(scanPoints[j]);			//左侧候选点,存入临时容器
                        }
                        for (j=startIndex; j<=stopIndex; j++)
                        {
                            int k;
                            uchar* data=bw.ptr<uchar>(scanPoints[j].x);
                            int r_boundary=scanPoints[j].y-(scanPoints[j].x/10)-20+((int)(left_last_solid_point[scanPoints[j].x].x)+10);
                            if(r_boundary<0)
                            {
                                r_boundary=0;
                            }
                            for(k=scanPoints[j].y-(scanPoints[j].x/10)+((int)(left_last_solid_point[scanPoints[j].x].x)+10);k>r_boundary;k--)							//k=scanPoints[j].y+10这里加10是加的车道线的宽度，防止车道线破损时这里只加1导致错误
                            {
                                if(data[k-1]-data[k]==255)
                                {
                                    scanPoints[j].y = k-1-((int)(left_last_solid_point[scanPoints[j].x].x)+10);
                                    break;
                                }
                            }
                            if(k==r_boundary)
                                scanPoints[j].y=0;
                        }
                    }
                    else
                    {
                        for (int j=startIndex; j<=stopIndex; j++)
                        {
                            scanPoints[j].y = 0;
                        }
                    }
                    startIndex = i+1;
                    flag = false;
                }
                else if(scanPoints[i+1].y-result>4)
                {
                    startIndex=i+1;
                    flag=true;
                }
                else
                {
                    stopIndex=i+1;
                }
                lanePoints.clear();
            }
        }
        else if(temp>3)
        {
            stopIndex=i;
            temp1=scanPoints[i+1].x-scanPoints[i].x;
            if(temp1<2)
            {
                startIndex=i+1;
                flag=true;
            }
            else
            {
                for (int j=startIndex; j<=stopIndex; j++)
                {
                    lanePoints.push_back(scanPoints[j]);
                }
                Mat polyFactor;
                polynomial_curve_fit(lanePoints, 1, polyFactor);
                double p0 = polyFactor.at<double>(0,0);
                double p1 = polyFactor.at<double>(1,0);
                double result = p0 + p1*scanPoints[i+1].x;
                if(scanPoints[i+1].y-result<-4)
                {
                    if (num > 0)
                    {
                        int j = 0;
                        for (j=startIndex; j<=stopIndex; j++)
                        {
                            dashLanePoints.push_back(scanPoints[j]);			//左侧候选点,存入临时容器
                        }
                        for (j=startIndex; j<=stopIndex; j++)
                        {
                            int k;
                            uchar* data=bw.ptr<uchar>(scanPoints[j].x);
                            int r_boundary=scanPoints[j].y-(scanPoints[j].x/10)-20+((int)(left_last_solid_point[scanPoints[j].x].x)+10);
                            if(r_boundary<0)
                            {
                                r_boundary=0;
                            }
                            for(k=scanPoints[j].y-(scanPoints[j].x/10)+((int)(left_last_solid_point[scanPoints[j].x].x)+10);k>r_boundary;k--)							//k=scanPoints[j].y+10这里加10是加的车道线的宽度，防止车道线破损时这里只加1导致错误
                            {
                                if(data[k-1]-data[k]==255)
                                {
                                    scanPoints[j].y = k-1-((int)(left_last_solid_point[scanPoints[j].x].x)+10);
                                    break;
                                }
                            }
                            if(k==r_boundary)
                                scanPoints[j].y=0;
                        }
                    }
                    else
                    {
                        for (int j=startIndex; j<=stopIndex; j++)
                        {
                            scanPoints[j].y = 0;
                        }
                    }
                    startIndex = i+1;
                    flag = false;
                }
                else if(scanPoints[i+1].y-result>4)
                {
                    startIndex=i+1;
                    flag=true;
                }
                else
                {
                    stopIndex=i+1;
                }
                lanePoints.clear();
            }
        }
        else
        {
            stopIndex=i+1;
        }
    }
    if (flag)						//如果flag为1，说明最后一段是从突变小处开始的，也将其添加到内侧点
    {
        int j = 0;
        for (j=startIndex; j<=stopIndex; j++)
        {
            dashLanePoints.push_back(scanPoints[j]);
        }
        for (j=startIndex; j<=stopIndex; j++)
        {
            //scanPoints[j].y = 0;
            int k;
            uchar* data=bw.ptr<uchar>(scanPoints[j].x);
            int r_boundary=scanPoints[j].y-(scanPoints[j].x/10)-20+((int)(left_last_solid_point[scanPoints[j].x].x)+10);
            if(r_boundary<0)
            {
                r_boundary=0;
            }
            for(k=scanPoints[j].y-(scanPoints[j].x/10)+((int)(left_last_solid_point[scanPoints[j].x].x)+10);k>r_boundary;k--)							//k=scanPoints[j].y+10这里加10是加的车道线的宽度，防止车道线破损时这里只加1导致错误
            {
                if(data[k-1]-data[k]==255)
                {
                    scanPoints[j].y = k-1-((int)(left_last_solid_point[scanPoints[j].x].x)+10);
                    break;
                }
            }
            if(k==r_boundary)
                scanPoints[j].y=0;
        }
    }
    //判断是否存在双线
    if (num>1 || (isDoubleLaneInLastFrame && num==1))			//对是否存在双线进行判断
    {
        isDoubleFlag = true;
        //doubleLaneNum += 1;
    }

    vector<Point2i>::iterator it;						//清除噪点
    it = scanPoints.begin();
    while (it != scanPoints.end())
    {
        if ((*it).y == 0)
        {
            it = scanPoints.erase(it);
        }
        else
        {
            it ++;
        }
    }
	
    if(scanPoints.size()>1)				//因为又对外侧点进行了补充，所以再进行两次简单滤波
    {
        Round1Filter(scanPoints);
    }
    if(scanPoints.size()>1)
    {
        Round2Filter(scanPoints);
    }
    if(scanPoints.size()>0)						//将点分类保存
    {
        for(size_t i=0;i<scanPoints.size();i++)
            solidLanePoints.push_back(scanPoints[i]);
    }
    else
    {
        for(size_t i=0;i<dashLanePoints.size();i++)
            solidLanePoints.push_back(dashLanePoints[i]);
    }
    return isDoubleFlag;
}

bool thresholdModeSwitch(Mat src,int left_boundary,bool thresholdFit)
{
	   /* int num = 0;*/
	    vector<int> rowcandi;
		vector<int> rowcandi1;
		uchar* thresholdData1=src.ptr<uchar>(10);
		for(int j=0;j<left_boundary;j++)			//提取出白色像素点
		{
			if(thresholdData1[j]==255)
			{
				rowcandi.push_back(j);
			}
		}
		uchar* thresholdData2=src.ptr<uchar>(10);
		for(int k=left_boundary;k<ROI_WIDTH;k++)
		{
			if(thresholdData2[k]==255)
			{
				rowcandi1.push_back(k);
			}
		}
		size_t size=rowcandi.size();
		size_t size1=rowcandi1.size();
		if(size > 50 || size1 >50)
		{
			thresholdFit = true;;
			rowcandi.clear();
		}
  //      uchar* thresholdData2=src.ptr<uchar>(20);
		//for(int j=0;j<left_boundary;j++)			//提取出白色像素点
		//{
		//	if(thresholdData2[j]==255)
		//	{
		//		rowcandi.push_back(j);
		//	}
		//}
		//size=rowcandi.size();
		//if(size > 50)
		//{
		//	num++;
		//	rowcandi.clear();
		//}
		//if(num > 1)
		//{
		//	thresholdFit = true;
		//}
		return thresholdFit;
}

int OstuThreshold(Mat src)
{
    // 自己实现方法
    int width = src.cols;  
    int height = src.rows;  
    int pixelCount[256] = {0};  
    float pixelPro[256] = {0};  
    int i, j, pixelSum = width * height, Threshold = 0;  
  
    // 统计直方图  
    for(i=0; i<height; i++)  
    {  
        uchar* g_ptr = src.ptr<uchar>(i);  
        for(j=0; j<width; j++)  
        {  
            pixelCount[ g_ptr[j] ]++;  
        }  
    }  
  
    // 每个灰度值的比例  
    for(i=0; i<256; i++)  
    {  
        pixelPro[i] = (float)pixelCount[i] / pixelSum;  
    }  
  
    // 计算最大类间方差  
    float w0, w1, u0tmp, u1tmp, u0, u1, deltaTmp, deltaMax = 0;    
    for(i = 0; i < 256; i++)    
    {    
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = deltaTmp = 0;    
        for(j = 0; j < 256; j++)    
        {    
            if(j <= i)       //背景部分    
            {    
                w0 += pixelPro[j];    
                u0tmp += j * pixelPro[j];    
            }    
            else            //前景部分    
            {    
                w1 += pixelPro[j];    
                u1tmp += j * pixelPro[j];    
            }    
        }    
        u0 = u0tmp / w0;    
        u1 = u1tmp / w1;    
        deltaTmp = (float)(w0 *w1* pow((u0 - u1), 2)) ;    
        if(deltaTmp > deltaMax)    
        {    
            deltaMax = deltaTmp;    
            Threshold = i;    
        }    
    } 
	return Threshold;
}

int secondThreshold(Mat grayImg,Mat BW)
{
	int width = grayImg.cols;  
    int height = grayImg.rows;  
    int pixelCount[256] = {0};  
    float pixelPro[256] = {0};  
    int i, j, pixelSum = 0, threshold = 0;

	for(i=0; i<height; i++)  
		{  
			uchar* gray = grayImg.ptr<uchar>(i); 
			uchar* bw = BW.ptr<uchar>(i);
			for(j=0; j<width; j++)  
			{  
				if(bw[j]==255)
				{
					pixelCount[ gray[j] ]++;
					pixelSum ++;
				} 
			}  
		}

	// 每个灰度值的比例
	for(i=0; i<256; i++)  
    {  
        pixelPro[i] = (float)pixelCount[i] / pixelSum;  
    }

	// 计算最大类间方差  
    float w0, w1, u0tmp, u1tmp, u0, u1, deltaTmp, deltaMax = 0;    
    for(i = 0; i < 256; i++)    
    {    
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = deltaTmp = 0;    
        for(j = 0; j < 256; j++)    
        {    
            if(j <= i)       //背景部分    
            {    
                w0 += pixelPro[j];    
                u0tmp += j * pixelPro[j];    
            }    
            else            //前景部分    
            {    
                w1 += pixelPro[j];    
                u1tmp += j * pixelPro[j];    
            }    
        }    
        u0 = u0tmp / w0;    
        u1 = u1tmp / w1;    
        deltaTmp = (float)(w0 *w1* pow((u0 - u1), 2)) ;    
        if(deltaTmp > deltaMax)    
        {    
            deltaMax = deltaTmp;    
            threshold = i;    
        }    
    } 
	return threshold;
}

void tsbh(Mat &imgTrans,vector<Point2f> Points,int &transAveCols,int &aveCols)
{
	int transCols = 0;
	int cols = 0;
	int num = 0;
	int num1 = 0;
	vector<Point2f>not_a_rect,is_a_rect;
	Mat warp_mat = cvCreateMat(3,3,CV_32FC1);
	//定义输入的边界点
	not_a_rect.push_back(Point(320-110,0)); //303-150
	not_a_rect.push_back(Point(378-110,0)); //383-150
	not_a_rect.push_back(Point(175-110,110)); //175-150
	not_a_rect.push_back(Point(521-110,110)); //565-150
	//定义输出的边界点
	is_a_rect.push_back(Point(65,0)); 
	is_a_rect.push_back(Point(411,0)); //390
	is_a_rect.push_back(Point(65,110));
	is_a_rect.push_back(Point(411,110));
	
	warp_mat = getPerspectiveTransform(not_a_rect,is_a_rect);
	vector<Point2f> PointsTrans;
	perspectiveTransform( Points, PointsTrans, warp_mat);
	size_t size = PointsTrans.size();
	if(size > 0 )
	{
        for(unsigned int i=0; i<size-1; i++ )
		{
			int y = PointsTrans[i].y;
			int x = PointsTrans[i].x+70;
			uchar* p = imgTrans.ptr<uchar>(y);
			if(x>0 && x<640)
			{
				p[x] = 255;
			    if(y>39 && y < 61)
				{
					transCols = transCols + x;
					num++;
				}
			}
			if(Points[i].x > 0 && Points[i].x < 640)
			{
				if(Points[i].y > 81 && Points[i].y < 103)
				{
					cols = cols + Points[i].x;
					num1++;
				}
			}
		}
		if(num > 0)
		{
		    transAveCols = transCols / num;
		}
		if(num1 > 0)
		{
			aveCols = cols / num1;
		}
	}

	//cout<<warp_mat<<endl;
	//warpPerspective(frame_roi,frame_tsbh,warp_mat,Size(460,110),INTER_LINEAR, BORDER_CONSTANT);
}





#endif 
