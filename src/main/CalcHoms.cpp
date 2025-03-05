//
// Created by falco on 02.03.25.
//

#include "../../include/CalcHoms.h"
#include "Injective_Hom_Count_By_Spasm.h"

#include <bitset>
#include <iostream>
#include <linear_Equations_F2_small.h>

#include "NextInjection.h"

//calc exponent of number of homs from H to CFI Graph of S based on a mapping
//every node in H is mapped to one in S (predecided through mapping disregarding the color)
//the exact number of homs can be calculated when applying 2^
//also note that when there are now homs -1 is returned
int CalcHoms::calcNumHomsCFI(const Graph& H, const Graph& S, const int* mapping) {

    const auto& neighborsS = S.neighbours;
    const auto& degS = S.degree;

    //We want to create an possible mapping from (vertice, neighbor) pairs to an index in the matrix (column)
    //this vector mapps every node in this to <beginning, end>, which is the range of the neighbors in S
    //and so the range of the columns in the matrix which this node in affiliated with
    //(from inclusive, to exclusive)
    auto indexMapping = vector<pair<int, int>>();
    indexMapping.reserve(H.numVertices);

    int columns = 0;
    for (int i = 0; i < H.numVertices; ++i) {
        indexMapping.emplace_back(columns, columns + degS[mapping[i]]);//H.nodes[i].color?? but why
        columns += degS[mapping[i]];
    }

    if (columns >= 128) {
        throw runtime_error("to many variables for the CFI homs solver");
    }

    //Now columns is the number of columns in the matrix

    //H.numVertices -> even subset guarantee for the mapped node
    //H.edges.size() -> same opinion on the mapped edge
    const int rows = H.numVertices + static_cast<int>(H.edges.size());

    //Now we create the matrix and initialize with 0
    auto matrix = vector<bitset<128>>(rows);

    //Fill with the even subset guarantee
    for (int i = 0; i < H.numVertices; i++) {
        for(int j = indexMapping[i].first; j < indexMapping[i].second; j++) {
            matrix[i][j] = 1;
        }
    }

    int currentRow = H.numVertices;

    //Same opinion on the mapped edge
    for (const auto [first,second] : H.edges) {
        //We have to find the index that the two nodes are affiliated with
        int firstIndex = -1;
        int secondIndex = -1;

        const int mFirst = mapping[first];
        const int mSecond = mapping[second];

        //Find firstIndex
        for (int i = 0; i < degS[mFirst]; ++i) {
            if (neighborsS[mFirst][i] == mSecond) {
                firstIndex = i;
                break;
            }
        }
        //Find secondIndex
        for (int i = 0; i < degS[mSecond]; ++i) {
            if (neighborsS[mSecond][i] == mFirst) {
                secondIndex = i;
                break;
            }
        }

        //mapping is invalid when edge is not found
        if (firstIndex == -1 || secondIndex == -1) {
            return -1;
        }

        matrix[currentRow][indexMapping[first].first + firstIndex] = 1;
        matrix[currentRow][indexMapping[second].first + secondIndex] = 1;
        ++currentRow;
    }

    //Now we can calculate the dimension of the solution space
    const int dimension = solution_space_dimension_f2_small_homogen(matrix,columns);

    return dimension;
}

