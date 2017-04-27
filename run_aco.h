/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   run_aco.h
 * Author: clc18
 *
 * Created on 27 April 2017, 16:57
 */

#ifndef RUN_ACO_H
#define RUN_ACO_H

using std::cout;
using std::endl;
using std::cin;

class run_aco
{
    int nr_nodes,
    iterations, 
    nr_ants,
    nr_clusters;

    float threshold;
    
    std::vector<int> partial_solution, final_solution;
    float solution_length;
    
    std::vector<std::vector<float> > pheromone;

    std::vector<ant> ants;
    std::vector<node> nodes;
    std::vector<cluster> clusters;
public:
    void set_nodes();
    void initialize_data();
    void display_nodes();
    void cluster_nodes();
    void set_clusters();
};

void run_aco::set_nodes()
{
    std::fstream nodes_file("small_dataset1.txt", std::ios_base::in); //D:
    cout<<">> Reading the file with coordinates:\n";
    float a,b,c;
    nr_nodes=1;
    
    cout<<"\nGetting nodes coordinates from file!\n";
    while (nodes_file /*>> c*/ >> a >> b)
    {
        nodes.resize(nr_nodes+1);
        nodes[nr_nodes].setid(nr_nodes);
        nodes[nr_nodes].set_coord(a,b);
        nodes[nr_nodes].set_cluster(nr_nodes);// marking each node as a separate cluster;
        nr_nodes++;
    }
    nr_nodes--;
}

void run_aco::initialize_data()
{
    cout<<">> Set variables boundaries:\n";
    int i,j;
    pheromone.resize(nr_nodes+1);
    partial_solution.resize(nr_nodes+1);
    
    //initialize pheromone matrix
    for(i=1;i<=nr_nodes;i++)
        pheromone[i].resize(nr_nodes+1);
    
    //initialize ant variables
    ants.resize(nr_ants+1);
    for(i=1;i<=nr_ants;i++)
    {
        ants[i].set_id(i);
        ants[i].solution.resize(nr_nodes+1);
        ants[i].visited.resize(nr_nodes+1);
        ants[i].set_sol_nr(0);
        ants[i].set_road_length(0);
    }

}

void run_aco::display_nodes()
{
    cout<<"\n>> Display nodes:\n";
    cout<<"Number of nodes is "<<nr_nodes<<endl;
    for(int i=1; i<=nr_nodes;i++)
    {
        cout<<"id: "<<nodes[i].get_id()<<", c: "<<nodes[i].get_c()<<"; coord: ("<<nodes[i].get_x()<<", "<<nodes[i].get_y()<<")\n";
    }
}

void run_aco::cluster_nodes()
{
    cout<<"\n>> Clustering nodes.\n";
    int cluster_nr=1;
    for(int i=1;i<=nr_nodes;i++)
        {
            cluster_nr=nodes[i].get_c();
            
            for (int j=i+1; j<=nr_nodes; j++)
                //the code under sqrt function is basically euclidian distance between 2 points
                if ( nodes[i].calc_dist(nodes[j]) < threshold )
                {
                    nodes[j].set_cluster(cluster_nr);
                }                            
            }                                
}

void run_aco::set_clusters()
{
    int i,j, valid=true;
    nr_clusters=0;
    clusters.resize(nr_clusters+1);

    for(i=1;i<=nr_nodes;i++)
    {
        valid=true;
        for(j=1;j<=nr_clusters;j++)
            if(nodes[i].get_c() == clusters[j].get_id())
            {
                valid=false;
                clusters[j].set_nr_nodes(clusters[j].get_nr_nodes()+1);
                clusters[j].node_list.resize(clusters[j].get_nr_nodes()+1);
                clusters[j].node_list[clusters[j].get_nr_nodes()]=nodes[i];
            }
        
        if(valid)
        {
            nr_clusters++;
            clusters.resize(nr_clusters+1);
            clusters[nr_clusters].set_id(nodes[i].get_c());
            clusters[nr_clusters].set_nr_nodes(1);
            clusters[nr_clusters].node_list.resize(clusters[nr_clusters].get_nr_nodes()+1);
            clusters[nr_clusters].node_list[clusters[j].get_nr_nodes()]=nodes[i];
        }

    }
    
    for(i=2;i<=nr_clusters;i++)
    {
        clusters[i].set_id(i); 
        for(j=1;j<=clusters[i].get_nr_nodes();j++) 
            nodes[clusters[i].node_list[j].get_id()].set_cluster(i); 
    }
        
}

#endif /* RUN_ACO_H */

