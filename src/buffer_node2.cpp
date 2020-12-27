#include <iostream>
#include "ros/ros.h"
// #include <my_code_msgs/myString.h>
// #include <my_code_msgs/myStringRequest.h>
// #include <my_code_msgs/myStringResponse.h>

#include <miniprojekt/newString.h>
#include <miniprojekt/newStringRequest.h>
#include <miniprojekt/newStringResponse.h>
using namespace std;

// bool callBack(my_code_msgs::myStringRequest &req, my_code_msgs::myStringResponse &res){
//     cout << req.str << endl;
//     string response = "hej";
//     res.str = response;
//     return true;
// }

bool callBack(miniprojekt::newStringRequest &req, miniprojekt::newStringResponse &res){
    cout << req.str << endl;
    string response = "hej med dig";
    res.str = response;
    return true;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "buffer_node2");
    ros::NodeHandle nh;
    //ros::ServiceServer service = nh.advertiseService("service_talker",callBack);
    ros::ServiceServer service = nh.advertiseService("service_talker",callBack);
    ros::spin();
    return 0;

}