#include "../include/cargui/ControlQNode.hpp"

ControlQNode::ControlQNode(){
	this->directioninfo="";
	cout<<"one thread is created"<<endl;
}

ControlQNode::~ControlQNode() {
    if(ros::isStarted()) {
      ros::shutdown(); // explicitly needed since we use ros::start();
      ros::waitForShutdown();
    }
	wait();
	cout<<"one thread is deleted"<<endl;
}



bool ControlQNode::init(const std::string &master_url, const std::string &host_url) {
	std::map<std::string,std::string> remappings;
	remappings["__master"] = master_url;
	remappings["__hostname"] = host_url;
	ros::init(remappings,"guicontrol");
	if ( ! ros::master::check() ) {
		return false;
	}
	ros::start(); // explicitly needed since our nodehandle is going out of scope.
	ros::NodeHandle n;
	
	control_publisher = n.advertise<std_msgs::String>("direction", 1000);
	start();
	return true;
}

void ControlQNode::setDirectioninfo(string direction){
	this->directioninfo=direction;
}

void ControlQNode::run() {
	ros::Rate loop_rate(1);
	std_msgs::String msg;
	
	while ( ros::ok() ) {
		
		
		if(this->directioninfo!=""){
		msg.data = directioninfo;
		control_publisher.publish(msg);
		cout<<directioninfo<<"!"<<endl;
		this->directioninfo="";
		
		ros::spinOnce();
		loop_rate.sleep();
		}
		
		
	}
	std::cout << "Ros shutdown, proceeding to close the gui." << std::endl;
	
}

