//
// Created by User on 23.04.2021.
//

#ifndef MOUNTAIN_QUEST_H
#define MOUNTAIN_QUEST_H

#include <string>
#include <vector>
#include <fstream>

struct QuestFile {
    std::u8string path;
    std::u8string content;
    std::u8string name;
};

struct QuestDirectory {
public:
    std::vector<QuestFile> files;
    std::u8string name;
};

class Quest {

public:
    std::u8string path;
    std::u8string answer;
    std::u8string name;
    std::u8string description;
    std::vector<QuestDirectory> directories;

};


#endif //MOUNTAIN_QUEST_H
