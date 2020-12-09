#include <boost/algorithm/string/join.hpp>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

struct Cmd {
    std::string name;
    int param = 0;
    bool executed = false;
};

struct Program {
    std::vector<Cmd> program;
    std::vector<unsigned> callstack;
    int acc = 0;

    void init() {
        acc = 0;
        callstack.clear();
        for (auto& cmd : program) {
            cmd.executed = false;
        }
    }

    void push(Cmd&& cmd) {
        program.push_back(std::forward<Cmd>(cmd));
    }

    bool execute() {
        init();
        for (auto it = program.begin(); it != program.end();) {
            Cmd& cmd = *it;

            if (cmd.executed) {
                std::cout << "PROGRAM INFINITE\n";
                return false;
            }
            cmd.executed = true;

            callstack.push_back(std::distance(program.begin(), it));

            if (cmd.name == "nop") {
                ++it;
            } else if (cmd.name == "jmp") {
                std::advance(it, cmd.param);
            } else if (cmd.name == "acc") {
                acc += cmd.param;
                ++it;
            } else {
                std::cout << "Unknown command: [" << cmd.name << "]\n";
                exit(1);
            }
        }
        std::cout << "PROGRAM FINISHED\n";
        return true;
    }
};

int main(int argc, char* argv[]) {

    Program prg1;

    std::string line;
    while(std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        Cmd cmd;
        cmd.name = line.substr(0, 3);
        cmd.param = stoi(line.substr(3));
        prg1.push(std::move(cmd));
    }

    bool finished = prg1.execute();
    std::cout << "Infinite loop! Acc=" << prg1.acc << "\n";
    for(auto call_it = prg1.callstack.rbegin(); call_it != prg1.callstack.rend(); ++call_it) {
        auto prg2 = prg1;
        for (; call_it != prg1.callstack.rend(); ++call_it) {
            auto& last = prg2.program.at(*call_it);
            if (last.name == "nop") {
                last.name = "jmp";
                break;
            } else if (last.name == "jmp") {
                last.name = "nop";
                break;
            }
        }
        finished = prg2.execute();
        if (finished) {
            std::cout << "Fixed program, acc=" << prg2.acc << "\n";
            break;
        }
    }

    return 0;
}
