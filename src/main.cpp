#include <iostream>
#include <string>
#include <ns.hpp>
#include <cstdlib>

int main(int argc, char** argv) {
    std::string* resource = new std::string("/hp.aspx");
    char* page = fdt::http::get(resource);
    fdt::Forum *panel = fdt::parsers::panel::parse(page);
    CLOG(*(panel->tostring()));
    delete resource;
    free(page);
    return 0;
}
