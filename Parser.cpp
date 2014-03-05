void toUpperCase(string s){

    // Convert string(ascii) to UPPER CASE

    for(string::iterator i=s.begin(); i!=s.end(); i++){
        if(*i<123 && *i>96){
            *i-=32;
        }
    }
    return;
}

void toLowerCase(string s){
    // Convert a string(ascii) to *lower case*

    for(string::iterator i=s.begin(); i!=s.end(); i++){
        if(*i<91 && *i>64){
            *i+=32;
        }
    }
    return;
}

void DBSystem::queryType(string query) {
    /*
     * Deteremine the type of the query (select/create) and
     * invoke appropriate method for it.
     * */

    ifstream myfile;
    string line;
    myfile.open("temp_file_to_parse");
    getline(myfile, line);
    cout<< line<<endl;
    stringstream ss(line);
    string word;
    while(getline(ss, word, ' '));
    cout << word;

    if(word == "t_gsp_deleteStatement")
        // call delete function
    if(word == "t_gsp_updateStatement");
        // call delete function
    if(word == "t_gsp_insertStatement");
    if(word == "t_gsp_selectStatement");

    //while(true){
    //    //myfile >> line;
    //    getline(myfile, line);
    //    if(myfile.eof()) break;
    //    cout << "Output by me - " << line << endl;
    //}

    cout << "query end\n\n";
    return;

}
void DBSystem::createCommand(string query) {
    /*
     * Use any SQL parser to parse the input query. Check if the table doesn't exists
     * and execute the query.
     * The execution of the query creates two files : <tablename>.data and
     * <tablename>.csv. An entry should be made in the system config file used
     * in previous deliverable.
     * Print the query tokens as specified at the end.
     * **format for the file is given below
     * */
}
void DBSystem::selectCommand(string query) {
    /*
     * Use any SQL parser to parse the input query. Perform all validations (table
     * name, attributes, datatypes, operations). Print the query tokens as specified
     * below.
     * */
}
