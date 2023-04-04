/**
 @file
* @name map.cc
* @brief Maps a maze and finds the most direct route from start to finish
*
* @authors Nick Andujar, Chase Deweese, Tyler Hixon, Norman Lee, Max Shumaker
* @link https://github.com/andujarnick/SoftwareToolsMazeSolver
* @class CS 3560
**/

#include <fstream>
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <iomanip>
#include "map.h"
using namespace std;

int main(int argc, char *argv[]){
//    cout << B_BLACK << "   ";
//    cout << B_BLUE << WHITE << setw(33) << left << "  Test of the colors   " << endl;   //test of white text on blue background
//    cout << B_BLACK << "   ";
//    cout << B_BLUE << RED << setw(33) << left << "  Test of the colors   " << endl;     //test of red text on blue background
//    cout << B_BLACK << "   ";
//    cout << B_BLUE << YELLOW << setw(33) << left << "  Test of the colors   " << endl;   //test of yellow text on blue background
//    cout << B_BLACK << "   ";
//    cout << B_BLUE << MAGENTA << setw(33) << left << "  Test of the colors   " << endl;   //test of magenta text on blue background
//    cout << B_BLACK << "   ";
//    cout << B_MAGENTA << WHITE << setw(33) << left << "  Test of the colors   " << endl;   //test of white text on magenta background
//    cout << B_BLACK << "   ";
//    cout << B_RED << WHITE << setw(33) << left << "  Test of the colors   " << endl;   //test of white text on red background
//    cout << B_BLACK << "   ";
//    cout << B_BLACK << BLUE << setw(33) << left << "  Test of the colors   " << endl;   //test of blue text on black background
//    cout << B_BLACK << "   ";
    
    //Node system for the graph to be stored in
    Node* graph;
    graph = NULL;
    Node* placeholder;
    placeholder = NULL;
    
    startMenu(graph);
    cin.ignore();
    //Node system for add function
    vector<string> directions;
    //Node system for backtracking function
    stack<string> backtracking;
    //Node system for remembering intersections
    stack<Node*> intersections;
    //prime the list with a node, prime directions and backtracking with straight
    //allows for intersection immediately
    add(graph, placeholder, "S", "S", directions);
        
    //add it to the directions and backtracking so it can find it off the bat
//    addToDirections(directions, "STRAIGHT");
//    addToBacktracking(backtracking, "STRAIGHT");
    
    //only used with text file
//    ifstream instream;
//    instream.open("input.txt");
    
    //Moves through the maze
    moveThroughMaze(graph, placeholder, backtracking, intersections, directions);
    
    inOrder(graph);
    //output the backtracking list
    printBacktracking(backtracking);
    printDirections(directions);
    printIntersections(graph, intersections);
//    cout << endl << "size " << size(graph) << endl;
    
    //section to print out a maze
    vector <vector <string> > maze;
    
    for(int i=0; i<20; i++)
    {
        vector <string> column;
        for(int j=0; j<20; j++)
        {
             column.push_back(" --- ");
        }
        maze.push_back(column);
    }
    
    printMaze(graph, directions, maze, maze.size()/2, maze.size()/2);
    
    for (int i = 0; i < maze.size(); i++)
    {
        for (int j = 0; j < maze[i].size(); j++)
        {
            cout << maze[i][j];
        }
        cout << endl;
    }
    return 0;
}

