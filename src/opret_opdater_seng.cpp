#include <ros/ros.h>
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

#include <miniprojekt/newString.h>

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

int main(int argc, char **argv)
{
    init(argc, argv, "opret_opdater_seng");
    int job;
    bool done = false;
    string input;
    string opgave;
    cout << "hej"<< endl;
    
    //while(ok){


    if(done == true){
       cout << "Vil du 1: tjekke, 2: oprette, 3: opdatere en plads eller 4: push til fil? \n";
   }
   else{
        cout << "Vil du 1: tjekke, 2: oprette eller 3: opdatere en plads? \n";
   
   }
    cin >> opgave;
    
    if (opgave == "tjekke" || opgave == "1"){
        job = 1;
    } 
    else if (opgave == "oprette" || opgave == "2"){
        job = 2;
    }
    else if (opgave == "opdatere" || opgave == "3"){
        job = 3;
    }
    else if (opgave == "push" || opgave == "4"){
        job = 4;
    }

    string line;
    fstream file;
    NodeHandle nh;
    ServiceClient client = nh.serviceClient<miniprojekt::newString>("service_talker" );
    miniprojekt::newString srv;

    switch (job){

        case(1):
           {    
            //tjekke
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
            //oprette
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

            

            Subscriber sub = nh.subscribe("amcl_pose", 1000, counterCallback);
            
            // Sætter hvor mange gange den skal lave callbacket hvert sekund
            Rate loop_rate(10);

            for(int i = 0; i<10; i++){
                spinOnce();
                loop_rate.sleep();
            }
            
            stringstream ff;

                ff << "plads: " << plads << ",seng: " << seng << "," << bed << "," <<info.x << ","<< info.y << "," << info.z << "," << info.w;
                string ss = ff.str();

                
                
                srv.request.head = "oprette";
                srv.request.str = ss;
                
                if(client.call(srv)){
                        cout << "calling :)" << endl;
                        cout << "anwser: " << srv.response << endl;
                        done = true;
                    }
                    else{
                            cout << "do not work" << endl;
                        }
//den er streget ud da vi nok nu kan køre det gennem buffer_node2
            // if (file.is_open()){
            //     file << "plads: " << plads << ",seng: " << seng << "," << bed << "," <<info.x << ","<< info.y << "," << info.z << "," << info.w;
                
                
            // }
        

            file.close();

            break;
        }



        case(3):
        {
            //opdatere
            file.open(getFilename(), ios::in | ios::out);
            cout << "Vælg en plads du vil opdatere: ";
            cin >> input;
            int line_to_change = stoi(input);
            
            string seng;
            cout << "angiv ny sengenummer" << '\n';
            cin >> seng;
            int bed_nr = stoi(seng);
            cout << bed_nr << endl;

            string tokens[checkLineNr()+1][7];
            int line = 1; 
            unsigned int file_pos = 0;
            if(file.is_open()){
                string line_elements[7] = "";
                
                while(getline(file, line_elements[0], ',')){
                    tokens[line][0] = line_elements[0];
                    //cout << "line: " << line <<'\n';//delete
                    cout << tokens[line][0] << '\n';//delete
                    //file.seekg(0,ios::beg);
                    for (int i = 1; i < 6; i++)
                    {
                        getline(file, line_elements[i], ',');
                        tokens[line][i] = line_elements[i];
                        cout << tokens[line][i] << '\n'; // delete
                    }
                    getline(file, line_elements[6]);
                    tokens[line][6] = line_elements[6];
                    cout << tokens[line][6] << '\n' << '\n';// delete

                    line ++;
                    if(line == line_to_change){ // find den rigtige plads til denne for at den kan tage fra linje 1 og til slut
                        file_pos = file.tellg();
                        cout << "file.tellg: " << file.tellg() << endl;

                        cout << "file_pos: " <<file_pos << endl;
                    }
                    
                }
                

                //succing edit time

            }
            file.close();

            file.open(getFilename(), ios:: out | ios::in);

            if(file.is_open()){
                
                //cout << file_pos << endl;
                file.seekp(file_pos);

                file << tokens[line_to_change][0] << ",seng: " << bed_nr <<",";
                //file << tokens[line_to_change][0];
                cout << "token line_to_change: "<< tokens[line_to_change][0] << endl;
                string temper ="";
                file.seekg(file_pos);
                getline(file, temper);
                cout << "temper: " << temper << endl;


            }
            break;
        }
        case(4):
        {
            srv.request.head = "push";
            srv.request.str = "";
            if(client.call(srv)){
                cout << "calling :)" << endl;
                cout << "anwser: " << srv.response << endl;
            }
            else{
                cout << "do not work" << endl;
            }
            break;
        }
        default:
        {
            cout << "Wrong" << endl;
        }

    }

   // }
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
      //cout << temp << '\n'; //delete this
    }
  }
  newfile.close();
  //cout << doc_line_nr << '\n';
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
      
  file_name = "/home/" + file_name + "/catkin_ws/src/miniprojekt/sengeplads.txt";
  return file_name;
 
}
