//
// Created by cubot on 18-9-28.
//
#include <ros/ros.h>
#include <std_msgs/Empty.h>
#include <sstream>
#include <Communication_Func.h>
using namespace std;
READ_WRITE::READ_WRITE() {
    ROS_INFO_STREAM("Initilizing serial port...");
    ser.setPort("/dev/ttyUSB0");//串口号
    ser.setBaudrate(115200);//波特率
    serial::Timeout to = serial::Timeout::simpleTimeout(1000);//字节间读写超时设为1s
    ser.setTimeout(to);//设置串口超时
    ser.open();//打开串口
    ROS_INFO_STREAM("Initilization complete");
    Read_Odom_Raw();
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
//                cout<<"odom.vx="<<odom.vx<<"\n"<<"odom.vy="<<odom.vy<<"\n"<<"odom.w="<<odom.w<<"\n"<<"odom.x="<<odom.x<<"\n"<<endl;
                cout<<"odom.x= "<<odom.x<<endl;//57.2957
                odom_raw.clear();
                }

        }

    }

}

void READ_WRITE::Send_Cmdvel() {

}



