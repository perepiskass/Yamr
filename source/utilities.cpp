#include <iostream>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include <fstream>
#include <utility>
#include <set>
#include <memory>
#include <algorithm>


namespace fs = boost::filesystem; 

std::vector<std::pair<size_t,size_t>> getBorders(const std::string& name_file,size_t m)
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

            // char b;
            // fread.get(b);
            // std::cout << "left - "<< b << ' ' << left_border << std::endl;
            // fread.seekg(-1,std::ios::cur);

            while (fread.get(buff))
            {
                if(buff == delim) break;
            }
            size_t right_border = fread.tellg();

            // fread.get(b);
            // std::cout << "right - " << b << ' ' << right_border << std::endl;
            // fread.seekg(right_border,std::ios::beg);
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
    // std::cout << "EXIT FROM CICLE\n";
    result.push_back(file_size);
    // std::cout << "After push back\n";
    std::vector<std::pair<size_t,size_t>> offset_size;
    std::sort(std::begin(result),std::end(result));
    result.resize(std::unique(std::begin(result),std::end(result))-std::begin(result));
    for (size_t i = 0; i < result.size()-1;++i)
    {
      offset_size.emplace_back(std::make_pair(result.at(i),result.at(i+1) - result.at(i)));
    }

    return offset_size;
}