/**
* @name add
* @brief Adds a node to the tree.
* @param root Type: Node, The root node of the maze tree.
* @param previousNode Type: Node, The previous tree node that this new node is linked from.
* @param chosenDirection Type: string, The selected direction to be input.
* @param line Type: string, direction from the root node.
* @param directions Type: vector<string>, vector containing the directs taken so far, in order to reach this node.
* @return returns the node that this new node is linked to.
**/
Node* add(Node *& root, Node *& previousNode, string chosenDirection, string line, vector<string> directions){
//    if(directions.size() != 0){
//        cout << "direction: " << directions[0];
//    }
    
//    cout << "directions.front(): " << directions[0] << ":" << endl;
    if(directions.size() > 0){
        cout << "directions:" << directions[0] << endl;
    }
    
    if(root == NULL){
        root=new Node;
        root->previous = previousNode;
        root->instruction = line;
        
        root->data = chosenDirection;
        root->left=root->right=root->straight=root->previous=NULL;
        root->directionCount = 0;
        root->directionsLeft = 0;
        root->distanceFromIntersection = 0;
        //trying to prime the list with a node to fix the problem instead of messing with returning itself
//        if (previousNode == NULL){
//            cout << "returning root" << endl;
//            return root;
//        }
        return previousNode;
    }
    else if (directions[0] == "LEFT"){
        cout << "going left" << endl;
        directions.erase(directions.begin());
        return add(root->left, root, chosenDirection, line, directions);
    }
    else if (directions[0] == "STRAIGHT"){
        cout << "going straight" << endl;
        directions.erase(directions.begin());
        return add(root->straight, root, chosenDirection, line, directions);
    }
    else{
        cout << "going right" << endl;
        directions.erase(directions.begin());
        return add(root->right, root, chosenDirection, line, directions);
    }
}

/**
* @name inOrder
* @brief Prints the node tree in order.
* @param root Type: Node, The root node of the maze tree.
* @return void.
**/
void inOrder(Node* root){
    if(root != NULL){
        inOrder(root->left);
        cout<<root->data<<endl;
        inOrder(root->right);
        inOrder(root->straight);
    }
}

/**
* @name chooseDirection
* @brief Selects the direction an input node will travel.
* @param line Type: string, the input of directions that can be selected.
* @param space1loc Tpye: int, location of the first space in the line substring.
* @param numDirections Tpye: int, the number of directions in the intersection.
* @param intersection Type: Node, the input intersection which will have a direction selected.
* @return Type: string, the selected direction of travel.
**/
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

/**
* @name countDirections
* @brief Counts the number of directions that can be visited from a current node.
* @param current Type: Node, the root to have it's possible directions counted.
* @return Type: int, number of possible directions of travel.
**/
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

/**
* @name printIntersections
* @brief Prints the intersections present in the node tree.
* @param graph Type: Node, the root node of the main maze graph.
* @param intersections Type: stack<Node> stack of nodes that contains all of the interesctions in the graph.
* @return void.
**/
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

/**
* @name copyNode
* @brief Copies an input Node
* @param graph Type: Node, the root node of the main maze graph.
* @param intersection Type: Node, the intersection that needs a node copied to it.
* @return void.
**/
void copyNode(Node* graph, Node* &intersection){
    intersection = new Node;//THIS IS THE ONE LINE THAT FIXES EVERYTHING
//    cout << "graph->instruction:" << graph->instruction << ":" << endl;
    if(graph->previous == NULL){
        intersection->instruction = graph->instruction;
    }
    else{
        if(graph->left != NULL){
            intersection->instruction = graph->left->instruction;
        }
        else if(graph->straight != NULL){
            intersection->instruction = graph->straight->instruction;
        }
        else if(graph->right != NULL){
            intersection->instruction = graph->right->instruction;
        }
        else
            cout << "Warning, no instruction was copied to the intersections list" << endl;
    }
    
    cout << "graph -> left:" << graph->left << ":" << endl;
    cout << "graph -> straight:" << graph->straight << ":" << endl;
    cout << "graph -> right:" << graph->right << ":" << endl << endl;
    
    intersection->data = graph->data;
    intersection->directionCount = graph->directionCount;
    intersection->directionsLeft = graph->directionsLeft;
    intersection->isIntersection = graph->isIntersection;
    intersection->distanceFromIntersection = graph->distanceFromIntersection;
    intersection->left = graph->left;
    intersection->right = graph->right;
    intersection->straight = graph->straight;
    intersection->previous = graph->previous;
    
    cout << "intersection -> left:" << intersection->left << ":" << endl;
    cout << "intersection -> straight:" << intersection->straight << ":" << endl;
    cout << "intersection -> right:" << intersection->right << ":" << endl << endl;
}

