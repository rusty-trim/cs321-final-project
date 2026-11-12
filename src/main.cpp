#include "application.hpp"
#include <exception>

int main()
{
    try
    {
        Application application;
        application.run();
    }
    catch (std::exception &ex)
    {
        printf("Fatal error: %s", ex.what());
    }
}
