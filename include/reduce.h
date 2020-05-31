#pragma once

#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <set>
#include <list>
#include <vector>
#include <algorithm>
#include <map>

namespace reduce
{
    bool reduce(const std::list<std::string>& input,const size_t offset,const size_t lenght)
    {
        auto it = input.begin();
        std::advance(it,offset);
        std::map<std::string,size_t> result;
        for(size_t i = 0; i < lenght; ++i)
        {
            if(result.find(*it)!=result.end()) return false;
            else result[*it] = 1;
            ++it;
        }
        
        std::string filename = "reduce " + std::to_string(offset);
        std::fstream file(filename,std::ios::out | std::ios::trunc);
        if(!file.is_open())
        {
            std::cout << "error open file reduce\n";
        }
        else
        {
            for(const auto& str : result)
            {
                file << str.first << ": " << str.second << std::endl;
            }
            file.close();
        }
        return true;
    }

    std::vector<std::pair<size_t,size_t>> getBorders(const std::list<std::string>& source,size_t r)
    {
        std::vector<std::pair<size_t,size_t>> result;
        if(r == 1)
        {
            result.emplace_back(std::make_pair(0,source.size()));
            return result;
        }
        else if (r > source.size()) r = source.size();
        
        size_t part_size = source.size() / r;
        // std::cout << part_size << " part\n";
        // std::cout << source.size() << " source size\n";
        std::vector<size_t> offsets;
        offsets.emplace_back(0);
        auto it = source.begin();

        for(size_t i=1 ; i < r ; ++i)
        {
            std::advance(it, part_size);
            auto cur = it;
            size_t offset_left = 0;
            while(it != source.begin())
            {   
                if (*(--it) != *cur)
                 {
                     break;
                 }
                else
                {
                    ++offset_left;
                }
            }
            // std::cout << "left " << offset_left << std::endl;
            it = cur;
            size_t offset_right = 0;
            while(it != source.end())
            {
                if (*(++it) != *cur)
                {
                    ++offset_right;
                    break;
                } 
                else
                {
                    ++offset_right;
                }
            }
            // std::cout << "right " << offset_right << std::endl;

            if(offset_left < offset_right) offsets.emplace_back(part_size*i - offset_left);
            else offsets.emplace_back(part_size*i + offset_right);
            it = cur;
        }
        offsets.emplace_back(source.size());
        // for(auto& a : offsets) std::cout << a << ' ';
        // std::cout << std::endl;
        std::sort(std::begin(offsets),std::end(offsets));
        offsets.resize(std::unique(std::begin(offsets),std::end(offsets))-std::begin(offsets));
        // std::cout << "\nOffset size " << offsets.size() << std::endl;

        for (size_t i = 0; i < offsets.size()-1;++i)
        {
            result.push_back(std::make_pair(offsets.at(i),offsets.at(i+1) - offsets.at(i)));
        }

        return result;
    }
}