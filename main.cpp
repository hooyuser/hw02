/* 基于智能指针实现双向链表 */
#include "List.hpp"
#include "Printer.hpp"

#include <iostream>

int main() {

	List<int> a;

	a.push_front(1, 4, 9, 2, 8, 5, 7);

	/*a.push_front(7);
	a.push_front(5);
	a.push_front(8);
	a.push_front(2);
	a.push_front(9);
	a.push_front(4);
	a.push_front(1);*/

	std::cout << a << std::endl;   // [ 1 4 9 2 8 5 7 ]

	a.at(2)->erase();

	std::cout << a << std::endl;   // [ 1 4 2 8 5 7 ]

	List<int> b = a;

	a.at(3)->erase();

	std::cout << a << std::endl;   // [ 1 4 2 5 7 ]
	std::cout << b << std::endl;   // [ 1 4 2 8 5 7 ]

	b = {};
	a = {};

	return 0;
}
