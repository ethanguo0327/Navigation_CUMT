//
// Created by cubot on 18-9-28.
//
#include <ros/ros.h>
#include <Communication_Func.h>

int main(int argc,char** argv)
{
    ros::init(argc,argv,"communication");
    ros::NodeHandle n;
    ros::Publisher chatter_pub = n.advertise<std_msgs::String>("message",1000);
    ros::Rate loop_rate(10);
    READ_WRITE read_write;
    return 0;

}
