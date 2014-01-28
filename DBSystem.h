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
#include <sys/stat.h>
#include <climits>

using namespace std;
typedef long long int LL;

vector< vector<string> > ParseCSV(string csvFilePath);
vector<string> tokenize(string line);
string strip_quotes(string input);

class Page
{
    private:

    public:
        //meta info of the table
        string tablename;
        int start_index;
        int end_index;
        int size;
        int LRU_age;

        Page(){
            tablename = "__none__";
            start_index = -1;
            end_index = -1;
            LRU_age = 0;
            size = 0;
        }

        //vector of the records.
        vector< vector < string > > records;
        //Using default constructor to initialize the page.
        // Use generate_page
        
        //modify the info of the page after creation of page.
        void generate_page(string arg_tablename, int arg_start_index)
        {
            // Clears any existing data and updates internal structures according
            // to the new page.

            //update metadata
            tablename = arg_tablename;
            start_index = arg_start_index;
            end_index = start_index;
            size = 0;

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
                    //uncomment this line if want to strip quotes
                    //string temp = strip_quotes(csv_records[i][j]);
                    //
                    //comment this line if dont want to strip quotes
                    string temp = csv_records[i][j];
                    tmp_row.push_back(temp);
                    //+1 for comma, if there, dunno what to add ??
                    size += temp.size();
                }
                records.push_back(tmp_row);
                end_index++;
            }
            end_index--;
            return true;
        }

        //will write to page_file.
        //will return 1 on success and 0 othereise.
        bool write_page_file(string path_to_pagefile)
        {
            ofstream outfile;
            //read config file
            outfile.open(path_to_pagefile.c_str());
            //abort if config file can't be opened
            if(!outfile.is_open())
            {
                perror("Error while opening file...\nAborting!!");
                return false;
            }

            for(int i=0; i<records.size(); i++)
            {
                for(int j=0; j<records[i].size(); j++)
                {
                    if(j!=0)
                        outfile << ",";
                    outfile << records[i][j];
                }
                outfile << "\n";
            }

            return true;
        }

        //will return the entire record in a vector.
        vector<string> get_record(int record_id)
        {
            vector<string> row;

            if(record_id >= start_index && record_id <= end_index)
            {
                int idx = record_id - start_index;
                row = records[idx];
            }

            return row;
        }

        //will return -1 on failure. Else index where new record is stored.
        int insert_record(vector<string> row, int page_size)
        {
            int new_size = 0;
            for(int i=0; i<row.size(); i++)
                new_size += row[i].size() + 1; // +1 for comma and for newline if its last record
            new_size--; //dont count newline as of now, see if its possible to insert it.
            if(size + new_size <= page_size)
            {
                //comment this part if dont want to deal with newline.
                if(size + new_size < page_size) // check if there's space for a newline also
                {
                    new_size++;
                }
                //comment till here

                records.push_back(row);
                end_index++;
                size += new_size;
                //-1 because of inclusive indexing.
                return end_index-1;
            }
            return -1;
        }
};

class DBSystem
{
    private:
        int page_size;
        int num_pages;
        int LRU_timer;
        string path;
        string pagefilepath;

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
        };

        map<int, PageInfo> MemoryMap;
        map<string, vector<PageFileInfo> > DiskMap;

        int checkRecordInMemory(string tablename, int recordID);
        int getRecordIntoMemory(string tablename, int recordID);
        PageFileInfo searchPageFile(string tablename, int recordID);

    public:
        Page* MainMemory;

        DBSystem();
        void initMainMemory();
        void readConfig(string str);
        void populateDBInfo();
        string getRecord(string tableName, int recordId);
        void insertRecord(string tableName, string record);
};

#endif

