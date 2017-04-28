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
    
    std::string dataset, logfile;
    
    std::vector<int> partial_solution, final_solution;
    float psol_length, fsol_length;
    
    std::vector<std::vector<float> > pheromone;

    std::vector<ant> ants;
    std::vector<node> nodes;
    std::vector<cluster> clusters;
public:
    void set_ants(int nr);
    void set_iterations(int nr);
    void set_threshold(float nr);
    void set_dataset(std::string filename);
    void set_logfile(std::string filename);
    void set_nodes();
    void initialize_data();
    void display_nodes();
    void cluster_nodes();
    void set_clusters();
    void display_clusters();
    void set_pheromone();
    void display_pheromone();
    void locate_ants();
    void display_ant_status();
    void reset();
    int check_visited(ant ants);
    void get_possible_routes(std::vector<int> &av_nodes, int &nr_av_nodes, ant ants);
    void set_probabilities(std::vector<float> &prob, std::vector<int> av_nodes, int nr_av_nodes);
    int roulette_selection(std::vector<float> prob, int nr_av_nodes);
    void move_ant(ant &antl, int exit_node, int new_node );
    void update_pheromone();
    void update_partial_solution();
    void start_aco();
    void construct_final_solution(int nr_elem);
    void two_opt();
    float calculate_length();
    std::vector<int> generate_final_solution();
};

void run_aco::set_ants(int nr)
{
    nr_ants=nr;
}

void run_aco::set_iterations(int nr)
{
    iterations=nr;
}

void run_aco::set_threshold(float nr)
{
    threshold=nr;
}

void run_aco::set_dataset(std::string filename)
{
    dataset=filename;
}

void run_aco::set_logfile(std::string filename)
{
    logfile=filename;
}

