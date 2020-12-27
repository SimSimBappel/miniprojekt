#include <iostream>
#include "ros/ros.h"
#include <string>
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
struct sengeliste{
    vector <string> he;
}liste;

bool callBack(miniprojekt::newStringRequest &req, miniprojekt::newStringResponse &res){
    cout << req.str << endl;
    string response = "hej med dig";
    string input;
    // stringstream ss;
    // ss << req.str;
    input = req.head;

    //getline(ss, input , ',');

    if (input == "oprette"){
        cout << "du havde sendt oprettet" << endl;
        res.str = "du havde sendt noget under oprettet";
        return true;
    }
    else if(input == "tjek"){
        cout << "du havde sendt tjek" << endl;
        res.str  = "du havde sendt noget under tjek" ;
        return true;

    }
    else if(input == "opdater"){
        cout << "du havde sendt opdater" << endl;
        res.str = "duhavde sendt noget med under opdater";
        return true;
    }
    else if(input == "done"){
        cout << "skal nok skrive det ind :)" << endl;
        res.str = "Det skrives ind";
        
        return true;
    }
    else{
        cout << "det du sendte gav ikke mening" << endl;
        res.str = "det gav ingen mening det du sendte";
        return false;
    }



    
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