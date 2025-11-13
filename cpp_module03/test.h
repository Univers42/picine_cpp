#ifndef TEST_H
# define TEST_H

#include <iostream>

class test
{
private:
    std::string testName;
    int         failures;
        
public:
    test();
    ~test();
};


#endif