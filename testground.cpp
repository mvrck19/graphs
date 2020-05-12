#include <iostream>
#include <thread>
#include <boost/asio.hpp>

int call_from_thread(int x)
{
    return x;
}

int main()
{
    boost::asio::thread_pool pool(4);
    boost::asio::post(pool,(void*) call_from_thread,10);
    pool.join();
    return 0;
}
