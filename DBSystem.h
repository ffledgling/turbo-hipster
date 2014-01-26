#ifndef DBSystem_H
#define DBSystem_H
#include <string>

using namespace std;
typedef long long int LL;

class DBSystem
{
        private:
            long long int page_size;
            long long int num_pages;
            string path;
            vector < string > tables;
            map < string, vector < pair<string, string> > > attributes;

	public:
            void readConfig(string str);
            void populateDBInfo();
            string getRecord(string tableName, int recordId);
            void insertRecord(string tableName, string record);
};

#endif
