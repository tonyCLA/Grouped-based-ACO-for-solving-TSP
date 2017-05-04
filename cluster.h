/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   cluster.h
 * Author: clc18
 *
 * Created on 27 April 2017, 10:52
 */

#ifndef CLUSTER_H
#define CLUSTER_H

#include "node.h"


class cluster
{
    int nr_nodes, id;
    node centroid;
    
public:
    std::vector<node> node_list;
    
    void set_id(int cid);
    int get_id(); 
    void set_nr_nodes(int nr);
    int get_nr_nodes();
    void calculate_centroid();
    node get_centroid();
};


void cluster::set_id(int cid)
{
    id=cid;
}

int cluster::get_id()
{
    return id;
}

void cluster::set_nr_nodes(int nr)
{
    nr_nodes=nr;
}

int cluster::get_nr_nodes()
{
    return nr_nodes;
}

node cluster::get_centroid()
{
    return centroid;
}

void cluster::calculate_centroid()
{
    float sum_x=0, sum_y=0;
    for(int i=1;i<=nr_nodes;i++)
    {
        sum_x+=node_list[i].get_x();
        sum_y+=node_list[i].get_y();
    }
    
    centroid.set_coord(sum_x/nr_nodes, sum_y/nr_nodes);
    centroid.setid(-1*id); 
}

#endif /* CLUSTER_H */

