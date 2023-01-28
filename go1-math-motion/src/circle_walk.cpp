#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include "sensor_msgs/Joy.h"
#include <stdio.h>
float ros_vel[3];
float ros_joy[3];
double max_ang_vel = 0.5;
double max_lin_vel = 0.7;

void JOYCallback(const sensor_msgs::Joy::ConstPtr& joymsg){
  ros_joy[0] = joymsg->axes[3];
  ros_joy[1] = joymsg->axes[1];
  ros_joy[2] = joymsg->axes[0];

  for(int i=0;i<3;i++){
   if(i != 0){
     ros_vel[i] = max_lin_vel*ros_joy[i];
   }
   else{
     ros_vel[i] = max_ang_vel*ros_joy[i];
   }
  }
 }

int main(int argc, char **argv){
	ros::init(argc, argv, "circle_walk");
	ros::NodeHandle nh;
	ros::Rate loop_rate(500);

	ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
  ros::Subscriber joymsg = nh.subscribe("joy", 100, JOYCallback);

	geometry_msgs::Twist twist;

	while (ros::ok())
	{

        twist.linear.x = ros_vel[1]; // radius (meters)
        twist.linear.y = ros_vel[2];
        twist.linear.z = 0;
        twist.angular.x = 0;
        twist.angular.y = 0;
        twist.angular.z = ros_vel[0]; // direction (positive = left, negative = right)

		pub.publish(twist);
		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;
}
