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
#include "node.h"

using namespace std;

/*
 * 
 */

struct Nodes {
    int c, id; //cluster number
    float x,y; //node coordinates(real numbers)
};

struct Ants {
   int node_id,x,y,sol_elements; // the id of the node as well as coordinates
   std::vector<int> solution;  // ant solution after each iteration; represented as a list of clusters
   std::vector<bool> visited; // remember the nodes a particular ant visited
   float road_length; // total distance of the solution
}; 

struct Clusters {
    int n_nodes,cluster_id;
    Nodes centroid;
    std::vector<int> node_list;
};


void initialize_data(vector<Nodes> &nodel, int &nr_nodes, vector<Ants> &antl, int &nr_ants, vector<vector<float> > &pher, vector<int> &partial_sol)
{
    int i,j;
    pher.resize(nr_nodes+1);
    partial_sol.resize(nr_nodes+1);
    
    //initialize pheromone matrix
    for(i=1;i<=nr_nodes;i++)
        pher[i].resize(nr_nodes+1);
    
    //initialize ant variables
    antl.resize(nr_ants+1);
    for(i=1;i<=nr_ants;i++)
    {
        antl[i].solution.resize(nr_nodes+1);
        antl[i].visited.resize(nr_nodes+1);
        antl[i].sol_elements=0;
        antl[i].road_length=0;
    }

}

void set_nodes(vector<Nodes> &nodel, int &nr_nodes)
{
    std::fstream nodes_file("small_dataset1.txt", std::ios_base::in); //D:

    float a,b,c;
    nr_nodes=1;
    
    cout<<"\nGetting nodes coordinates from file!\n";
    while (nodes_file /*>> c*/ >> a >> b)
    {
        nodel.resize(nr_nodes+1);
        nodel[nr_nodes].id=nr_nodes;
        nodel[nr_nodes].x=a;
        nodel[nr_nodes].y=b;
        nodel[nr_nodes].c=nr_nodes;// marking each node as a separate cluster;
        nr_nodes++;
    }
    nr_nodes--;
}

void display_nodes(vector<Nodes> nodel, int nr_nodes)
{
    cout<<"Number of nodes is "<<nr_nodes<<endl;
    for(int i=1; i<=nr_nodes;i++)
    {
        cout<<nodel[i].c<<" ("<<nodel[i].x<<", "<<nodel[i].y<<")\n";
    }
}

float distance(float x1, float x2, float y1, float y2)
{
   return sqrt( pow((x1-x2),2) + pow((y1-y2),2));
}

void cluster_nodes(float threshold, vector<Nodes> &nodel, int nr_nodes)
{
    int cluster_nr=1;
    for(int i=1;i<=nr_nodes;i++)
        {
            cluster_nr=nodel[i].c;
            
            for (int j=i+1; j<=nr_nodes; j++)
                //the code under sqrt function is basically euclidian distance between 2 points
                if ( distance( nodel[i].x, nodel[j].x, nodel[i].y, nodel[j].y ) < threshold )
                {
                    nodel[j].c=cluster_nr;
                }                            
            }                                
}

void cluster_nodes2(vector<Nodes> &nodel, int nr_nodes, vector<Clusters> &clusterl, int &nr_clusters)
{
   
}

void set_clusters(vector<Nodes> &nodel,int nr_nodes ,vector<Clusters> &clusterl, int &nr_clusters)
{
    int i,j, valid=true;
    nr_clusters=0;
    clusterl.resize(nr_clusters+1);

    for(i=1;i<=nr_nodes;i++)
    {
        valid=true;
        for(j=1;j<=nr_clusters;j++)
            if(nodel[i].c == clusterl[j].cluster_id)
            {
                valid=false;
                clusterl[j].n_nodes++;
                clusterl[j].node_list.resize(clusterl[j].n_nodes+1);
                clusterl[j].node_list[clusterl[j].n_nodes]=i;
            }
        
        if(valid)
        {
            nr_clusters++;
            clusterl.resize(nr_clusters+1);
            clusterl[nr_clusters].cluster_id=nodel[i].c;
            clusterl[nr_clusters].n_nodes=1;
            clusterl[nr_clusters].node_list.resize(clusterl[nr_clusters].n_nodes+1);
            clusterl[nr_clusters].node_list[clusterl[j].n_nodes]=i;
        }

    }
    
    for(i=2;i<=nr_clusters;i++)
    {
        clusterl[i].cluster_id=i;
        for(j=1;j<=clusterl[i].n_nodes;j++)
            nodel[clusterl[i].node_list[j]].c=i;
    }
        
}

