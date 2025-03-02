//
// Created by Christoph Greger on 02.03.25.
//

#include "../../include/Spasm_k_Matching.h"
#include <sstream>
#include <fstream>
#include "utilities.h"

#define set nauty_set //Wirklich kein Plan warum es das hier braucht

int maxn = 20;
int maxm = 1;


inline std::string getCanonicalString(graph *g, const int numVertices) {

    int lab[maxn], ptn[maxn], orbits[maxn]; //maxn
    static DEFAULTOPTIONS_GRAPH(options);
    options.getcanon = TRUE;
    statsblk stats;

    graph canon[maxn*maxm]; //maxn*maxm

    EMPTYGRAPH(canon, maxm, numVertices);

    densenauty(g, lab, ptn, orbits, &options, &stats, maxm, numVertices, canon);

    std::ostringstream oss;
    // Optional: stelle sicher, dass der Stream im hexadezimalen Format arbeitet.
    oss << std::hex;
    for (int i = 0; i < maxm*numVertices; i++) {
        // setfill und setw sorgen für führende Nullen, sodass jedes setword als 16-stellige hexadezimale Zahl ausgegeben wird.
        oss << std::setfill('0') << std::setw(16) << canon[i];
    }
    return oss.str();
}


//works only for k <= 10
void createMultiSpasm_k_Matching(const int k, const std::string &filename) {

    long long total = 0;
    // Map: Schlüssel ist die kanonische Form (als String),
    // Wert ist ein Paar aus der String-Repräsentation des Quotientgraphen und der Zählung.

    // First int in the second pair is the number of times the graph appeared
    // The long long is the factor in the möbius inversion
    std::unordered_map<std::string, std::pair<std::string, std::pair<int, long long>>> canonicalMap;

    const int numEdges = k;
    const int numVertices = 2 * k;

    // partition[i] gibt an, zu welchem Block der Knoten i gehört.
    std::vector<int> partition(numVertices, -1);
    partition[0] = 0;  // Der erste Knoten wird fest Block 0 zugeordnet
    std::vector<int> numperpartition(numVertices, 0);


    // Rekursive Backtracking-Funktion:
    //&g muss gross genug sein!
    //edgearray sollte numEdges platz haben!
    std::function<void(int, int, graph*, std::pair<int, int> *)> backtrack = [&](const int vertex, const int currentMax, graph *g, std::pair<int, int> *edgeArray) {
        if (vertex == numVertices) {
            // Erzeuge den Quotientgraphen anhand der aktuellen Partition.

            int numEdgeshere = 0;

            const int numVerticeshere = currentMax + 1;

            EMPTYGRAPH(g, maxm, numVerticeshere);

            // Für jede Kante im Originalgraphen, die zwei verschiedene Blöcke verbindet,
            // wird im Quotientgraphen eine entsprechende Kante eingefügt.
            for (int i = 0; i < numVertices; i += 2) {
                const int bu = partition[i];
                const int bv = partition[i+1];

                if (ISELEMENT(&g[bu], bv)) {
                    continue;
                }
                ADDONEEDGE(g, bu, bv, maxm);
                edgeArray[numEdgeshere] = std::make_pair(bu, bv);
                ++numEdgeshere;
            }
            // Berechne die kanonische Form des Quotientgraphen.

            const std::string canon = getCanonicalString(g, currentMax+1);

            //Computing the factor (or at least the Partition element product part of it) of the graph
            long long factor = 1;

            for (int i = 0; i < numVertices; i++) {
                numperpartition[i] = 0;
            }

            for (int i = 0; i < numVertices; i++) {
                ++numperpartition[partition[i]];
            }

            for (int i = 0; i < numVerticeshere; i++) {
                factor *= factorial(numperpartition[i]-1);
            }
            //End of factor computation (first part)


            if (!canonicalMap.contains(canon)) {
                std::stringstream ss;
                ss << numVerticeshere << " " << numEdgeshere << "\n";
                for (int i = 0; i < numEdgeshere; i++) {
                    ss << edgeArray[i].first << " -> " << edgeArray[i].second << "\n";
                }
                canonicalMap[canon] = std::make_pair(ss.str(), std::make_pair(1, factor));
            } else {
                canonicalMap[canon].second.first++;
                canonicalMap[canon].second.second += factor;
            }
            ++total;


            //DEBUGGING
            if (total % 1000000 == 0) {
                std::cout << "Total: " << total << std::endl;
            }
            return;
        }

        // Versuche, den aktuellen Knoten in jeden existierenden Block einzufügen.
        for (int block = 0; block <= currentMax; block++) {
            bool valid = true;

            // Überprüfe, ob das Hinzufügen in Block 'block' zu einer Self-Loop (Kante innerhalb des Blocks) führen würde.
            /**for (int v = 0; v < vertex; v++) {
                if (partition[v] == block) {
                    if (isEdgebySet(v, vertex)) {
                        valid = false;
                        break;
                    }
                }
            }
            **/

            //Hier jetzt auf K-Matching optimiert:
            if (vertex % 2 == 1) {
                if (partition[vertex - 1] == block) {
                    valid = false;
                }

            } else {
                if (partition[vertex + 1] == block) {
                    valid = false;
                }

            }

            //Ende K Matching Optimierung

            if (!valid) continue;
            partition[vertex] = block;
            backtrack(vertex + 1, currentMax, g, edgeArray);
        }
        // Eröffne einen neuen Block für den aktuellen Knoten.
        partition[vertex] = currentMax + 1;
        backtrack(vertex + 1, currentMax + 1, g, edgeArray);
    };

    auto *g = new graph[maxn * maxm];
    auto *edgearray = new std::pair<int, int>[numEdges];
    backtrack(1, 0, g, edgearray);

    // Schreibe die Ergebnisse in die Datei.
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Fehler beim Öffnen der Datei: " << filename << std::endl;
        return;
    }

    const auto totaldifferent = canonicalMap.size();
    outFile << totaldifferent << "\n\n";

    for (const auto &[fst, snd] : canonicalMap) {

        //Now we have to get the sign of the factor
        const int QuotiengraphnumVertices = std::stoi(snd.first);
        const int signexponent = numVertices - QuotiengraphnumVertices;
        long long factor = snd.second.second;
        if (signexponent % 2 == 1) {
            factor = -factor;
        }

        outFile << snd.first << "\n"; //zuerst ein Graph dieser kanonischen Form
        outFile << fst << "\n"; //dann die kanonische Form
        outFile << snd.second.first << "\n"; //dann die Anzahl, eigentlich unnötig
        outFile << factor << "\n\n"; //dann der Faktor
    }
    std::cout << "Total: " << total << std::endl;
    outFile.close();
    std::cout << "Total different: " << totaldifferent << std::endl;
    delete[] g;
    delete[] edgearray;
}

