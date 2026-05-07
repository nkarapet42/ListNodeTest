#pragma once

#include "ListNode.hpp"

#include <unordered_map>
#include <exception>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

class ListSerializer {
private:
    ListSerializer() = delete;

public:

    static ListNode* LoadFile(const std::string& input);

    static void Serialize(ListNode* head, const std::string& filename);
    static ListNode* Deserialize(const std::string& filename);

    static void PrintListToFile(ListNode* head, const std::string& filename);
    static void FreeList(ListNode* head);
};