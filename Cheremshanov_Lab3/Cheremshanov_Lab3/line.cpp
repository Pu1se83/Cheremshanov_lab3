#include "line.h"
#include "pipeline.h"
#include "cs.h"

#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
#include <fstream>

using namespace std;

void Line::check_connection(const unordered_map<int, Pipeline>& pipes, const unordered_map<int, CS>& css) {
    for (const auto& pipe : pipes) {
        int input_cs = pipe_inputs.count(pipe.first) ? pipe_inputs.at(pipe.first) : -1;
        int output_cs = pipe_outputs.count(pipe.first) ? pipe_outputs.at(pipe.first) : -1;

        if (input_cs != -1 && output_cs != -1) {
            cout << "КС ID=" << input_cs << " --> Труба ID=" << pipe.first
                << " --> КС ID=" << output_cs << "\n";
        }
        else if (input_cs != -1) {
            cout << "КС ID=" << input_cs << " --> Труба ID=" << pipe.first << "\n";
        }
        else if (output_cs != -1) {
            cout << "Труба ID=" << pipe.first << " --> КС ID=" << output_cs << "\n";
        }
        else {
            cout << "Труба ID=" << pipe.first << " не подключен.\n";
        }
    }
}

void Line::create_connection(unordered_map<int, Pipeline>& pipes, unordered_map<int, CS>& css) {
    cout << "Выберите тип подключения:\n1) Подключить Pipe --> CS\n2) Подключить CS --> Pipe\n0) Выход\n";
    int choice = read_input<int>("Введите действие: ", 0, 2);

    while (choice != 0) {
        int pipe_id = read_input<int>("Введите ID трубы: ", 0, pipes.size() - 1);
        if (pipes.find(pipe_id) == pipes.end()) {
            cout << "ОШИБКА: ID трубы не найден.\n";
            continue;
        }

        if (choice == 1) {
            int cs_id = read_input<int>("Введите ID КС (Выxод): ", 0, css.size() - 1);
            if (css.find(cs_id) == css.end()) {
                cout << "ОШИБКА: ID КС не найден.\n";
                continue;
            }
            pipe_outputs[pipe_id] = cs_id;  // Устанавливаем выход трубы
        }
        else if (choice == 2) {
            int cs_id = read_input<int>("Введите ID КС (Вxод) : ", 0, css.size() - 1);
            if (css.find(cs_id) == css.end()) {
                cout << "ОШИБКА: ID КС не найден.\n";
                continue;
            }
            pipe_inputs[pipe_id] = cs_id;  // Устанавливаем вход трубы
        }

        choice = read_input<int>("Выберите тип подключения (1/2/0): ", 0, 2);
    }
}

void Line::topologicalSort(const unordered_map<int, Pipeline>& pipes, const unordered_map<int, CS>& css) {
    int n = css.size();
    vector<vector<int>> weightMatrix(n, vector<int>(n, 0));

    // Заполнение матрицы весов
    for (const auto& input : pipe_inputs) {
        auto& pipe_id = input.first;
        auto& input_cs = input.second;
        if (pipe_outputs.count(pipe_id)) {
            int output_cs = pipe_outputs.at(pipe_id);
            if (input_cs != -1 && output_cs != -1) {
                weightMatrix[input_cs][output_cs] = pipes.at(pipe_id).LengthOfPipe;
            }
        }
    }

    // Вывод матрицы весов
    cout << "Весовая матрица (Длина труб):\n";
    cout << "   ";
    for (int i = 0; i < n; ++i) {
        cout << i << " ";
    }
    cout << "\n";

    for (int i = 0; i < n; ++i) {
        cout << i << ": ";
        for (int j = 0; j < n; ++j) {
            cout << weightMatrix[i][j] << " ";
        }
        cout << "\n";
    }

    // Рассчитываем входящие степени
    vector<int> inDegree(n, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (weightMatrix[i][j] > 0) {
                ++inDegree[j];
            }
        }
    }

    // Очередь вершин с входящей степенью 0
    queue<int> zeroDegreeQueue;
    for (int i = 0; i < n; ++i) {
        if (inDegree[i] == 0) {
            zeroDegreeQueue.push(i);
        }
    }

    // Топологическая сортировка
    vector<int> sortedOrder;
    while (!zeroDegreeQueue.empty()) {
        int current = zeroDegreeQueue.front();
        zeroDegreeQueue.pop();
        sortedOrder.push_back(current);

        for (int i = 0; i < n; ++i) {
            if (weightMatrix[current][i] > 0) {
                if (--inDegree[i] == 0) {
                    zeroDegreeQueue.push(i);
                }
            }
        }
    }

    // Проверка на цикл
    if (sortedOrder.size() != n) {
        cerr << "ОШИБКА: График содержит цикл или несвязанные компоненты.\n";
        return;
    }

    // Вывод результата
    cout << "Порядок топографической сортировки(КС IDs):\n";
    for (int id : sortedOrder) {
        cout << id << " ";
    }
    cout << "\n";
}

