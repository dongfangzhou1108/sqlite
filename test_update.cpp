#include <bits/stdc++.h>
#include <sqlite3.h>
#include "common.h"
#include "tic_toc.h"

using namespace std;

int main(int argc, char** argv){
    ifstream dat_file(dat_path, ios::in | ios::binary);
    if(!dat_file) {
        cerr << "Error: File path is wrong" <<endl;
        return 0;
    }
    cout << "Open file success" << endl;

    TicToc t_read_dat; // cost 8 ms
    dat_file.seekg(0,std::ios::end);
    int file_size = dat_file.tellg();
    dat_file.seekg(0,std::ios::beg);
    char *dat_buf = new char[(int)(file_size/10)];		
    dat_file.read(dat_buf,(int)(file_size/10));
    cout << "Read .dat file cost: " << t_read_dat.toc() << " ms" << endl;

    sqlite3 *sql_db = NULL;                              
    int iRet = sqlite3_open(db_update.c_str(), &sql_db);

    // 10MB/1MB 1100ms; 100KB 70ms; 10KB/1KB 50ms
    // 1KB -> 10MB 200ms-1000ms 与更新量大小相关
    TicToc t_update_db;
    sqlite3_stmt *stmt = NULL;
    iRet = sqlite3_prepare_v2(sql_db,
                                "UPDATE sd_map "
                                "SET road_dat=? "
                                "WHERE country='shanghai_1MB'",
                                -1, &stmt, NULL);
    if(iRet){
        cout << "sqlite3_prepare_v2 fail" << endl;
        return 0;
    }
    iRet = sqlite3_bind_blob(stmt, 1, dat_buf, file_size/10, SQLITE_STATIC);
    if(iRet){
        cout << "sqlite3_bind_blob fail" << endl;
        return 0;
    }
    iRet = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    cout << "Update success, cost " << t_update_db.toc() << " ms" << endl;

    sqlite3_close(sql_db);
    return 0;
}