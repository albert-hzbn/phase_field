#include <string>
#include <fstream>
#include <map>

class LoadInput
{
private:
    std::map<std::string, std::string> input;

public:
    LoadInput();
    std::map<std::string, std::string> ReadFromFile(std::string);
    ~LoadInput();
};
