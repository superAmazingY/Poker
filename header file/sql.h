#pragma once
#include <iostream>
#include<vector>
#include<string>
#include<mysql.h>
using namespace std;

typedef struct T_data
{
    int playercount;
    double preflopfirst;
    double preflopsecond;
    double preflopsamecolor;
    double historycount;
    double winprobability;

}T_data;
class TableManager {
    TableManager();
    ~TableManager();
public:

    static TableManager* GetInstance()//����ģʽ����Ŀȷ��ֻ��һ��ʵ�壩
    {
        static TableManager  TableManager;
        return &TableManager;
    }


    //���
    static double get_data(int playercount, double preflopfirst, double preflopsecond, double preflopsamecolor);

private:
    static MYSQL mysql;//���ݿ���
    const char* host = "127.0.0.1";
    const char* user = "root";
    const char* pw = "2318bsx520";
    const char* database_name = "game_db";
    const int port = 3306;
};

