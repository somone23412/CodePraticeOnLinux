#include <iostream>
#include <vector>
#include <unordered_map>

int main() {
	std::cout << "Hello World!" << std::endl;
	
	std::vector<int> nums = {1, 2, 3, 4, 5};
	for (auto num : nums) {
		std::cout << num << " ";
	}
	std::cout << std::endl;
	
	std::unordered_map<std::string, int> hashMap;
	hashMap["quyan"] = 2018110086;
	hashMap["liuxinyu"] = 2018110184;

	for (auto mPair : hashMap) {
		std::cout << mPair.first << " : " << mPair.second << std::endl;
	}
	
	return 0;
}
