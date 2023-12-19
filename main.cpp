
#include <iostream>
#include <random>
#include <vector>
#include <list>
#include <stack>
#include <limits>
#include <algorithm>
#include <fstream>



using namespace std;
void generateGraphFile(const string &filename, int numNodes, int desiredEdges, int maxWeight);
int IvestiNodeSk();
int IvestiSvorSk();
int PasirinkimoGen();
string FailoPav();
int MinEdge(int node );
string IveskiteFailoPav();

class Graph {
    int vertices;
    vector<list<pair<int, int>>> adjList;

public:
    explicit Graph(int V) : vertices(V), adjList(V) {}

    void addNode(int from, int to, int weight) {
        adjList[from - 1].push_back({to - 1, weight});
    }

    void topologicalSortUtil(int v, vector<bool> &visited, stack<int> &stack) {
        visited[v] = true;

        for (auto &neighbor: adjList[v]) {
            int u = neighbor.first;
            if (!visited[u])
                topologicalSortUtil(u, visited, stack);
        }

        stack.push(v);
    }

    pair<vector<int>, int> topologicalSort() {
        stack<int> stack;
        vector<bool> visited(vertices, false);

        for (int i = 0; i < vertices; ++i) {
            if (!visited[i])
                topologicalSortUtil(i, visited, stack);
        }

        vector<int> distance(vertices, numeric_limits<int>::min());
        vector<int> parent(vertices, -1);

        distance[0] = 0; // Starting node

        while (!stack.empty()) {
            int u = stack.top();
            stack.pop();

            if (distance[u] != numeric_limits<int>::min()) {
                for (auto &neighbor: adjList[u]) {
                    int v = neighbor.first;
                    int weight = neighbor.second;

                    if (distance[u] + weight > distance[v]) {
                        distance[v] = distance[u] + weight;
                        parent[v] = u;
                    }
                }
            }
        }

        // Find the node with the smallest total weight
        int maxWeightNode = max_element(distance.begin(), distance.end()) - distance.begin();

        // Reconstruct the path
        vector<int> path;
        int node = maxWeightNode;
        while (node != -1) {
            path.push_back(node);
            node = parent[node];
        }
        reverse(path.begin(), path.end());

        return {path, distance[maxWeightNode]};
    }

    static Graph readGraphFromFile(const string &filename) {
        ifstream file(filename);

        if (!file) {
            cerr << "Error: Could not open file " << filename << endl;
            exit(1);
        }

        int vertices;
        file >> vertices;

        Graph graph(vertices);

        int from, to, weight;
        while (file >> from >> to >> weight) {
            graph.addNode(from, to, weight);
        }

        file.close();
        return graph;
    }

    static Graph RankinisIvedimas() {

        int nodeKiekis;// iveskite kiek bus nodes
        cout << "Iveskite nodes kieki (TURI BUTI NATURALUS SKAICIUS GRIEZTAI  DIDESNIS UZ 1 : " << endl;
        cin >> nodeKiekis;
        do {
            if (nodeKiekis > 1) {

            } else {
                cout << "Kladinga ivestis, bandykite dar karta (naturalu skaiciu didesni uz 1 )" << endl;
                cin.clear();
                cin.ignore(10000, '\n');
                cin >> nodeKiekis;
            }
        } while ( nodeKiekis <= 1);


        int uzduociuKiekis;// iveskite kiek bus uzduociu
        cout << "Iveskite uzduociu kieki (TURI BUTI NATURALUS SKAICIUS NEMAZESNIS UZ NODES KIEKI) : " << endl;
        cin >> uzduociuKiekis;
        do {
            if (cin.good() && (uzduociuKiekis >= nodeKiekis)) {
                int from, to, weight;
                Graph graph(nodeKiekis);
                for (int i = 0; i < uzduociuKiekis; i++) {
                    cout << "Iveskite " << i + 1 << " uzduoti" << endl;
                    cout << "Is ko eina kelias (" << i + 1
                         << " uzduotis): (iveskite node pavadinima skaiciais pvz: 1):  " << endl;
                    from = IvestiNodeSk();
                    cout << "I kur eina kelias (" << i + 1
                         << " uzduotis): (iveskite node pavadinima skaiciais pvz: 2):  " << endl;
                    to = IvestiNodeSk();
                    cout << "Koks " << i + 1 << " uzduoties laikas arba kelio svoris: (iveskite svori pvz : 4):  "
                         << endl;
                    weight = IvestiSvorSk();
                    graph.addNode(from, to, weight);


                }
                return graph;
            } else {
                cout << "Kladinga ivestis, bandykite dar karta (iveskite naturalu skaiciu NE MAZESNI uz UZ NODE KIEKI )"
                     << endl;
                cin.clear();
                cin.ignore(10000, '\n');
                cin >> uzduociuKiekis;
            }
        } while (!cin && uzduociuKiekis < nodeKiekis);

    }
};

struct Edge {
    int from;
    int to;
    int weight;
};

