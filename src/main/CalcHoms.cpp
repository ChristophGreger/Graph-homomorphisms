//
// Created by falco on 02.03.25.
//

#include "../../include/CalcHoms.h"

#include <boost/pending/disjoint_sets.hpp>
#include <bitset>
#include <linear_Equations_F2_small.h>
#include "NextInjection.h"
#include "Spasm.h"
#include <boost/thread.hpp>
#include <boost/asio.hpp>

LinearSystemOfEquations generateCFI_LSOE_unoptimized(const Graph& H, const Graph& S, const int* mapping, const pair<int,int> &edge = {0,0}) {

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

    if (columns >= MAX_LSOE_BITSET - 1) {
        throw runtime_error("to many variables for the CFI homs solver");
    }

    //Now columns is the number of columns in the matrix

    //H.numVertices -> even subset guarantee for the mapped node
    //H.edges.size() -> same opinion on the mapped edge
    const int rows = H.numVertices + static_cast<int>(H.edges.size());

    //Now we create the matrix and initialize with 0
    auto matrix = vector<bitset<MAX_LSOE_BITSET>>(rows);

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
            throw runtime_error("mapping is invalid");
        }

        matrix[currentRow][indexMapping[first].first + firstIndex] = 1;
        matrix[currentRow][indexMapping[second].first + secondIndex] = 1;

        if ((edge.first == mFirst && edge.second == mSecond) || (edge.first == mSecond && edge.second == mFirst)) {
            matrix[currentRow][columns] = 1;
        }

        ++currentRow;
    }

    LinearSystemOfEquations result = {matrix, columns};
    return result;
}

LinearSystemOfEquations generateCFI_LSOE(const Graph& H, const Graph& S, const int* mapping, const int vertice) {

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

    //Now columns is the number of columns in the matrix

    //UNION FIND INIT
    vector<int> parent(columns);
    vector<int> rank(columns);

    boost::disjoint_sets<int*,int*> ds(&rank[0], &parent[0]);

    for (int i = 0; i < columns; ++i) {
        ds.make_set(i);
    }

    //UNION FIND INIT

    //UNION FIND combine variables

    int numVars = columns;

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
            throw runtime_error("mapping is invalid");
        }

        const int var1 = indexMapping[first].first + firstIndex;
        const int var2 = indexMapping[second].first + secondIndex;

        ds.union_set(var1, var2);
        numVars -= 1;
    }

    //UNION FIND combine variables

    //UNION FIND adjust columns

    int counter = 0;
    vector<int> mapRoots(columns);

    for (int i = 0; i < columns; ++i) {
        if (ds.find_set(i) == i) {//root found
            mapRoots[i] = counter;
            counter++;
        }
    }
    columns = counter;

    if (columns >= MAX_LSOE_BITSET - 1) {
        throw runtime_error("to many variables for the CFI homs solver");
    }

    //UNION FIND adjust columns

    //H.numVertices -> even subset guarantee for the mapped node
    //H.edges.size() -> same opinion on the mapped edge
    const int rows = H.numVertices + static_cast<int>(H.edges.size());

    //Now we create the matrix and initialize with 0
    auto matrix = vector<bitset<MAX_LSOE_BITSET>>(rows);

    //Fill with the even subset guarantee
    for (int i = 0; i < H.numVertices; i++) {
        for(int j = indexMapping[i].first; j < indexMapping[i].second; j++) {
            matrix[i][mapRoots[ds.find_set(j)]] = 1;
        }
        //uneven subset guarantee
        if (mapping[i] == vertice) {
            matrix[i][columns] = 1;
        }
    }

    LinearSystemOfEquations result = {matrix, columns};
    return result;
}

//calc exponent of number of homs from H to CFI Graph of S based on a mapping
//every node in H is mapped to one in S (predecided through mapping disregarding the color)
//the exact number of homs can be calculated when applying 2^
//also note that when there are now homs -1 is returned
int CalcHoms::calcNumHomsCFI_unoptimized(const Graph& H, const Graph& S, const int* mapping, const bool inverted, const pair<int, int> &edge) {

    auto lsoe = generateCFI_LSOE_unoptimized(H,S,mapping, edge);

    //Now we can calculate the dimension of the solution space
    int dimension;
    if (inverted) {
        dimension = solution_space_dimension_f2_small_inhomogen(lsoe);
    } else {
        dimension = solution_space_dimension_f2_small_homogen(lsoe);
    }

    if (dimension > 62) {
        throw runtime_error("Dimension of solution space was to big for long long");
    }

    return dimension;
}

