#include "magalgorithm.h"

std::vector<std::string> splitString(std::string str,std::string delim)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str += delim;//扩展字符串以方便操作
    int size = str.size();

    for(int i = 0; i < size; i++)
    {
        pos = str.find(delim, i);
        if(pos < size)
        {
            std::string s = str.substr(i, pos - i);
            if(s != "")
                result.push_back(s);
            i = pos + delim.size() - 1; //迭代开始会i++
        }
    }
    return result;
}
