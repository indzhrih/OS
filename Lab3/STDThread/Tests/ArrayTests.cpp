#include <bandit/bandit.h>
#include <sstream>
#include "../Headers/Array.h"
#include "TestFixtures.cpp"

using namespace bandit;
using namespace snowhouse;

go_bandit([]{
    describe("Array", []{
        describe("constructor", []{
            it("default constructor sets size to zero", []{
                Array a;
                AssertThrows(std::out_of_range, a.checkArrayInitialization());
            });

            it("constructor with positive size creates array", []{
                Array a(3);
            });

            it("constructor with size 1 creates single element", []{
                Array a(1);
            });

            it("constructor with zero size throws invalid_argument", []{
                AssertThrows(std::invalid_argument, Array(0));
            });

            it("constructor with negative size throws invalid_argument", []{
                AssertThrows(std::invalid_argument, Array(-5));
            });

            it("constructor with size 2 initializes first element to zero", []{
                Array a(2);
                AssertThat(a[0], Equals(0));
            });

            it("constructor with size 2 initializes second element to zero", []{
                Array a(2);
                AssertThat(a[1], Equals(0));
            });

            it("constructor with size 3 initializes all elements to zero", []{
                Array a(3);
                AssertThat(a[2], Equals(0));
            });
        });

        describe("operator[]", []{
            it("gets value at index 0 after assignment", []{
                Array a(3);
                a[0] = 10;
                AssertThat(a[0], Equals(10));
            });

            it("gets value at index 1 after assignment", []{
                Array a(3);
                a[1] = 20;
                AssertThat(a[1], Equals(20));
            });

            it("gets value at index 2 after assignment", []{
                Array a(3);
                a[2] = 30;
                AssertThat(a[2], Equals(30));
            });

            it("throws out_of_range on negative index -1", []{
                Array a(2);
                AssertThrows(std::out_of_range, a[-1]);
            });

            it("throws out_of_range on index equal to size", []{
                Array a(2);
                AssertThrows(std::out_of_range, a[2]);
            });

            it("throws out_of_range on index greater than size", []{
                Array a(2);
                AssertThrows(std::out_of_range, a[3]);
            });

            it("allows assignment at index 0", []{
                Array a(3);
                a[0] = 5;
                AssertThat(a[0], Equals(5));
            });

            it("allows assignment at index 1", []{
                Array a(3);
                a[1] = 5;
                AssertThat(a[1], Equals(5));
            });

            it("allows assignment at index 2", []{
                Array a(3);
                a[2] = 5;
                AssertThat(a[2], Equals(5));
            });
        });

        describe("printArray", []{
            it("prints array header", []{
                Array a(1);
                CoutRedirect out;
                a.printArray();
                std::string output = out.oss.str();
                bool containsHeader = output.find("Array: ") == 0;
                AssertThat(containsHeader, Equals(true));
            });

            it("prints single element array", []{
                Array a(1);
                a[0] = 42;
                CoutRedirect out;
                a.printArray();
                std::string output = out.oss.str();
                bool contains42 = output.find("42") != std::string::npos;
                AssertThat(contains42, Equals(true));
            });

            it("prints first element of array", []{
                Array a(3);
                a[0] = 1;
                CoutRedirect out;
                a.printArray();
                std::string output = out.oss.str();
                bool contains1 = output.find("1") != std::string::npos;
                AssertThat(contains1, Equals(true));
            });

            it("prints second element of array", []{
                Array a(3);
                a[1] = 2;
                CoutRedirect out;
                a.printArray();
                std::string output = out.oss.str();
                bool contains2 = output.find("2") != std::string::npos;
                AssertThat(contains2, Equals(true));
            });

            it("prints third element of array", []{
                Array a(3);
                a[2] = 3;
                CoutRedirect out;
                a.printArray();
                std::string output = out.oss.str();
                bool contains3 = output.find("3") != std::string::npos;
                AssertThat(contains3, Equals(true));
            });

            it("ends output with newline character", []{
                Array a(1);
                CoutRedirect out;
                a.printArray();
                std::string output = out.oss.str();
                bool endsWithNewline = !output.empty() && output.back() == '\n';
                AssertThat(endsWithNewline, Equals(true));
            });
        });

        describe("checkArrayInitialization", []{
            it("throws out_of_range on default constructed array", []{
                Array a;
                AssertThrows(std::out_of_range, a.checkArrayInitialization());
            });

            it("does not throw on array with size 1", []{
                Array a(1);
                a.checkArrayInitialization();
            });

            it("does not throw on array with size 5", []{
                Array a(5);
                a.checkArrayInitialization();
            });
        });

        describe("destructor", []{
            it("destroys array of size 3", []{
                Array* a = new Array(3);
                delete a;
            });

            it("destroys array of size 1", []{
                Array* a = new Array(1);
                delete a;
            });

            it("destroys array of size 10", []{
                Array* a = new Array(10);
                delete a;
            });
        });

        describe("array initialization", []{
            it("initializes element at index 0 to zero", []{
                Array a(5);
                AssertThat(a[0], Equals(0));
            });

            it("initializes element at index 1 to zero", []{
                Array a(5);
                AssertThat(a[1], Equals(0));
            });

            it("initializes element at index 2 to zero", []{
                Array a(5);
                AssertThat(a[2], Equals(0));
            });

            it("initializes element at index 3 to zero", []{
                Array a(5);
                AssertThat(a[3], Equals(0));
            });

            it("initializes element at index 4 to zero", []{
                Array a(5);
                AssertThat(a[4], Equals(0));
            });

            it("allows modification of element at index 0", []{
                Array a(3);
                a[0] = 10;
                AssertThat(a[0], Equals(10));
            });

            it("allows modification of element at index 1", []{
                Array a(3);
                a[1] = 20;
                AssertThat(a[1], Equals(20));
            });

            it("allows modification of element at index 2", []{
                Array a(3);
                a[2] = 30;
                AssertThat(a[2], Equals(30));
            });
        });
    });
});
