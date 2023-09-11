#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

// A data structure specialized in use for Kruskal's algorithm for finding a
// minimum spanning tree of a graph
class DisjointSets{
private:
    vector<vector<int>> edges;
    vector<int> parent;
    vector<int> seniority;

    void Link(int node1, int node2);
    void PrintMatrix(vector<vector<int>> matrix);
public:
    int Find(int node);
    bool Union(int node1, int node2);
    vector<vector<int>> GetEdges();
    DisjointSets(vector<vector<int>> weights);
    ~DisjointSets();
};

// Utility function that prints out a given 2D integer array
void DisjointSets::PrintMatrix(vector<vector<int>> matrix){
    for (int i = 0; i < (int)matrix.size(); i++){
        vector<int> row = matrix.at(i);

        for (int j = 0; j < (int)row.size(); j++){
            cout << row.at(j) << " ";
        }
        cout << endl;
    }
    
    return;
}

// 1. Creates a "parent" and "seniority" array and sets the parent array to
// be equal to its indexes and seniority array all set to 0.
// Size of the arrays is equal to the number of elements in the given weights array
// 2. Converts the given weights array into an array of edges:
// {weight, startNode, endNode}
// 3. Sorts the edges into ascending order (std::sort)
DisjointSets::DisjointSets(vector<vector<int>> weights){
    // Initialising all nodes to be the parent of themselves
    // and all nodes to have an initial seniority of 0
    int size = weights.size();

    for (int i = 0; i < size; i++){
        parent.push_back(i);
        seniority.push_back(0);
    }
    
    // Converting the weights adjacency array into an array of edges
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            // Don't add "edges" from the same node back to itself
            if (i == j){
                continue;
            }
            
            vector<int> edge;
            edge.push_back(weights.at(i).at(j));
            edge.push_back(i);
            edge.push_back(j);
            edges.push_back(edge);
        }
    }
    
    // Sorts the edges into ascending order by weight
    sort(edges.begin(), edges.end());
}

DisjointSets::~DisjointSets(){

}

// Simply returns the set the given node belongs to
// Sets are represented by a representative node a part of the set
// The representative node is the first node that was a part of that set
int DisjointSets::Find(int node){
    if (parent.at(node) == node){
        return node;
    } else {
        parent.at(node) = Find(parent.at(node));
        return parent.at(node);
    }
}

// Links 2 nodes together by making the parent of one node the parent of the other
// which node gets what parent depends on their seniority
// This signifies the 2 nodes are now part of the same set
void DisjointSets::Link(int node1, int node2){
    if (seniority.at(node1) < seniority.at(node2)){
        parent.at(node1) = node2;
    } else {
        parent.at(node2) = node1;

        if (seniority.at(node1) == seniority.at(node2)){
            seniority.at(node1)++;
        }
    }

    return;
}

// Decides whether 2 nodes should be linked
// If they are from different sets, (have different representatives of the set/parents)
// they are linked, and this function returns true
// (To signify the edge they make should be made a part of the minimum spanning tree)
// Else, returns false and does nothing
bool DisjointSets::Union(int node1, int node2){
    node1 = Find(node1);
    node2 = Find(node2);

    if (node1 == node2){
        return false;
    } else {
        Link(node1, node2);
        return true;
    }
}

vector<vector<int>> DisjointSets::GetEdges(){
    return edges;
}


// Represents the input graph via an adjacency array called "country"
// and an edge weights 2D array called "pathCosts"
// Converts the input into an appropriate matrix of numbers for the edge weights,
// can perform Kruskal's algorithm to find the graph's minimum spanning tree,
// and can convert that tree into a numeric cost of implementing that connection given
// the cities initial connections given in the "country" array
class Graph{
private:
    vector<vector<int>> country;
    vector<vector<char>> build;
    vector<vector<char>> destroy;
    vector<vector<int>> pathCosts;
    int size;

    void PrintMatrix(vector<vector<int>> matrix);
    int ConvertCostToInt(char input);
public:
    Graph(vector<vector<int>> inpCountry, vector<vector<char>> build, vector<vector<char>> destroy);
    ~Graph();

    vector<vector<int>> FindMinimumSpanningTree();
    int FindCheapestCost();
};

// Utility function that prints out a given 2D integer array
void Graph::PrintMatrix(vector<vector<int>> matrix){
    for (int i = 0; i < (int)matrix.size(); i++){
        vector<int> row = matrix.at(i);

        for (int j = 0; j < (int)row.size(); j++){
            cout << row.at(j) << " ";
        }
        cout << endl;
    }
    
    return;
}

// Converts the cost of an element in the build/destroy matrices from a letter to a number
int Graph::ConvertCostToInt(char input){
    if (input >= 'A' && input <= 'Z'){
        return input - 'A';
    } else if (input >= 'a' && input <= 'z') {
        return input - 'a' + 26;
    }

    return 0;
}

