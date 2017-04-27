/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   node.h
 * Author: clc18
 *
 * Created on 27 April 2017, 10:52
 */

#ifndef NODE_H
#define NODE_H

class node{
    int c, id; //cluster number, node id
    float x,y; //node coordinates(real numbers)
public:
    void set_coord(float cx, float cy);
    void setid(int id);
    void set_cluster(int cluster);
    int get_id();
    float get_x();
    float get_y();
    int get_c();
    float calc_dist(node end_node);
};

void node::set_coord(float cx, float cy)
{
    x=cx;
    y=cy;
}

void node::setid(int nid)
{
    id=nid;
}

void node::set_cluster(int cluster)
{
    c=cluster;
}

int node::get_id()
{
   return id; 
}

int node::get_c()
{
    return c;
}

float node::get_x()
{
    return x;
}

float node::get_y()
{
    return y;
}

float node::calc_dist(node end_node)
{
    return sqrt( pow((x-end_node.x),2) + pow((y-end_node.y),2));;
}

#endif /* NODE_H */

