//
// Created by cubot on 18-9-28.
//
#include <ros/ros.h>
#include <sstream>
#include <std_msgs/Empty.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_broadcaster.h>
#include <geometry_msgs/Twist.h>
#include <Communication_Func.h>
#include "middle_filter.h"
using namespace std;
READ_WRITE::READ_WRITE() {
    ROS_INFO_STREAM("Initilizing serial port...");
    ser.setPort("/dev/usart_nav");//串口号(将电脑上的某个固定的usb口重命名为usart,放弃使用易变的/dev/ttyUSB*)
    ser.setBaudrate(115200);//波特率
    serial::Timeout to = serial::Timeout::simpleTimeout(1000);//字节间读写超时设为1s
    ser.setTimeout(to);//设置串口超时
    ser.open();//打开串口
    ROS_INFO_STREAM("Initilization complete");
    pub = n.advertise<nav_msgs::Odometry>("odom",10);//发布odom消息,odom前面不需加/
    /*!@TODO：
      * @this 什么意思？
      */
    sub=n.subscribe("/cmd_vel_mux/input/teleop",10,&READ_WRITE::Send_Cmdvel,this);//10：不想等待太多，只想要最新的消息，所以取较小的数
}

void READ_WRITE::Read_Odom_Raw() {
    vector<uint8_t > odom_raw;
    union Char2Float
    {/*!@TODO：
      * @无符号和有符号偶的区别
      */
        uint8_t hex[32];//32个uint8_t
        float odom_serial[8];//长度等于8个float
    }c2f;

    while(ros::ok())
    {
        //读串口，相当于不停地push_back
        ser.read(odom_raw);
        //判断帧头
        if(odom_raw.size()==4&&(odom_raw[0]!=0xaa||odom_raw[1]!=0xaa||odom_raw[2]!=0xf1))
        {
            odom_raw.clear();
        }
        if(odom_raw.size()==29)
        {
            //校验和
            uint8_t sum=0;
            for(int8_t j=0;j<28;j++)
                sum+=odom_raw[j];
            if (odom_raw[28]!=sum)
               {odom_raw.clear();
               continue;}
            else{
                //存储为uint8_t型
                for (int8_t i = 0; i < 29; i++)
                    c2f.hex[i] = odom_raw[i];
                //还原为float
                odom.vx = c2f.odom_serial[1];
                odom.vy = c2f.odom_serial[2];
                odom.w = c2f.odom_serial[3];
                odom.x = c2f.odom_serial[4];
                odom.y = c2f.odom_serial[5];
                odom.angle = c2f.odom_serial[6];
//                cout<<"odom.y= "<<odom.y<<endl;//57.2957
                odom_raw.clear();
                //中值滤波过滤跳变
                odom_out=middle_filter(odom);
                cout<<odom_out.x<<endl;
                ros::Time current_time=ros::Time::now();
                //发布odom topic
                Pub_Odom(&odom_out,current_time);
                //发布odom tf
                Pub_tf(&odom_out,current_time);
                }

        }
        //每一次读串口循环结束后，进入回调函数下发控制指令
        ros::spinOnce();
    }

}

void READ_WRITE::Pub_Odom(ODOM* odom1,ros::Time current_time) {
    nav_msgs::Odometry odom;
    odom.header.stamp=current_time;//本消息的时间戳
    odom.header.frame_id="odom";// 消息里的frame_id表示这些消息的参考原点
    /*!@TODO：
     * @child_frame 在消息里做什么用？
     */
    odom.child_frame_id="base_footprint";
    odom.pose.pose.position.x = odom1->x;
    odom.pose.pose.position.y = odom1->y;
    odom.pose.pose.position.z = 0.0;
    geometry_msgs::Quaternion odom_quaternion = tf::createQuaternionMsgFromYaw(odom1->angle);//将角度（弧度制）转换为四元数
    odom.pose.pose.orientation = odom_quaternion;
    pub.publish(odom);
}

void READ_WRITE::Pub_tf(ODOM* odom2,ros::Time current_time){
    geometry_msgs::TransformStamped odom_trans;
    odom_trans.header.stamp = current_time;
    odom_trans.header.frame_id = "odom";
    odom_trans.child_frame_id = "base_footprint";
    odom_trans.transform.translation.x = odom2->x;
    odom_trans.transform.translation.y = odom2->y;
    odom_trans.transform.translation.z = 0.0;
    geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(odom2->angle);
    odom_trans.transform.rotation = odom_quat;
    odom_broadcaster_.sendTransform(odom_trans);
}

void READ_WRITE::Send_Cmdvel(const geometry_msgs::Twist cmd_vel) {
    cout<<"got the cmd_vel,sending it.."<<endl;
    //帧头
    string s1(1,0xaa);
    string s2(1,0xf1);
    string cmd2string=s1+s2;//将char型转换为字符串型
    geometry_msgs::Twist cmd=cmd_vel;
    //将控制指令打包到string里
    //vx
    static uint8_t i=0;
    i=0;
    while( i<8)
    {cmd2string+=(*((char *)&cmd.linear.x+(i++)));}
    //vy
    i=0;
    while( i<8)
    {cmd2string+=(*((char *)&cmd.linear.y+(i++)));}
    //w
    i=0;
    while( i<8)
    {cmd2string+=(*((char *)&cmd.angular.z+(i++)));}
    //校验和
    char sum=0;
    for(uint8_t i=0;i<cmd2string.size();i++)
         sum+=char (cmd2string[i]);
    cmd2string+=sum;
    //往串口写
    static size_t number=0;
    number=ser.write(cmd2string);
    cout<<number<<endl;
}



