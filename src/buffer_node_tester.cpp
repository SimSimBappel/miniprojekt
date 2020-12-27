#include <iostream>
#include "ros/ros.h"
#include <cstdlib>
#include <string>

// #include <my_code_msgs/myString.h>
// #include <my_code_msgs/myStringRequest.h>
// #include <my_code_msgs/myStringResponse.h>
#include <miniprojekt/newString.h>
// #include <miniprojekt/newStringRequest.h>
#include <miniprojekt/newStringRequest.h>
#include <miniprojekt/newStringResponse.h>
using namespace std;


int main(int argc, char **argv)
{
    ros::init(argc, argv, "buffer_node_tester");
    ros::NodeHandle nh;
    //ros::ServiceClient client = nh.serviceClient<my_code_msgs::myString>("service_talker");
    ros::ServiceClient client = nh.serviceClient<miniprojekt::newString>("service_talker");
    //my_code_msgs::myString srv;
    miniprojekt::newString srv;
    while(ros::ok){
    string input;
    cin >> input;
    string test = "plads: 11,seng: 6,ja,-1.03911781216,11.1295263925,0.753757344949,0.0";
    string send = input + "," + test;
    srv.request.str = test ;
    srv.request.head = input;
    
    //cout << srv << endl;
    if(client.call(srv)){
        cout << "calling :)" << endl;
        cout << "anwser: " << srv.response << endl;
    }
    else{
        cout << "do not work" << endl;
    }
    }
    return 0;
}