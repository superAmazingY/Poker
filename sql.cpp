#include"sql.h"

TableManager::TableManager() {
    //��ʼ��
    mysql_init(&mysql);
    //�����ַ�����
    mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
    if (mysql_real_connect(&mysql, host, user, pw, database_name, port, NULL, 0) == NULL) {
        std::cout << "����ԭ��:" << mysql_error(&mysql) << std::endl;
        std::cout << "���ݿ�����ʧ��" << std::endl;
        exit(-1);
    }
    else {
        std::cout << "���ݿ����ӳɹ�" << std::endl;
    }

}
TableManager::~TableManager() {
    mysql_close(&mysql);          //�ر���mysql������
}

double TableManager::get_data(int playercount, double preflopfirst, double preflopsecond, double preflopsamecolor) {
    double value = 0;
   
    
    vector<T_data>data_List;
    char sql[1024];
    T_data data;
    //����sql���
    sprintf(sql, "select *from  data ");
    if (mysql_query(&mysql, sql)) {
        fprintf(stderr, "Failed to selete data:Error:%s\n", mysql_error(&mysql));
        return {};
    }
    //����ѯ���Ľ�������浽res��
    MYSQL_RES* res = mysql_store_result(&mysql);
    MYSQL_ROW row;
    //��ȡÿһ�е�����
    while ((row = mysql_fetch_row(res))) {
        data.playercount = atoi(row[0]);
        data.preflopfirst = strtod(row[1], NULL);
        data.preflopsecond = strtod(row[2], NULL);
        data.preflopsamecolor = strtod(row[3], NULL);
        data.historycount = strtod(row[4], NULL);
        data.winprobability = strtod(row[5], NULL);
        data_List.push_back(data);
    }
    for (int i = 0; i < data_List.size(); i++) {
        if (playercount == data_List[i].playercount && preflopfirst == data_List[i].preflopfirst &&
            preflopsecond == data_List[i].preflopsecond && preflopsamecolor == data_List[i].preflopsamecolor) {
            value = data_List[i].winprobability;
        }
        else
        {
            value = 0;
            return value;
        }
           
    }
    return value;
}

