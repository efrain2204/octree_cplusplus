#ifndef OCT_QUANTIZER_H_INCLUDED
#define OCT_QUANTIZER_H_INCLUDED


#include"octreequantizer.h"
#include"color.h"
#include"octree.h"


    OctreeNode::OctreeNode(int level,OctreeQuantizer * parent){
        color = Color(0, 0, 0);
        pixel_count = 0;
        palette_index = 0;
        for(int i = 0 ; i<8 ;i++){
            children[i]=nullptr;
        }
        if (level < max_depth - 1)
            parent->add_level_node(level,this);

    }
    bool OctreeNode::is_leaf(){
        return pixel_count > 0;
    }
    vector<OctreeNode *> OctreeNode::get_leaf_nodes(){
        vector<OctreeNode *> leaf_nodes;
        for(int i = 0 ; i < 8; i++){
            OctreeNode *node = children[i];
            if(node){
                if (node->is_leaf())
                    leaf_nodes.push_back(node);
                /*else
                    leaf_nodes.push_back(node->get_leaf_nodes());*/
            }
        }
        return leaf_nodes;
}
    int OctreeNode::get_nodes_pixel_count(){
        int sum_count = pixel_count;
        for(int i = 0; i<8 ; i++){
            OctreeNode * node = children[i];
            if(node){
                sum_count += node->pixel_count;
            }
        }
        return sum_count;
    }
    void OctreeNode::add_color( Color color,int  level, OctreeQuantizer *parent){
        if(level >= parent->get_max_depth()){
            color.red += color.red;
            color.green += color.green;
            color.blue += color.blue;
            pixel_count += 1;
            return;
        }
        int index = get_color_index_for_level(color, level);
        if(!children[index]){
            children[index] = new OctreeNode(level, parent);
        }
        children[index]->add_color(color, level + 1, parent);
    }
    int OctreeNode::get_palette_index(Color color, int level){
        if (is_leaf())
            return palette_index;
        int index = get_color_index_for_level(color, level);
        if(children[index]){
            return children[index]->get_palette_index(color, level + 1);
        }else{
            for(int i = 0; i<8 ;i++){
                if (children[i])
                    return children[i]->get_palette_index(color, level + 1);
            }
        }
    }
    int OctreeNode::remove_leaves(){
        int result = 0;
        for(int i = 0 ; i<8 ; i++){
            OctreeNode *node = children[i];
            if (node){
                color.red += node->color.red;
                color.green += node->color.green;
                color.blue += node->color.blue;
                pixel_count += node->pixel_count;
                result += 1;
            }
        }
        return result-1;
    }
    int  OctreeNode::get_color_index_for_level(Color color, int level){
        int index = 0;
        int mask = 0x80 >> level;
        if (color.red & mask)
            index |= 4;
        if (color.green & mask)
            index |= 2;
        if (color.blue & mask)
            index |= 1;
        return index;
    }
    Color OctreeNode::get_color(){
        return Color(     color.red / pixel_count,
            color.green / pixel_count,
            color.blue / pixel_count);
    }

/*Quantizer*/

    OctreeQuantizer::OctreeQuantizer(){
        for(int i=0; i<8; i++){
            levels[i].reserve(8);
            for(int j = 0 ; j<8; j++){
                levels[i][j]=nullptr;
            }
        }
        root= new OctreeNode(0,this);
    }
    /*
    void OctreeQuantizer::get_leaves(){
        return [node for node in root.get_leaf_nodes()];
    }*/
    void OctreeQuantizer::add_level_node(int level,OctreeNode *node){
        levels[level].push_back(node);
    }
    int OctreeQuantizer::get_max_depth(){
        return max_depth;
    }
    void OctreeQuantizer::add_color(Color color){
        root->add_color(color, 0, this);
    }
    /*
    vector<OctreeNode> OctreeQuantizer::make_palette(Color color_count){
        vector<OctreeNode>palette;
        int palette_index = 0;
        int leaf_count = len(get_leaves());

        for (level in xrange(OctreeQuantizer.MAX_DEPTH - 1, -1, -1)){
            if (levels[level]):
                for (node in levels[level]){
                    leaf_count -= node.remove_leaves();
                    if(leaf_count <= color_count)
                        break;
                }
                if(leaf_count <= color_count)
                    break;
                levels[level] = [];
        }
        for(node in get_leaves()){
            if(palette_index >= color_count)
                break;
            if(node.is_leaf())
                palette.push_back(node.get_color());
            node.palette_index = palette_index;
            palette_index += 1;
        }
        return palette;
    }
    void OctreeQuantizer::get_palette_index(Color color){
        return root->get_palette_index(color, 0);
    }*/

#endif // OCT_QUANTIZER_H_INCLUDED
