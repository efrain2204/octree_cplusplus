#ifndef COLOR_H_INCLUDED
#define COLOR_H_INCLUDED

#include<iostream>
#include<vector>
using namespace std;

class Color{
public:
    int red,green,blue,alpha;
    Color(int red=0,int green=0,int blue=0,int alpha=1){
        red=red;
        green=green;
        blue=blue;
        alpha=alpha;
    }
};


#endif // COLOR_H_INCLUDED
