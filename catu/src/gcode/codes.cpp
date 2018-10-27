#include <gcode/codes.hpp>

#include <map>
#include <iostream>
#include <fstream>
#include <cmath>

namespace GCode{

    Command::~Command(){}
    G01Command::G01Command(coord x, coord y, coord z): _x(x), _y(y), _z(z){}

    void G01Command::Execute() const {
        
    }

    float G01Command::GetLength() const {
        float x = _x ? (*_x)*(*_x) : 0.0f;
        float y = _y ? (*_y)*(*_y) : 0.0f;
        float z = _z ? (*_z)*(*_z) : 0.0f;

        return std::sqrt(x+y+x);
    }

    Movement G01Command::GetMovement() const {
        return  Movement(_x,_y,_z);

    }

    G01Command::~G01Command(){}
    /*void G00Command::Execute() const {
    
    }


    G00Command::~G00Command(){}*/

    std::unique_ptr<Command> BuildCommand(const std::map<char, float> values){
        std::optional<float> x,y,z;
        if(values.count('X')){
            x = values.at('X');
        }
        if(values.count('Y')){
            y = values.at('Y');
        }
        if(values.count('Z')){
            z = values.at('Z');
        }
        return std::unique_ptr<Command>(new G01Command(x,y,z));
    }

    std::unique_ptr<Command> ParseLine(const std::string& line){
        size_t i = 0;
        std::map<char, float> values;

        while(i<line.size()){
            size_t parsed = 0;
            switch (line[i]){
                case 'N':{
                    auto n = std::stoi(line.substr(i+1, line.size() - i), &parsed);
                         break;}
                case 'G':{
                    auto command = std::stoi(line.substr(i+1, line.size() - i), &parsed);
                    if (command != 1){
                        std::cout<<"Bad command id:"<<command<<std::endl;
                        return {};
                    }
                         break;}
                case 'X':
                case 'Y':
                case 'Z':
                    auto value = std::stof(line.substr(i+1, line.size() - i), &parsed);
                    values[line.at(i)] = value;
            }
            i += 1;//char
            i += parsed; //if any
        }
        return BuildCommand(values);
    }

    Program Loader::Load(const std::string& filePath) {
        Program commands;

        {
            std::ifstream file;
            file.open(filePath);
            try{
            if(!file.is_open()){
                std::cout<<"file is not open..."<<filePath<<std::endl;
                return {};
            }
            std::string line;
            while(std::getline(file, line)){
                if(line.size() == 0){
                    continue;
                }
                auto comm = ParseLine(line);
                if(comm == nullptr){
                    return {};
                }
                commands.push_back(std::move(comm));
            }
            }catch(...){
                return {};
            }
        }
        return commands;
    }
} // namespace GCode
