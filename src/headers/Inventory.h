#pragma once
#include "Key.h"
#include <iostream>
#include <string>
class Inventory {
public:

private:
	static bool key1;
	static bool key2;
	static bool key3;
	static bool key4;
private:
	Inventory();

public:
	static void findKey(Key* aKey);
	static bool getKey(std::string );
};