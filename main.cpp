#include "loader.h"

int main () {
    s21::Loader loader;
    loader.LoadFromFile("myobj.obj");
    loader.PrintLoadedInfo();
    return 0;
}