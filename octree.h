#ifndef OCTREE_H_INCLUDED
#define OCTREE_H_INCLUDED

#include<iostream>
#include<vector>
using namespace std;

int max_depth=8;

class OctreeNode{
    vector<OctreeNode*>children;
    Color color;
    int pixel_count,palette_index;
public:
    OctreeNode(int level,OctreeQuantizer * parent);
    bool is_leaf();
    vector<OctreeNode*> get_leaf_nodes();
    int get_nodes_pixel_count();
    void add_color( Color color,int  level, OctreeQuantizer *parent);
    int get_palette_index(Color color, int level);
    int remove_leaves();
    int get_color_index_for_level(Color color, int level);
    Color get_color();
};



#endif // OCTREE_H_INCLUDED
