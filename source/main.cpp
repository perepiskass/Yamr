#include <iostream>
#include "utilitis.h"

std::pair<std::string,std::pair<size_t,size_t>> getArg(int argc,char** argv);

int main(int argc, char *argv[]) 
{
  try
  {
    auto args = getArg(argc,argv);
    std::string file_name{args.first};
    size_t m = args.second.first;
    // size_t r = args.second.second;
    
    std::vector<std::pair<size_t,size_t>> offset_size;
    offset_size.reserve(m+1); 
    offset_size = getBorders(file_name,m);
    for(auto& i : offset_size)
    {
      std::cout << i.first << ' ' << i.second << std::endl;
    }

    std::fstream fread (file_name,std::ios::in);
    for(size_t i = 0;i < offset_size.size();++i)
    {
      fread.seekg(offset_size[i].first,std::ios::beg);
      char* buf = new char(offset_size[i].second);
      fread.read(buf,offset_size[i].second);
      buf[offset_size[i].second] = '\0';
      std::cout << buf << std::endl;
      delete buf;
      fread.clear();
    }
    fread.close();
  }
  
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }

    return 0;
}
