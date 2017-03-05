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
#include <vector>

using namespace std;

/*
 * 
 */

struct Ants {
   int   x,y;
}; 

struct Nodes {
    int x,y,id;
};

int clusters[10][10], 
    pheromone[10][10], 
    node_nr,
    iterations, 
    ants_number,
    threshold;

struct Ants ants[10];
struct Nodes nodes[10];
/*
 * - graph = each node of the graph
 * - pheromone = pheromone levels on vertices NOT nodes
 * - iterations = the number of cycle the program will perform
 * - nodes = the number of cities(graph nodes) 
 */

void initialize_parameter()
{
    //graph.resize(100);
}

void initialize_matrices()
{
    std::fstream nodes_file("graph_nodes.txt", std::ios_base::in); //D:

    int a,b;
    node_nr=1;
    while (nodes_file >> a >> b)
    {
        printf("%d %d", a, b);
        nodes[node_nr].x=a;
        nodes[node_nr].y=b;
        node_nr++;
        clusters[a][b]=node_nr;// marking each node as a separate cluster;
    }
    getchar();
}

void display_variables()
{
    cout<<"Number of nodes is "<<node_nr<<endl;
    for(int i=1; i<=node_nr;i++)
    {
        cout<<nodes[i].x<<" - "<<nodes[i].y<<endl;
    }
}

int search_node(int x,int y)
{
    for(int i=1; i<=node_nr; i++)
    {
        if( nodes[i].x==x && nodes[i].y==y) return true;
    }
}

void set_pher()
{
    for (int a=1 ; a<=node_nr; a++)
        for (int b=1; b<=node_nr; b++)
            if (a!=b)
                pheromone[a][b]=0.5;
}

void cluster_nodes()
{
    int cluster_nr=1;
    for(int i=1;i<=node_nr;i++)
        for(int j=1; j<=node_nr; j++)
            if(clusters[i][j]>0)
            {
                cluster_nr=clusters[i][j];
                for (int a=1; a<=node_nr; a++)
                    for (int b=1; b<=node_nr; b++)
                        if ( sqrt( pow((i-a),2) + pow((j - b),2) ) <= threshold ) //   if euclidean distance between 2 points
                            clusters[a][b]=cluster_nr;                            //is smaller than a certain threshold the nodes are marked
            }                                                                     // as being part of same cluster
                            
            
}

void locate_ants()
{
    for (int i=1; i<=ants_number; i++)
    {
        int posx = rand() % node_nr + 1; // random number between 1 and 100; for x location 
        int posy = rand() & node_nr + 1; // same but for y location
        ants[i].x=posx;
        ants[i].y=posy;
    }
}

int start_aco()
{
    int x , y;
    for(int i=1 ; i<=iterations; i++)
    {
        locate_ants();
        for(int j=i; j<=ants_number; j++)
        {
          cout<<"s";  
        }
    }
    return 0; 
}
   

int main(int argc, char** argv) {
    //start_aco();
    //display_variables();
    initialize_matrices();
    display_variables();
    return 0;
}

