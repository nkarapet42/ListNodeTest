#include "ListSerializer.hpp"


ListNode* ListSerializer::LoadFile(const std::string& input) {
    std::ifstream inputFile(input);
    if (!inputFile.is_open()) throw std::runtime_error("Cannot open input file");

    std::vector<ListNode*> nodes;
    std::vector<int32_t> randIndices;

    std::string line;
    
    while (std::getline(inputFile, line)) {
        size_t separator = line.rfind(';');
        
        if (separator == std::string::npos) throw std::runtime_error("Invalid line format");
        
        std::string data = line.substr(0, separator);
        if (data.empty()) {
            ListSerializer::FreeList(nodes.empty() ? nullptr : nodes[0]);
            throw std::runtime_error("Data cannot be empty");
        }
        if (data.size() > 1000) {
            ListSerializer::FreeList(nodes.empty() ? nullptr : nodes[0]);
            throw std::runtime_error("Data exceeds maximum allowed length");
        }
        std::string randPart = line.substr(separator + 1);

        try {
            std::stoll(randPart);
        } catch (const std::exception&) {
            ListSerializer::FreeList(nodes.empty() ? nullptr : nodes[0]);
            throw std::runtime_error("Invalid rand index format");
        }
        int32_t randIndex = std::stoll(randPart);
        
        ListNode* node = new ListNode();
        if (!node) {
            ListSerializer::FreeList(nodes.empty() ? nullptr : nodes[0]);
            throw std::runtime_error("Memory allocation failed");
        }
        node->data = std::move(data);
        
        nodes.push_back(node);
        randIndices.push_back(randIndex);
    }
    
    for (size_t i = 0; i < nodes.size(); ++i) {
        if (i > 0) nodes[i]->prev = nodes[i - 1];
        if (i + 1 < nodes.size()) nodes[i]->next = nodes[i + 1];
    }

    for (size_t i = 0; i < nodes.size(); ++i) {
        int32_t ri = randIndices[i];
        
        if (ri == -1) {
            nodes[i]->rand = nullptr;
        } else {
            if (ri < 0 || ri >= static_cast<int32_t>(nodes.size())) {
                ListSerializer::FreeList(nodes.empty() ? nullptr : nodes[0]);
                throw std::runtime_error("Invalid rand index");
            }
            nodes[i]->rand = nodes[ri];
        }
    }
    
    return nodes.empty() ? nullptr : nodes[0];
}

void ListSerializer::Serialize(ListNode* head, const std::string& filename) {
    std::ofstream out(filename, std::ios::binary);
    if (!out.is_open()) throw std::runtime_error("Cannot open output file");

    std::unordered_map<ListNode*, int32_t> nodeToIndex;
    std::vector<ListNode*> nodes;
    ListNode* current = head;
    while (current) {
        nodes.push_back(current);
        nodeToIndex[current] = nodes.size() - 1;
        current = current->next;
    }
    
    uint32_t count = nodes.size();
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));
    
    for (ListNode* node : nodes) {
        uint32_t strSize = static_cast<uint32_t>(node->data.size());
        out.write(reinterpret_cast<const char*>(&strSize), sizeof(strSize));
        out.write(node->data.data(), strSize);
        
        int32_t randIndex = -1;
        if (node->rand) randIndex = static_cast<int32_t>(nodeToIndex[node->rand]);
        out.write(reinterpret_cast<const char*>(&randIndex), sizeof(randIndex));
    }
    
}

ListNode* ListSerializer::Deserialize(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) throw std::runtime_error("Cannot open binary file");

    uint32_t count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));

    std::vector<ListNode*> nodes(count);
    std::vector<int32_t> randIndices(count);
    
    for (uint32_t i = 0; i < count; ++i) {
        ListNode* node = new ListNode();
        if (!node) {
            ListSerializer::FreeList(nodes.empty() ? nullptr : nodes[0]);
            throw std::runtime_error("Memory allocation failed");
        }
        
        uint32_t strSize = 0;
        in.read(reinterpret_cast<char*>(&strSize), sizeof(strSize));
        if (strSize > 1000) {
            delete node;
            ListSerializer::FreeList(nodes.empty() ? nullptr : nodes[0]);
            throw std::runtime_error("Data exceeds maximum allowed length");
        }
        
        node->data.resize(strSize);
        in.read(&node->data[0], strSize);
        
        int32_t randIndex;
        in.read(reinterpret_cast<char*>(&randIndex), sizeof(randIndex));
        randIndices[i] = randIndex;
        
        nodes[i] = node;
    }
    
    for (uint32_t i = 0; i < count; ++i) {
        if (i > 0) nodes[i]->prev = nodes[i - 1];
        if (i + 1 < count) nodes[i]->next = nodes[i + 1];
    }

    for (uint32_t i = 0; i < count; ++i) {
        int32_t ri = randIndices[i];
        if (ri != -1) {
            if (ri < 0 || ri >= static_cast<int32_t>(count)) {
                ListSerializer::FreeList(nodes.empty() ? nullptr : nodes[0]);
                throw std::runtime_error("Corrupted rand index");
            }
            nodes[i]->rand = nodes[ri];
        }
    }

    return nodes.empty() ? nullptr : nodes[0];
}

void ListSerializer::PrintListToFile(ListNode* head, const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) throw std::runtime_error("Cannot open output file");

    ListNode* current = head;
    while (current) {
        out << "Data: " << current->data << ", Rand: "
            << (current->rand ? current->rand->data : "nullptr") << std::endl;
        current = current->next;
    }
}

void ListSerializer::FreeList(ListNode* head) {
    while (head) {
        ListNode* next = head->next;
        delete head;
        head = next;
    }
}