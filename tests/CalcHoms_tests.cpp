//
// Created by falco on 04.03.25.
//

#include <gtest/gtest.h>
#include "utilities.h"

#include "Graph.h"
#include "CalcHoms.h"
#include "RandomGraphGenerator.h"
#include "CFIGraph.h"
#include "GraphTemplate.h"
#include "Spasm.h"

//Check one specific graph
//S is a triangle with two equal color (0,1)
//H has two nodes with color (0,1) and one edge
//we map the nodes of H to different nodes in S
//the result should be the same in both tests
TEST(CalcHomsTest, calcNumHomsCFI_unoptimized_1) {
    //S is a triangle
    GraphTemplate STemplate = GraphTemplate();
    STemplate.addNode(Node(0));//node has id: 0
    STemplate.addNode(Node(1));
    STemplate.addNode(Node(1));
    STemplate.addEdge(0,1);//edge between node with id 0 and 1
    STemplate.addEdge(0,2);
    STemplate.addEdge(1,2);

    Graph S(STemplate);

    GraphTemplate HTemplate = GraphTemplate();
    HTemplate.addNode(Node(0));
    HTemplate.addNode(Node(1));
    HTemplate.addEdge(0,1);

    Graph H(HTemplate);

    int* mapping = new int[2];
    mapping[0] = 0;
    mapping[1] = 1;

    const int dim = CalcHoms::calcNumHomsCFI_unoptimized(H,S,mapping);

    EXPECT_EQ(dim, 1);

    int* mapping2 = new int[2];
    mapping2[0] = 0;
    mapping2[1] = 2;

    const int dim2 = CalcHoms::calcNumHomsCFI_unoptimized(H,S,mapping2);

    EXPECT_EQ(dim2, 1);
}

//Check Graphs up to 10 vertices
//S is mapped into the CFI of S
//This testcase does not cover duplicate colors in S
TEST(CalcHomsTest, calcNumHomsCFI_unoptimized_2) {
    for (int vertices = 2; vertices < 10; vertices++) {
        for (int edges = vertices-1; edges <= (vertices * (vertices - 1)) / 2; edges++) {
            RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(vertices, edges, true, true);
            Graph S = randomGraphGenerator.generateRandomConnectedGraph();
            int* mapping = new int[vertices];
            for (int j = 0; j < vertices; j++) {
                mapping[j] = j;
            }
            int dim = CalcHoms::calcNumHomsCFI_unoptimized(S,S,mapping);
            int expected = (edges - vertices + 1);
            ASSERT_EQ(dim, expected);
        }
    }
}

//Check Graphs up to 10 vertices
//A Graph with 4 nodes and 4 edges is mapped into the CFI of a random Graph S
//This testcase does not cover duplicate colors in S
TEST(CalcHomsTest, calcNumHomsInvCFI_unoptimized_1) {
    int count = 0;
    for (int vertices = 2; vertices < 8; vertices++) {
        for (int edges = vertices-1; edges <= (vertices * (vertices - 1)) / 2; edges++) {

            cout << "check Graph combination:" << count << endl;
            count++;

            RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(vertices, vertices * (vertices-1)/2, true, true);
          
            Graph S = randomGraphGenerator.generateRandomConnectedGraph();

            RandomGraphGenerator randomGraphGenerator2 = RandomGraphGenerator(vertices, edges, true, true);
            Graph H = randomGraphGenerator2.generateRandomConnectedGraph();

            int* mapping = new int[vertices];
            for (int j = 0; j < vertices; j++) {
                mapping[j] = j;
            }

            int dim = -1;
            try {
                dim = CalcHoms::calcNumHomsCFI_unoptimized(H,S,mapping, true, S.edgeArray[0]);
            } catch (const std::exception& e) {
                std::cerr << "Caught exception: " << e.what() << std::endl;
            }

            if (dim != -1) {
                cout << "dim:" << dim << endl;
                cout << "S:" << endl;
                S.printGraph();
                cout << "H:" << endl;
                H.printGraph();
            }
            long long result = 0;
            if (dim != -1) {
                result = intPow(2,dim);
            }

            CFIGraph CFI = CFIGraph(S, true);
            Graph G = CFI.toGraph();
            long long expected = CalcHoms::calcNumHoms(H,G);

            ASSERT_EQ(result, expected);

        }
    }
}


