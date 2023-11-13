#include <iostream>
#include <string>
#include "bmh.cpp"
using namespace std;

struct Node{
    int bookID;
    string bookName;
    string authorName;
    string availability;
    string borrowedBy;
    BinaryMinHeap bmh;

    Node* left;
    Node* right;
    char color;
    Node* parent;

    //Constructor for node
    Node(int bookID, string bookName, string authorName, string availability) : 
    bookID(bookID), bookName(bookName), authorName(authorName), availability(availability), borrowedBy(""), left(nullptr), right(nullptr), color('R'), parent(nullptr), bmh(BinaryMinHeap()){}
};

class RedBlackTree{
private:
    //Rotation booleans
    bool leftLeft;
    bool rightRight;
    bool leftRight;
    bool rightLeft;

    //Tracks color swaps
    int colorSwaps;
    Node* root;

    //Rotation Functions
    Node* rotateLeftInsertion(Node* node){
        Node* right = node->right;
        Node* left = right->left;

        right->left = node;
        node->right = left;
        node->parent = right;

        if (left != nullptr) left->parent = node;
        return right;
    }

    void rotateLeftDeletion(Node* node){
        Node* right = node->right;
        if (node == root) root = right;
        if (node->parent != nullptr && node == node->parent->left){
            node->parent->left = right;
        }
        else if(node->parent != nullptr){
            node->parent->right = right;
        }
        right->parent = node->parent;
        node->parent = right;

        node->right = right->left;
        if (right->left != NULL) right->left->parent = node;
        right->left = node;
    }

    Node* rotateRightInsertion(Node* node){
        Node* left = node->left;
        Node* right = left->right;

        left->right = node;
        node->left = right;
        node->parent = left;

        if (right != nullptr) right->parent = node;
        return left;
    }

    void rotateRightDeletion(Node* node){
        Node* left = node->left;
        if (node == root) root = left;
        if (node->parent != nullptr && node == node->parent->left){
            node->parent->left = left;
        } 
        else if(node->parent != nullptr){
            node->parent->right = left;
        }
    
        node->left = left->right;
        if (left->right != NULL) left->right->parent = node;
        left->right = node;
    }
    
    //Insert using recursion
    Node* insertRecursion(Node* root, int bookID, string bookName, string authorName, string availability){
        bool doubleRed = false;

        //Find where to input node and see if there are two red nodes in a row
        if(root == nullptr){
            return new Node(bookID, bookName, authorName, availability);
        }
        else if(bookID < root->bookID){
            root->left = insertRecursion(root->left, bookID, bookName, authorName, availability);
            root->left->parent = root;
            if (root != this->root && (root->color == 'R' && root->left->color == 'R')) doubleRed = true;
        }
        else{
            root->right = insertRecursion(root->right, bookID, bookName, authorName, availability);
            root->right->parent = root;
            if (root != this->root && (root->color == 'R' && root->right->color == 'R')) doubleRed = true;
        }

        //Rotate based off how unbalanced it is
        if (leftLeft){
            leftLeft = false;
            root = rotateLeftInsertion(root);
            if(root->parent != nullptr) root->parent = nullptr;
            if(root->left->color == 'B') colorSwaps++;
            if(root->color == 'R') colorSwaps++;
            root->left->color = 'R';
            root->color = 'B';
        } 
        else if (rightRight){
            rightRight = false;
            root = rotateRightInsertion(root);
            if(root->right->color == 'B') colorSwaps++;
            if(root->color == 'R') colorSwaps++;
            root->right->color = 'R';
            root->color = 'B';
            if(root->parent != nullptr) root->parent = nullptr;
        } 
        else if (rightLeft){
            rightLeft = false;
            root->right = rotateRightInsertion(root->right);
            root->right->parent = root;
            root = rotateLeftInsertion(root);
            if(root->left->color == 'B') colorSwaps++;
            if(root->color == 'R') colorSwaps++;
            root->left->color = 'R';
            root->color = 'B';
            if(root->parent != nullptr) root->parent = nullptr;
        } 
        else if (leftRight){
            leftRight = false;
            root->left = rotateLeftInsertion(root->left);
            root->left->parent = root;
            root = rotateRightInsertion(root);
            if(root->right->color == 'B') colorSwaps++;
            if(root->color == 'R') colorSwaps++;
            root->right->color = 'R';
            root->color = 'B';
            if(root->parent != nullptr) root->parent = nullptr;
        }

        //If there are two red nodes in a row
        if(doubleRed){
            //Check if right child
            if(root->parent->right == root){
                //Figure out what rotation needs to be done
                if(root->parent->left == nullptr || root->parent->left->color == 'B'){
                    if(root->left != nullptr && root->left->color == 'R') rightLeft = true;
                    else if(root->right != nullptr && root->right->color == 'R') leftLeft = true;
                }
                else{
                    if(root->parent->left->color == 'R') colorSwaps++;
                    if(root->color == 'R') colorSwaps++;
                    root->parent->left->color = 'B';
                    root->color = 'B';
                    if (root->parent != this->root){
                        if(root->parent->color == 'B') colorSwaps++;
                        root->parent->color = 'R';
                    }
                }
            }
            //Else if left child
            else{
                //Figure out what rotation needs to be done
                if (root->parent->right == nullptr || root->parent->right->color == 'B'){
                    if (root->left != nullptr && root->left->color == 'R') rightRight = true;
                    else if (root->right != nullptr && root->right->color == 'R') leftRight = true;
                } 
                else{
                    if(root->parent->right->color == 'R') colorSwaps++;
                    if(root->color == 'R') colorSwaps++;
                    root->parent->right->color = 'B';
                    root->color = 'B';
                    if (root->parent != this->root){
                        if(root->parent->color == 'B') colorSwaps++;
                        root->parent->color = 'R';
                    }
                }
            }
            doubleRed = false;
        }
        return root;
    }

