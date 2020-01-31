#ifndef OCTREEQUANTIZER_H_INCLUDED
#define OCTREEQUANTIZER_H_INCLUDED

#include<iostream>
#include<vector>

using namespace std;
class OctreeNode;
class Color;

class OctreeQuantizer{
    vector<OctreeNode*> levels[8];
    OctreeNode* root;
public:
    OctreeQuantizer();
    //void get_leaves();
    void add_level_node(int level,OctreeNode *node);
    int get_max_depth();
    void add_color(Color color);
    //vector<OctreeNode> make_palette(Color color_count);
    //int get_palette_index(Color color);
};


#endif // OCTREEQUANTIZER_H_INCLUDED