/**
* @name addToDirections
* @brief Adds a chosen direction to the vector of directions
* @param directions Type: vector<string>, vector that holds the directions traveled so far.
8 @param intersections Type: string, the chosen next direction of travel.
* @return void.
**/
void addToDirections(vector<string> &directions, string chosenDirection){
    directions.push_back(chosenDirection);
}

/**
* @name printDirections
* @brief Prints the directions traveled so far
* @param directions Type: vector<string>, vector that holds the directions traveled so far.
* @return void.
**/
void printDirections(vector<string> directions){
    cout << endl;
    cout << "Directions List:" << endl;
    for(int i=0; i < directions.size(); i++){
        cout << directions[i] << endl;
    }
}

/**
* @name numDirectionsCount
* @brief Counts the number of directions present in the input line.
* @param line Type: string, input string of possible directions.
* @param space1loc Type: int location of the first space, default input is -1
* @param space2loc Type: int location of the second space, default input is -1
* @return int, the number of directions of travel possible.
**/
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

/**
* @name addToBacktracking
* @brief Adds the direction traveled to backtracking (the opposite is entered).
* @param backtracking Type: stack<string>, stack of directions traveled for backtracking purposes.
* @param chosenDirection Type: string, the direction of travel chosen.
* @return void.
**/
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

/**
* @name printBacktracking
* @brief Prints the list of directions to return to start from the current node.
* @param backtracking Type: stack<string>, stack of directions traveled for backtracking purposes.
* @return void.
**/
void printBacktracking(stack<string> backtracking){
    cout << endl;
    cout << "Backtracking List:" << endl;
    while (!backtracking.empty()) {
        cout << backtracking.top() << endl;
        backtracking.pop();
       }
}

/**
* @name backtrack
* @brief Backtracks the program to a previous intersection.
* @param root Type: Node, the root node of the node tree.
* @param cursor Type: Node, the current location of the backtracking function.
* @param backtracking Type: stack<string>, stack of directions traveled for backtracking purposes.
* @param directions Type: vector<string> the directions possible.
* @param distanceFromIntersection Type: int, the number of nodes away from the previous intersection.
* @return void.
**/
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

/**
* @name isIntersection
* @brief Determines if a node is an intersection
* @param root Type: Node, the node to be examined.
* @return Type: bool, True if the node has more than one direction of travel possible, false if one or fewer.
**/
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

