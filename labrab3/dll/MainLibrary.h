#pragma once

/*
 *	Вариант #7 
 *	Создать библиотеку, считающую количество указанных символов в тексте,
 *	а также находящую самый часто встречающийся символ.
 */

#include <iostream>

//A good tutorial: https://learn.microsoft.com/en-us/cpp/build/walkthrough-creating-and-using-a-dynamic-link-library-cpp?view=msvc-170
#ifdef MAINLIBRARY_EXPORTS
#define MAINLIBRARY_API __declspec(dllexport)
#else
#define MAINLIBRARY_API __declspec(dllimport)
#endif

// Функция для подсчета количества указанных символов в тексте
extern "C" MAINLIBRARY_API int count_сharacters(const std::string& text, char targetChar);

// Функция для нахождения самого часто встречающегося символа
extern "C" MAINLIBRARY_API char most_frequent_character(const std::string& text);
