#include <string>
#include <vector>
#include <string>
std::vector<std::string> customsplit(std::string s, std::string delimiter)
{
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    if(int(s.length()) > 0)
    {
        tokens.push_back(s);
    }
    return tokens;
}