void Line::delete_pipe(int pipe_id, unordered_map<int, Pipeline>& pipes) {
    if (pipes.find(pipe_id) == pipes.end()) {
        cout << "ОШИБКА: ID трубы не найден.\n";
        return;
    }

    // Удаляем соединения, связанные с трубой
    pipe_inputs.erase(pipe_id);
    pipe_outputs.erase(pipe_id);

    // Удаляем саму трубу
    pipes.erase(pipe_id);
    cout << "Труба с ID " << pipe_id << " была удалена.\n";
}

void Line::delete_cs(int cs_id, unordered_map<int, CS>& css) {
    if (css.find(cs_id) == css.end()) {
        cout << "ОШИБКА: ID КС не найден.\n";
        return;
    }

    // Удаляем все соединения, связанные с данным КС
    for (auto it = pipe_inputs.begin(); it != pipe_inputs.end(); ) {
        if (it->second == cs_id) {
            it = pipe_inputs.erase(it);
        }
        else {
            ++it;
        }
    }

    for (auto it = pipe_outputs.begin(); it != pipe_outputs.end(); ) {
        if (it->second == cs_id) {
            it = pipe_outputs.erase(it);
        }
        else {
            ++it;
        }
    }

    // Удаляем сам КС
    css.erase(cs_id);
    cout << "КС с ID " << cs_id << " была удалена.\n";
}

void Line::save_connections(ofstream& out) {
    for (const auto& con : pipe_outputs) {
        out << "@connection@\n";
        out << "OUTPUT " << con.first << " " << con.second << "\n";
    }

    for (const auto& con : pipe_inputs) {
        out << "@connection@\n";
        out << "INPUT " << con.first << " " << con.second << "\n";
    }

    cout << "Соединения успешно сохранены.\n";
}



void Line::load_connections(ifstream& in, unordered_map<int, Pipeline>& pipes, unordered_map<int, CS>& css)
{
    string line;
    while (getline(in, line)) {
        if (line == "@connection@") {
            int pipe_id, cs_id;
            string connection_type;

            if (!(in >> connection_type)) {
                cout << "ОШИБКА: В файле отсутствует тип подключения. Пропущена запись.\n";
                in.clear();
                in.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (!(in >> pipe_id) || pipes.find(pipe_id) == pipes.end()) {
                cout << "ОШИБКА: Недопустимый или несуществующий ID трубы в файле. Пропущена запись.\n";
                in.clear();
                in.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (!(in >> cs_id) || css.find(cs_id) == css.end()) {
                cout << "ОШИБКА: Недопустимый или несуществующий ID КС в файле. Пропущена запись.\n";
                in.clear();
                in.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (connection_type == "OUTPUT") {
                pipe_outputs[pipe_id] = cs_id;  // Устанавливаем выход трубы
            }
            else if (connection_type == "INPUT") {
                pipe_inputs[pipe_id] = cs_id;  // Устанавливаем вход трубы
            }
            else {
                cout << "ОШИБКА: Неизвестный тип подключения '" << connection_type << "'. Пропущена запись.\n";
                in.clear();
                in.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }

    cout << "Соединения успешно загруженны.\n";
    in.close();
}