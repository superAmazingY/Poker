#include <iostream>
#include"Program.h"
#include "sql.h"

//��ǰʤ��
double Program::PreflopWinProbCalc(TexasHoldemData thdata) {

  
    Poker first = reinterpret_cast<const OnHand&>(thdata.getOnHands()[0]).porkers[0];
    Poker second = reinterpret_cast<const OnHand&>(thdata.getOnHands()[0]).porkers[1];

    std::cout << first.getDs() << std::endl;
    std::cout << second.getDs() << std::endl;

    Poker* t;
    if (first.getDs() < second.getDs()) {
        t = &first;
        first = second;
        second = *t;
    }

    return TableManager::GetInstance()->get_data(TexasHoldemData::iPlayerCount, first.getDs(),
        second.getDs(), first.getHs() == second.getHs());
}

//����ʤ��
double Program::FlopWinProbCalc(TexasHoldemData td) {
    
    int wc = 0;
    TexasHoldemData thdata(td.iPlayerCount, std::vector<int>(),100);
    for (int w = 0; w < simulateCount; w++) {
        thdata.Init(thdata.iPlayerCount, std::vector<int>());

        std::vector<Poker> pks = {
                Poker(td.CommonPorkers[0].getHs(), td.CommonPorkers[0].getDs()),
                Poker(td.CommonPorkers[1].getHs(), td.CommonPorkers[1].getDs()),
                Poker(td.CommonPorkers[2].getHs(), td.CommonPorkers[2].getDs()),
        };

        thdata.Flop_After(pks);
        thdata.Preflop_After(td.getOnHands()[0].porkers);
        thdata.Turn();
        thdata.River();
        thdata.CalcPorkersType();

        int ret = 0;
        int j;
        for (j = 1; j < thdata.iPlayerCount; j++) {
            if (thdata.JudgeWhoWin(0, j) > 0) {
                ret = 1;
                break;
            }
        }

        if (j == thdata.iPlayerCount) {
            ret = -1;
        }

        if (ret < 0) {
            wc++;
        }
        
    }

    return static_cast<double>(wc) / simulateCount;
}

//ת��ʤ��
double Program::TurnWinProbCalc(TexasHoldemData td) {
    int wc = 0;
  
    TexasHoldemData thdata(td.iPlayerCount, std::vector<int>(),100);

    for (int w = 0; w < simulateCount; w++) {
        thdata.Init(thdata.iPlayerCount, std::vector<int>());

        Poker index = td.CommonPorkers[3];

        thdata.Turn_After(index);

        std::vector<Poker> pks = {
                Poker(td.CommonPorkers[0].getHs(), td.CommonPorkers[0].getDs()),
                Poker(td.CommonPorkers[1].getHs(), td.CommonPorkers[1].getDs()),
                Poker(td.CommonPorkers[2].getHs(), td.CommonPorkers[2].getDs()),
        };


        thdata.Flop_After(pks);
        thdata.Preflop_After(td.getOnHands()[0].porkers);
        thdata.River();
        thdata.CalcPorkersType();

        int ret = 0, j;
        for (j = 1; j < thdata.iPlayerCount; j++) {
            if (thdata.JudgeWhoWin(0, j) > 0) {
                ret = 1;
                break;
            }
        }

        if (j == thdata.iPlayerCount) {
            ret = -1;
        }

        if (ret < 0) {
            wc++;
        }
    }

    return static_cast<double>(wc) / simulateCount;
}

//����ʤ��
double Program::RiverWinProbCalc(TexasHoldemData td) {
    int wc = 0;
    TexasHoldemData thdata(td.iPlayerCount, std::vector<int>(),100);
   
    for (int w = 0; w < simulateCount; w++) {
        thdata.Init(thdata.iPlayerCount, std::vector<int>());

        Poker index = td.CommonPorkers[3];

        thdata.Turn_After(index);

        index = td.CommonPorkers[4];

        thdata.River_After(index);

        std::vector<Poker> pks = {
                Poker(td.CommonPorkers[0].getHs(), td.CommonPorkers[0].getDs()),
                Poker(td.CommonPorkers[1].getHs(), td.CommonPorkers[1].getDs()),
                Poker(td.CommonPorkers[2].getHs(), td.CommonPorkers[2].getDs()),
        };

        thdata.Flop_After(pks);
        thdata.Preflop_After(td.getOnHands()[0].porkers);
        thdata.CalcPorkersType();

        int ret = 0, j;
        for (j = 1; j < thdata.iPlayerCount; j++) {
            if (thdata.JudgeWhoWin(0, j) > 0) {
                ret = 1;
                break;
            }
        }

        if (j == thdata.iPlayerCount) {
            ret = -1;
        }

        if (ret < 0) {
            wc++;
        }
    }
    return static_cast<double>(wc) / simulateCount;
}