//returns the number of homs from H to CFI Graph of S, (by trying every possible mapping) (works only for uncolored)
//Be sure that H has <= 9 vertices and S has maxdegree <= 4
long long CalcHoms::calcNumHomsCFI_uncolored(Graph &H, const Graph &S) {

    long long total = 0;

    //generate all homs and for each hom count the number of cfi homs from H to CFI(S)

    const int *nodeIndex = H.nodeIndex;

    int* hom = new int[H.numVertices];
    for (int i = 0; i < H.numVertices; i++) {
        hom[i] = 0;
    }

    int currtochange = 1;
    bool increment = false;


    while (true) {

        if (currtochange == -1) {
            break; //All homomorphisms have been checked
        }

        if (increment) {
            if (hom[currtochange] == S.numVertices - 1) {
                increment = true;
                --currtochange;
                continue;
            }
            if (currtochange == 0) {
                ++hom[currtochange];
                increment = false;
                ++currtochange;
                continue;
            }
            bool foundani = false;
            for (int i = hom[currtochange] + 1; i < S.numVertices; i++) {
                bool iworks = true;
                for (int edgeindex = nodeIndex[currtochange-1]; edgeindex < nodeIndex[currtochange]; edgeindex++) {
                    if (!S.isEdge(hom[H.edgeArray[edgeindex].first], i)) {
                        iworks = false;
                        break;
                    }
                }
                if (iworks) {
                    foundani = true;
                    hom[currtochange] = i;
                    increment = false;
                    ++currtochange;
                    break;
                }
            }
            if (!foundani) {
                increment = true;
                --currtochange;
            } else {
                continue;
            }
        }


        //If going right in the hom array
        if (!increment) {
            bool foundani = false;
            for (int i = 0; i < S.numVertices; i++) {
                bool iworks = true;
                for (int edgeindex = nodeIndex[currtochange-1]; edgeindex < nodeIndex[currtochange]; edgeindex++) {

                    if (!S.isEdge(hom[H.edgeArray[edgeindex].first], i)) {
                        iworks = false;
                        break;
                    }
                }
                if (iworks) {
                    foundani = true;
                    hom[currtochange] = i;
                    if (currtochange == H.numVertices - 1) {


                        //ONLY REAL DIFFERENCE TO COUNTING HOMS

                        //Handling the found Hom!
                        total += powBase2(calcNumHomsCFI(H, S, hom));

                        //END ONLY REAL DIFFERENCE TO COUNTING HOMS

                    } else {
                        break;
                    }
                }
            }
            if (!foundani || currtochange == H.numVertices - 1) {
                increment = true;
                --currtochange;
                continue;
            }
            ++currtochange;
            continue;
        }

    }
    delete[] hom;

    return total;
}


//returns the number of homomorphisms from  to the CFI graph of S
long long CalcHoms::calcNumInjectiveHomsCFI(const std::string &small_spasm_file_name, Graph &S) {

    //Now we have to get the small_spasm of the small_spasm_file_name
    auto smallspasm = getFromFile_spasm_smaller(small_spasm_file_name);

    //Now we have to calculate the number of homomorphisms for every component
    std::unordered_map<std::string, long long> componentMap;

    for (const auto &comp : smallspasm.components) {
        std::string canon = comp.canonicalStr;
        Graph graph = comp.graph;
        componentMap.emplace(canon, calcNumHomsCFI_uncolored(graph, S));
    }

    long long total = 0;

    for (const auto &fullGraph : smallspasm.fullGraphs) {
        long long factor = fullGraph.factor;
        for (const auto &comp : fullGraph.components) {
            std::string canon = comp.first.canonicalStr;
            const int exponent = comp.second;
            factor *= powlong(componentMap[canon], exponent);
        }
        total += factor;
    }

    return total;
}

/*
 * Normal Graph
 */

