#include "DBSystem.h"

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
}

string DBSystem::getRecord(string tableName, int recordId)
{
}

void DBSystem::insertRecord(string tableName, string record)
{
}

int main()
{
    DBSystem data;
    data.readConfig("config.txt");
    return 0;
}