TEST(CalcHomsTest, calcNumHomsInvCFI_unoptimized_2) {
    RandomGraphGenerator Sgen = RandomGraphGenerator(10, 15, true, true);
    for (int i = 0; i < 100; i++) {
        auto S = Sgen.generateRandomConnectedGraph();
        int* mapping = new int[S.numVertices];
        for (int j = 0; j < S.numVertices; j++) {
            mapping[j] = j;
        }
        ASSERT_EQ(-1, CalcHoms::calcNumHomsCFI_unoptimized(S, S, mapping, true, S.edgeArray[0]));
        delete [] mapping;
    }
}


TEST(CalcHomsTest, NumAutomorphisms) {
    //K-Matching Graphs
    for (int i = 1; i < 6; i++) {
        GraphTemplate g = GraphTemplate();
        for (int j = 0; j < i * 2; j++) {
            g.addNode(Node());
        }
        for (int j = 0; j < i*2; j+=2) {
            g.addEdge(j, j+1);
        }
        Graph G(g);
        ASSERT_EQ(CalcHoms::calcNumAutomorphisms(G), powBase2(i) * factorial(i));
    }
}


//SECTION: calcNumHomsCFI

//Check one specific graph
//S is a triangle with two equal color (0,1)
//H has two nodes with color (0,1) and one edge
//we map the nodes of H to different nodes in S
//the result should be the same in both tests
TEST(CalcHomsTest, calcNumHomsCFI_1) {
    //S is a triangle
    GraphTemplate STemplate = GraphTemplate();
    STemplate.addNode(Node(0));//node has id: 0
    STemplate.addNode(Node(1));
    STemplate.addNode(Node(1));
    STemplate.addEdge(0,1);//edge between node with id 0 and 1
    STemplate.addEdge(0,2);
    STemplate.addEdge(1,2);

    Graph S(STemplate);

    GraphTemplate HTemplate = GraphTemplate();
    HTemplate.addNode(Node(0));
    HTemplate.addNode(Node(1));
    HTemplate.addEdge(0,1);

    Graph H(HTemplate);

    int* mapping = new int[2];
    mapping[0] = 0;
    mapping[1] = 1;

    const int dim = CalcHoms::calcNumHomsCFI(H,S,mapping);

    EXPECT_EQ(dim, 1);

    int* mapping2 = new int[2];
    mapping2[0] = 0;
    mapping2[1] = 2;

    const int dim2 = CalcHoms::calcNumHomsCFI(H,S,mapping2);

    EXPECT_EQ(dim2, 1);
}

//Check Graphs up to 10 vertices
//S is mapped into the CFI of S
//This testcase does not cover duplicate colors in S
TEST(CalcHomsTest, calcNumHomsCFI_2) {
    for (int vertices = 2; vertices < 10; vertices++) {
        for (int edges = vertices-1; edges <= (vertices * (vertices - 1)) / 2; edges++) {
            RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(vertices, edges, true, true);
            Graph S = randomGraphGenerator.generateRandomConnectedGraph();
            int* mapping = new int[vertices];
            for (int j = 0; j < vertices; j++) {
                mapping[j] = j;
            }
            int dim = CalcHoms::calcNumHomsCFI(S,S,mapping);
            int expected = (edges - vertices + 1);
            ASSERT_EQ(dim, expected);
        }
    }
}

