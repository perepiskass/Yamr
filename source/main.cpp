#include <iostream>
// #include "utilities.h"
#include <utility>
#include <fstream>
#include <vector>

std::pair<std::string,std::pair<size_t,size_t>> getArg(int argc,char** argv);
std::vector<std::pair<size_t,size_t>> getBorders(const std::string& name_file,size_t m);

int main(int argc, char *argv[]) 
{
  try
  {
    auto args = getArg(argc,argv);
    std::string file_name{args.first};
    size_t m = args.second.first;

    auto offset_size = getBorders(file_name,m);
    for(auto& i : offset_size)
    {
      std::cout << i.first << ' ' << i.second << std::endl;
    }

    std::ifstream fread (file_name);
    // char buf[1024];
    for(size_t i = 0;i < offset_size.size();++i)
    {
      char* buf = new char(offset_size[i].second+1);
      fread.seekg(offset_size[i].first,std::ios::beg);
      fread.read(buf,offset_size[i].second);
      fread.clear();
      buf[offset_size[i].second] = '\0';
      std::cout << buf << std::endl;
      delete buf;
    }
    fread.close();
  }
  
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }

    return 0;
}
