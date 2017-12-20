#ifndef CARGUI_HPP
#define CARGUI_HPP

#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <QLabel>
#include <QImage>
#include <QPixmap>  
#include <QLineEdit>
#include <iostream>
#include <sensor_msgs/CompressedImage.h>
#include <sensor_msgs/image_encodings.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <image_transport/image_transport.h>
#include "ControlQNode.hpp"
using namespace cv;
using namespace std;
using namespace Qt;
class Cargui : public QWidget
{
    Q_OBJECT

public:
    Cargui(QWidget *parent = 0);
    ~Cargui();
    
private:

    QLabel* imageLabel;
    QGridLayout* imageLayout;

    QGridLayout* mainLayout;

    QGridLayout* carcontrol_Layout;
    QPushButton* car_forward;
    QPushButton* car_back;
    QPushButton* car_left;
    QPushButton* car_right;

   
    QLabel* hostlabel;
    QLabel* masterlabel;
    QLineEdit* hosturl;
    QLineEdit* masterurl;
    QGridLayout* urlinfo_Layout;
    QPushButton* car_connect;

    ControlQNode control_q;
    
    
    image_transport::Subscriber imgshow_subscriber;
    cv::Mat conversion_mat_;
    

    bool init(const std::string &master_url, const std::string &host_url);
    void imgcallback(const sensor_msgs::ImageConstPtr& msg);
    void callback(const std_msgs::String::ConstPtr& msg);
public Q_SLOTS:
    void forward_clicked();
    void back_clicked();
    void left_clicked();
    void right_clicked();
    void connect_clicked();
};

#endif // CARGUI_HPP