/**
* @name moveThroughMaze
* @brief Moves through the maze, utilizing other functions to backtrack and find paths.
* @param graph Type: Node, the root of the main graph of the maze.
* @param placeholder Type: Node, holds the place of the program currently
* @param backtracking Type: stack<string>, stack of directions traveled for backtracking purposes.
* @param intersections Type: stack<Node> stack of nodes that contains all of the intersections in the graph.
* @param directions Type: vector<string> vector that holds directions gone so far.
* @return void.
**/
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
//            backtrack(graph, cursor, backtracking, directions, intersections.top()->distanceFromIntersection);
//            chosenDirection = chooseDirection(intersections.top()->instruction, space1loc, numDirections, intersections->top());
//            //            addToDirections(chosenDirection); still moving somewhere else
//            while(chosenDirection == "AGAIN"){
//                intersections.pop();//pops the last intersection from the list
//                backtrack(graph, cursor, backtracking, directions, intersections.top()->distanceFromIntersection);
//                chosenDirection = chooseDirection(intersections.top()->instruction, space1loc, numDirections);//checks again to see if any directions are available
//                //                addToDirections(chosenDirection);still moving somewhere else
            numDirections = numDirectionsCount(intersections.top()->instruction, space1loc, space2loc);
            //before doing anything else, I need to:
            //- copy the node at the top of the stack
            //- pop it from the stack
            //- take that copy's link to the next node, and THAT NODE's previous
            //- new intersection node
            //- copyNode(add()) at that spot, but in the new direction.
            //- push that intersection to the stack now with its old links and the new ones
            //the lists are now up to date
            
            //what is this for??? does it do anything other than put a node in and pop it?
            Node* topOfTheStack = NULL;
            copyNode(topOfTheStack, intersections.top());
            intersections.pop();
            Node* newIntersection = NULL;
            
            addToDirections(directions, chosenDirection);
            copyNode(add(graph, placeholder, chosenDirection, line, directions), newIntersection);//copies the new node with the info from the intersection
            intersections.push(newIntersection);//pushes that new intersection onto the stack
            intersections.top()->distanceFromIntersection++;//increments the distance from the intersection by 1
            
            
            
            addToBacktracking(backtracking, chosenDirection);//adds to the backtracking list
            intersections.top()->distanceFromIntersection++;
        }
        else{
            if (numDirections > 1){
//                cout << "here1" << endl;
                Node* newIntersection = NULL;
                addToDirections(directions, chosenDirection);
//                cout << "here2" << endl;
                copyNode(add(graph, placeholder, chosenDirection, line, directions), newIntersection);//copies the new node with the info from the intersection
//                cout << "here3" << endl;
                intersections.push(newIntersection);//pushes that new intersection onto the stack
//                cout << "here4" << endl;
                //this function still has issues incrementing the distance
                intersections.top()->distanceFromIntersection++;//increments the distance from the intersection by 1
            }
            else{
                addToDirections(directions, chosenDirection);
                add(graph, placeholder, chosenDirection, line, directions);
            }
            addToBacktracking(backtracking, chosenDirection);//adds to the backtracking list

//            cout << "intersections.top->distancefromintersection:" << intersections.top()->distanceFromIntersection++ << ":" << endl;
            
//            Not working right now. Doesn't know what to do when there's no intersection
            if(intersections.top() != NULL){
                intersections.top()->distanceFromIntersection++;
            }
        }
        
    }
}

/**
* @name printIntersections
* @brief Prints the list of intersections in the maze that have been encountered.
* @param graph Type: Node, the root of the main graph of the maze.
* @param intersections Type: stack<Node> stack of nodes that contains all of the intersections in the graph.
* @return void.
**/
void printIntersections(Node* graph, stack<Node*> intersections){
    cout << endl;
    cout << "Intersections List:" << endl;
    int size = intersections.size();
    for (int i=0; i < size-1; i++) {
        cout << "intersection -> left:" << intersections.top()->left << ":" << endl;
        cout << "intersection -> straight:" << intersections.top()->straight << ":" << endl;
        cout << "intersection -> right:" << intersections.top()->right << ":" << endl << endl;
        intersections.pop();
       }
}

/**
* @name printMaze
* @brief Prints the the maze graphically, as it has been seen so far.
* @param root Type: Node, the root of the main node tree.
* @param directions Type: vector<string> the directions encountered so far.
* @param maze Type vector<vector<sting>> 2d vector that makes up a representation of the maze, in a coordinate system of sorts.
* @return void.
**/
void printMaze(Node * root, vector<string> &directions, vector <vector <string> > &maze, int i, int j){
    maze[i][j] = "  X  ";
    
    if(root->left != NULL){
        printMaze(root->left, directions, maze, i, j-1);
    }
    if (root->straight != NULL){
        printMaze(root->straight, directions, maze, i+1, j);
    }
    if (root->right != NULL){
        printMaze(root->right, directions, maze, i, j+1);
    }
}

