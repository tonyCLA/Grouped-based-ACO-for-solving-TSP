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

/*
 * this class acts as a manager
 * it performs the most important steps
 */

#ifndef RUN_ACO_H
#define RUN_ACO_H

#include <vector>

#include "node.h"
#include "cluster.h"
#include "ant.h"
#include "output.h"


using std::cout;
using std::endl;
using std::cin;

class run_aco
{
    int nr_nodes,
    iterations, 
    nr_ants,
    nr_clusters,
    verbosity,
    distance_priority,
    pheromone_priority;
    float threshold, initial_pher, evaporation;
    
    
    std::string dataset, logfile;
    
    std::vector<int> partial_solution, final_solution;
    float psol_length, fsol_length;
    
    std::vector<std::vector<float> > pheromone_table;

    std::vector<ant> ants;
    std::vector<node> nodes;
    std::vector<cluster> clusters;
public:
    void set_nr_ants(int nr);
    void set_nr_iterations(int nr);
    void set_verbosity(int nr);
    void set_threshold(float nr);
    void set_IO_files(std::string data_file, std::string log_file);
    void set_priority_params(int p1, int p2);
    void set_pher_params(float initial_pherv, float evaporation_val);
    void set_nodes();
    void initialize_data();
    void display_nodes();
    void cluster_nodes_method1();
    void cluster_nodes_method2();
    void organize_clusters();
    void display_clusters();
    void set_pheromone(float initial_pher_levels);
    void display_pheromone();
    void locate_ants();
    void display_ant_status();
    void reset();
    int check_visited(ant ants);
    void get_possible_routes(std::vector<int> &av_nodes, int &nr_av_nodes, ant ants);
    void set_probabilities(std::vector<float> &prob, std::vector<int> av_nodes, int nr_av_nodes);
    int roulette_selection(std::vector<float> prob, int nr_av_nodes);
    void move_ant(ant &antl, int exit_node, int new_node );
    void update_pheromone(float evaporation);
    void update_partial_solution();
    void start_aco(float evaporation);
    void construct_final_solution(int nr_elem);
    std::vector<int> two_opt_local(std::vector<int> sample, int sample_size);
    void refine_solution();
    float calculate_length(std::vector<int> tour, int tour_size);
    std::vector<int> generate_final_solution(int mode);
};

void run_aco::set_nr_ants(int nr)
{
    nr_ants=nr;
}

void run_aco::set_nr_iterations(int nr)
{
    iterations=nr;
}

void run_aco::set_verbosity(int nr)
{
    verbosity=nr;
}

void run_aco::set_threshold(float nr)
{
    threshold=nr;
}

void run_aco::set_IO_files(std::string data_file, std::string log_file)
{
    dataset=data_file;
    logfile=log_file;
}

void run_aco::set_priority_params(int p1, int p2)
{
    pheromone_priority=p1;
    distance_priority=p2; 
}

void run_aco::set_pher_params(float initial_pherv, float evaporation_val)
{
    initial_pher=initial_pherv;
    evaporation=evaporation_val;
}

void run_aco::set_nodes()
{
    std::fstream nodes_file(dataset.c_str(), std::ios_base::in); //D:
    float a,b,c;
    nr_nodes=1;
    while (nodes_file >> c >> a >> b)
    {
        nodes.resize(nr_nodes+1);
        nodes[nr_nodes].setid(nr_nodes);
        nodes[nr_nodes].set_coord(a,b);
        nodes[nr_nodes].set_cluster(nr_nodes);// marking each node as a separate cluster;
        nr_nodes++;
    }
    nr_nodes--;
    nodes_file.close();
}

void run_aco::initialize_data()
{
    int i,j;
    partial_solution.resize(nr_nodes+1);   
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
    FileOutput log(logfile.c_str());
    log<<"\n>> Display nodes:\n";
    log<<"Number of nodes is "<<nr_nodes<<"\n";
    for(int i=1; i<=nr_nodes;i++)
    {
        log<<"id: "<<nodes[i].get_id()<<", c: "<<nodes[i].get_c()<<"; coord: ("<<nodes[i].get_x()<<", "<<nodes[i].get_y()<<")\n";
    }
}


