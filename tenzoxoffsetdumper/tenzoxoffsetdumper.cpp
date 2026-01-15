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
    std::vector<std::tuple<std::string, std::string, std::string>> targets_normal = {
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
        {"Player", "m_WeaponData", "Player_WeaponData"},
        {"Weapon", "<FireComponent>k__BackingField", "WeaponData"},
        {"WeaponData", "IntWeaponType", "IntWeaponType"},
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

    std::vector<std::tuple<std::string, std::string, std::string>> targets_encrypted = {
        {"MatchGame", "m_Match", "CurrentMatch"},
        {"NFJPHMKKEBF", "LICPHHNNPPF ILGECLEFCCO", "MatchStatus"},
        {"NFJPHMKKEBF", "Player FJPEHEGICBO", "LocalPlayer"},
        {"AttackableEntity", "bool FHMPKFMFEPM", "Player_IsDead"},
        {"Player", "string OIAJCBLDHKP", "Player_Name"},
        {"PlayerNetwork.HHCBNAPCKHF", "FBCAHNCLMDC ADFIDIPODGK", "XPose"},
        {"Player", "AvatarManager FOGJNGDMJKJ", "AvatarManager"},
        {"AvatarManager", "IUmaAvatar EEAGBKBMBLD", "Avatar"},
        {"Player", "FollowCamera CHDOHNOEBML", "FollowCamera"},
        {"Player", "Quaternion <KCFEHMAIINO>k__BackingField", "AimRotation"},
        {"Player", "GPBDEDFKJNA ActiveUISightingWeapon", "Weapon"},
        {"Player", "int KDKFDCPBIGE", "Player_WeaponData"},
        {"GPBDEDFKJNA", "CHEJCCHHDMH <NOAOCMKGLAH>k__BackingField", "WeaponData"},
        {"GPBDEDFKJNA", "OOIPMACFIFL LAEMLAPIAFD", "IntWeaponType"},
        {"OACEDDHKLIM", "float EFMCDHABKGP", "WeaponRecoil"},
        {"Player", "bool <LPEIEILIKGC>k__BackingField", "sAim1"},
        {"Player", "MADMMIICBNN GEGFCFDGGGP", "sAim2"},
        {"MADMMIICBNN", "Vector3 BOGOIAMJFDN", "sAim3"},
        {"MADMMIICBNN", "Vector3 NHKKHPLFMNG", "sAim4"},
        {"NFJPHMKKEBF", "FNCMBMMKLLI BGGJJKKKFDC", "CurrentObserver"},
        {"FNCMBMMKLLI", "Player NJMDHHGDNPJ", "ObserverPlayer"},
        {"Player", "Collider HECFNHJKOMN", "AimbotVisible"},
        {"UGCAPIGenerate", "<>f__mg$cache26", "ViewMatrixV1"},
        {"UGCAPIGenerate", "<>f__mg$cache9", "ViewMatrixV2"},
        {"PlayerAttributes", "ShootNoReload", "NoReload"},
        {"TimeService", "m_DeltaTime", "GameTimer"},
        {"TimeService", "m_FixedDeltaTime", "FixedDeltaTime"}
    };

    std::vector<std::tuple<std::string, std::string, std::string>> bones_normal = {
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

    std::vector<std::tuple<std::string, std::string, std::string>> bones_encrypted = {
        {"Player", "OLCJOGDHJJJ", "head"},
        {"Player", "OLJBCONDGLO", "hip"},
        {"Player", "HCLMADAFLPD", "chest"},
        {"Player", "MPJBGDJJJMJ", "root"},
        {"Player", "BMGCHFGEDDA", "leftankle"},
        {"Player", "AGHJLIMNPJA", "rightankle"},
        {"Player", "FDMBKCKMODA", "lefttoe"},
        {"Player", "CKABHDJDMAP", "righttoe"},
        {"Player", "LIBEIIIAGIK", "leftshoulder"},
        {"Player", "HDEPJIBNIIK", "rightshoulder"},
        {"Player", "NJDDAPKPILB", "righthand"},
        {"Player", "JHIBMHEMJOL", "lefthand"},
        {"Player", "JBACCHNMGNJ", "rightforearm"},
        {"Player", "FGECMMJKFNC", "leftforearm"}
    };

    std::map<std::string, std::set<std::string>> group_labels = {
        {"// General", {"DictionaryEntities", "MatchStatus", "CurrentMatch", "LocalPlayer"}},
        {"// Player", {"Player_Data", "Avatar_Data", "Avatar", "Player_Name", "CurrentMatch", "Player_IsDead", "Avatar_IsVisible", "Player_ShadowBase", "XPose", "AvatarManager", "Avatar_Data_IsTeam", "Player_WeaponData"}},
        {"// Camera", {"MainCameraTransform", "Camera", "FollowCamera", "AimRotation"}},
        {"// Armas", {"WeaponData", "ViewMatrixV2", "WeaponRecoil", "Weapon", "ViewMatrixV1", "IntWeaponType"}},
        {"// Silent Aim", {"sAim2", "sAim3", "sAim4", "sAim1"}},
        {"// ESP IN OBSERVE", {"CurrentObserver", "ObserverPlayer"}},
        {"// Aimbot Visible", {"AimbotVisible"}},
        {"// Speed Internal", {"FixedDeltaTime", "GameTimer"}}
    };

    bool isEncryptedMode = false;

public:
    void setConsoleSize(int columns = 130, int lines = 30) {
        std::string command = "mode con: cols=" + std::to_string(columns) + " lines=" + std::to_string(lines);
        system(command.c_str());
    }

    void typewrite(const std::string& text, int delay_ms = 50) {
        for (char c : text) {
            std::cout << c << std::flush;
            Sleep(delay_ms);
        }
        std::cout << std::endl;
    }

    void printBanner() {
        std::cout << "==============================================================" << std::endl;
        std::cout << "                   TENZO X OFFSET DUMPER" << std::endl;
        std::cout << "==============================================================" << std::endl;
        std::cout << std::endl;
    }

    void setEncryptionMode(bool encrypted) {
        isEncryptedMode = encrypted;
        if (encrypted) {
            std::cout << "MODE: Encrypted dump mode selected" << std::endl;
        }
        else {
            std::cout << "MODE: Normal dump mode selected" << std::endl;
        }
        std::cout << std::endl;
    }

    std::string toLower(const std::string& str) {
        std::string result = str;
        for (char& c : result) {
            c = std::tolower(static_cast<unsigned char>(c));
        }
        return result;
    }

    std::string trim(const std::string& str) {
        size_t start = str.find_first_not_of(" \t\n\r");
        size_t end = str.find_last_not_of(" \t\n\r");
        if (start == std::string::npos) return "";
        return str.substr(start, end - start + 1);
    }

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
            hex_pos = line.find("0x", field_pos);
            if (hex_pos == std::string::npos) {
                return "";
            }
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

        const auto& targets = isEncryptedMode ? targets_encrypted : targets_normal;

        for (const auto& target : targets) {
            std::string class_def = std::get<0>(target);
            std::string field = std::get<1>(target);
            std::string result_name = std::get<2>(target);

            bool inside_class = false;
            int brace_count = 0;
            bool found = false;

            for (const auto& current_line : lines) {
                std::string trimmed_line = trim(current_line);

                if (!inside_class) {
                    if (isEncryptedMode) {
                        if (trimmed_line.find("class " + class_def) != std::string::npos ||
                            trimmed_line.find("internal class " + class_def) != std::string::npos ||
                            trimmed_line.find("public class " + class_def) != std::string::npos ||
                            trimmed_line.find("private class " + class_def) != std::string::npos) {
                            inside_class = true;
                            brace_count = countOccurrences(current_line, '{') - countOccurrences(current_line, '}');
                            continue;
                        }
                    }
                    else {
                        if (trimmed_line.find("class " + class_def) != std::string::npos) {
                            inside_class = true;
                            brace_count = countOccurrences(current_line, '{') - countOccurrences(current_line, '}');
                            continue;
                        }
                    }
                }

                if (inside_class) {
                    brace_count += countOccurrences(current_line, '{') - countOccurrences(current_line, '}');

                    if (brace_count <= 0) {
                        inside_class = false;
                        continue;
                    }

                    std::string offset;
                    if (isEncryptedMode) {
                        if (current_line.find(field) != std::string::npos) {
                            offset = extractOffset(current_line, field);
                        }
                    }
                    else {
                        offset = extractOffset(current_line, field);
                    }

                    if (!offset.empty()) {
                        extracted.push_back({ result_name, toLower(offset) });
                        found = true;
                        std::cout << "FOUND: " << result_name << " = 0x" << offset;
                        if (isEncryptedMode) {
                            std::cout << " (Encrypted: " << class_def << "::" << field << ")";
                        }
                        std::cout << std::endl;
                        break;
                    }
                }
            }

            if (!found) {
                std::cout << "NOT FOUND: " << class_def << "::" << field;
                if (isEncryptedMode) {
                    std::cout << " (Encrypted mode)";
                }
                std::cout << std::endl;
            }
        }

        return extracted;
    }

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

        const auto& bones_targets = isEncryptedMode ? bones_encrypted : bones_normal;

        for (const auto& target : bones_targets) {
            std::string class_def = std::get<0>(target);
            std::string field = std::get<1>(target);
            std::string result_name = std::get<2>(target);

            bool inside_class = false;
            int brace_count = 0;
            bool found = false;

            for (const auto& current_line : lines) {
                std::string trimmed_line = trim(current_line);

                if (!inside_class) {
                    if (isEncryptedMode) {
                        if (trimmed_line.find("class " + class_def) != std::string::npos ||
                            trimmed_line.find("internal class " + class_def) != std::string::npos ||
                            trimmed_line.find("public class " + class_def) != std::string::npos) {
                            inside_class = true;
                            brace_count = countOccurrences(current_line, '{') - countOccurrences(current_line, '}');
                            continue;
                        }
                    }
                    else {
                        if (trimmed_line.find("class " + class_def) != std::string::npos) {
                            inside_class = true;
                            brace_count = countOccurrences(current_line, '{') - countOccurrences(current_line, '}');
                            continue;
                        }
                    }
                }

                if (inside_class) {
                    brace_count += countOccurrences(current_line, '{') - countOccurrences(current_line, '}');

                    if (brace_count <= 0) {
                        inside_class = false;
                        continue;
                    }

                    std::string offset;
                    if (isEncryptedMode) {
                        if (current_line.find(field) != std::string::npos) {
                            offset = extractOffset(current_line, field);
                        }
                    }
                    else {
                        offset = extractOffset(current_line, field);
                    }

                    if (!offset.empty()) {
                        extracted.push_back({ result_name, toLower(offset) });
                        found = true;
                        std::cout << "FOUND BONE: " << result_name << " = 0x" << offset;
                        if (isEncryptedMode) {
                            std::cout << " (Encrypted: " << class_def << "::" << field << ")";
                        }
                        std::cout << std::endl;
                        break;
                    }
                }
            }

            if (!found) {
                std::cout << "BONE NOT FOUND: " << class_def << "::" << field;
                if (isEncryptedMode) {
                    std::cout << " (Encrypted mode)";
                }
                std::cout << std::endl;
            }
        }

        return extracted;
    }

    void writeOffsets(const std::vector<std::pair<std::string, std::string>>& extracted, const std::string& outputFile) {
        std::ofstream file(outputFile);
        if (!file.is_open()) {
            std::cout << "ERROR: Cannot create file: " << outputFile << std::endl;
            return;
        }

        file << "namespace TXC\n";
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
    int countOccurrences(const std::string& str, char ch) {
        int count = 0;
        for (char c : str) {
            if (c == ch) count++;
        }
        return count;
    }

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

    dumper.setConsoleSize(130, 30);
    system("title TENZO X OFFSET DUMPER");

    dumper.printBanner();

    std::string dumpFilePath;
    std::cout << "Enter the path to your dump.cs file: ";
    std::getline(std::cin, dumpFilePath);

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

    std::cout << "Is this an ENCRYPTED dump file? (y/n): ";
    std::string modeInput;
    std::getline(std::cin, modeInput);

    bool isEncrypted = false;
    if (modeInput == "y" || modeInput == "Y" || modeInput == "yes" || modeInput == "YES") {
        isEncrypted = true;
        std::cout << "Using ENCRYPTED mode with obfuscated strings..." << std::endl;
    }
    else {
        std::cout << "Using NORMAL mode with standard strings..." << std::endl;
    }
    std::cout << std::endl;

    dumper.setEncryptionMode(isEncrypted);

    dumper.typewrite("Welcome to Tenzo X Offset Dumper!");
    Sleep(500);

    dumper.typewrite("Analyzing dump file...");
    Sleep(500);

    std::cout << "Dumping Offsets..." << std::endl;
    std::cout << std::endl;

    auto extracted = dumper.extractOffsets(dumpFilePath);
    dumper.writeOffsets(extracted, "offsets.txt");

    Sleep(500);
    std::cout << std::endl;
    std::cout << "SUCCESS: Dumped " << extracted.size() << " offsets to 'offsets.txt'" << std::endl;

    std::cout << std::endl;
    std::cout << "Dumping Bones..." << std::endl;
    std::cout << std::endl;

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
