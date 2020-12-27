#include <ros/ros.h>
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <stdio.h>

//#include <filesystem> //remove if not used
//#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

using namespace std;
using namespace ros;


int checkLineNr();
string getFilename();

class Cla{
public:
    double x;
    double y;
    double z;
    double w;

    
}info;

void counterCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr &msg){

    double x = static_cast<double>(msg->pose.pose.position.x);
    double y = static_cast<double>(msg->pose.pose.position.y);
    double z = static_cast<double>(msg->pose.pose.orientation.z);
    double w = static_cast<double>(msg->pose.pose.orientation.w);
    
    info.x = x;
    info.y = y;
    info.z = z;
    info.w = w;
}

int main(int argc, char **argv){

    int job;

    string input;
    string opgave;


    cout << "Vil du tjekke, oprette eller opdatere en plads? \n";
    cin >> opgave;
    
    if (opgave == "tjekke"){
        job = 1;
    } 
    if (opgave == "oprette"){
        job = 2;
    }
    if (opgave == "opdatere"){
        job = 3;
    }

    string line;
    fstream file;

    switch (job){

        case(1):
           {    
            
            file.open(getFilename());
            
            cout << "Vælg en plads:";
            cin >> input;
            
            int lineno = stoi(input);
            int lines_read = 0;

            if (file.is_open()){

                while(file){
            
                    getline(file,line);
                    lines_read++;
            

            if(lines_read==lineno){
                line.resize(16);
                cout << line << endl;
                break;
                    }   
                }
        file.close();
        if (lines_read<lineno)
            cout << "Kunne ikke finde plads" << lineno << '\n';
            return 0;
            
            }
            break;
        }

        case(2):
        {
            init(argc, argv, "class_forsog_node");
            
            int plads;
            int seng;

            
            file.open(getFilename(),ios::app);

            cout << "Venligst skriv plads nummer \n" ;
            cin >> plads;

            cout << "Skriv senge nummer eller 0 for ingen seng \n";
            cin >> seng;
            string bed;
            if(seng== 0){
                bed = "nej";
            }
            else{
                bed = "ja";
            }

            NodeHandle nh;

            Subscriber sub = nh.subscribe("amcl_pose", 1000, counterCallback);
            
            // Sætter hvor mange gange den skal lave callbacket hvert sekund
            Rate loop_rate(10);

            for(int i = 0; i<10; i++){
                spinOnce();
                loop_rate.sleep();
            }
            
            

            if (file.is_open()){
                file << "plads: " << plads << ",seng: " << seng << "," << bed << "," <<info.x << ","<< info.y << "," << info.z << "," << info.w;
            }
        

            file.close();

            break;
        }

        case(3):
        {
            file.open(getFilename());
            cout << "Vælg en plads du vil opdatere:";
            cin >> input;
            string seng;
            cout << "angiv ny sengenummer" << '\n';
            cin >> seng;
            int lineno = stoi(input);
            string line_list[6];

            if(file.is_open()){
                
                file.seekg (lineno);
                getline(file,line);
                cout << line;
                stringstream line;
                getline(line,line_list[0], ',');
                getline(line,line_list[1], ',');
                getline(line,line_list[2], ',');
                getline(line,line_list[3], ',');
                getline(line,line_list[4], ',');
                getline(line,line_list[5]);
                line_list[1] = "seng: " + seng;


            }
            string new_line = line_list[0] + line_list[1] + line_list[2] + line_list[3] + line_list[4] + line_list[5];
            string strReplace = line;
            cout << new_line << '\n';    
        
            

          }

        }
    }

int checkLineNr(){
  fstream newfile;
  int doc_line_nr = 0;
  
  newfile.open(getFilename(),ios::in);
  if (newfile.is_open())
  {
    string temp;
    while(getline(newfile, temp)){
      doc_line_nr++;
    }
  }
  newfile.close();
  return  doc_line_nr;
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
      
  file_name = "/home/" + file_name + "/catkin_ws/src/p1_ws/sengeplads.txt";
  return file_name;
 
}