int count_clusters(std::vector<node> nodelist, int nr_of_nodes)
{
    int count=0;
    bool check=false;
    for(int i=1;i<=nr_of_nodes;i++)
    {
        check=false;
        for(int j=i-1;j>=1;j--)
        {
            if(nodelist[i].get_c()==nodelist[j].get_c())
                check=true;
        }
        if(!check)
            count++;
    }       
    
    return count;
}

/*
 * Method 1 
 *  Clustering hierarchically using a threshold
 *         initially each node is a cluster
 *         combining two closest clusters (euclidean distance) at each iterations
 *         until the distance between any two clusters is above the threshold
 */

void run_aco::cluster_nodes_method1()
{
    float dist, min_dist;
    int c1, c2;
    bool step=false;
    nr_clusters=0;
    while(!step)
    {
        step=true;
        min_dist=std::numeric_limits<float>::max();
        for(int i=1;i<=nr_nodes && step;i++)
        {
            c1=nodes[i].get_c();
            for(int j=1;j<=nr_nodes;j++)
            {
                dist=nodes[i].calc_dist(nodes[j]);
                if(i!=j && nodes[i].get_c()!=nodes[j].get_c() && dist<min_dist && dist<threshold)
                {
                    c2=nodes[j].get_c();
                    min_dist=dist;
                    step=false;
                }
            }

        }

        for(int j=1;j<=nr_nodes;j++)
            if(nodes[j].get_c() == c2)
            {
                nodes[j].set_cluster(c1);
            }
    }
}

/*
 * Method 2
 * Clustering hierarchically assuming the dataset has n clusters
 *  group closest clusters at each iteration
 *  until the number of clusters is higher than the expected number of clusters
 */

void run_aco::cluster_nodes_method2()
{
    float dist, min_dist;
    int c1, c2;
    bool step=false;
    while(!step)
    {
        step=true;
        min_dist=std::numeric_limits<float>::max();
        for(int i=1;i<=nr_nodes;i++)
        {
            for(int j=1;j<=nr_nodes;j++)
            {
                dist=nodes[i].calc_dist(nodes[j]);
                if(i!=j && nodes[i].get_c()!=nodes[j].get_c() &&  dist<min_dist)
                {
                    cout<<"dist:"<< dist<<"nodes: "<<nodes[i].get_id()<<"->"<<nodes[j].get_id()<<endl;
                    c1=nodes[i].get_c();
                    c2=nodes[j].get_c();
                    min_dist=dist;
                    step=false;
                }
            }
        }
       if(count_clusters(nodes,nr_nodes)>threshold)
       {
        for(int j=1;j<=nr_nodes;j++)
            if(nodes[j].get_c() == c2 || nodes[j].get_c() == c1)
            {
                nodes[j].set_cluster((c1+c2)*(-1));
            }
         cout<<"Join cluster "<<c1<<" with "<< c2<<" - "<<count_clusters(nodes,nr_nodes)<<endl;
        }
    }
}

void run_aco::organize_clusters()
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
    
    for(i=1;i<=nr_clusters;i++)
    {
        clusters[i].set_id(i); 
        for(j=1;j<=clusters[i].get_nr_nodes();j++) 
            nodes[clusters[i].node_list[j].get_id()].set_cluster(i); 
        clusters[i].calculate_centroid();
    }
        clusters[1].calculate_centroid();
}

