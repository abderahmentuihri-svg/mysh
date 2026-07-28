#include <iostream>
#include <sys/wait.h>
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

std::string find_in_path(std::string command)
{
    char *path_env = getenv("PATH");

    if (path_env == nullptr)
        return "";

    std::string path = path_env;
    std::vector<std::string> dirs = split(path, ':');

    for (std::string dir : dirs)
    {
        std::string full_path = dir + "/" + command;

        if (access(full_path.c_str(), X_OK) == 0)
        {
            return full_path;
        }
    }

    return "";
}
int main(){
    std::string line;

    while (1){
        std::cout << getcwd() << " $ ";
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
	else if (cmd=="type"){
		if (args.size()==0){
			std::cout<<std::endl;
		}
		else{
			for (int i=0;i<args.size();i++){
				if (args[i]=="exit" || args[i]=="echo" || args[i]=="type" || args[i]=="pwd" || args[i]=="cd"){
				std::cout<<args[i]<<" is a shell builtin.\n";
				}
				else if (""!=find_in_path(args[i])){
						std::cout<<args[i]<<" is "<<find_in_path(args[i])<<std::endl;
				}
				
			}
		}		
	}
       	else{
    std::string path = find_in_path(cmd);

    if (path == ""){
        std::cout << line << ": command not found" << std::endl;
    }
    else{
        pid_t pid = fork();

        if (pid == 0){
            // child process

            std::vector<char*> c_args;

            c_args.push_back(const_cast<char*>(cmd.c_str()));

            for (std::string& arg : args){
                c_args.push_back(const_cast<char*>(arg.c_str()));
            }

            c_args.push_back(nullptr);

            execvp(cmd.c_str(), c_args.data());

            // only runs if execvp failed
            perror("execvp");
            exit(1);
        }
        else if (pid > 0){
            // parent process
            waitpid(pid, nullptr, 0);
        }
        else{
            perror("fork");
        }
    }
}
    }

    return 0;
}
