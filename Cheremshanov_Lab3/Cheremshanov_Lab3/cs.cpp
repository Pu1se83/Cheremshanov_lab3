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
    cs.CSName = read_input<string>("Введите название КС: ");
    cs.NumberOfWorkshops = read_input<int>("Введите количество цехов: ", 0);
    cs.WorkshopsInWork = read_input<int>("Введите количество, количество рабочих цехов: ", 0, cs.NumberOfWorkshops);
    cs.Efficiency = read_input<int>("Введите эффективность (0-100): ", 0, 100);
    css.insert({ cs.GetID(), cs });
    return in;
}

ostream& operator << (ostream& out, const unordered_map<int, CS>& css)
{
    if (css.size() == 0) { cout << "У вас нет КС\n"; return out; }
    for (const auto& cs : css) {
        out << cs.second;
    }
    return out;
};

ostream& operator << (ostream& out, const CS& cs)
{
    out << "\nID: " << cs.CSID
        << "\nНазвание: " << cs.CSName
        << "\nКоличество цехов: " << cs.NumberOfWorkshops
        << "\nКоличество рабочих цехов: " << cs.WorkshopsInWork
        << "\nЭффективность: " << cs.Efficiency << endl;
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
        cout << "ОШИБКА КС ID=" << cs.CSID << " имеет " << cs.NumberOfWorkshops << " цехов";
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
                cout << "ОШИБКА КС в файле\n";
                continue;
            }
            if (!(in >> cs.NumberOfWorkshops) || cs.NumberOfWorkshops <= 0) {
                cout << "ОШИБКА КС в файле\n";
                in.clear(); in.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            if (!(in >> cs.WorkshopsInWork) || cs.WorkshopsInWork < 0 || cs.WorkshopsInWork > cs.NumberOfWorkshops) {
                cout << "ОШИБКА КС в файле\n";
                in.clear(); in.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            if (!(in >> cs.Efficiency) || cs.Efficiency < 0 || cs.Efficiency > 100) {
                cout << "ОШИБКА КС в файле\n";
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
    string name = read_input<string>("Введите имя КС для поиска: ");
    vector<int> ids;
    bool flag = false;
    for (const auto& p : cs) {
        if (p.second.CSName.find(name) != string::npos) {
            cout << p.second;
            ids.push_back(p.second.CSID);
            flag = true;
        }
    }
    if (!flag) { cout << "У тебя нету КС с таким именем: " << name << endl; return; }
    int ch = read_input<int>("0 - Меню:\n1 - Поменять КС\n", 0, 1);
    if (ch == 0) { return; }
    ch = read_input<int>("Введите число рабочих КС ", 0);
    for (const auto& id : ids)
    {
        change_workshops_in_work(cs[id], ch);
    }
}

void filter_cs_by_work_percentage(unordered_map<int, CS>& cs)
{
    int percentage = read_input<int>("Введите % рабочих цехов (0-100) для поиска: ", 0, 100);
    int choice = read_input<int>("Выберите опцию: 1 - Больше, 2 - Меньше, 3 - Равно", 1, 3);
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
        if (choice == 1) { cout << "Вы больше не имеете КС, у которых % рабочих цехов > " << percentage << endl; }
        else if (choice == 2) { cout << "Вы больше не имеете КС, у которых % рабочих цехов < " << percentage << endl; }
        else { cout << "Вы больше не имеете КС, у которых % рабочих цехов = " << percentage << endl; }
        return;
    }
    int ch = read_input<int>("0 - Меню\n1 - Изменить КС\n", 0, 1);
    if (ch == 0) return;
    ch = read_input<int>("Введите число рабочих цехов ", 0);
    for (const auto& id : ids)
    {
        change_workshops_in_work(cs[id], ch);
    }
}