void run_aco::display_clusters()
{
    FileOutput log(logfile.c_str());
    log<<"\nThe centroid of any cluster is not a dataset point!";
    log<<"\n>> Display clusters:\n";
    log<<"There are "<< nr_clusters<<" clusters:\n";
    for(int i=1; i<=nr_clusters; i++ , log<<"\n")
    {
        log<<"cluster "<<i<<" has the following nodes ("<<clusters[i].get_nr_nodes()<<")\n";
        log<<"cluster centroid: "<< clusters[i].get_centroid().get_x()<<","<<clusters[i].get_centroid().get_y()<<"\n";
        for(int j=1;j<=clusters[i].get_nr_nodes();j++)
            log<<clusters[i].node_list[j].get_id()<<",";
    }
    log<<"\n";
}

void run_aco::set_pheromone(float initial_pher_levels)
{
    
    pheromone_table.resize(nr_clusters+1);
    //initialize pheromone matrix
     
    for(int i=1;i<=nr_clusters;i++)
        pheromone_table[i].resize(nr_clusters+1);
     
    for (int a=1 ; a<=nr_clusters; a++)
        for (int b=1; b<=nr_clusters; b++)
            if (a!=b)
                pheromone_table[a][b]=initial_pher_levels;
}

void run_aco::display_pheromone()
{
    int i,j;
    for(i=1;i<=nr_nodes;i++,cout<<endl)
        for(j=1;j<=nr_nodes;j++)
        {
            cout<<pheromone_table[i][j]<<" ";
        }
}

void run_aco::locate_ants()
{
    for (int i=1; i<=nr_ants; i++)
    {   
        int rand_pos = rand() % nr_nodes + 1; // random number between 1 and node_nr;  
        ants[i].set_position(nodes[rand_pos]);
        ants[i].set_sol_nr(1);
        ants[i].solution[ants[i].get_sol_nr()]=rand_pos;
        ants[i].visited[rand_pos]=true;
        for(int j=1;j<=nr_nodes;j++)
            if( nodes[j].get_c() == nodes[rand_pos].get_c() )
                ants[i].visited[j]=true;
    }
}

void run_aco::display_ant_status()
{
    FileOutput log(logfile.c_str());
    int i,j;
    for(i=1;i<=nr_ants;i++)
        {
            log<<"\n\nAnt number: "<<ants[i].get_id()<<endl;
            log<<"Node id: "<<ants[i].get_postion().get_id()<<endl;
            log<<"Ant coodinates: "<<ants[i].get_postion().get_x()<<"-"<<ants[i].get_postion().get_y()<<endl;
            log<<"Number_of_elements: "<<ants[i].get_sol_nr()<<endl;
            
            log<<"Ant solution vector: ";
            for(int j=1;j<=nr_nodes;j++)
                log<<ants[i].solution[j]<<"->";
            log<<"\nAnt visited vector: ";
            for(int j=1;j<=nr_nodes;j++)
                log<<ants[i].visited[j]<<"->";
        }
    log<<"\n";
}

void run_aco::reset()
{
    int i,j;
    for(i=1; i<=nr_ants; i++)
    {
        ants[i].set_sol_nr(0);
        ants[i].set_road_length(0);
        for(j=1;j<=nr_nodes;j++)
                ants[i].visited[j]=false;
    }
}

int run_aco::check_visited(ant ants)
{
    for (int i=1;i<=nr_nodes;i++)
        if(ants.visited[i] == false) 
            return 0;
    return 1;
}

