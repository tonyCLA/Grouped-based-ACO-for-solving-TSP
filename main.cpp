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

struct Solution {
    int cluster[100],p;// current solution for this ant as a vector;
                                         //is_cluster will have an appropriate value to indicate if the node in solution vector is a cluster; 
                                         //p represents the progress
};

struct Ants {
   int node_id,x,y; // the id of the node as well as coordinates
   Solution ant_sol; 
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
        ants[i].ant_sol.p=0;
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
        ants[i].ant_sol.p++;
        ants[i].ant_sol.cluster[ants[i].ant_sol.p]=nodes[rand_pos].c;
        ants[i].visited[rand_pos]=true;
        for(int j=1;j<=node_nr;j++)
            if(nodes[j].c == nodes[ants[i].node_id].c)
                ants[i].visited[j]=true;
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

//needs update 
void update_pheromone()
{
    int i,ant_id=1;
    for(i=2;i<=ants_number;i++)
        if(ants[i].road_length<ants[ant_id].road_length)
        {
            ant_id=i;
        }
    //cout<<"\nUpdating pheromone of ant"<<ant_id<<endl;
    for(i=2;i<=ants[ant_id].ant_sol.p;i++)
    {
        pheromone[ants[ant_id].ant_sol.cluster[i-1]][ants[ant_id].ant_sol.s[i]]+=0.5; 
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
            if(ants[i].ant_sol.p < node_nr)return 0;
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
        ants[ant_nr].ant_sol.p++;
        ants[ant_nr].ant_sol.s[ants[ant_nr].ant_sol.p]=tmp;
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
            cout<<ants[q].ant_sol.s[w]<<"->";
            if(update_solution)
                final_solution[w]=ants[q].ant_sol.s[w];
        }
        cout<<endl;
        update_solution=false;
    }   
}

//takes a vector as parameter, with following attributes;
//the position of each element in vector represents the node IDs
//the value of each vector represents the probability
//second parameter represents the number of nodes
int roulette_selection(float v[][2],int n)
{
    int i;
    float sum=0,rand_n;
    for(int i=1;i<=n;i++)
        sum+=v[i][1];
    
    rand_n=static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/sum));
    sum=0;
    cout<<"rand_n - "<<rand_n<<endl;
    for(int i=1;i<=n;i++)
    {
        if(sum<rand_n && sum+v[i][1]>rand_n)
            return v[i][2];
        sum+=v[i][1];
    }
    
    return 0;
}


//calculate the probability of moving from node1 to node2 
void calculate_probability(int node1,int node2,float &probability,int ant_id)
{
    int i,j;
    bool is_in_solution=false;
    float sum=0;
    for(i=1;i<=node_nr;i++)
    {
        is_in_solution=false;
        if(i!=node1 && i!=node2)
            for(j=1;j<=ants[ant_id].ant_sol.p;j++)
                if(ants[ant_id].ant_sol.s[j]==i)
                    is_in_solution=true;
        if (!is_in_solution)    
            sum+=pow( sqrt(pow((nodes[node1].x - nodes[i].x),2) + pow((nodes[node1].y - nodes[i].y),2) ), 2  )  *  pow(pheromone[node1][i],2);
    }
    probability=1/ pow( sqrt(pow((nodes[node1].x - nodes[node2].x),2) + pow((nodes[node1].y - nodes[node2].y),2) ), 2  )  *  pow(pheromone[node1][node2],2)/sum;
    
}

//calculate the available nodes to move at
void available_nodes(int v[],int &nr_of_nodes,int ant_id)
{
    int i,j,k,c,tmp;
    bool valid=true;
    nr_of_nodes=0;
    float dist=std::numeric_limits<float>::max();
    for(i=1;i<=node_nr;i++)
    {
        if(nodes[i].c != nodes[ants[ant_id].node_id].c)
        {
        c=nodes[i].c;
        dist=std::numeric_limits<float>::max();
        valid=false;
        for(j=1;j<=node_nr;j++)
        {
            if(nodes[j].c==c && i!=j && ants[ant_id].visited[j]==false)
            {
                if(sqrt( pow((nodes[j].x - ants[ant_id].x),2) + pow((nodes[j].y - ants[ant_id].y),2) ) < dist)
                {
                    dist=sqrt( pow((nodes[j].x - ants[ant_id].x),2) + pow((nodes[j].y - ants[ant_id].y),2) );
                    tmp=j;
                }
            }
            
        }
        
            for(k=1;k<=nr_of_nodes;k++)
                if(nodes[v[k]].c==c)valid=false;
            
            if(valid==true)
                v[++nr_of_nodes]=j;
        }
    }
            
}

void set_probabilities(float prob[][2], int v[], int nr_of_nodes, int ant_id)
{
    int i,j;
    for(i=1;i<=nr_of_nodes;i++)
    {
        prob[i][1]=v[i];
        calculate_probability(ants[ant_id].node_id,v[i],prob[i][2],ant_id);
    }
}

void move_ant(int ant_id,int new_node)
{
    if(new_node!=0)
                {
                    cout<<"> next node is "<<new_node<<":"<<nodes[new_node].x<<","<<nodes[new_node].y;
                    ants[ant_id].ant_sol.p++;
                    ants[ant_id].ant_sol.cluster[ants[ant_id].ant_sol.p]=ants;
                    ants[ant_id].road_length+=sqrt( pow((nodes[new_node].x - ants[ant_id].x),2) + pow((nodes[new_node].y - ants[ant_id].y),2) );
                    ants[ant_id].node_id=new_node;
                    ants[ant_id].x=nodes[new_node].x;
                    ants[ant_id].y=nodes[new_node].y;
                    ants[ant_id].visited[new_node]=true;
                    for (int i=1 ;i<=node_nr;i++)
                        if(nodes[ants[ant_id].node_id].c == nodes[i].c)
                            ants[ant_id].visited[i]=true;
                } 
}


//returns 1 or 0 if every cluster/node has been visited by the current ant (provided by param)
int check_visited(int ant_id)
{
    
    for (int i=1;i<=node_nr;i++)
        if(ants[ant_id].visited[i] == false) 
            return 1;
    return 0;
}

void run_aco()
{
    int x,y,tmp,av_nodes[100],nr_of_nodes;
    float dist,probabilities[100][2], highest_probability;
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
            while(check_visited(j))
            {
                available_nodes(av_nodes,nr_of_nodes,j); // picking up the minimum distances between ant and other clusters
            
                set_probabilities(probabilities,av_nodes,nr_of_nodes,j);
            
                tmp=roulette_selection(probabilities,nr_of_nodes);
                move_ant(j,tmp);
                
            }
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
    
    float v[6][2];
    v[1][1]=0.5;v[1][2]=3;
    v[2][1]=0.3;v[2][2]=4;
    v[3][1]=0.7;v[3][2]=5;
    v[4][1]=0.1;v[4][2]=6;
    v[5][1]=0.2;v[5][2]=10;
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
