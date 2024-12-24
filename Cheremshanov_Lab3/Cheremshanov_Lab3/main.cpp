#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <iomanip>

#include "cs.h"
#include "pipeline.h"
#include "logs.h"
#include "line.h"

using namespace std;

class Line;

int main()
{
    setlocale(LC_ALL, "Russian");
    ofstream logfile("log.txt");
    redirect_output_wrapper redirector(cerr);
    redirector.redirect(logfile);


    unordered_map<int, Pipeline> pipes;
    unordered_map<int, CS> css;
    Line line;

    short int menu_choice = -1;

    while (true)
    {
        cout << endl;
        cout << setw(30) << setfill('=') << "" << endl;
        cout << setw(17) << setfill(' ') << " ����:" << endl;
        cout << setw(30) << setfill('=') << "" << endl;

        cout << "1)  �������� �����" << endl;
        cout << "2)  �������� ��" << endl;
        cout << "3)  �������� ���� ��������" << endl;
        cout << "4)  ��������� � ����" << endl;
        cout << "5)  ��������� �� �����" << endl;
        cout << "6)  ����� ����� �� ��������" << endl;
        cout << "7)  ����� ���� �� ������� �������" << endl;
        cout << "8)  ����� �� �� ��������" << endl;
        cout << "9)  ����� �� �� % ������� �����" << endl;
        cout << "10) ������� �����" << endl;
        cout << "11) ������� ��" << endl;
        cout << "12) ��������� ����������" << endl;
        cout << "13) ������� ����������" << endl;
        cout << "14) �������������� ����������" << endl;
        cout << "0)  �����" << endl;

        cout << setw(30) << setfill('=') << "" << endl;
        cout << endl;

        menu_choice = read_input<short int>("������� ��������: ", 0, 14);
        switch (menu_choice)
        {
        case 1: cin >> pipes; break;
        case 2: cin >> css; break;
        case 3: cout << pipes; cout << css; break;
        case 4: {
            if (css.size() != 0 || pipes.size() != 0)
            {
                ofstream outfile(read_input<string>("������� �������� �����: ")); 
                outfile << css; 
                outfile << pipes; line.save_connections(outfile); 
                outfile.close(); cout << "�������� � ����\n";
            }
            else { cout << "� ��� ��� ���� � ��\n"; } break;
        }
        case 5: {
            ifstream outfile(read_input<string>("������� �������� �����: ")); outfile >> css;
            outfile.clear(); 
            outfile.seekg(0, ios::beg); 
            outfile >> pipes; outfile.clear(); 
            outfile.seekg(0, ios::beg); 
            line.load_connections(outfile, pipes, css);  
            outfile.close(); break;
        }
        case 6: filter_pipes_by_name(pipes); break;
        case 7: filter_pipes_by_repair(pipes); break;
        case 8: filter_cs_by_name(css); break;
        case 9: filter_cs_by_work_percentage(css); break;
        case 10: { cout << pipes; int ch = read_input<int>("������� ID �����: ", 0, pipes.size() - 1); line.delete_pipe(ch, pipes); pipes.erase(ch); break; }
        case 11: { cout << css; int ch = read_input<int>("������� ID ��: ", 0, css.size() - 1); line.delete_cs(ch, css); css.erase(ch); break; }
        case 12: { line.check_connection(pipes, css); break; }
        case 13: { line.create_connection(pipes, css); break; }
        case 14: { line.topologicalSort(pipes, css); break; }
        case 0: logfile.close(); return 0;
        }
    }

    return 0;
}