int CalcHoms::calcNumHomsCFI(const Graph& H, const Graph& S, const int* mapping, const bool inverted, const int vertice) {

    auto lsoe = generateCFI_LSOE(H,S,mapping, vertice);

    //Now we can calculate the dimension of the solution space
    int dimension;
    if (inverted) {
        dimension = solution_space_dimension_f2_small_inhomogen(lsoe);
    } else {
        dimension = solution_space_dimension_f2_small_homogen(lsoe);
    }

    if (dimension > 62) {
        throw runtime_error("Dimension of solution space was to big for long long");
    }

    return dimension;
}


//returns the number of homs from H to CFI Graph of S, (by trying every possible mapping) (works only for uncolored)
//Be sure that H has <= 9 vertices and S has maxdegree <= 4
int256_t CalcHoms::calcNumHomsCFI_uncolored(const Graph &H, const Graph &S, const bool inverted) {

    if (S.edges.size() < 1) {
        throw runtime_error("S has to have at least one edge!");
    }

    //Edge S to be inverted with if inverted = true
    const auto edge = S.edgeArray[0];

    int256_t total = 0;

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
                        //We have to calculate the number of homs from H to CFI(S) with this mapping
                        int exponent = calcNumHomsCFI(H, S, hom, inverted, 0);
                        if (exponent > -1) {
                            total += powBase2(exponent);
                        }
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

int256_t CalcHoms::calcNumHomsCFI_colored(const Graph &H, const Graph &S, const bool inverted) {

    if (!H.colored || !S.colored) {
        throw runtime_error("Both graphs have to be colored!");
    }

    if (S.edges.size() < 1) {
        throw runtime_error("S has to have at least one edge!");
    }

    int256_t total = 0;

    //generate all homs and for each hom count the number of cfi homs from H to CFI(S)

    const int *nodeIndex = H.nodeIndex;

    int* hom = new int[H.numVertices];
    for (int i = 0; i < H.numVertices; i++) {
        hom[i] = 0;
    }

    int currtochange = 1;
    bool increment = false;

    // Precompute valid mappings based on color
    // possibleMappings[v].first is an array of valid node-IDs in G
    // possibleMappings[v].second is how many valid node-IDs in that array
    pair<int*, int> * possibleMappings = new pair<int*, int>[H.numVertices];
    for (int v = 0; v < H.numVertices; v++) {
        vector<int> valid;
        for (int w = 0; w < S.numVertices; w++) {
            // If color matches, node w in G is a valid image of v
            if (S.nodes[w].equals(H.nodes[v])) {
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
                    if (!S.isEdge(hom[H.edgeArray[edgeindex].first], candidate)) {
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
                        if (!S.isEdge(hom[H.edgeArray[edgeindex].first], candidate)) {
                            works = false;
                            break;
                        }
                    }
                }
                if (works) {
                    hom[currtochange] = candidate;
                    candidateIndex[currtochange] = idx;
                    if (currtochange == H.numVertices - 1) {

                        //ONLY REAL DIFFERENCE TO COUNTING HOMS

                        //Handling the found Hom!
                        //We have to calculate the number of homs from H to CFI(S) with this mapping
                        int exponent = calcNumHomsCFI(H, S, hom, inverted, 0);
                        if (exponent > -1) {
                            total += powBase2(exponent);
                        }
                        //END ONLY REAL DIFFERENCE TO COUNTING HOMS

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
    for (int v = 0; v < H.numVertices; v++) {
        delete[] possibleMappings[v].first;
    }
    delete[] possibleMappings;

    delete[] hom;
    delete[] candidateIndex;

    return total;
}


auto mtx = std::mutex();

void fillMap(std::unordered_map<std::string, int256_t> &componentMap, const Graph &graph, const std::string &canon, const Graph &G, const bool CFI, const bool inverted, const bool colored = false) {
    int256_t num = 0;
    if (CFI) {
        num = colored ? CalcHoms::calcNumHomsCFI_colored(graph, G, inverted) : CalcHoms::calcNumHomsCFI_uncolored(graph, G, inverted);
    } else {
        num = CalcHoms::calcNumHoms(graph, G);
    }

    // Sicherer Mutex-Schutz
    std::lock_guard<std::mutex> lock(mtx);
    componentMap[canon] = num;
}

int256_t CalcHoms::calcNumInjHoms(const std::string &spasm_file_name, const Graph &G, bool CFI_OF_G, bool CFI_inverted) {
    auto spasm = Spasm::getFromFile(spasm_file_name);
    return calcNumInjHoms(spasm, G, CFI_OF_G, CFI_inverted);
}



int256_t CalcHoms::calcNumInjHoms(Spasm::Spasm spasm, const Graph &G, bool CFI_OF_G, bool CFI_inverted) {

    if (spasm.underlying_Graph.colored && !G.colored) {
        throw runtime_error("Graphs do not match in being colored or not!");
    }

    const bool colored = spasm.underlying_Graph.colored;

    std::unordered_map<std::string, int256_t> componentMap;

    auto numThreads = boost::thread::physical_concurrency();
    if (numThreads == 0) {
        numThreads = 10;
        cout << "Could not determine number of threads!" << endl;
    }

    //cout << "Using " << numThreads << " threads" << endl;

    boost::asio::thread_pool pool(numThreads);

    for (const auto &comp : spasm.Components) {
        std::string canon = comp.canonicalString;
        Graph graph = comp.Graph;

        boost::asio::post(pool, [&, canon, graph]() {
            fillMap(componentMap, graph, canon, G, CFI_OF_G, CFI_inverted, colored);
        });
    }

    //cout << "Waiting for threads to finish!" << endl;
    pool.join();

    int256_t total = 0;

    for (auto &fullGraph : spasm.Graphs) {
        int256_t factor = fullGraph.Factor;
        for (auto &comp : fullGraph.Components) {
            std::string canon = comp.first;
            const int exponent = comp.second;

            int256_t newFactor = int256_pow(componentMap[canon], exponent);

            // Secure overflow check (positive and negativ):
            if (factor > 0) {
                if (newFactor > 0) {
                    if (factor > std::numeric_limits<int256_t>::max() / newFactor)
                        throw std::overflow_error("Overflow detected in multiplication");
                } else if (newFactor < 0) {
                    if (newFactor < std::numeric_limits<int256_t>::min() / factor)
                        throw std::overflow_error("Overflow detected in multiplication");
                }
            } else if (factor < 0) {
                if (newFactor > 0) {
                    if (factor < std::numeric_limits<int256_t>::min() / newFactor)
                        throw std::overflow_error("Overflow detected in multiplication");
                } else if (newFactor < 0) {
                    if (factor != 0 && newFactor < std::numeric_limits<int256_t>::max() / factor)
                        throw std::overflow_error("Overflow detected in multiplication");
                }
            }

            // Durchführen der Multiplikation nach Prüfung:
            factor *= newFactor;

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

//calc number of automorphisms
int256_t CalcHoms::calcNumAutomorphisms(const Graph &G) {
    return CalcHoms::calcNumInjHoms(G, G);
}

int256_t CalcHoms::calcNumSubgraphs(const std::string &spasm_file_name, const Graph &G, const bool CFI_OF_G, const bool CFI_inverted) {
    Spasm::Spasm spasm = Spasm::getFromFile(spasm_file_name);
    return calcNumSubgraphs(spasm, G, CFI_OF_G, CFI_inverted);
}

int256_t CalcHoms::calcNumSubgraphs(const Spasm::Spasm& spasm, const Graph &G, bool CFI_OF_G, bool CFI_inverted) {
    return calcNumInjHoms(spasm, G, CFI_OF_G, CFI_inverted) / spasm.numAutomorphisms;
}