    //Finds sucessor for node for deletion
    Node* findSuccessor(Node* node){
        if (node->left != nullptr && node->right != nullptr){
            Node* temp = node;
            temp = node->right;
            while (temp->left != nullptr){
                temp = temp->left;
            }
            return temp;
        }
        else if(node->left == nullptr && node->right == nullptr){
            return nullptr;
        }
        else if(node->left != nullptr){
            return node->left;
        }
        else{
            return node->right;
        }
    }

    void fixTree(Node* node){
        if(node == root) return;

        //Get sibling and parent of node
        Node* sibling;
        Node* parent = node->parent;
        if(parent == nullptr) sibling = nullptr;
        else if(node == parent->left) sibling = parent->right;
        else sibling = parent->left;

        if(sibling == nullptr){
            fixTree(parent);
        }
        else{
            //If sibling black
            if(sibling->color == 'B'){
                //If has red child
                if((sibling->left != nullptr && sibling->left->color == 'R') || (sibling->right != nullptr && sibling->right->color == 'R')){
                    //If red child is on left and sibling is a right child
                    if(sibling->left != nullptr && sibling->left->color == 'R'){
                        if(sibling == sibling->parent->right){
                            if(sibling->left->color != parent->color) colorSwaps++;
                            sibling->left->color = parent->color;
                            rotateRightDeletion(sibling);
                            rotateLeftDeletion(parent);
                        }
                        //If red child is on left and sibling is a left child
                        else{
                            if(sibling->left->color != sibling->color) colorSwaps++;
                            sibling->left->color = sibling->color;
                            if(sibling->color != parent->color) colorSwaps++;
                            sibling->color = parent->color;
                            rotateRightDeletion(parent);
                        }
                    }
                    //If red child is on right and sibling is left child
                    else{
                        if(sibling == sibling->parent->right){
                            if(sibling->right->color != sibling->color) colorSwaps++;
                            if(sibling->color != parent->color) colorSwaps++;
                            sibling->right->color = sibling->color;
                            sibling->color = parent->color;
                            rotateLeftDeletion(parent);
                        }
                        else{
                            if(sibling->right->color != parent->color) colorSwaps++;
                            sibling->right->color = parent->color;
                            rotateLeftDeletion(sibling);
                            rotateRightDeletion(parent);
                        }
                    }
                    if(parent->color == 'R') colorSwaps++;
                    parent->color = 'B';
                }
                //If no red children
                else{
                    if(sibling->color == 'B') colorSwaps++;
                    sibling->color = 'R';
                    if (parent->color == 'B') fixTree(parent);
                    else{
                        if(parent->color == 'R') colorSwaps++;
                        parent->color = 'B';
                    }
                }
            }
            //If sibling red
            else{
                if(parent->color == 'B') colorSwaps++;
                if(sibling->color == 'R') colorSwaps++;
                parent->color = 'R';
                sibling->color = 'B';
                if(sibling == parent->left) rotateRightDeletion(parent);
                else rotateLeftDeletion(parent);
                fixTree(node);
            }
        }
    }
    void deleteNode(Node* node){
        Node* successor = findSuccessor(node);
        Node* parent = node->parent;

        //Check sucessor
        //If leaf node
        if(successor == nullptr){
            if(node != root){
                //If double black, fix
                if((successor == nullptr || successor->color == 'B') && (node->color == 'B')){
                    fixTree(node);
                }
                //Either u or v is red
                else{
                    Node* sibling;
                    if(parent == nullptr) sibling = nullptr;
                    else if(node == parent->left) sibling = parent->right;
                    else sibling = parent->left;

                    if(sibling != nullptr){
                        if(sibling->color == 'B') colorSwaps++;
                        sibling->color = 'R';
                    }
                }
                //Remove node from tree
                if(node == parent->right){
                    parent->right = nullptr;
                }
                else{
                    parent->left = nullptr;
                }
            }
            else{
                root = nullptr;   
            }   
            delete node;
            return;
        }
        //If node has one children
        if(node->left == nullptr || node->right == nullptr){
            //If not root, replace with sucessor and detach node from tree
            if(node != root){
                if(node == parent->left){
                    parent->left = successor;
                }
                else{
                    parent->right = successor;
                }
                delete node;
                successor->parent = parent;
                if((successor == nullptr || successor->color == 'B') && (node->color == 'B')){
                    fixTree(successor);
                }
                else{
                    if(successor->color == 'R') colorSwaps++;
                    successor->color = 'B';
                }
            }
            //If root, replace with sucessor and delete
            else{
                node->bookID = successor->bookID;
                node->bookName = successor->bookName;
                node->authorName = successor->authorName;
                node->availability = successor->availability;
                node->borrowedBy = successor->borrowedBy;
                node->bmh = successor->bmh;

                node->left = nullptr;
                node->right = nullptr;
                delete successor;
            }
            return;
        }
        //If node has two children
        else{
            //Swap data and recursion
            int tempID = node->bookID;
            string tempTitle = node->bookName;
            string tempAuthor = node->authorName;
            string tempAvail = node->availability;
            string tempBorrowed = node->borrowedBy;
            BinaryMinHeap tempbmh = node->bmh;

            node->bookID = successor->bookID;
            node->bookName = successor->bookName;
            node->authorName = successor->authorName;
            node->availability = successor->availability;
            node->borrowedBy = successor->borrowedBy;
            node->bmh = successor->bmh;

            successor->bookID = tempID;
            successor->bookName = tempTitle;
            successor->authorName = tempAuthor;
            successor->availability = tempAvail;
            successor->borrowedBy = tempBorrowed;
            successor->bmh = tempbmh;

            deleteNode(successor);
        }
    }
public:
    RedBlackTree() : root(nullptr), leftLeft(false), rightRight(false), leftRight(false), rightLeft(false), colorSwaps(0){}

