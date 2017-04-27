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
#include "node.h"
#include "cluster.h"
#include "ant.h"
#include "run_aco.h"

using namespace std;

/*
 * 
 */

void initialize_data(int &nr_nodes, vector<ant> &antl, int &nr_ants, vector<vector<float> > &pher, vector<int> &partial_sol)
{
    cout<<">> Set variables boundaries:\n";
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
        antl[i].set_id(i);
        antl[i].solution.resize(nr_nodes+1);
        antl[i].visited.resize(nr_nodes+1);
        antl[i].set_sol_nr(0);
        antl[i].set_road_length(0);
    }

}

void set_nodes(vector<node> &nodel, int &nr_nodes)
{
    std::fstream nodes_file("small_dataset1.txt", std::ios_base::in); //D:
    cout<<">> Reading the file with coordinates:\n";
    float a,b,c;
    nr_nodes=1;
    
    cout<<"\nGetting nodes coordinates from file!\n";
    while (nodes_file /*>> c*/ >> a >> b)
    {
        nodel.resize(nr_nodes+1);
        nodel[nr_nodes].setid(nr_nodes);
        nodel[nr_nodes].set_coord(a,b);
        nodel[nr_nodes].set_cluster(nr_nodes);// marking each node as a separate cluster;
        nr_nodes++;
    }
    nr_nodes--;
}

void display_nodes(vector<node> nodel, int nr_nodes)
{
    cout<<"\n>> Display nodes:\n";
    cout<<"Number of nodes is "<<nr_nodes<<endl;
    for(int i=1; i<=nr_nodes;i++)
    {
        cout<<"id: "<<nodel[i].get_id()<<", c: "<<nodel[i].get_c()<<"; coord: ("<<nodel[i].get_x()<<", "<<nodel[i].get_y()<<")\n";
    }
}

/*
float distance(float x1, float x2, float y1, float y2)
{
   return sqrt( pow((x1-x2),2) + pow((y1-y2),2));
}
 */

void cluster_nodes(float threshold, vector<node> &nodel, int nr_nodes)
{
    cout<<"\n>> Clustering nodes.\n";
    int cluster_nr=1;
    for(int i=1;i<=nr_nodes;i++)
        {
            cluster_nr=nodel[i].get_c();
            
            for (int j=i+1; j<=nr_nodes; j++)
                //the code under sqrt function is basically euclidian distance between 2 points
                if ( nodel[i].calc_dist(nodel[j]) < threshold )
                {
                    nodel[j].set_cluster(cluster_nr);
                }                            
            }                                
}

void cluster_nodes2(vector<node> &nodel, int nr_nodes, vector<cluster> &clusterl, int &nr_clusters)
{
   
}

void set_clusters(vector<node> &nodel,int nr_nodes ,vector<cluster> &clusterl, int &nr_clusters)
{
    int i,j, valid=true;
    nr_clusters=0;
    clusterl.resize(nr_clusters+1);

    for(i=1;i<=nr_nodes;i++)
    {
        valid=true;
        for(j=1;j<=nr_clusters;j++)
            if(nodel[i].get_c() == clusterl[j].get_id())
            {
                valid=false;
                clusterl[j].set_nr_nodes(clusterl[j].get_nr_nodes()+1);
                clusterl[j].node_list.resize(clusterl[j].get_nr_nodes()+1);
                clusterl[j].node_list[clusterl[j].get_nr_nodes()]=nodel[i];
            }
        
        if(valid)
        {
            nr_clusters++;
            clusterl.resize(nr_clusters+1);
            clusterl[nr_clusters].set_id(nodel[i].get_c());
            clusterl[nr_clusters].set_nr_nodes(1);
            clusterl[nr_clusters].node_list.resize(clusterl[nr_clusters].get_nr_nodes()+1);
            clusterl[nr_clusters].node_list[clusterl[j].get_nr_nodes()]=nodel[i];
        }

    }
    
    for(i=2;i<=nr_clusters;i++)
    {
        clusterl[i].set_id(i); 
        for(j=1;j<=clusterl[i].get_nr_nodes();j++) 
            nodel[clusterl[i].node_list[j].get_id()].set_cluster(i); 
    }
        
}

void display_clusters(vector<cluster> clusterl, int nr_clusters)
{
    cout<<"\n>> Display clusters:\n";
    cout<<"There are "<< nr_clusters<<" clusters:\n";
    for(int i=1; i<=nr_clusters; i++ , cout<<endl)
    {
        cout<<"cluster "<<i<<" has the following nodes ("<<clusterl[i].get_nr_nodes()<<")\n";
        for(int j=1;j<=clusterl[i].get_nr_nodes();j++)
            cout<<clusterl[i].node_list[j].get_id()<<",";
    }
    cout<<endl;
}

