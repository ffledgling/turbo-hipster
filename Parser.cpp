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
    
    string first_word = query.substr(0, query.find_first_of(' '));
    toLowerCase(first_word);
    cout << "first word is: "<< first_word <<"\n";
    if(first_word == "select"){
        selectCommand(query);
        cout << "SELECT\n";
    } else if(first_word == "create"){
        createCommand(query);
        cout << "CREATE\n";
    } else {
        cout << "Invalid Query. \"" << query << "\" is not a valid query\n";
    }


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
