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
#include <cmath>

using namespace std;

/*
 * 
 */

int graph[100][100], 
        clusters[100][100], 
        pheromone[100][100], 
        iterations, 
        nodes, 
        ants,
        threshold; 
/*
 * - graph = each node of the graph
 * - pheromone = pheromone levels on vertices NOT nodes
 * - iterations = the number of cycle the program will perform
 * - nodes = the number of cities(graph nodes) 
 */

void initialize_matrices()
{
    std::fstream nodes_file("graph.txt", std::ios_base::in); //D:

    int a,b, node_nr=1;
    while (nodes_file >> a >> b)
    {
        printf("%d %d", a, b);
        graph[a][b]=node_nr; //marking the node on the matrix
        node_nr++;
        clusters[a][b]=1;
    }

    getchar();

    return 0;
}

void set_pher()
{
    for (int i=1 ; i<=nodes; i++)
        for (int j=1; j<=nodes; j++)
            if(graph[i][j]!=0)
                for (int a=1; a<=nodes; a++)
                    for (int b=1; b<=nodes; b++)
                        if (i!=a && j!=b && graph[i][j]!=0)
                            pheromone[graph[i][j]][graph[a][b]]=0.5;
}

void cluster_nodes()
{
    int cluster_nr=1;
    for(int i=1;i<=nodes;i++)
        for(int j=1; j<=nodes; j++)
            if(clusters[i][j]>1)
            {
                cluster_nr++;
                for (int a=1; a<=nodes; a++)
                    for (int b=1; b<=nodes; b++)
                        if ( sqrt( pow((i-a),2) + pow((j - b),2) ) <= threshold )
                            clusters[a][b]=cluster_nr;
            }               
                            
            
}

void locate_ants()
{
    
}

void next_city(int &x, int&y)
{
    
}


void add_to_solution()
{
}

void update_pheromone()
{
    
}
int start_aco()
{
    int x , y;
    for(int i=1 ; i<=iterations; i++)
    {
        locate_ants();
        for(int j=i; j<=ants; j++)
        {
            next_city(x,y);
            update_pheromone(x,y);
            add_to_solution(x,y);
        }
    }
    return 0; 
}
   

int main(int argc, char** argv) {
    //start_aco();
    return 0;
}