void set_pher(vector<vector<float> > &pher, int nr_nodes)
{
    cout<<"\n>> Set pheromone levels.\n";
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

void locate_ants(vector<node> &nodel, int &nr_nodes, vector<ant> &antl, int &nr_ants)
{
    cout<<">>> Positioning ants\n";
    for (int i=1; i<=nr_ants; i++)
    {   
        int rand_pos = rand() % nr_nodes + 1; // random number between 1 and node_nr;  
        antl[i].set_position(nodel[rand_pos]);
        antl[i].set_sol_nr(1);
        antl[i].solution[antl[i].get_sol_nr()]=rand_pos;
        antl[i].visited[rand_pos]=true;
        for(int j=1;j<=nr_nodes;j++)
            if( nodel[j].get_c() == nodel[rand_pos].get_c() )
                antl[i].visited[j]=true;
        }
}

void display_ants_status(vector<ant> antl, int nr_ants, int nr_nodes)
{
    int i,j;
    for(i=1;i<=nr_ants;i++)
        {
            cout<<"\n\nAnt number: "<<antl[i].get_id()<<endl;
            cout<<"Node id: "<<antl[i].get_postion().get_id()<<endl;
            cout<<"Ant coodinates: "<<antl[i].get_postion().get_x()<<"-"<<antl[i].get_postion().get_y()<<endl;
            cout<<"Number_of_elements: "<<antl[i].get_sol_nr()<<endl;
            
            cout<<"Ant solution vector: ";
            for(int j=1;j<=nr_nodes;j++)
                cout<<antl[i].solution[j]<<"->";
            cout<<"\nAnt visited vector: ";
            for(int j=1;j<=nr_nodes;j++)
                cout<<antl[i].visited[j]<<"->";
        }
    cout<<endl;
}

void reset(vector<ant> &antl, int nr_ants, int nr_nodes)
{
    cout<<">>> (Re)Setting variables\n";
    int i,j;
    for(i=1; i<=nr_ants; i++)
    {
        antl[i].set_sol_nr(0);
        antl[i].set_road_length(0);
        for(j=1;j<=nr_nodes;j++)
                antl[i].visited[j]=false;
    }
}

int check_visited(ant antl, int nr_nodes)
{
    for (int i=1;i<=nr_nodes;i++)
        if(antl.visited[i] == false) 
            return 0;
    return 1;
}

void get_possible_routes(vector<int> &av_nodes, int &nr_av_nodes, ant antl, vector<node> nodel, int nr_nodes, vector<cluster> clusterl, int nr_clusters)
{
    int ant_cluster=antl.get_postion().get_c();
    int i,j,k;
    int node1, node2;
    float dist;
    node n1,n2;
    
    nr_av_nodes=0;
    for(i=1;i<=nr_clusters;i++)
    {
        node1=0;
        node2=0;
        dist=std::numeric_limits<float>::max();
        if(antl.visited[clusterl[i].node_list[1].get_id()]==false && i!=ant_cluster)
        {
            for(j=1;j<=clusterl[ant_cluster].get_nr_nodes();j++)
            {
                n1=nodel[clusterl[ant_cluster].node_list[j].get_id()];
                for(k=1;k<=clusterl[i].get_nr_nodes();k++)
                {
                    n2=nodel[clusterl[i].node_list[k].get_id()];
                    if(n1.calc_dist(n2) < dist)
                    {
                        dist=n1.calc_dist(n2);
                        node1=clusterl[i].node_list[k].get_id();
                        node2=clusterl[ant_cluster].node_list[j].get_id();
                    }

                }
            }   
        }
        if(node1+node2 !=0 )
        {
            if(nodel[node1].get_c() != ant_cluster)
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

void set_probabilities( vector<float> &prob, vector<int> av_nodes, int nr_av_nodes, vector<node> nodel, vector<vector<float> > pher)
{
    int i,j;
    float sum;
    prob.resize(nr_av_nodes/2+1);
    
    for(j=1;j<=nr_av_nodes/2;j++)
    {
        sum+=pow( 1/nodel[av_nodes[j*2]].calc_dist(nodel[av_nodes[j*2-1]]), 2  )  *  pow(pher[av_nodes[j*2]][av_nodes[j*2-1]],2);

    }
    
    for(i=1;i<=nr_av_nodes/2;i++)
    {
        prob[i]= pow( 1/nodel[av_nodes[i*2]].calc_dist(nodel[av_nodes[i*2-1]]), 2  )  *  pow(pher[av_nodes[i*2]][av_nodes[i*2-1]],2)/sum;
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

void move_ant(ant &antl, int exit_node, int new_node, vector<node> &nodel, int &nr_nodes )
{
    //cout<<"next node >>>>>> "<<new_node<<endl;
    if(new_node !=0 )
    {
        //cout<<"> next node is "<<new_node<<": "<<nodel[new_node].x<<","<<nodel[new_node].y<<endl;
        antl.update_sol_nr(1);
        antl.solution.resize(antl.get_sol_nr()+1);
        antl.solution[antl.get_sol_nr()]=new_node;
        //cout<<"calculate distance between"<<exit_node<<"-"<<new_node<<", and add it to solution length"<<endl;
        antl.update_road_length(nodel[new_node].calc_dist(nodel[exit_node]));
        antl.set_position(nodel[new_node]);
        antl.visited[new_node]=true;
        for (int i=1 ;i<=nr_nodes;i++)
            if(antl.get_postion().get_c() == nodel[i].get_c())
                antl.visited[i]=true;
    }
}

void update_pheromone(vector<node> nodel, int nr_nodes, vector<ant> antl, int nr_ants, vector<vector<float> > &pher)
{
    int i,j,k;
    node n1,n2,copy;
    float dist, evaporation=0.1;
    cout<<">> Update pheromone.\n";
    for(i=1;i<=nr_ants;i++)
        for(j=1;j<antl[i].get_sol_nr();j++)
        {
            n1=nodel[antl[i].solution[j]];
            n2=nodel[antl[i].solution[j+1]];
            //cout<<n1.id<<"->"<<n2.id<<endl;
            dist=std::numeric_limits<float>::max();
            for(k=1;k<=nr_nodes;k++)
            {
                if( nodel[k].calc_dist(n2) < dist && n1.get_c() != n2.get_c() && n1.get_c() == nodel[k].get_c())
                {
                    dist=nodel[k].calc_dist(n2);
                    copy=nodel[k];
                }
            }
            pher[copy.get_id()][n2.get_id()]+=1/dist;
            pher[n2.get_id()][copy.get_id()]+=1/dist;
        }
    
       for(i=1;i<=nr_nodes;i++)
           for(j=1;j<=nr_nodes;j++)
               if(pher[i][j]-evaporation>=0)
                   pher[i][j]-=evaporation;
}

void update_final_solution(vector<ant> antl, int nr_ants, vector<int> &solution, float &sol_dist)
{
    int i, id;
    id=0;
    for(i=1;i<=nr_ants;i++)
    {
        if(antl[i].get_road_length()<sol_dist)
        {
            id=i;
            sol_dist= antl[i].get_road_length();
        }
    }
    if(id!=0)
        for(i=1;i<=antl[id].get_sol_nr();i++)
        {  
            solution.resize(antl[id].get_sol_nr()+1);
            solution[i]=antl[id].solution[i];
        }
}

void run_aco(int iter, vector<node> nodel, int nr_nodes, vector<ant> &antl, int &nr_ants, vector<vector<float> > &pher, vector<int> &partial_sol, float &ps_dist, vector<cluster> clusterl, int nr_clusters)
{
    int x,y,tmp,nr_of_avnodes=0;
    ps_dist=std::numeric_limits<float>::max();
    vector<float> probabilities;
    vector<int> available_nodes;
    
    for(int i=1 ; i<=iter; i++)
    {
        cout<<">>> Starting iteration "<<i<<endl;
        
        reset(antl, nr_ants, nr_nodes);
        
        locate_ants(nodel, nr_nodes, antl, nr_ants);
        cout<<">>> Generate solution for each ant\n";
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
            move_ant(antl[j],antl[j].solution[antl[j].get_sol_nr()],antl[j].solution[1], nodel, nr_nodes);
            
        }// end of ants loop
        cout<<"After iteration "<<i<<"(total travelled distance between clusters)"<<endl;
        update_final_solution(antl, nr_ants, partial_sol, ps_dist);
        for(y=1;y<=nr_ants;y++)
        {
            cout<<"Ant "<<y<< " final solution ("<<antl[y].get_road_length()<<"): ";
            for(x=1;x<antl[y].get_sol_nr();x++)
                cout<<antl[y].solution[x]<<" -> ";
            cout<<antl[y].solution[antl[y].get_sol_nr()]<<endl;
        }
        update_pheromone(nodel, nr_nodes, antl, nr_ants,pher); 
    }//end of iterations loop
}

vector<int> construct_final_solution(vector<int> ps, int nr_elem, vector<node> nodel, int nr_nodes, vector<cluster> clusterl)
{
    int i,j,k, current_cluster, current_node, pos=1, cpos, x1,x2, y1, y2;
    double min_dist;
    vector<int> final_sol;
    vector<bool> visited;
    final_sol.resize(nr_nodes+2);
    
    for(k=1;k<nr_elem;k++)
    {
        min_dist=std::numeric_limits<float>::max();
        current_cluster=nodel[ps[k]].get_c();
        current_node=ps[k];
        x1=nodel[current_node].get_x();
        y1=nodel[current_node].get_y();
        final_sol[pos]=current_node;
      
        visited.resize(clusterl[current_cluster].get_nr_nodes()+1);
        pos++;
        
        for(i=1;i<=clusterl[current_cluster].get_nr_nodes();i++)
            if(clusterl[current_cluster].node_list[i].get_id()==current_node)
                visited[i]=true;
            else visited[i]=false;
        
        for(j=1;j<clusterl[current_cluster].get_nr_nodes();j++)
        {
            min_dist=std::numeric_limits<float>::max();
            for(i=1;i<=clusterl[current_cluster].get_nr_nodes();i++)
            {
                x2=clusterl[current_cluster].node_list[i].get_x();
                y2=clusterl[current_cluster].node_list[i].get_y();
                if(nodel[current_node].calc_dist(clusterl[current_cluster].node_list[i]) < min_dist && visited[i]==false)
                {
                    min_dist=nodel[current_node].calc_dist(clusterl[current_cluster].node_list[i]);
                    cpos=i;
                }
            }
            visited[cpos]=true;
            final_sol[pos]=clusterl[current_cluster].node_list[cpos].get_id();
            current_node=final_sol[pos];
            x1=nodel[current_node].get_x();
            y1=nodel[current_node].get_y();
            pos++;
        }
    }
    final_sol[pos]=final_sol[1];
    return final_sol;
}

//further optimize the local tours of each cluster
void two_opt(vector<node> nodel, int nr_nodes, vector<int> partial_sol, int nr_clusters, vector<int> &final_sol, vector<cluster> clusterl)
{
    float min_dist=std::numeric_limits<float>::max();; //shortest road length
    bool check=true; // boolean value which will be update after each 2opt iteration
                     // it will turn to false if the are aren't any other solutions
    int s_pos,e_pos,cluster=nodel[final_sol[1]].get_c(), i,j; /* local tour starting (s_pos) and ending position(e_pos) */
            
    while(check)
    {
        for(i=2;i<nr_nodes;i++)
        {
            if(nodel[i].get_c() == cluster)
                for(j=i+2;;j++)
                {
                    
                }
        }
    }
}

float calculate_length(vector<int> solution, vector<node> all_nodes, int nr_nodes)
{
    float total=0;
    for(int i=2;i<=nr_nodes+1;i++)
            total+=all_nodes[solution[i]].calc_dist(all_nodes[solution[i-1]]);
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
    
    std::vector<vector<float> > pheromone;

    std::vector<ant> ants;
    std::vector<node> nodes;
    std::vector<cluster> clusters;
    
    
    set_nodes(nodes,node_nr);
    
    
    initialize_data(node_nr, ants, ant_nr, pheromone , partial_solution);
    
    
    display_nodes(nodes, node_nr);
    
    
    cluster_nodes(threshold, nodes, node_nr);
    
    set_clusters(nodes, node_nr , clusters, cluster_nr);

    
    display_clusters(clusters, cluster_nr);
    
    
    set_pher(pheromone, node_nr);
    
    
    cout<<"\n>> Run ACO:\n ";
    run_aco(iterations, nodes, node_nr, ants, ant_nr, pheromone, partial_solution, solution_length, clusters, cluster_nr);
    
    cout<<"\n\n Partial solution (made of clusters) found with given parameters is ("<<solution_length<<"):";
    for(int i=1;i<=cluster_nr+1;i++)
        cout<<nodes[partial_solution[i]].get_c()<<"("<<partial_solution[i]<<")"<<"->";
        
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
    
    vector<node>::const_iterator first = nodes.begin() + 10;
    vector<node>::const_iterator last = nodes.begin() + 16;
    vector<node> newVec(first, last);
    
    display_nodes(newVec, 5);
    
    node n1,n2;
    n1.set_coord(3,4);
    n2.set_coord(5,4);
    cout<<n1.calc_dist(n2);
    
    return 0;
}
