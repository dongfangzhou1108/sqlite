#include <bits/stdc++.h>
#include <sqlite3.h>
#include "common.h"
#include "tic_toc.h"

using namespace std;

int main(int argc, char** argv){
    sqlite3 *sql_db = NULL;                              
    int iRet = sqlite3_open(db_delet.c_str(), &sql_db);

    // 10 / 100 KB - 50ms 1MB - 80ms 10MB - 1.2s 
    TicToc t_delet;
    char* sql = "DELETE FROM sd_map WHERE country='shanghai_100KB'";
    char* err_msg;
    iRet = sqlite3_exec(sql_db, sql, NULL, 0, &err_msg);
    if(iRet)
        return 0;
    else
        cout << "Delet success, cost " << t_delet.toc() << " ms" << endl;
    sqlite3_close(sql_db);

    return 0;
}