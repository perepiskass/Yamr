#pragma once

#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <set>
#include <list>
#include <vector>
#include <algorithm>

namespace reduce
{
    bool reduce(const std::list<std::string>& input,const size_t offset,const size_t lenght)
    {
        std::set<std::string> result;
        auto it  = input.begin();
        std::advance(it,offset);
        auto end = it;
        std::advance(end,lenght);
        for( ;it!=end ;++it)
        {
            if(result.find(*it) ==result.end()) result.insert(*it);
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