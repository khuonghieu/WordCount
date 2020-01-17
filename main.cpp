#include <iostream>
#include <thread>
#include <fstream>
#include <map>

using namespace std;


int main() {
    ifstream inFile;
    inFile.open("/home/hieukhuong/Desktop/ThreadConcurrency/ebook.txt");
    string line;
    int count = 0;
    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            count++;
        }
    } else {
        cout << "Unable to open";
    }
    inFile.close();
    cout << "There are " << count << " lines";

    map<string, int> wordDict = {};

    return 0;
}