    //Returns node based of bookID
    Node* searchNode(int bookID){
        Node *temp = root;
        while (temp != nullptr){
            if (bookID < temp->bookID){
                if (temp->left == nullptr) break;
                else temp = temp->left;
            } 
            else if(bookID == temp->bookID){
                return temp;
                break;
            }
            else{
                if (temp->right == nullptr) break;
                else temp = temp->right;
            }
        }
        return nullptr;
    }

    //Inserts book into R-B Tree
    void InsertBook(int bookID, string bookName, string authorName, string availability){
        if(root == nullptr){
            root = new Node(bookID, bookName, authorName, availability);
            root->color = 'B';
        }
        else{
            root = insertRecursion(root, bookID, bookName, authorName, availability);
        }
    }

    //Prints data for book
    void PrintBook(int bookID, ofstream& out){
        Node* temp = searchNode(bookID);
        if(temp != nullptr){
            out << "BookID = " << temp->bookID << endl;
            out << "Title = \"" << temp->bookName << "\"" << endl;
            out << "Author = \"" << temp->authorName << "\"" << endl;
            out << "Availability = \"" << temp->availability << "\"" << endl;
            if(temp->borrowedBy == ""){
                out << "BorrowedBy = " << "None" << endl;
            }
            else{
                out << "BorrowedBy = " << temp->borrowedBy << endl;
            }
            out << "Reservations = [" << temp->bmh.getContents() << "]" << endl << endl;
        }
        else{
            out << "Book " << bookID << " not found in the Library" << endl << endl;
        }
    }

