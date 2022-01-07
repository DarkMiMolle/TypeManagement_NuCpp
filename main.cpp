#include <iostream>
#include "Var.h"
#include "Typeof.h"
#include "Any.h"


using namespace std;
int main() {
    var v = 42;
    var<int> v2;
    var t = Int;
    var<Any> a = Any(42);
    cout << Typeof(a).Name << endl;
    a->PointTo(Type);
    cout << Typeof(a).Name << endl;
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
