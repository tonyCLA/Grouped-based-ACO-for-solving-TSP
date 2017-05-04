/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: tonyc
 *
 * Created on 27 februarie 2017, 14:25
 */
#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>
#include <string>
#include <time.h> 
#include "node.h"
#include "cluster.h"
#include "ant.h"
#include "run_aco.h"

using namespace std;

int main(int argc, char** argv) 
{
    int nr_ants=1, iterations=2, mode=1, pher_influence=2, heuristics_influence=2;
    
    float threshold=2.5, evaporation=0.1, initial_pher=0.5;
    string dataset="small_dataset1.txt";
    string logfile="log.txt";
    string change_param="no";
    
    cout<<"This program can run in two modes:\n";
    cout<<"1. This mode will run the program as a typical Ant System algorithm\n";
    cout<<"2. This mode will group the nodes that are close to each other and run Ant System algorithm on groups of nodes\n";
    
    cout<<"Please enter mode(1 or 2): ";
    cin>>mode;
    if(mode!=1 && mode!=2)return 0;
    
    cout<<"Please enter the dataset location and filename (.txt file): ";
    cin.ignore();
    getline (cin,dataset); 
    
    cout<<"\nDefault parameters are as follows:\n";
    cout<<"> Number of ants - 10\n> Number of iterations - 50\n";
    cout<<"> Initial pheromone level - 0.5\n> Pheromone evaporation - 0.1\n";
    cout<<"> Pheromone influence - 2\n> Heuristics influence - 2\n";
    
    cout<<" Do you want to change these parameters (yes/no)?";
    cin.ignore();
    getline (cin,change_param);
    
    if(change_param.compare("yes") == 0)
    {
        cin.ignore();
        cout<<"\nEnter the number of ants: ";
        cin>>nr_ants;
        cout<<"\nEnter number of iterations: ";
        cin>>iterations;
        cout<<"\nEnter the initial pheromone level value: ";
        cin>>initial_pher;
        cout<<"\nEnter the pheromone evaporation: ";
        cin>>evaporation;
        cout<<"\nEnter pheromone influence:";
        cin>>pher_influence;
        cout<<"\nEnter heuristics influence:";
        cin>>heuristics_influence;
       
        
        cout<<"\nACO will run with the following parameters: \n";
        cout<<"> Iterations: "<< iterations<<endl;
        cout<<"> Nr. ants: "<< nr_ants<<endl;
        cout<<"> Dataset file: "<< dataset<<endl;
    }
    
    run_aco session1;
    session1.set_ants(nr_ants);
    session1.set_iterations(iterations);
    session1.generate_final_solution(mode, threshold, evaporation, initial_pher, dataset, logfile);
  
    cout<<"\nProgram Terminated!\n";
    return 0;
}
