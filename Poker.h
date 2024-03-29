#pragma once
#ifndef PORKER_H
#define PORKER_H
#include <vector>

class Poker {
    int hs;
    int ds;

public:
    Poker(int hs, int ds);
    [[nodiscard]] int getHs() const;
    void setHs(int hs);
    [[nodiscard]] int getDs() const;
    void setDs(int ds);
};

enum class TypeName
{
    THS,  //ͬ��˳
    SIT, //����
    HL, //��«
    TH, // ͬ��
    SZ, //˳��
    SANT, //����
    LD, //����
    DZ, //����
    GP, //����
    FOLD  //����
};

class PorkersCombineType
{
private:
    int type;
    std::vector<int> pts;

public:
    PorkersCombineType(int type, const std::vector<int>& pts);
    [[nodiscard]] int GetType() const;
    void SetType(int type);
    [[nodiscard]] std::vector<int> GetPts() const;
    void SetPts(const std::vector<int> pts);
};

#endif  // PORKER_H


#pragma once