void display_clusters(vector<Clusters> clusterl, int nr_clusters)
{
    cout<<"There are "<< nr_clusters<<" clusters:\n";
    for(int i=1; i<=nr_clusters; i++ , cout<<endl)
    {
        cout<<"cluster "<<i<<" has the following nodes ("<<clusterl[i].n_nodes<<")\n";
        for(int j=1;j<=clusterl[i].n_nodes;j++)
            cout<<clusterl[i].node_list[j]<<",";
    }
    cout<<endl;
}

void set_pher(vector<vector<float> > &pher, int nr_nodes)
{
    for (int a=1 ; a<=nr_nodes; a++)
        for (int b=1; b<=nr_nodes; b++)
            if (a!=b)
                pher[a][b]=0.5;
}

void display_pheromone(vector<vector<float> > pher, int nr_nodes)
{
    int i,j;
    for(i=1;i<=nr_nodes;i++,cout<<endl)
        for(j=1;j<=nr_nodes;j++)
        {
            cout<<pher[i][j]<<" ";
        }
}

void locate_ants(vector<Nodes> &nodel, int &nr_nodes, vector<Ants> &antl, int &nr_ants)
{
    for (int i=1; i<=nr_ants; i++)
    {   
        int rand_pos = rand() % nr_nodes + 1; // random number between 1 and node_nr;  
        antl[i].node_id=rand_pos;
        antl[i].x=nodel[rand_pos].x;
        antl[i].y=nodel[rand_pos].y;
        antl[i].sol_elements=1;
        antl[i].solution[antl[i].sol_elements]=rand_pos;
        antl[i].visited[rand_pos]=true;
        for(int j=1;j<=nr_nodes;j++)
            if(nodel[j].c == nodel[rand_pos].c)
                antl[i].visited[j]=true;
        }
}

void display_ants_status(vector<Ants> antl, int nr_ants, int nr_nodes)
{
    int i,j;
    for(i=1;i<=nr_ants;i++)
        {
            cout<<"\n\nAnt number: "<<i<<endl;
            cout<<"Node id: "<<antl[i].node_id<<endl;
            cout<<"Ant coodinates: "<<antl[i].x<<"-"<<antl[i].y<<endl;
            cout<<"Number_of_elements: "<<antl[i].sol_elements<<endl;
            
            cout<<"Ant solution vector: ";
            for(int j=1;j<=nr_nodes;j++)
                cout<<antl[i].solution[j]<<"->";
            cout<<"\nAnt visited vector: ";
            for(int j=1;j<=nr_nodes;j++)
                cout<<antl[i].visited[j]<<"->";
        }
    cout<<endl;
}

void reset(vector<Ants> &antl, int nr_ants, int nr_nodes)
{
    int i,j;
    
    for(i=1; i<=nr_ants; i++)
    {
        antl[i].sol_elements=0;
        antl[i].road_length=0;
        for(j=1;j<=nr_nodes;j++)
                antl[i].visited[j]=false;
    }
}

int check_visited(Ants antl, int nr_nodes)
{
    for (int i=1;i<=nr_nodes;i++)
        if(antl.visited[i] == false) 
            return 0;
    return 1;
}

void get_possible_routes(vector<int> &av_nodes, int &nr_av_nodes, Ants antl, vector<Nodes> nodel, int nr_nodes, vector<Clusters> clusterl, int nr_clusters)
{
    int ant_cluster=nodel[antl.node_id].c;
    int i,j,k;
    int x1,x2,y1,y2, node1, node2;
    float dist;
    
    nr_av_nodes=0;
    for(i=1;i<=nr_clusters;i++)
    {
        node1=0;
        node2=0;
        dist=std::numeric_limits<float>::max();
        if(antl.visited[clusterl[i].node_list[1]]==false && i!=ant_cluster)
        {
            for(j=1;j<=clusterl[ant_cluster].n_nodes;j++)
            {
                x2=nodel[clusterl[ant_cluster].node_list[j]].x;
                y2=nodel[clusterl[ant_cluster].node_list[j]].y;
                for(k=1;k<=clusterl[i].n_nodes;k++)
                {
                    x1=nodel[clusterl[i].node_list[k]].x;
                    y1=nodel[clusterl[i].node_list[k]].y;
                    if(distance(x1,x2,y1,y2) < dist)
                    {
                        dist=distance(x1,x2,y1,y2);
                        node1=clusterl[i].node_list[k];
                        node2=clusterl[ant_cluster].node_list[j];
                    }

                }
            }   
        }
        if(node1+node2 !=0 )
        {
            if(nodel[node1].c != ant_cluster)
            {
                k=node1;
                node1=node2;
                node2=k;
            }
            nr_av_nodes+=2;
            av_nodes.resize(nr_av_nodes+1);
            av_nodes[nr_av_nodes-1]=node1;
            av_nodes[nr_av_nodes]=node2;         
        }
    }    
}

