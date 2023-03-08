/********
 * Team: Maze Solver
 * map.cc: Primary code for maze solver
 * Class: CS 3560
********/

#include <fstream>
#include <iostream>
#include <string>
#include <stack>
#include <vector>
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
Node* add(Node *& root, Node *& previousNode, string chosenDirection, string line, vector<string> directions);

//adds to the directions stack
void addToDirections(stack<string> &directions, string chosenDirection);

//Prints all the directions stored
void printDirections(vector<string> directions);

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
    vector<string> directions;
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
Node* add(Node *& root, Node *& previousNode, string chosenDirection, string line, vector<string> directions){
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
string chooseDirection(string line, int space1loc, int numDirections, Node* intersection){
    if(intersection == NULL){//none of the paths in front of the bot have ever been visited before. NULL is deliberately sent
        if (line.substr(0, space1loc) == "L"){
            return "LEFT";
        }
        if (line.substr(0, space1loc) == "S"){
            return "STRAIGHT";
        }
        if(line.substr(0, space1loc) == "D"){
            return "DEADEND";
        }
        else
            return "RIGHT";
    }
    else{//one or more paths have already been visited before, came from backtracking
        if(numDirections > 1){
            //left doesn't need to be tried again, check for straight and then maybe right
            int Slocation = line.find("S");
            int Rlocation = line.find("R");
            if((intersection->straight != NULL) && (Slocation != -1)){//if straight is an option in the string AND it has not been visited before
                return "STRAIGHT";
            }
            else if((intersection->right != NULL) && (Rlocation != -1)){
                return "RIGHT";
            }
            else{
                return "AGAIN";//all the pathways at the intersection have been visited, backtracking needs to be applied again
            }
        }
        else{
            return "AGAIN";//signal to loop the backtracking again
        }
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
    intersection->left = graph->left;
    intersection->right = graph->right;
    intersection->straight = graph->straight;
    intersection->previous = graph->previous;
}

void addToDirections(vector<string> &directions, string chosenDirection){
    directions.push_back(chosenDirection);
}

void printDirections(vector<string> directions){
    cout << endl;
    for(int i=0; i < directions.size(); i++){
        cout << directions[i] << endl;
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

void backtrack(Node* root, Node* cursor, stack<string> &backtracking, vector<string> &directions, int distanceFromIntersection){

    //debugging code
    cout << "Here is the backtracking list right now:";
    printBacktracking(backtracking);

    for(int i=0; i < distanceFromIntersection; i++){
        cout << "I'm moving: " << backtracking.top() << endl;;
        backtracking.pop();
        directions.pop_back();
    }
    cout << "Here is the backtracking list after:" << endl;
    printBacktracking(backtracking);
}

bool isIntersection(Node * root){
    int directionCount = 0;
    if(root->left != NULL){
        directionCount++;
    }
    if(root->straight != NULL){
        directionCount++;
    }
    if(root->right != NULL){
        directionCount++;
    }

    if(directionCount > 1){
        return 1;
    }
    else{
        return 0;
    }
}

void moveThroughMaze(Node* &graph, Node* placeholder, stack<string> &backtracking, stack<Node*> &intersections, vector<string> &directions){
    int space1loc = -1;
    int space2loc = -1;
    int numDirections = 0;
    string chosenDirection;
    string line;
    
    Node* cursor;
    cursor = graph;
    intersections.push(cursor);//starts the intersection list at the root
    
    while(line != "Q"){
        cout << endl << "What's in front of me?" << endl;
        getline(cin, line);
        
        if(line == "Q") {
            break;
        }
        numDirections = numDirectionsCount(line, space1loc, space2loc);
        chosenDirection = chooseDirection(line, space1loc, numDirections, NULL);
        //        addToDirections(chosenDirection); moving somewhere else, I think this is wrong
        cout << "The chosen direction is: " << chosenDirection << endl;
        
        if(chosenDirection == "DEADEND"){
            backtrack(graph, cursor, backtracking, intersections.top()->distanceFromIntersection);
            chosenDirection = chooseDirection(intersections.top()->instruction, space1loc, numDirections);
            //            addToDirections(chosenDirection); still moving somewhere else
            while(chosenDirection == "AGAIN"){
                intersections.pop();//pops the last intersection from the list
                backtrack(graph, cursor, backtracking, intersections.top()->distanceFromIntersection);
                chosenDirection = chooseDirection(intersections.top()->instruction, space1loc, numDirections);//checks again to see if any directions are available
                //                addToDirections(chosenDirection);still moving somewhere else
            }
            numDirections = numDirectionsCount(intersections->top()->instruction, space1loc, space2loc);
            //before doing anything else, I need to:
            //- copy the node at the top of the stack
            //- pop it from the stack
            //- take that copy's link to the next node, and THAT NODE's previous
            //- new intersection node
            //- copyNode(add()) at that spot, but in the new direction.
            //- push that intersection to the stack now with its old links and the new ones
            //the lists are now up to date
            Node* topOfTheStack;
            copyNode(topOfTheStack, intersections.top());
            intersections.pop();
            Node* newIntersection;
            
            //right now I'm right here. I need to make sure it copies and adds at the new spot, but it's going to go to the bottom of the list in the completely wrong way until I fix the add function with a new stack of directions.
            addToDirections(directions, chosenDirection);
            copyNode(add(graph, placeholder, chosenDirection, line, directions), newIntersection);//copies the new node with the info from the intersection
            intersections.push(newIntersection);//pushes that new intersection onto the stack
            intersections.top()->distanceFromIntersection++;//increments the distance from the intersection by 1
            
            
            
            addToBacktracking(backtracking, chosenDirection);//adds to the backtracking list
            intersections.top()->distanceFromIntersection++;
        }
        
        else{
            if (numDirections > 1){
                Node* newIntersection;
                addToDirections(directions, chosenDirection);
                copyNode(add(graph, placeholder, chosenDirection, line, directions), newIntersection);//copies the new node with the info from the intersection
                intersections.push(newIntersection);//pushes that new intersection onto the stack
                intersections.top()->distanceFromIntersection++;//increments the distance from the intersection by 1
            }
            else{
                addToDirections(directions, chosenDirection);
                add(graph, placeholder, chosenDirection, line, directions);
            }
            addToBacktracking(backtracking, chosenDirection);//adds to the backtracking list
            intersections.top()->distanceFromIntersection++;
        }
        
    }
}

void printIntersections(Node* graph, stack<Node*> intersections){
    cout << endl;
    while (!intersections.empty()) {
        cout << "intersection -> left:" << intersections.top()->left << ":" << endl;
        cout << "intersection -> straight:" << intersections.top()->straight << ":" << endl;
        cout << "intersection -> right:" << intersections.top()->right << ":" << endl;
        intersections.pop();
       }
}