TEST(CalcHomsTest, calcNumHomsInvCFI_1) {
    int count = 0;
    for (int vertices = 2; vertices < 8; vertices++) {
        for (int edges = vertices-1; edges <= (vertices * (vertices - 1)) / 2; edges++) {

            cout << "check Graph combination:" << count << endl;
            count++;

            RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(vertices, vertices * (vertices-1)/2, true, true);
            Graph S = randomGraphGenerator.generateRandomConnectedGraph();

            RandomGraphGenerator randomGraphGenerator2 = RandomGraphGenerator(vertices, edges, true, true);
            Graph H = randomGraphGenerator2.generateRandomConnectedGraph();

            int* mapping = new int[vertices];
            for (int j = 0; j < vertices; j++) {
                mapping[j] = j;
            }

            int dim = -1;
            try {
                dim = CalcHoms::calcNumHomsCFI(H,S,mapping, true, 0);
            } catch (const std::exception& e) {
                std::cerr << "Caught exception: " << e.what() << std::endl;
            }

            if (dim != -1) {
                cout << "dim:" << dim << endl;
                cout << "S:" << endl;
                S.printGraph();
                cout << "H:" << endl;
                H.printGraph();
            }
            long long result = 0;
            if (dim != -1) {
                result = intPow(2,dim);
            }

            CFIGraph CFI = CFIGraph(S, true);
            Graph G = CFI.toGraph();
            long long expected = CalcHoms::calcNumHoms(H,G);

            ASSERT_EQ(result, expected);

        }
    }
}

TEST(CalcHomsTest, calcNumHomsInvCFI_2) {
    RandomGraphGenerator Sgen = RandomGraphGenerator(10, 15, true, true);
    for (int i = 0; i < 100; i++) {
        auto S = Sgen.generateRandomConnectedGraph();
        int* mapping = new int[S.numVertices];
        for (int j = 0; j < S.numVertices; j++) {
            mapping[j] = j;
        }
        ASSERT_EQ(-1, CalcHoms::calcNumHomsCFI(S, S, mapping, true, 0));
        delete [] mapping;
    }
}

//SECTION: speed comparison

TEST(CalcHomsTest, speedComparisonCFI) {

    for (int vertices = 2; vertices < 12; vertices++) {

        long long durationCFI1 = 0;
        long long durationCFI2 = 0;

        for (int times = 0; times < 10; times++) {
            for (int edges = vertices-1; edges <= (vertices * (vertices - 1)) / 2; edges++) {
                RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(vertices, edges, true, true);
                Graph S = randomGraphGenerator.generateRandomConnectedGraph();
                int* mapping = new int[vertices];
                for (int j = 0; j < vertices; j++) {
                    mapping[j] = j;
                }

                auto start1 = std::chrono::high_resolution_clock::now();
                int dim1 = CalcHoms::calcNumHomsCFI_unoptimized(S, S, mapping);
                auto end1 = std::chrono::high_resolution_clock::now();
                auto duration1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end1 - start1);
                durationCFI1 += duration1.count();

                auto start2 = std::chrono::high_resolution_clock::now();
                int dim2 = CalcHoms::calcNumHomsCFI(S, S, mapping);
                auto end2 = std::chrono::high_resolution_clock::now();
                auto duration2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - start2);
                durationCFI2 += duration2.count();

                int expected = (edges - vertices + 1);
                ASSERT_EQ(dim1, expected);
                ASSERT_EQ(dim1,dim2);
            }
        }

        std::cout << "Execution time for vertices = " << vertices
        << " CFI1: " << durationCFI1 << " nanoseconds"
        << " CFI2: " << durationCFI2 << " nanoseconds";

        if (durationCFI2 < durationCFI1) {
            std::cout << " FASTER ";
        }

        std::cout << std::endl;
    }
}

TEST(CalcHomsTest, speedComparisonInvCFI) {

    for (int vertices = 2; vertices < 10; vertices++) {

        long long totalDuration1 = 0;
        long long totalDuration2 = 0;

        for (int edges = vertices-1; edges <= (vertices * (vertices - 1)) / 2; edges++) {

            RandomGraphGenerator randomGraphGenerator = RandomGraphGenerator(vertices, vertices * (vertices-1)/2, true, true);
            Graph S = randomGraphGenerator.generateRandomConnectedGraph();

            RandomGraphGenerator randomGraphGenerator2 = RandomGraphGenerator(vertices, edges, true, true);
            Graph H = randomGraphGenerator2.generateRandomConnectedGraph();

            int* mapping = new int[vertices];
            for (int j = 0; j < vertices; j++) {
                mapping[j] = j;
            }

            auto start1 = std::chrono::high_resolution_clock::now();

            CalcHoms::calcNumHomsCFI_unoptimized(H,S,mapping, true, S.edgeArray[0]);

            auto end1 = std::chrono::high_resolution_clock::now();
            auto duration1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end1 - start1);
            totalDuration1 += duration1.count();


            auto start2 = std::chrono::high_resolution_clock::now();

            CalcHoms::calcNumHomsCFI(H,S,mapping, true, 0);

            auto end2 = std::chrono::high_resolution_clock::now();
            auto duration2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - start2);
            totalDuration2 += duration2.count();

        }

        std::cout << "Execution time for H.vertices = " << vertices
            << " CFI1: " << totalDuration1 << " nanoseconds"
            << " CFI2: " << totalDuration2 << " nanoseconds";

        if (totalDuration2 < totalDuration1) {
            std::cout << " FASTER ";
        }

        std::cout << std::endl;
    }
}


