#include "ros/ros.h"
#include "std_msgs/String.h"

#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void skinExtract(const Mat &frame, Mat &skinArea); //选出肤色

//-----------------------------------【全局函数声明部分】--------------------------------------
//  描述：全局函数声明
//-----------------------------------------------------------------------------------------------
Mat img;
int CrL = 5;    //轨迹条滑块对应的值
int CrH = 5;
int CbL = 5;
int CbH = 5;

int main(int argc, char *argv[])
{
	 
	ros::init(argc, argv, "gesturecontrol");

	
	ros::NodeHandle n;

	
	ros::Publisher chatter_pub = n.advertise<std_msgs::String>("gesture", 1000);

	ros::Rate loop_rate(10);

	Mat frame, frameROI, skinArea;
    VideoCapture capture;
    int num=0;
    
    capture.open(0);
    if (!capture.isOpened())
    {
        cout<<"No camera!\n"<<endl;
        return -1;
    }

	int count = 0;
	while (ros::ok())
	{
		
		capture >> frame;
        
        if (frame.empty())
            break;
        
        num = 0;
        Mat show_img;
        
        GaussianBlur(frame, frame, Size(3, 3), 0);  //高斯滤波
        rectangle(frame,Rect(200,200,600,300), Scalar(255,0,0),4,1,0);
        frameROI = frame(Rect(200,200,600,300));
        imshow("frame", frame);
        
        skinArea.create(frameROI.rows, frameROI.cols, CV_8UC1);
        skinExtract(frameROI, skinArea);
        frameROI.copyTo(show_img, skinArea);
        
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        
        //寻找轮廓
        findContours(skinArea, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
        
        if(contours.size() == 0 || hierarchy.size() == 0)
            continue;
        
        // 找到最大的轮廓
        int index = 0;
        double area, maxArea(0);
        for (int i=0; i < contours.size(); i++)
        {
            area = contourArea(Mat(contours[i]));
            if (area > maxArea)
            {
                maxArea = area;
                index = i;
            }
        }
        
        
        //drawContours(frame, contours, 1, Scalar(0, 0, 255), 2, 8, hierarchy );
        
        Moments moment = moments(skinArea, true);
        Point center(moment.m10/moment.m00, moment.m01/moment.m00);
        circle(show_img, center, 8 ,Scalar(0, 0, 255), CV_FILLED);
        
        // 寻找指尖1
        vector<Point> couPoint = contours[index];
        vector<Point> fingerTips;
        Point tmp;
        int max(0), count(0), notice(0);
        for (int i = 0; i < couPoint.size(); i++)
        {
            tmp = couPoint[i];
            int dist = (tmp.x - center.x) * (tmp.x - center.x) + (tmp.y - center.y) * (tmp.y - center.y);
            if (dist > max)
            {
                max = dist;
                notice = i;
            }
            
            // 计算最大值保持的点数，如果大于 （这个值需要设置，本来想根据max值来设置，
            // 但是不成功，不知道为何），那么就认为这个是指尖
            if (dist != max)
            {
                count++;
                if (count > 100)
                {
                    count = 0;
                    max = 0;
                    bool flag = false;
                    // 低于手心的点不算
                    if (center.y < couPoint[notice].y+10 )
                        continue;
                    // 离得太近的不算
                    for (int j = 0; j < fingerTips.size(); j++)
                    {
                        if (abs(couPoint[notice].x - fingerTips[j].x) < 20)
                        {
                            flag = true;
                            break;
                        }
                    }
                    if (flag) continue;
                    num++;
                    fingerTips.push_back(couPoint[notice]);
                    circle(show_img, couPoint[notice], 6 ,Scalar(0, 255, 0), CV_FILLED);
                    line(show_img, center, couPoint[notice], Scalar(255, 0, 0), 2);
                }
            }
        }
        
        
        if ( cvWaitKey(20) == 'q' )
            break;
        
        imshow("show_img", show_img);
        
        cout << "Number of Fingers is " << num << endl;



		std_msgs::String msg;

		std::stringstream ss;
		ss << num;
		msg.data = ss.str();

		ROS_INFO("%s", msg.data.c_str());
		chatter_pub.publish(msg);

		ros::spinOnce();

		loop_rate.sleep();
		
	}

	return 0;
}

void skinExtract(const Mat &frame, Mat &skinArea)
{
    Mat YCbCr;
    vector<Mat> planes;
    
    //转换为YCrCb颜色空间
    cvtColor(frame, YCbCr, CV_RGB2YCrCb);
    //将多通道图像分离为多个单通道图像
    split(YCbCr, planes);
    
     //运用迭代器访问矩阵元素
     MatIterator_<uchar> it_Cb = planes[1].begin<uchar>(),
     it_Cb_end = planes[1].end<uchar>();
     MatIterator_<uchar> it_Cr = planes[2].begin<uchar>();
     MatIterator_<uchar> it_skin = skinArea.begin<uchar>();
    
    namedWindow("show",1);
    
    //创建轨迹条
    createTrackbar("CrL：","show", &CrL,20);
    createTrackbar("CrH：","show", &CrH,20);
    createTrackbar("CbL：","show", &CbL,20);
    createTrackbar("CbH：","show", &CbH,20);
     
    //人的皮肤颜色在YCbCr色度空间的分布范围: 138<=Cr<=175; 100<=Cb<=118
     for( ; it_Cb != it_Cb_end; ++it_Cr, ++it_Cb, ++it_skin)
     {
     if (CrL+133 <= *it_Cr &&  *it_Cr <= CrH+170 && CbL+95 <= *it_Cb &&  *it_Cb <= CbH+112)
     *it_skin = 255;
     else
     *it_skin = 0;
     }
     
     //膨胀和腐蚀，膨胀可以填补凹洞（将裂缝桥接），腐蚀可以消除细的凸起（“斑点”噪声）
     dilate(skinArea, skinArea, Mat(5, 5, CV_8UC1), Point(-1, -1),2);
     erode(skinArea, skinArea, Mat(5, 5, CV_8UC1), Point(-1, -1),1);

}
