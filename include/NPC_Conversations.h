#include <unordered_map>
#include <string>
//
const std::unordered_map<std::string, const std::unordered_map<char, std::string>> NPC_CONVERSATIONS({
    {"npc1", std::unordered_map<char, std::string>({
        {' ', "Can't you talk?"},
        {'h', "Hello world"},
        {'b', "Bye world"}
        })}
    });
