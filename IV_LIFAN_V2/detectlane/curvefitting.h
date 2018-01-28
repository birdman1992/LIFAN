#ifndef CURVEFITTING_H
#define CURVEFITTING_H
#pragma once
#include<iostream>
#include<vector>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<math.h>

using namespace std;
using namespace cv;

bool polynomial_curve_fit(vector<Point2f>& key_point, int n, cv::Mat& A)					//最小二乘拟合，参数为拟合点，拟合次数，输出矩阵
{  
    //Number of key points  
    int N = key_point.size();  
  
    //构造矩阵X  
    cv::Mat X = cv::Mat::zeros(n + 1, n + 1, CV_64FC1);  
    for (int i = 0; i < n + 1; i++)  
    {  
        for (int j = 0; j < n + 1; j++)  
        {  
            for (int k = 0; k < N; k++)  
            {  
                X.at<double>(i, j) = X.at<double>(i, j) +  
                    std::pow(key_point[k].x, i + j);  
            }  
        }  
    }  
  
    //构造矩阵Y  
    cv::Mat Y = cv::Mat::zeros(n + 1, 1, CV_64FC1);  
    for (int i = 0; i < n + 1; i++)  
    {  
        for (int k = 0; k < N; k++)  
        {  
            Y.at<double>(i, 0) = Y.at<double>(i, 0) +  
                std::pow(key_point[k].x, i) * key_point[k].y;  
        }  
    }  
  
    A = cv::Mat::zeros(n + 1, 1, CV_64FC1);  
    //求解矩阵A  
    cv::solve(X, Y, A, cv::DECOMP_LU);  
    return true;  
} 

void polynomial_fit_val(vector<Point2f>& fittedPoints, vector<int>& cor_seq, Mat &A)			//根据最小二乘的输出矩阵计算点序列的计算结果
{
    //int col=A.cols;
	int row=A.rows;

	for(size_t i=0;i<cor_seq.size();i++)
	{
		double cor_fitted=0;
		for(int j=0;j<row;j++)
		{
			cor_fitted+=A.at<double>(j,0)*pow((double)cor_seq[i],(double)j);			//根据多项式系数计算拟合值
		}
		fittedPoints.push_back(Point2f(cor_fitted,cor_seq[i]));
	}
}

void curvefitting(vector<Point2f>& fittedPoints,vector<Point2f>& lanePoints, int n)					//分段直线拟合
{
	//if(n!=1)
	//{
	//	vector<Point2f>::iterator it;						//清除扫描得到的无效点
	//	it = lanePoints.begin();
	//	for(int i=0;i<3;i++)
	//	{
	//		it = lanePoints.erase(it);
	//		it++;
	//	}
	//}
	size_t size=lanePoints.size();
	int sec=size/n;			
	int j;
	vector<Point2f> temp_points;	
	vector<int> temp_y_seq;			//每一段拟合的坐标序列
	vector<Point2f> temp_fitted_points;		//每一段拟合得到的点
	Mat temp_p;				//临时存放拟合参数的矩阵
	for(j=0;j<n;j++)
	{
		if(j==0)							
		{
			for(int k=0;k<sec;k++)
				temp_points.push_back(lanePoints[j*sec+k]);
			if(n==1)						//如果只拟合一段，则y坐标为1：end
			{
				for(int l=0;l<ROI_HEIGH;l++)
					temp_y_seq.push_back(l);
			}
			else
			{
				int stop_y=lanePoints[sec*(j+1)].x;
				int k=0;
				while(k<stop_y)
				{
					temp_y_seq.push_back(k);
					k++;
				}
			}
			//if(temp_points[temp_points.size()-1].x<35)
			if(temp_points[0].x<12&&temp_points[temp_points.size()-1].x<50)
			{
				if(n==1)
				{
					polynomial_curve_fit(temp_points,1,temp_p);					//计算拟合系数
				}
				else
				{
					polynomial_curve_fit(temp_points,3,temp_p);					//计算拟合系数
					//cout<<temp_p.at<double>(0,0)<<"  "<<temp_p.at<double>(1,0)<<"  "<<temp_p.at<double>(2,0)<<"  "<<temp_p.at<double>(3,0)<<endl;
				}
			}
			else
				polynomial_curve_fit(temp_points,1,temp_p);					//计算拟合系数
			// polynomial_curve_fit(temp_points,1,temp_p);					//计算拟合系数
			polynomial_fit_val(temp_fitted_points,temp_y_seq,temp_p);		//计算拟合结果
			
		}
		else if(j<n-1)
		{
			for(int k=0;k<sec;k++)
				temp_points.push_back(lanePoints[j*sec+k]);				//添加用于拟合的点
			int stop_y=lanePoints[sec*(j+1)].x;
			int k=lanePoints[j*sec].x;
			while(k<stop_y)
			{
				temp_y_seq.push_back(k);
				k++;
			}
			polynomial_curve_fit(temp_points,1,temp_p);					//计算拟合系数
			polynomial_fit_val(temp_fitted_points,temp_y_seq,temp_p);	//计算拟合值
		}
		else
		{
			for(size_t k=0;k<size-sec*(n-1);k++)						//添加用于拟合的点
				temp_points.push_back(lanePoints[j*sec+k]);		
			int stop_y=ROI_HEIGH;
			int k=lanePoints[j*sec].x;
			while(k<stop_y)
			{
				temp_y_seq.push_back(k);
				k++;
			}
			polynomial_curve_fit(temp_points,1,temp_p);					//计算拟合系数
			polynomial_fit_val(temp_fitted_points,temp_y_seq,temp_p);	//计算拟合值
		}
		size_t size1=temp_fitted_points.size();
		for(size_t k=0;k<size1;k++)							//将每一段的拟合结果push_back到输出vector中
			fittedPoints.push_back(temp_fitted_points[k]);
		temp_fitted_points.clear();
		temp_points.clear();
		temp_y_seq.clear();
	}
}

