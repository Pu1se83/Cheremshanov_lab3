#include "pipeline.h"
#include "logs.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int Line::MaxIDPipe = 0;

Pipeline::Pipeline()
{
    PipeID = Line::MaxIDPipe++;
}

istream& operator >> (istream& in, unordered_map<int, Pipeline>& pipes) {
    Pipeline pipe;
    pipe.PipeName = read_input<string>("Введите название труды: ");
    pipe.LengthOfPipe = read_input<int>("Введите её длину: ", 0);
    pipe.Diameter = read_input<int>("Введите её диаметр: ", 0);
    pipe.RepairIndicator = read_input<int>("Выберите её состаяние (0 - Работает, 1 - На ремонте): ", 0, 1);
    pipes.insert({ pipe.GetID(), pipe });
    return in;
}

ostream& operator << (ostream& out, const unordered_map<int, Pipeline>& pipes)
{
    if (pipes.size() == 0) { cout << "У вас нету труб\n"; return out; }
    for (const auto& pipe : pipes) {
        out << pipe.second;
    }
    return out;
};

ostream& operator << (ostream& out, const Pipeline& pipe)
{
    out << "\nID: " << pipe.PipeID
        << "\nНазвание: " << pipe.PipeName
        << "\nДлина: " << pipe.LengthOfPipe
        << "\nДиаметр: " << pipe.Diameter
        << "\nСостояние: " << pipe.RepairIndicator << endl;
    return out;
};

ofstream& operator << (ofstream& out, const unordered_map<int, Pipeline>& pipes)
{
    for (const auto& pipe : pipes) {
        out << pipe.second;
    }
    return out;
}

ofstream& operator << (ofstream& out, const Pipeline& pipe)
{
    out << "@pipeline@" << '\n'
        << pipe.PipeName << '\n'
        << pipe.LengthOfPipe << '\n'
        << pipe.Diameter << '\n'
        << pipe.RepairIndicator << '\n';
    return out;
};

ifstream& operator >> (ifstream& in, unordered_map<int, Pipeline>& pipes) {
    string line;
    while (getline(in, line)) {
        if (line == "@pipeline@") {
            Pipeline pipe;
            if (!getline(in, pipe.PipeName) || pipe.PipeName.empty()) {
                cout << "ОШИБКА трубы в файле\n";
                continue;
            }
            if (!(in >> pipe.LengthOfPipe) || pipe.LengthOfPipe < 0) {
                in.clear(); in.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "ОШИБКА трубы в файле\n";
                continue;
            }
            if (!(in >> pipe.Diameter) || pipe.Diameter <= 0) {
                in.clear(); in.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "ОШИБКА трубы в файле\n";
                continue;
            }
            if (!(in >> pipe.RepairIndicator) || (pipe.RepairIndicator != 0 && pipe.RepairIndicator != 1)) {
                in.clear(); in.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "ОШИБКА трубы в файле\n";
                continue;
            }
            pipes.insert({ pipe.GetID(), pipe });
        }
    }
    return in;
}

void ChangeRepair(Pipeline& pipe, bool repair)
{
    pipe.RepairIndicator = repair;
}

void filter_pipes_by_name(unordered_map<int, Pipeline>& pipes)
{
    string name = read_input<string>("Введите название или часть трубы для поиска: ");
    vector<int> ids;
    bool flag = false;
    for (const auto& p : pipes) {
        if (p.second.PipeName.find(name) != string::npos) {
            cout << p.second;
            ids.push_back(p.second.PipeID);
            flag = true;
        }
    }
    if (!flag) { cout << "У вас нет трубы с таким именем: " << name << endl; return; }
    int ch = read_input<int>("0 - Меню:\n1 - Поменять трубу\n", 0, 1);
    if (ch == 0) { return; }
    ch = read_input<int>("Введите статус", 0, 1);
    for (const auto& id : ids)
    {
        ChangeRepair(pipes[id], ch);
    }
}

void filter_pipes_by_repair(unordered_map<int, Pipeline>& pipes)
{
    int repair_status = read_input<int>("Введите статус (0 - Работает, 1 - На ремонте): ", 0, 1);
    vector<int> ids;
    bool flag = false;
    for (const auto& p : pipes) {
        if (p.second.RepairIndicator == repair_status) {
            cout << p.second;
            ids.push_back(p.second.PipeID);
            flag = true;
        }
    }
    if (!flag) { cout << "У вас нет трубы с таким статусом: " << repair_status << endl; return; }
    int ch = read_input<int>("0 - Меню:\n1 - Поменять трубы\n", 0, 1);
    if (ch == 0) { return; }
    ch = read_input<int>("Введите статус", 0, 1);
    for (const auto& id : ids)
    {
        ChangeRepair(pipes[id], ch);
    }
}