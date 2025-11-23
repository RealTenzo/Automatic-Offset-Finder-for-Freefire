#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include <map>
#include <set>
#include <algorithm>
#include <cctype>

class TenzoXOffsetDumper {
private:
    // all the stuff we gona search in dump, class, field, name we call it
    std::vector<std::tuple<std::string, std::string, std::string>> targets = {
        {"MatchGame", "m_Match", "CurrentMatch"},
        {"Match", "m_State", "MatchStatus"},
        {"Match", "m_LocalPlayer", "LocalPlayer"},
        {"MatchGame", "m_ReplicationEntitis", "DictionaryEntities"},
        {"AttackableEntity", "m_IsDead", "Player_IsDead"},
        {"Player", "m_NickName", "Player_Name"},
        {"ReplicationEntity", "m_PRIDataPool", "Player_Data"},
        {"PlayerNetwork", "m_ShadowState", "Player_ShadowBase"},
        {"ShadowState", "TargetPhysXPose", "XPose"},
        {"PlayerNetwork.ShadowState", "TargetPhysXPose", "XPose"},
        {"Player", "m_AvatarManager", "AvatarManager"},
        {"AvatarManager", "m_Avatar", "Avatar"},
        {"UmaAvatarSimple", "IsVisible", "Avatar_IsVisible"},
        {"UMAAvatarBase", "umaData", "Avatar_Data"},
        {"UMAData", "isTeammate", "Avatar_Data_IsTeam"},
        {"Player", "m_FollowCamera", "FollowCamera"},
        {"VehicleMarkController", "m_mainCamera", "Camera"},
        {"Player", "<m_AimRotation>k__BackingField", "AimRotation"},
        {"Player", "MainCameraTransform", "MainCameraTransform"},
        {"Player", "ActiveUISightingWeapon", "Weapon"},
        {"Weapon", "<FireComponent>k__BackingField", "WeaponData"},
        {"WeaponFireComponentLongRangeBase", "tangentTheta", "WeaponRecoil"},
        {"UGCAPIGenerate", "<>f__mg$cache26", "ViewMatrixV1"},
        {"UGCAPIGenerate", "<>f__mg$cache9", "ViewMatrixV2"},
        {"Player", "<IsPrepareAttack>k__BackingField", "sAim1"},
        {"Player", "m_LastAimingInfoFromWeapon", "sAim2"},
        {"HitObjectInfo", "StartPosition", "sAim3"},
        {"HitObjectInfo", "RayDir", "sAim4"},
        {"Match", "m_LocalObserver", "CurrentObserver"},
        {"Observer", "m_TargetPlayer", "ObserverPlayer"},
        {"PlayerAttributes", "ShootNoReload", "NoReload"},
        {"Player", "m_Attributes", "PlayerAttributes"},
        {"Player", "m_HeadCollider", "AimbotVisible"},
        {"TimeService", "m_DeltaTime", "GameTimer"},
        {"TimeService", "m_FixedDeltaTime", "FixedDeltaTime"}
    };

    // bones list, same format
    std::vector<std::tuple<std::string, std::string, std::string>> bones_targets = {
        {"Player", "HeadNode", "Head"},
        {"Player", "m_BloodEffectNode", "Breast"},
        {"Player", "m_RootNode", "Root"},
        {"Player", "HipNode", "hip"},
        {"Player", "m_LeftHandNode", "LeftHand"},
        {"Player", "m_RightHandNode", "RightHand"},
        {"Player", "m_LeftArmNode", "LeftShoulder"},
        {"Player", "m_RightArmNode", "RightShoulder"},
        {"Player", "m_RightToeNode", "LeftFoot"},
        {"Player", "m_LeftToeNode", "RightFoot"},
        {"Player", "m_RightAnkleNode", "LeftLeg"},
        {"Player", "m_LeftAnkleNode", "RightLeg"},
        {"Player", "m_RightForeArmNode", "rightankle"},
        {"Player", "m_LeftForeArmNode", "leftankle"}
    };

    // grouping labels for output so file look organized
    std::map<std::string, std::set<std::string>> group_labels = {
        {"// General", {"DictionaryEntities", "MatchStatus", "CurrentMatch", "LocalPlayer"}},
        {"// Player", {"Player_Data", "Avatar_Data", "Avatar", "Player_Name", "CurrentMatch", "Player_IsDead", "Avatar_IsVisible", "Player_ShadowBase", "XPose", "AvatarManager", "Avatar_Data_IsTeam"}},
        {"// Camera", {"MainCameraTransform", "Camera", "FollowCamera", "AimRotation"}},
        {"// Armas", {"WeaponData", "ViewMatrixV2", "WeaponRecoil", "Weapon", "ViewMatrixV1"}},
        {"// Silent Aim", {"sAim2", "sAim3", "sAim4", "sAim1"}},
        {"// ESP IN OBSERVE", {"CurrentObserver", "ObserverPlayer"}},
        {"// Aimbot Visible", {"AimbotVisible"}},
        {"// Speed Internal", {"FixedDeltaTime", "GameTimer"}}
    };

public:
    // change console size so text fit nicer
    void setConsoleSize(int columns = 130, int lines = 30) {
        std::string command = "mode con: cols=" + std::to_string(columns) + " lines=" + std::to_string(lines);
        system(command.c_str());
    }

