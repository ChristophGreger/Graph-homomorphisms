//
// Created by Christoph Greger on 24.12.24.
//

#include <iostream>

#include <Graph.h>
#include <CFIGraph.h>
#include <CFINode.h>

#include "BruteForceAlgo.h"
#include "RandomGraphGenerator.h"
#include "utilities.h"

int main() {
    std::cout << "Hello, World! :)" << std::endl;
    
    
    Graph G = Graph(true);
    Node node0 = Node(0);
    Node node1 = Node(1);
    Node node2 = Node(2);
    Node node3 = Node(3);
    Node node4 = Node(4);
    Node node5 = Node(5);
    Node node6 = Node(6);
    G.addNode(node0);
    G.addNode(node1);
    G.addNode(node2);
    G.addNode(node3);
    G.addNode(node4);
    G.addNode(node5);
    G.addNode(node6);
    G.addEdge(0, 1);
    G.addEdge(0, 3);
    G.addEdge(1, 3);
    G.addEdge(3, 2);
    G.addEdge(2, 1);
    G.addEdge(3,4);
    G.addEdge(2,4);
    G.addEdge(4,5);
    G.addEdge(0,6);
    G.addEdge(3,6);

    CFIGraph CFI = CFIGraph(G);

    for(const std::pair<int, int>& entry: CFI.edgeMap) {
        std::cout << entry.first << "->" << entry.second << std::endl;
    }
    

    //CFINode n1 = CFINode(0, {0,0,0,0,1});
    //CFINode n2 = CFINode(1, {0,0,0,0,0});
    //bool r1 = CFI.isEdge(n1,n2);

    //long long numHoms = calculateNumberofHomomorphisms(&CFI, &G);

    int vertices = 10;
    int edges = 30;
    RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(vertices, edges, true, true);
    Graph G = randomGraphGenerator.generateRandomConnectedGraph();
    CFIGraph CFI = CFIGraph(G);
    //Graph H = CFI.toGraph();
    //cout << "G: " << G.numVertices << " " << G.edges.size() << endl;
    //cout << "H: " << H.numVertices << " " << H.edges.size() << endl;
    unsigned long time = clock();
    long long numHoms = calculateNumberofHomomorphisms(&CFI, &G);
    long long expected = intPow(2, (edges - vertices + 1));
    cout << expected << endl;
    cout << "Time in ms: " << (clock() - time)/1000 << endl;
    cout << "Calculated successfully" << endl;

    std::cout << "numHoms: " << numHoms << std::endl;

    return 0;
}