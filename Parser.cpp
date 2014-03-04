void DBSystem::queryType(string query) {
    /*
     * Deteremine the type of the query (select/create) and
     * invoke appropriate method for it.
     * */
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
