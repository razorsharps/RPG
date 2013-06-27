#pragma once
#include "../headers/Inventory.h"

bool Inventory::key1 = false;
bool Inventory::key2 = false;
bool Inventory::key3 = false;
bool Inventory::key4 = false;

void Inventory::findKey(Key * aKey){
	std::cout << " i found a key" << std::endl;
	if( aKey->name == "Key1"){
		key1 = true;
		return;
	}
	if( aKey->name == "Key2"){
		key2 = true;
		return;
	}
	if( aKey->name == "Key3"){
		key3 = true;
		return;
	}
	if( aKey->name == "Key4"){
		key4 = true;
		return;
	}
}

bool Inventory::getKey(std::string aKey) {
	if( aKey == "Key1" && key1){
		return true;
	}
	if( aKey== "Key2" && key2){
		return true;
	}
	if( aKey == "Key3" && key3){
		return true;
	}
	if( aKey == "Key4" && key4){
		return true;
	}
	return false;	
}