//calc homs from H to G
//works for colored and uncolored
long long CalcHoms::calcNumHoms(const Graph& H, const Graph& G) {
    const int *nodeIndex = H.nodeIndex;
    bool coloredhoms = H.colored && G.colored;
    long long numHomomorphisms = 0;

    //Initialize the homomorphism array with only 0s
    int* hom = new int[H.numVertices];
    for (int i = 0; i < H.numVertices; i++) {
        hom[i] = 0;
    }

    int currtochange = 1;
    bool increment = false; //If the current node has to be incremented, only false at the first iteration; increment = false means going right in the hom array

    if (!coloredhoms) {
        while (true) {

            if (currtochange == -1) {
                break; //All homomorphisms have been checked
            }

            if (increment) {
                if (hom[currtochange] == G.numVertices - 1) {
                    increment = true;
                    --currtochange;
                    continue;
                }
                if (currtochange == 0) {
                    ++hom[currtochange];
                    increment = false;
                    ++currtochange;
                    continue;
                }
                bool foundani = false;
                for (int i = hom[currtochange] + 1; i < G.numVertices; i++) {
                    bool iworks = true;
                    for (int edgeindex = nodeIndex[currtochange-1]; edgeindex < nodeIndex[currtochange]; edgeindex++) {
                        if (!G.isEdge(hom[H.edgeArray[edgeindex].first], i)) {
                            iworks = false;
                            break;
                        }
                    }
                    if (iworks) {
                        foundani = true;
                        hom[currtochange] = i;
                        increment = false;
                        ++currtochange;
                        break;
                    }
                }
                if (!foundani) {
                    increment = true;
                    --currtochange;
                } else {
                    continue;
                }
            }

            //If going right in the hom array
            if (!increment) {
                bool foundani = false;
                for (int i = 0; i < G.numVertices; i++) {
                    bool iworks = true;
                    for (int edgeindex = nodeIndex[currtochange-1]; edgeindex < nodeIndex[currtochange]; edgeindex++) {
                        if (!G.isEdge(hom[H.edgeArray[edgeindex].first], i)) {
                            iworks = false;
                            break;
                        }
                    }
                    if (iworks) {
                        foundani = true;
                        hom[currtochange] = i;
                        if (currtochange == H.numVertices - 1) {
                            ++numHomomorphisms;
                        } else {
                            break;
                        }
                    }
                }
                if (!foundani || currtochange == H.numVertices - 1) {
                    increment = true;
                    --currtochange;
                    continue;
                }
                ++currtochange;
                continue;
            }

        }
        delete[] hom;
        return numHomomorphisms;
        // ---------- COLORED BRANCH (MODIFIED) ---------- AI GENERATED (some parts)
    } else {
        // Precompute valid mappings based on color
        // possibleMappings[v].first is an array of valid node-IDs in G
        // possibleMappings[v].second is how many valid node-IDs in that array
        pair<int*, int> * possibleMappings = new pair<int*, int>[H.numVertices];
        for (int v = 0; v < H.numVertices; v++) {
            vector<int> valid;
            for (int w = 0; w < G.numVertices; w++) {
                // If color matches, node w in G is a valid image of v
                if (G.nodes[w].equals(H.nodes[v])) {
                    valid.push_back(w);
                }
            }
            // Store them in the pair
            int *arr = new int[valid.size()];
            for (int i = 0; i < (int)valid.size(); i++) {
                arr[i] = valid[i];
            }
            possibleMappings[v] = make_pair(arr, (int)valid.size());
        }

        // candidateIndex[v] is the index in the array possibleMappings[v].first
        int* candidateIndex = new int[H.numVertices];

        // Initialize
        for (int v = 0; v < H.numVertices; v++) {
            hom[v] = -1;            // no candidate chosen
            candidateIndex[v] = 0;  // start with 0
        }

        currtochange = 0;

        // Start the backtracking:
        while (true) {

            if (currtochange == -1) {
                break; // done
            }

            if (increment) {
                // We have to move to the next candidate for hom[currtochange]
                bool foundNext = false;

                // Iterate over valid color-candidates
                auto &pm = possibleMappings[currtochange];
                for (int idx = candidateIndex[currtochange] + 1; idx < pm.second; idx++) {
                    int candidate = pm.first[idx];
                    // Check adjacency constraints
                    bool works = true;

                    if (currtochange == 0) {
                        hom[currtochange] = candidate;
                        candidateIndex[currtochange] = idx;
                        foundNext = true;
                        increment = false;
                        ++currtochange;
                        break;
                    }

                    for (int edgeindex = nodeIndex[currtochange - 1];
                         edgeindex < nodeIndex[currtochange];
                         edgeindex++)
                    {
                        if (!G.isEdge(hom[H.edgeArray[edgeindex].first], candidate)) {
                            works = false;
                            break;
                        }
                    }
                    if (works) {
                        hom[currtochange] = candidate;
                        candidateIndex[currtochange] = idx;
                        foundNext = true;
                        increment = false;
                        ++currtochange;
                        break;
                    }
                }

                if (!foundNext) {
                    // Need to backtrack further
                    increment = true;
                    --currtochange;
                }
            }
            else {
                // "Go right" fill from scratch
                bool foundFirst = false;
                auto &pm = possibleMappings[currtochange];
                for (int idx = 0; idx < pm.second; idx++) {
                    int candidate = pm.first[idx];
                    bool works = true;
                    // We only need to check edges whose 'second' is currtochange
                    if (currtochange != 0) {
                        for (int edgeindex = nodeIndex[currtochange - 1];
                             edgeindex < nodeIndex[currtochange];
                             edgeindex++)
                        {
                            if (!G.isEdge(hom[H.edgeArray[edgeindex].first], candidate)) {
                                works = false;
                                break;
                            }
                        }
                    }
                    if (works) {
                        hom[currtochange] = candidate;
                        candidateIndex[currtochange] = idx;
                        if (currtochange == H.numVertices - 1) {
                            // We assigned the last vertex => valid homomorphism found
                            ++numHomomorphisms;
                            continue;
                        } else {
                            foundFirst = true;
                            break;
                        }
                    }
                }
                if (!foundFirst || currtochange == H.numVertices - 1) {
                    increment = true;
                    --currtochange;
                } else {
                    ++currtochange;
                }
            }
        } // end while

        // Clean up
        if (possibleMappings) {
            for (int v = 0; v < H.numVertices; v++) {
                delete[] possibleMappings[v].first;
            }
            delete[] possibleMappings;
        }
        delete[] hom;
        delete[] candidateIndex;
        return numHomomorphisms;
    }

}