/**
* @name startMenu
* @brief Prints the the start menu and sets up the initial operation.
* @param root Type: Node, the root node of the tree
* @return void.
**/
void startMenu(Node * root){
    string userIn;
    string mazeName;
    string mazeFileName;
    ifstream instream;
    ofstream oustream;

    cout << "Maze Mapper / Solver" << endl << "Enter Letter to Select" << endl;
    cout << "N: New Maze" << endl << "C: Continue Maze" << endl << "Q: Quit" << endl;

    cin >> userIn;
//    userIn = toupper(userIn);
    
    vector <char> extras;
    
    
    
    if(userIn == "N"){
        cout << endl << "New Maze Selected" << endl;
        cout << "Enter Maze Name: ";
        cin.ignore();
//        cin.ignore(numeric_limits<std::streamsize>::max());
        getline(cin, mazeName, '\n');

        cout << "Start a new maze named " << mazeName << "?" << endl;
        cout << "Yes = Y, No = N" << endl;
        cin >> userIn;
//        userIn = toupper(userIn);
        if(userIn == "Y"){
            cout << "Creating new maze named: "  << mazeName << "." << endl;
        }
        else{
            cout << "Cancelling..." << endl;
        }
    }
    else if(userIn == "C"){
        cout << "Continuing from previous save, enter maze name: ";
        getline(cin, mazeName);
        cout << "Continue from maze named  " << mazeName << " ?" << endl;
        cout << "Yes = Y, No = N" << endl;
        cin >> userIn;
//        userIn = toupper(userIn);

        if(userIn == "Y"){
            getline(cin, mazeName);
            cout << "Continue from maze named  " << mazeName << " ?" << endl;
            cout << "Yes = Y, No = N" << endl;
            cin >> userIn;
//            userIn = toupper(userIn);
        }
        else{
            cout << "Cancelling..." << endl;
        }
    }
    else{
        exit(0);
    }
    
    
//    switch (userIn){
//        ///Case for new maze
//        case "N":
//            cout << endl << "New Maze Selected" << endl;
//            cout << "Enter Maze Name: ";
//            cin.ignore(numeric_limits<std::streamsize>::max());
//            getline(cin, mazeName, '\n');
//
//            cout << "Start a new maze named  " << mazeName << " ?" << endl;
//            cout << "Yes = Y, No = N";
//            cin >> userIn;
//            userIn = toupper(userIn);
//            switch (userIn){
//                case "Y":
//                    cout << "Creating new maze named: "  << mazeName << "." << endl;
//                    ///Insert making a save file here, just name it like <mazeName>.txt or something
//                    ///Make sure to do error handling too.
//                    root = NULL;
//                    cout << "New maze named " << mazeName << " created!" << endl;
//                break;
//
//        case "N":
//            cout << "Canceling..." << endl;
//        break;
//    }
//        break;
//
//        ///Case for loading from previous file
//        case "C":
//        cout << "Continuing from previous save, enter maze name: ";
//        getline(cin, mazeName);
//        cout << "Continue from maze named  " << mazeName << " ?" << endl;
//            cout << "Yes = Y, No = N";
//            cin >> userIn;
//            userIn = toupper(userIn);
//
//            switch (userIn){
//                case "Y":
//                    cout << "Opening maze named: "  << mazeName << "." << endl;
//                    mazeFileName = mazeName + ".txt";
//                    instream.open(mazeFileName);
//                    if(instream.fail()){
//                        cout << "Could not find maze named " << mazeName << "." << endl;
//                    }
//                    else{
//                        cout << "Maze named " << mazeName << " opened!" << endl;
//                    }
//                    ///Insert code for reading input here, possibly a function?
//                break;
//
//                case "N":
//                    cout << "Canceling..." << endl;
//                break;
//
//        break;
//
//        case "Q":
//            exit(0);
//        break;
//    }
//    return;
//}
}

/**
* @name copyFromFile
* @brief updates the list from a save file of directions
* @param root Type: node pointer, the root of the tree
* @param directions Type: vector<string>, directions, the list of directions from the file
* @return void.
**/
void copyFromFile(Node * root, vector<string> directions){
    //need main to copy directions list before this function starts

    Node* empty = NULL;//empty node to point to
    vector <string> directionsNew;//new directions list being built from the old one

    for(int i=0; i < directions.size(); i++){
        directionsNew.push_back(directions[i]);//copies over one item from the directions list to build a new one
        string direction = directions[0];
        add(root, empty, direction, direction, directionsNew);//adds a node to the tree
    }
}