    //Prints data for books in range
    void PrintBooks(int bookID1, int bookID2, ofstream& out){
        for(int i = bookID1; i <= bookID2; i++){
            PrintBook(i, out);
        }
    }

    //Finds closest book to the targetID
    void FindClosestBook(int targetID, ofstream& out){
        Node* under = root;
        int underTarget = targetID;
        Node* over = root;
        int overTarget = targetID;
        bool underFound = false;
        bool overFound = false;

        //Loop until a value is found
        while(!underFound && !overFound){
            under = searchNode(underTarget);
            if(under != nullptr){
                underFound = true;
            }

            over = searchNode(overTarget);
            if(over != nullptr){
                overFound = true;
            }

            if(!overFound){ 
                overTarget++;
                over = root;
            }
            if(!underFound){
                under = root;
                underTarget--;
            }
        }

        //Prints multiple books if there is a tie and the over and under are different
        if(underFound){
            out << "BookID = " << under->bookID << endl;
            out << "Title = \"" << under->bookName << "\"" << endl;
            out << "Author = \"" << under->authorName << "\"" << endl;
            out << "Availability = \"" << under->availability << "\"" << endl;
            if(under->borrowedBy == ""){
                out << "BorrowedBy = " << "None" << endl;
            }
            else{
                out << "BorrowedBy = " << under->borrowedBy << endl;
            }
            out << "Reservations = [" << under->bmh.getContents() << "]" << endl << endl;
        }
        if(overFound && (over->bookID != under->bookID)){
            out << "BookID = " << over->bookID << endl;
            out << "Title = \"" << over->bookName << "\"" << endl;
            out << "Author = \"" << over->authorName << "\"" << endl;
            out << "Availability = \"" << over->availability << "\"" << endl;
            if(over->borrowedBy == ""){
                out << "BorrowedBy = " << "None" << endl;
            }
            else{
                out << "BorrowedBy = " << over->borrowedBy << endl;
            }
            out << "Reservations = [" << over->bmh.getContents() << "]" << endl << endl;
        }
    }

    //Deletes book from tree
    void DeleteBook(int bookID){
        if(root == nullptr) return;
        Node* node = searchNode(bookID);
        deleteNode(node);
    }

    //Borrows book to patron
    void BorrowBook(int patronID, int bookID, int patronPriority, ofstream& out){
        Node* book = searchNode(bookID);
        if(book->borrowedBy != ""){
            //Create reservation
            book->bmh.insert(patronPriority, patronID);
            out << "Book " << bookID << " Reserved by Patron " << patronID << endl << endl;
        }
        else{
            //First time borrowing book
            book->borrowedBy = to_string(patronID);
            out << "Book " << bookID << " Borrowed by Patron " << patronID << endl << endl;
            book->availability = "No";
        }
    }

    //Returns book from patron and gives to next patron using binary min heap
    void ReturnBook(int patronID, int bookID, ofstream& out){
        Node* book = searchNode(bookID);
        out << "Book " << bookID << " Returned by Patron " << patronID << endl << endl;
        if(book->bmh.getSize() >= 1){
            book->borrowedBy = to_string(book->bmh.getMin().second);
            out << "Book " << bookID << " Allotted to Patron " << book->bmh.getMin().second << endl << endl;
            book->bmh.removeMin();
        }
        if(book->bmh.getSize() == 0){
            book->availability = "Yes";
            book->borrowedBy = "None";
        }
    }

    //Outputs color flip count
    void ColorFlipCount(ofstream& out){
        out << "Color Flip Count: " << colorSwaps << endl << endl;
    }
};