void set_probabilities( vector<float> &prob, vector<int> av_nodes, int nr_av_nodes, vector<Nodes> nodel, vector<vector<float> > pher)
{
    int i,j,x1,x2,y1,y2;
    float sum;
    prob.resize(nr_av_nodes/2+1);
    
    for(j=1;j<=nr_av_nodes/2;j++)
    {
        x1=nodel[av_nodes[j*2]].x;
        x2=nodel[av_nodes[j*2-1]].x;
        y1=nodel[av_nodes[j*2]].y;
        y2=nodel[av_nodes[j*2-1]].y;
        sum+=pow( 1/distance(x1,x2,y1,y2), 2  )  *  pow(pher[av_nodes[j*2]][av_nodes[j*2-1]],2);

    }
    
    for(i=1;i<=nr_av_nodes/2;i++)
    {
        x1=nodel[av_nodes[i*2]].x;
        x2=nodel[av_nodes[i*2-1]].x;
        y1=nodel[av_nodes[i*2]].y;
        y2=nodel[av_nodes[i*2-1]].y;
        prob[i]= pow( 1/distance(x1,x2,y1,y2), 2  )  *  pow(pher[av_nodes[i*2]][av_nodes[i*2-1]],2)/sum;
    }
}

int roulette_selection(vector<float> prob, int nr_av_nodes)
{
    int i;
    float sum=0,rand_n;
    for(int i=1;i<=nr_av_nodes;i++)
        sum+=prob[i];
    
    rand_n=static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/sum));
    sum=0;
    for(int i=1;i<=nr_av_nodes;i++)
    {
        if(sum<rand_n && sum+prob[i]>rand_n)
            return i;
        sum+=prob[i];
    }
    
    return 0;
}

void move_ant(Ants &antl, int exit_node, int new_node, vector<Nodes> &nodel, int &nr_nodes )
{
    //cout<<"next node >>>>>> "<<new_node<<endl;
    if(new_node !=0 )
    {
        //cout<<"> next node is "<<new_node<<": "<<nodel[new_node].x<<","<<nodel[new_node].y<<endl;
        antl.sol_elements++;
        antl.solution.resize(antl.sol_elements+1);
        antl.solution[antl.sol_elements]=new_node;
        //cout<<"calculate distance between"<<exit_node<<"-"<<new_node<<", and add it to solution length"<<endl;
        antl.road_length+=distance(nodel[new_node].x, nodel[exit_node].x, nodel[new_node].y, nodel[exit_node].y);
        antl.node_id=new_node;
        antl.x=nodel[new_node].x;
        antl.y=nodel[new_node].y;
        antl.visited[new_node]=true;
        for (int i=1 ;i<=nr_nodes;i++)
            if(nodel[antl.node_id].c == nodel[i].c)
                antl.visited[i]=true;
    }
}

void update_pheromone(vector<Nodes> nodel, int nr_nodes, vector<Ants> antl, int nr_ants, vector<vector<float> > &pher)
{
    int i,j,k;
    Nodes n1,n2,copy;
    float dist, evaporation=0.1;
    cout<<">> Update pheromone.\n";
    for(i=1;i<=nr_ants;i++)
        for(j=1;j<antl[i].sol_elements;j++)
        {
            n1=nodel[antl[i].solution[j]];
            n2=nodel[antl[i].solution[j+1]];
            //cout<<n1.id<<"->"<<n2.id<<endl;
            dist=std::numeric_limits<float>::max();
            for(k=1;k<=nr_nodes;k++)
            {
                if(distance( nodel[k].x, n2.x, nodel[k].y, n2.y) < dist && n1.c != n2.c && n1.c == nodel[k].c)
                {
                    dist=distance( nodel[k].x, n2.x, nodel[k].y, n2.y);
                    copy=nodel[k];
                }
            }
            pher[copy.id][n2.id]+=1/dist;
            pher[n2.id][copy.id]+=1/dist;
        }
       for(i=1;i<=nr_nodes;i++)
           for(j=1;j<=nr_nodes;j++)
               if(pher[i][j]-evaporation>=0)
                   pher[i][j]-=evaporation;
}

