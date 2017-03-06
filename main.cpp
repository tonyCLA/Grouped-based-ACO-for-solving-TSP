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
   int node_id,x,y; // the id of the node as well as coordinates
   int solution[100], p; // current solution for this ant as a vector; p represents the progress
   float road_length; // total distance of the solution
}; 

struct Nodes {
    int x,y, cluster; // coordinates and cluster number
};

int clusters[100][100], 
    pheromone[100][100], 
    visited[100][100],
    node_nr,
    iterations, 
    ants_number;

double threshold;

struct Ants ants[100];
struct Nodes nodes[100];
/*
 * - graph = each node of the graph
 * - pheromone = pheromone levels on vertices NOT nodes
 * - iterations = the number of cycle the program will perform
 * - nodes = the number of cities(graph nodes) 
 */

void initialize_variables()
{
    for(int i=1;i<=node_nr;i++)
        for(int j=1; j<=node_nr; j++)
        {
            clusters[i][j]=0;
            pheromone[i][j]=0;
            visited[i][j]=0;
        }
}

void initialize_node_matrices()
{
    std::fstream nodes_file("graph_nodes.txt", std::ios_base::in); //D:

    int a,b;
    node_nr=1;
    cout<<"Getting nodes coordinates from file!\n";
    while (nodes_file >> a >> b)
    {
        printf("%d %d", a, b);
        nodes[node_nr].x=a;
        nodes[node_nr].y=b;
        clusters[a][b]=node_nr;// marking each node as a separate cluster;
        node_nr++;
        cout<<endl;
    }
    node_nr--;
}

void display_nodes()
{
    cout<<"Number of nodes is "<<node_nr<<endl;
    for(int i=1; i<=node_nr;i++, cout<<endl)
    {
        cout<<nodes[i].x<<" - "<<nodes[i].y;
    }
}

void display_clusters()
{
    cout<<"Clusters will be represent as a matrix:\n";
    for(int i=1; i<=node_nr; i++ , cout<<endl)
    {
        for(int j=1; j<=node_nr; j++ )
            cout<<clusters[i][j]<<" ";
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
        {
            cluster_nr=clusters[nodes[i].x][nodes[i].y];
            
            for (int j=i+1; j<=node_nr; j++)
                //the code under sqrt function is basically euclidian distance between 2 points
                if ( sqrt( pow((nodes[i].x - nodes[j].x),2) + pow((nodes[i].y - nodes[j].y),2) ) <= threshold )
                {
                    clusters[nodes[j].x][nodes[j].y]=cluster_nr;
                    nodes[j].cluster=cluster_nr;
                }                            
            }                                
}

void locate_ants()
{
    for (int i=1; i<=ants_number; i++)
    {   
        int rand_pos = rand() % node_nr + 1; // random number between 1 and 100; for x location 
        ants[i].x=nodes[rand_pos].x;
        ants[i].y=nodes[rand_pos].y;
        }
}

void update_pheromone()
{
    
}

/*
 * ant represents the details of the ant
 * p represents the progress of solution; e.g. how many nodes are there in the solution already; it has a pointer
 *      so it will be dynamically updated
 * node_id represents the ant location and the starting point for calculating shortest distance
 */
void shortest_path(Ants *ant,int ant_nr)
{
    int min_dist=100;
    for (int i=1; i<=node_nr; i++)
    {
        if(ant->node_id == i)
        for (int j=1; j<=node_nr; j++)
        {
            if(visited[i][j]==0) 
                //distance between initial node and 
                if (sqrt( pow((nodes[i].x - nodes[j].x),2) + pow((nodes[i].y - nodes[j].y),2) ) < min_dist)
                    {
                        min_dist=sqrt( pow((nodes[i].x - nodes[j].x),2) + pow((nodes[i].y - nodes[j].y),2) );
                        ant->node_id=j;
                    }
        ant->p++;
        ants[ant_nr].solution[ant->p]=ant->node_id;
        ants[ant_nr].road_length+=min_dist;
        ants[ant_nr].node_id=ant->node_id;
        ants[ant_nr].x=nodes[ant->node_id].x;
        ants[ant_nr].y=nodes[ant->node_id].y;
        visited[ants[ant_nr].x][ants[ant_nr].y]=1;
        }
    }
}

void run_aco()
{
    int x,y,h=0; // x,y are the coordinates; h will represent the heuristics;
    for(int i=1 ; i<=iterations; i++)
    {
        locate_ants();
        for(int j=1; j<=ants_number; j++)
        {
            //chosing next node
            shortest_path(&ants[j],j);
            //---
            for(int k=1; k<=node_nr; k++)
            {
                if ( ants[j].node_id != k && nodes[k].cluster != nodes[ants[j].node_id].cluster && !visited[nodes[k].x][nodes[k].y])
                    //the code under sqrt function is basically euclidian distance between 2 points
                    if( sqrt( pow((nodes[k].x - ants[j].x),2) + pow((nodes[k].y - ants[j].y),2) )/ pheromone[ants[j].node_id][k] < h)
                    {
                        ants[j].solution[ants[j].p]=k;
                        h=sqrt( pow((nodes[k].x - ants[j].x),2) + pow((nodes[k].y - ants[j].y),2) )/ pheromone[ants[j].node_id][k];
                    }        
            }
                
        }
    } 
}
   

int main(int argc, char** argv) {
    cout<<"Initialize matrices";
    initialize_node_matrices();
    cout<<"Matrices created! Press any key to continue...";
    getchar();
    
    cout<<"Please specify distance threshold for clustering:\n";
    cin>>threshold;
    cout<<"Distance threshold is : "<<threshold;
    
    cout<<"Cluster nodes and display";
    display_nodes();
    cluster_nodes();
    display_clusters();
    cout<<"Press any key to continue...";
    getchar();
    
    cout<<"Starting algorithm...";
    //run_aco();
    
    return 0;
}