//�ж���
int Program::think(TexasHoldemData thdata, int cs, int iHumanActionCode, int iHumanAmount, int amount) {

   


    int r = 0;
    double p = 0;

    std::cout << cs << std::endl;

    switch (cs) {
    case 0:
        
        p = PreflopWinProbCalc(thdata);
        break;
    case 1:
      
        p = FlopWinProbCalc(thdata);
        break;
    case 2:
        
        p = TurnWinProbCalc(thdata);
        break;
    case 3:
       
        p = RiverWinProbCalc(thdata);
        break;
    }

    double a = 0;

    if (p < 1) {
        int tttmm = 0;
        for (int i = 0; i < thdata.iPlayerCount; i++) {
            if (static_cast<int>(thdata.thisTurnMoneys[i]) > tttmm) {
                tttmm = static_cast<int>(thdata.thisTurnMoneys[i]);
            }
        }

        a = (thdata.bonus + tttmm * thdata.iAliveCount) * p / (2 - p - p * thdata.iAliveCount / 2);
    }
    else {
        a = static_cast<int>(thdata.moneys[0]);
    }

    switch (iHumanActionCode) {
    case 0:
        if (a >= static_cast<int>(thdata.moneys[0])) {
            if (p >= 0.75) {
                r = 4;
                amount = static_cast<int>(thdata.moneys[0]);
                
            }
            else {
                amount = static_cast<int>((thdata.bonus + 100 * thdata.iAliveCount) * p / (2 - p - p * thdata.iAliveCount / 2));
               
                if (amount < static_cast<int>(thdata.moneys[0]) && amount >= 100) {
                    r = 3;
                }
                else {
                    r = 5;
                }
            }
        }
        else if (a >= 100) {
            r = 3;
            amount = static_cast<int>(a);
        }
        else {
            r = 5;
        }
        break;

        // CHECK
    case 5:
        if (a >= static_cast<int>(thdata.moneys[0])) {
            if (p >= 0.6) {
                r = 4;
                amount = static_cast<int>(thdata.moneys[0]);

            }
            else {
                amount = static_cast<int>((thdata.bonus + 100 * thdata.iAliveCount) * p / (2 - p - p * thdata.iAliveCount / 2));
                if (amount < static_cast<int>(thdata.moneys[0]) && amount >= 100) {
                    r = 3;
                }
                else {
                    r = 2;
                }
            }
        }
        else if (a >= 100) {
            r = 3;
            amount = static_cast<int>(a);
            
        }
        else {
            r = 2;
        }
        break;

        // RAISE
    case 3:
        if (a >= static_cast<int>(thdata.moneys[0])) {
            if (p >= 0.7) {
                r = 4;
                amount = static_cast<int>(thdata.moneys[0]);
             
            }
            else if (20000 - static_cast<int>(thdata.moneys[0]) - earnchips >= oppshouldc) {
                r = 2;
            }
            else {
                r = 6;
            }
        }
        else if (a >= iHumanAmount * 2) {
            r = 3;
            amount = static_cast<int>(a);
            
        }
        else if (a >= iHumanAmount) {
            r = 2;
            amount = static_cast<int>(a);
           
        }
        else if (20000 - static_cast<int>(thdata.moneys[0]) - earnchips >= oppshouldc) {
            r = 2;
        }
        else {
            r = 6;
        }
        break;

        // CALL
    case 2:
        if (cs == 0) {
            if (a >= static_cast<int>(thdata.moneys[0])) {
                if (p > 0.7) {
                    r = 4;
                    amount = static_cast<int>(thdata.moneys[0]);
                  
                }
                else {
                    amount = static_cast<int>((thdata.bonus + 100 * thdata.iAliveCount) * p /
                        (2 - p - p * thdata.iAliveCount / 2));
                    
                    if (amount < static_cast<int>(thdata.moneys[0]) && amount >= 200) {
                        r = 3;
                    }
                    else {
                        r = 5;
                    }
                }
            }
            else if (a >= 200) {
                r = 3;
                amount = static_cast<int>(a);
               
            }
            else {
                r = 5;
            }
        }
        break;

        // ALLIN
    case 4:
        if (a >= static_cast<int>(thdata.moneys[0])) {
            if (allinflag == 1) {
                if (p >= 0.664) {
                    r = 2;
                    amount = static_cast<int>(thdata.moneys[0]);
                   
                }
                else if (20000 - static_cast<int>(thdata.moneys[0]) - earnchips >= oppshouldc) {
                    r = 2;
                }
                else {
                    r = 6;
                }
            }
            else {
                if (p >= 0.79) {
                    r = 2;
                    amount = static_cast<int>(thdata.moneys[0]);
                    
                }
                else if (20000 - static_cast<int>(thdata.moneys[0]) - earnchips >= oppshouldc) {
                    r = 2;
                }
                else {
                    r = 6;
                }
            }
        }
        else if (20000 - static_cast<int>(thdata.moneys[0]) - earnchips >= oppshouldc) {
            r = 2;
        }
        else {
            r = 6;
        }
        break;

        // FOLD
    case 6:
        break;
    }
    return r;
}

