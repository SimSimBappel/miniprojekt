//inkluderer bibliotek(er)
#include <ros/ros.h>
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <miniprojekt/newString.h>

//declare namespaces
using namespace std;
using namespace ros;

//declare funcions (name, parameters, returntype)
int checkLineNr();
string getFilename();



class Cla{
public: 
 //declare
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
    // init variables
    info.x = x;
    info.y = y;
    info.z = z;
    info.w = w;
}

int main(int argc, char **argv)
{
    init(argc, argv, "opret_opdater_seng");// nu ved ros denne node kører, den skal initalizes før den kan kommunikere med ros
    int job;
    
    bool done = false;
    string input;
    string opgave;
    //så længe ros er okay så vil der blive kørt dette loop 
    while(ok)
    {
        
        if(done == true){
        cout << "Vil du 1: tjekke, 2: oprette, 3: opdatere en plads eller 4: push til fil? \n";
        }
        else{
            cout << "Vil du 1: tjekke, 2: oprette eller 3: opdatere en plads? ";
        }

        cin >> opgave;
        
        //chaining
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

        //declarations
        string line;
        fstream file; 

        NodeHandle nh;
        ServiceClient client = nh.serviceClient<miniprojekt::newString>("service_talker" );
        miniprojekt::newString srv;

        //
        switch (job){

            case(1):
            {  //klammerne gør at variablerne er lokalt declreret 

                //tjekke
                //vi fortæller "file" at den nu er 
                file.open(getFilename());
                
                cout << "Vælg en plads:";
                cin >> input;
                
                int line_no = stoi(input);// input is converted from string to int
                int lines_read = 0;

                if (file.is_open()){

                    while(file){
                
                        getline(file,line);
                        lines_read++;
                
                        if(lines_read==line_no){
                            line.resize(16); //ikke god hvis plads- og/eller sengetallet er større end 1 char
                            cout << line << endl;
                        }   
                    }

                    file.close();
                    if (lines_read<line_no){
                        cout << "Kunne ikke finde plads" << line_no << '\n';
                    }
                    
                }
                break;
            }



            case(2):
            {
                //oprette
                
                int plads;
                int seng;


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

                
                Subscriber sub = nh.subscribe("amcl_pose", 1000, counterCallback);//declare and init 'sub'.. bruger subcribe funktionen med paramtre navn, quesize, 
                
                // Sætter hvor mange gange den skal lave callbacket hvert sekund
                Rate loop_rate(10);
                
                //for lykker har 3 statements, nr1 kører én gang først nr2 definerer en condition, nr3 kører hver gang blokken er execveret til sidst
                for(int i = 0; i < 10; i++){
                    spinOnce();
                    loop_rate.sleep();
                }
                
                stringstream ff;//dec.. 

                ff << "plads: " << plads << ",seng: " << seng << "," << bed << "," <<info.x << ","<< info.y << "," << info.z << "," << info.w;
                string ss = ff.str();

                
                //strcutet som indeholder head og str bliver initialiseret
                srv.request.head = "oprette";
                srv.request.str = ss;
                
                if(client.call(srv)){  //service clienten får .head og .str
                    cout << "calling :)" << endl; 
                    cout << "anwser: " << srv.response << endl;
                    done = true;
                }
                else{
                    cout << "does not work" << endl;
                }


                break;
            }

            case(3):
            {
                //opdatere
                file.open(getFilename());
                cout << "Vælg en plads du vil opdatere: ";
                cin >> input;
                int line_to_change = stoi(input);
                
                string seng;
                cout << "angiv ny sengenummer: ";
                cin >> seng;
                int bed_nr = stoi(seng);
                if(bed_nr > 999 || bed_nr == 0)
                {
                    cout << "not allowed nr. Exitting" << endl;
                    exit(1);
                }

                string tokens[checkLineNr()+1][7];
                unsigned int line = 1; 
                unsigned int file_pos = 0;
                if(file.is_open()){
                    
                    while(getline(file, tokens[line][0], ',')){

                        for (int i = 1; i < 6; i++)
                        {
                            getline(file, tokens[line][i], ',');
                        }
                        getline(file, tokens[line][6]);
                        
                        line ++;

                        if(line == line_to_change){ 
                            file_pos = file.tellg();

                            cout << "file_pos: " <<file_pos << endl;
                        }
                    }
                }
                file.close();

                file.open(getFilename());

                if(file.is_open())
                {
                    file.seekp(file_pos);
                    string jn = "";
                    if(bed_nr == 0)
                    {
                        jn= "nej";
                    }
                    else{
                        jn = "ja";
                    }

                    file << tokens[line_to_change][0] << ",seng: " << bed_nr << "," << jn;

                    for(int i = 3; i < 6; i++)
                    {
                        file << "," << tokens[line_to_change][i];
                    }
                    file << ",0.0  ";

                    cout << tokens[line_to_change][0] << " has beed changed from: " 
                    << tokens[line_to_change][1] << " to: seng: " << bed_nr << endl;
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
            //hvis inputtet ikke passer med nogle af casesne så vil defaulten blive brugt
            default:
            {
                cout << "Something went wrong" << endl;
            }

        }

    }
}

int checkLineNr(){
  fstream newfile;
  int doc_line_nr = 0;
  string temp ="";
  newfile.open(getFilename(),ios::in);
  if (newfile.is_open())
  {
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
      
  file_name = "/home/" + file_name + "/catkin_ws/src/miniprojekt/sengeplads.txt";
  return file_name;
 
}