void update_final_solution(vector<Ants> antl, int nr_ants, vector<int> &solution, float &sol_dist)
{
    int i, id;
    id=0;
    for(i=1;i<=nr_ants;i++)
    {
        if(antl[i].road_length<sol_dist)
        {
            id=i;
            sol_dist= antl[i].road_length;
        }
    }
    if(id!=0)
        for(i=1;i<=antl[id].sol_elements;i++)
        {  
            solution.resize(antl[id].sol_elements+1);
            solution[i]=antl[id].solution[i];
        }
}

void run_aco(int iter, vector<Nodes> nodel, int nr_nodes, vector<Ants> &antl, int &nr_ants, vector<vector<float> > &pher, vector<int> &partial_sol, float &ps_dist, vector<Clusters> clusterl, int nr_clusters)
{
    int x,y,tmp,nr_of_avnodes=0;
    ps_dist=std::numeric_limits<float>::max();
    vector<float> probabilities;
    vector<int> available_nodes;
    
    for(int i=1 ; i<=iter; i++)
    {
        cout<<">>> Starting iteration "<<i<<endl;
        cout<<">>> (Re)Setting variables\n";
        reset(antl, nr_ants, nr_nodes);
        cout<<">>> Positioning ants\n";
        locate_ants(nodel, nr_nodes, antl, nr_ants);
        
        for(int j=1; j<=nr_ants; j++)
        {
           
            while(!check_visited(antl[j],nr_nodes))
            {
                //picking up the minimum distances between ant current cluster and other clusters
                get_possible_routes(available_nodes,nr_of_avnodes, antl[j], nodel, nr_nodes, clusterl, nr_clusters);      
                set_probabilities(probabilities,available_nodes,nr_of_avnodes, nodel, pher);
                
                //gives more verbosity to output
                //cout<<"> Ant located on "<<antl[j].node_id<<" has the following options (routes)"<<nr_of_avnodes/2<<endl;
                //for(int k=1;k<=nr_of_avnodes;k+=2)
                //    cout<<probabilities[k/2+1]<<" | "<<available_nodes[k]<<"->"<<available_nodes[k+1]<<endl;
                
                if(nr_of_avnodes>2)
                    tmp=roulette_selection(probabilities,nr_of_avnodes/2)*2;
                else tmp=2;
                move_ant(antl[j],available_nodes[tmp-1],available_nodes[tmp], nodel, nr_nodes);
            }
            
            //move ant to initial cluster
            move_ant(antl[j],antl[j].solution[antl[j].sol_elements],antl[j].solution[1], nodel, nr_nodes);
            
        }// end of ants loop
        cout<<"After iteration "<<i<<"(total travelled distance between clusters)"<<endl;
        update_final_solution(antl, nr_ants, partial_sol, ps_dist);
        for(y=1;y<=nr_ants;y++)
        {
            cout<<"Ant "<<y<< " final solution ("<<antl[y].road_length<<"): ";
            for(x=1;x<=antl[y].sol_elements;x++)
                cout<<antl[y].solution[x]<<" -> ";
            cout<<"\n\n";
        }
        update_pheromone(nodel, nr_nodes, antl, nr_ants,pher); 
    }//end of iterations loop
}

