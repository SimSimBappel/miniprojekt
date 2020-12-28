#include <iostream>
#include "ros/ros.h"
#include <string>
#include <fstream>

#include <miniprojekt/newString.h>
// #include <miniprojekt/newStringRequest.h>
// #include <miniprojekt/newStringResponse.h>
using namespace std;

void terminalWrite();
void sortering();
void readText();
void writeText();
string getFilename();


struct sengeliste{
    vector <string> text;
}liste;

bool callBack(miniprojekt::newStringRequest &req, miniprojekt::newStringResponse &res){
    
    string input;
    string space;
    
    input = req.head;

    if (input == "oprette"){
        liste.text.push_back(req.str);
        cout << req.str << " have being commited" << endl;
        res.str = "Have being commited";
        
        return true;
    }
    else if(input == "tjek"){
        cout << "du havde sendt tjek" << endl;
        res.str  = "du havde sendt noget under tjek" ;
        return true;

    }
    // else if(input == "opdater"){
    //     cout << "du havde sendt opdater" << endl;
    //     res.str = "duhavde sendt noget med under opdater";
    //     return true;
    // }
    else if(input == "push"){
        cout << "Pushing" << endl;
        
        readText();
        terminalWrite();
        sortering();
        terminalWrite();
        writeText();
        liste.text.clear();
        cout << "pushed" << endl;
        res.str = "pushed";
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
    cout << "The buffer node for updates for sengeplads have been started :)" << endl;
    ros::NodeHandle nh;
    
    ros::ServiceServer service = nh.advertiseService("service_talker",callBack);
    ros::spin();
    return 0;

}

void terminalWrite(){
    for(int i = 0; i < liste.text.size();i++){
        cout << liste.text[i] << endl;
    }

}

void sortering(){
    cout << "sorting" << endl;
int good = 1;
while(good == 1){
    good = 0;

    for(int i = 0; i < liste.text.size(); i++){
      if(i+1 < liste.text.size()) { 
        
        string holder[4];
        
        stringstream s;
        stringstream ss;
        ss << liste.text[i+1];
        
        s << liste.text[i];
        
        getline(s, holder[0], ' ');
        
        getline(s, holder[1], ',');
        
        getline(ss, holder[2], ' ');
        
        getline(ss, holder[3], ',');
        
        int check_1 = stoi(holder[1]);
        
        int check_2 = stoi(holder[3]);
        
        if(check_1 > check_2 && i+1 < liste.text.size() ){
            
            swap(liste.text[i],liste.text[i+1]);
            
            good = 1;
        }
        
      }

    }
    
    }

}

void readText(){
    fstream file;
    file.open(getFilename(), ios::in);
    string temper;
    if(file.is_open()){
        while(getline(file,temper)){
            liste.text.push_back(temper);
        }
    }
    else{
        cout << "Could not open the file :(" << endl;
    }
    file.close();
}

void writeText(){
    fstream file;
    file.open(getFilename(), ios::trunc |ios::out);
    if(file.is_open()){
        for (int i = 0; i < liste.text.size(); i++)
        {
            file << liste.text[i];
        }
        
    }
    else{
        cout << "Could not open til file :(" << endl;
    }
    file.close();
}


string getFilename(){
  string file_name;
  char username[L_cuserid];
  if(cuserid(username) == NULL)
  {
    cout << "could not resolve username\n";
    exit(1);
  }
  file_name = username;
      
  file_name = "/home/" + file_name + "/catkin_ws/src/miniprojekt/sengeplads.txt";
  return file_name;
 
}

