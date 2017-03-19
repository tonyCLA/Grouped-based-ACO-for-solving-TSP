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
#include <limits>

using namespace std;

/*
 * 
 */

struct Ants {
   int node_id,x,y; // the id of the node as well as coordinates
   int solution[100], p; // current solution for this ant as a vector; p represents the progress
   int visited[100][100]; // remember the nodes a particular ant visited
   float road_length; // total distance of the solution
}; 

struct Nodes {
    int x,y, cluster; // coordinates and cluster number
};

int clusters[100][100],
    final_solution[100],
    node_nr,
    iterations=5, 
    ants_number=4;

float pheromone[100][100],
      optimal_length=std::numeric_limits<float>::max();

double threshold;

struct Ants ants[100];
struct Nodes nodes[100];
/*
 * - graph = each node of the graph
 * - pheromone = pheromone levels on vertices NOT nodes
 * - iterations = the number of cycle the program will perform
 * - nodes = the number of cities(graph nodes) 
 */

void initialize()
{
    std::fstream nodes_file("small_dataset2.txt", std::ios_base::in); //D:

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

void display_pheromone_matrix()
{
    int i,j;
    for(i=1;i<=node_nr;i++,cout<<endl)
        for(j=1;j<=node_nr;j++)
        {
            cout<<pheromone[i][j]<<" ";
        }
}

//resetting all the variables that are going to be used for next iteration
void reset()
{
    int i,j,k;
    
    for(i=1; i<=ants_number; i++)
    {
        ants[i].p=0;
        ants[i].road_length=0;
        for(j=1;j<=node_nr;j++)
            for(k=1; k<=node_nr; k++)
            {
                ants[i].visited[j][k]=0;
            }
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
        ants[i].node_id=rand_pos;
        ants[i].x=nodes[rand_pos].x;
        ants[i].y=nodes[rand_pos].y;
        ants[i].p++;
        ants[i].solution[ants[i].p]=rand_pos;
        ants[i].visited[nodes[rand_pos].x][nodes[rand_pos].y]=1;
        }
}

void evaporate_pheromone()
{
    for(int i=1;i<=node_nr;i++)
        for(int j=1;j<=node_nr;j++)
        {
            if(pheromone[i][j]>0.2)
                pheromone[i][j]-=0.2; 
        }
}

void update_pheromone()
{
    int i,j;
    for(i=1;i<=ants_number;i++)
        for(j=2;j<=ants[i].p;j++)
        {
            if(i!=j && ants[i].solution[j]!=ants[i].solution[j-1])
                pheromone[ants[i].solution[j-1]][ants[i].solution[j]]+=0.5; 
        }
    evaporate_pheromone();
}

//returns the number of nodes in a cluster=
int get_cluster_nodes(int cluster_id, int ant_id)
{
    int i,j, nr=0;
    for(i=1;i<=node_nr;i++)
        for(j=1;j<=node_nr;j++)
            if(clusters[i][j]==cluster_id && ants[ant_id].visited[i][j]==0)nr++;
    
    return nr;
}

//checks if all nodes from a cluster are visited
int check_cluster_visited(int cluster_id, int ant_id)
{
    int i,j,nr=0;
    for(i=1;i<=node_nr;i++)
        for(j=1;j<=node_nr;j++)
            if(ants[ant_id].visited[i][j]==0 && clusters[i][j]==cluster_id)return 1;
        
        return 0;
}

//checks if all nodes of the graph are visited
// parameter ant id
int visited_all_nodes(int ant_id)
{
    int i,j;
    
    for(i=1;i<=node_nr;i++)
        for(j=1;j<=node_nr;j++)
            if(ants[ant_id].visited[i][j]==0 && clusters[i][j]!=0) return 1;
        
        return 0;
}

int ants_created_solution()
{
    int i,j;
    for(i=1;i<=ants_number;i++)
        for(j=1;j<=node_nr;j++)
            if(ants[i].p < node_nr)return 0;
        return 1;
}
/*
 * ant represents the details of the ant that is about to be moved
 * ant_nr represents the id of the ant 
 */
void shortest_path_within_cluster(int ant_nr)
{
    float min_dist;
    int tmp,x,y;
    cout<<"\n>> ant moving in cluster "<<clusters[ants[ant_nr].x][ants[ant_nr].y]<<endl;
    int cluster_nodes=get_cluster_nodes(clusters[ants[ant_nr].x][ants[ant_nr].y],ant_nr);
    //cout<<cluster_nodes<<endl;
    while(cluster_nodes>0)
    {
        tmp=0;
        min_dist=std::numeric_limits<float>::max();
        cout<<">> Moving (within cluster) ant number "<<ant_nr<<" currently on "<<ants[ant_nr].node_id<<":"<<ants[ant_nr].x<<","<<ants[ant_nr].y<<endl;
        for (int j=1; j<=node_nr; j++)
        {
            x=nodes[j].x;
            y=nodes[j].y;
            
            if(ants[ant_nr].visited[x][y]==0 && 
               ants[ant_nr].node_id!=j && 
               clusters[ants[ant_nr].x][ants[ant_nr].y] == clusters[x][y]) 
                if (sqrt( pow((ants[ant_nr].x - nodes[j].x),2) + pow((ants[ant_nr].y - nodes[j].y),2) ) < min_dist)
                    {
                        min_dist=sqrt( pow((ants[ant_nr].x - nodes[j].x),2) + pow((ants[ant_nr].y - nodes[j].y),2) );
                        tmp=j; // remembering the node id and the minimum distance
                    }
        }
        cout<<"> next node is "<<tmp<<":"<<nodes[tmp].x<<","<<nodes[tmp].y<<endl;
        ants[ant_nr].p++;
        ants[ant_nr].solution[ants[ant_nr].p]=tmp;
        ants[ant_nr].road_length+=min_dist;
        ants[ant_nr].node_id=tmp;
        ants[ant_nr].x=nodes[tmp].x;
        ants[ant_nr].y=nodes[tmp].y;
        ants[ant_nr].visited[ants[ant_nr].x][ants[ant_nr].y]=1;
        cluster_nodes--; 
    } 
    cout<<">> ant "<<ant_nr<< " visited cluster "<<clusters[ants[ant_nr].x][ants[ant_nr].y]<<endl;
    cout<<"Ant is at position "<<ants[ant_nr].node_id<<":"<<ants[ant_nr].x<<","<<ants[ant_nr].y<<"\n\n";
}

void update_final_solution()
{
    bool update_solution=false;
    for(int q=1;q<=ants_number;q++)
    {
        cout<<"Distance: "<<ants[q].road_length<<" == ";
        if(ants[q].road_length<optimal_length)
        {
            optimal_length=ants[q].road_length;
            update_solution=true;

        }
        cout<<"ant"<<q<<" had the solution:";
        for(int w=1;w<=node_nr+1;w++)
        {
            cout<<ants[q].solution[w]<<"->";
            if(update_solution)
                final_solution[w]=ants[q].solution[w];
        }
        cout<<endl;
        update_solution=false;
    }   
}

void calculate_probability(int node1,int node2,float *probability,int ant_id)
{
    int i,j;
    bool is_in_solution=false;
    float sum=0;
    for(i=1;i<=node_nr;i++)
    {
        is_in_solution=false;
        if(i!=node1 && i!=node2)
            for(j=1;j<=ants[ant_id].p;j++)
                if(ants[ant_id].solution[j]==i)
                    is_in_solution=true;
        if (!is_in_solution)    
            sum+=pow( sqrt(pow((nodes[node1].x - nodes[i].x),2) + pow((nodes[node1].y - nodes[i].y),2) ), 4  )  *  pow(pheromone[node1][i],2);
    }
    *probability=pow( sqrt(pow((nodes[node1].x - nodes[node2].x),2) + pow((nodes[node1].y - nodes[node2].y),2) ), 4  )  *  pow(pheromone[node1][node2],2)/sum;
}

void run_aco()
{
    int x,y,tmp;
    float dist,probability, highest_probability; 
    for(int i=1 ; i<=iterations; i++)
    {
        cout<<">>> Starting iteration "<<i<<endl;
        cout<<">>> (Re)Setting variables\n";
        reset();
        cout<<">>> Positioning ants\n";
        locate_ants();
        //while(!ants_created_solution())
        for(int j=1; j<=ants_number; j++)
        {
            //calculate shortest path within a cluster
            while(ants[j].p<node_nr)
                if(check_cluster_visited( clusters[ants[j].x][ants[j].y], j) )
                    shortest_path_within_cluster(j);
                else 
                {
                    cout<<"\n>> Moving (cluster change) ant number "<<j<<" currently on "<<ants[j].node_id<<":"<<ants[j].x<<","<<ants[j].y<<endl;
                    tmp=0;
                    dist=std::numeric_limits<float>::max();
                    highest_probability=std::numeric_limits<float>::max();
                    for(int k=1; k<=node_nr; k++)
                    {
                        calculate_probability(ants[j].node_id,k, &probability, j);
                        //cout<<ants[j].node_id<<","<<k<<"=> probability: "<<probability<<endl;
                        if ( ants[j].node_id != k && 
                            nodes[k].cluster != nodes[ants[j].node_id].cluster && 
                            !ants[j].visited[nodes[k].x][nodes[k].y])
                        if( probability < highest_probability)
                        {
                            //cout<<ants[j].node_id<<","<<k<<"=> probability: "<<probability<<endl;
                        
                            tmp=k;
                            highest_probability=probability;
                            dist=sqrt( pow((nodes[k].x - ants[j].x),2) + pow((nodes[k].y - ants[j].y),2) );
                        }        
                    }
                    if(tmp!=0)
                    {
                        cout<<"> next node is "<<tmp<<":"<<nodes[tmp].x<<","<<nodes[tmp].y;
                        ants[j].p++;
                        ants[j].solution[ants[j].p]=tmp;
                        ants[j].road_length+=dist;
                        ants[j].node_id=tmp;
                        ants[j].x=nodes[tmp].x;
                        ants[j].y=nodes[tmp].y;
                        ants[j].visited[ants[j].x][ants[j].y]=1;
                    }
                } 
            //adding 1st node as final node
            cout<<">> Adding starting node at the end of solution! \n";
            ants[j].p++;
            tmp=ants[j].solution[1];
            ants[j].solution[ants[j].p]=tmp;
            ants[j].road_length+=sqrt( pow((nodes[tmp].x - ants[j].x),2) + pow((nodes[tmp].y - ants[j].y),2) );
            ants[j].node_id=tmp;
            ants[j].x=nodes[tmp].x;
            ants[j].y=nodes[tmp].y;
            cout<<"\n\n";
        }// end of ants loop
        
        update_pheromone();
        cout<<"After iteration "<<i<<endl;
        update_final_solution();
        cout<<endl;
    }//end of iterations loop
}
  
void solution()
{
    int i,j;
    cout<<"\n The most efficient path has the total distance: "<< optimal_length;
    cout<<"\n The nodes that form this path are: ";
    for(i=1;i<=node_nr;i++)
        cout<<final_solution[i]<<"->";
    cout<<final_solution[node_nr+1]<<endl;
}

int main(int argc, char** argv) {
    cout<<"Initialize matrices";
    initialize();
    cout<<"Matrices created! Press any key to continue...";
    getchar();
    
    cout<<"Please specify distance threshold for clustering:\n";
    cin>>threshold;
    cout<<"Distance threshold is : "<<threshold<<endl;
    
    cout<<"Cluster nodes and display\n";
    display_nodes();
    cluster_nodes();
    display_clusters();
    
    set_pher();
    
    cout<<"Starting algorithm...";
    run_aco();
    
    //cout<<"pheromone matrix after computations \n ------------------"<<endl;
    //display_pheromone_matrix();
    
    solution();
    
    return 0;
}