//�ͷ������Ľ���
void Program::SendAction(int r, int cs, int amount, SOCKET socket_ai, TexasHoldemData td) {
   
    switch (r) {
        cout << r << endl;
    case 2: {
        td.Call(0);
        std::wstring t = L"call";
        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, t.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::vector<char> buffer(bufferSize);
        WideCharToMultiByte(CP_UTF8, 0, t.c_str(), -1, buffer.data(), bufferSize, nullptr, nullptr);

        send(socket_ai, buffer.data(), bufferSize, 0);

        std::wcout << t << std::endl;
        break;
    }
    case 3: {
        if (said[0]) {
            td.Raise(0, amount);
        }
        else if (cs == 0 && td.blindpos == 0) {
            td.Raise(0, amount);
        }
        else {
            td.Bet(0, amount);
        }
        std::wstring t = L"raise " + std::to_wstring(amount);
        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, t.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::vector<char> buffer(bufferSize);
        WideCharToMultiByte(CP_UTF8, 0, t.c_str(), -1, buffer.data(), bufferSize, nullptr, nullptr);
        for (auto a : buffer) {
            cout << a;
        }
        send(socket_ai, buffer.data(), bufferSize, 0);

        std::wcout << t << std::endl;
        break;
    }
    case 4: {
        allinflag = true;
        td.Allin(0);
        std::wstring t = L"allin";
        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, t.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::vector<char> buffer(bufferSize);
        WideCharToMultiByte(CP_UTF8, 0, t.c_str(), -1, buffer.data(), bufferSize, nullptr, nullptr);

        send(socket_ai, buffer.data(), bufferSize, 0);

        std::wcout << t << std::endl;
        break;
    }
    case 5: {
        td.Check(0);
        std::wstring t = L"check";
        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, t.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::vector<char> buffer(bufferSize);
        WideCharToMultiByte(CP_UTF8, 0, t.c_str(), -1, buffer.data(), bufferSize, nullptr, nullptr);

        send(socket_ai, buffer.data(), bufferSize, 0);

        std::wcout << t << std::endl;
        break;
    }
    case 6:
    {
        td.Fold(0);
        td.EndGame();
        std::wstring t = L"fold";
        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, t.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::vector<char> buffer(bufferSize);
        WideCharToMultiByte(CP_UTF8, 0, t.c_str(), -1, buffer.data(), bufferSize, nullptr, nullptr);
        send(socket_ai, buffer.data(), bufferSize, 0);
        break;
    }
    }

}
