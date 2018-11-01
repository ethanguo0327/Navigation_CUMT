//
// Created by cubot on 18-10-29.
//

#include "middle_filter.h"

using namespace std;
ODOM middle_filter(ODOM odom_)
{
    static int num=0;
    static vector<vector<float> > container;
    ODOM odom_out={0.0,0.0,0.0,0.0,0.0,0.0};
    num=(num+1)%3;
    container.resize(6);
    if (container[0].size() < 3)
    {
        for(int i=0;i<6;i++)
           container[i].push_back(*((float *)&odom_ + i));
        return odom_;
    }
    else
    {
        for(int i=0;i<6;i++)
        {container[i][num]=*((float *)&odom_+i);
         sort(container[i].begin(),container[i].end());
         *((float *)&odom_out+i)=container[i][1];}
         return odom_out;
    }

}