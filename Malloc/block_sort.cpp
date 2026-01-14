#include "block_sort.h"
#include <cstdint>

// Returns the second half of the list, first half is the original Block pointer given
static Block* split_list(Block* head) {
    Block* fast = head->next;
    Block* slow = head;

    while (fast != nullptr && fast->next != nullptr) {
        fast = fast->next->next;
        slow = slow->next;
    }
    // Once fast is at the end,slow is in the middle, so split

    Block* second_list = slow->next;
    slow->next = nullptr;
    
    return second_list;
}

static Block* merge_lists(Block* list1, Block* list2) {
    if (list1 == nullptr) { return list2; }
    if (list2 == nullptr) { return list1; }

    if (list1->size < list2->size) {
        list1->next = merge_lists(list1->next, list2); // Take the first value of list 1, and prepend to the merge of the remaining 2 lists
        return list1;
    } else {
        list2->next = merge_lists(list2->next, list1);
        return list2;
    }
}

Block* merge_sort(Block* list) {
    // Base case - empty or single value
    if (list == nullptr || list->next == nullptr) {
        return list;
    }

    Block* second_list = split_list(list);

    list = merge_sort(list);
    second_list = merge_sort(second_list);

    return merge_lists(list, second_list);
}