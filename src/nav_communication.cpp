//
// Created by cubot on 18-9-28.
//
#include <ros/ros.h>
#include <Communication_Func.h>

int main(int argc,char** argv)
{
    //节点初始化
    ros::init(argc,argv,"communication");
    //实例化类，开启订阅cmd_vel线程
    READ_WRITE read_write;
    //读里程计，在while中执行发布odom topic、tf、回调函数中的串口下发速度指令
    read_write.Read_Odom_Raw();

    return 0;

}
