#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

struct Player {
	std::string name;
	std::string country;
	std::string team;
	std::string position;
	std::string status;
};

int main() {
	int startByte = 164113;
	int endByte = 175825;
	std::vector<std::string> countries = { "England", "France", "Sweden", "Spain", "Holland", "Germany", "Italy", "Brazil" };
	std::vector<int> countriesFirstByte = { startByte, 165482, 166860, 168045, 169478, 171051, 172887, 174303 };

	std::ifstream file("players.csv");
	std::string line;
	std::vector<Player> players;
	std::map<std::string, int> lastCityPlayerIndex;

	while (std::getline(file, line)) {
		Player player;
		size_t pos = 0;
		pos = line.find(';'); player.name = line.substr(0, pos); line.erase(0, pos + 1);
		pos = line.find(';'); player.country = line.substr(0, pos); line.erase(0, pos + 1);
		pos = line.find(';'); player.team = line.substr(0, pos); line.erase(0, pos + 1);
		pos = line.find(';'); player.position = line.substr(0, pos); line.erase(0, pos + 1);
		player.status = line;

		players.push_back(player);
		if (player.team != "International") {
			lastCityPlayerIndex[player.country] = players.size() - 1;
		}
	}

	std::ofstream tmpFile("tmp_players.csv");
	std::fstream binaryFile("Super Kick Off.sms", std::ios::in | std::ios::out | std::ios::binary);

	// TODO
	for (int i = 0; i < countries.size(); i++) {
		std::string country = countries[i];
		int start = countriesFirstByte[i] - startByte;
		int end = (i < countries.size() - 1 ? countriesFirstByte[i + 1] : endByte+1) - startByte;
		int targetLength = end - start;

		std::string names;
		for (const auto& player : players) {
			if (player.country == country) {
				names += player.name + '\0';
			}
		}
		int length = names.size();

		// Adjust the length of names to fit the memory positions
		while (length > targetLength) {
			//for (auto& player : players) {
			//for (auto& index : lastCityPlayerIndex) {
				//Player player = players[index.second];
			for (auto it = players.rbegin(); it != players.rend(); ++it) {
				auto& player = *it;
				if ((player.country == country) && (player.team != "International")) {
					if (player.name.size() > 2) {
						player.name.pop_back();
						player.name.pop_back();
						player.name.push_back('.');
						length--;
						if (length <= targetLength) break;
					}
				}
			}
		}
		while (length < targetLength) {
			//for (auto& player : players) {
			//for (auto& index : lastCityPlayerIndex) {
				//Player player = players[index.second];
			for (auto it = players.rbegin(); it != players.rend(); ++it) {
				auto& player = *it;
				if ((player.country == country) && (player.team != "International")) {
					player.name.push_back(' ');
					length++;
					if (length >= targetLength) break;
				}
			}
		}

		names.clear();
		for (const auto& player : players) {
			if (player.country == country) {
				names += player.name + '\0';
			}
		}

		// Write the names to the binary file
		binaryFile.seekp(start + startByte-1, std::ios::beg);
		binaryFile.write(names.c_str(), names.size());
	}

	// Generate the new tmp_players.csv file
	for (const auto& player : players) {
		tmpFile << player.name << ';' << player.country << ';' << player.team << ';' << player.position << ';' << player.status << '\n';
	}

	tmpFile.close();
	binaryFile.close();

	return 0;
}
