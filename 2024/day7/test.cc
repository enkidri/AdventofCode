#include <iostream>
#include <string>
using namespace std;

// Recursive function to evaluate expressions
bool findExpression(int nums[], int index, int size, int current, string expression, int target) {
    if (index == size) {
        // Base case: if we've used all numbers, check if the current result matches the target
        if (current == target) {
            cout << "Expression: " << expression << " = " << target << endl;
            return true;
        }
        return false;
    }

    // Try adding the next number
    if (findExpression(nums, index + 1, size, current + nums[index], expression + " + " + to_string(nums[index]), target)) {
        return true;
    }

    // Try multiplying the next number
    if (findExpression(nums, index + 1, size, current * nums[index], expression + " * " + to_string(nums[index]), target)) {
        return true;
    }

    return false; // If neither addition nor multiplication works
}

int main() {
    int nums[] = { 81, 40, 27 };
    int target = 3267;

    // Start the recursion with the first number
    if (!findExpression(nums, 1, 3, nums[0], to_string(nums[0]), target)) {
        cout << "No valid expression found." << endl;
    }

    return 0;
}
