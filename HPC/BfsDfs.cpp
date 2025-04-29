#include <iostream>
#include <vector>
#include <queue>
#include <omp.h> // OpenMP header for parallel programming

using namespace std;

// Structure to represent a graph
struct Graph {
    int V; // Number of vertices
    vector<vector<int>> adj; // Adjacency list

    // Constructor to initialize the graph with V vertices
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    // Function to add an undirected edge between u and v
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Breadth-First Search starting from a given node
    void BFS(int start) {
        vector<bool> visited(V, false); // Track visited nodes
        queue<int> q; // Queue for BFS

        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            cout << u << " "; // Print the current node

            // Parallelize the loop using OpenMP
            #pragma omp parallel for
            for (int i = 0; i < adj[u].size(); i++) {
                int v = adj[u][i];
                if (!visited[v]) {
                    // Critical section to prevent race conditions
                    #pragma omp critical
                    {
                        visited[v] = true;
                        q.push(v);
                    }
                }
            }
        }
        cout << endl;
    }

    // Depth-First Search starting from a given node
    void DFS(int start) {
        vector<bool> visited(V, false); // Track visited nodes

        // OpenMP parallel region
        #pragma omp parallel
        {
            // Ensure a single thread executes the DFS traversal
            #pragma omp single nowait
            {
                DFSUtil(start, visited);
            }
        }
        cout << endl;
    }

    // Utility function for recursive DFS
    void DFSUtil(int u, vector<bool>& visited) {
        visited[u] = true;
        cout << u << " "; // Print the current node

        // Parallelize loop to explore adjacent nodes
        #pragma omp parallel for
        for (int i = 0; i < adj[u].size(); i++) {
            int v = adj[u][i];
            if (!visited[v]) {
                // Recursive DFS call
                DFSUtil(v, visited);
            }
        }
    }
};

int main() {
    int V;
    cout << "Enter the number of vertices: ";
    cin >> V;

    Graph g(V); // Create graph with V vertices

    int edgeCount;
    cout << "Enter the number of edges: ";
    cin >> edgeCount;

    cout << "Enter the edges (in format 'source destination'): \n";
    for (int i = 0; i < edgeCount; i++) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v); // Add edge to the graph
    }

    cout << "BFS traversal starting from node 0: ";
    g.BFS(0); // Perform BFS starting from node 0

    cout << "DFS traversal starting from node 0: ";
    g.DFS(0); // Perform DFS starting from node 0

    return 0;
}
/*
-------------------------
How to Compile and Run:
-------------------------
g++ -fopenmp BfsDfs.cpp -o bfs
./bfs

-------------------------
Sample Input:
-------------------------
Enter number of vertices: 5
Enter number of edges: 4
Enter edges (format: u v):
0 1
0 2
1 3
1 4

Expected Output:
Parallel BFS traversal starting from node 0:
0 1 2 3 4
*/