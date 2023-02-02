#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <omp.h>
using namespace std;

struct Graph{size_t n, m, *adj, *xadj;};

/* 
This function is unoptimized in this state! There is no input checking!
Even though most partitioners spends significant time on parsing the input, 
fast file parsing is another field
*/
Graph read_graph(string filename){
    ifstream in(filename);
    Graph g;
    string mod,line;
    do{
        getline(in,line); 
    } while(line.length()>0 && line[0]=='%'); // skip comments

    istringstream iss(line);
    iss >> g.n >> g.m >> mod;
    g.m *= 2; // Metis format contains backwards edges but doesn't count them -_- 
    if (!(mod=="" || mod=="0")) {
        cerr<< "Weights are not supported yet!\n"; 
        exit(-1);
    }
    g.xadj = new size_t[g.n+2]; 
    // we have headstone g.xadj[0]==0,g.xadj[1]==0   
    // vertices start from 1 for compatibility
    g.adj = new size_t[g.m];
    size_t edge_pos=0, source = 1, target;
    while(getline(in,line)){
        if(line.length()>0 && line[0]=='%') // skip comments
            continue; 
        for(istringstream iss(line); iss>> target; edge_pos++){
            g.adj[edge_pos] = target;            
        }
        g.xadj[++source] = edge_pos;
    }
    return g;
}
int main(int argc, char *argv[]){
    double time;
    if (argc!=4) {
        cout << "Usage:"<< argv[0] <<" <metis file path> <number of partitions> <allowed imbalance ratio>\n";
        return -1;
    }
    string infile = argv[1];
    int parts = atoi(argv[2]);
    float ufactor = atof(argv[3]);
    cerr << "Reading graph...\n";
    time = omp_get_wtime();
    Graph g = read_graph(infile);
    cerr << "Took "<< omp_get_wtime()-time << " seconds\n";

}