void run_aco::get_possible_routes(std::vector<int> &av_nodes, int &nr_av_nodes, ant ants)
{
    int ant_cluster=ants.get_postion().get_c();
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
        if(ants.visited[clusters[i].node_list[1].get_id()]==false && i!=ant_cluster)
        {
            for(j=1;j<=clusters[ant_cluster].get_nr_nodes();j++)
            {
                n1=nodes[clusters[ant_cluster].node_list[j].get_id()];
                for(k=1;k<=clusters[i].get_nr_nodes();k++)
                {
                    n2=nodes[clusters[i].node_list[k].get_id()];
                    if(n1.calc_dist(n2) < dist)
                    {
                        dist=n1.calc_dist(n2);
                        node1=clusters[i].node_list[k].get_id();
                        node2=clusters[ant_cluster].node_list[j].get_id();
                    }

                }
            }   
        }
        if(node1+node2 !=0 )
        {
            if(nodes[node1].get_c() != ant_cluster)
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

void run_aco::set_probabilities( std::vector<float> &prob, std::vector<int> av_nodes, int nr_av_nodes)
{
    int j,c1,c2;
    float sum=0;
    prob.resize(nr_av_nodes/2+1);

    for(j=1;j<=nr_av_nodes/2;j++)
    {
        c1=nodes[av_nodes[j*2]].get_c();
        c2=nodes[av_nodes[j*2-1]].get_c();
        sum+=pow( 1/nodes[av_nodes[j*2]].calc_dist(nodes[av_nodes[j*2-1]]), distance_priority  )  *  pow(pheromone_table[c1][c2], pheromone_priority );
    }
    
    for(j=1;j<=nr_av_nodes/2;j++)
    {
        c1=nodes[av_nodes[j*2]].get_c();
        c2=nodes[av_nodes[j*2-1]].get_c();
        prob[j]=(pow( 1/nodes[av_nodes[j*2]].calc_dist(nodes[av_nodes[j*2-1]]), distance_priority  )  *  pow(pheromone_table[c1][c2], pheromone_priority ))/sum;

    }
}

int run_aco::roulette_selection(std::vector<float> prob, int nr_av_nodes)
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

void run_aco::move_ant(ant &antl, int exit_node, int new_node )
{
    if(new_node !=0 )
    {
        antl.update_sol_nr(1);
        antl.solution.resize(antl.get_sol_nr()+1);
        antl.solution[antl.get_sol_nr()]=new_node;
        antl.update_road_length(nodes[new_node].calc_dist(nodes[exit_node]));
        antl.set_position(nodes[new_node]);
        antl.visited[new_node]=true;
        for (int i=1 ;i<=nr_nodes;i++)
            if(antl.get_postion().get_c() == nodes[i].get_c())
                antl.visited[i]=true;
    }
}

void run_aco::update_pheromone(float evaporation)
{
    int j,k;
    node n1,n2,copy;

    //pheromone evaporation
   for(k=1;k<=nr_clusters;k++)
       for(j=1;j<=nr_clusters;j++)
           if(pheromone_table[k][j]-evaporation>=0)
               pheromone_table[k][j]-=evaporation;
           else pheromone_table[k][j]=0.001;
    
    for(int i=1;i<=nr_ants;i++)
        for(j=1;j<ants[i].get_sol_nr();j++)
        {
            n1=nodes[ants[i].solution[j]];
            n2=nodes[ants[i].solution[j+1]];
            pheromone_table[n1.get_c()][n2.get_c()]+=1/ants[i].get_road_length();
            pheromone_table[n2.get_c()][n1.get_c()]+=1/ants[i].get_road_length();
        }
    
    
}

void run_aco::update_partial_solution()
{
    int i, id;
    id=0;
    for(i=1;i<=nr_ants;i++)
    {
        if(ants[i].get_road_length()<psol_length)
        {
            id=i;
            psol_length= ants[i].get_road_length();
        }
    }
    if(id!=0)
        for(i=1;i<=ants[id].get_sol_nr();i++)
        {  
            partial_solution.resize(ants[id].get_sol_nr()+1);
            partial_solution[i]=ants[id].solution[i];
        }
}

void run_aco::start_aco(float evaporation)
{
    FileOutput log(logfile.c_str());
    log<<"\n>> Run ACO:\n ";
    int x,y,tmp,nr_of_avnodes=0, ant_id;
    psol_length=std::numeric_limits<float>::max();
    float best_length=std::numeric_limits<float>::max();
    std::vector<float> probabilities;
    std::vector<int> available_nodes;
    
    for(int i=1 ; i<=iterations; i++)
    {
        log<<"\n>>> Starting iteration "<<i;
        
        log<<"\n>>> (Re)Setting variables\n";
        reset(); 
        
        log<<">>> Positioning ants\n";
        locate_ants();
        
        log<<">>> Generate solution for each ant\n";
        for(int j=1; j<=nr_ants; j++)
        {
            while(!check_visited(ants[j]))
            {
                //picking up the minimum distances between ant current cluster and other clusters
                get_possible_routes(available_nodes,nr_of_avnodes, ants[j]);      
                set_probabilities(probabilities,available_nodes,nr_of_avnodes);
                if(nr_of_avnodes>2)
                    tmp=roulette_selection(probabilities,nr_of_avnodes/2)*2;
                else tmp=2;
                if(verbosity>=3)
                {
                log<<"> Ant located on cluster "<<ants[j].get_postion().get_c()<<" has the following options (probability | route)"<<nr_of_avnodes/2<<"\n";
                for(int k=1;k<=nr_of_avnodes;k+=2)
                   log<<probabilities[k/2+1]<<" | "<<available_nodes[k]<<"->"<<available_nodes[k+1]<<"\n";         
                log<<"next node is "<<available_nodes[tmp]<<"\n";
                } 
                move_ant(ants[j],available_nodes[tmp-1],available_nodes[tmp]);
            } 
            //move ant to initial cluster
            move_ant(ants[j],ants[j].solution[ants[j].get_sol_nr()],ants[j].solution[1]);
            
        }// end of ants loop
        
        update_partial_solution();
        
        if(verbosity>=2)
        {
            log<<"After iteration "<<i<<"(total travelled distance between clusters)\n";
            for(y=1;y<=nr_ants;y++)
            {
                if(ants[y].get_road_length() < best_length)
                {
                    psol_length;
                    ant_id=ants[y].get_id();
                }
                log<<"Ant "<<y<< " partial solution ("<<ants[y].get_road_length()<<"): ";
                for(x=1;x<ants[y].get_sol_nr();x++)
                    log<<nodes[ants[y].solution[x]].get_c()<<" -> ";
                log<<nodes[ants[y].solution[ants[y].get_sol_nr()]].get_c()<<"\n";
            }
        }
        log<<">> Update pheromone.\n";
        update_pheromone(evaporation); 
    }//end of iterations loop
}

void run_aco::construct_final_solution(int nr_elem)
{
    int i,j,k, current_cluster, current_node, pos=1, cpos, x1,x2, y1, y2;
    double min_dist;
    std::vector<bool> visited;
    final_solution.resize(nr_nodes+2);
    
    for(k=1;k<nr_elem;k++)
    {
        min_dist=std::numeric_limits<float>::max();
        current_cluster=nodes[partial_solution[k]].get_c();
        current_node=partial_solution[k];
        x1=nodes[current_node].get_x();
        y1=nodes[current_node].get_y();
        final_solution[pos]=current_node;
      
        visited.resize(clusters[current_cluster].get_nr_nodes()+1);
        pos++;
        
        for(i=1;i<=clusters[current_cluster].get_nr_nodes();i++)
            if(clusters[current_cluster].node_list[i].get_id()==current_node)
                visited[i]=true;
            else visited[i]=false;
        
        for(j=1;j<clusters[current_cluster].get_nr_nodes();j++)
        {
            min_dist=std::numeric_limits<float>::max();
            for(i=1;i<=clusters[current_cluster].get_nr_nodes();i++)
            {
                x2=clusters[current_cluster].node_list[i].get_x();
                y2=clusters[current_cluster].node_list[i].get_y();
                if(nodes[current_node].calc_dist(clusters[current_cluster].node_list[i]) < min_dist && visited[i]==false)
                {
                    min_dist=nodes[current_node].calc_dist(clusters[current_cluster].node_list[i]);
                    cpos=i;
                }
            }
            visited[cpos]=true;
            final_solution[pos]=clusters[current_cluster].node_list[cpos].get_id();
            current_node=final_solution[pos];
            x1=nodes[current_node].get_x();
            y1=nodes[current_node].get_y();
            pos++;
        }
    }
    final_solution[pos]=final_solution[1];
}

void swap_val(int &x, int &y)
{
    int placeholder;
    placeholder=x;
    x=y;
    y=placeholder;
}

//further optimize the local tour, params : local tour and size of local tour
std::vector<int> run_aco::two_opt_local(std::vector<int> sample, int sample_size)
{
    float min_length=calculate_length(sample,sample_size); //shortest road length
    bool check=true;      
    std::vector<int> copy;
    copy.resize(sample_size+1);
    int i,j,placeholder;
    copy=sample;
    while(check)
    {
        check=false;
        for(i=2;i<=sample_size-2 && !check;i++)
        {
            for(j=i+1;j<=sample_size-1 && !check;j++)
            {
                
                swap_val(copy[i],copy[j]);
                if(calculate_length(copy,sample_size)< min_length)
                {
                    check=true;
                    min_length=calculate_length(copy,sample_size);
                    sample=copy;
                }
                
                copy=sample;
            }
        }
    }
    return sample;
}

void vector_append(std::vector<int> &v1, int v1_size, std::vector<int> v2, int v2_size)
{   
    v1.resize(v1_size+v2_size+1);
    for(int i=1;i<v2_size;i++)
        v1[v1_size+i]=v2[i];
}

void run_aco::refine_solution()
{
    int i,tour_s, position=0;
    std::vector<int> copy;
    
    for(i=1;i<partial_solution.size()-1;i++)
    {
        tour_s=clusters[nodes[partial_solution[i]].get_c()].get_nr_nodes()+1;
        std::vector<int>::const_iterator limit1 = final_solution.begin()+ position;
        std::vector<int>::const_iterator limit2 = final_solution.begin() + position + tour_s + 1;
        std::vector<int> sub_vec(limit1, limit2);
        if(tour_s>=4)
        {
            sub_vec=two_opt_local(sub_vec,tour_s);
        }
        vector_append(copy,position,sub_vec,tour_s);
        position+=tour_s-1;
        
    }
    copy[copy.size()-1]=copy[1];
    final_solution=copy;
}

float run_aco::calculate_length(std::vector<int> tour, int tour_size)
{
    float total=0;
    for(int i=2;i<=tour_size;i++)
            total+=nodes[tour[i]].calc_dist(nodes[tour[i-1]]);
    return total;
}

std::vector<int> run_aco::generate_final_solution(int mode)
{
    //the following line will ensure that the program will use different seed each time it runs
    // so that the random number generated using rand() function will be different on each run
    srand (time(NULL));
    FileOutput log(logfile.c_str());
    cout<<"\n\n    Starting Session\n";
    cout<<"\n>> Reading the dataset:\n";
    set_nodes();
    cout<<">> Set variables boundaries:\n";
    initialize_data();
    display_nodes();

    if(mode==1)
    {
        cout<<"\n>> Clustering nodes.\n";
        cluster_nodes_method2();
    }
    
    organize_clusters();
    display_clusters();
    cout<<"\n>> Set pheromone levels.\n";
    set_pheromone(0.5);
    
    cout<<"<<<<<<"<<count_clusters(nodes,nr_nodes);
    
    start_aco(evaporation); 
    
    final_solution.resize(nr_nodes+1);
    construct_final_solution(nr_clusters+1);
    
    log<<"\n>> Refine final solution.";
    refine_solution();
    
    log<<"\n\nTotal length : "<<calculate_length(final_solution, nr_nodes+1);
    log<<"\nFinal Solution: ";
    for(int i=1;i<=nr_nodes;i++)
        log<<final_solution[i]<<"->";
    log<<final_solution[nr_nodes+1]<<"\n";
    
    cout<<"\nSession Terminated!\n";
    
    return final_solution;
}

#endif /* RUN_ACO_H */