void run_aco::set_nodes()
{
    std::fstream nodes_file(dataset.c_str(), std::ios_base::in); //D:
    cout<<">> Reading the file with coordinates:\n";
    float a,b,c;
    nr_nodes=1;
    
    cout<<"\nGetting nodes coordinates from file!\n";
    while (nodes_file >> c >> a >> b)
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

void run_aco::display_clusters()
{
    cout<<"\n>> Display clusters:\n";
    cout<<"There are "<< nr_clusters<<" clusters:\n";
    for(int i=1; i<=nr_clusters; i++ , cout<<endl)
    {
        cout<<"cluster "<<i<<" has the following nodes ("<<clusters[i].get_nr_nodes()<<")\n";
        for(int j=1;j<=clusters[i].get_nr_nodes();j++)
            cout<<clusters[i].node_list[j].get_id()<<",";
    }
    cout<<endl;
}

void run_aco::set_pheromone()
{
    cout<<"\n>> Set pheromone levels.\n";
    for (int a=1 ; a<=nr_nodes; a++)
        for (int b=1; b<=nr_nodes; b++)
            if (a!=b)
                pheromone[a][b]=0.5;
}

void run_aco::display_pheromone()
{
    int i,j;
    for(i=1;i<=nr_nodes;i++,cout<<endl)
        for(j=1;j<=nr_nodes;j++)
        {
            cout<<pheromone[i][j]<<" ";
        }
}

void run_aco::locate_ants()
{
    cout<<">>> Positioning ants\n";
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
    int i,j;
    for(i=1;i<=nr_ants;i++)
        {
            cout<<"\n\nAnt number: "<<ants[i].get_id()<<endl;
            cout<<"Node id: "<<ants[i].get_postion().get_id()<<endl;
            cout<<"Ant coodinates: "<<ants[i].get_postion().get_x()<<"-"<<ants[i].get_postion().get_y()<<endl;
            cout<<"Number_of_elements: "<<ants[i].get_sol_nr()<<endl;
            
            cout<<"Ant solution vector: ";
            for(int j=1;j<=nr_nodes;j++)
                cout<<ants[i].solution[j]<<"->";
            cout<<"\nAnt visited vector: ";
            for(int j=1;j<=nr_nodes;j++)
                cout<<ants[i].visited[j]<<"->";
        }
    cout<<endl;
}

void run_aco::reset()
{
    cout<<">>> (Re)Setting variables\n";
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
    int i,j;
    float sum;
    prob.resize(nr_av_nodes/2+1);
    
    for(j=1;j<=nr_av_nodes/2;j++)
    {
        sum+=pow( 1/nodes[av_nodes[j*2]].calc_dist(nodes[av_nodes[j*2-1]]), 2  )  *  pow(pheromone[av_nodes[j*2]][av_nodes[j*2-1]],2);

    }
    
    for(i=1;i<=nr_av_nodes/2;i++)
    {
        prob[i]= pow( 1/nodes[av_nodes[i*2]].calc_dist(nodes[av_nodes[i*2-1]]), 2  )  *  pow(pheromone[av_nodes[i*2]][av_nodes[i*2-1]],2)/sum;
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
    //cout<<"next node >>>>>> "<<new_node<<endl;
    if(new_node !=0 )
    {
        //cout<<"> next node is "<<new_node<<": "<<nodes[new_node].x<<","<<nodes[new_node].y<<endl;
        antl.update_sol_nr(1);
        antl.solution.resize(antl.get_sol_nr()+1);
        antl.solution[antl.get_sol_nr()]=new_node;
        //cout<<"calculate distance between"<<exit_node<<"-"<<new_node<<", and add it to solution length"<<endl;
        antl.update_road_length(nodes[new_node].calc_dist(nodes[exit_node]));
        antl.set_position(nodes[new_node]);
        antl.visited[new_node]=true;
        for (int i=1 ;i<=nr_nodes;i++)
            if(antl.get_postion().get_c() == nodes[i].get_c())
                antl.visited[i]=true;
    }
}

void run_aco::update_pheromone()
{
    int i,j,k;
    node n1,n2,copy;
    float dist, evaporation=0.1;
    cout<<">> Update pheromone.\n";
    for(i=1;i<=nr_ants;i++)
        for(j=1;j<ants[i].get_sol_nr();j++)
        {
            n1=nodes[ants[i].solution[j]];
            n2=nodes[ants[i].solution[j+1]];
            //cout<<n1.id<<"->"<<n2.id<<endl;
            dist=std::numeric_limits<float>::max();
            for(k=1;k<=nr_nodes;k++)
            {
                if( nodes[k].calc_dist(n2) < dist && n1.get_c() != n2.get_c() && n1.get_c() == nodes[k].get_c())
                {
                    dist=nodes[k].calc_dist(n2);
                    copy=nodes[k];
                }
            }
            pheromone[copy.get_id()][n2.get_id()]+=1/dist;
            pheromone[n2.get_id()][copy.get_id()]+=1/dist;
        }
        //pheromone evaporation
       for(i=1;i<=nr_nodes;i++)
           for(j=1;j<=nr_nodes;j++)
               if(pheromone[i][j]-evaporation>=0)
                   pheromone[i][j]-=evaporation;
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

void run_aco::start_aco()
{
    cout<<"\n>> Run ACO:\n ";
    int x,y,tmp,nr_of_avnodes=0;
    psol_length=std::numeric_limits<float>::max();
    std::vector<float> probabilities;
    std::vector<int> available_nodes;
    
    for(int i=1 ; i<=iterations; i++)
    {
        cout<<">>> Starting iteration "<<i<<endl;
        reset(); 
        locate_ants();
        cout<<">>> Generate solution for each ant\n";
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
                move_ant(ants[j],available_nodes[tmp-1],available_nodes[tmp]);
            } 
            //move ant to initial cluster
            move_ant(ants[j],ants[j].solution[ants[j].get_sol_nr()],ants[j].solution[1]);
            
        }// end of ants loop
        cout<<"After iteration "<<i<<"(total travelled distance between clusters)"<<endl;
        update_partial_solution();
        for(y=1;y<=nr_ants;y++)
        {
            cout<<"Ant "<<y<< " final solution ("<<ants[y].get_road_length()<<"): ";
            for(x=1;x<ants[y].get_sol_nr();x++)
                cout<<ants[y].solution[x]<<" -> ";
            cout<<ants[y].solution[ants[y].get_sol_nr()]<<endl;
        }
        update_pheromone(); 
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

//further optimization of the local tours of each cluster
void run_aco::two_opt()
{
    float min_dist=std::numeric_limits<float>::max();; //shortest road length
    bool check=true; // boolean value which will be update after each 2opt iteration
                     // it will turn to false if the are aren't any other solutions
    int s_pos,e_pos,cluster=nodes[final_solution[1]].get_c(), i,j; /* local tour starting (s_pos) and ending position(e_pos) */
            
    while(check)
    {
        for(i=2;i<nr_nodes;i++)
        {
            if(nodes[i].get_c() == cluster)
                for(j=i+2;;j++)
                {
                    
                }
        }
    }
}

float run_aco::calculate_length()
{
    float total=0;
    for(int i=2;i<=nr_nodes+1;i++)
            total+=nodes[final_solution[i]].calc_dist(nodes[final_solution[i-1]]);
    return total;
}

std::vector<int> run_aco::generate_final_solution()
{
    cout<<"\n    Starting Program\n";
    set_nodes();
    initialize_data();
    display_nodes();
    cluster_nodes();
    set_clusters();
    display_clusters();
    set_pheromone();
    start_aco(); 
    cout<<"\n\n Partial solution (made of clusters) found with given parameters is ("<<psol_length<<"):";
    for(int i=1;i<=nr_clusters+1;i++)
        cout<<nodes[partial_solution[i]].get_c()<<"("<<partial_solution[i]<<")"<<"->";
    final_solution.resize(nr_nodes+1);
    construct_final_solution(nr_clusters+1);
    
    cout<<"\nTotal length: "<<calculate_length()<<endl;
    cout<<"Final Solution: ";
    for(int i=1;i<=nr_nodes;i++)
        cout<<final_solution[i]<<"->";
    cout<<final_solution[nr_nodes+1]<<endl;
    return final_solution;
}

#endif /* RUN_ACO_H */

