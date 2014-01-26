#ifndef DBSystem_H
#define DBSystem_H
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib>


using namespace std;
typedef long long int LL;

class DBSystem
{
    private:
        LL page_size;
        LL num_pages;
        string path;
        vector < string > tables;
        map < string, vector < pair < string, string > > > attributes;
        
        vector< vector<string> > ParseCSV(string csvFilePath);

    public:
        void readConfig(string str);
        void populateDBInfo();
        string getRecord(string tableName, int recordId);
        void insertRecord(string tableName, string record);
};

#endif
