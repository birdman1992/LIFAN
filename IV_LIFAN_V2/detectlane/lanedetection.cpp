#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cstdlib>
#include <math.h>
#include "includes.h"
using namespace cv;
using namespace std;


#include"Data.h"
#include"KeyPointExt.h"
#include"curvefitting.h"
//#include"processors.h"

vector<Point2f> last_dash_point;		//上一帧右侧虚线的拟合结果
vector<Point2f> last_solid_point;		//上一帧右侧实线的拟合结果
vector<Point2f> left_last_solid_point;	//上一帧左侧实线的拟合结果
vector<Point2f> left_last_dash_point;	//上一帧左侧虚线的拟合结果

int heir_num=0;							//继承上一帧的次数
int doubleLaneNum=0;					//右侧连续出现双线的帧数
int left_doubleLaneNum=0;				//左侧连续出现双线的帧数
bool isDoubleLaneInLastFrame=false;		//右侧上一帧是否存在双线的标志
bool left_isDoubleLaneInLastFrame=false;	//左侧上一帧是否存在双线的标志
int frame_num=0;						//当前检测有效的帧数累计值
int total_frame=0;						//总帧数
vector<int> dash_err;					//记录右侧虚线本帧与上一帧偏差的vector
vector<int> solid_err;					//记录右侧实线本帧与上一帧偏差的vector
vector<int> left_dash_err;				//记录左侧虚线本帧与上一帧偏差的vector
vector<int> left_solid_err;				//记录左侧实线本帧与上一帧偏差的vector
bool acc_flag=false;					//加速的标志
bool switch_video=false;				//切换视频的标志
float a_horizon = 78.0 / 640;                     //换算系数
//int thresh = 240;

bool solid_exist = 0;							//右侧存在实线的标志
bool left_solid_exist = 0;					//左侧存在实线的标志
bool isDoubleLane;						//当前帧是否为双车道的标志
bool left_isDoubleLane;					//左侧当前帧是否为双车道的标志
bool thresholdFlag;                     //判断阈值分割是否需要切换标志位
bool lane_exist = 0;                              //当前是否检测到右侧车道线
bool left_lane_exist =0 ;                      //当前是否检测到左侧车道线
bool err_exist = 0 ;                               //当前是否有中线偏差数据
float err = 0;                          //与车道中线偏差值

Mat frame;			//图片读取

VideoCapture cam(1);

