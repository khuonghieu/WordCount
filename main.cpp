//Name: Hieu Khuong
//TUID: 915399644
//Homework1
//CIS4307: Distributed Systems

#include <string>
#include <string.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <thread>
#include <fstream>
#include <iostream>
#include <ctime>

using namespace std;

#define MAXSTRING 512

//Add a certain word to a small map
map<string, int> addToMap(char *word, map<string, int> givenMap) {
    string str = string(word);
    map<string, int>::iterator iter = givenMap.find(str);
    //Already exist, increase count
    if (iter != givenMap.end()) {
        givenMap.at(str) += 1;
        int i = givenMap.size();
    }
        //Not exist, create one
    else {
        givenMap.insert(pair<string, int>(str, 1));
        int j = givenMap.size();
    }
    return givenMap;
}

//Start routine for threads
//Word with a certain segment and small map
void workInSegment(int section_num, char *fileName, int startLine, int endLine, map<string, int> *designatedMap) {
    char line[MAXSTRING]; /* or other suitable maximum line size */
    char *word;
    FILE *fptr = fopen(fileName, "r"); // scan thru file
    int count = 0;
    if (fptr == NULL) {
        cout << "Thread" << section_num << "cannot open file" << endl;
        exit;
    }
    while (fgets(line, sizeof(line), fptr) != NULL) {
        if (count == endLine) {
            cout << "Thread " << section_num << " reached end of segment\n";
            break;
        }
        if (count >= startLine && count < endLine) {
            if (count == startLine) {
                cout << "Thread " << section_num << " reached start of segment\n";
            }
            word = strtok(line, " *!#%&\'/\\()\"\n\r`:,.;?-”");
            while (word != NULL) {
                *designatedMap = addToMap(word, *designatedMap);
                word = strtok(NULL, " *!#%&\'/\\()\"\n\r`:,.;?-”");
            }
        }
        count++;
    }
    int size = designatedMap->size();
    cout << "Small map " << section_num << " has size " << size << endl;
}

int main(int argc, char *argv[]) {

    //Check arguments
    if (argc < 2) {
        cout << "Not enough arguments" << endl;
        return 1;
    }
    int segmentNumber = atoi(argv[2]);
    char *fileName;
    fileName = argv[1];

    cout << "\nNum Segments are " << segmentNumber << endl;
    cout << "\nFilename is " << fileName << endl;

    //Count the number of line of the text file
    int lineCount = 0;
    FILE *countLineFile = fopen(fileName, "r");
    string s;
    char readLine[MAXSTRING];
    while (fgets(readLine, sizeof(readLine), countLineFile) != NULL) {
        lineCount++;
    }
    //Array to store pointers to small maps (which stores unique words and their occurrences of each segment
    vector<map<string, int> *> mapVector(segmentNumber);
    for (int i = 0; i < segmentNumber; i++) {
        mapVector[i] = (map<std::string, int> *) (malloc(sizeof(map<string, int>)));
    }

    time_t clock = time(nullptr);
    //Divide the segments and assign 1 thread to each segment
    int rc;
    int segmentLength = lineCount / segmentNumber;
    for (int j = 0; j < segmentNumber; j++) {
        int startLine = segmentLength * j;
        int endLine = startLine + segmentLength;
        if (endLine > lineCount) {
            endLine = lineCount;
        }
        cout << "Creating thread " << j << endl;
        thread worker(workInSegment, j, fileName, startLine, endLine, mapVector[j]);
        cout << "Created thread " << j << endl;
        worker.join();
        cout << "Thread " << j << " joined" << endl;
    }

    //Make a dictionary to store all unique words and occurrences
    map<string, int> dictionary;

    //Iterate through all small maps
    for (int i = 0; i < segmentNumber; i++) {
        map<string, int>::iterator iter = (mapVector[i])->begin();
        while (iter != mapVector[i]->end()) {
            string word = iter->first;
            int occurrence = iter->second;

            //Add to dictionary
            if (dictionary.empty()) {
                dictionary.insert(pair<string, int>(word, occurrence));
            } else {
                map<string, int>::iterator search = dictionary.find(word);
                if (search != dictionary.end()) {
                    dictionary.at(word) += occurrence;
                } else {
                    dictionary.insert(pair<string, int>(word, occurrence));
                }
            }
            iter++;
        }
    }
    //Write the content of dictionary to output file
    ofstream myfile;
    myfile.open("hw1_out.txt");
    map<string, int>::iterator it = dictionary.begin();
    while (it != dictionary.end()) {
        myfile << it->first << ":" << it->second << endl;
        it++;
    }
    myfile.close();
    time_t elapsed = time(nullptr) - clock;
    cout << elapsed << " seconds passed" << endl;
    cout << "Done\n";
    return 0;
}
