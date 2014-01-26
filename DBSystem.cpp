#include "DBSystem.h"

void DBSystem::initMainMemory(){
    // Generate array of Page Objects
    MainMemory = new Page[num_pages];
    // Initialize tracking info for these.
    // MemoryMap[New PageInfo Object] = array index to which it corresponds;
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

            infile >> temp;
            if(temp.compare("END") == 0)
            {
                //break, as one table is complete
                break;
            }

            //remove last comma, if present
            if(temp[temp.size() - 1] == ',')
            {
                attr = temp.substr(0, temp.size() - 1);
            }
            else
            {
                attr = temp;
            }

            infile >> data_type;

            attr_list.push_back(make_pair(attr, data_type));
        }

        attributes[tablename] = attr_list;
    }
    infile.close();

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
        pfi.path = *v + "_PageFile_" + string(intconversionptr) + ".csv";

        temp.generate_page(*v, record_count);
        for(t=table.begin(); t!=table.end(); t++){
            
            if(temp.insert_record(*t, page_size)!=-1){
                record_count++;
            } else {

                // Set PageFile info last record
                pfi.end_record_id = record_count;
                // Write PageFile to disk
                temp.write_page_file(pfi.path);
                // Insert pfi to diskmap here
                pagefiles.push_back(pfi);
                // Increment PageFile count for table
                pagefile_count++;

                record_count++;
                temp.generate_page(*v, record_count);
                temp.insert_record(*t, page_size);

                // Start the fresh PageFileInfo stuff here:

                pfi.start_record_id = record_count;
                // This is why C++ sucks sweaty donkey balls;
                // TODO: Fix this to use sstream/ostream magic.
                    char intconversionptr[16];
                    sprintf(intconversionptr,"%d", pagefile_count);
                // Damn you C++, just give me a to_string() already.
                pfi.path = *v + "_PageFile_" + string(intconversionptr) + ".csv";


            }
        }

        // One Last flush to clear any residual record/pagefile

        // Set PageFile info last record
        pfi.end_record_id = record_count;
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
}

void DBSystem::insertRecord(string tableName, string record)
{
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

int main()
{
    DBSystem data;
    data.readConfig("config.txt");
    data.initMainMemory();
    data.populateDBInfo();

    Page temp;
    temp.generate_page("countries", 1);
    temp.read_page_file("countries.csv");
    temp.write_page_file("out.csv");
    return 0;
}
