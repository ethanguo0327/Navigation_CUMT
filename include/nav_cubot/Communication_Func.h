//
// Created by cubot on 18-9-28.
//

#ifndef PROJECT_NAV_COMMUNICATION_H
#define PROJECT_NAV_COMMUNICATION_H

#endif //PROJECT_NAV_COMMUNICATION_H
#include <serial/serial.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include <tf/transform_broadcaster.h>
/*! 里程计结构体
*/
typedef struct Odometry{
    float x;
    float y;
    float angle;
    float vx;
    float vy;
    float w;
}ODOM;


class READ_WRITE
{
public:
    /*! 构造函数
  * \param null
  * \return null
  */
    READ_WRITE();
    /*!biref:读取串口数据，并将四个字节解析为float型里程计
   * \param null
   * \return null
   */
    void Read_Odom_Raw();
    /*!biref:将里程计信息打包成nav_msgs::Odometry格式,发布odom topic
    * \param odom1 ：里程计信息
    * \return null
    */
    void Pub_Odom(ODOM* odom1,ros::Time current_time);
    /*!biref:将里程计信息打包成nav_msgs::Odometry格式,发布odom topic
    * \param odom1 ：里程计信息打包成geometry_msgs::TransformStamped格式,发布/odom tf
    * \param current_time :发送odom topic和odom tf的时间
    * \return null
    */
    void Pub_tf(ODOM* odom2,ros::Time current_time);
    /*!biref:通过串口发送控制底盘指令.将读到的geometry_msgs消息打包到string里，并设置帧头为“cu”,帧尾为校验和
    * \param cmd_vel ：订阅到的控制指令
    * \param current_time :发送odom topic和odom tf的时间
    * \return null
    */
    void Send_Cmdvel(const geometry_msgs::Twist cmd_vel);
private:
    ODOM odom={0.0,0.0,0.0,//声明里程计结构体
               0.0,0.0,0.0};
    serial::Serial ser; //声明串口对象
    ros::NodeHandle n;//ros节点句柄
    ros::Publisher pub;//odom topic 发布器
    ros::Subscriber sub;//cmd_vel订阅器
    tf::TransformBroadcaster odom_broadcaster_;//tf发布器
};

