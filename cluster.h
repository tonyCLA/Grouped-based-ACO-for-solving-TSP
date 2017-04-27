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

class cluster
{
    int c_nr, id;
    node centroid;
    std::vector<node> node_list;
public:
    void set_id(int cid);
    int get_id(); 
    void set_c_nr(int nr);
    int get_c_nr();
};


void cluster::set_id(int cid)
{
    id=cid;
}

int cluster::get_id()
{
    return id;
}


#endif /* CLUSTER_H */