// Initialises the variables of the graph
Graph::Graph(vector<vector<int>> inpCountry, vector<vector<char>> inpBuild, vector<vector<char>> inpDestroy){
    size = inpCountry.size();
    country = inpCountry;
    build = inpBuild;
    destroy = inpDestroy;
    
    // Sets up the path costs matrix ready for the minimum search tree processing
    pathCosts = country;

    for (int i = 0; i < size; i++){
        vector<int> row = pathCosts.at(i);

        for (int j = 0; j < size; j++){
            int element = row.at(j);

            // All paths that don't exist are given their positive build value as their cost
            if (element == 0){
                pathCosts.at(i).at(j) = ConvertCostToInt(build.at(i).at(j));
            } else {
            // All paths that do exist are given their negative destroy value as their cost
                pathCosts.at(i).at(j) = -ConvertCostToInt(destroy.at(i).at(j));
            }
            // This ensures that the MST will prefer to keep paths that exist, and cost
            // more to destroy, then start building new paths from the cheapest to the
            // most expensive
        }
    }

}

Graph::~Graph(){
    
}

// 4. For every edge in the array of edges:
//  a. Call Find() on the startNode of the edge
//  b. Call Find() on the endNode of the edge
//  c. Call Union() passing the 2 results of these functions:
//  Union(Find(startNode), Find(endNode))
//  d. If Union() returns true, add the edge to the minimum spanning tree
// Returns the adjacency array of the minimum spanning tree
vector<vector<int>> Graph::FindMinimumSpanningTree(void){

    // Disjoint sets data object to hold all the edges of the graph
    // for Kruskal's algorithm
    DisjointSets disjointSets(pathCosts);

    // The final adjacency matrix the minimum spanning tree will be
    // represented by
    vector<vector<int>> mstAdjArray;

    // Initialising the adjacency array with values of 0
    for (int i = 0; i < size; i++){
        vector<int> edge;
        edge.resize(size,0);
        mstAdjArray.push_back(edge);
    }

    vector<vector<int>> edges = disjointSets.GetEdges();

    for (int i = 0; i < (int)edges.size(); i++){
        vector<int> edge = edges.at(i);
        int node1 = edge.at(1);
        int node2 = edge.at(2);

        bool partOfMST = disjointSets.Union(node1, node2);

        if (partOfMST){
            // cout << endl;
            // cout << "Adding: " << node1 << " " << node2;
            mstAdjArray.at(node1).at(node2) = 1;
            mstAdjArray.at(node2).at(node1) = 1;
        }
    }

    // cout << endl;
    // cout << "Country:" << endl;
    // PrintMatrix(country);
    // cout << "MST:" << endl;
    // PrintMatrix(mstAdjArray);

    return mstAdjArray;
}

// Returns the total cost of building the MST of the graph
int Graph::FindCheapestCost(void){
    vector<vector<int>> mstAdjArray = FindMinimumSpanningTree();
    int cheapestCost = 0;

    // Only go through the upper diagonal triangle of the
    // adjacency array, as it is symmetrical along the
    // identity matrix, and we don't want to count the
    // cost of each edge twice!
    for (int i = 0; i < size - 1; i++){
        for (int j = i + 1; j < size; j++){
            // Build cost
            if (mstAdjArray.at(i).at(j) == 1 && country.at(i).at(j) == 0){
                cheapestCost += ConvertCostToInt(build.at(i).at(j));
            // Destroy cost
            } else if (mstAdjArray.at(i).at(j) == 0 && country.at(i).at(j) == 1){
                cheapestCost += ConvertCostToInt(destroy.at(i).at(j));
            }
        }
    }

    return cheapestCost;
}


int main(int argc, char const *argv[]){
    // Getting console input
    string input;
    getline(cin, input);
    stringstream wordStream(input);
    string word;

    // The 2D input arrays that will be used to initialise
    // the graph object
    vector<vector<int>> country = {{}};
    vector<vector<char>> build = {{}};
    vector<vector<char>> destroy = {{}};
    int wordCount = 0;

    while (wordStream >> word){
        int rowCount = 0;
        int columnCount = 0;

        for (int i = 0; i < (int)word.length(); i++){
            if (word.at(i) == ','){
                if (wordCount == 0){
                    country.push_back({});
                } else if (wordCount == 1){
                    build.push_back({});
                } else {
                    destroy.push_back({});
                }

                columnCount = 0;
                rowCount++;
            } else {
                if (wordCount == 0){
                    country.at(rowCount).push_back((word.at(i) == '1'));
                } else if (wordCount == 1){
                    build.at(rowCount).push_back(word.at(i));
                } else {
                    destroy.at(rowCount).push_back(word.at(i));
                }

                columnCount++;
            }
        }
        
        wordCount++;
    }
    
    // And now... creating the graph object and initialising it
    // with these 2D vectors
    Graph graph(country, build, destroy);
    // And finding the total cost of the cheapest path with 1 connection each to every city
    cout << graph.FindCheapestCost() << endl;

    return 0;
}
