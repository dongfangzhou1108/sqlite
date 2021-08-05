/*
 * @Author: your name
 * @Date: 2021-07-16 14:27:15
 * @LastEditTime: 2021-07-16 14:35:21
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /0715/src/test_insert.cpp
 */
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
    char *dat_buf = new char[(int)(file_size/100)];		
    dat_file.read(dat_buf,(int)(file_size/100));
    cout << "Read .dat file cost: " << t_read_dat.toc() << " ms" << endl;

    TicToc t_open_db; // cost 0.12 ms
    sqlite3 *sql_db = NULL;                              
    int iRet = sqlite3_open(db_insert.c_str(), &sql_db);
    if (iRet){
        cout << "Database open failure, reason is: " << sqlite3_errmsg(sql_db) << endl;
        return 0;
    }else
        cout << "Open database cost: " << t_open_db.toc() << " ms" << endl;
    char* sql;
    char* err_msg;

    // make table cost 58.8354 ms
    /*
    TicToc t_make_table;
    sql = "CREATE TABLE sd_map("
            "country CHAR(50) PRIMARY KEY,"
            "road_dat BLOB);";
    sqlite3_exec(sql_db, sql, NULL, 0, &err_msg);
    if (iRet != SQLITE_OK){
        cout << "Make table sd_map failure，reason is: " << err_msg << endl;
        sqlite3_free(err_msg);
    }else
        cout << "Make table sd_map cost: " << t_make_table.toc() << " ms" << endl;
    */

    // https://www.sqlite.org/intern-v-extern-blob.html
    // test insert 10MB -> 500 ms ; 1MB -> 60 ms ; 100KB -> 60 ms ; 10KB -> 60 ms ;1KB -> 80 ms
    TicToc t_insert_db;
    sqlite3_stmt *stmt = NULL;
    iRet = sqlite3_prepare_v2(sql_db,
                                "INSERT INTO sd_map(country, road_dat) "
                                "VALUES('shanghai_100KB_009', ?)",
                                -1, &stmt, NULL);
    iRet = sqlite3_bind_blob(stmt, 1, dat_buf, file_size/100, SQLITE_STATIC);
    iRet = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    cout << "Insert data success, cost " << t_insert_db.toc() << " ms" << endl;
 
    sqlite3_close(sql_db);
    return 0;
}