#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <stdexcept>
#include <limits.h>
std::vector<std::string> split(std::string s,char delimiter) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos+1);
    }
    tokens.push_back(s);

    return tokens;
}

std::string getcwd() {
    char buffer[PATH_MAX];

    if (::getcwd(buffer, sizeof(buffer)) != nullptr) {
        return std::string(buffer);
    } else {
        throw std::runtime_error("getcwd failed");
    }
}
int main(){
    std::string line;

    while (1){
        std::cout << getcwd() << "$ ";
        std::getline(std::cin, line);

        std::vector<std::string> parts = split(line, ' ');
        if (parts.empty() || parts[0].empty()) continue;

        std::string cmd = parts[0];

        std::vector<std::string> args;
        for (int i = 1; i < parts.size(); i++){
            args.push_back(parts[i]);
        }

        if (cmd == "exit"){
            break;
        }
        else if (cmd == "echo"){
            for (int i = 0; i < args.size(); i++) {
                std::cout << args[i] << '\n';
            }
        }
        else if (cmd == "cd"){
            if (args.size() != 1){
                std::cout << "cd: too many arguments" << std::endl;
                continue;
            }

            if (chdir(args[0].c_str()) != 0) {
                perror("cd");
            }
        }
        else if (cmd == "pwd"){
            if (args.size() != 0){
                std::cout << "pwd: too many arguments" << std::endl;
            }
            else{
                std::cout << getcwd() << std::endl;
            }
        }
        else{
            std::cout << line << ": command not found" << std::endl;
        }
    }

    return 0;
}
