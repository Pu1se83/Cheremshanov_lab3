#pragma once

#ifndef LINE_H_
#define LINE_H_

#include "logs.h"
#include "pipeline.h"
#include "cs.h"

#include <unordered_map>
#include <vector>

using namespace std;

class Pipeline;
class CS;

class Line
{
private:
    static int MaxIDPipe;
    static int MaxIDCS;
    unordered_map<int, int> pipe_inputs;  // Входные CS для труб
    unordered_map<int, int> pipe_outputs; // Выходные CS для труб
public:
    void delete_pipe(int pipe_id, unordered_map<int, Pipeline>& pipes);
    void delete_cs(int cs_id, unordered_map<int, CS>& css);
    void save_connections(ofstream& out);
    void load_connections(ifstream& in, unordered_map<int, Pipeline>& pipes, unordered_map<int, CS>& css);
    void check_connection(const unordered_map<int, Pipeline>& pipes, const unordered_map<int, CS>& css);
    void create_connection(unordered_map<int, Pipeline>& pipes, unordered_map<int, CS>& css);
    void topologicalSort(const unordered_map<int, Pipeline>& pipes, const unordered_map<int, CS>& css);

    friend class CS;
    friend class Pipeline;
};
#endif