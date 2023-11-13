#pragma once
#include <iostream>
#include <string>
using namespace std;

class BinaryMinHeap{
private:
    pair<int, int>* heapArr;
    int size;
public:
    //Constructor
    BinaryMinHeap(){
        heapArr = new pair<int, int>[20];
        size = 0;
    }

    //Inserts into binary min queue
    void insert(int priority, int patron){
        pair<int, int> p;
        p.first = priority;
        p.second = patron;

        if(size == 20){
            //Make it print something if full?
            return;
        }

        int currentIndex = size;
        heapArr[currentIndex] = p;
        size++;

        pair<int, int> parent = heapArr[(currentIndex-1)/2];
        //Move up if priority is less than parent's priority
        while(currentIndex != 0 && parent.first > heapArr[currentIndex].first){
            heapArr[(currentIndex-1)/2] = heapArr[currentIndex];
            heapArr[currentIndex] = parent;
            currentIndex = (currentIndex-1)/2;
        }
    }

    //Removes the minimum from the heap
    pair<int,int> removeMin(){
        if(size == 0){
            return pair<int,int> {-1,-1};
        }
        if(size == 1){
            size--;
            return heapArr[0];
        }
        
        pair<int,int> min = heapArr[0];
        heapArr[0] = heapArr[size-1];
        size--;
        heapify(0);
        return min;
    }

    //Makes sure that the heap follows the min heap structure. Moves higher priority up
    void heapify(int index){
        //Figure out which child is sucessor
        int leftIndex = 2*index + 1;
        int rightIndex = 2*index + 2;
        int smallerVal = index;

        if(leftIndex < size && heapArr[leftIndex].first < heapArr[index].first){
            smallerVal = leftIndex;
        }
        if(rightIndex < size && heapArr[rightIndex].first < heapArr[smallerVal].first){
            smallerVal = rightIndex;
        }
        if(smallerVal != index){
            //Swap then recursion
            pair<int, int> temp  = heapArr[index];
            heapArr[index] = heapArr[smallerVal];
            heapArr[smallerVal] = temp;

            heapify(smallerVal);
        }
    }

    //Returns min
    pair<int,int> getMin(){
        return heapArr[0];
    }
    
    //Returns size
    int getSize(){
        return size;
    }

    //Return all patrons in min hepap
    string getContents(){
        string result = "";
        if(size > 0){
            result += to_string(heapArr[0].second);
        }
        for(int i = 1; i < size; i++){
            result += ", " + to_string(heapArr[i].second);
        }
        return result;
    }

};