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

void terminalWrite();
void sort();
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
    string space;
    // stringstream ss;
    // ss << req.str;
    input = req.head;

    //getline(ss, input , ',');

    if (input == "oprette"){
        cout << "du havde sendt oprettet" << endl;
        res.str = "Den er blevet oprettet";
        liste.he.push_back(req.str);
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
        sort();
        terminalWrite();
        liste.he.clear();
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

void terminalWrite(){
    for(int i = 0; i < liste.he.size();i++){
        cout << liste.he[i] << endl;
    }

}

void sort(){


    for(int i = 0; i < liste.he.size(); i++){
        string holder[4];
        stringstream s;
        stringstream ss;
        ss << liste.he[i+1];
        s << liste.he[i];
        getline(s, holder[0], ' ');
        getline(s, holder[1], ',');

        getline(ss, holder[2], ' ');
        getline(ss, holder[3], ',');
        int check_1 = stoi(holder[1]);
        int check_2 = stoi(holder[3]);

        if(check_1 < check_2 && i+1 > liste.he.size() ){
            cout << "swap" << endl;
            swap(liste.he[i],liste.he[i+1]);
            
        }


        // getline(, holder[1], ',');
        
        

    }
    
 

}