TEST(CalcHomsTest, calcNumHomsCFI_uncolored) {
    RandomGraphGenerator randomH = RandomGraphGenerator(5, 8);
    RandomGraphGenerator randomS = RandomGraphGenerator(5, 8, true);

    Graph H = randomH.generateRandomConnectedGraph();
    Graph S = randomS.generateRandomConnectedGraph();

    CFIGraph cfiS = CFIGraph(S);
    Graph cfiGraph = cfiS.toGraph();

    long long numHomsBruteForce = CalcHoms::calcNumHoms(H,cfiGraph);

    cout << "numHomsBruteForce: " << numHomsBruteForce << endl;

    int256_t numHoms = CalcHoms::calcNumHomsCFI_uncolored(H, S);

    cout << "numHoms: " << numHoms << endl;

    ASSERT_EQ(numHoms, numHomsBruteForce);
}


TEST(CalcHomsTest, calcNumInjHomsCFI) {

    GraphTemplate K2_mal_3T = GraphTemplate(false);

    for (int i = 0; i < 6; i++) {
        K2_mal_3T.addNode(Node());
    }

    K2_mal_3T.addEdge(0, 1);
    K2_mal_3T.addEdge(2, 3);
    K2_mal_3T.addEdge(4, 5);

    Graph K2_mal_3(K2_mal_3T);

    RandomGraphGenerator randomS = RandomGraphGenerator(5, 6, true);

    for (int i = 0; i < 20; i++) {
        Graph S = randomS.generateRandomConnectedGraph();

        CFIGraph cfiS = CFIGraph(S);
        Graph cfiGraph = cfiS.toGraph();

        long long numHomsBruteForce = CalcHoms::calcNumInjHoms(K2_mal_3, cfiGraph);

        cout << "numHomsBruteForce: " << numHomsBruteForce << endl;

        int256_t numHoms = CalcHoms::calcNumInjHoms("k_3.txt", S, true);

        cout << "numHoms: " << numHoms << endl;

        ASSERT_EQ(numHoms, numHomsBruteForce);
    }
}


TEST(CalcHomsTest, calcNumInjHomsCFI_inverted) {
    GraphTemplate K2_mal_3T = GraphTemplate(false);

    for (int i = 0; i < 6; i++) {
        K2_mal_3T.addNode(Node());
    }

    K2_mal_3T.addEdge(0, 1);
    K2_mal_3T.addEdge(2, 3);
    K2_mal_3T.addEdge(4, 5);

    Graph K2_mal_3(K2_mal_3T);


    RandomGraphGenerator randomS = RandomGraphGenerator(6, 7, true);

    for (int i = 0; i < 3; i++) {
        Graph S = randomS.generateRandomConnectedGraph();

        auto cfiS = CFIGraph(S, true);
        Graph cfiGraph = cfiS.toGraph();


        long long numHomsBruteForce = CalcHoms::calcNumInjHoms(K2_mal_3, cfiGraph);

        cout << "numHomsBruteForce: " << numHomsBruteForce << endl;

        int256_t numHoms = CalcHoms::calcNumInjHoms("k_3.txt", S, true, true);

        cout << "numHoms: " << numHoms << endl;

        ASSERT_EQ(numHoms, numHomsBruteForce);

    }
}