vector<int> construct_final_solution(vector<int> ps, int nr_elem, vector<Nodes> nodel, int nr_nodes, vector<Clusters> clusterl)
{
    int i,j,k, current_cluster, current_node, pos=1, cpos, x1,x2, y1, y2;
    double min_dist;
    vector<int> final_sol;
    vector<bool> visited;
    final_sol.resize(nr_nodes+2);
    
    for(k=1;k<nr_elem;k++)
    {
        min_dist=std::numeric_limits<float>::max();
        current_cluster=nodel[ps[k]].c;
        current_node=ps[k];
        x1=nodel[current_node].x;
        y1=nodel[current_node].y;
        final_sol[pos]=current_node;
      
        visited.resize(clusterl[current_cluster].n_nodes+1);
        pos++;
        
        for(i=1;i<=clusterl[current_cluster].n_nodes;i++)
            if(clusterl[current_cluster].node_list[i]==current_node)
                visited[i]=true;
            else visited[i]=false;
        
        for(j=1;j<clusterl[current_cluster].n_nodes;j++)
        {
            min_dist=std::numeric_limits<float>::max();
            for(i=1;i<=clusterl[current_cluster].n_nodes;i++)
            {
                x2=nodel[clusterl[current_cluster].node_list[i]].x;
                y2=nodel[clusterl[current_cluster].node_list[i]].y;
                if(distance(x1,x2,y1,y2) < min_dist && visited[i]==false)
                {
                    min_dist=distance(x1,x2,y1,y2);
                    cpos=i;
                }
            }
            visited[cpos]=true;
            final_sol[pos]=clusterl[current_cluster].node_list[cpos];
            current_node=final_sol[pos];
            x1=nodel[current_node].x;
            y1=nodel[current_node].y;
            pos++;
        }
    }
    final_sol[pos]=final_sol[1];
    return final_sol;
}

void two_opt(vector<Nodes> nodel, int nr_nodes, vector<int> partial_sol, int nr_clusters, vector<int> &final_sol)
{
    
}

float calculate_length(vector<int> solution, vector<Nodes> all_nodes, int nr_nodes)
{
    float total=0;
    for(int i=2;i<=nr_nodes+1;i++)
            total+=distance(all_nodes[solution[i]].x, all_nodes[solution[i-1]].x, all_nodes[solution[i]].y, all_nodes[solution[i-1]].y);
    return total;
}

int main(int argc, char** argv) {

    int node_nr,
    iterations=5, 
    ant_nr=4,
    cluster_nr;

    float threshold=2.5;
    
    vector<int> partial_solution, final_solution;
    float solution_length;
    
    vector<vector<float> > pheromone;

    vector<Ants> ants;
    vector<Nodes> nodes;
    vector<Clusters> clusters;
    
    
    cout<<">> Reading the file with coordinates:\n";
    set_nodes(nodes,node_nr);
    
    cout<<">> Set data boundaries:\n";
    initialize_data(nodes, node_nr, ants, ant_nr, pheromone , partial_solution);
    
    cout<<"\n>> Display nodes:\n";
    display_nodes(nodes, node_nr);
    
    cout<<"\n>> Clustering nodes.\n";
    cluster_nodes(threshold, nodes, node_nr);
    
    set_clusters(nodes, node_nr , clusters, cluster_nr);

    cout<<"\n>> Display clusters:\n";
    display_clusters(clusters, cluster_nr);
    
    cout<<"\n>> Set pheromone levels.\n";
    set_pher(pheromone, node_nr);
    
    
    cout<<"\n>> Run ACO:\n ";
    run_aco(iterations, nodes, node_nr, ants, ant_nr, pheromone, partial_solution, solution_length, clusters, cluster_nr);
    
    cout<<"\n\n Partial solution (made of clusters) found with given parameters is ("<<solution_length<<"):";
    for(int i=1;i<=cluster_nr+1;i++)
        cout<<nodes[partial_solution[i]].c<<"("<<partial_solution[i]<<")"<<"->";
        
    //cout<<"\n>> Display clusters:\n";
    //display_clusters(clusters, cluster_nr);
    
    final_solution.resize(node_nr+1);
    final_solution=construct_final_solution(partial_solution,cluster_nr+1, nodes, node_nr, clusters);
    cout<<"\nTotal length: "<<calculate_length(final_solution, nodes, node_nr)<<endl;
    cout<<"Final Solution: ";
    for(int i=1;i<=node_nr;i++)
        cout<<final_solution[i]<<"->";
    cout<<final_solution[node_nr+1]<<endl;
    
    /*
    float s=0;
    s+=distance(nodes[1].x,nodes[8].x,nodes[1].y,nodes[8].x);
    s+=distance(nodes[8].x,nodes[38].x,nodes[8].y,nodes[38].x);
    s+=distance(nodes[38].x,nodes[31].x,nodes[38].y,nodes[31].x); 
    s+=distance(nodes[31].x,nodes[44].x,nodes[31].y,nodes[44].x); 
    s+=distance(nodes[44].x,nodes[18].x,nodes[44].y,nodes[18].x);
   
    
    cout<<endl<<s;
     */
    
    node n1,n2;
    n1.set_coord(3,4);
    n2.set_coord(5,4);
    cout<<n1.calc_dist(n2);
    
    return 0;
}
