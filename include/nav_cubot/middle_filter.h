//
// Created by cubot on 18-10-29.
//

#ifndef NAV_CUBOT_MIDDLE_FILTER_H
#define NAV_CUBOT_MIDDLE_FILTER_H

#include "Communication_Func.h"
/*!biref:存储三组里程计信息,进行中值滤波,防止有跳变
* \param ODOM odom_ ：最新的里程计消息
* \return ODOM :返回滤波结果
*/
ODOM middle_filter(ODOM odom_);

#endif //NAV_CUBOT_MIDDLE_FILTER_H
