#include <iostream>
#include <vector>
#include <string>
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
int main(){
	std::string line;
	while (1){
	std::cout<<"$ ";
	std::getline(std::cin,line);
	std::vector<std::string> parts=split(line, ' ');
	std::string cmd=parts[0];
	std::vector<std::string> args;
	for (int i=1;i<parts.size();i++){
		args.push_back(parts[i]);
	}
	if (cmd=="exit"){
		break;
	}
	else if (cmd=="echo"){
		for (int i=0;i<args.size();i++) {
    			std::cout << args[i]<<'\n';
		}
	}
	else{
		std::cout<<line<<": command not found"<<std::endl;
	}
	}
	return 0;
	}