int lane_main()
{
	
	//system("pause");
	//int count=0;

	//CameraGetCount(&count);
	//printf("Camera count: %d\n",count);

	//if(count<1) 
	//	break;

	//CameraInit(0);
	//CameraSetGain(0,1);
	//CameraSetExposure(0,1);

	//CameraSetSnapMode(0,CAMERA_SNAP_CONTINUATION);

	//int width=0, height=0, len=0;
	//CameraGetResolution(0,0,&width,&height);
	//CameraGetImageBufferSize(0,&len,CAMERA_IMAGE_BMP);
	//IplImage *image = cvCreateImage(cvSize(width,height),8,3);

    //int width = 640;
    //int height = 480;
	int i = 0;
	double rightA = 0;
	double leftA = 0;
 //   VideoCapture capture("Video_test1_20171110.avi");//("Video_test1_1025||video1_2017918.avi");    //(读取视频)
    //VideoWriter writer("Video_test.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25.0, Size(width,height));
    //VideoWriter writer1("Video_test1.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25.0, Size(width,height));
    //VideoWriter writer2("Video_test2.avi", CV_FOURCC('M', 'P', '4', '2'), 25.0, Size(ROI_WIDTH,ROI_HEIGH),0);
	//VideoCapture capture("test_video4.avi");									//视频类，读取视频文件，支持.avi，可以用格式转换器先将格式转换
	//VideoCapture capture("C:\\Users\\ASUS\\Desktop\\roadtest2.wmv");			
	//VideoCapture capture(1);			//读取视频文件
    //sleep(20);
    //VideoCapture capture1("video3.avi");			//视频类，读取视频文件，支持.avi，可以用格式转换器先将格式转换(读取视频)
    //double rate= capture.get(CV_CAP_PROP_FPS);		//VideoCapture::get 返回指定视频类的相关参数信息 CV_CAP_PROP_FPS读取帧率，输入时可以先输入CV_CAP_PROP再用ctrl+j查找对应项(读取视频)
    //double duration;

	Mat ROI;			//获取ROI
	Mat BW;				//二值图
	Mat bw;
	Mat img;
	Mat gray;			//灰度图
	Mat leftPolyFactor;   //左车道拟合斜率参数                                      
	Mat rightPolyFactor;   //右车道拟合斜率参数
	//Mat tshbROI;
    //namedWindow("VideoTest");			//显示二值图像
	//createTrackbar("阈值:","VideoTest",&thresh,255,NULL); 
//	namedWindow("VideoTest2");			//显示检测结果
	//namedWindow("right_ROI");			//显示检测结果
	//namedWindow("left_ROI");			//显示检测结果
    //bool predict_label=true;			//对虚线部分进行预测的标志

  //  while(1)
    {
		int transRightCols = 0;                      //检测出车道线所在列数
        int transLeftCols = 0;
		int rightCols = 0;
		int leftCols = 0;
		int transMiddleCols = 0;
		int middleCols = 0;
		Mat imgTrans = Mat::zeros(110,640,CV_8UC1);           //透视变换图
		//if(CameraQueryImage(0,(unsigned char*)image->imageData, &len,CAMERA_IMAGE_BMP)==API_OK);
        //duration=static_cast<double>(getTickCount());			//计时
		//namedWindow("Canny");
//        int flag;                     //(读取视频)
//       if(!switch_video)				//是否切换视频，bool switch_video = false;
//        capture.read(frame);	//VideoCapture::read抓住，解码并返回下一个视频帧
        //else							//切换
            //flag=capture1.read(frame);	//VideoCapture::read抓住，解码并返回下一个视频帧
        /*if(!flag)
            break;     */                 //(读取视频)
		//flip(frame,frame,-1);
		//Mat frame = cvarrToMat(image);
		//Mat frameRaw = cvarrToMat(image);
        cam >> frame;
		//writer1 << frame;

        solid_exist=false;							//右侧存在实线的标志
        left_solid_exist=false;					//左侧存在实线的标志
        //float VP;
        isDoubleLane=false;						//当前帧是否为双车道的标志
        left_isDoubleLane=false;					//左侧当前帧是否为双车道的标志
        thresholdFlag = false;                     //判断阈值分割是否需要切换标志位
        lane_exist=false;                              //当前是否检测到右侧车道线
        left_lane_exist=false;                      //当前是否检测到左侧车道线
        err_exist=false;                               //当前是否有中线偏差数据
        err = false;
        //bool left_thresholdFlag = false;                //判断阈值分割是否需要切换标志位

      if(frame.cols>0&&frame.rows>0)
       {
//           bool flag;
        //flag = cam.read(frame);
		frame_num++;											//有效循环的累计帧数
		total_frame++;											//总帧数，当前帧数
		i++;
        //cout<<"frame_num="<<frame_num<<endl;					//输出有效循环的累计帧数
        //cout<<"total_frame_num="<<total_frame<<endl;			//输出总帧数，当前帧数
		ROI=frame(Rect(ROI_X,ROI_Y,ROI_WIDTH,ROI_HEIGH));		//frame中的ROI区域，Data.h文件中，矩形ROI区域，左上角坐标ROI_X=200，ROI_Y=260，宽ROI_WIDTH=420，高ROI_HEIGH=120
 		ROI.copyTo(img);										//将ROI区域复制到img中，对img进行操作，师兄用的img=ROI.clone();这样可能会出现内存溢出，.cppyTo会先创建一个矩阵
		
		cvtColor(img,gray,CV_RGB2GRAY);	 						//灰度变换
        //double otsu_thresh;
		//threshold(gray,BW,double(thresh),255,THRESH_BINARY);
		//threshold(gray,BW,0,255,CV_THRESH_OTSU);				//二值化
		int Threshold = OstuThreshold(gray);
		threshold(gray, BW, Threshold, 255, CV_THRESH_BINARY);
		//adaptiveThreshold(gray,BW,255,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY_INV,25,10);
		//Mat element = getStructuringElement(MORPH_RECT,Size(3,3));
		//morphologyEx(BW,BW,MORPH_OPEN,element);
		//imshow("Gray",gray);

		vector<Point2i> scanPoints;						//右侧扫描得到的点
		vector<Point2f> dashLanePoints;					//右侧过滤后的虚线车道点
		vector<Point2f> solidLanePoints;				//右侧过滤后的实线车道点
		vector<Point2f> dashLaneFitted;					//右侧虚线拟合结果
		vector<Point2f> solidLaneFitted;				//右侧实线拟合结果

		vector<Point2i> leftscanPoints;					//左侧扫描得到的点
		vector<Point2f> left_dashLanePoints;			//左侧过滤后的虚线车道点
		vector<Point2f> left_solidLanePoints;			//左侧过滤后的实线车道点
		vector<Point2f> left_dashLaneFitted;			//左侧虚线拟合结果
		vector<Point2f> left_solidLaneFitted;			//左侧实线拟合结果
		
		
		/****************右侧扫描边界****************/
		int r_bound=0;									
		if(frame_num>1)
		{
			for(size_t i=0;i<left_last_solid_point.size();i++)		//long unsigned int，size_t的全称应该是size type，就是说“一种用来记录大小的数据类型”。这个for循环用来找到左车道线最右侧点，并赋给r_bound
			{
				int temp=left_last_solid_point[i].x;
				if(temp>r_bound)
					r_bound=temp;
			}
			r_bound+=5;												//当前帧的右检测起始边界点r_bound=上一帧左车道线最右点+5
		}
		else														//第一帧右扫描起始边界为ROI正中间
		{
			r_bound=245+20;
		}
		if(r_bound<1||r_bound>BW.cols-1)							//如果由上面计算出来的r_bound的值不在图幅内，则重新定义右起始边界r_bound为ROI正中间（此时为左丢线的情况）
		{
			r_bound=245+20;
		}

		/*********************左侧扫描边界***************/
		int l_bound=680;											
		if(frame_num>1)												
		{
			for(size_t i=0;i<last_solid_point.size();i++)			//这个for循环用来找到右侧实车道线的最左点，并赋值给l_bound
			{
				int temp=last_solid_point[i].x;
				if(temp<l_bound)
					l_bound=temp;
			}
			l_bound-=5;												//当前帧的左车道检测终点l_bound=上一帧的右侧实车道线的最左点-5
		}
		else														//第一帧的左车道检测终点为ROI的正中间
		{
			l_bound=245-20;									
		}
		if(l_bound<1||l_bound>BW.cols-1)							//如果由上面计算出来的L_bound的值不在图幅内，则重新定义左车道检测终点l_bound为ROI正中间（此时为右丢线的情况）
		{
			l_bound=245-20;
		}
		//imshow("VideoTest100",BW);
		//writer2 << BW;

		//判断阈值分割模式是否合适
		thresholdFlag = thresholdModeSwitch(BW,l_bound,thresholdFlag);
		if(thresholdFlag) //若自适应阈值出现问题，通过跟换为固定阈值
		{
			leftscanPoints.clear();
			scanPoints.clear();
			/*Ptr<cv::CLAHE> clahe = createCLAHE();  
            clahe->setClipLimit(3);  
            clahe->apply(gray, gray);*/
			//adaptiveThreshold(gray, BW, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 5,7);
            /*threshold(gray,BW,Threshold+45,255,THRESH_BINARY);*/ //33
			Threshold = secondThreshold(gray, BW);
			//Threshold = secondThreshold(gray, BW);
			threshold(gray,BW,Threshold,255,THRESH_BINARY);
		}
        //imshow("VideoTest",BW);
		//writer2 << BW;


		//透视变换
		//tsbh(BW,tsbhImg);
		//imshow("tsbh",tsbhImg);

		//点扫描程序
		leftPointsExt(BW,leftscanPoints,l_bound,left_last_solid_point,left_last_dash_point,frame_num,left_isDoubleLaneInLastFrame); 
		rightPointsExt(BW,scanPoints,r_bound,last_solid_point,last_dash_point,frame_num,isDoubleLaneInLastFrame);

//		size_t leftSize = leftscanPoints.size();
//		size_t rightSize = scanPoints.size();

//        for(unsigned int i = 0 ; i < leftSize ; i++)
//		{
//		    circle(frame,Point(leftscanPoints[i].y+ROI_X,leftscanPoints[i].x+ROI_Y),5,Scalar(0,0,0),-1);
//		}
//        for(unsigned int i = 0 ; i < rightSize ; i++)
//		{
//			circle(frame,Point(scanPoints[i].y+ROI_X,scanPoints[i].x+ROI_Y),5,Scalar(0,0,0),-1);
//		}


		//if(thresholdFlag) //若自适应阈值出现问题，通过跟换为固定阈值
		//{
		//	leftscanPoints.clear();
		//	scanPoints.clear();
  //          threshold(gray,BW,165,255,THRESH_BINARY);
		//	leftPointsExt(BW,leftscanPoints,l_bound,left_last_solid_point,left_last_dash_point,frame_num,left_isDoubleLaneInLastFrame); 
		//    rightPointsExt(BW,scanPoints,r_bound,last_solid_point,last_dash_point,thresholdFlag,frame_num,isDoubleLaneInLastFrame);
		//	//if(leftscanPoints.size()==0 && scanPoints.size()==0)
		//	//{
		//	//	leftscanPoints.clear();
		//	//    scanPoints.clear();
		//	//	threshold(gray,BW,155,255,THRESH_BINARY);
		//	//	leftPointsExt(BW,leftscanPoints,l_bound,left_last_solid_point,left_last_dash_point,frame_num,left_isDoubleLaneInLastFrame); 
		// //       rightPointsExt(BW,scanPoints,r_bound,last_solid_point,last_dash_point,thresholdFlag,frame_num,isDoubleLaneInLastFrame);
		//	//}
		//}
		//imshow("VideoTest",BW);
		//writer2 << BW;

		
		//点过少时，直接重新开始
		if(leftscanPoints.size()>0&&scanPoints.size()>0) 
		{
			if(frame_num==1)
			{
				vector<Point2i> mirror_leftscanPoints;
                for(unsigned int i=0;i<leftscanPoints.size();i++)
				{
					mirror_leftscanPoints.push_back(Point2f(leftscanPoints[i].x,ROI_WIDTH-leftscanPoints[i].y));
				}
				//将二值图镜像
				Mat mirror_BW=Mat::zeros(BW.rows, BW.cols, CV_8UC1);
				for(int i=0;i<BW.rows;i++)
				{
					uchar* data= BW.ptr<uchar>(i);//取BW图像每行的首地址
					uchar* data1= mirror_BW.ptr<uchar>(i);//取镜像BW图像每行的首地址
					for(int j=0;j<BW.cols;j++)
					{
						data1[j]=data[ROI_WIDTH-1-j];
					}
				}
   				isDoubleLane=FeatureExtraction(scanPoints,doubleLaneNum,isDoubleLaneInLastFrame,dashLanePoints,solidLanePoints,BW);	//获取右侧车道的特征点
				left_isDoubleLane=FeatureExtraction(mirror_leftscanPoints,left_doubleLaneNum,left_isDoubleLaneInLastFrame,
					left_dashLanePoints,left_solidLanePoints,mirror_BW);															//获取左侧车道特征点

				if(dashLanePoints.size()==0&&isDoubleLane)
				{
					isDoubleLane=false;
				}
				if(left_dashLanePoints.size()==0&&left_isDoubleLane)
				{
					left_isDoubleLane=false;
				}
				//将镜像过滤的点还原
                for(unsigned int i=0;i<left_solidLanePoints.size();i++)
				{
					left_solidLanePoints[i].y=ROI_WIDTH-left_solidLanePoints[i].y;
				}
				if(left_dashLanePoints.size()>0)
				{
                    for(unsigned int i=0;i<left_dashLanePoints.size();i++)
					{
						left_dashLanePoints[i].y=ROI_WIDTH-left_dashLanePoints[i].y;
					}
				}
			}
			else if(frame_num > 1)
			{
				//对点进行"拉伸"，得到类似于逆透视的效果
                for(unsigned int i=0;i<leftscanPoints.size();i++)
				{
					leftscanPoints[i].y-=((int)(left_last_solid_point[leftscanPoints[i].x].x)+10);
				}
                for(unsigned int i=0;i<scanPoints.size();i++)
				{
					scanPoints[i].y-=((int)(last_dash_point[scanPoints[i].x].x)+10);
				}
				//在拉伸的基础上，对点进行去躁和聚类
				Filter1(scanPoints);
				if(scanPoints.size()>3)
				{
                    isDoubleLane=Filter2(scanPoints,isDoubleLaneInLastFrame,dashLanePoints,solidLanePoints,last_dash_point,BW);
				}
				leftFilter1(leftscanPoints);
				if(leftscanPoints.size()>3)
				{
                    left_isDoubleLane=leftFilter2(leftscanPoints,left_isDoubleLaneInLastFrame,left_dashLanePoints,left_solidLanePoints,left_last_solid_point,BW);
				}
				//对拉伸的点进行还原
				if(solidLanePoints.size()>1)
				{
                    for(unsigned int i=0;i<solidLanePoints.size();i++)
					{
						solidLanePoints[i].y+=((int)(last_dash_point[solidLanePoints[i].x].x)+10);
					}
				}
				if(dashLanePoints.size()>1)
				{
                    for(unsigned int i=0;i<dashLanePoints.size();i++)
					{
						dashLanePoints[i].y+=((int)(last_dash_point[dashLanePoints[i].x].x)+10);
					}
				}
				if(left_solidLanePoints.size()>1)
				{
                    for(unsigned int i=0;i<left_solidLanePoints.size();i++)
					{
						left_solidLanePoints[i].y+=((int)(left_last_solid_point[left_solidLanePoints[i].x].x)+10);
					}
				}
				if(left_dashLanePoints.size()>1)
				{
                    for(unsigned int i=0;i<left_dashLanePoints.size();i++)
					{
						left_dashLanePoints[i].y+=((int)(left_last_solid_point[left_dashLanePoints[i].x].x)+10);
					} 
				}
			}

			if(left_solidLanePoints.size()>4||solidLanePoints.size()>4)				//点的数量基本足够时，可直接进行拟合 //原程序为 if(left_solidLanePoints.size()>4&&solidLanePoints.size()>4)
			{
                if(isDoubleLane||solidLanePoints.size()>0.45*ROI_HEIGH)					//判断左侧是否存在实线
				{
					solid_exist=true;
				}
                if(left_isDoubleLane||left_solidLanePoints.size()>0.45*ROI_HEIGH)		//判断右侧是否存在实线
				{
					left_solid_exist=true;
				}
				//车道线拟合部分
 				if(solid_exist && left_solid_exist && solidLanePoints.size()>4 && left_solidLanePoints.size()>4)									
				{
					if(isDoubleLane)												
					{
						curvefitting(dashLaneFitted,dashLanePoints,1);				
						curvefitting(solidLaneFitted,solidLanePoints,4);		
					}
					else
					{

						curvefitting(solidLaneFitted,solidLanePoints,4);		
					}
					if(left_isDoubleLane)
					{
						curvefitting(left_dashLaneFitted,left_dashLanePoints,1);
						curvefitting(left_solidLaneFitted,left_solidLanePoints,4);	
					}
					else
					{
						curvefitting(left_solidLaneFitted,left_solidLanePoints,4);
					}
				}
				else if(solid_exist && (!left_solid_exist) && solidLanePoints.size()>4 && left_solidLanePoints.size()>4)
				{
					if(isDoubleLane)												
					{
						curvefitting(dashLaneFitted,dashLanePoints,1);				
						curvefitting1(solidLaneFitted,solidLanePoints,4);			//实线拟合
					}
					else
					{
						curvefitting1(solidLaneFitted,solidLanePoints,4);		//实线拟合
					}
					
					if(frame_num>1)							
					{
						//判断顶部点与上一帧的坐标差
						//if((abs(solidLaneFitted[0].x-last_solid_point[0].x)>5)&&solidLanePoints[0].x>5)
						//{
						//	cout<<"top_dep="<<solidLaneFitted[0].x-last_solid_point[0].x<<endl;
						//	solidLaneFitted=last_solid_point;
						//	//heir_num+=1;
						//}
					}
					if(left_solidLanePoints.size()>0 && solidLanePoints.size()>0)
					{
                        //VP=findVP(left_solidLanePoints,solidLanePoints,solidLaneFitted,left_solidLaneFitted);
					}
					if(left_solidLaneFitted.size()==0)
					{
						if(frame_num>1)
							left_solidLaneFitted=left_last_solid_point;
						else
						{
							heir_num=0;
							frame_num=0;
							last_dash_point.clear();
							last_solid_point.clear();
							left_last_solid_point.clear();
							left_last_dash_point.clear();
							doubleLaneNum=0;
							isDoubleLaneInLastFrame=false;
							left_doubleLaneNum=0;
							left_isDoubleLaneInLastFrame=false;
						}
					}

				}
				else if((!solid_exist) && left_solid_exist && left_solidLanePoints.size()>4 && solidLanePoints.size()>4)
				{
					if(left_isDoubleLane)
					{
						curvefitting(left_dashLaneFitted,left_dashLanePoints,1);
						curvefitting(left_solidLaneFitted,left_solidLanePoints,4);	
					}
					else
					{
						curvefitting(left_solidLaneFitted,left_solidLanePoints,4);
					}
					if(left_solidLanePoints.size()>0 && solidLanePoints.size()>0)
					{
                        //VP=findVPAndMatchRight(left_solidLanePoints,solidLanePoints,left_solidLaneFitted,solidLaneFitted);
					}
					if(solidLaneFitted.size()==0)
					{
						if(frame_num>1)
							solidLaneFitted=last_solid_point;
						else
						{
							heir_num=0;
							frame_num=0;
							last_dash_point.clear();
							last_solid_point.clear();
							left_last_solid_point.clear();
							left_last_dash_point.clear();
							doubleLaneNum=0;
							isDoubleLaneInLastFrame=false;
							left_doubleLaneNum=0;
							left_isDoubleLaneInLastFrame=false;
						}
					}
				}
				else if((!solid_exist)&&(!left_solid_exist))                       //原程序为 else
				{
					if(solidLanePoints.size()>0 && left_solidLanePoints.size()>0)
					{
						curvefitting(solidLaneFitted,solidLanePoints,1);				
					    curvefitting(left_solidLaneFitted,left_solidLanePoints,1);
					}
					if(solidLanePoints.size()>0 && left_solidLanePoints.size()==0)       //增加判断
					{
						curvefitting(solidLaneFitted,solidLanePoints,1);
					}
					if(solidLanePoints.size()==0 && left_solidLanePoints.size()>0)       //增加判断
					{
						curvefitting(left_solidLaneFitted,left_solidLanePoints,1);
					}
				}
				
				//记录左侧实线与上一帧的偏差
				if(frame_num>1 && left_solidLaneFitted.size()>0)													
				{
					int err_num=0;
                    for(unsigned int i=0;i<left_solidLaneFitted.size();i++)
					{
						if(abs(left_solidLaneFitted[i].x-left_last_solid_point[i].x)>DEPART_MAX)
							err_num+=1;
					}
					left_solid_err.push_back(err_num);
					//cout<<"left_err_num="<<err_num<<endl;
					if(err_num>DEPART_POINT_MAX)
					{
						heir_num+=1;
						left_solidLaneFitted=left_last_solid_point;
					}
					/*else
						heir_num=0;*/
				}				
				if(frame_num>1 && solidLaneFitted.size()>0)													
				{
					int err_num=0;
                    for(unsigned int i=0;i<solidLaneFitted.size();i++)
					{
						if(abs(solidLaneFitted[i].x-last_solid_point[i].x)>DEPART_MAX)
							err_num+=1;
					}
					solid_err.push_back(err_num);
					//cout<<"right_err_num="<<err_num<<endl;  
					if(err_num>DEPART_POINT_MAX)
					{
						heir_num+=1;
						solidLaneFitted=last_solid_point;
					}
					/*else
						heir_num=0;*/
				}				
				if(isDoubleLane&&isDoubleLaneInLastFrame&&(doubleLaneNum>2))				
				{
					int err_num=0;
                    for(unsigned int i=0;i<dashLaneFitted.size();i++)
					{
						if(abs(dashLaneFitted[i].x-last_dash_point[i].x)>DEPART_MAX)
							err_num+=1;
					}
					dash_err.push_back(err_num);
					if(err_num>DEPART_POINT_MAX)
					{
						dashLaneFitted=last_dash_point;
					}
				}
				if(isDoubleLane && solidLaneFitted.size()>0)
				{
					if(dashLaneFitted[0].x>solidLaneFitted[0].x||dashLaneFitted[ROI_HEIGH-1].x>solidLaneFitted[ROI_HEIGH-1].x||(dashLaneFitted[0].x-solidLaneFitted[0].x)<-30||(dashLaneFitted[ROI_HEIGH-1].x-solidLaneFitted[ROI_HEIGH-1].x)<-60)
					{
						if(isDoubleLaneInLastFrame)
						{
							dashLaneFitted=last_dash_point;
						}
						else
						{
							isDoubleLane=false;
						}
					}
				}
			}
			else					//过滤后点不够时的处理方法，，如果是第一帧则直接重新开始，否则继承上一帧
			{
				if(frame_num>1)
				{
					heir_num+=1;
					solidLaneFitted=last_solid_point;
					dashLaneFitted=last_dash_point;
					left_solidLaneFitted=left_last_solid_point;
					left_dashLaneFitted=left_last_dash_point;
				}
				else
				{
					heir_num=0;
					frame_num=0;
					last_dash_point.clear();
					last_solid_point.clear();
					left_last_solid_point.clear();
					left_last_dash_point.clear();
					doubleLaneNum=0;
					isDoubleLaneInLastFrame=false;
					left_doubleLaneNum=0;
					left_isDoubleLaneInLastFrame=false;
				}
			}


			polynomial_curve_fit(left_solidLaneFitted, 1, leftPolyFactor);
			polynomial_curve_fit(solidLaneFitted, 1, rightPolyFactor);
			leftA = leftPolyFactor.at<double>(1,0);
			rightA = rightPolyFactor.at<double>(1,0);
			cout << "leftA =" << leftA << endl;
			cout << "rightA = " << rightA << endl;

			if(frame_num>0)
			{

                if(solidLaneFitted.size()>0 && rightA < 2 && rightA > 0.1)
				{
					for(size_t i=0;i<solidLaneFitted.size();i++)								//显示拟合结果
					{
						circle(frame,Point(solidLaneFitted[i].x+ROI_X,solidLaneFitted[i].y+ROI_Y),2,Scalar(0,255,0));
					}
					tsbh(imgTrans,solidLaneFitted,transRightCols,rightCols);

					if(isDoubleLane)
					{
						for(size_t i=0;i<dashLaneFitted.size();i++)
						{
							if(dashLaneFitted[i].x<=BW.cols)
							{
								circle(ROI,dashLaneFitted[i],2,Scalar(0,255,255));
							}
						}
						//tsbh(imgTrans,dashLaneFitted,cols);
					}
				}

                if(left_solidLaneFitted.size()>0 && leftA > -2 && leftA < -0.1)
				{
					for(size_t i=0;i<left_solidLaneFitted.size();i++)
					{
						circle(frame,Point2f(left_solidLaneFitted[i].x+ROI_X,left_solidLaneFitted[i].y+ROI_Y),2,Scalar(0,255,0));
					}
					tsbh(imgTrans,left_solidLaneFitted,transLeftCols,leftCols);
					if(left_isDoubleLane)
					{
						for(size_t i=0;i<left_dashLaneFitted.size();i++)
						{
							if(left_dashLaneFitted[i].x<=BW.cols)
							{
								circle(ROI,left_dashLaneFitted[i],2,Scalar(0,255,255));
							}
						}
						//tsbh(imgTrans,left_dashLaneFitted);
					}
				}
				//for(int i=0;i<ROI_HEIGH/5;i++)    //自车道可行驶区域显示
				//{
				//	line(ROI,left_solidLaneFitted[i],solidLaneFitted[i],Scalar(0,155,50),1);
				//}
				//for(int i=2*ROI_HEIGH/5;i<3*ROI_HEIGH/5;i++)
				//{
				//	line(ROI,left_solidLaneFitted[i],solidLaneFitted[i],Scalar(0,155,50),1);
				//}
				//for(int i=4*ROI_HEIGH/5;i<ROI_HEIGH;i++)
				//{
				//	line(ROI,left_solidLaneFitted[i],solidLaneFitted[i],Scalar(0,155,50),1);
				//}
				if(rightA < 1.5 && rightA > 0.1)
				{
					if (isDoubleLane)
					{
						putText(frame, "doubleline", Point(480, 40), cv::FONT_HERSHEY_DUPLEX, 0.8, cv::Scalar(255, 0, 255), 2);
						line(frame, Point(535, 50), Point(535, 150), Scalar(0, 255, 255), 5);
						line(frame, Point(520, 50), Point(520, 70), Scalar(0, 255, 255), 5);
						line(frame, Point(520, 88), Point(520, 108), Scalar(0, 255, 255), 5);
						line(frame, Point(520, 126), Point(520, 150), Scalar(0, 255, 255), 5);
                        lane_exist=true;
					}
					else if (solid_exist)
					{
						putText(frame, "solidline", Point(480, 40), cv::FONT_HERSHEY_DUPLEX, 0.8, cv::Scalar(255, 0, 255), 2);
						line(frame, Point(530, 50), Point(530, 150), Scalar(0, 255, 255), 5);
                        lane_exist=true;
					}
					else
					{
						putText(frame, "dashline", Point(480, 40), cv::FONT_HERSHEY_DUPLEX, 0.8, cv::Scalar(255, 0, 255), 2);
						line(frame, Point(520, 50), Point(520, 70), Scalar(0, 255, 255), 5);
						line(frame, Point(520, 88), Point(520, 108), Scalar(0, 255, 255), 5);
						line(frame, Point(520, 126), Point(520, 150), Scalar(0, 255, 255), 5);
                        lane_exist=true;
					}
				}
				if(leftA > -1.5 && leftA < -0.1)
				{
					if (left_isDoubleLane)
					{
						putText(frame, "doubleline", Point(320, 40), cv::FONT_HERSHEY_DUPLEX, 0.8, cv::Scalar(255, 0, 255), 2);
						line(frame, Point(365, 50), Point(365, 150), Scalar(0, 255, 255), 5);
						line(frame, Point(380, 50), Point(380, 70), Scalar(0, 255, 255), 5);
						line(frame, Point(380, 88), Point(380, 108), Scalar(0, 255, 255), 5);
						line(frame, Point(380, 126), Point(380, 15), Scalar(0, 255, 255), 5);
                        left_lane_exist=true;
					}
					else if (left_solid_exist)
					{
						putText(frame, "solidline", Point(320, 40), cv::FONT_HERSHEY_DUPLEX, 0.8, cv::Scalar(255, 0, 255), 2);
						line(frame, Point(370, 50), Point(370, 150), Scalar(0, 255, 255), 5);
                        left_lane_exist=true;
					}
					else
					{
						putText(frame, "dashline", Point(320, 40), cv::FONT_HERSHEY_DUPLEX, 0.8, cv::Scalar(255, 0, 255), 2);
						line(frame, Point(370, 50), Point(370, 70), Scalar(0, 255, 255), 5);
						line(frame, Point(370, 88), Point(370, 108), Scalar(0, 255, 255), 5);
						line(frame, Point(370, 126), Point(370, 150), Scalar(0, 255, 255), 5);
                        left_lane_exist=true;
					}
				}

			}

			//保存本帧检测结果
			isDoubleLaneInLastFrame=isDoubleLane;					
			last_solid_point=solidLaneFitted;
			left_isDoubleLaneInLastFrame=left_isDoubleLane;
			left_last_solid_point=left_solidLaneFitted;
			if(isDoubleLane)
			{
				last_dash_point=dashLaneFitted;
			}
			else
			{
				last_dash_point=solidLaneFitted;
			}			
			if(left_isDoubleLane)
			{
				left_last_dash_point=left_dashLaneFitted;
			}
			else
			{
				left_last_dash_point=left_solidLaneFitted;
			}
		}
		else												//扫描到的点不足时，直接重头开始
		{
			heir_num=0;
			frame_num=0;
			last_dash_point.clear();
			last_solid_point.clear();
			left_last_solid_point.clear();
			left_last_dash_point.clear();
			doubleLaneNum=0;
			isDoubleLaneInLastFrame=false;
			left_doubleLaneNum=0;
			left_isDoubleLaneInLastFrame=false;
		}
		if(heir_num>5)
		{
			heir_num=0;
			frame_num=0;
			last_dash_point.clear();
			last_solid_point.clear();
			left_last_solid_point.clear();
			left_last_dash_point.clear();
			doubleLaneNum=0;
			isDoubleLaneInLastFrame=false;
			left_doubleLaneNum=0;
			left_isDoubleLaneInLastFrame=false;
		}
		//rectangle(frame,Rect(150,300,400,160),Scalar(0,0,255),2);
		line(frame,Point(0,ROI_Y),Point(frame.cols-1,ROI_Y),Scalar(0,0,255),2);
		line(frame,Point(0,ROI_Y+ROI_HEIGH),Point(frame.cols-1,ROI_Y+ROI_HEIGH),Scalar(0,0,255),2);
		line(frame,Point(345,ROI_Y),Point(345,ROI_Y+ROI_HEIGH),Scalar(0,0,255),1); 


		if(transRightCols > 0 && transLeftCols > 0 && rightA < 1 && rightA > 0.1 && leftA > -1.5 && leftA < -0.1)
		{
			transMiddleCols = (transRightCols+transLeftCols)/2;
			middleCols = (rightCols+leftCols)/2;
		    circle(frame,Point(middleCols+110,92+270),3,Scalar(0,0,255),-1);
			err = - tan(a_horizon * (transMiddleCols + 110 - 70 - 345) * pi / 180) * sqrt((double)(CAMERA_HEIGHT * CAMERA_HEIGHT + 6000 * 6000)) * 2 / 3 + 5;
//			if(err > -1750 && err < 1750)
            err_exist=true;
		}
        cout << "err = " << err << endl;

//		imshow("VideoTest2",frame);

        //waitKey(20);
//        startWindowThread();
        //imshow("Trans",imgTrans);
		//imgTrans.release();
		//writer << frame;
        //duration=static_cast<double>(getTickCount())-duration;
        //int time_used=duration/getTickFrequency()*1000;			//计算时间
        //cout<<"heir_num="<<heir_num<<endl<<"time_used="<<time_used<<endl;
		
		//waitKey(27);
//		if(acc_flag)
//			waitKey(0);
// 		char c=waitKey(15);
//   		if(c==27)
//            break;
//		if(c==32)
//			acc_flag=!acc_flag;
//		if(c==9)
//		{
//			switch_video=!switch_video;
//			frame_num=0;
//			doubleLaneNum=0;
//			isDoubleLaneInLastFrame=false;
//		}
		scanPoints.clear();					//释放循环里面的变量空间
		dashLanePoints.clear();
		dashLaneFitted.clear();
		solidLanePoints.clear();
		solidLaneFitted.clear();

		leftscanPoints.clear();
		left_solidLanePoints.clear();
		left_solidLaneFitted.clear();
        //cam.release();

    }
      /*
      unsigned char  line_type_l; //solid(true)and dash(false)
      unsigned char  line_type_r;
      double line_dist; //+,vehicle is left to the centerline of two lanes.  -: right
      unsigned char  line_type_l_flag; //solid(true)and dash(false)
      unsigned char  line_type_r_flag;
      double line_dist_flag; //+,vehicle is left to the centerline of two lanes.  -: right
*/
      decisionMake.line_type_l = left_solid_exist;
      decisionMake.line_type_r = solid_exist;
      decisionMake.line_dist = err;
      decisionMake.line_type_l_flag = left_lane_exist;
      decisionMake.line_type_r_flag = lane_exist;
      decisionMake.line_dist_flag = err_exist;

    }
        //destroyWindow("VideoTest");
//        destroyWindow("VideoTest2");
//        capture.release();       //(读取视频)
//        cam.release();
        return 0;
}
