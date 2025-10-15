#include <bandit/bandit.h>
#include <sstream>
#include <iostream>
#include "../Headers/Array.h"

using namespace bandit;

struct CoutRedirect {
    std::streambuf* old;
    std::ostringstream oss;
    CoutRedirect()  : old(std::cout.rdbuf(oss.rdbuf())) {}
    ~CoutRedirect() { std::cout.rdbuf(old); }
};

go_bandit([]{
    describe("Array", []{
        it("ctor validates size (<=0 -> invalid_argument)", []{
            AssertThrows(std::invalid_argument, Array(0));
            AssertThrows(std::invalid_argument, Array(-3));
        });

        it("operator[] get/set returns reference and save value", []{
            Array a(3);
            a[0]=10; a[1]=-5; a[2]=7;
            AssertThat(a[0], Equals(10));
            AssertThat(a[1], Equals(-5));
            AssertThat(a[2], Equals(7));
        });

        it("operator[] check bounds (out_of_range)", []{
            Array a(2);
            AssertThrows(std::out_of_range, a[-1]);
            AssertThrows(std::out_of_range, a[2]);
        });

        it("printArray prints elements", []{
            Array a(4);
            for(int i=0;i<4;++i) a[i]=i+1;
            CoutRedirect out;
            a.printArray();
            auto s = out.oss.str();
            AssertThat(s.find("Array: 1 2 3 4") != std::string::npos, Equals(true));
        });

        it("checkArrayInitialization throws exceptions, when array is not initialized", []{
            Array a;
            AssertThrows(std::out_of_range, a.checkArrayInitialization());
        });
    });
});
