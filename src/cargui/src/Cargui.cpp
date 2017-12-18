#include "../include/cargui/Cargui.hpp"

Cargui::Cargui(QWidget *parent)
    : QWidget(parent)
{
    
    imageLabel=new QLabel("?????");
    imageLayout=new QGridLayout();
    imageLayout->addWidget(imageLabel);


    car_forward=new QPushButton();
    car_forward->setText("Forward");
    car_back=new QPushButton();
    car_back->setText("Back");
    car_left=new QPushButton();
    car_left->setText("Left");
    car_right=new QPushButton();
    car_right->setText("Right");

    hostlabel=new QLabel("host url");
    masterlabel=new QLabel("master url");
    hosturl=new QLineEdit();
    masterurl=new QLineEdit();
    hosturl->setText("192.168.31.210");
    masterurl->setText("http://192.168.31.210:11311");
    car_connect=new QPushButton();
    car_connect->setText("connect");
    urlinfo_Layout=new QGridLayout();
    urlinfo_Layout->addWidget(hostlabel,0,0,1,1);
    urlinfo_Layout->addWidget(masterlabel,1,0,1,1);
    urlinfo_Layout->addWidget(hosturl,0,1,1,1);
    urlinfo_Layout->addWidget(masterurl,1,1,1,1);
    urlinfo_Layout->addWidget(car_connect,2,1,1,1);

    QObject::connect(car_forward,SIGNAL(clicked()),this,SLOT(forward_clicked()));
    QObject::connect(car_back,SIGNAL(clicked()),this,SLOT(back_clicked()));
    QObject::connect(car_left,SIGNAL(clicked()),this,SLOT(left_clicked()));
    QObject::connect(car_right,SIGNAL(clicked()),this,SLOT(right_clicked()));
    QObject::connect(car_connect,SIGNAL(clicked()),this,SLOT(connect_clicked()));
    carcontrol_Layout=new QGridLayout();
    carcontrol_Layout->addWidget(car_forward,0,1,1,1);
    carcontrol_Layout->addWidget(car_left,1,0,1,1);
    carcontrol_Layout->addWidget(car_back,1,1,1,1);
    carcontrol_Layout->addWidget(car_right,1,2,1,1);
    mainLayout=new QGridLayout(this);
    
    
    mainLayout->setMargin(0);

    mainLayout->addLayout(imageLayout,0,0,5,5);
    mainLayout->addLayout(carcontrol_Layout,4,4,2,2);
    mainLayout->addLayout(urlinfo_Layout,1,4,1,2);
   
    this->setLayout(mainLayout);
    setMinimumSize(1500,1000);
    setMaximumSize(1500,1000);
}

void Cargui::forward_clicked(){
    cout<<'f'<<endl;
    
    control_q.setDirectioninfo(string("f"));
}
void Cargui::back_clicked(){
    cout<<'b'<<endl;
    control_q.setDirectioninfo(string("b"));
}
void Cargui::right_clicked(){
    cout<<'r'<<endl;
    control_q.setDirectioninfo(string("r"));
}
void Cargui::left_clicked(){
    cout<<'l'<<endl;
    control_q.setDirectioninfo(string("l"));
}


void Cargui::imgcallback(const sensor_msgs::CompressedImageConstPtr& msg) {
    cout<<"111"<<endl;

    cv_bridge::CvImagePtr cv_ptr;  
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::RGB8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
    conversion_mat_ = cv_ptr->image;
    QImage image(conversion_mat_.data, conversion_mat_.cols, conversion_mat_.rows, conversion_mat_.step[0], QImage::Format_RGB888);
    imageLabel->setPixmap(QPixmap::fromImage(image));  
    imageLabel->resize(image.size());  
    imageLabel->show();  
    //ros::spin();
}


bool Cargui::init(const std::string &master_url, const std::string &host_url) {
	std::map<std::string,std::string> remappings;
	remappings["__master"] = master_url;
	remappings["__hostname"] = host_url;
	ros::init(remappings,"imageshow");
	if ( ! ros::master::check() ) {
		return false;
	}
	ros::start(); // explicitly needed since our nodehandle is going out of scope.
	ros::NodeHandle n;
	// Add your ros communications here.
    //image_transport::ImageTransport it(n);
	imgshow_subscriber = n.subscribe("/raspicam_node/compressed",1,&Cargui::imgcallback,this);
	
    return true;
}


void Cargui::connect_clicked(){
    if(!control_q.init(masterurl->text().toStdString(),hosturl->text().toStdString())){
        cout<<"connect error"<<endl;
    }
    else{
        cout<<"connect success"<<endl;
    }
    if(!Cargui::init(masterurl->text().toStdString(),hosturl->text().toStdString())){
        cout<<"imgshowconnect error"<<endl;
    }
    else{
        cout<<"imgshowconnect success"<<endl;
    }
}

Cargui::~Cargui()
{

}