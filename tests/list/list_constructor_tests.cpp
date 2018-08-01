#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../../include/list.h"
#include "../test_class.h"
#include "../test_allocator.h"

BOOST_AUTO_TEST_SUITE(list)

BOOST_AUTO_TEST_CASE(empty_list)
{
	blk::list<int> list;
}

BOOST_AUTO_TEST_CASE(empty_list_with_allocator)
{
	blk::list<TestClass, std::allocator<TestClass>> list;
}

BOOST_AUTO_TEST_CASE(for_empty_list_begin_is_end)
{
	blk::list<int> list;
	
	BOOST_CHECK(list.begin() == list.end());
	BOOST_CHECK(list.cbegin() == list.cend());
	BOOST_CHECK(list.rbegin() == list.rend());
	BOOST_CHECK(list.crbegin() == list.crend());
}

BOOST_AUTO_TEST_CASE(list_with_n_copies)
{
	int n = 100;
	int value = 1;
	blk::list<int> list(n, value);
	BOOST_CHECK(list.size() == n);
	auto it = list.begin();
	for (int i = 0; i < n; i++, ++it)
		BOOST_CHECK(*it == value);
	BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_CASE(list_with_n_default_copies)
{
	int n = 100;
	blk::list<int> list(n);
	BOOST_CHECK(list.size() == n);
	auto it = list.begin();
	for (int i = 0; i < n; i++, ++it)
		BOOST_CHECK(*it == int());
	BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_CASE(move_list)
{
	int n = 10;
	blk::list<TestClass> l1;
	for (int i = 0; i < 10; i++)
		l1.emplace_back(i);
	blk::list<TestClass> l2 = std::move(l1);
	BOOST_CHECK(l2.size() == n);
	auto it = l2.begin();
	for (int i = 0; i < n; i++, ++it)
		BOOST_CHECK(it->getValue() == i);
	BOOST_CHECK(it == l2.end());
}

BOOST_AUTO_TEST_CASE(move_list_with_same_allocator)
{
	int n = 10;
	blk::list<TestClass> l1;
	for (int i = 0; i < 10; i++)
		l1.emplace_back(i);
	blk::list<TestClass> l2(std::move(l1), std::allocator<TestClass>());
	BOOST_CHECK(l2.size() == n);
	auto it = l2.begin();
	for (int i = 0; i < n; i++, ++it)
		BOOST_CHECK(it->getValue() == i);
	BOOST_CHECK(it == l2.end());
}

BOOST_AUTO_TEST_CASE(move_list_with_different_allocator)
{
	int n = 10;
	blk::list<TestClass, TestAllocator<TestClass>> l1(TestAllocator<TestClass>(0));
	for (int i = 0; i < 10; i++)
		l1.emplace_back(i);
	blk::list<TestClass, TestAllocator<TestClass>> l2(std::move(l1), TestAllocator<TestClass>(1));
	BOOST_CHECK(l2.size() == n);
	auto it = l2.begin();
	for (int i = 0; i < n; i++, ++it)
		BOOST_CHECK(it->getValue() == i);
	BOOST_CHECK(it == l2.end());
}

BOOST_AUTO_TEST_CASE(list_from_range)
{
	int n = 10;
	blk::list<TestClass> l1;
	for (int i = 0; i < n; i++)
		l1.emplace_back(i);
	blk::list<TestClass> l2(l1.begin(), l1.end());
	BOOST_CHECK(l2.size() == n);
	auto it = l2.begin();
	for (int i = 0; i < n; i++, ++it)
		BOOST_CHECK(it->getValue() == i);
	BOOST_CHECK(it == l2.end());
}

BOOST_AUTO_TEST_CASE(list_copy)
{
	int n = 10;
	blk::list<TestClass> l1;
	for (int i = 0; i < n; i++)
		l1.emplace_back(i);
	blk::list<TestClass> l2(l1);
	BOOST_CHECK(l1.size() == n);
	BOOST_CHECK(l2.size() == n);
	auto it1 = l1.begin();
	auto it2 = l2.begin();
	for (int i = 0; i < n; i++, ++it1, ++it2)
	{
		BOOST_CHECK(it1->getValue() == i);
		BOOST_CHECK(it2->getValue() == i);
	}
	BOOST_CHECK(it1 == l1.end());
	BOOST_CHECK(it2 == l2.end());
}

BOOST_AUTO_TEST_CASE(list_copy_with_allocator)
{
	int n = 10;
	blk::list<TestClass> l1;
	for (int i = 0; i < n; i++)
		l1.emplace_back(i);
	blk::list<TestClass> l2(l1, std::allocator<TestClass>());
	BOOST_CHECK(l1.size() == n);
	BOOST_CHECK(l2.size() == n);
	auto it1 = l1.begin();
	auto it2 = l2.begin();
	for (int i = 0; i < n; i++, ++it1, ++it2)
	{
		BOOST_CHECK(it1->getValue() == i);
		BOOST_CHECK(it2->getValue() == i);
	}
	BOOST_CHECK(it1 == l1.end());
	BOOST_CHECK(it2 == l2.end());
}

BOOST_AUTO_TEST_CASE(list_from_initializer_list)
{
	blk::list<TestClass> list { TestClass(0), TestClass(1), TestClass(2), TestClass(3), TestClass(4) };
	BOOST_CHECK(list.size() == 5);
	auto it = list.begin();
	for (int i = 0; i < 5; i++, ++it)
		BOOST_CHECK(it->getValue() == i);
	BOOST_CHECK(it == list.end());
}

BOOST_AUTO_TEST_SUITE_END()