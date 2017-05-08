/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: tonyc
 *
 * Created on 27 feb 2017, 14:25
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
#include "output.h"

using namespace std;

int main(int argc, char** argv) 
{
    int nr_ants=3, 
            iterations=5, 
            mode=1, 
            pher_influence=2, 
            heuristics_influence=2, 
            verbosity=1,
            clustering_type=0,
            aco_type=0;
    
    float threshold=5, evaporation=0.1, initial_pher=0.5;
    string dataset="small_dataset1.txt",
            logfile="output.log", 
            change_param="no",
            loop="yes";
    
    while(loop.compare("yes") == 0)
    {
        cout<<"This program can run in two modes:\n";
        cout<<"1. This mode will run the program as a typical Ant System algorithm\n";
        cout<<"2. This mode will group the nodes that are close to each other and run Ant System algorithm on groups of nodes\n";

        cout<<"Enter mode(1 or 2): ";
        cin>>mode;
        
        cout<<"\nThere are also 3 variations of Ant System algorithm:\n";
        cout<<"1. Ant System\n";
        cout<<"2. Min-Max Ant System\n";
        cout<<"     When using this approach the initial pheromone value should be higher\n";
        cout<<"as Min-Max AS will allow pheromone levels between an interval (p_min, p_max)";
        cout<<"due to current limitations for this type of algorithm initial pheromone value represent p_max\n";
        cout<<"and p_min is a few units smaller( number randomly chosen)\n";
        cout<<"3. Rank-based Ant System\n";
        cout<<"Select the type of Ant System:";
        cin>>aco_type;
        
        
        if(mode == 2)
        {
            cout<<"There are 2 types of clustering available\n";
            cout<<"1.A hierarchical implementation (distance threshold needed)\n";
            cout<<"2.A k-means implementation (number of clusters needed)\n";
            cout<<"Select one option:";
            cin>>clustering_type;
        }
        
        cout<<"Enter the dataset location and filename (text file): ";
        cin.ignore();
        getline (cin,dataset); 

        cout<<"If the dataset is large and the verbosity is high the output is too big to fit in console display!\n";
        cout<<"Enter the name of the logfile (.log file): ";
        getline (cin,logfile);

        cout<<"\nThe output can have different levels of verbosity (1-3)";
        cout<<"\nLevel 1 will only display main processes(Default)";
        cout<<"\nLevel 2 will additionally display solutions of each ant at the end of an iteration";
        cout<<"\nLevel 3 will additionally display each step of constructing ant solution";
        cout<<"\nEnter the level of verbosity(1,2,3): ";
        cin>>verbosity;
        cin.ignore();

        //creating the logfile
        std::ofstream output(logfile.c_str(), std::ios_base::out);
        output.close();
        FileOutput log(logfile.c_str());  

        cout<<"\nDefault parameters are as follows:\n";
        cout<<"> Number of ants - 10\n> Number of iterations - 50\n";
        cout<<"> Initial pheromone level - 0.5\n> Pheromone evaporation - 0.1\n";
        cout<<"> Pheromone influence - 2\n> Heuristics influence - 2\n";

        cout<<" Do you want to change these parameters (yes/no)?";
        getline (cin,change_param);

        if(change_param.compare("yes") == 0)
        {
            cout<<"Enter the number of ants: ";
            cin>>nr_ants;
            cout<<"Enter number of iterations: ";
            cin>>iterations;
            cout<<"Enter the initial pheromone level value: ";
            cin>>initial_pher;
            cout<<"Enter the pheromone evaporation: ";
            cin>>evaporation;
            cout<<"Enter pheromone influence: ";
            cin>>pher_influence;
            cout<<"Enter heuristics influence: ";
            cin>>heuristics_influence;
            cin.ignore();
        }

        log<<"\n> Input file: "<< dataset;
        log<<"\n> Output file: "<< logfile;
        log<<"\n This session will run with the following parameters:";
        log<<"\n Mode: "<< mode;
        log<<"\n ACO: "<< aco_type;
        log<<"\n Clustering: "<< clustering_type;
        log<<"\n> Iterations: "<< iterations;
        log<<"\n> Nr. ants: "<< nr_ants;
        log<<"\n> Pheromone initial value: "<< initial_pher;
        log<<"\n> Pheromone evaporation value: "<< evaporation;
        log<<"\n> Pheromone influence: "<< pher_influence;
        log<<"\n> Heuristics influence: "<< heuristics_influence;

        log.close();

        run_aco session1;
        session1.set_nr_ants(nr_ants);
        session1.set_nr_iterations(iterations);
        session1.set_threshold(threshold);
        session1.set_verbosity(verbosity);
        session1.set_pher_params(initial_pher, evaporation);
        session1.set_IO_files(dataset,logfile);
        session1.set_priority_params(pher_influence, heuristics_influence);
        session1.generate_final_solution(mode, clustering_type, aco_type);

        cout<<"\n\n";
        
        cout<<"Do you want to begin another session ?(yes/no) ";  
        getline (cin,loop);
    }
    
    return 0;
}
