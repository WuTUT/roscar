#ifndef QNODE_HPP_
#define QNODE_HPP_

#include <ros/ros.h>
#include <ros/network.h>
#include <string>
#include <std_msgs/String.h>
#include <iostream>
#include <QThread>
#include <QStringListModel>
using namespace std;
class QNode : public QThread
{
    Q_OBJECT
public:
	QNode();
	virtual ~QNode();
	
	bool init(const std::string &master_url, const std::string &host_url);
	void run();
	void setDirectioninfo(string direction);
	
private:
	string directioninfo;
	ros::Publisher control_publisher;
};

#endif // QNODE_HPP
