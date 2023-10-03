// MathLibrary.cpp : Defines the exported functions for the DLL.
#include "pch.h"
#include "MainLibrary.h"

#include <unordered_map>

int count_сharacters(const std::string& text, char targetChar) {
    int count = 0;
    for (char c : text) {
        if (c == targetChar) {
            count++;
        }
    }
    return count;
}

char most_frequent_character(const std::string& text) {
    std::unordered_map<char, int> charFrequency;
    char mostFrequent = '\0';
    int maxFrequency = 0;

    for (char c : text) {
        charFrequency[c]++;
        if (charFrequency[c] > maxFrequency) {
            maxFrequency = charFrequency[c];
            mostFrequent = c;
        }
    }

    return mostFrequent;
}