//
// Created by Christoph Greger on 16.04.25.
//

#ifndef GENG_H
#define GENG_H

#include <vector>
#include "Graph.h"



namespace Geng {

    struct Graph_Matrix {
        int numVertices_max;
        int minEdges;
        int maxEdges;
        int numGraphs;
        vector<Graph> graphs; //Sorted for the matrix
        long long * matrix; //Needs to be at least numGraphs * numGraphs
    };



    std::vector<Graph> generateGraphs(int n, int minEdges, int maxEdges);
    std::vector<Graph> generateGraphs_without_isolated(int n, int minEdges, int maxEdges);
    void write_Components_Combinations_to_File(const std::string &filename, int n, int minEdges, int maxEdges);
    void storeMatrix_Hom(const std::string &filename, const std::string &combination_filename, int n, int minEdges, int maxEdges);
    Graph_Matrix readMatrix(const std::string &filename);
    void storeMatrix(const std::string &filename, const Graph_Matrix &hom_matrix);
    void storeMatrix_Surj(const std::string &filename, const std::string &hom_matrix);
}



#endif //GENG_H
