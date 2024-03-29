

//IPv4 地址 . . . . . . . . . . . . : 192.168.10.5
/*
    SOCKET socket_ai = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//信箱
    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;//协议版本
    std::string ipstr;
    std::cin >> ipstr; //对战平台ip
    std::string portstr = "10001";//占用端口
    std::string message;
    server_address.sin_port = htons(std::stoi(portstr));
    inet_pton(AF_INET, ipstr.c_str(), &(server_address.sin_addr));


    int m = connect(socket_ai, (struct sockaddr*)&server_address, sizeof(server_address));
    if (m ==-1 ) {
        printf("服务器连接失败!\n");
        return -1;
    }
    printf("服务器连接成功!\n");


      try {
         if (connect(socket_ai, (struct sockaddr*)&server_address, sizeof(server_address)) != 0) {
             std::cout << "连接失败！" << std::endl;
             return 0;
         }
         std::cout << "连接成功！" << std::endl;
    */

    /*
     // 发送和接收数据
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    // 发送数据
    std::string sendData = "Hello, server!";
    strncpy(buffer, sendData.c_str(), sizeof(buffer));
    send(socket_ai, buffer, sizeof(buffer), 0);

    // 接收数据
    memset(buffer, 0, sizeof(buffer));
    recv(socket_ai, buffer, sizeof(buffer), 0);
    std::string receivedData(buffer);
    std::cout << "收到的数据: " << receivedData << std::endl;
     // 关闭连接
    close(socket_ai);
} catch (std::exception& e) {
    std::cout << "发生异常: " << e.what() << std::endl;
}
    */
#include <sys/types.h>

#include <sstream>
#include <ws2tcpip.h>
#include <iostream>
#include <io.h>
#include"OnHand.h"
#include"Poker.h"
#include"Program.h"
#include"texasHoldemData.h"
#include"sql.h"
#include <windows.h>
#pragma execution_character_set("utf-8")


#pragma comment(lib,"ws2_32.lib")

std::vector<bool> Program::said;
int Program::allinflag;
int Program::oppshouldc;
int Program::earnchips;
int TexasHoldemData::iPlayerCount;
MYSQL TableManager::mysql;
HWND hWnd;
/*
std::vector<std::string> SplitString(const std::string& input, const std::string& delimiters) {
    std::vector<std::string> result;
    std::size_t startPos = 0;
    std::size_t endPos = input.find_first_of(delimiters);

    while (endPos != std::string::npos) {
        std::string token = input.substr(startPos, endPos - startPos);
        if (!token.empty()) {
            result.push_back(token);
        }
        startPos = endPos + 1;
        endPos = input.find_first_of(delimiters, startPos);
    }

    std::string lastToken = input.substr(startPos);
    if (!lastToken.empty()) {
        result.push_back(lastToken);
    }

    return result;
}
*/


vector<string> split(const string& s, const string& seperator) {
    vector<string> result;
    typedef string::size_type string_size;
    string_size i = 0;

    while (i != s.size()) {
        //找到字符串中首个不等于分隔符的字母；
        int flag = 0;
        while (i != s.size() && flag == 0) {
            flag = 1;
            for (string_size x = 0; x < seperator.size(); ++x)
                if (s[i] == seperator[x]) {
                    ++i;
                    flag = 0;
                    break;
                }
        }

        //找到又一个分隔符，将两个分隔符之间的字符串取出；
        flag = 0;
        string_size j = i;
        while (j != s.size() && flag == 0) {
            for (string_size x = 0; x < seperator.size(); ++x)
                if (s[j] == seperator[x]) {
                    flag = 1;
                    break;
                }
            if (flag == 0)
                ++j;
        }
        if (i != j) {
            result.push_back(s.substr(i, j - i));
            i = j;
        }
    }
    return result;
}




std::string& trim(std::string& s)
{
    if (s.empty())
    {
        return s;
    }

    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}
