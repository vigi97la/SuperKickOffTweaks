
// From Bing Copilot, with manual changes

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main() {
	int startByte = 164113;
	int endByte = 175825;
    std::vector<std::string> countries = {"England", "France", "Sweden", "Spain", "Holland", "Germany", "Italy", "Brazil"};
    std::vector<int> countriesFirstByte = {startByte, 165482, 166860, 168045, 169478, 171051, 172887, 174303};
    //std::vector<int> countriesFirstPlayer = {1, 190, 382, 575, 768, 958, 1150, 1332};  // There seem to be a variable number of players per country (189-193)

    std::ifstream input("Super Kick Off.sms", std::ios::binary);
    std::ofstream output("players.csv");

    if (!input.is_open() || !output.is_open()) {
        std::cerr << "Error opening files" << std::endl;
        return 1;
    }

    // Move the get pointer to the starting byte
    input.seekg(startByte-1);

    int player = 0;
    int countryIndex = -1;
    int teamCount = 0;
    int playerNumber = 0;
    std::string name;
    char ch;
    for (int i = startByte; i <= endByte; i++) {
        if (countryIndex >= (int)countries.size()) { // If we have processed all countries, stop reading the file
			break;
		}
		if ((countryIndex < (int)countries.size()-1)&&(i == countriesFirstByte[countryIndex+1])) {
			countryIndex++;
			teamCount = 1;
			playerNumber = 0;
		}
        input.get(ch);
        if (ch != '\0') { // If the character is not a NUL char, add it to the current name
            name += ch;
        } else if (!name.empty()) { // If the character is a NUL char and the current name is not empty, write it to the CSV file
            player++;
            playerNumber++;
            std::string teamType = (teamCount == 1) ? "International" : "City "+std::to_string(teamCount-1);
            std::string playerPosition = "Unknown";
            if (teamCount == 1) {
				if (playerNumber <= 2) playerPosition = "Keeper";
				else if (playerNumber <= 8) playerPosition = "Defender";
				else if (playerNumber <= 16) playerPosition = "Midfield";
				else if (playerNumber <= 21) playerPosition = "Forward";
			}
            std::string playerDefaultStatus = "Substitute";
        	if ((playerNumber != 1)&&(playerNumber != 7)&&(playerNumber != 8)&&(playerNumber != 9)&&(playerNumber != 10)&&(playerNumber != 13)&&(playerNumber != 16)&&(playerNumber != 17)&&(playerNumber != 19)&&(playerNumber != 21)) playerDefaultStatus = "Starter";
            output << name << ';' << countries[countryIndex] << ';' << teamType << ';' << playerPosition << ';' << playerDefaultStatus << '\n';
            name.clear();            
            if (playerNumber >= 21) { // If we have processed all players for the current team, move to the next team
                playerNumber = 0;
                teamCount++;
            }
            // Not handled: the first player of the first city appear to be the same as the last player of the international team
        }
    }

    input.close();
    output.close();

    return 0;
}
