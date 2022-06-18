#ifndef _TEST_SHARED_LIB_H_
#define _TEST_SHARED_LIB_H_

#include <string>

class TestSharedLib {
public:
    TestSharedLib();
    // TestSharedLib(int i);
    virtual ~TestSharedLib();
    int hello(const std::string& s);

    int aaa();
    void bbb(int );
};

#endif
