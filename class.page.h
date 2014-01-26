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
        //constructor to initialize the page.
        void Page(string arg_tablename, LL arg_start_index, LL arg_end_index)
        {
            tablename = arg_tablename;
            start_index = arg_start_index;
            end_index = arg_end_index;
        }
        
        //modify the info of the page after creation of page.
        void modify_info(string arg_tablename, LL arg_start_index, LL arg_end_index)
        {
            tablename = arg_tablename;
            start_index = arg_start_index;
            end_index = arg_end_index;
        }

        //will read the given file.
        //will return 1 on success and 0 othereise.
        bool read_page_file(string path_to_pagefile)
        {
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
}