int main() {
    //初始化大小
    Program::said.resize(2, 0);
    WSADATA wsdata;
    if (WSAStartup(MAKEWORD(2, 2), &wsdata))
    {
        std::cout << "init socket failed!" << std::endl;
        WSACleanup();
        return FALSE;
    }
    //检测版本号
    if (LOBYTE(wsdata.wVersion) != 2 || HIBYTE(wsdata.wHighVersion) != 2) {
        std::cout << "Error: wsadata.wVersion != 2 ." << std::endl;
        WSACleanup();
        return -1;
    }
    
  
    
    SOCKET socket_ai;
    socket_ai = socket(PF_INET, SOCK_STREAM, 0);
    /*
    unsigned long ul = 1;
    int ret;
    ret = ioctlsocket(socket_ai, FIONBIO, (unsigned long*)&ul);
    if (ret == SOCKET_ERROR)//设置失败。

    {
        cout<< "socket is not open!" << endl;
    }
    */  
    

    if (socket_ai == INVALID_SOCKET)
    {
        std::cout << "create socket fail" << std::endl;
        WSACleanup();
        return FALSE;
    }
    //填充服务端信息
    SOCKADDR_IN server_addr;
  
        
    server_addr.sin_family = AF_INET;
    std::string ipstr;
    std::cin >> ipstr; //对战平台ip
    std::string message;
    server_addr.sin_addr.S_un.S_addr = inet_addr(ipstr.c_str());
    server_addr.sin_port = htons(10001);
    //发送连接请求 请求连接服务器
    if (connect(socket_ai, (SOCKADDR*)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
        std::cout << "Server connection failed" << std::endl;
        WSACleanup();
        return -1;
    }
    std::cout << "Server connection succeeded" << std::endl;

    //获取数据
    std::vector<int>moneys;
    for (int i = 0; i < 2; i++) {
        moneys.push_back(20000);
    }
    TexasHoldemData td(2, moneys, 100);
    int cs = 0;
    int r;
    int amount = 0;
    int iHumanActionCode;
    int iHumanAmount;
    int BIGBLIND = 100;
    int WinFlag = 0;
    int NumOfGames = 0;
    int allincount = 0;
    int shouldc = 0;
    int n = 0;

    do {
        char recvBuf[BUFSIZ];
        amount = 0;       
		memset(recvBuf, 0, sizeof(recvBuf));       
        recv(socket_ai, recvBuf, sizeof(recvBuf), 0);
        n++;
       // ret = recv(socket_ai, recvBuf, sizeof(recvBuf), 0);
       // std::cout << ret << std::endl;
        std::string receive(recvBuf);
        std::cout << receive << std::endl;
        std::vector<std::string> temp=split(receive, "| <, >" );
        if (trim(temp[0]) == "name") {
            std::wstring t = L"Constellation";
            int bufferSize = WideCharToMultiByte(CP_UTF8, 0, t.c_str(), -1, nullptr, 0, nullptr, nullptr);
            std::vector<char> buffer(bufferSize);
            WideCharToMultiByte(CP_UTF8, 0, t.c_str(), -1, buffer.data(), bufferSize, nullptr, nullptr);
            send(socket_ai, buffer.data(), bufferSize, 0);
            std::wcout << t << std::endl;
        }
        else if (temp[0] == "preflop") {
                
                if (WinFlag == 0) {
                    NumOfGames++;
                    if ((70 - NumOfGames) % 2 == 0) {
                        shouldc = ((70 - NumOfGames) / 2) * 150;
                        Program::oppshouldc = ((70 - NumOfGames) / 2) * 150;
                    }
                    else {
                        if (td.blindpos == 1) {
                            shouldc = ((69 - NumOfGames) / 2) * 150 + 50;
                            Program::oppshouldc = ((69 - NumOfGames) / 2) * 150 + 100;
                        }
                        else {
                            shouldc = ((69 - NumOfGames) / 2) * 150 + 100;
                            Program::oppshouldc = ((69 - NumOfGames) / 2) * 150 + 50;
                        }
                    }
                }
                
                
                Program::allinflag = false;
                td.Init(2, moneys);
                cs = 0;

                std::vector<Poker> porker1 = {
                        Poker(std::stoi(temp[2]), std::stoi(temp[3])),
                        Poker(std::stoi(temp[4]), std::stoi(temp[5]))
                };
                OnHand oh(porker1);
                td.onHands.push_back(oh);
                if (temp[1] == "SMALLBLIND") {
                    td.blindpos = 0;
                    td.PreflopMoneyInit(2);
                    iHumanActionCode = 3;
                    iHumanAmount = BIGBLIND;
                   // td.Bet(1, iHumanAmount);
                    if (WinFlag == 1)
                    {
                        r = 6;
                    }
                    else {
                        r = Program::think(td, cs, iHumanActionCode, iHumanAmount, amount);
                    }
                    Program::SendAction(r, cs, amount, socket_ai, td);
                    Program::said[0] = true;
                   
                }
                else
                {
                    td.blindpos = 1;
                    td.PreflopMoneyInit(2);
                }
            }
        else if (temp[0] == "flop") {
                cs = 1;
                td.CommonPorkers.emplace_back(Poker(std::stoi(temp[1]), std::stoi(temp[2])));
                td.CommonPorkers.emplace_back(Poker(std::stoi(temp[3]), std::stoi(temp[4])));
                td.CommonPorkers.emplace_back(Poker(std::stoi(temp[5]), std::stoi(temp[6])));
                if (td.blindpos == 1) {
                    iHumanActionCode = 0;
                    iHumanAmount = 0;
                    if (WinFlag == 1)
                    {
                        r = 6;
                    }
                    else {
                        r = Program::think(td, cs, iHumanActionCode, iHumanAmount, amount);
                    }
                    Program::SendAction(r, cs, amount, socket_ai, td);
                    Program::said[0] = true;
                   
                }
            }
        else if (temp[0] == "turn") {

             

                cs = 2;
                td.CommonPorkers.emplace_back(Poker(std::stoi(temp[1]), std::stoi(temp[2])));
                if (td.blindpos == 1) {
                    iHumanActionCode = 0;
                    iHumanAmount = 0;
                    if (WinFlag == 1)
                    {
                        r = 6;
                    }
                    else {
                        r = Program::think(td, cs, iHumanActionCode, iHumanAmount, amount);
                    }
                    Program::SendAction(r, cs, amount, socket_ai, td);
                    Program::said[0] = true;
                   
                }
            }
        else if (temp[0] == "river") {

                

                cs = 3;
                td.CommonPorkers.emplace_back(Poker(std::stoi(temp[1]), std::stoi(temp[2])));
                if (td.blindpos == 1) {
                    iHumanActionCode = 0;
                    iHumanAmount = 0;
                    if (WinFlag == 1) {
                        r = 6;
                    }
                    else {
                        r = Program::think(td, cs, iHumanActionCode, iHumanAmount, amount);
                    }
                    Program::SendAction(r, cs, amount, socket_ai, td);
                    Program::said[0] = true;
                    
                }
            }
        else if (temp[0] == "bet") {

            

                Program::said[1] = true;
              
                iHumanActionCode = 3;
                std::string str = temp[1];
                iHumanAmount = std::stoi(str);
                td.Bet(1, iHumanAmount);
                if (WinFlag == 1)
                {
                    r = 6;
                }
                else {
                    r = Program::think(td, cs, iHumanActionCode, iHumanAmount, amount);
                }
                Program::SendAction(r, cs, amount, socket_ai, td);
                Program::said[0] = true;
              
            }    
            //错
        else if (trim(temp[0]) == "call") {
                

                Program::said[1] = true;
                
                iHumanActionCode = 2;
                iHumanAmount = (int)td.thisTurnMoneys[0];
                td.Call(1);
                if (Program::said[0]==false)
                {         
                    if (WinFlag == 1)
                    {
                        r = 6;
                    }
                    else {
                        r = Program::think(td, cs, iHumanActionCode, iHumanAmount, amount);
                    }
                       Program::SendAction(r, cs, amount, socket_ai, td);
                    Program::said[0] = true;
                    
                }
                
            }
        else if (temp[0] == "raise") {
                iHumanActionCode = 3;
                std::string str = temp[1];
                iHumanAmount = std::stoi(str);
                if (Program::said[1]) {
                    td.Raise(1, iHumanAmount);
                }
                else if (cs == 0 && td.blindpos == 1) {
                    td.Raise(1, iHumanAmount);
                }
                else {
                    td.Bet(1, iHumanAmount);
                }
                Program::said[1] = true;
                if (WinFlag == 1)
                {
                    r = 6;
                }
                else {
                    r = Program::think(td, cs, iHumanActionCode, iHumanAmount, amount);
                }
                Program::SendAction(r, cs, amount, socket_ai, td);
                Program::said[0] = true;
              
            }
        else if (trim(temp[0]) == "allin") {
                if (Program::allinflag == 0) {
                    if (cs == 0) {
                        allincount++;
                    }
                    if (allincount >= 20) {
                        Program::allinflag = 1;
                    }
                }
                Program::said[1] = true;
                
                td.Allin(1);
                iHumanActionCode = 4;
                iHumanAmount = (int)td.thisTurnMoneys[1];
                if (Program::allinflag == false) {
                    if (WinFlag == 1)
                    {
                        r = 6;
                    }
                    else {
                        r = Program::think(td, cs, iHumanActionCode, iHumanAmount, amount);
                    }
                    Program::SendAction(r, cs, amount, socket_ai, td);
                    Program::said[0] = true;
                   
                }
            }
          //check错
        else if (trim(temp[0]) == "check") {
                Program::said[1] = true;
                iHumanActionCode = 5;
                iHumanAmount = 0;
                td.Check(1);
                if (Program::said[0]==false) {         
                    if (WinFlag == 1)
                    {
                        r = 6;
                    }
                    else
                    {
                        r = Program::think(td, cs, iHumanActionCode, iHumanAmount,  amount);
                    }
                    Program::SendAction(r, cs, amount, socket_ai, td);
                    Program::said[0] = true;
                    
                }
            }
        else if (!temp.empty() && temp[0] == "fold") {
             
                Program::said[1] = true;
                td.Fold(1);
                td.EndGame();
            }
        else if (temp[0] == "earnChips") {
                
          std::string str = temp[1];
                Program::earnchips += std::stoi(str);
                if (WinFlag == 0) {
                    if (Program::earnchips > shouldc) {
                        WinFlag = 1;
                    }
                }
                moneys.clear();
                for (int i = 0; i < 2; i++) {
                    moneys.push_back(20000);
                }
                
            }
        }
        while (true);
 } 