TEST(CalcHomsTest, calcNumInjHomsCFI_speed) {


    RandomGraphGenerator randomS = RandomGraphGenerator(30, 40, false);

    for (int i = 0; i < 1; i++) {
        Graph S = randomS.generateRandomConnectedGraph();

        int256_t numHoms = CalcHoms::calcNumInjHoms("k_8.txt", S, true, true);

        cout << "numHoms: " << numHoms << endl;

        int256_t numHoms_not_inverted = CalcHoms::calcNumInjHoms("k_8.txt", S, true, false);

        ASSERT_EQ(numHoms_not_inverted, numHoms);

    }
}


TEST(CalcHomsTest, Clique_testing3) {
    int n = 6;
    RandomGraphGenerator randomS = RandomGraphGenerator(n, n+2);
    auto S = randomS.generateRandomConnectedGraph();
    std::string filename = "k_" + std::to_string(n) + ".txt";
    auto inverted = CalcHoms::calcNumInjHoms(filename, S, true, true);
    auto normal = CalcHoms::calcNumInjHoms(filename, S, true, false);
    cout << "k = " << n <<" Inverted: " << inverted << " Normal: " << normal << endl;
    ASSERT_EQ(inverted, normal);
}

TEST(CalcHomsTest, SubGraphs) {

    //square in double square
    GraphTemplate squaret = GraphTemplate(false);
    for (int i = 0; i < 4; i++) {
        squaret.addNode(Node());
    }
    squaret.addEdge(0, 1);
    squaret.addEdge(1, 2);
    squaret.addEdge(2, 3);
    squaret.addEdge(3, 0);

    Graph square = Graph(squaret);

    Spasm::create_and_store_Spasm("squarefortest.txt", square, -1, uint256_t(CalcHoms::calcNumAutomorphisms(square)));

    squaret.addNode(Node());
    squaret.addNode(Node());

    squaret.addEdge(1, 4);
    squaret.addEdge(4, 5);
    squaret.addEdge(5, 2);

    Graph doubleSquare = Graph(squaret);

    ASSERT_EQ(CalcHoms::calcNumSubgraphs("squarefortest.txt", doubleSquare, false, false), 2);
}

TEST(CalcHomsTest, SubGraphs2) {
    GraphTemplate star3 = GraphTemplate(false);
    for (int i = 0; i < 4; i++) {
        star3.addNode(Node());
    }
    star3.addEdge(0, 1);
    star3.addEdge(0, 2);
    star3.addEdge(0, 3);

    Graph star3g = Graph(star3);

    ASSERT_EQ(CalcHoms::calcNumSubgraphs("k_1.txt", star3g, false, false), 3);
    ASSERT_EQ(CalcHoms::calcNumSubgraphs("k_2.txt", star3g, false, false), 0);
}


TEST(CalcHomsTest, calcNumHomsCFI_colored1) {
    for (int times = 0; times < 10000; ++times) {
        RandomGraphGenerator randomH = RandomGraphGenerator(5, 8, true, false, 3, false);
        RandomGraphGenerator randomS = RandomGraphGenerator(7, 12, true, false, 5, false);

        auto H = randomH.generateRandomConnectedGraph();
        auto S = randomS.generateRandomConnectedGraph();

        CFIGraph cfiS = CFIGraph(S, false);
        Graph cfiGraph = cfiS.toGraph();
        CFIGraph cfiS_inv = CFIGraph(S, true);
        Graph cfiGraph_inv = cfiS_inv.toGraph();

        auto numHoms = CalcHoms::calcNumHoms(H, cfiGraph);
        auto numHoms_inv = CalcHoms::calcNumHoms(H, cfiGraph_inv);
        auto numHomsCFI = CalcHoms::calcNumHomsCFI_colored(H, S, false);
        auto numHomsCFI_inv = CalcHoms::calcNumHomsCFI_colored(H, S, true);
        ASSERT_EQ(numHoms, numHomsCFI);
        ASSERT_EQ(numHoms_inv, numHomsCFI_inv);
        if (numHoms != 0) {
            cout << "numHoms: " << numHoms << " numHoms_inv: " << numHoms_inv << " numHomsCFI: " << numHomsCFI << " numHomsCFI_inv: " << numHomsCFI_inv << endl;
        }
    }
}
