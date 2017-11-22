#include "Set.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	Set s;
	assert(s.empty());
	assert(s.size() == 0);
	ItemType xa = 1;
	bool b = s.insert(xa);
	assert(!s.empty());
	assert(s.size() == 1);
	assert(b);
	ItemType xa2 = 10043;
	bool b2 = s.insert(xa2);
	assert(s.size() == 2);
	assert(b2);
	bool b3 = s.insert(xa2);
	assert(s.size() == 2);
	assert(!b3);
	bool b4 = s.erase(xa2);
	assert(s.size() == 1);
	assert(b4);
	bool b5 = s.erase(xa2);
	assert(s.size() == 1);
	assert(!b5);
	s.insert(xa2);
	ItemType xa3 = 2;
	s.insert(xa3);
	bool b6 = s.contains(xa);
	bool b7 = s.contains(xa3);
	bool b8 = s.contains(xa2);
	ItemType xa4 = 999;
	bool b9 = s.contains(xa4);
	assert(b6);
	assert(b7);
	assert(b8);
	assert(!b9);
	s.insert(xa4);
	ItemType type;
	bool b10 = s.get(0, type);
	assert(b10);
	assert(type == xa);
	bool b11 = s.get(1, type);
	assert(b11);
	assert(type == xa3);
	bool b12 = s.get(2, type);
	assert(b12);
	assert(type == xa4);
	bool b13 = s.get(3, type);
	assert(b13);
	assert(type == xa2);
	bool b14 = s.get(-1, type);
	assert(!b14);
	bool b15 = s.get(4, type);
	assert(!b15);
	Set s2;
	ItemType t = 45;
	ItemType t2 = 66;
	ItemType t3 = 69;
	ItemType t4 = 10098;
	ItemType t5 = 666;
	s2.insert(t);
	s2.insert(t2);
	s2.insert(t3);
	s2.insert(t4);
	s2.insert(t5);
	s.swap(s2);
	assert(s.size() == 5);
	assert(s2.size() == 4);
	assert(s.contains(t) && s.contains(t2) && s.contains(t3) && s.contains(t4) && s.contains(t5));
	assert(s2.contains(xa) && s2.contains(xa2) && s2.contains(xa3) && s2.contains(xa4));
}