    // type text slow for drama lol
    void typewrite(const std::string& text, int delay_ms = 50) {
        for (char c : text) {
            std::cout << c << std::flush;
            Sleep(delay_ms);
        }
        std::cout << std::endl;
    }

    // banner for program look cool
    void printBanner() {
        std::cout << "==============================================================" << std::endl;
        std::cout << "                   TENZO X OFFSET DUMPER" << std::endl;
        std::cout << "==============================================================" << std::endl;
        std::cout << std::endl;
    }

    // lowercase helper
    std::string toLower(const std::string& str) {
        std::string result = str;
        for (char& c : result) {
            c = std::tolower(static_cast<unsigned char>(c));
        }
        return result;
    }

    // trim spaces cus dumps messy
    std::string trim(const std::string& str) {
        size_t start = str.find_first_not_of(" \t\n\r");
        size_t end = str.find_last_not_of(" \t\n\r");
        if (start == std::string::npos) return "";
        return str.substr(start, end - start + 1);
    }

    // look in a line for field then find 0x after ; and return hex digits only
    std::string extractOffset(const std::string& line, const std::string& field) {
        size_t field_pos = line.find(field);
        if (field_pos == std::string::npos) {
            return "";
        }

        size_t semicolon_pos = line.find(';', field_pos);
        if (semicolon_pos == std::string::npos) {
            return "";
        }

        size_t hex_pos = line.find("0x", semicolon_pos);
        if (hex_pos == std::string::npos) {
            return "";
        }

        std::string hex_value;
        for (size_t i = hex_pos + 2; i < line.size(); i++) {
            char c = line[i];
            if (isxdigit(static_cast<unsigned char>(c))) {
                hex_value += c;
            }
            else {
                break;
            }
        }

        return hex_value;
    }

    // parse dump.cs and search for offsets using targets vector
    std::vector<std::pair<std::string, std::string>> extractOffsets(const std::string& filePath) {
        std::vector<std::pair<std::string, std::string>> extracted;

        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cout << "ERROR: Cannot open file: " << filePath << std::endl;
            return extracted;
        }

        std::string line;
        std::vector<std::string> lines;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();

        std::cout << "Reading " << lines.size() << " lines..." << std::endl;

        for (const auto& target : targets) {
            std::string class_def = std::get<0>(target);
            std::string field = std::get<1>(target);
            std::string result_name = std::get<2>(target);

            bool inside_class = false;
            int brace_count = 0;
            bool found = false;

            for (const auto& current_line : lines) {
                std::string trimmed_line = trim(current_line);

                // try find class
                if (!inside_class && trimmed_line.find("class " + class_def) != std::string::npos) {
                    inside_class = true;
                    brace_count = countOccurrences(current_line, '{') - countOccurrences(current_line, '}');
                    continue;
                }

                if (inside_class) {
                    brace_count += countOccurrences(current_line, '{') - countOccurrences(current_line, '}');

                    // left class
                    if (brace_count <= 0) {
                        inside_class = false;
                        continue;
                    }

                    // look for field
                    std::string offset = extractOffset(current_line, field);
                    if (!offset.empty()) {
                        extracted.push_back({ result_name, toLower(offset) });
                        found = true;
                        std::cout << "FOUND: " << result_name << " = 0x" << offset << std::endl;
                        break;
                    }
                }
            }

            if (!found) {
                std::cout << "NOT FOUND: " << class_def << "::" << field << std::endl;
            }
        }

