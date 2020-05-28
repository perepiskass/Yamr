#pragma once

#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <set>

bool reduce(const std::list<std::string>& input,const size_t offset,const size_t lenght)
{
    std::set<std::string> result;
    auto it  = input.begin();
    std::advance(it,offset);
    auto end = it;
    std::advance(end,lenght);
    for( ;it!=end ;++it)
    {
        if(result.find(*it)!=result.end()) result.insert(*it);
        else return false;
    }

    std::fstream file("reduce_"+std::to_string(offset),std::ios::ate);
    if(!file.is_open())
    {
        std::cout << "error open file reduce\n";
    }
    else
    {
        for(const auto& str : result)
        {
            file << str << ": 0\n";
        }
        file.close();
    }
    return true;
}