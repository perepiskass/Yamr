#include <iostream>
#include <utility>
#include <fstream>
#include "threadpool.h"
#include "mapping.h"
#include "reduce.h"
#include <future>
#include <algorithm>

std::pair<std::string,std::pair<size_t,size_t>> getArg(int argc,char** argv);

std::vector<std::pair<size_t,size_t>> getBorders(const std::string& name_file,const size_t m);
std::vector<std::pair<size_t,size_t>> getBorders(const std::list<std::string>& source,size_t r);

std::string test(size_t i);
std::string test2(std::string file_name,size_t offset,size_t lenght);
std::string test3(std::string file_name,size_t offset,size_t lenght);

int main(int argc, char *argv[]) 
{
  try
  {
    auto args = getArg(argc,argv);
    std::string file_name{args.first};
    size_t m = args.second.first;
    // size_t r = args.second.second;

    auto param = getBorders(file_name,m);
    std::cout << "Border good\n";
    ThreadPool t{param.size()};
    // std::vector<std::future<std::list<std::string>>> res;
    std::vector<std::future<std::string>> res;
    for(size_t i = 0;i < param.size() ; ++i)
    {
      res.emplace_back(t.addTask(test3,file_name,param[i].first,param[i].second));
    }

    for(auto& i : res)
    {
      
      std::cout << i.get() << std::endl;
      // for(auto& z : i.get())
      // {
      //   std::cout << z << std::endl;
      // }
    }

  }
  
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }

    return 0;
}
