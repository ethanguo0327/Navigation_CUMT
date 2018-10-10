//
// Created by cubot on 18-9-28.
//

#ifndef PROJECT_NAV_COMMUNICATION_H
#define PROJECT_NAV_COMMUNICATION_H

#endif //PROJECT_NAV_COMMUNICATION_H
#include <serial/serial.h>
#include <std_msgs/String.h>

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
    /*!biref:通过串口发送控制底盘指令
    * \param null
    * \return null
    */
    void Send_Cmdvel();

private:
    ODOM odom={0,0,0,
               0,0,0};
    serial::Serial ser; //声明串口对象
};