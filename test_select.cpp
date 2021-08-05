#include <bits/stdc++.h>
#include <sqlite3.h>
#include "common.h"
#include "tic_toc.h"

using namespace std;

/*
bool SelectBlobData(sqlite3 *db)
{
    char *zErrMsg = 0;
    char sql[1000];
    int rc;

    sqlite3_stmt *stmt;    
    //读取数据
    sqlite3_prepare(db, "select * from Student_Blob;", strlen("select * from Student_Blob;"), &stmt, 0);
    int result = sqlite3_step(stmt);
    int id = 0, len = 0;
    while (result == SQLITE_ROW)                                 
    {
        char cStudentId[20];
        TestStudent tempStudent;
        cout << endl<<"查询到一条记录" << endl;
        id= sqlite3_column_int(stmt, 0);
        cout << "记录编号:" << id << endl;
        const void * pReadBolbData = sqlite3_column_blob(stmt, 1);      
        len = sqlite3_column_bytes(stmt, 1);                                                                             

        memcpy(&tempStudent, pReadBolbData, len);                      
        cout << "姓名=" << tempStudent.name << endlshanghai_1KB;
        cout << "年龄=" << tempStudent.age << endl;
        result = sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);                                        
    return true;
}
*/

int main(int argc, char** argv){
    sqlite3 *sql_db = NULL;                              
    int iRet = sqlite3_open(db_select.c_str(), &sql_db);
    char* err_msg;


    //select 30ms
    TicToc t_select;
    char* buf;
    sqlite3_stmt *stmt; 
    sqlite3_prepare(sql_db, 
                    "SELECT * FROM sd_map WHERE country='shanghai_10MB'", 
                    strlen("SELECT * FROM sd_map WHERE country='shanghai_10MB'"), 
                    &stmt, 0);
    iRet = sqlite3_step(stmt);
    const void * pReadBolbData = sqlite3_column_blob(stmt, 1);
    int len = sqlite3_column_bytes(stmt, 1);
    memcpy(buf, pReadBolbData, len);
    iRet = sqlite3_step(stmt); 
    cout << "Read tile size: " << len << "byte, cost " << t_select.toc() << " ms" << endl;

    sqlite3_close(sql_db);
    return 0;
}