//Calc injective homs from H to G
//works for colored and uncolored
long long CalcHoms::calcNumInjHoms(const Graph &H, const Graph &G) {

    if (G.numVertices < H.numVertices) {
        return 0;
    }

    long long numHomomorphisms = 0;
    auto nextInjection = NextInjection(H.numVertices, G.numVertices);
    const vector<int>& hom = nextInjection.current();



    bool alreadynexthom = true;
    bool isnohom = false;

    while (true) {

        isnohom = false;

        if (!alreadynexthom) {
            if (!nextInjection.next()) {
                break;
            }
        }

        alreadynexthom = false;

        if (H.colored && G.colored) {
            for (int i = 0; i < H.numVertices; i++) {
                if (!G.nodes[hom[i]].equals(H.nodes[i])) {
                    isnohom = true;
                    break;
                }
            }
            if (isnohom) {
                continue;
            }
        }

        for (int i = 0; i < H.edges.size(); i++) {
            if (!G.isEdge(hom[H.edgeArray[i].first], hom[H.edgeArray[i].second])) {
                isnohom = true;
                break;
            }
        }

        if (isnohom) {
            continue;
        }

        numHomomorphisms++;
    }
    return numHomomorphisms;
}

//not tested and not used
/*
long long Graph::calculateNumberofAutomorphismsWithoutColoring() {
    bool oldcolored = colored;
    colored = false;
    long long numAutomorphisms = calculateNumberofHomomorphismsTo(*this);
    colored = oldcolored;
    return numAutomorphisms;
}

long long Graph::calculateNumberofAutomorphismsWithColoring() {
    bool oldcolored = colored;
    colored = true;
    long long numAutomorphisms = calculateNumberofHomomorphismsTo(*this);
    colored = oldcolored;
    return numAutomorphisms;
}

long long Graph::calculateNumberofSubGraphsTo(Graph &H) {
    if (colored && H.colored) {
        return calculateNumberofInjectiveHomomorphismsTo(H) / calculateNumberofAutomorphismsWithColoring();
    }
    return calculateNumberofInjectiveHomomorphismsTo(H) / calculateNumberofAutomorphismsWithoutColoring();
}
*/