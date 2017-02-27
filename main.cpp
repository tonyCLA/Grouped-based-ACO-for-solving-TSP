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

#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

/*
 * 
 */

int graph[100][100], heuristic[100][100][3], iterations; 
/*
 * - graph matrix display represent each nodes of the graph
 * - heuristic matrix represent the vertices of the graph with heuristics(1 distance and 2 pheromone level)
 *   E.g.: graph[54][21][1] - has the distance between nodes 54 and 21;
 *         graph[54][21][2] - has the pheromone level of the vertex between 
 * - iterations represent the number of cycle the program will perform 
 */

void initialize_matrices()
{
    
}

void group_nodes()
{
    
}

void locate_ants()
{
    
}

void next_city(int &x, int&y)
{
    
}

int start_aco()
{
    
    for(int i=1 ; i<=iterations; i++)
    {
        locate_ants();
        next_city();
    }
    return 0; 
}
   

int main(int argc, char** argv) {
    return 0;
}

