
// From Bing Copilot, with manual changes

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

std::string adjustNameLength(std::string name, int desiredLength) {
	int currentLength = name.length();
	if (currentLength > desiredLength) {
		name = name.substr(0, desiredLength - 1) + ".";
	}
	else if (currentLength < desiredLength) {
		name += std::string(desiredLength - currentLength, ' ');
	}
	return name;
}

std::vector<std::string> readNamesFromFile(const std::string& filename) {
	std::vector<std::string> names;
	std::ifstream file(filename);
	std::string line;
	while (std::getline(file, line)) {
		names.push_back(line);
	}
	return names;
}

void writeNamesToFile(const std::vector<std::string>& names, const std::string& filename) {
	std::ofstream file(filename);
	for (const auto& name : names) {
		file << name << std::endl;
	}
}

void modifyBinaryFile(const std::string& filename, const std::vector<std::string>& names, int startByte, int endByte) {
	std::fstream file(filename, std::ios::in | std::ios::out | std::ios::binary);
	if (!file) {
		std::cerr << "Cannot open file!" << std::endl;
		return;
	}

	int nameIndex = 0;
	for (int i = startByte-1; i < endByte; ) {
		if (nameIndex >= names.size()) {
			break;
		}

		file.seekp(i);
		std::string name = names[nameIndex++];
		for (char c : name) {
			file.write(&c, 1);
		}
		i += name.length()+5;
	}

	file.close();
}

int main() {
	std::vector<std::string> originalList = readNamesFromFile("referees_orig.csv");
	std::vector<std::string> newList = readNamesFromFile("referees.csv");
	std::vector<std::string> originalListSorted = originalList;
	std::vector<std::string> newListListSorted = newList;
	std::vector<std::string> tmpList = newList;

	// To improve...
	//std::sort(originalListSorted.begin(), originalListSorted.end(), [](const std::string& a, const std::string& b) {
	//	return a.length() < b.length();
	//	});

	//std::sort(newListListSorted.begin(), newListListSorted.end(), [](const std::string& a, const std::string& b) {
	//	return a.length() < b.length();
	//	});

	//for (int i = 0; i < originalList.size(); i++) {
	//	for (int j = 0; j < originalListSorted.size(); j++) {
	//		if (originalList[i] == originalListSorted[j]) tmpList[i] = newList[i];
	//	}
	//}

	for (int i = 0; i < originalList.size(); i++) {
		tmpList[i] = adjustNameLength(tmpList[i], originalList[i].length());
	}

	writeNamesToFile(tmpList, "tmp_referees.csv");

	modifyBinaryFile("Super Kick Off.sms", tmpList, 56958, 57189);
	modifyBinaryFile("Super Kick Off.sms", tmpList, 57194, 57425);

	return 0;
}
