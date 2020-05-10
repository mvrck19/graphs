#include <boost/asio.hpp>

void mytask(){}

int main(int argc, char const *argv[])
{
    boost::asio::thread_pool pool(4);

    boost::asio::post(pool,mytask);
    
    pool.join();

    return 0;
}
