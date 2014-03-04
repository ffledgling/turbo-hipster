#include "DBSystem.h"
#include "Parser.cpp"

DBSystem::DBSystem()
{
    MainMemoryInited = false;
    LRU_timer = 0;

    // Create Dir for pagefiles
    pagefilepath = "PageFiles/";
    mkdir(pagefilepath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void DBSystem::initMainMemory()
{
    // Generate array of Page Objects
    MainMemory = new Page[num_pages];
    // Initialize tracking info for these.
    // MemoryMap[New PageInfo Object] = array index to which it corresponds;
}

int DBSystem::checkRecordInMemory(string tablename, int recordID)
{
    // If record in Memory, return Page Number i.e index for MainMemory[].
    // else return -1

    for(int i=0; i<num_pages; i++){
        if( MainMemory[i].tablename == tablename \
                && MainMemory[i].start_index <= recordID \
                && MainMemory[i].end_index >= recordID) return i;
    }
    return -1;
}

int DBSystem::getRecordIntoMemory(string tablename, int recordID)
{
    // returns Page Number i.e index of MainMemory[] incase of success
    // else returns  -1.
    //
    PageFileInfo pfi; // Page File Info.
    pfi = searchPageFile(tablename, recordID);
    if(pfi.path == "__none__"){
        cout << "No such record was found, aborting ..."<< endl;
        exit(1);
        return -1;
    }

    // Find LRU Page.
    int min_index = 0;
    for(int i=1; i<num_pages; i++){
        if(MainMemory[i].LRU_age < MainMemory[min_index].LRU_age) min_index = i;
    }
    MainMemory[min_index].generate_page(tablename, pfi.start_record_id);
    if(!MainMemory[min_index].read_page_file(pfi.path)){
        cout << "Something went wrong, could not load page... aborting!" << endl;
        exit(1);
        return -1;
    }
    return min_index;
}

DBSystem::PageFileInfo DBSystem::searchPageFile(string tablename, int recordID)
{
    vector<PageFileInfo>::iterator v;

    for(v=DiskMap[tablename].begin(); v!=DiskMap[tablename].end(); v++){
        if((*v).start_record_id <= recordID && (*v).end_record_id >= recordID){
            return (*v);
            // do something here
        }
    }
    // Return null string if no such record exists.
    return PageFileInfo();
}


void DBSystem::readConfig(string pathToConfigFile)
{
    ifstream infile;
    //read config file
    infile.open(pathToConfigFile.c_str());
    //abort if config file can't be opened
    if(!infile.is_open())
    {
        perror("Error while opening file...\nAborting!!");
        exit(0);
    }

    int pageSize, numPages;
    string dataPath, label;
    infile >> label >> pageSize;
    infile >> label >> numPages;
    infile >> label >> dataPath;

    //initialize config parameters
    page_size = pageSize;
    num_pages = numPages;
    path = dataPath;

    //read table data
    while(!infile.eof())
    {
        vector < pair < string, string > >attr_list;
        string tablename;
        string temp;
        infile >> temp;
        if(temp == "")
        {
            break;
        }

        if(temp.compare("BEGIN") != 0)
        {
            perror("Unable to find BEGIN token...\nAborting!!");
            exit(0);
        }

        //push tablename into tables vector
        infile >> tablename;
        tables.push_back(tablename);
        attr_list.clear();

        while(!infile.eof())
        {
            string attr, data_type;

            getline(infile, temp, ',');
            if(temp.compare("END") == 0)
            {
                //break, as one table is complete
                break;
            }

            attr = temp;

            infile >> data_type;

            attr_list.push_back(make_pair(attr, data_type));
        }

        attributes[tablename] = attr_list;
    }
    infile.close();

    // Init main memory here.
    initMainMemory();


}

void DBSystem::populateDBInfo()
{
    vector<string>::iterator v;
    vector< pair <string, string> >::iterator r;
    vector< vector <string> > table;
    vector< vector <string> >::iterator t;
    if(*path.rbegin() != '/'){
        path.append("/");
    }

    for(v=tables.begin(); v!=tables.end(); v++){
        // vector<string> PageFileList;
        // DiskMap[v] = PageFileList;
        table = ParseCSV(path + (*v) + ".csv");

        int record_count = 0;
        int pagefile_count = 0;
        vector<PageFileInfo> pagefiles;

        // print table name
        // cout << *v << endl;

        Page temp;
        // Start Pagefileinfo for DiskMap
        PageFileInfo pfi;
        pfi.start_record_id = record_count;

        // This is why C++ sucks sweaty donkey balls;
        // TODO: Fix this to use sstream/ostream magic.
            char intconversionptr[16];
            sprintf(intconversionptr,"%d", pagefile_count);
        // Damn you C++, just give me a to_string() already.
        pfi.path = pagefilepath + *v + "_PageFile_" + string(intconversionptr) + ".csv";

        temp.generate_page(*v, record_count);
        for(t=table.begin(); t!=table.end(); t++){
            
            if(temp.insert_record(*t, page_size)!=-1){
                record_count++;
            } else {

                //record_count--;
                // Set PageFile info last record
                pfi.end_record_id = record_count - 1;
                // Write PageFile to disk
                temp.write_page_file(pfi.path);
                // Insert pfi to diskmap here
                pagefiles.push_back(pfi);
                // Increment PageFile count for table
                pagefile_count++;

                temp.generate_page(*v, record_count);
                temp.insert_record(*t, page_size);

                // Start the fresh PageFileInfo stuff here:

                pfi.start_record_id = record_count;
                // This is why C++ sucks sweaty donkey balls;
                // TODO: Fix this to use sstream/ostream magic.
                    char intconversionptr[16];
                    sprintf(intconversionptr,"%d", pagefile_count);
                // Damn you C++, just give me a to_string() already.
                pfi.path = pagefilepath + *v + "_PageFile_" + string(intconversionptr) + ".csv";

                record_count++;

            }
        }

        // One Last flush to clear any residual record/pagefile

        // Set PageFile info last record
        pfi.end_record_id = record_count-1;
        // Write PageFile to disk
        temp.write_page_file(pfi.path);
        // Insert pfi to diskmap here
        pagefiles.push_back(pfi);

        // Flush Vector for pagefiles to DiskMap
        DiskMap[*v] = pagefiles;
        pagefiles.clear();



        // Print Attributes for the table?
        // for(r=attributes[*v].begin(); r!=attributes[*v].end(); r++){
        //     cout << (*r).first << " " << (*r).second << endl;
        // }

    }
    // Print the DiskMap after
    // vector<PageFileInfo>::iterator pfi_iter;

    // for(v=tables.begin(); v!=tables.end(); v++){

    //     for(pfi_iter=(DiskMap[*v]).begin(); pfi_iter!=(DiskMap[*v]).end(); pfi_iter++){

    //         cout << *v+" : " << (*pfi_iter).path + " " << (*pfi_iter).start_record_id
    //              << "," << (*pfi_iter).end_record_id << endl;
    //     }
    // }
}

string DBSystem::getRecord(string tableName, int recordId)
{
    vector<string> tmp_record;
    string record;

    int pgno = checkRecordInMemory(tableName, recordId);
    if(pgno!=-1){
        tmp_record = MainMemory[pgno].get_record(recordId);
        // increment LRU
        LRU_timer++;
        MainMemory[pgno].LRU_age = LRU_timer;
        cout << "HIT\n";  //<< tableName << " " << recordId << endl;
    } else {
        pgno = getRecordIntoMemory(tableName, recordId);

        tmp_record = MainMemory[pgno].get_record(recordId);
        // increment LRU
        LRU_timer++;
        MainMemory[pgno].LRU_age = LRU_timer;
        cout << "MISS " << pgno << endl;
    }
    vector<string>::iterator v;
    for(v=tmp_record.begin(); v!=tmp_record.end(); v++){
        record.append(*v);
        record.append(",");
    }
    // Do not send \n at the end of record.
    //*record.rbegin() = '\n';
    record = record.substr(0, record.size()-1);

    return record;
}

void DBSystem::insertRecord(string tableName, string record)
{
    string table;
    //get pagefile count
    int pagefile_count = DiskMap[tableName].size();

    //tokenize record
    vector<string>record_row = tokenize(record);
    
    PageFileInfo temp = *(DiskMap[tableName].rbegin());
    int pgno = checkRecordInMemory(tableName, temp.end_record_id);

    int already_in_mem = 1;
    //bring record in memory, if not present
    if(pgno == -1)
    {
        already_in_mem = 0;
        pgno = getRecordIntoMemory(tableName, temp.end_record_id);
    }
    
    //assuming page already in memory, try to insert
    if(MainMemory[pgno].insert_record(record_row, page_size) != -1)
    {
        //update record IDs in DiskMap
        temp.end_record_id++;
        *(DiskMap[tableName].rbegin()) = temp;
        
        //write to page file
        PageFileInfo pfi;
        char intconversionptr[16];
        sprintf(intconversionptr,"%d", pagefile_count - 1);
        pfi.path = pagefilepath + tableName + "_PageFile_" + string(intconversionptr) + ".csv";
        MainMemory[pgno].write_page_file(pfi.path);
        //write to original CSV file
        table = path + tableName + ".csv";
        append_to_file(table, record);

        LRU_timer++;
        MainMemory[pgno].LRU_age = LRU_timer;
        return;
    }

    else
    {
        //page not in memory, but new record cant be inserted
        if(already_in_mem == 0)
        {
            PageFileInfo pfi;
            pfi.start_record_id = temp.end_record_id + 1;

            //record can't be inserted into current page, insert into new page
            MainMemory[pgno].generate_page(tableName, temp.end_record_id + 1);
            pfi.end_record_id = MainMemory[pgno].insert_record(record_row, page_size);
            
            // This is why C++ sucks sweaty donkey balls;
            // TODO: Fix this to use sstream/ostream magic.
            char intconversionptr[16];
            sprintf(intconversionptr,"%d", pagefile_count);
            // Damn you C++, just give me a to_string() already.
            pfi.path = pagefilepath + tableName + "_PageFile_" + string(intconversionptr) + ".csv";
            MainMemory[pgno].write_page_file(pfi.path);
            //write to original CSV file
            table = path + tableName + ".csv";
            append_to_file(table, record);

            LRU_timer++;
            MainMemory[pgno].LRU_age = LRU_timer;
    
            DiskMap[tableName].push_back(pfi);
        }

        //page in memory, but new record cant be inserted
        else
        {
            PageFileInfo pfi;
            pfi.start_record_id = temp.end_record_id + 1;

            // Find LRU Page.
            int min_index = 0;
            for(int i=1; i<num_pages; i++){
                if(MainMemory[i].LRU_age < MainMemory[min_index].LRU_age) min_index = i;
            }

            pgno = min_index;

            //record can't be inserted into current page, insert into new page
            MainMemory[pgno].generate_page(tableName, temp.end_record_id + 1);
            pfi.end_record_id = MainMemory[pgno].insert_record(record_row, page_size);
            
            // TODO: Fix this to use sstream/ostream magic.
            char intconversionptr[16];
            sprintf(intconversionptr,"%d", pagefile_count);
            
            pfi.path = pagefilepath + tableName + "_PageFile_" + string(intconversionptr) + ".csv";
            MainMemory[pgno].write_page_file(pfi.path);
            //write to original CSV file
            table = path + tableName + ".csv";
            append_to_file(table, record);

            LRU_timer++;
            MainMemory[pgno].LRU_age = LRU_timer;
    
            DiskMap[tableName].push_back(pfi);
        }
    }
}

//Append record to file
void append_to_file(string filename, string record)
{
    ofstream outfile;
    //read config file
    outfile.open(filename.c_str(), ios::app);
    //abort if config file can't be opened
    if(!outfile.is_open())
    {
        perror("Error while opening file...\nAborting!!");
        return;
    }
    outfile << record;
    outfile << endl;
    return;
}

// Global CSV parser
vector< vector<string> > ParseCSV(string csvFilePath)
{
    // Construct Path from string and table name somewhere
    string line;
    string record_attr;

    ifstream csvfile;

    vector<string> record;
    vector< vector<string> > table;

    vector<string>::iterator r;
    vector< vector<string> >::iterator t;

    csvfile.open(csvFilePath.c_str());
    if(!csvfile.is_open()){
        cout << "Error, couldn't read " << csvFilePath << " aborting..."<<endl;
        exit(1);
    }

    // Read file line by line.
    while(getline(csvfile, line)){

        // Convert line to string stream
        stringstream ss(line);

        // read string stream with ',' as delimiter
        while(getline(ss, record_attr, ',')){
            record.push_back(record_attr);
        }
        // Insert record into table and clear temp storage record.
        table.push_back(record);
        record.clear();
    }

    // print records in table
    // for(t=table.begin(); t!=table.end(); t++){
    //     record = *t;
    //     for(r=record.begin(); r!=record.end(); r++){
    //         cout << *r <<"||";
    //     }
    //     cout << endl;
    // }

    csvfile.close();

    return table;
}

vector<string> tokenize(string line)
{
    vector<string>record;
    string record_attr;

    stringstream ss(line);

    // read string stream with ',' as delimiter
    while(getline(ss, record_attr, ',')){
        record.push_back(record_attr);
    }
    return record;
}

string strip_quotes(string input)
{
    int start = 0;
    int end = input.length() - 1;
    while(input[start] == ' ' || input[start] == '"')
    {
        start++;
    }

    while(input[end] == ' ' || input[end] == '"')
    {
        end--;
    }

    int size = end - start + 1;

    return input.substr(start, size);
}

/*
int main()
{
    DBSystem data;
    data.readConfig("config.txt");
    data.initMainMemory();
    data.populateDBInfo();

    for(int i=0; i<44; i++)
    {
        data.getRecord("countries", i);

        // for(int j=0; j<4; j++)
        // {
        //     cout << data.MainMemory[j].tablename << " " << data.MainMemory[j].start_index << " " << data.MainMemory[j].end_index << " " << data.MainMemory[j].LRU_age << endl;
        // }
    }
    data.insertRecord("countries", "\"23534\",\"ZZ\",\"Shadow\",\"AJS\"");
    data.insertRecord("countries", "\"23534\",\"ZZ\",\"Shadow2\",\"AJS\"");
    
    data.getRecord("countries", 45);
    data.getRecord("countries", 44);

    return 0;
}
*/
