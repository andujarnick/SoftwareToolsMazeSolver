/**
 * @file
 * @brief Header file of the maze solver
* @name map.h
* @authors Nick Andujar, Chase Deweese, Tyler Hixon, Norman Lee, Max Shumaker
* @link https://github.com/andujarnick/SoftwareToolsMazeSolver
* @class CS 3560
**/

#include "colors.h"

using namespace std;

//Nodes that let you go left, right or straight. Also can traverse the tree backwards
struct Node{
    string instruction;
    string data;//stored direction
    int directionCount;
    int distanceFromIntersection;
    bool directionsLeft;
    bool isIntersection;
    
    Node * left;
    Node * right;
    Node * straight;
    Node * previous;
};

//function to output everything mapped so far
void inOrder(Node * root);

//Calculates the size of the graph
int size(Node * root);

//adds a node to the graph
Node* add(Node *& root, Node *& previousNode, string chosenDirection, string line, vector<string> directions);

//adds to the directions stack
void addToDirections(vector<string> &directions, string chosenDirection);

//Prints all the directions stored
void printDirections(vector<string> directions, ifstream &instream, ofstream &outstream);

//looks to see if the node is an intersection
bool isIntersection(Node * root);

//chooses the correct default direction
string chooseDirection(string line, int space1loc, int numDirections, Node* intersection);

//counts the number of directions given to the computer
int numDirectionsCount(string line, int &space1loc, int &space2loc);

//adds to backtracking
void addToBacktracking(stack<string> &backtracking, string direction);

//outputs backtracking list
void printBacktracking(stack<string> backtracking);

//backtracks up the list
void backtrack(Node* root, Node* cursor, stack<string> &backtracking, vector<string> &directions, int distanceFromIntersection);

//Moves through the maze with a cursor over the nodes
void moveThroughMaze(Node* &graph, Node* placeholder, stack<string> &backtracking, stack<Node*> &intersections, vector<string> &directions);

//Prints out all the intersections stored
void printIntersections(Node* graph, stack<Node*> intersections);

//Copies the node to the node stack
void copyNode(Node* graph, Node* &intersection);

void printMaze(Node * root, vector<string> &directions, vector< vector <string> > &maze, int i, int j);

//Start Menu
void startMenu(Node * root, ifstream &instream, vector <string> &directions);

//Copys from file and briefly updates list from a save file of directions
void copyFromFile(Node * root, vector<string> directions);
