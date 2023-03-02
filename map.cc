/********
 * Team: Maze Solver
 * map.cc: Primary code for maze solver
 * Class: CS 3560
********/

#include <fstream>
#include <iostream>
#include <string>
using namespace std;


//Nodes that let you go left, right or straight. Also can traverse the tree backwards  
struct Node{
    string data;
    Node * left;
    Node * right;
    Node * straight;
    Node * previous;
};

//Adds a node to the graph
void add(Node *& root, Node *& previousNode, string chosenDirection);

//Calcs the size of the graph
int size(Node * root);

//function to output everything mapped so far
void inorder(Node* root);

//function to choose the direction the node should travel
string chooseDirection(Node * current);

int main(int argc, char *argv[]){
//  Variables to be used in future, not working function
//    int space1loc = -1;
//    int space2loc = -1;
    string direction;

    Node* graph; //declare variable for root node for map
    graph = NULL; //set equal to null
    Node* placeholder; //declare variable
    placeholder = NULL; //set equal to null
    
    ifstream instream; //create stream variable
    instream.open("input.txt"); //open input file

    string line; //create variable for line of input
    getline(instream, line); //get that line for processing

    //Takes the input from the input file
    do //do the work
    {
        getline(instream, line);
        //Will eventually actually use the "ChooseDirection Variable, when we have that function and that string available"
        add(graph, placeholder, "Chosen Direction");
    }while(!instream.eof());
    
    cout << endl;
    inorder(graph);
    
    //Prints the size of the tree in total (plus one is since function is recursive)
    cout << endl << endl << "size: " << size(graph) + 1 << endl;
    
    instream.close(); //close the file
    
    return 0;
}

//root: the tree's initial node
//previousNode: previous node in the tree, which this is linked from
//ChosenDirection: String containing info and what direction should be selected (will be part of a future function).
void add(Node *& root, Node *& previousNode, string direction){
    if(root == NULL){
        root=new Node;
        root->previous = previousNode;
        root->data = direction;
        root->left=root->right=root->straight=root->previous=NULL;
    }
    else if (direction == "LEFT")
        add(root->left, root, direction);
    else if (direction == "STRAIGHT")
        add(root->straight, root, direction);
    else
        add(root->right, root, direction);
}

//root: the root of the tree, initial node
int size(Node * root){
    if(root == NULL) return 0;
    else
    return size(root->left) + size(root->right) + size(root->straight) + 1;
}

void inorder(Node* root){
    if(root != NULL){
        inorder(root->left);
        cout<<root->data<<endl;
        inorder(root->right);
        inorder(root->straight);
    }
}


string chooseDriection(Node * current){
    if(current != NULL){



    }
}