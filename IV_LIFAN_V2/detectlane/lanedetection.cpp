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

vector<Point2f> last_dash_point;		//��һ֡�Ҳ����ߵ���Ͻ��
vector<Point2f> last_solid_point;		//��һ֡�Ҳ�ʵ�ߵ���Ͻ��
vector<Point2f> left_last_solid_point;	//��һ֡���ʵ�ߵ���Ͻ��
vector<Point2f> left_last_dash_point;	//��һ֡������ߵ���Ͻ��

int heir_num=0;							//�̳���һ֡�Ĵ���
int doubleLaneNum=0;					//�Ҳ���������˫�ߵ�֡��
int left_doubleLaneNum=0;				//�����������˫�ߵ�֡��
bool isDoubleLaneInLastFrame=false;		//�Ҳ���һ֡�Ƿ����˫�ߵı�־
bool left_isDoubleLaneInLastFrame=false;	//�����һ֡�Ƿ����˫�ߵı�־
int frame_num=0;						//��ǰ�����Ч��֡���ۼ�ֵ
int total_frame=0;						//��֡��
vector<int> dash_err;					//��¼�Ҳ����߱�֡����һ֡ƫ���vector
vector<int> solid_err;					//��¼�Ҳ�ʵ�߱�֡����һ֡ƫ���vector
vector<int> left_dash_err;				//��¼������߱�֡����һ֡ƫ���vector
vector<int> left_solid_err;				//��¼���ʵ�߱�֡����һ֡ƫ���vector
bool acc_flag=false;					//���ٵı�־
bool switch_video=false;				//�л���Ƶ�ı�־
float a_horizon = 78.0 / 640;                     //����ϵ��
//int thresh = 240;

bool solid_exist = 0;							//�Ҳ����ʵ�ߵı�־
bool left_solid_exist = 0;					//������ʵ�ߵı�־
bool isDoubleLane;						//��ǰ֡�Ƿ�Ϊ˫�����ı�־
bool left_isDoubleLane;					//��൱ǰ֡�Ƿ�Ϊ˫�����ı�־
bool thresholdFlag;                     //�ж���ֵ�ָ��Ƿ���Ҫ�л���־λ
bool lane_exist = 0;                              //��ǰ�Ƿ��⵽�Ҳ೵����
bool left_lane_exist =0 ;                      //��ǰ�Ƿ��⵽��೵����
bool err_exist = 0 ;                               //��ǰ�Ƿ�������ƫ������
float err = 0;                          //�복������ƫ��ֵ

Mat frame;			//ͼƬ��ȡ

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
 //   VideoCapture capture("Video_test1_20171110.avi");//("Video_test1_1025||video1_2017918.avi");    //(��ȡ��Ƶ)
    //VideoWriter writer("Video_test.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25.0, Size(width,height));
    //VideoWriter writer1("Video_test1.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25.0, Size(width,height));
    //VideoWriter writer2("Video_test2.avi", CV_FOURCC('M', 'P', '4', '2'), 25.0, Size(ROI_WIDTH,ROI_HEIGH),0);
	//VideoCapture capture("test_video4.avi");									//��Ƶ�࣬��ȡ��Ƶ�ļ���֧��.avi�������ø�ʽת�����Ƚ���ʽת��
	//VideoCapture capture("C:\\Users\\ASUS\\Desktop\\roadtest2.wmv");			
	//VideoCapture capture(1);			//��ȡ��Ƶ�ļ�
    //sleep(20);
    //VideoCapture capture1("video3.avi");			//��Ƶ�࣬��ȡ��Ƶ�ļ���֧��.avi�������ø�ʽת�����Ƚ���ʽת��(��ȡ��Ƶ)
    //double rate= capture.get(CV_CAP_PROP_FPS);		//VideoCapture::get ����ָ����Ƶ�����ز�����Ϣ CV_CAP_PROP_FPS��ȡ֡�ʣ�����ʱ����������CV_CAP_PROP����ctrl+j���Ҷ�Ӧ��(��ȡ��Ƶ)
    //double duration;

	Mat ROI;			//��ȡROI
	Mat BW;				//��ֵͼ
	Mat bw;
	Mat img;
	Mat gray;			//�Ҷ�ͼ
	Mat leftPolyFactor;   //�󳵵����б�ʲ���                                      
	Mat rightPolyFactor;   //�ҳ������б�ʲ���
	//Mat tshbROI;
    //namedWindow("VideoTest");			//��ʾ��ֵͼ��
	//createTrackbar("��ֵ:","VideoTest",&thresh,255,NULL); 
