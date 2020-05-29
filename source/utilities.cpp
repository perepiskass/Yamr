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



