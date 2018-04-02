#include "ros/ros.h"
#include "std_msgs/String.h"


#include <iostream>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
using namespace cv;
using namespace std;



//-----------------------------------【全局函数声明部分】--------------------------------------
//  描述：全局函数声明
//-----------------------------------------------------------------------------------------------
Mat img;
int CrL = 5;    //轨迹条滑块对应的值
int CrH = 15;
int CbL = 5;
int CbH = 20;
int pub_count=0;
int fignum[5];
int realnum;
void skinExtract1(const Mat &frame, Mat &skinArea); //选出肤色
void skinExtract2(const Mat &frame, Mat &skinArea); //选出肤色
bool isCover(Point a,Point b);
int mymax(int a[]);
int main(int argc, char **argv)
{
    for(int ci=0;ci<5;ci++)
    fignum[ci]=0;
	 int num=0;
     
    Mat skinArea,frameROI;
    
    Mat frame;
    VideoCapture capture(0);
  
   
    

    if (!capture.isOpened())
    {
        cout<<"No camera!\n"<<endl;
        return -1;
    }
    

	ros::init(argc, argv, "gesturecon");

	ros::NodeHandle n;

	
	ros::Publisher chatter_pub = n.advertise<std_msgs::String>("gesturecontrol", 1);

	ros::Rate loop_rate(10);

	
	int count = 0;
	while (ros::ok())
	{
		capture >> frame;
        
        if (frame.empty())
            cout<<"no frame"<<endl;
        
        num = 0;
        Mat show_img;
        
       GaussianBlur(frame, frame, Size(3, 3), 0);  //高斯滤波
        rectangle(frame,Rect(120,100,300,300), Scalar(255,0,0),4,1,0);
        frameROI = frame(Rect(120,100,300,300));
        imshow("frame", frame);
        
        skinArea.create(frameROI.rows, frameROI.cols, CV_8UC1);
        skinExtract2(frameROI, skinArea);
        frameROI.copyTo(show_img, skinArea);
        
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        
        //寻找轮廓
        findContours(skinArea, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
        
        if(contours.size() == 0 || contours.size()==1)
            continue;
         drawContours(show_img, contours, 1, Scalar(0, 0, 255), 2, 8, hierarchy);
        
        vector<vector<Point> >hull(contours.size());
        for (int i = 0; i < contours.size(); i++){
            convexHull(Mat(contours[i]), hull[i], false);
            //drawContours(show_img, hull, i,Scalar(0, 0, 255), 2, 8, vector<Vec4i>(), 0, Point());
        }
        // 找到最大的轮廓
        int index = 0;
        double area, maxArea(0);
        for (int i=0; i < hull.size(); i++)
        {
            area = contourArea(Mat(hull[i]));
            if (area > maxArea)
            {
                maxArea = area;
                index = i;
            }
        }
        drawContours(show_img, contours, index,Scalar(0, 0, 255), 2, 8, vector<Vec4i>(), 0, Point());
        drawContours(show_img, hull, index,Scalar(0, 0, 255), 2, 8, vector<Vec4i>(), 0, Point());
        
        //drawContours(frame, contours, 1, Scalar(0, 0, 255), 2, 8, hierarchy );
        Moments moment = moments(skinArea, true);
        Point center(moment.m10/moment.m00, moment.m01/moment.m00);
        circle(show_img, center, 8 ,Scalar(0, 0, 255), CV_FILLED);
        
        
       // 寻找指尖1
        vector<Point> couPoint = contours[index];
        vector<Point> couPoint2 = hull[index];
        vector<Point> fingerTips;
        Point tmp,tmp2;
        int max(0), count(0), notice(0);
        for (int i = 0; i < couPoint.size(); i++)
        {
            tmp = couPoint[i];
            int dist = (tmp.x - center.x) * (tmp.x - center.x) + (tmp.y - center.y) * (tmp.y - center.y);
            
            for (int j=0; j < couPoint2.size(); j++) {
                tmp2 = couPoint2[j];
                if ( isCover(tmp,tmp2) && dist > max )
                {
                    max = dist;
                    notice = i;
                }
            }
            
            
            // 计算最大值保持的点数，如果大于 （这个值需要设置，本来想根据max值来设置，
            // 但是不成功，不知道为何），那么就认为这个是指尖
            if (dist != max)
            {
                count++;
                if (count > 80)
                {
                    count = 0;
                    max = 0;
                    bool flag = false;
                    // 低于手心的点不算
                    if (center.y < couPoint[notice].y )
                        continue;
                    // 离得太近的不算
                    for (int j = 0; j < fingerTips.size(); j++)
                    {
                        if (abs(couPoint[notice].x - fingerTips[j].x) < 11)
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
        fignum[num]++;
        

		std_msgs::String msg;

		std::stringstream ss;
		

		

		if(pub_count%25==0){
        ss << mymax(fignum);
		msg.data = ss.str();    
        ROS_INFO("%s", msg.data.c_str());
		chatter_pub.publish(msg);
        for(int ci=0;ci<5;ci++)
    fignum[ci]=0;
        ros::spinOnce();

		loop_rate.sleep();
        }
        
        

        pub_count++;
		
		
	}

	return 0;
}

int mymax(int a[]){
    int maxi=0;
    int maxnum=-1;
    for(int i=0;i<5;i++){
        if(maxnum<a[i]){
        maxnum=a[i];
        maxi=i;
        }
        
    }
    cout<<"maxi is "<< maxi<<endl;
    return maxi;
}
//肤色提取，skinArea为二值化肤色图像
void skinExtract1(const Mat &frame, Mat &skinArea)
{
    Mat YCbCr;
    vector<Mat> planes;
    
    //转换为YCrCb颜色空间
    cvtColor(frame, YCbCr, CV_RGB2YCrCb);
    //将多通道图像分离为多个单通道图像
    split(YCbCr, planes);
    
    Mat target = planes[2];
    threshold(target, target, 0, 255, CV_THRESH_OTSU);              // 二值化处理
    //imshow("target", target);
    
    Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));   // 开运算去除噪点
    morphologyEx(target, target, MORPH_OPEN, element);
    //imshow("open", target);
    target.copyTo(skinArea);
}

void skinExtract2(const Mat &frame, Mat &skinArea)
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
    
    //人的皮肤颜色在YCbCr色度空间的分布范围:100<=Cb<=118, 138<=Cr<=175
    for( ; it_Cb != it_Cb_end; ++it_Cr, ++it_Cb, ++it_skin)
    {
        if (CrL+130 <= *it_Cr &&  *it_Cr <= CrH+160 && 90+CbL <= *it_Cb &&  *it_Cb <= 110+CbH )
            *it_skin = 255;
        else
            *it_skin = 0;
    }
    
    //膨胀和腐蚀，膨胀可以填补凹洞（将裂缝桥接），腐蚀可以消除细的凸起（“斑点”噪声）
    dilate(skinArea, skinArea, Mat(2, 2, CV_8UC1), Point(-1, -1),2);
    erode(skinArea, skinArea, Mat(3, 3, CV_8UC1), Point(-1, -1),1);
    dilate(skinArea, skinArea, Mat(2, 2, CV_8UC1), Point(-1, -1),2);
    
}
bool isCover(Point a,Point b){
    if(a.x == b.x && a.y == b.y)
        return true;
    else
        return false;
}