#include <iostream>
#include <string>
#include <sys/stat.h>
#include <fstream>
#include <map>
#include <ctime>
#include <iterator>
#include <unistd.h>
#define TODOFILE "todo.txt"
#define DONEFILE "done.txt"

inline bool exists_test3 (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

class ToDo {
private:
    std::string usage;
    time_t current = time(0);

public:
    ToDo() {
        if(!exists_test3(TODOFILE)) {
            std::fstream file;
            file.open(TODOFILE, std::ios::out);
            file.close();
        }
        if(!exists_test3(DONEFILE)) {
            std::fstream file;
            file.open(DONEFILE, std::ios::out);
            file.close();
        }
        usage = "Usage :-\n$ ./todo add \"todo item\"  # Add a new todo\n$ ./todo ls               # Show remaining todos\n$ ./todo del NUMBER       # Delete a todo\n$ ./todo done NUMBER      # Complete a todo\n$ ./todo help             # Show usage\n$ ./todo report           # Statistics\n";
    }
    void printUsage() {
        std::cout<<usage;
    }
    std::map<int, std::string> ls(int &counter) {
        std::map<int, std::string> lines;
        std::ifstream file(TODOFILE);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                counter++;
                lines.insert({counter, line.c_str()});
            }
            file.close();
        }

        return lines;
    }
    void add(std::string args, int counter) {
        std::map<int, std::string> lines = this->ls(counter);
        lines.insert({0, args});
        std::ofstream file(TODOFILE);
        if(file.is_open()) {
            for(auto const& it: lines) {
                file<<(it.second + "\n");
            }
        }
        std::cout<<"Added todo: \""<<args<<"\""<<"\n";
    }
    void del(int args, int& counter) {
        std::map<int, std::string> lines = this->ls(counter);
        if(args > counter || args == 0)
            std::cout<<"Error: todo #"<<args<<" does not exist. Nothing deleted.";
        else {
            lines.erase(counter - args + 1);
            std::ofstream file(TODOFILE);
            if(file.is_open()) {
                for(auto const& it: lines) {
                    file<<(it.second + "\n");
                }
            }
            std::cout<<"Deleted todo #"<<args<<"\n";
        }
    }
    void done(int args, int& counter) {
        std::map<int, std::string> lines = this->ls(counter);
        if(args > counter)
            std::cout<<"Error: todo #"<<args<<" does not exist.";
        else {
            std::map<int, std::string>::iterator it = lines.find(counter - args + 1);
            if(it != lines.end()) {
                tm* ltm = localtime(&current);

                std::fstream file1;
                file1.open(DONEFILE, std::ios::app);
                file1<<"x "<<1900 + ltm->tm_year<<"-"<<ltm->tm_mon + 1<<"-"<<ltm->tm_mday<<" "<<it->second<<"\n";
                file1.close();

                lines.erase(counter - args + 1);
                std::ofstream file(TODOFILE);
                if(file.is_open()) {
                    for(auto const& it: lines) {
                        file<<(it.second + "\n");
                    }
                }
                file.close();
                std::cout<<"Marked todo #"<<args<<" as done.";
            }
            else
                std::cout<<"Error: todo #"<<args<<" does not exist.";
        }
    }
    void report(int& counter) {
        tm* ltm = localtime(&current);

        this->ls(counter);
        int completed;
        std::ifstream file(DONEFILE);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                completed++;
            }
            file.close();
        }
        std::cout<<ltm->tm_year + 1900<<"-"<<ltm->tm_mon + 1<<"-"<<ltm->tm_mday<<" Pending : "<<counter<<" Completed : "<<completed<<"\n";
    }
};

int main(int argc, char* argv[])
{
    int counter;
    ToDo *obj = new ToDo();

    if(argc < 2)
        obj->printUsage();
    else {
        std::string args = (std::string)argv[1];
        if(!args.compare("help"))
            obj->printUsage();
        else if(!args.compare("ls")) {
            counter = 0;
            std::map<int, std::string> vals = obj->ls(counter);
            if(counter == 0)
                std::cout<<"There are no pending todos!";
            else {
                for(auto const& it: vals) {
                    std::cout<<"["<<counter - it.first + 1<<"] "<<it.second<<"\n";
                }
            }
        }
        else if(!args.compare("add")) {
            if(argc == 2)
                std::cout<<"Error: Missing todo string. Nothing added!"<<"\n";
            else {
                counter = 0;
                obj->add(argv[2], counter);
            }
        }
        else if(!args.compare("del")) {
            if(argc == 2)
                std::cout<<"Error: Missing NUMBER for deleting todo."<<"\n";
            else {
                counter = 0;
                obj->del(std::stoi(argv[2]), counter);
            }
        }
        else if(!args.compare("done")) {
            if(argc == 2)
                std::cout<<"Error: Missing NUMBER for marking todo as done."<<"\n";
            else {
                counter = 0;
                obj->done(std::stoi(argv[2]), counter);
            }
        }
        else if(!args.compare("report")) {
            counter = 0;
            obj->report(counter);
        }
    }
    return 0;
}
