#include <iostream>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include <fstream>
#include <utility>
#include <set>
#include <memory>
#include <algorithm>
#include <list>


namespace fs = boost::filesystem; 

std::string test(size_t i)
{
    std::string str{"It is a test number - "};
    str+= std::to_string(i);
    return str;
}

std::string test2(std::string file_name,size_t offset,size_t lenght)
{
     char* buf = new char[lenght+1];
        std::ifstream fread(file_name);
        if(!fread.is_open()) std::cout << "ERROR OPEN FILE\n";
      fread.seekg(offset,std::ios::beg);
      fread.read(buf,lenght);
      fread.clear();
      buf[lenght] = '\0';
    //   std::cout << buf << std::endl;
    std::string str{buf};
      delete[] buf;
      return str;
}

std::string test3(std::string file_name,size_t offset,size_t lenght)
{
     char* buf = new char[lenght+1];
        std::ifstream fread(file_name);
        if(!fread.is_open()) std::cout << "ERROR OPEN FILE\n";
      fread.seekg(offset,std::ios::beg);
      fread.getline(buf,lenght);
      fread.clear();
      buf[lenght] = '\0';
    //   std::cout << buf << std::endl;
    std::string str{buf,3};
      delete[] buf;
      return str;
}

std::vector<std::pair<size_t,size_t>> getBorders(const std::string& name_file,const size_t m)
{
    std::vector<size_t> result;
    size_t file_size = fs::file_size(fs::path(name_file));
    size_t part_size = file_size / m;
       
    std::ifstream fread (name_file,std::ios::in);
    if (fread.is_open())
    {
        const char delim = '\n';
        result.push_back(0);
        for(size_t i = 1;i < m;++i)
        {
            fread.seekg(part_size*i,std::ios::beg);
            std::size_t lenght = fread.tellg();
            std::cout << lenght << std::endl;
            char buff;
            for(size_t l = lenght;l >0 ;--l)
            {
                fread.seekg(-1,std::ios::cur);
                fread.get(buff);
                if(buff == delim) break;
                fread.seekg(-1,std::ios::cur);
            }
            size_t left_border = fread.tellg();

            while (fread.get(buff))
            {
                if(buff == delim) break;
            }
            size_t right_border = fread.tellg();

            fread.clear();

            if(part_size*i - left_border < right_border - part_size*i)
            {
                result.push_back(left_border);
                std::cout << "left to vector" << std::endl;
            }
            else
            {
                result.push_back(right_border);
                std::cout<< "right to vector" << std::endl;
            }        
        }
    }
    fread.close();
    result.push_back(file_size);
    std::vector<std::pair<size_t,size_t>> offset_size;
    std::sort(std::begin(result),std::end(result));
    result.resize(std::unique(std::begin(result),std::end(result))-std::begin(result));
    for (size_t i = 0; i < result.size()-1;++i)
    {
      offset_size.emplace_back(std::make_pair(result.at(i),result.at(i+1) - result.at(i)));
    }

    return offset_size;
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
            if (*(--it) != *cur) break;
            else
            {
                ++offset_left;
            }
        }

        it = cur;
        size_t offset_right = 0;
        while(it != source.end())
        {
            if (*(++it) != *cur) break;
            else
            {
                ++offset_right;
            }
        }

        if(offset_left < offset_right) offsets.emplace_back(part_size*i - offset_left);
        else offsets.emplace_back(part_size*i + offset_right);

        it = cur;
    }
    offsets.emplace_back(source.size());

    std::sort(std::begin(offsets),std::end(offsets));
    result.resize(std::unique(std::begin(offsets),std::end(offsets))-std::begin(offsets));
    for (size_t i = 0; i < offsets.size()-1;++i)
    {
      result.emplace_back(std::make_pair(offsets.at(i),offsets.at(i+1) - offsets.at(i)));
    }

    return result;
}