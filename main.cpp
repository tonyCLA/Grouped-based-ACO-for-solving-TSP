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
    int nr_ants, iterations;
    string dataset="small_dataset2.txt";
    
    cout<<"Please enter number of iterations: ";
    //cin>>iterations;
    
    cout<<"Please enter the number of ants: ";
    //cin>>nr_ants;
    
    cout<<"Please enter the dataset location and filename (.txt file): ";
    //getline (cin.ignore(),dataset); 

    cout<<"\nACO will run with the following parameters: \n";
    cout<<"> Iterations: "<< iterations<<endl;
    cout<<"> Nr. ants: "<< nr_ants<<endl;
    cout<<"> Dataset file: "<< dataset<<endl;
    
    run_aco session1;
    session1.set_dataset(dataset);
    session1.set_logfile("output.log");
    session1.set_ants(1);
    session1.set_iterations(10);
    session1.set_threshold(2.5);
    session1.generate_final_solution(1);
  
    cout<<"\nProgram Terminated!\n";
    return 0;
}
