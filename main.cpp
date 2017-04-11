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
   bool visited[100]; // remember the nodes a particular ant visited
   float road_length; // total distance of the solution
}; 

struct Nodes {
    int c; //cluster number
    float x,y; //node coordinates(real numbers)
};

int final_solution[100],
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
    std::fstream nodes_file("small_dataset1.txt", std::ios_base::in); //D:

    float a,b;
    node_nr=1;
    
    cout<<"\nGetting nodes coordinates from file!\n";
    while (nodes_file >> a >> b)
    {
        nodes[node_nr].x=a;
        nodes[node_nr].y=b;
        nodes[node_nr].c=node_nr;// marking each node as a separate cluster;
        node_nr++;
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
    cout<<"Clusters:\n";
    for(int i=1; i<=node_nr; i++ , cout<<endl)
    {
        cout<<"node "<<i<<" in cluster : "<<nodes[i].c<<" ";
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
    int i,j;
    
    for(i=1; i<=ants_number; i++)
    {
        ants[i].p=0;
        ants[i].road_length=0;
        for(j=1;j<=node_nr;j++)
                ants[i].visited[j]=false;
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
            cluster_nr=nodes[i].c;
            
            for (int j=i+1; j<=node_nr; j++)
                //the code under sqrt function is basically euclidian distance between 2 points
                if ( sqrt( pow((nodes[i].x - nodes[j].x),2) + pow((nodes[i].y - nodes[j].y),2) ) <= threshold )
                {
                    nodes[j].c=cluster_nr;
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
        ants[i].visited[rand_pos]=true;
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
    int i,ant_id=1;
    for(i=2;i<=ants_number;i++)
        if(ants[i].road_length<ants[ant_id].road_length)
        {
            ant_id=i;
        }
    for(i=2;i<=ants[ant_id].p;i++)
    {
        pheromone[ants[ant_id].solution[i-1]][ants[ant_id].solution[i]]+=0.5; 
    }
evaporate_pheromone();
}

//returns the number of nodes in a cluster=
int get_cluster_nodes(int cluster_id, int ant_id)
{
    int i, nr=0;
    for(i=1;i<=node_nr;i++)
        if(nodes[i].c==cluster_id && ants[ant_id].visited[i]==false)nr++;
    
    return nr;
}

//checks if all nodes from a cluster are visited
int check_cluster_visited(int cluster_id, int ant_id)
{
    int i,nr=0;
    for(i=1;i<=node_nr;i++)
        if(ants[ant_id].visited[i]==false && nodes[i].c==cluster_id)return 1;
        
        return 0;
}

//checks if all nodes of the graph are visited
// parameter ant id
int visited_all_nodes(int ant_id)
{
    int i,j;
    
    for(i=1;i<=node_nr;i++)
        if(ants[ant_id].visited[i]==0 && nodes[i].c!=false) return 1;
        
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
    cout<<"\n>> ant moving in cluster "<<nodes[ants[ant_nr].node_id].c<<endl;
    int cluster_nodes=get_cluster_nodes(nodes[ants[ant_nr].node_id].c,ant_nr);
    //cout<<cluster_nodes<<endl;
    while(cluster_nodes>0)
    {
        tmp=0;
        min_dist=std::numeric_limits<float>::max();
        cout<<">> Moving (within cluster) ant number "<<ant_nr<<" currently on "<<ants[ant_nr].node_id<<":"<<ants[ant_nr].x<<","<<ants[ant_nr].y<<endl;
        for (int j=1; j<=node_nr; j++)
        {   
            if(ants[ant_nr].visited[j]==0 && 
               ants[ant_nr].node_id!=j && 
               nodes[ants[ant_nr].node_id].c == nodes[j].c) 
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
        ants[ant_nr].visited[tmp]=true;
        cluster_nodes--; 
    } 
    cout<<">> ant "<<ant_nr<< " visited cluster "<<nodes[ants[ant_nr].node_id].c<<endl;
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

//takes a vector as parameter, with following attributes;
//the position of each element in vector represents the node IDs
//the value of each vector represents the probability
//second parameter represents the number of nodes
int roulette_selection(float v[],int n)
{
    int i;
    float sum=0,rand_n;
    for(int i=1;i<=n;i++)
        sum+=v[i];
    
    rand_n=static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/sum));
    sum=0;
    cout<<"rand_n - "<<rand_n<<endl;
    for(int i=1;i<=n;i++)
    {
        if(sum<rand_n && sum+v[i]>rand_n)
            return i;
        sum+=v[i];
    }
    
    return 0;
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
                if(check_cluster_visited( nodes[ants[j].node_id].c, j) )
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
                            nodes[k].c != nodes[ants[j].node_id].c && 
                            !ants[j].visited[k])
                        {
                            //cout<<probability<<endl;
                            if( probability < highest_probability)
                            {

                                //cout<<ants[j].node_id<<","<<k<<"=> probability: "<<probability<<endl;

                                tmp=k;
                                highest_probability=probability;
                                dist=sqrt( pow((nodes[k].x - ants[j].x),2) + pow((nodes[k].y - ants[j].y),2) );
                            }
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
                        ants[j].visited[tmp]=true;
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
    
    float v[6];
    v[1]=0.5;
    v[2]=0.3;
    v[3]=0.7;
    v[4]=0.1;
    v[5]=0.2;
    for(int i=1;i<=10;i++)
    {
    cout<<roulette_selection(v,5)<<" - "<<static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1.8))<<endl;
    }
    
    cout<<"Initialize matrices";
    initialize();
    cout<<"Core variables set! Press any key to continue...";
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
