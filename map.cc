/********
 * Team: Maze Solver
 * map.cc: Primary code for maze solver
 * Class: CS 3560
********/

#include <fstream>
#include <iostream>
#include <string>
#include <stack>
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
void inorder(Node * root);

//Calculates the size of the graph
int size(Node * root);

//adds a node to the graph
Node* add(Node *& root, Node *& previousNode, string chosenDirection, string line, stack<string> directions);

//adds to the directions stack
void addToDirections(stack<string> &directions, string chosenDirection);

//Prints all the directions stored
void printDirections(stack<string> directions);

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
void backtrack(Node* root, Node* cursor, stack<string> &backtracking, stack<string> &directions, int distanceFromIntersection);

//Moves through the maze with a cursor over the nodes
void moveThroughMaze(Node* &graph, Node* placeholder, stack<string> &backtracking, stack<Node*> &intersections, stack<string> &directions);

//Prints out all the intersections stored
void printIntersections(Node* graph, stack<Node*> intersections);

//Copies the node to the node stack
void copyNode(Node* graph, Node* &intersection);

int main(int argc, char *argv[]){
    
    //Node system for the graph to be stored in
    Node* graph;
    graph = NULL;
    Node* placeholder;
    placeholder = NULL;
    
    //Node system for add function
    stack<string> directions;
    //Node system for backtracking function
    stack<string> backtracking;
    //Node system for remembering intersections
    stack<Node*> intersections;
    
    
    ifstream instream;
    instream.open("input.txt");
    
    //Moves through the maze
    moveThroughMaze(graph, placeholder, backtracking, intersections, directions);
    
    cout << endl;
    inorder(graph);
    //output the backtracking list
    printBacktracking(backtracking);
    printIntersections(graph, intersections);
//    cout << endl << "size " << size(graph) << endl;
    
    
    return 0;
}

//root: the tree's initial node
//previousNode: previous node in the tree, which this is linked from
//ChosenDirection: String containing info and what direction should be selected (will be part of a future function).
Node* add(Node *& root, Node *& previousNode, string chosenDirection, string line, queue<string> directions){
    if(root == NULL){
        root=new Node;
        root->previous = previousNode;
        root->instruction = line;
        root->data = chosenDirection;
        root->left=root->right=root->straight=root->previous=NULL;
        root->directionCount = 0;
        root->directionsLeft = 0;
        root->distanceFromIntersection = 0;
        
        return previousNode;
    }
    else if (directions.top() == "LEFT"){
        directions.pop_front();
        return add(root->left, root, chosenDirection, line, directions);
    }
    else if (directions.top() == "STRAIGHT"){
        directions.pop_front();
        return add(root->straight, root, chosenDirection, line, directions);
    }
    else{
        directions.pop_front();
        return add(root->right, root, chosenDirection, line, directions);
    }
}

void inorder(Node* root){
    if(root != NULL){
        inorder(root->left);
        cout<<root->data<<endl;
        inorder(root->right);
        inorder(root->straight);
    }
}

//Function to choose the direction the node will travel
string chooseDirection(Node * current){
    if(current != NULL){
        if(current->left != NULL){
            return "LEFT";
        }
        else if(current->straight != NULL){
            return "STRAIGHT";
        }
        else if(current->right != NULL){
            return "RIGHT";
        }
        else if(current->previous != NULL){
            return "BACK";
        }
        else{
            return "NULL";
        }
    }
    else{
        return "NULL";
    }
}

//Function to count number of directions at intersection
int countDirections(Node * current){
    int numDirections = 0;
    if(current != NULL){
        if(current->left != NULL){
            numDirections++;
        }
        if(current->straight != NULL){
            numDirections++;
        }
        if(current->right != NULL){
            numDirections++;
        }
    }
    return numDirections;
}

void printIntersections(Node* graph, stack<Node> intersections){
//    cout << "graph -> left:" << graph->left << ":" << endl;
//    cout << "graph -> straight:" << graph->straight << ":" << endl;
//    cout << "graph -> right:" << graph->right << ":" << endl;
    cout << endl;
//    cout << "outside the loop" << endl;
    while (!intersections.empty()) {
//        cout << "inside the loop" << endl;
        cout << "intersection -> left:" << intersections.top().left << ":" << endl;
        cout << "intersection -> straight:" << intersections.top().straight << ":" << endl;
        cout << "intersection -> right:" << intersections.top().right << ":" << endl;
        intersections.pop();
       }
}

void copyNode(Node graph, Node* &intersection){
    intersection->instruction = graph.instruction;
    intersection->data = graph.data;
    intersection->directionCount = graph.directionCount;
    intersection->directionsLeft = graph.directionsLeft;
    intersection->isIntersection = graph.isIntersection;
    intersection->distanceFromIntersection = graph.distanceFromIntersection;
    intersection->left = graph.left;
    intersection->right = graph.right;
    intersection->straight = graph.straight;
    intersection->previous = graph.previous;
}

void addToDirections(stack<string> &directions, string chosenDirection){
    directions.push(chosenDirection);
}

void printDirections(stack<string> directions){
    cout << endl;
    while (!directions.empty()) {
        cout << directions.top() <<" ";
        directions.pop();
       }
}

int numDirectionsCount(string line, int &space1loc, int &space2loc){
    space1loc = line.find(' ');
    space2loc = line.find(' ', space1loc+1);
    if(space1loc == -1)
        return 1;
    else if(space2loc != -1)
        return 3;
    else
        return 2;
}

void addToBacktracking(stack<string> &backtracking, string chosenDirection){
    if(chosenDirection == "LEFT"){
        backtracking.push("RIGHT");
    }
    else if(chosenDirection == "STRAIGHT"){
        backtracking.push("STRAIGHT");
    }
    else{
        backtracking.push("LEFT");
    }
}

void printBacktracking(stack<string> backtracking){
    cout << endl;
    while (!backtracking.empty()) {
        cout << backtracking.top() <<" ";
        backtracking.pop();
       }
}

void backtrack(Node* root, Node* cursor, stack<string> &backtracking, stack<string> &directions, int distanceFromIntersection){

    //debugging code
    cout << "Here is the backtracking list right now:";
    printBacktracking(backtracking);

    for(int i=0; i < distanceFromIntersection; i++){
        cout << "I'm moving: " << backtracking.top() << endl;;
        backtracking.pop();
        directions.pop();
    }
    cout << "Here is the backtracking list after:" << endl;
    printBacktracking(backtracking);
}