void curvefitting1(vector<Point2f>& fittedPoints,vector<Point2f>& lanePoints, int n)					//分段直线拟合
{
	bool flag;					
	size_t size=lanePoints.size();
	int sec=size/n;			
	int j;
	vector<Point2f> temp_points;	
	vector<int> temp_y_seq;			//每一段拟合的坐标序列
	vector<Point2f> temp_fitted_points;		//每一段拟合得到的点
	Mat temp_p;				//临时存放拟合参数的矩阵
    for(unsigned int i=size-1;i>3*size/4;i--)
	{
		temp_points.push_back(lanePoints[i]);
	}
	polynomial_curve_fit(temp_points,1,temp_p);	
	double p0 = temp_p.at<double>(0,0);
	double p1 = temp_p.at<double>(1,0);
	double result = p0 + p1*lanePoints[0].x;
	double dep=result-lanePoints[0].y;
	if(abs(dep)>5)
	{
		flag=true;
	}
	else
	{
		flag=false;
	}
	cout<<"dep="<<dep<<"  "<<"flag="<<flag<<endl;
	temp_points.clear();
	for(j=0;j<n;j++)
	{
		if(j==0)							
		{
			for(int k=0;k<sec;k++)
				temp_points.push_back(lanePoints[j*sec+k]);
			if(n==1)						//如果只拟合一段，则y坐标为1：end
			{
				for(int l=0;l<ROI_HEIGH;l++)
					temp_y_seq.push_back(l);
			}
			else
			{
				int stop_y=lanePoints[sec*(j+1)].x;
				int k=0;
				while(k<stop_y)
				{
					temp_y_seq.push_back(k);
					k++;
				}
			}
			//if(temp_points[temp_points.size()-1].x<35)
			if(temp_points[0].x<15&&temp_points[temp_points.size()-1].x<50)
			{
				if(n==1)
				{
					polynomial_curve_fit(temp_points,1,temp_p);					//计算拟合系数
				}
				else
				{
					if(flag)
						polynomial_curve_fit(temp_points,3,temp_p);					//计算拟合系数
					else
						polynomial_curve_fit(temp_points,1,temp_p);
				}
			}
			else
				polynomial_curve_fit(temp_points,1,temp_p);					//计算拟合系数
			// polynomial_curve_fit(temp_points,1,temp_p);					//计算拟合系数
			polynomial_fit_val(temp_fitted_points,temp_y_seq,temp_p);		//计算拟合结果
			
		}
		else if(j<n-1)
		{
			for(int k=0;k<sec;k++)
				temp_points.push_back(lanePoints[j*sec+k]);				//添加用于拟合的点
			int stop_y=lanePoints[sec*(j+1)].x;
			int k=lanePoints[j*sec].x;
			while(k<stop_y)
			{
				temp_y_seq.push_back(k);
				k++;
			}
			polynomial_curve_fit(temp_points,1,temp_p);					//计算拟合系数
			polynomial_fit_val(temp_fitted_points,temp_y_seq,temp_p);	//计算拟合值
		}
		else
		{
			for(size_t k=0;k<size-sec*(n-1);k++)						//添加用于拟合的点
				temp_points.push_back(lanePoints[j*sec+k]);		
			int stop_y=ROI_HEIGH;
			int k=lanePoints[j*sec].x;
			while(k<stop_y)
			{
				temp_y_seq.push_back(k);
				k++;
			}
			polynomial_curve_fit(temp_points,1,temp_p);					//计算拟合系数
			polynomial_fit_val(temp_fitted_points,temp_y_seq,temp_p);	//计算拟合值
		}
		size_t size1=temp_fitted_points.size();
		for(size_t k=0;k<size1;k++)							//将每一段的拟合结果push_back到输出vector中
			fittedPoints.push_back(temp_fitted_points[k]);
		temp_fitted_points.clear();
		temp_points.clear();
		temp_y_seq.clear();
	}
}
void curvefitting_solid(vector<Point2f>& fittedPoints,vector<Point2f>& lanePoints)			
{
	size_t size=lanePoints.size();
	vector<Point2f> temp_points;
	//vector<Point2f> temp_points2;
	vector<int> temp_y_seq;							//拟合用的y序列
	vector<Point2f> temp_fitted_points;				//保存一段的拟合结果
	Mat temp_p;
    for(unsigned int i=0;i<size/2;i++)								//上面一半的点用三次曲线拟合
		temp_points.push_back(lanePoints[i]);
	for(int i=0;i<=lanePoints[size/2+1].x;i++)
		temp_y_seq.push_back(i);
	polynomial_curve_fit(temp_points,3,temp_p);
	polynomial_fit_val(temp_fitted_points,temp_y_seq,temp_p);
	size_t size1=temp_fitted_points.size();
	for(size_t k=0;k<size1;k++)
		fittedPoints.push_back(temp_fitted_points[k]);			//输出一段的拟合结果
	temp_fitted_points.clear();
	temp_points.clear();
	temp_y_seq.clear();
    for(unsigned int i=size/2;i<size-5;i++)							//去掉最后的5个点（避免趋势错误），剩余20个点做直线拟合
		temp_points.push_back(lanePoints[i]);
	for(int i=lanePoints[size/2+1].x+1;i<ROI_HEIGH;i++)
		temp_y_seq.push_back(i);
	polynomial_curve_fit(temp_points,1,temp_p);
	polynomial_fit_val(temp_fitted_points,temp_y_seq,temp_p);
	size_t size2=temp_fitted_points.size();
	for(size_t k=0;k<size2;k++)
		fittedPoints.push_back(temp_fitted_points[k]);			//输出第二段的拟合结果
}
float findVP(vector<Point2f> &leftLanePoints,vector<Point2f> &solidLanePoints,vector<Point2f> &solidLaneFitted,vector<Point2f> &leftLaneFitted)
{
	vector<Point2f> temp_leftPoints1;
	vector<Point2f> temp_leftPoints2;
	vector<Point2f> temp_rightPoints1;
	vector<Point2f> temp_rightPoints2;
	vector<Point2f> temp_leftLaneFitted;

    vector<unsigned int> cut_index;			//存放分割点的索引值
	Mat left_p;
	Mat right_p;
    //int mostPointsFlag;
    float VP,VP1=0,VP2=0;//VP3=0;			//每个区域求出来的消失点的值
    float factor1=0,factor2=0;//factor3=0;	//每个区域求出来的匹配系数
	int factor_num=0;				//有效的区域的个数
	float x_left, x_right;
	double a0,a1,b0,b1;
    unsigned int startIndex=0;
    unsigned int stopIndex=0;
	solidLanePoints.clear();
	for(size_t i=0;i<solidLaneFitted.size();i++)
	{
		solidLanePoints.push_back(Point2f(solidLaneFitted[i].y,solidLaneFitted[i].x));
	}
	for(size_t i=0;i<leftLanePoints.size()-1;i++)
	{
		float temp=leftLanePoints[i+1].x-leftLanePoints[i].x;
		if(temp>3)					//将点分成点集
		{
			stopIndex=i;
			if(stopIndex-startIndex>3)
			{
				cut_index.push_back(i);
				cut_index.push_back(leftLanePoints[i].x);
				cut_index.push_back(leftLanePoints[i+1].x);
				factor_num+=1;
			}
			startIndex=i+1;
		}
		else
		{
			stopIndex=i+1;
		}
	}
	if(startIndex==leftLanePoints.size()-1)
	{
		stopIndex=startIndex;
	}
	if(stopIndex-startIndex>3)
	{
		factor_num+=1;
	}
	switch (factor_num)
	{
	case 1:
		startIndex=0;
		stopIndex=0;
		for(size_t i=0;i<leftLanePoints.size()-1;i++)
		{
			float temp=leftLanePoints[i+1].x-leftLanePoints[i].x;
			if(temp>3)					//将点分成点集
			{
				stopIndex=i;
				if(stopIndex-startIndex>3)
				{
                    for(unsigned int i=startIndex;i<=stopIndex;i++)
						temp_leftPoints1.push_back(leftLanePoints[i]);
					stopIndex=0;
					break;
				}
				startIndex=i+1;
			}
			else
			{
				stopIndex=i+1;
			}
		}
		if(startIndex==leftLanePoints.size()-1)
		{
			stopIndex=startIndex;
		}
		if(stopIndex-startIndex>3)
		{
            for(unsigned int i=startIndex;i<=stopIndex;i++)
				temp_leftPoints1.push_back(leftLanePoints[i]);
		}
		startIndex=temp_leftPoints1[0].x;
		stopIndex=temp_leftPoints1[temp_leftPoints1.size()-1].x;
        for(unsigned int i=0;i<solidLanePoints.size();i++)
		{
			if(solidLanePoints[i].x>=startIndex&&solidLanePoints[i].x<=stopIndex)
				temp_rightPoints1.push_back(solidLanePoints[i]);
		}
		polynomial_curve_fit( temp_leftPoints1, 1, left_p);
		polynomial_curve_fit( temp_rightPoints1, 1, right_p);
		a0=left_p.at<double>(0,0);
		a1=left_p.at<double>(1,0);
		b0=right_p.at<double>(0,0);
		b1=right_p.at<double>(1,0);
		VP1=(a0-b0)/(b1-a1);           //两直线交点的y坐标
		x_left=a0+a1*(ROI_HEIGH-1);
		x_right=b0+b1*(ROI_HEIGH-1);
		factor1=(x_right-x_left)/((ROI_HEIGH-1)-VP1);
		for(size_t i=0;i<solidLaneFitted.size();i++)
		{
			temp_leftLaneFitted.push_back(Point2f(solidLaneFitted[i].x-factor1*(i-VP1),i));
		}
		//if(frame_count==1)
		//{
		//	startIndex=temp_leftPoints1[0].x;
		//	stopIndex=temp_leftPoints1[temp_leftPoints1.size()-1].x;
		//	for(int i=0;i<solidLanePoints.size();i++)
		//	{
		//		if(solidLanePoints[i].x>=startIndex&&solidLanePoints[i].x<=stopIndex)
		//			temp_rightPoints1.push_back(solidLanePoints[i]);
		//	}
		//	polynomial_curve_fit( temp_leftPoints1, 1, left_p);
		//	polynomial_curve_fit( temp_rightPoints1, 1, right_p);
		//	a0=left_p.at<double>(0,0);
		//	a1=left_p.at<double>(1,0);
		//	b0=right_p.at<double>(0,0);
		//	b1=right_p.at<double>(1,0);
		//	VP1=(a0-b0)/(b1-a1);
		//	x_left=a0+a1*(ROI_HEIGH-1);
		//	x_right=b0+b1*(ROI_HEIGH-1);
		//	factor1=(x_right-x_left)/((ROI_HEIGH-1)-VP1);
		//	for(size_t i=0;i<solidLaneFitted.size();i++)
		//	{
		//		temp_leftLaneFitted.push_back(Point2f(solidLaneFitted[i].x-factor1*(i-VP1),i));
		//	}
		//}
		//else
		//{
		//	if(temp_leftPoints1[0].x>ROI_HEIGH/2)
		//	{
		//		for(int i=ROI_HEIGH/2-10;i<ROI_HEIGH/2;i++)
		//		{
		//			temp_leftPoints2.push_back(Point2f(left_last_solid_point[i].y,left_last_solid_point[i].x));
		//		}
		//		for(int i=0;i<solidLanePoints.size();i++)
		//		{
		//			if(solidLanePoints[i].x>=temp_leftPoints1[0].x&&solidLanePoints[i].x<=temp_leftPoints1[temp_leftPoints1.size()-1].x)
		//				temp_rightPoints1.push_back(solidLanePoints[i]);
		//			if(solidLanePoints[i].x>=temp_leftPoints2[0].x&&solidLanePoints[i].x<=temp_leftPoints2[temp_leftPoints2.size()-1].x)
		//				temp_rightPoints2.push_back(solidLanePoints[i]);
		//		}
		//		polynomial_curve_fit( temp_leftPoints2, 1, left_p);
		//		polynomial_curve_fit( temp_rightPoints2, 1, right_p);
		//		a0=left_p.at<double>(0,0);
		//		a1=left_p.at<double>(1,0);
		//		b0=right_p.at<double>(0,0);
		//		b1=right_p.at<double>(1,0);
		//		VP2=(a0-b0)/(b1-a1);
		//		x_left=a0+a1*(ROI_HEIGH-1);
		//		x_right=b0+b1*(ROI_HEIGH-1);
		//		factor2=(x_right-x_left)/((ROI_HEIGH-1)-VP2);
		//		for(size_t i=0;i<ROI_HEIGH/2;i++)
		//		{
		//			temp_leftLaneFitted.push_back(Point2f(solidLaneFitted[i].x-factor2*(i-VP2),i));
		//		}
		//		polynomial_curve_fit( temp_leftPoints1, 1, left_p);
		//		polynomial_curve_fit( temp_rightPoints1, 1, right_p);
		//		a0=left_p.at<double>(0,0);
		//		a1=left_p.at<double>(1,0);
		//		b0=right_p.at<double>(0,0);
		//		b1=right_p.at<double>(1,0);
		//		VP1=(a0-b0)/(b1-a1);
		//		x_left=a0+a1*(ROI_HEIGH-1);
		//		x_right=b0+b1*(ROI_HEIGH-1);
		//		factor1=(x_right-x_left)/((ROI_HEIGH-1)-VP1);
		//		for(size_t i=ROI_HEIGH/2;i<ROI_HEIGH;i++)
		//		{
		//			temp_leftLaneFitted.push_back(Point2f(solidLaneFitted[i].x-factor1*(i-VP1),i));
		//		}
		//	}
		//	else if(temp_leftPoints1[temp_leftPoints1.size()-1].x<ROI_HEIGH/2)
		//	{
		//		/*for(int i=ROI_HEIGH/2;i<ROI_HEIGH/2+10;i++)
		//		{
		//			temp_leftPoints2.push_back(Point2f(left_last_solid_point[i].y,left_last_solid_point[i].x));
		//		}
		//		for(int i=0;i<solidLanePoints.size();i++)
		//		{
		//			if(solidLanePoints[i].x>=temp_leftPoints1[0].x&&solidLanePoints[i].x<=temp_leftPoints1[temp_leftPoints1.size()-1].x)
		//				temp_rightPoints1.push_back(solidLanePoints[i]);
		//			if(solidLanePoints[i].x>=temp_leftPoints2[0].x&&solidLanePoints[i].x<=temp_leftPoints2[temp_leftPoints2.size()-1].x)
		//				temp_rightPoints2.push_back(solidLanePoints[i]);
		//		}
		//		polynomial_curve_fit( temp_leftPoints1, 1, left_p);
		//		polynomial_curve_fit( temp_rightPoints1, 1, right_p);
		//		a0=left_p.at<double>(0,0);
		//		a1=left_p.at<double>(1,0);
		//		b0=right_p.at<double>(0,0);
		//		b1=right_p.at<double>(1,0);
		//		VP1=(a0-b0)/(b1-a1);
		//		x_left=a0+a1*(ROI_HEIGH-1);
		//		x_right=b0+b1*(ROI_HEIGH-1);
		//		factor1=(x_right-x_left)/((ROI_HEIGH-1)-VP1);
		//		for(size_t i=0;i<ROI_HEIGH/2;i++)
		//		{
		//			temp_leftLaneFitted.push_back(Point2f(solidLaneFitted[i].x-factor1*(i-VP1),i));
		//		}
		//		polynomial_curve_fit( temp_leftPoints2, 1, left_p);
		//		polynomial_curve_fit( temp_rightPoints2, 1, right_p);
		//		a0=left_p.at<double>(0,0);
		//		a1=left_p.at<double>(1,0);
		//		b0=right_p.at<double>(0,0);
		//		b1=right_p.at<double>(1,0);
		//		VP2=(a0-b0)/(b1-a1);
		//		x_left=a0+a1*(ROI_HEIGH-1);
		//		x_right=b0+b1*(ROI_HEIGH-1);
		//		factor2=(x_right-x_left)/((ROI_HEIGH-1)-VP2);
		//		for(size_t i=ROI_HEIGH/2;i<ROI_HEIGH;i++)
		//		{
		//			temp_leftLaneFitted.push_back(Point2f(solidLaneFitted[i].x-factor2*(i-VP2),i));
		//		}*/
		//	}
		//	else
		//	{
		//		startIndex=temp_leftPoints1[0].x;
		//		stopIndex=temp_leftPoints1[temp_leftPoints1.size()-1].x;
		//		for(int i=0;i<solidLanePoints.size();i++)
		//		{
		//			if(solidLanePoints[i].x>=startIndex&&solidLanePoints[i].x<=stopIndex)
		//				temp_rightPoints1.push_back(solidLanePoints[i]);
		//		}
		//		polynomial_curve_fit( temp_leftPoints1, 1, left_p);
		//		polynomial_curve_fit( temp_rightPoints1, 1, right_p);
		//		a0=left_p.at<double>(0,0);
		//		a1=left_p.at<double>(1,0);
		//		b0=right_p.at<double>(0,0);
		//		b1=right_p.at<double>(1,0);
		//		VP1=(a0-b0)/(b1-a1);
		//		x_left=a0+a1*(ROI_HEIGH-1);
		//		x_right=b0+b1*(ROI_HEIGH-1);
		//		factor1=(x_right-x_left)/((ROI_HEIGH-1)-VP1);
		//		for(size_t i=0;i<solidLaneFitted.size();i++)
		//		{
		//			temp_leftLaneFitted.push_back(Point2f(solidLaneFitted[i].x-factor1*(i-VP1),i));
		//		}
		//	}
		//}
		break;
	case 2:
        for(unsigned int i=0;i<=cut_index[0];i++)
			temp_leftPoints1.push_back(leftLanePoints[i]);
        for(unsigned int i=cut_index[0]+1;i<leftLanePoints.size();i++)
			temp_leftPoints2.push_back(leftLanePoints[i]);
        for(unsigned int i=0;i<solidLanePoints.size();i++)
		{
			if(solidLanePoints[i].x>=temp_leftPoints1[0].x&&solidLanePoints[i].x<=temp_leftPoints1[temp_leftPoints1.size()-1].x)
				temp_rightPoints1.push_back(solidLanePoints[i]);
			if(solidLanePoints[i].x>=temp_leftPoints2[0].x&&solidLanePoints[i].x<=temp_leftPoints2[temp_leftPoints2.size()-1].x)
				temp_rightPoints2.push_back(solidLanePoints[i]);
		}
		polynomial_curve_fit( temp_leftPoints1, 1, left_p);
		polynomial_curve_fit( temp_rightPoints1, 1, right_p);
		a0=left_p.at<double>(0,0);
		a1=left_p.at<double>(1,0);
		b0=right_p.at<double>(0,0);
		b1=right_p.at<double>(1,0);
		VP1=(a0-b0)/(b1-a1);
		x_left=a0+a1*(ROI_HEIGH-1);
		x_right=b0+b1*(ROI_HEIGH-1);
		factor1=(x_right-x_left)/((ROI_HEIGH-1)-VP1);
		for(size_t i=0;i<cut_index[1]+(cut_index[2]-cut_index[1])/4;i++)
		{
			temp_leftLaneFitted.push_back(Point2f(solidLaneFitted[i].x-factor1*(i-VP1),i));
		}
		polynomial_curve_fit( temp_leftPoints2, 1, left_p);
		polynomial_curve_fit( temp_rightPoints2, 1, right_p);
		a0=left_p.at<double>(0,0);
		a1=left_p.at<double>(1,0);
		b0=right_p.at<double>(0,0);
		b1=right_p.at<double>(1,0);
		VP2=(a0-b0)/(b1-a1);
		x_left=a0+a1*(ROI_HEIGH-1);
		x_right=b0+b1*(ROI_HEIGH-1);
		factor2=(x_right-x_left)/((ROI_HEIGH-1)-VP2);
		for(size_t i=cut_index[1]+(cut_index[2]-cut_index[1])/4;i<cut_index[2]-(cut_index[2]-cut_index[1])/4;i++)
		{
			temp_leftLaneFitted.push_back(Point2f(solidLaneFitted[i].x-(factor1+factor2)/2*(i-(VP1+VP2)/2),i));
		}
		for(size_t i=cut_index[2]-(cut_index[2]-cut_index[1])/4;i<ROI_HEIGH;i++)
		{
			temp_leftLaneFitted.push_back(Point2f(solidLaneFitted[i].x-factor2*(i-VP2),i));
		}
		break;
	default:
		break;
	}

	vector<int> temp_y_seq;							//拟合用的y序列
	vector<Point2f> tempPoints;							//拟合用的y序列
	Mat temp_p;
	if(temp_leftLaneFitted.size()>0)
	{
		for(size_t i=0;i<temp_leftLaneFitted.size();i++)
		{
			tempPoints.push_back(Point2f(temp_leftLaneFitted[i].y,temp_leftLaneFitted[i].x));
		}
		curvefitting_solid(leftLaneFitted,tempPoints);
	}
	leftLanePoints=temp_leftLaneFitted;
	VP=VP1;
	return VP;
}
float findVPAndMatchRight(vector<Point2f> &leftLanePoints,vector<Point2f> &rightLanePoints,vector<Point2f> &leftLaneFitted,vector<Point2f> &rightLaneFitted)
{
	vector<Point2f> temp_leftPoints1;
	vector<Point2f> temp_leftPoints2;
	vector<Point2f> temp_rightPoints1;
	vector<Point2f> temp_rightPoints2;
	vector<Point2f> temp_rightLaneFitted;

    vector<unsigned int> cut_index;			//存放分割点的索引值
	Mat left_p;
	Mat right_p;
    float VP,VP1=0,VP2=0;//VP3=0;			//每个区域求出来的消失点的值
    float factor1=0,factor2=0;//factor3=0;	//每个区域求出来的匹配系数
	int factor_num=0;				//有效的区域的个数
	float x_left, x_right;
	double a0,a1,b0,b1;
    unsigned int startIndex=0;
    unsigned int stopIndex=0;
	leftLanePoints.clear();

	for(size_t i=0;i<leftLaneFitted.size();i++)
	{
		leftLanePoints.push_back(Point2f(leftLaneFitted[i].y,leftLaneFitted[i].x));
	}
	for(size_t i=0;i<rightLanePoints.size()-1;i++)
	{
		float temp=rightLanePoints[i+1].x-rightLanePoints[i].x;
		if(temp>3)					//将点分成点集
		{
			stopIndex=i;
			if(stopIndex-startIndex>3)
			{
				factor_num+=1;
				cut_index.push_back(i);
				cut_index.push_back(rightLanePoints[i].x);
				cut_index.push_back(rightLanePoints[i+1].x);
			}
			startIndex=i+1;
		}
		else
		{
			stopIndex=i+1;
		}
	}
	if(startIndex==rightLanePoints.size()-1)
	{
		stopIndex=startIndex;
	}
	if(stopIndex-startIndex>3)
	{
		factor_num+=1;
	}
  	switch (factor_num)
	{
	case 1:
		startIndex=0;
		stopIndex=0;
		for(size_t i=0;i<rightLanePoints.size()-1;i++)
		{
			float temp=rightLanePoints[i+1].x-rightLanePoints[i].x;
			if(temp>3)					//将点分成点集
			{
				stopIndex=i;
				if(stopIndex-startIndex>3)
				{
                    for(unsigned int i=startIndex;i<=stopIndex;i++)
						temp_rightPoints1.push_back(rightLanePoints[i]);
					stopIndex=0;
					break;
				}
				startIndex=i+1;
			}
			else
			{
				stopIndex=i+1;
			}
		}
		if(startIndex==rightLanePoints.size()-1)
		{
			stopIndex=startIndex;
		}
		if(stopIndex-startIndex>3)
		{
            for(unsigned int i=startIndex;i<=stopIndex;i++)
				temp_rightPoints1.push_back(rightLanePoints[i]);
		}
		startIndex=rightLanePoints[0].x;
		stopIndex=rightLanePoints[rightLanePoints.size()-1].x;
        for(unsigned int i=0;i<leftLanePoints.size();i++)
		{
			if(leftLanePoints[i].x>=startIndex && leftLanePoints[i].x<=stopIndex)
				temp_leftPoints1.push_back(leftLanePoints[i]);
		}
		polynomial_curve_fit( temp_rightPoints1, 1, right_p);
		polynomial_curve_fit( temp_leftPoints1, 1, left_p);
		a0=right_p.at<double>(0,0);
		a1=right_p.at<double>(1,0);
		b0=left_p.at<double>(0,0);
		b1=left_p.at<double>(1,0);
		VP1=(a0-b0)/(b1-a1);
		x_right=a0+a1*(ROI_HEIGH-1);
		x_left=b0+b1*(ROI_HEIGH-1);
		factor1=(x_right-x_left)/((ROI_HEIGH-1)-VP1);
		for(size_t i=0;i<leftLaneFitted.size();i++)
		{
			temp_rightLaneFitted.push_back(Point2f(leftLaneFitted[i].x + factor1*(i-VP1),i));
		}

		break;
	case 2:
        for(unsigned int i=0;i<=cut_index[0];i++)
			temp_rightPoints1.push_back(rightLanePoints[i]);

        for(unsigned int i=cut_index[0]+1;i<rightLanePoints.size();i++)
			temp_rightPoints2.push_back(rightLanePoints[i]);

        for(unsigned int i=0;i<leftLanePoints.size();i++)
		{
			if(leftLanePoints[i].x>=temp_rightPoints1[0].x && leftLanePoints[i].x<=temp_rightPoints1[temp_rightPoints1.size()-1].x)
				temp_leftPoints1.push_back(leftLanePoints[i]);

			if(leftLanePoints[i].x>=temp_rightPoints2[0].x && leftLanePoints[i].x<=temp_rightPoints2[temp_rightPoints2.size()-1].x)
				temp_leftPoints2.push_back(leftLanePoints[i]);
		}

		polynomial_curve_fit( temp_rightPoints1, 1, right_p);
		polynomial_curve_fit( temp_leftPoints1, 1, left_p);
		a0=right_p.at<double>(0,0);
		a1=right_p.at<double>(1,0);
		b0=left_p.at<double>(0,0);
		b1=left_p.at<double>(1,0);
		VP1=(a0-b0)/(b1-a1);
		x_right=a0+a1*(ROI_HEIGH-1);
		x_left=b0+b1*(ROI_HEIGH-1);
		factor1=(x_right-x_left)/((ROI_HEIGH-1)-VP1);
		
		for(size_t i=0;i<cut_index[1]+(cut_index[2]-cut_index[1])/4;i++)
		{
			temp_rightLaneFitted.push_back(Point2f(leftLaneFitted[i].x + factor1*(i-VP1),i));
		}
		polynomial_curve_fit( temp_rightPoints2, 1, right_p);
		polynomial_curve_fit( temp_leftPoints2, 1, left_p);
		a0=right_p.at<double>(0,0);
		a1=right_p.at<double>(1,0);
		b0=left_p.at<double>(0,0);
		b1=left_p.at<double>(1,0);
		VP2=(a0-b0)/(b1-a1);
		x_right=a0+a1*(ROI_HEIGH-1);
		x_left=b0+b1*(ROI_HEIGH-1);
		factor2=(x_right-x_left)/((ROI_HEIGH-1)-VP2);
		for(size_t i=cut_index[1]+(cut_index[2]-cut_index[1])/4;i<cut_index[2]-(cut_index[2]-cut_index[1])/4;i++)
		{
			temp_rightLaneFitted.push_back(Point2f(leftLaneFitted[i].x + (factor1+factor2)/2*(i-(VP1+VP2)/2),i));
		}
		for(size_t i=cut_index[2]-(cut_index[2]-cut_index[1])/4;i<ROI_HEIGH;i++)
		{
			temp_rightLaneFitted.push_back(Point2f(leftLaneFitted[i].x + factor2*(i-VP2),i));
		}
		break;
	default:
		break;
	}

	vector<Point2f> tempPoints;							//拟合用的y序列
	Mat temp_p;
	if(temp_rightLaneFitted.size()>0)
	{
		for(size_t i=0;i<temp_rightLaneFitted.size();i++)
		{
			tempPoints.push_back(Point2f(temp_rightLaneFitted[i].y,temp_rightLaneFitted[i].x));
		}
		curvefitting_solid(rightLaneFitted,tempPoints);
	}
	//leftLaneFitted=temp_leftLaneFitted;
	VP=VP1;
	return VP;
}

#endif