void generateGraphFile(const string &filename, int numNodes, int desiredEdges, int maxWeight) {
    ofstream file(filename);

    if (!file) {
        cerr << "Error: Could not create file " << filename << endl;
        exit(1);
    }

    file << numNodes << "\n";

    srand(time(0));  // Seed for randomization
    int fullEdge = (numNodes * (numNodes - 1)) / 2;
    int reqIterations = static_cast<int>(ceil(desiredEdges / static_cast<double>(fullEdge)));  // Round up

    vector<Edge> edges;
    for (int k = 1; k <= reqIterations; k++) {
        for (int i = 1; i <= numNodes; ++i) {
            for (int j = i + 1; j <= numNodes; ++j) {
                int weight = rand() % (maxWeight + 1);  // Random weight between 0 and maxWeight
                edges.push_back({i, j, weight});
            }
        }
    }
    // Shuffle the edges vector to randomize the order
    shuffle(edges.begin(), edges.end(), std::mt19937(std::random_device()()));

    // Ensure not to remove the first line
    int edgesToRemove = max(0, static_cast<int>(edges.size()) - desiredEdges);

    // Remove edges from the end of the vector
    edges.resize(edges.size() - edgesToRemove);

    // Clear and rewrite the file with the selected edges
    file.clear();
    file.seekp(0);
    file << numNodes << "\n";
    for (const auto &edge: edges) {
        file << edge.from << " " << edge.to << " " << edge.weight << "\n";
    }

    file.close();
}

int IvestiNodeSk() {
    int skaicius;
    cin >> skaicius;
    do {
        if (skaicius > 0) {

            return skaicius;
        } else {
            cout << "Kladinga ivestis, bandykite dar karta  )" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            cin >> skaicius;
        }

    } while ( skaicius <= 0);

}

int IvestiSvorSk() {
    int skaicius;
    cin >> skaicius;
    do {
        if (cin.good()) {

            return skaicius;
        } else {
            cout << "Kladinga ivestis, bandykite dar karta  )" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            cin >> skaicius;
        }
    } while (!cin);
}

int PasirinkimoGen() {
    int skaicius;
    cin >> skaicius;
    do {
        if (cin.good() and (skaicius == 0 or skaicius == 1 or skaicius == 2 or skaicius == 3)) {

            return skaicius;
        } else {
            cout << "Kladinga ivestis, bandykite dar karta  )" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            cin >> skaicius;
        }
    } while (!cin or skaicius != 0 or skaicius != 1 or skaicius != 2 or skaicius != 3);
}

string FailoPav() {
    string failoPavadinimas;
    cout << "Koki faila noretumete nuskenuoti :" << endl;
    system("dir *.txt");
    cout << "Iveskite pilna failo pavadinima (pvz: input.txt): " << endl;
    cin >> failoPavadinimas;
    return failoPavadinimas;

}

int MinEdge(int node) {
    int edge;
    int minEdge = (node * (node - 1)) / 2;
    cout << "Iveskite kiek norite uzduociu ne maziau nei :" << minEdge << endl;
    cin >> edge;
    do {
        if (cin.good() and edge >= minEdge) {

            return edge;
        } else {
            cout << "Kladinga ivestis, bandykite dar karta  )" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            cin >> edge;
        }
    } while (!cin or edge < minEdge);
}

string IveskiteFailoPav() {
    string failoPav;
    cout << "Iveskite failo pavadinima generavimui pvz : input.txt" << endl;
    cin >> failoPav;
    return failoPav;


}


int main() {


    cout << "SVEIKI,CIA MINIMALAUS DARBU ATLIKIMO ALGORITMAS " << endl;
    cout << "Ka darysite toliau (iveskite tolimesnio zingsio numeri):" << endl;
    cout << "(1) Ivesite duomenis ranka.  " << endl;
    cout << "(2) Nuskaitysite duomenis is failo." << endl;
    cout << "(3) Generuosite failus." << endl;
    cout << "(0) Baigti programa." << endl;
    int pasirinkimas = PasirinkimoGen();

    if (pasirinkimas == 1) {
        //ranka

        Graph graph = graph.RankinisIvedimas();
        cout << "Topological Sort and Longest Path with Smallest Total Weight:\n";
        auto result = graph.topologicalSort();
        cout << "Order of Nodes in the Path: ";
        for (int node: result.first) {
            cout << node + 1 << " ";
        }
        cout << "\nTotal Weight: " << result.second << endl;

    } else if (pasirinkimas == 2) {
        //is failo
        string filename = FailoPav();
        Graph graph = graph.readGraphFromFile(filename);
        cout << "Topological Sort and Longest Path with Smallest Total Weight:\n";
        auto result = graph.topologicalSort();
        cout << "Order of Nodes in the Path: ";
        for (int node: result.first) {
            cout << node + 1 << " ";
        }
        cout << "\nTotal Weight: " << result.second << endl;
    } else if (pasirinkimas == 3) {
        // generuoti
        cout << "Iveskite kiek bus Nodes" << endl;
        int nodes = IvestiNodeSk();
        cout << "Iveskite kiek bus uzduociu" << endl;
        int edges = MinEdge(nodes);
        string filename = IveskiteFailoPav();
        cout << "Iveskite turetu buti didziausias svoris uzduotyje " << endl;
        int weight = IvestiSvorSk();
        generateGraphFile(filename, nodes, edges, weight);// max Sum[n-j,{j,1,n}// // ]
        cout << "Sugeneruotas " << filename << endl;

    }
    else {
        // iseiti
        exit(0);
    }
    system("pause");
    return 0;
}