        return extracted;
    }

    // extract bones using bones_targets
    std::vector<std::pair<std::string, std::string>> extractBones(const std::string& filePath) {
        std::vector<std::pair<std::string, std::string>> extracted;

        std::ifstream file(filePath);
        if (!file.is_open()) {
            return extracted;
        }

        std::string line;
        std::vector<std::string> lines;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();

        for (const auto& target : bones_targets) {
            std::string class_def = std::get<0>(target);
            std::string field = std::get<1>(target);
            std::string result_name = std::get<2>(target);

            bool inside_class = false;
            int brace_count = 0;
            bool found = false;

            for (const auto& current_line : lines) {
                std::string trimmed_line = trim(current_line);

                if (!inside_class && trimmed_line.find("class " + class_def) != std::string::npos) {
                    inside_class = true;
                    brace_count = countOccurrences(current_line, '{') - countOccurrences(current_line, '}');
                    continue;
                }

                if (inside_class) {
                    brace_count += countOccurrences(current_line, '{') - countOccurrences(current_line, '}');

                    if (brace_count <= 0) {
                        inside_class = false;
                        continue;
                    }

                    std::string offset = extractOffset(current_line, field);
                    if (!offset.empty()) {
                        extracted.push_back({ result_name, toLower(offset) });
                        found = true;
                        std::cout << "FOUND BONE: " << result_name << " = 0x" << offset << std::endl;
                        break;
                    }
                }
            }

            if (!found) {
                std::cout << "BONE NOT FOUND: " << class_def << "::" << field << std::endl;
            }
        }

        return extracted;
    }

    // write offsets to file with groups
    void writeOffsets(const std::vector<std::pair<std::string, std::string>>& extracted, const std::string& outputFile) {
        std::ofstream file(outputFile);
        if (!file.is_open()) {
            std::cout << "ERROR: Cannot create file: " << outputFile << std::endl;
            return;
        }

        file << "namespace DarknetHaxor\n";
        file << "{\n";
        file << "    internal static class Offsets\n";
        file << "    {\n";

        std::set<std::string> used_groups;

        for (const auto& entry : extracted) {
            std::string name = entry.first;
            std::string offset = entry.second;

            std::string group = findGroup(name);
            if (!group.empty() && used_groups.find(group) == used_groups.end()) {
                file << "\n";
                file << "        " << group << "\n";
                used_groups.insert(group);
            }

            file << "        internal static uint " << name << " = 0x" << offset << ";\n";
        }

        file << "    }\n";
        file << "}\n";
        file.close();
    }

    // write bones simple format
    void writeBones(const std::vector<std::pair<std::string, std::string>>& extracted, const std::string& outputFile) {
        std::ofstream file(outputFile);
        if (!file.is_open()) {
            std::cout << "ERROR: Cannot create file: " << outputFile << std::endl;
            return;
        }

        for (const auto& entry : extracted) {
            file << entry.first << " = 0x" << entry.second << ";\n";
        }
        file.close();
    }

private:
    // little helper to count braces
    int countOccurrences(const std::string& str, char ch) {
        int count = 0;
        for (char c : str) {
            if (c == ch) count++;
        }
        return count;
    }

    // find which group a name belong so we can put label in file
    std::string findGroup(const std::string& name) {
        for (const auto& group : group_labels) {
            if (group.second.find(name) != group.second.end()) {
                return group.first;
            }
        }
        return "";
    }
};

int main() {
    TenzoXOffsetDumper dumper;

    // set console big so text not wrap
    dumper.setConsoleSize(130, 30);
    system("title TENZO X OFFSET DUMPER");

    // banner coz why not
    dumper.printBanner();

    // ask for dump file path, user paste path
    std::string dumpFilePath;
    std::cout << "Enter the path to your dump.cs file: ";
    std::getline(std::cin, dumpFilePath);

    // check file exist else bail
    std::ifstream testFile(dumpFilePath);
    if (!testFile.is_open()) {
        std::cout << std::endl;
        std::cout << "ERROR: File not found: " << dumpFilePath << std::endl;
        std::cout << "Please check the path and try again." << std::endl;
        std::cout << std::endl;
        std::cout << "Press Enter to exit...";
        std::cin.get();
        return 1;
    }
    testFile.close();

    std::cout << std::endl;
    std::cout << "SUCCESS: Found file: " << dumpFilePath << std::endl;
    std::cout << std::endl;

    // some silly messages for user
    dumper.typewrite("Welcome to Tenzo X Offset Dumper!");
    Sleep(500);

    dumper.typewrite("Analyzing dump file...");
    Sleep(500);

    std::cout << "Dumping Offsets..." << std::endl;
    std::cout << std::endl;

    // get offsets and write file
    auto extracted = dumper.extractOffsets(dumpFilePath);
    dumper.writeOffsets(extracted, "offsets.txt");

    Sleep(500);
    std::cout << std::endl;
    std::cout << "SUCCESS: Dumped " << extracted.size() << " offsets to 'offsets.txt'" << std::endl;

    std::cout << std::endl;
    std::cout << "Dumping Bones..." << std::endl;
    std::cout << std::endl;

    // bones
    auto extractedBones = dumper.extractBones(dumpFilePath);
    dumper.writeBones(extractedBones, "bones.txt");

    Sleep(500);
    std::cout << std::endl;
    std::cout << "SUCCESS: Dumped " << extractedBones.size() << " bones to 'bones.txt'" << std::endl;

    std::cout << std::endl;
    std::cout << "FINAL RESULTS:" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "Offsets found: " << extracted.size() << std::endl;
    std::cout << "Bones found: " << extractedBones.size() << std::endl;
    std::cout << "Total: " << (extracted.size() + extractedBones.size()) << std::endl;
    std::cout << std::endl;

    std::cout << "Output files created:" << std::endl;
    std::cout << "- offsets.txt" << std::endl;
    std::cout << "- bones.txt" << std::endl;
    std::cout << std::endl;

    std::cout << "Press Enter to exit...";
    std::cin.get();

    return 0;
}
