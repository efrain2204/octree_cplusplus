#include <bits/stdc++.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

vector<int> binario(int A){
    vector<int> bin;
    while(A>1){
        bin.insert(bin.begin(),A%2);
        A=A-A%2;
        A=A/2;
    }
    bin.insert(bin.begin(),A);
    int i=bin.size();
    while(i<8){
        bin.insert(bin.begin(),0);
        i++;
    }
    return bin;
}

vector<int> posicion(Vec3b current){
    vector<int> bin1,bin2,bin3,bin;
    bin1=binario(current[0]);
    bin2=binario(current[1]);
    bin3=binario(current[2]);
    for(int i=0;i<bin1.size();i++)
        bin.push_back(bin1[i]*4+bin2[i]*2+bin3[i]*1);
    return bin;
}


struct OctreNode{
    int red = 0;
    int green = 0;
    int blue = 0;
    int cont;
    OctreNode* parent=nullptr;
    vector<OctreNode*> children;
};

class OctreeQuantizer{
    OctreNode * root;
    int nivel;
    Mat Paleta;
    vector<OctreNode*> paletacolores;
public:
    OctreeQuantizer(){
        root=nullptr;
        nivel=8;
    }
    void ingreso(Vec3b current){
        vector<int> bin=posicion(current);
        if(root==nullptr){
            OctreNode *nuevo=new OctreNode();
            root=nuevo;
        }
        OctreNode* temp=root;
        for(int i=0;i<bin.size();i++){
            temp->red+=current[0];
            temp->green+=current[1];
            temp->blue+=current[2];
            temp->cont+=1;
            if(temp->children.size()==0){
                vector<OctreNode*> g;
                for(int j=0;j<8;j++){
                    OctreNode* nuev=new OctreNode();
                    nuev->parent=temp;
                    g.push_back(nuev);
                }
                temp->children=g;
            }
            temp=temp->children[bin[i]];
        }
        temp->red+=current[0];
        temp->green+=current[1];
        temp->blue+=current[2];
        temp->cont+=1;
    }
    OctreNode* captura(Vec3b current){
        vector<int> bin=posicion(current);
        OctreNode * temp=root;
        for(int i=0;i<nivel;i++){
            temp=temp->children[bin[i]];
        }
        return temp;
    }
    void captar_paleta(){
        captar(1,root);
    }
    void captar(int conti, OctreNode* temp){
        if(temp!=nullptr){
            if(conti<nivel){
                int i=0;
                while(i<temp->children.size()){
                    captar(conti+1,temp->children[i]);
                    i++;
                }
            }
            else{
                int i=0;
                while(i<temp->children.size()){
                    if(temp->children[i]->cont>0){
                        paletacolores.push_back(temp->children[i]);
                    }
                    i++;
                }
            }
        }
    }
    void make_paleta(){
        int f=paletacolores.size();
        cout<<f;
        int tam=int(sqrt(f))+1;
        Mat palett(tam*10,tam*10,CV_8UC3);
        for(int i=0;i<f;i++){
            int x=i/tam;
            int y=i%tam;
            Vec3b pixa={
                paletacolores[i]->red/paletacolores[i]->cont,
                paletacolores[i]->green/paletacolores[i]->cont,
                paletacolores[i]->blue/paletacolores[i]->cont};
            for(int q=0;q<10;q++){
                for(int w=0;w<10;w++){
                    palett.at<Vec3b>(10*x+q,10*y+w)=pixa;
                }
            }
        }
        Paleta=palett;
    }
    void reducir(){
        nivel--;
    }
    Mat paletas(){
        return Paleta;
    }
};
int main(){
    Mat image;
    image = imread("test2.png");
    Mat imagen_salida(image.rows,image.cols,CV_8UC3);
    OctreeQuantizer img;
    for(int i=0;i<image.rows;i++){
        for(int j=0;j<image.cols;j++){
            Vec3b pixels = image.at<Vec3b>(i, j);
            img.ingreso(pixels);
        }
    }
    img.reducir();
    img.reducir();
    img.reducir();
    img.reducir();
    img.reducir();
    img.reducir();

    for(int i=0;i<image.rows;i++){
        for(int j=0;j<image.cols;j++){
            Vec3b pixeles = image.at<Vec3b>(i, j);
            OctreNode *f=img.captura(pixeles);
            Vec3b current={(f->red)/(f->cont),(f->green)/(f->cont),(f->blue)/(f->cont)};
            imagen_salida.at<Vec3b>(i,j)=current;
        }
    }
    /*Obtener paleta*/
    img.captar_paleta();
    img.make_paleta();
    Mat ppp=img.paletas();
    namedWindow( "window", WINDOW_AUTOSIZE );
    imshow( "window", imagen_salida );
    imwrite("imagen_r2.jpg",imagen_salida);

    imshow( "window", ppp );
    imwrite("paleta2.jpg",ppp);
    waitKey(0);
    return 0;
}
