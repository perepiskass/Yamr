#include <gtest/gtest.h>
#include "version_lib.h"

#include "threadpool.h"
#include "mapping.h"
#include "reduce.h"
#include <set>




// Тест кейсы на проверку версий
TEST(version_test_case, version_test_patch)
{
    ASSERT_GE(version_patch(), 1);
}
TEST(version_test_case, version_test_minor)
{
    EXPECT_EQ(version_minor(), 1);
}
TEST(version_test_case, version_test_major)
{
    EXPECT_EQ(version_major(), 1);
}

// Тест кейсы на проверку тред
TEST(threadpool_test_case, count_threads_test)
{
    size_t count = 5;
    ThreadPool tp{count};
    std::vector<std::future<std::thread::id>> thread_futures;
    for(size_t i = 0;i<count;++i)
    {
        thread_futures.emplace_back(tp.addTask([](){return std::this_thread::get_id();}));
    }
    std::set<std::thread::id> thread_names;
    for(auto& id : thread_futures)
    {
        thread_names.emplace(id.get());
    }

    EXPECT_EQ(thread_names.size(), count);
}

// Тест кейсы на проверку функций для mapping
TEST(mapping_test_case, get_border_size_test)
{
    std::string filename = "test";
    std::fstream file{filename,std::ios::out | std::ios::trunc};
    if(!file.is_open()) std::cout << "ERR open file " << filename << std::endl;
    int count_strings = 5;
    int count_symbols = 5;

    for(auto i = 0; i < count_strings;++i)
    {
        for(auto z = 0;z < count_symbols;++z)
        {
            file << i;
        }
        file << '\n';
    }

    file.close();

    auto borders = mapping::getBorders(filename,3);

    EXPECT_EQ(borders.size(), 3);
    
    fs::remove(fs::path(filename));
}

TEST(mapping_test_case, get_mapping_test)
{
    std::string filename = "test";
    std::fstream file{filename,std::ios::out | std::ios::trunc};
    if(!file.is_open()) std::cout << "ERR open file " << filename << std::endl;
    int count_strings = 5;
    int count_symbols = 5;

    for(auto i = 0; i < count_strings;++i)
    {
        for(auto z = 0;z < count_symbols;++z)
        {
            file << z;
        }
        file << '\n';
    }

    file.close();

    auto result = mapping::mapping(filename,count_symbols,0,6);
    EXPECT_FALSE(result.first);
    EXPECT_EQ(result.second.front(),"01234");

    result = mapping::mapping(filename,count_symbols+1,0,6);
    EXPECT_TRUE(result.first);
    EXPECT_EQ(result.second.front(),"01234");
    
    fs::remove(fs::path(filename));
}

// Тест кейсы функций на подсчет результатов полученных от функции mapping
TEST(reduce_test_case, get_reduce_borders_duplicates_test)
{
    std::list<std::string> data;
    for(auto i = 0;i<5;++i)
    {
        data.push_back("string-1");
        data.push_back("string-2");
        data.push_back("string-3");
    }
    data.sort();

    auto result = reduce::getBorders(data,3);
    EXPECT_EQ(result.size(),3);
    result = reduce::getBorders(data,2);
    EXPECT_EQ(result.size(),2);

    result = reduce::getBorders(data,5);
    EXPECT_EQ(result.size(),3);
    result = reduce::getBorders(data,99);
    EXPECT_EQ(result.size(),3);
}

TEST(reduce_test_case, get_reduce_borders_no_duplicates_test)
{
    std::list<std::string> data;
    for(auto i = 0;i<5;++i)
    {
        data.push_back("string"+std::to_string(i));
    }
    data.sort();

    auto result = reduce::getBorders(data,6);
    EXPECT_EQ(result.size(),5);
    result = reduce::getBorders(data,66);
    EXPECT_EQ(result.size(),5);
}


TEST(reduce_test_case, reduce_duplicates_test)
{   
    std::string filename = "reduce 1";
    if(fs::exists(fs::path(filename))) fs::remove(fs::path(filename));
    std::list<std::string> data;
    for(auto i = 0;i<5;++i)
    {
        data.push_back("string-1");
        data.push_back("string-2");
        data.push_back("string-3");
    }
    data.sort();

    auto result = reduce::reduce(data,false,1,0,data.size());
    EXPECT_TRUE(result);
    EXPECT_FALSE(fs::exists(fs::path(filename)));
}

TEST(reduce_test_case, reduce_no_duplicates_test)
{   
    std::string filename = "reduce 1";
    if(fs::exists(fs::path(filename))) fs::remove(fs::path(filename));
    std::list<std::string> data;
    for(auto i = 0;i<5;++i)
    {
        data.push_back("string"+std::to_string(i));
    }
    data.sort();

    auto result = reduce::reduce(data,false,1,0,data.size());
    EXPECT_FALSE(result);
    EXPECT_TRUE(fs::exists(fs::path(filename)));

    fs::remove(fs::path(filename));
}

TEST(reduce_test_case, reduce_duplicates_in_source_test)
{   
    std::string filename = "reduce 1";
    if(fs::exists(fs::path(filename))) fs::remove(fs::path(filename));
    std::list<std::string> data;
    for(auto i = 0;i<5;++i)
    {
        data.push_back("string-1");
        data.push_back("string-2");
        data.push_back("string-3");
    }
    data.sort();

    auto result = reduce::reduce(data,true,1,0,data.size());
    EXPECT_TRUE(result);
    EXPECT_TRUE(fs::exists(fs::path(filename)));

    fs::remove(fs::path(filename));
}





int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}