//	namedWindow("VideoTest2");			//��ʾ�����
	//namedWindow("right_ROI");			//��ʾ�����
	//namedWindow("left_ROI");			//��ʾ�����
    //bool predict_label=true;			//�����߲��ֽ���Ԥ��ı�־

  //  while(1)
    {
		int transRightCols = 0;                      //������������������
        int transLeftCols = 0;
		int rightCols = 0;
		int leftCols = 0;
		int transMiddleCols = 0;
		int middleCols = 0;
		Mat imgTrans = Mat::zeros(110,640,CV_8UC1);           //͸�ӱ任ͼ
		//if(CameraQueryImage(0,(unsigned char*)image->imageData, &len,CAMERA_IMAGE_BMP)==API_OK);
        //duration=static_cast<double>(getTickCount());			//��ʱ
		//namedWindow("Canny");
//        int flag;                     //(��ȡ��Ƶ)
//       if(!switch_video)				//�Ƿ��л���Ƶ��bool switch_video = false;
//        capture.read(frame);	//VideoCapture::readץס�����벢������һ����Ƶ֡
        //else							//�л�
            //flag=capture1.read(frame);	//VideoCapture::readץס�����벢������һ����Ƶ֡
        /*if(!flag)
            break;     */                 //(��ȡ��Ƶ)
		//flip(frame,frame,-1);
		//Mat frame = cvarrToMat(image);
		//Mat frameRaw = cvarrToMat(image);
        cam >> frame;
		//writer1 << frame;

        solid_exist=false;							//�Ҳ����ʵ�ߵı�־
        left_solid_exist=false;					//������ʵ�ߵı�־
        //float VP;
        isDoubleLane=false;						//��ǰ֡�Ƿ�Ϊ˫�����ı�־
        left_isDoubleLane=false;					//��൱ǰ֡�Ƿ�Ϊ˫�����ı�־
        thresholdFlag = false;                     //�ж���ֵ�ָ��Ƿ���Ҫ�л���־λ
        lane_exist=false;                              //��ǰ�Ƿ��⵽�Ҳ೵����
        left_lane_exist=false;                      //��ǰ�Ƿ��⵽��೵����
        err_exist=false;                               //��ǰ�Ƿ�������ƫ������
        err = false;
        //bool left_thresholdFlag = false;                //�ж���ֵ�ָ��Ƿ���Ҫ�л���־λ

      if(frame.cols>0&&frame.rows>0)
       {
//           bool flag;
        //flag = cam.read(frame);
		frame_num++;											//��Чѭ�����ۼ�֡��
		total_frame++;											//��֡������ǰ֡��
		i++;
        //cout<<"frame_num="<<frame_num<<endl;					//�����Чѭ�����ۼ�֡��
        //cout<<"total_frame_num="<<total_frame<<endl;			//�����֡������ǰ֡��
		ROI=frame(Rect(ROI_X,ROI_Y,ROI_WIDTH,ROI_HEIGH));		//frame�е�ROI����Data.h�ļ��У�����ROI�������Ͻ�����ROI_X=200��ROI_Y=260����ROI_WIDTH=420����ROI_HEIGH=120
 		ROI.copyTo(img);										//��ROI�����Ƶ�img�У���img���в�����ʦ���õ�img=ROI.clone();�������ܻ�����ڴ������.cppyTo���ȴ���һ������
		
		cvtColor(img,gray,CV_RGB2GRAY);	 						//�Ҷȱ任
        //double otsu_thresh;
		//threshold(gray,BW,double(thresh),255,THRESH_BINARY);
		//threshold(gray,BW,0,255,CV_THRESH_OTSU);				//��ֵ��
		int Threshold = OstuThreshold(gray);
		threshold(gray, BW, Threshold, 255, CV_THRESH_BINARY);
		//adaptiveThreshold(gray,BW,255,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY_INV,25,10);
		//Mat element = getStructuringElement(MORPH_RECT,Size(3,3));
		//morphologyEx(BW,BW,MORPH_OPEN,element);
		//imshow("Gray",gray);

		vector<Point2i> scanPoints;						//�Ҳ�ɨ��õ��ĵ�
		vector<Point2f> dashLanePoints;					//�Ҳ���˺�����߳�����
		vector<Point2f> solidLanePoints;				//�Ҳ���˺��ʵ�߳�����
		vector<Point2f> dashLaneFitted;					//�Ҳ�������Ͻ��
		vector<Point2f> solidLaneFitted;				//�Ҳ�ʵ����Ͻ��

		vector<Point2i> leftscanPoints;					//���ɨ��õ��ĵ�
		vector<Point2f> left_dashLanePoints;			//�����˺�����߳�����
		vector<Point2f> left_solidLanePoints;			//�����˺��ʵ�߳�����
		vector<Point2f> left_dashLaneFitted;			//���������Ͻ��
		vector<Point2f> left_solidLaneFitted;			//���ʵ����Ͻ��
		
		
		/****************�Ҳ�ɨ��߽�****************/
		int r_bound=0;									
		if(frame_num>1)
		{
			for(size_t i=0;i<left_last_solid_point.size();i++)		//long unsigned int��size_t��ȫ��Ӧ����size type������˵��һ��������¼��С���������͡������forѭ�������ҵ��󳵵������Ҳ�㣬������r_bound
			{
				int temp=left_last_solid_point[i].x;
				if(temp>r_bound)
					r_bound=temp;
			}
			r_bound+=5;												//��ǰ֡���Ҽ����ʼ�߽��r_bound=��һ֡�󳵵������ҵ�+5
		}
		else														//��һ֡��ɨ����ʼ�߽�ΪROI���м�
		{
			r_bound=245+20;
		}
		if(r_bound<1||r_bound>BW.cols-1)							//�����������������r_bound��ֵ����ͼ���ڣ������¶�������ʼ�߽�r_boundΪROI���м䣨��ʱΪ���ߵ������
		{
			r_bound=245+20;
		}

		/*********************���ɨ��߽�***************/
		int l_bound=680;											
		if(frame_num>1)												
		{
			for(size_t i=0;i<last_solid_point.size();i++)			//���forѭ�������ҵ��Ҳ�ʵ�����ߵ�����㣬����ֵ��l_bound
			{
				int temp=last_solid_point[i].x;
				if(temp<l_bound)
					l_bound=temp;
			}
			l_bound-=5;												//��ǰ֡���󳵵�����յ�l_bound=��һ֡���Ҳ�ʵ�����ߵ������-5
		}
		else														//��һ֡���󳵵�����յ�ΪROI�����м�
		{
			l_bound=245-20;									
		}
		if(l_bound<1||l_bound>BW.cols-1)							//�����������������L_bound��ֵ����ͼ���ڣ������¶����󳵵�����յ�l_boundΪROI���м䣨��ʱΪ�Ҷ��ߵ������
		{
			l_bound=245-20;
		}
		//imshow("VideoTest100",BW);
		//writer2 << BW;

		//�ж���ֵ�ָ�ģʽ�Ƿ����
		thresholdFlag = thresholdModeSwitch(BW,l_bound,thresholdFlag);
		if(thresholdFlag) //������Ӧ��ֵ�������⣬ͨ������Ϊ�̶���ֵ
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


		//͸�ӱ任
		//tsbh(BW,tsbhImg);
		//imshow("tsbh",tsbhImg);

		//��ɨ�����
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


		//if(thresholdFlag) //������Ӧ��ֵ�������⣬ͨ������Ϊ�̶���ֵ
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

		
		//�����ʱ��ֱ�����¿�ʼ
		if(leftscanPoints.size()>0&&scanPoints.size()>0) 
		{
			if(frame_num==1)
			{
				vector<Point2i> mirror_leftscanPoints;
                for(unsigned int i=0;i<leftscanPoints.size();i++)
				{
					mirror_leftscanPoints.push_back(Point2f(leftscanPoints[i].x,ROI_WIDTH-leftscanPoints[i].y));
				}
				//����ֵͼ����
				Mat mirror_BW=Mat::zeros(BW.rows, BW.cols, CV_8UC1);
				for(int i=0;i<BW.rows;i++)
				{
					uchar* data= BW.ptr<uchar>(i);//ȡBWͼ��ÿ�е��׵�ַ
					uchar* data1= mirror_BW.ptr<uchar>(i);//ȡ����BWͼ��ÿ�е��׵�ַ
					for(int j=0;j<BW.cols;j++)
					{
						data1[j]=data[ROI_WIDTH-1-j];
					}
				}
   				isDoubleLane=FeatureExtraction(scanPoints,doubleLaneNum,isDoubleLaneInLastFrame,dashLanePoints,solidLanePoints,BW);	//��ȡ�Ҳ೵����������
				left_isDoubleLane=FeatureExtraction(mirror_leftscanPoints,left_doubleLaneNum,left_isDoubleLaneInLastFrame,
					left_dashLanePoints,left_solidLanePoints,mirror_BW);															//��ȡ��೵��������

				if(dashLanePoints.size()==0&&isDoubleLane)
				{
					isDoubleLane=false;
				}
				if(left_dashLanePoints.size()==0&&left_isDoubleLane)
				{
					left_isDoubleLane=false;
				}
				//��������˵ĵ㻹ԭ
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
				//�Ե����"����"���õ���������͸�ӵ�Ч��
                for(unsigned int i=0;i<leftscanPoints.size();i++)
				{
					leftscanPoints[i].y-=((int)(left_last_solid_point[leftscanPoints[i].x].x)+10);
				}
                for(unsigned int i=0;i<scanPoints.size();i++)
				{
					scanPoints[i].y-=((int)(last_dash_point[scanPoints[i].x].x)+10);
				}
				//������Ļ����ϣ��Ե����ȥ��;���
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
				//������ĵ���л�ԭ
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

			if(left_solidLanePoints.size()>4||solidLanePoints.size()>4)				//������������㹻ʱ����ֱ�ӽ������ //ԭ����Ϊ if(left_solidLanePoints.size()>4&&solidLanePoints.size()>4)
			{
                if(isDoubleLane||solidLanePoints.size()>0.45*ROI_HEIGH)					//�ж�����Ƿ����ʵ��
				{
					solid_exist=true;
				}
                if(left_isDoubleLane||left_solidLanePoints.size()>0.45*ROI_HEIGH)		//�ж��Ҳ��Ƿ����ʵ��
				{
					left_solid_exist=true;
				}
				//��������ϲ���
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
						curvefitting1(solidLaneFitted,solidLanePoints,4);			//ʵ�����
					}
					else
					{
						curvefitting1(solidLaneFitted,solidLanePoints,4);		//ʵ�����
					}
					
					if(frame_num>1)							
					{
						//�ж϶���������һ֡�������
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
				else if((!solid_exist)&&(!left_solid_exist))                       //ԭ����Ϊ else
				{
					if(solidLanePoints.size()>0 && left_solidLanePoints.size()>0)
					{
						curvefitting(solidLaneFitted,solidLanePoints,1);				
					    curvefitting(left_solidLaneFitted,left_solidLanePoints,1);
					}
					if(solidLanePoints.size()>0 && left_solidLanePoints.size()==0)       //�����ж�
					{
						curvefitting(solidLaneFitted,solidLanePoints,1);
					}
					if(solidLanePoints.size()==0 && left_solidLanePoints.size()>0)       //�����ж�
					{
						curvefitting(left_solidLaneFitted,left_solidLanePoints,1);
					}
				}
				
				//��¼���ʵ������һ֡��ƫ��
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
			else					//���˺�㲻��ʱ�Ĵ�������������ǵ�һ֡��ֱ�����¿�ʼ������̳���һ֡
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
					for(size_t i=0;i<solidLaneFitted.size();i++)								//��ʾ��Ͻ��
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
				//for(int i=0;i<ROI_HEIGH/5;i++)    //�Գ�������ʻ������ʾ
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

			//���汾֡�����
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
		else												//ɨ�赽�ĵ㲻��ʱ��ֱ����ͷ��ʼ
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
        //int time_used=duration/getTickFrequency()*1000;			//����ʱ��
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
		scanPoints.clear();					//�ͷ�ѭ������ı����ռ�
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
//        capture.release();       //(��ȡ��Ƶ)
//        cam.release();
        return 0;
}
