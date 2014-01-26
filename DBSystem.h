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

vector< vector<string> > ParseCSV(string csvFilePath);
string strip_quotes(string input);

class Page
{
    private:
        //meta info of the table
        string tablename;
        LL start_index;
        LL end_index;

        //vector of the records.
        vector< vector < string > > records;

    public:
        //Using default constructor to initialize the page.
        // Use generate_page
        
        //modify the info of the page after creation of page.
        void generate_page(string arg_tablename, LL arg_start_index)
        {
            // Clears any existing data and updates internal structures according
            // to the new page.

            //update metadata
            tablename = arg_tablename;
            start_index = arg_start_index;
            end_index = start_index;

            //clear records vector
            records.clear();
        }

        //will read the given file.
        //will return 1 on success and 0 othereise.
        bool read_page_file(string path_to_pagefile)
        {
            //get records from csv, quotes may be present
            vector < vector < string > > csv_records;
            csv_records = ParseCSV(path_to_pagefile);

            int len = csv_records.size();

            for(int i = 0; i < len; i++)
            {
                vector< string > tmp_row;
                int record_len = csv_records[i].size();
                for(int j = 0; j < record_len; j++)
                {
                    string temp = strip_quotes(csv_records[i][j]);
                    tmp_row.push_back(temp);
                }
                records.push_back(tmp_row);
            }
            return true;
        }

        //will write to page_file.
        //will return 1 on success and 0 othereise.
        bool write_page_file(string path_to_pagefile)
        {
        }

        //will return the entire record in a vector.
        vector<string> get_record(LL record_id)
        {
        }

        //will return -1 on failure. Else index where new record is stored.
        LL insert_record(vector<string>)
        {
        }
};

class DBSystem
{
    private:
        LL page_size;
        LL num_pages;
        string path;
        vector < string > tables;
        map < string, vector < pair < string, string > > > attributes;

        // Struct for page info
        class PageInfo{
            public:
                string tablename;
                string page_file_path;
                int start_record_id;
                int end_record_id;
                int LRU_age;

                PageInfo(){
                    tablename = "__none__";
                    page_file_path = "__none__";
                    start_record_id = -1;
                    end_record_id = -1;
                    LRU_age = 0;
                }
        };

        class PageFileInfo{
            public:
                string path;
                int start_record_id;
                int end_record_id;
            PageFileInfo(){
                path = "__none__";
                start_record_id = -1;
                end_record_id = -1;
            }
        }

        map<PageInfo, int> MemoryMap;
        map<string, vector<PageFileInfo> > DiskMap;

        Page* MainMemory;

    public:
        void initMainMemory();
        void readConfig(string str);
        void populateDBInfo();
        string getRecord(string tableName, int recordId);
        void insertRecord(string tableName, string record);
};

#endif

