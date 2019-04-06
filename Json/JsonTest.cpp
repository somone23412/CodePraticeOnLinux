#include <iostream>
#include <fstream>
#include "json.h"

int main() {
    std::ifstream i("data.json");
    nlohmann::json j;
    i >> j;
    std::cout << j << std::endl;
    j["hobby"] = "reading";
    std::cout << j << std::endl;
    return 0;
}
