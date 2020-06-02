#include <gtest/gtest.h>
#include "version_lib.h"

#include "threadpool.h"
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
TEST(threadpool_test_case, count_threadpool_test)
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






int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}