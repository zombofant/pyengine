#include <iostream>
#include <boost/python.hpp>

using namespace std;
using namespace boost::python;

struct World
{
    void set(std::string msg) { this->msg = msg; }
    std::string greet() { return msg; }
    std::string msg;
};

BOOST_PYTHON_MODULE(hello)
{
    class_<World>("World")
        .def("greet", &World::greet)
        .def("set", &World::set)
    ;
}

int main() {
    PyImport_AppendInittab("hello", &inithello);
    Py_Initialize();
    object main = import("__main__");
    object main_namespace = main.attr("__dict__");

    exec(
"\
import hello;\
print(\"Hello World from inside-python print()!\");\
test = hello.World();\
test.set(\"Hello World from hello.World!\");\
print(test.greet());\
",
                          main_namespace);

    
}
