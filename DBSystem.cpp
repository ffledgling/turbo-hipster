#include "DBSystem.h"

void DBSystem::initMainMemory(){
    // Generate array of Page Objects
    new (&MainMemory) Page[num_pages];
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

    LL pageSize, numPages;
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

        if(temp.compare("BEGIN") != 0)
        {
            perror("Unable to find BEGIN toke...\nAborting!!");
            exit(0);
        }
        while(!infile.eof())
        {
            string attr, data_type;
            attr_list.clear();
            //push tablename into tables vector
            infile >> tablename;
            tables.push_back(tablename);

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
}

void DBSystem::populateDBInfo()
{
    ParseCSV("countries.csv");
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



   return table;
}

int main()
{
    cout<<"here\n";
    DBSystem data;
    cout<<"there\n";
    data.readConfig("config.txt");
    data.initMainMemory();
    data.populateDBInfo();
    return 0;
}
