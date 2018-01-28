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

    //int candi_num=0;			//��ѡ���ص�ĸ���
    //int blank_num=0;			//��ѡ����汳�����ص�ĸ���
	vector<int> rowcandi;		//���ÿһ�еĳ�ʼ��ѡ��
    unsigned int width_min,width_max;	//ÿһ��ɨ��������
    unsigned int dep_left,dep_right;		//������һ֡�Ƿ����˫����ȷ�����˳��ķ�Χ
	if(isDoubleLane)			//���һ��������������ߣ����˳���Χ��С
	{
		dep_left=-10;
		dep_right=10;
	}
	else						//���������˫�ߣ����˳���Χ�ϴ���Ϊ�����Χһֱ��С���Ҳ���˫��
	{
		dep_left=-60;
		dep_right=10;
	}
	for(int i=0;i<BW.rows;i++)
	{
		if(i<80)				//Զ�ӳ��Ŀ������
		{
			width_min=2;
			width_max=15;
		}
		else					//���ӳ��Ŀ������
		{
			width_min=10;
			width_max=30;
		}
		uchar* data=BW.ptr<uchar>(i);
		for(int j=boundary;j<BW.cols;j++)			//��ȡ����ɫ���ص�
		{
			if(data[j]==255)
			{
				rowcandi.push_back(j);
			}
		}
        unsigned int startIndex=0,stopIndex=0;				//ÿһ�ε���ʼ����
		size_t size=rowcandi.size();
		//if( i == 1,size > 50)                       //�ж���ֵ�ָ��Ƿ���ʣ�true���ʾ��Ҫ�л���ֵģʽ
		//{
		//	thresholdFlag = true;
		//}
		if(size>3)									//��ɫ���ص���������ֵ�Ž��зֶΣ������С�������ڳ���ȱʧ���ҳ������
		{
			for(size_t k=0;k<size-1;k++)
			{
				if(rowcandi[k+1]-rowcandi[k]>3) 
				{
					stopIndex=k;
					if(stopIndex-startIndex<=width_max&&stopIndex-startIndex>=width_min)			//��ȷ���������������Ƿ����һ֡ƫ�����ȷ���Ƿ���
					{
						if(frame_count==1)
						{
							scanPoints.push_back(Point2f(i,rowcandi[startIndex]));			//��һ֡����û����һ֡��Ϣ��ֱ�ӱ���
							stopIndex=0;
							break;
						}
						else																//����֡������һ֡ƫ�����ĵ��򲻱�����������
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
			if(stopIndex==rowcandi.size()-1)			//���һ�β�����������ͬ�ķ������д���
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
	vector<int> rowcandi;		//���ÿһ�еĳ�ʼ��ѡ��
	int width_min,width_max;	//ÿһ��ɨ��������
	int dep_left,dep_right;		//������һ֡�Ƿ����˫����ȷ�����˳��ķ�Χ
	if(isDoubleLane)			//���һ��������������ߣ����˳���Χ��С
	{
		dep_left=-10;
		dep_right=10;
	}
	else						//���������˫�ߣ����˳���Χ�ϴ���Ϊ�����Χһֱ��С���Ҳ���˫��
	{
		dep_left=-60;
		dep_right=10;
	}

	for(int i=0;i<BW.rows;i++)
	{
		if(i<BW.rows/2)			//Զ�ӳ��Ŀ������
		{
			width_min=2;
			width_max=15;
		}
		else					//���ӳ��Ŀ������
		{
			width_min=8;
			width_max=30;
		}
		uchar* data=BW.ptr<uchar>(i);
		for(int j=0;j<boundary;j++)		//��ȡ����ɫ���ص�
		{
			if(data[j]==255)
			{
				rowcandi.push_back(j);
			}
		}
		size_t size=rowcandi.size();
		//if( i == 1,size > 30)                       //�ж���ֵ�ָ��Ƿ���ʣ�true���ʾ��Ҫ�л���ֵģʽ
		//{
		//	left_thresholdFlag = true;
		//}
		int startIndex=size-1,stopIndex=size-1;		//ÿһ�ε���ʼ����
		if(size>3)
		{
			for(size_t k=size-1;k>0;k--)
			{
				if(rowcandi[k]-rowcandi[k-1]>3)		//��ɫ���ص���������ֵ�Ž��зֶΣ������С�������ڳ���ȱʧ���ҳ������
				{
					stopIndex=k;
					if(startIndex-stopIndex<=width_max&&startIndex-stopIndex>=width_min)		//��ȷ���������������Ƿ����һ֡ƫ�����ȷ���Ƿ���
					{
						if(frame_count==1)														//��һ֡����û����һ֡��Ϣ��ֱ�ӱ���
						{
							leftscanPoints.push_back(Point2f(i,rowcandi[startIndex]));
							stopIndex=size-1;
							break;
						}
						else
						{
							if(last_dash_points.size()>0 && last_left_points.size()>0)
							{
								if(((rowcandi[startIndex]-last_dash_points[i].x)<dep_right)&&((rowcandi[startIndex]-last_left_points[i].x)>dep_left)) //ͨ������֡��õ��������һ֡����бȽϣ�����ָ����Χɸ�����ŵ�
								{
									leftscanPoints.push_back(Point2f(i,rowcandi[startIndex]));		//����֡������һ֡ƫ�����ĵ��򲻱�����������
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
				if(startIndex-stopIndex<=width_max&&startIndex-stopIndex>=width_min)			//���һ�β�����������ͬ�ķ������д���
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
	vector<Point2i>::iterator it;						//���ɨ��õ�����Ч��
	vector<Point2f> lanePoints;
	size_t size=scanPoints.size();
	int upperIndex=size-1;						//��¼ÿһ�ε����ص�������
	int belowIndex=size-1;						//��¼ÿһ�ε����ص�������
	int temp=0;
	Mat polyFactor;								//��¼���е�һ�ε�ֱ����ϲ���
	double p0=0,p1=0;							//��������ihou
	for(size_t i=size-1;i>0;i--)
	{
		temp=scanPoints[i-1].y-scanPoints[i].y;
		if(temp>3||temp<-2)						//�����еĺ������ֵ����ʱ���д���
		{
			upperIndex=i;
			if((belowIndex-upperIndex<5))		//��һ�εĵ����
			{
				for (int j=upperIndex; j<=belowIndex; j++)
				{
					scanPoints[j].x=0;
				}
				if(p1!=0&&abs(scanPoints[i-1].x*p1+p0-scanPoints[i-1].y)>20)			//��ͻ������������жϣ���ȷ����ζ�������������
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
				polynomial_curve_fit(lanePoints, 1, polyFactor);			//�����������㹻ʱ��ͨ���Դ˶ε���ϣ�ȷ��ͻ�䴦������Ƿ�Ϊ���
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
		else															//����û��ͻ��ʱ�Ĵ�����
		{
			if((upperIndex!=belowIndex))								//����������ʱ��ÿһ���������ж�
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
	if((belowIndex-upperIndex<5))		//���һ�εĵ����
	{
		for (int j=upperIndex; j<=belowIndex; j++)
		{
			scanPoints[j].x=0;
		}
	}
	it = scanPoints.begin();			//���������Ҫ��ĵ�
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

void Filter1(vector<Point2i> &scanPoints)				//�Ҳ೵���ߵ��˲���һ��	
{//ȥ��������ֵ�仯��С�����

    unsigned int startIndex = 0;							//ÿһ�ε���ʼ����
    unsigned int stopIndex = 0;							//ÿһ�ε���ֹ����
    int temp = 0;
    int temp1=0;
    unsigned int seq_min=0;								//��̵ĵ����еĳ��ȣ�����ʱֱ���˳������ӳ�����Ϊ8��Զ�ӳ�����Ϊ4
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
			if (stopIndex-startIndex < seq_min)						//��Գ����ڵĳ�������㣬���������������ʱ���˳�
			{
                for (unsigned int j=startIndex; j<=stopIndex; j++)
				{
					scanPoints[j].y = 0;
				}
			}
			startIndex = i+1;
		}
		else if(temp > 2)										//��Գ���ȱʧ
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
	if (stopIndex-startIndex < 5)							//������һ�ε����ʱ���˳�������̻����ȵ�Ӱ��
	{
        for (unsigned int j=startIndex; j<=stopIndex; j++)
		{
			scanPoints[j].y = 0;
		}
	}

	vector<Point2i>::iterator it;				//�˳����
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
			if(temp>2&&temp1<2)							//�жϳ������Ƿ����ȱʧ
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

void leftFilter1(vector<Point2i> &scanPoints)				//��೵���ߵ��˲���һ��	
{//ȥ��������ֵ�仯��С�����

    unsigned int startIndex = 0;							//ÿһ�ε���ʼ����
    unsigned int stopIndex = 0;							//ÿһ�ε���ֹ����
	int temp = 0;
	int temp1=0;
    unsigned int seq_min=0;								//��̵ĵ����еĳ��ȣ�����ʱֱ���˳������ӳ�����Ϊ8��Զ�ӳ�����Ϊ4
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
			if (stopIndex-startIndex < seq_min)						//��Գ����ڵĳ�������㣬���������������ʱ���˳�
			{
                for (unsigned int j=startIndex; j<=stopIndex; j++)
				{
					scanPoints[j].y = 0;
				}
			}
			startIndex = i+1;
		}
		else if(temp > 2)										//��Գ���ȱʧ
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
	if (stopIndex-startIndex < 5)							//������һ�ε����ʱ���˳�������̻����ȵ�Ӱ��
	{
        for (unsigned int j=startIndex; j<=stopIndex; j++)
		{
			scanPoints[j].y = 0;
		}
	}

	vector<Point2i>::iterator it;				//�˳����
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
			if(temp<-2&&temp1<2)							//�жϳ������Ƿ����ȱʧ
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

void Round1Filter(vector<Point2i> &scanPoints)				//�Ҳ೵���ߵ��˲���һ��
	{//ȥ����������ֵ�仯�ϴ�Ĺ������
		int temp1 = scanPoints[0].y;
		size_t size = scanPoints.size();

		for (size_t i=0; i<size-1; i++)
		{
			if ((scanPoints[i+1].y-temp1) <0)				//ȥ���������С�ĵ�
			{
				temp1 = scanPoints[i+1].y;
				scanPoints[i+1].y = 0;						//����������0
			}
			else if ((scanPoints[i+1].y - temp1) > 3)		//ȥ����������̫��ĵ�
			{
				temp1 = scanPoints[i+1].y;
				scanPoints[i+1].y = 0;
			}
			else 
			{
				temp1 = scanPoints[i+1].y;
			}
		}

		vector<Point2i>::iterator it;						//������
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

void Round2Filter(vector<Point2i> &scanPoints)				//�Ҳ೵���ߵ��˲��ڶ���	
{//ȥ��������ֵ�仯��С�����

    unsigned int startIndex = 0;							//ÿһ�ε���ʼ����
    unsigned int stopIndex = 0;							//ÿһ�ε���ֹ����
	int temp = 0;
    unsigned int seq_min=0;								//��̵ĵ����еĳ��ȣ�����ʱֱ���˳������ӳ�����Ϊ8��Զ�ӳ�����Ϊ4
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
		if (temp < -3)                                   //�����ѭ��˵��ɨ��㲻����
		{
			stopIndex = i;
			if (stopIndex-startIndex < seq_min)						//��Գ����ڵĳ�������㣬���������������ʱ���˳�
			{
                for (unsigned int j=startIndex; j<=stopIndex; j++)
				{
					scanPoints[j].y = 0;
				}
			}
			startIndex = i+1;
		}
		else if(temp > 3)										//��Գ���ȱʧ���������߲�����
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
	if (stopIndex-startIndex < 5)							//������һ�ε����ʱ���˳�������̻����ȵ�Ӱ��
	{
        for (unsigned int j=startIndex; j<=stopIndex; j++)
		{
			scanPoints[j].y = 0;
		}
	}

	vector<Point2i>::iterator it;				//�˳����
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
{//�������������Ⱥ

	int num = 0;   //��¼���������Ⱥ������
	int startIndex = 0;
	int stopIndex = 0;
	bool flag = false;				//���һ���Ƿ��Ǵ�ͻ��С����ʼ�ı��λ
	int temp;
	vector<Point2f> lanePoints;		// ��ʱ����������������ϵĵ㣬������ʵ�߷���
	Mat polyFactor;
	size_t size = scanPoints.size();
	int seq_min=0;								//��̵ĵ����еĳ��ȣ�����ʱֱ���˳������ӳ�����Ϊ8��Զ�ӳ�����Ϊ4

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
		if (temp<=-3)									//�벽������ƣ����϶�ÿһ�εķ����ǩ��������㣬���˳���������Ч�㣬�����
		{
			stopIndex = i;
			if (stopIndex-startIndex < seq_min)
			{
				for (int j=startIndex; j<=stopIndex; j++)			//�˳����������
				{
					scanPoints[j].y = 0;
				}
			}
			else
			{
				flag = true;							//ͻ��С����1
			}

			startIndex = i+1;							//��������
		}
		else if(temp > 4)
		{
			stopIndex = i;
			//flag = false;								//������ͻ���ʱ��һ����flag��Ϊfalse����Ҳ�п�������Ϊ���߶ε�ȱʧ���µ�
			if (stopIndex-startIndex < seq_min)
			{
				for (int j=startIndex; j<=stopIndex; j++)			//�˳����������
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

					Mat polyFactor;                                         //������ߵĲ�������
					polynomial_curve_fit(lanePoints, 1, polyFactor);
					double p0 = polyFactor.at<double>(0,0);
					double p1 = polyFactor.at<double>(1,0);
					double result = p0 + p1*scanPoints[i+1].x;

					if (((scanPoints[i+1].x-scanPoints[i].x<30) && (scanPoints[i+1].y-result>6)) || ((scanPoints[i+1].x-scanPoints[i].x>30) && (scanPoints[i+1].y-result>30)))
					{
						num += 1;								//�Ҳ���ߺ�ѡ���ּ�1����ѡ���ִ���2������Ϊ����˫��
						startIndex = i+1;
						flag = false;							//���ж�Ϊ����˫�������������ͻ��ʱ����flag��Ϊfalse
					}
					else if (((scanPoints[i+1].x-scanPoints[i].x<30) && (scanPoints[i+1].y-result<-6)) || ((scanPoints[i+1].x-scanPoints[i].x>30) && (scanPoints[i+1].y-result<-30)))
					{
						startIndex = i+1;
						flag = true;							//���ж�Ϊ����˫�������������ͻ��ʱ����flag��Ϊfalse��ԭΪtrue��
					}
					else
					{
						stopIndex = i+1;						//��ǰһ����Ͻ���ͺ�һ�εĽӽ�����ʱ��Ϊ�����߻�����ʵ������ĳ��ԭ�����м�ȱʧ
					}
					lanePoints.clear();
				}
				else
				{
					num += 1;								//�Ҳ���ߺ�ѡ���ּ�1����ѡ���ִ���2������Ϊ����˫��
					startIndex = i+1;
					flag = false;							//���ж�Ϊ����˫�������������ͻ��ʱ����flag��Ϊfalse
				}
			}	
		}
		else 
		{
			if (scanPoints[i+1].x-scanPoints[i].x > 10)				//��������仯������Χ֮�ڣ�Ҫ��Գ����Ƿ�ȷʵ���������һ�ν����ж�
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

				if ((scanPoints[i+1].y-result) < -6)						//��һ�κ���һ��֮����Ϊʵ�ߵ�ȱ����ʵ�ߺ�����֮��ĺ�������С
				{															//�������һ�κ���һ��֮��ľ�������ж�
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
	{					 //������һ���Ǵ�ͻ��С����ʼ�ģ�����Ϊ�˶�Ҳ�����߶�
		num += 1;
	}

	vector<Point2i>::iterator it;			//�˳����
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
		
	bool isDoubleFlag = false;			//�Ƿ����˫�ߵı�־
	startIndex = 0;						//��������һ����ȫһ�£�ֻ������˵����洢
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
			seq_min=8;                            //ԭ��4
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
							dashLanePoints.push_back(scanPoints[j]);			//����ѡ��,������ʱ����
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
							for(k=scanPoints[j].y+(scanPoints[j].x/10);k<r_boundary;k++)							//k=scanPoints[j].y+10�����10�Ǽӵĳ����ߵĿ�ȣ���ֹ����������ʱ����ֻ��1���´���
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
					flag = true;							//���ж�Ϊ����˫�������������ͻ��ʱ����flag��Ϊtrue
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
						dashLanePoints.push_back(scanPoints[j]);			//����ѡ��,������ʱ����
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
						for(k=scanPoints[j].y+(scanPoints[j].x/10);k<r_boundary;k++)							//k=scanPoints[j].y+10�����10�Ǽӵĳ����ߵĿ�ȣ���ֹ����������ʱ����ֻ��1���´���
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

	if (flag)						//���flagΪ1��˵�����һ���Ǵ�ͻ��С����ʼ�ģ�Ҳ������ӵ��ڲ��
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
			for(k=scanPoints[j].y+10;k<r_boundary;k++)			//�ڲ��е㣬�������������������Ե���в���
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

		
	//�ж��Ƿ����˫��
	if (num>1 || (isDoubleLaneInLastFrame && num==1))			//���Ƿ����˫�߽����ж�
	{
		/*float dashMid = (dashLanePoints[0].y + dashLanePoints[dashLanePoints.size()-1].y) / 2;				//�˲����Ƿ�ֹ��⵽��ĳ������������
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

		//if (abs(a0-b0+(a1-b1)*ROI_HEIGH/2)>40)		//�����ʵ�߼�������˵�����ʵ�߲��Ǳ�����
		//{
		//	isDoubleFlag = false;

		//	it = scanPoints.begin();						//ɾ��solidPoints
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

	it = scanPoints.begin();						//�˳����
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
	
	if(scanPoints.size()>1)				//��Ϊ�ֶ���������˲��䣬�����ٽ������μ��˲�
	{ 
		Round1Filter(scanPoints);
	}
	if(scanPoints.size()>1)
	{ 
		Round2Filter(scanPoints);
	}	
	if(scanPoints.size()>0)						//������ౣ��
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
	bool isDoubleLane=false;					//�Բ���1-3��������
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
//	/*�Ժ���������߱�С̫��ĵ��������ȥ��*/
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
//	/*          ��С���ڵ�������ȥ��          */
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
//	/*��ÿ�γ��Ƚ����жϣ�ȥ������С��6�ģ������Ƿ���Ҫ��*/
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

//	vector<Point2i> temp_scanPoints;				//������践��1��160�Ľ���������һ����ʱ������ţ�0��1��160��
//	for(size_t i=0;i<size;i++)
//	{
//		if(i<0)
//		{
//			if(predict_label)						//����Ԥ�⣬���ڶ������Ͽ��ƥ��Ŀ��Ƶ㣬������������
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


//	vector<Point2f> upperPoints;			//�������꽫���Ϊ���ӳ���Զ�ӳ�
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
//		//if(scanPoints[i].y==0)						//��Ч�㣬�����
//		//	belowPoints.push_back(Point2i(scanPoints[i].x,leftPointsMatched[i].x));
//		//else
//		//	belowPoints.push_back(Point2i(scanPoints[i].x,(scanPoints[i].y+leftPointsMatched[i].x)/2));
//		belowPoints.push_back(scanPoints[i]);
//	}
//	for(int i=0;i<10;i++)
//		upperPoints.push_back(belowPoints[i]);
//	for(size_t i=0;i<upperPoints.size();i++)
//	{
//		if(upperPoints[i].y!=0)						//��Ч�㣬�����
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
//	bool swapFlag = false;        //�����־
//	int times = 0;               //��¼���������Ƿ����㳵�����
//	int temp = 0;                    //�����������ص����ز�
//	int frontPoint = 0;

//	if (frameNum == 1)
//	{
//		if (leftOrRight == RIGHT)               //ɨ���Ҳ�Ŀ���
//		{
//			for (int i=0; i<height/2; i++)                 //�ȶ�ROI�ϰ벿ɨ��
//			{
//				for (int j=midPoint+1; j<width-1; j++)
//				{
//					temp = gray.at<uchar>(i,j) - gray.at<uchar>(i,j-1);
//					if (temp>FRONT_LANE_PIXEL_SWAP_IN_ZONE1 && !swapFlag)                       //������������
//					{
//						frontPoint = j;
//						swapFlag = true;
//						times = 1;
//					}
//					else if (temp<REAR_LANE_PIXEL_SWAP_IN_ZONE1 && swapFlag)                 //��������һ�ߵ�����
//					{
//						if (times>=ZONE1_MIN_LANE_WIDTH && times<=ZONE1_MAX_LANE_WIDTH)
//						{
//							scanPoints.push_back(Point2i(i, frontPoint));        //x��y����λ�ã��������
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

//		if (leftOrRight == LEFT)                   //����߽���ɨ��
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
//	else                    //���ǵ�һ֡
//	{
//		if (leftOrRight == RIGHT)               //ɨ���Ҳ�Ŀ���
//		{
//			for (int i=0; i<height/2; i++)                 //�ȶ�ROI�ϰ벿ɨ��
//			{
//				for (int j=scanInitPoint+5; j<width-1; j++)
//				{
//					temp = gray.at<uchar>(i,j) - gray.at<uchar>(i,j-1);
//					if (temp>FRONT_LANE_PIXEL_SWAP_IN_ZONE1 && !swapFlag)                       //������������
//					{
//						frontPoint = j;
//						swapFlag = true;
//						times = 1;
//					}
//					else if (temp<REAR_LANE_PIXEL_SWAP_IN_ZONE1 && swapFlag)                 //��������һ�ߵ�����
//					{
//						if (times>=ZONE1_MIN_LANE_WIDTH && times<=ZONE1_MAX_LANE_WIDTH)
//						{
//							scanPoints.push_back(Point2i(i, frontPoint));        //x��y����λ�ã��������
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

//		if (leftOrRight == LEFT)                   //����߽���ɨ��
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
	int num = 0;   //��¼���������Ⱥ������
	int startIndex = 0;
	int stopIndex = 0;
	bool flag = false;				//���һ���Ƿ��Ǵ�ͻ��С����ʼ�ı��
	int temp;						//��������ĺ������
	int temp1;						//�������ŵ��������
	vector<Point2f> lanePoints;		// ��ʱ����������������ϵĵ㣬������ʵ�߷���
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
	{					 //������һ���Ǵ�ͻ��С����ʼ�ģ�����Ϊ�˶�Ҳ�����߶�
		num += 1;
	}

	bool isDoubleFlag = false;			//�Ƿ����˫�ߵı�־
	startIndex = 0;						//��������һ����ȫһ�£�ֻ������˵����洢
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
						dashLanePoints.push_back(scanPoints[j]);			//����ѡ��,������ʱ����
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
						for(k=scanPoints[j].y+((int)(last_dash_point[scanPoints[j].x].x)+10)+(scanPoints[j].x/10);k<r_boundary;k++)							//k=scanPoints[j].y+10�����10�Ǽӵĳ����ߵĿ�ȣ���ֹ����������ʱ����ֻ��1���´���
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
							dashLanePoints.push_back(scanPoints[j]);			//����ѡ��,������ʱ����
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
							for(k=scanPoints[j].y+((int)(last_dash_point[scanPoints[j].x].x)+10)+(scanPoints[j].x/10);k<r_boundary;k++)							//k=scanPoints[j].y+10�����10�Ǽӵĳ����ߵĿ�ȣ���ֹ����������ʱ����ֻ��1���´���
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
							dashLanePoints.push_back(scanPoints[j]);			//����ѡ��,������ʱ����
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
							for(k=scanPoints[j].y+((int)(last_dash_point[scanPoints[j].x].x)+10)+(scanPoints[j].x/10);k<r_boundary;k++)							//k=scanPoints[j].y+10�����10�Ǽӵĳ����ߵĿ�ȣ���ֹ����������ʱ����ֻ��1���´���
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
							dashLanePoints.push_back(scanPoints[j]);			//����ѡ��,������ʱ����
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
							for(k=scanPoints[j].y+((int)(last_dash_point[scanPoints[j].x].x)+10)+(scanPoints[j].x/10);k<r_boundary;k++)							//k=scanPoints[j].y+10�����10�Ǽӵĳ����ߵĿ�ȣ���ֹ����������ʱ����ֻ��1���´���
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
	if (flag)						//���flagΪ1��˵�����һ���Ǵ�ͻ��С����ʼ�ģ�Ҳ������ӵ��ڲ��
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
			for(k=scanPoints[j].y+((int)(last_dash_point[scanPoints[j].x].x)+10)+(scanPoints[j].x/10);k<r_boundary;k++)							//k=scanPoints[j].y+10�����10�Ǽӵĳ����ߵĿ�ȣ���ֹ����������ʱ����ֻ��1���´���
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
	//�ж��Ƿ����˫��
	if (num>1 || (isDoubleLaneInLastFrame && num==1))			//���Ƿ����˫�߽����ж�
	{
		isDoubleFlag = true;
		//doubleLaneNum += 1;
	}			

	vector<Point2i>::iterator it;						//������
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
	
	if(scanPoints.size()>1)				//��Ϊ�ֶ���������˲��䣬�����ٽ������μ��˲�
	{ 
		Round1Filter(scanPoints);
	}
	if(scanPoints.size()>1)
	{ 
		Round2Filter(scanPoints);
	}	
	if(scanPoints.size()>0)						//������ౣ��
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
    int num = 0;   //��¼���������Ⱥ������
    int startIndex = 0;
    int stopIndex = 0;
    bool flag = false;				//���һ���Ƿ��Ǵ�ͻ��󴦿�ʼ�ı��
    int temp;						//��������ĺ������
    int temp1;						//�������ŵ��������
    vector<Point2f> lanePoints;		// ��ʱ����������������ϵĵ㣬������ʵ�߷���
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

    bool isDoubleFlag = false;			//�Ƿ����˫�ߵı�־
    startIndex = 0;						//��������һ����ȫһ�£�ֻ������˵����洢
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
                        dashLanePoints.push_back(scanPoints[j]);			//�ڲ��ѡ��,������ʱ����
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
                        for(k=scanPoints[j].y-(scanPoints[j].x/10)+((int)(left_last_solid_point[scanPoints[j].x].x)+10);k>r_boundary;k--)							//k=scanPoints[j].y+10�����10�Ǽӵĳ����ߵĿ�ȣ���ֹ����������ʱ����ֻ��1���´���
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
                            dashLanePoints.push_back(scanPoints[j]);			//����ѡ��,������ʱ����
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
                            for(k=scanPoints[j].y-(scanPoints[j].x/10)+((int)(left_last_solid_point[scanPoints[j].x].x)+10);k>r_boundary;k--)							//k=scanPoints[j].y+10�����10�Ǽӵĳ����ߵĿ�ȣ���ֹ����������ʱ����ֻ��1���´���
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
                            dashLanePoints.push_back(scanPoints[j]);			//����ѡ��,������ʱ����
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
                            for(k=scanPoints[j].y-(scanPoints[j].x/10)+((int)(left_last_solid_point[scanPoints[j].x].x)+10);k>r_boundary;k--)							//k=scanPoints[j].y+10�����10�Ǽӵĳ����ߵĿ�ȣ���ֹ����������ʱ����ֻ��1���´���
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
    if (flag)						//���flagΪ1��˵�����һ���Ǵ�ͻ��С����ʼ�ģ�Ҳ������ӵ��ڲ��
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
            for(k=scanPoints[j].y-(scanPoints[j].x/10)+((int)(left_last_solid_point[scanPoints[j].x].x)+10);k>r_boundary;k--)							//k=scanPoints[j].y+10�����10�Ǽӵĳ����ߵĿ�ȣ���ֹ����������ʱ����ֻ��1���´���
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
    //�ж��Ƿ����˫��
    if (num>1 || (isDoubleLaneInLastFrame && num==1))			//���Ƿ����˫�߽����ж�
    {
        isDoubleFlag = true;
        //doubleLaneNum += 1;
    }

    vector<Point2i>::iterator it;						//������
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
	
    if(scanPoints.size()>1)				//��Ϊ�ֶ���������˲��䣬�����ٽ������μ��˲�
    {
        Round1Filter(scanPoints);
    }
    if(scanPoints.size()>1)
    {
        Round2Filter(scanPoints);
    }
    if(scanPoints.size()>0)						//������ౣ��
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
		for(int j=0;j<left_boundary;j++)			//��ȡ����ɫ���ص�
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
		//for(int j=0;j<left_boundary;j++)			//��ȡ����ɫ���ص�
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
    // �Լ�ʵ�ַ���
    int width = src.cols;  
    int height = src.rows;  
    int pixelCount[256] = {0};  
    float pixelPro[256] = {0};  
    int i, j, pixelSum = width * height, Threshold = 0;  
  
    // ͳ��ֱ��ͼ  
    for(i=0; i<height; i++)  
    {  
        uchar* g_ptr = src.ptr<uchar>(i);  
        for(j=0; j<width; j++)  
        {  
            pixelCount[ g_ptr[j] ]++;  
        }  
    }  
  
    // ÿ���Ҷ�ֵ�ı���  
    for(i=0; i<256; i++)  
    {  
        pixelPro[i] = (float)pixelCount[i] / pixelSum;  
    }  
  
    // ���������䷽��  
    float w0, w1, u0tmp, u1tmp, u0, u1, deltaTmp, deltaMax = 0;    
    for(i = 0; i < 256; i++)    
    {    
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = deltaTmp = 0;    
        for(j = 0; j < 256; j++)    
        {    
            if(j <= i)       //��������    
            {    
                w0 += pixelPro[j];    
                u0tmp += j * pixelPro[j];    
            }    
            else            //ǰ������    
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

	// ÿ���Ҷ�ֵ�ı���
	for(i=0; i<256; i++)  
    {  
        pixelPro[i] = (float)pixelCount[i] / pixelSum;  
    }

	// ���������䷽��  
    float w0, w1, u0tmp, u1tmp, u0, u1, deltaTmp, deltaMax = 0;    
    for(i = 0; i < 256; i++)    
    {    
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = deltaTmp = 0;    
        for(j = 0; j < 256; j++)    
        {    
            if(j <= i)       //��������    
            {    
                w0 += pixelPro[j];    
                u0tmp += j * pixelPro[j];    
            }    
            else            //ǰ������    
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
	//��������ı߽��
	not_a_rect.push_back(Point(320-110,0)); //303-150
	not_a_rect.push_back(Point(378-110,0)); //383-150
	not_a_rect.push_back(Point(175-110,110)); //175-150
	not_a_rect.push_back(Point(521-110,110)); //565-150
	//��������ı߽��
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
