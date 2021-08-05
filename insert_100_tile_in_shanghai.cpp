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

    string str1 = "INSERT INTO sd_map(country, road_dat) ";
    string str2 = "VALUES('shanghai_1KB___', ?)";
    vector<string> command;
    for(int i=0; i<10; i++){
        str2[21] = to_string(i)[0];
        for(int j=0; j<10; j++){
            str2[22] = to_string(j)[0]; 
            string str3 = str1 + str2;
            command.push_back(str3);
        }
    }

    TicToc t_open_db; // cost 0.12 ms
    sqlite3 *sql_db = NULL;                              
    int iRet = sqlite3_open(db_shanghai.c_str(), &sql_db);
    if (iRet){
        cout << "Database open failure, reason is: " << sqlite3_errmsg(sql_db) << endl;
        return 0;
    }else
        cout << "Open database cost: " << t_open_db.toc() << " ms" << endl;
    char* sql;
    char* err_msg;

    // cost 20s 稳定性差 跳变
    /*
    TicToc t_insert_100_tile;
    sqlite3_stmt *stmt = NULL;
    for(int i=0; i<command.size(); i++){
        TicToc one_insert;
        iRet = sqlite3_prepare_v2(sql_db,
                                command[i].c_str(),
                                -1, &stmt, NULL);
        iRet = sqlite3_bind_blob(stmt, 1, dat_buf, file_size/100, SQLITE_STATIC);
        iRet = sqlite3_step(stmt);
        cout << "One insert cost " << one_insert.toc() << " ms" << endl;
    }
    sqlite3_finalize(stmt);
    cout << "Insert 100 tiles cost " << t_insert_100_tile.toc() << " ms" << endl;
    */

    // use stmt cost 20s once 100-200ms 稳定性能好 存在少量跳变现象
    // 策略 不循环效果好
    sqlite3_exec(sql_db, "BEGIN TRANSACTION", NULL, NULL, NULL);
	sqlite3_stmt* stmt = NULL;
	sqlite3_prepare_v2(sql_db, 
        "INSERT INTO sd_map(country, road_dat) VALUES(?, ?)", 
        -1, &stmt, NULL); //跳过sql语句的解析编译，速度变快
    string tmp = "shanghai_100KB_trans_";
    TicToc t_insert_100_tile;
	for (int i = 0; i < 10; i++){
        tmp[13] = to_string(i)[0];
        for(int j = 0; j < 10; j++){
            tmp[14] = to_string(j)[0];
		    sqlite3_bind_text(stmt, 1, tmp.c_str(), strlen(tmp.c_str()),NULL);
            sqlite3_bind_blob(stmt, 2, dat_buf, file_size/100, SQLITE_STATIC);
		    sqlite3_step(stmt);
		    sqlite3_reset(stmt);
        }
	}
    cout << "Insert 100 tiles cost " << t_insert_100_tile.toc() << " ms" << endl;
	sqlite3_finalize(stmt);
    sqlite3_exec(sql_db, "END TRANSACTION", NULL, NULL, NULL);
    

    sqlite3_close(sql_db);
    return 0;
}