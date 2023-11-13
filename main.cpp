#include <iostream>
#include <string>
#include <fstream> 
#include "bmh.cpp"
#include "rbt.cpp"
using namespace std;

int main(int argc, char *argv[]){
    RedBlackTree rbt;

    //Variables for taking in input from file and outputting to file
    ifstream inputFile(argv[1]);
    string line;
    string inputFileName = argv[1];
    inputFileName = inputFileName.substr(0, inputFileName.length()-4);
    ofstream out(inputFileName + "_output_file.txt");

    //Go through each line, parse instructions and parameters and execute
    while (getline(inputFile, line)) {
        if(line.substr(0,6) == "Insert"){
            line = line.substr(11);
            int bookID = stoi(line.substr(0, line.find(',')));
            line = line.substr(line.find('"'));
            string title = line.substr(1, line.find(',') - 2);
            line = line.substr(line.find(',') + 2);
            string author = line.substr(1, line.find(',') - 2);
            line = line.substr(line.find(',') + 2);
            string availability = line.substr(1, line.find(')') - 2);

            rbt.InsertBook(bookID, title, author, availability);
        }
        else if(line.substr(0,10) == "PrintBook("){
            line = line.substr(10);
            int bookID = stoi(line.substr(0, line.find(')')));

            rbt.PrintBook(bookID, out);
        }
        else if(line.substr(0,10) == "PrintBooks"){
            line = line.substr(11);
            int bookID1 = stoi(line.substr(0, line.find(',')));
            line = line.substr(line.find(','));
            int bookID2 = stoi(line.substr(1, line.find(')')));

            rbt.PrintBooks(bookID1, bookID2, out);
        }
        else if(line.substr(0,6) == "Borrow"){
            line = line.substr(11);
            int patronID = stoi(line.substr(0, line.find(',')));
            line = line.substr(line.find(','));
            int bookID = stoi(line.substr(1, line.find(',') - 1));
            line = line.substr(1);
            line = line.substr(line.find(','));
            int priority = stoi(line.substr(1, line.find(')' - 1)));

            rbt.BorrowBook(patronID, bookID, priority, out);
        }
        else if(line.substr(0,6) == "Return"){
            line = line.substr(11);
            int patronID = stoi(line.substr(0, line.find(',')));
            line = line.substr(line.find(','));
            int bookID = stoi(line.substr(1, line.find(')' - 1)));
            
            rbt.ReturnBook(patronID, bookID, out);
        }
        else if(line.substr(0,15) == "FindClosestBook"){
            line = line.substr(16);
            int bookID = stoi(line.substr(0, line.find(')')));

            rbt.FindClosestBook(bookID, out);
        }
        else if(line.substr(0,14) == "ColorFlipCount"){
            line = line.substr(15);

            rbt.ColorFlipCount(out);
        }
        else if(line.substr(0,6) == "Delete"){
            line = line.substr(11);
            int bookID = stoi(line.substr(0, line.find(')')));

            out << "Book " << bookID << " is no longer available.";
            if(rbt.searchNode(bookID)->bmh.getSize() > 1){
                out << " Reservations made by Patrons " << rbt.searchNode(bookID)->bmh.getContents() << " have been cancelled!";
            }
            else if(rbt.searchNode(bookID)->bmh.getSize() == 1){
                out << " Reservation made by Patron " << rbt.searchNode(bookID)->bmh.getContents() << " has been cancelled!";
            }
            out << endl << endl;
            
            rbt.DeleteBook(bookID);
        }
        else if(line.substr(0,4) == "Quit"){
            out << "Program Terminated!!";
            return 0;
        }
    }
    return 0;
}