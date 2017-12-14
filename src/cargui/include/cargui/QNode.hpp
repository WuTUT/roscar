#ifndef QNODE_HPP_
#define QNODE_HPP_

#include <ros/ros.h>
#include <ros/network.h>
#include <string>
#include <std_msgs/String.h>
#include <sstream>
#include <QThread>
#include <QStringListModel>

class QNode : public QThread
{
    Q_OBJECT
public:
	QNode();
	virtual ~QNode();
	
	bool init(const std::string &master_url, const std::string &host_url);
	void run();

	
private:
	
	ros::Publisher control_publisher;
};

#endif // QNODE_HPP
