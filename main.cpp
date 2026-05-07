#include "ListSerializer.hpp"

int main( void ) {
    try {
        ListNode* head =
            ListSerializer::LoadFile("inlet.in");

        ListSerializer::PrintListToFile(head, "list_output.txt");  
        
        ListSerializer::Serialize(head, "outlet.out");

        ListSerializer::FreeList(head);

        ListNode* restored =
            ListSerializer::Deserialize("outlet.out");

        ListSerializer::PrintListToFile(restored, "restored_output.txt");

        ListSerializer::FreeList(restored);
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}