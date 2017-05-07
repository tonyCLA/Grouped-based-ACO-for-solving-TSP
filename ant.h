/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ant.h
 * Author: clc18
 *
 * Created on 27 April 2017, 14:46
 */

#ifndef ANT_H
#define ANT_H

class ant
{
    node position;
    int id, sol_nr; 
    float tour_length; // total distance of the solution  
    
public:
    std::vector<int> solution;  // ant solution after each iteration; represented as a list of nodes; each node represent its cluster
    std::vector<bool> visited; // remember the nodes a particular ant visited
    
    void set_id(int ant_id);
    int get_id();
    void set_position(node ant_pos);
    node get_postion();
    void set_sol_nr(int ant_sol_nr);
    int get_sol_nr();
    void update_sol_nr(int add);
    void set_tour_length(float length);
    float get_tour_length();
    void update_tour_length(float add);
};

void ant::set_id(int ant_id)
{
    id=ant_id;
}

int ant::get_id()
{
    return id;
}

void ant::set_position(node ant_pos)
{
    position=ant_pos;
}

node ant::get_postion()
{
    return position;
}

void ant::set_sol_nr(int ant_sol_nr)
{
    sol_nr=ant_sol_nr;
}

int ant::get_sol_nr()
{
    return sol_nr;
}

void ant::update_sol_nr(int add)
{
    sol_nr+=add;
}

void ant::set_tour_length(float length)
{
    tour_length=length;
}

float ant::get_tour_length()
{
    return tour_length;
}

void ant::update_tour_length(float add)
{
    tour_length+=add;
}

#endif /* ANT_H */

