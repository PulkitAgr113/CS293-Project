#include "stack.h"
#include <cstddef>

pixels_data::pixels_data() {
    // Initially no previous neighbor
    prev = NULL;
}

stack_pixels::stack_pixels() {
    // When stack is empty, topmost element is null
    top = NULL;
    size = 0;
}

void stack_pixels::push(pixels_data* elem) {
    // If stack is empty, then add elem to the stack, and put top at elem
    if(size == 0) {
        top = elem;
    }

    // Else if there is already an element in the stack, then the repvious element of elem is the current top
    // Now update top with the value of elem
    else {
        elem -> prev = top;
        top = elem;
    }

    // Increase number of elements in stack by 1
    size ++;
}

// pop is run under the assumption that stack has at least one element
// This assumption is ensured whenever the function pop is called
pixels_data stack_pixels::pop() {
    // If there is only one element in the stack, then return this element and make top null
    if(size == 1) {
        pixels_data temp = *top;
        top = NULL;
        size --;
        return temp;
    }

    // Else decrease size by 1, and make top point to its previous element
    // Return contents of original top element
    size --;
    top = top -> prev;
    return *top;
}
