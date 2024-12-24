#include "cs.h"
#include "logs.h"

#include <iostream>
#include <fstream>

using namespace std;

int Line::MaxIDCS = 0;

CS::CS()
{
    CSID = Line::MaxIDCS++;
}

istream& operator >> (istream& in, unordered_map<int, CS>& css) {
    CS cs;
    cs.CSName = read_input<string>("������� �������� ��: ");
    cs.NumberOfWorkshops = read_input<int>("������� ���������� �����: ", 0);
    cs.WorkshopsInWork = read_input<int>("������� ����������, ���������� ������� �����: ", 0, cs.NumberOfWorkshops);
    cs.Efficiency = read_input<int>("������� ������������� (0-100): ", 0, 100);
    css.insert({ cs.GetID(), cs });
    return in;
}

ostream& operator << (ostream& out, const unordered_map<int, CS>& css)
{
    if (css.size() == 0) { cout << "� ��� ��� ��\n"; return out; }
    for (const auto& cs : css) {
        out << cs.second;
    }
    return out;
};

ostream& operator << (ostream& out, const CS& cs)
{
    out << "\nID: " << cs.CSID
        << "\n��������: " << cs.CSName
        << "\n���������� �����: " << cs.NumberOfWorkshops
        << "\n���������� ������� �����: " << cs.WorkshopsInWork
        << "\n�������������: " << cs.Efficiency << endl;
    return out;
};

void change_workshops_in_work(CS& cs, int& new_WorkshopsInWork)
{
    if (new_WorkshopsInWork <= cs.NumberOfWorkshops)
    {
        cs.WorkshopsInWork = new_WorkshopsInWork;
        return;
    }
    else
    {
        cout << "������ �� ID=" << cs.CSID << " ����� " << cs.NumberOfWorkshops << " �����";
        return;
    }
}

ofstream& operator << (ofstream& out, const unordered_map<int, CS>& css)
{
    for (const auto& cs : css) {
        out << cs.second;
    }
    return out;
}

ofstream& operator << (ofstream& out, const CS& cs)
{
    out << "@cs@" << '\n'
        << cs.CSName << '\n'
        << cs.NumberOfWorkshops << '\n'
        << cs.WorkshopsInWork << '\n'
        << cs.Efficiency << '\n';
    return out;
};

ifstream& operator >> (ifstream& in, unordered_map<int, CS>& css) {
    string line;
    while (getline(in, line)) {
        if (line == "@cs@") {
            CS cs;
            if (!getline(in, cs.CSName) || cs.CSName.empty()) {
                cout << "������ �� � �����\n";
                continue;
            }
            if (!(in >> cs.NumberOfWorkshops) || cs.NumberOfWorkshops <= 0) {
                cout << "������ �� � �����\n";
                in.clear(); in.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            if (!(in >> cs.WorkshopsInWork) || cs.WorkshopsInWork < 0 || cs.WorkshopsInWork > cs.NumberOfWorkshops) {
                cout << "������ �� � �����\n";
                in.clear(); in.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            if (!(in >> cs.Efficiency) || cs.Efficiency < 0 || cs.Efficiency > 100) {
                cout << "������ �� � �����\n";
                in.clear(); in.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            css.insert({ cs.GetID(), cs });
        }
    }
    return in;
}

void filter_cs_by_name(unordered_map<int, CS>& cs)
{
    string name = read_input<string>("������� ��� �� ��� ������: ");
    vector<int> ids;
    bool flag = false;
    for (const auto& p : cs) {
        if (p.second.CSName.find(name) != string::npos) {
            cout << p.second;
            ids.push_back(p.second.CSID);
            flag = true;
        }
    }
    if (!flag) { cout << "� ���� ���� �� � ����� ������: " << name << endl; return; }
    int ch = read_input<int>("0 - ����:\n1 - �������� ��\n", 0, 1);
    if (ch == 0) { return; }
    ch = read_input<int>("������� ����� ������� �� ", 0);
    for (const auto& id : ids)
    {
        change_workshops_in_work(cs[id], ch);
    }
}

void filter_cs_by_work_percentage(unordered_map<int, CS>& cs)
{
    int percentage = read_input<int>("������� % ������� ����� (0-100) ��� ������: ", 0, 100);
    int choice = read_input<int>("�������� �����: 1 - ������, 2 - ������, 3 - �����", 1, 3);
    bool flag = false;
    vector<int> ids;
    for (const auto& p : cs) {
        int current_percentage = (p.second.WorkshopsInWork * 100) / p.second.NumberOfWorkshops;
        if ((choice == 1 && current_percentage > percentage) ||
            (choice == 2 && current_percentage < percentage) ||
            (choice == 3 && current_percentage == percentage))
        {
            cout << p.second;
            ids.push_back(p.second.CSID);
        }
    }
    if (!flag)
    {
        if (choice == 1) { cout << "�� ������ �� ������ ��, � ������� % ������� ����� > " << percentage << endl; }
        else if (choice == 2) { cout << "�� ������ �� ������ ��, � ������� % ������� ����� < " << percentage << endl; }
        else { cout << "�� ������ �� ������ ��, � ������� % ������� ����� = " << percentage << endl; }
        return;
    }
    int ch = read_input<int>("0 - ����\n1 - �������� ��\n", 0, 1);
    if (ch == 0) return;
    ch = read_input<int>("������� ����� ������� ����� ", 0);
    for (const auto& id : ids)
    {
        change_workshops_in_work(cs[id], ch);
    }
}