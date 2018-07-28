#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../../include/list.h"
#include "../test_class.h"

BOOST_AUTO_TEST_SUITE(list)

BOOST_AUTO_TEST_CASE(empty_list)
{
	blk::list<int> list;
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
	for (auto item : list)
		BOOST_CHECK(item == value);
}

BOOST_AUTO_TEST_CASE(list_with_n_default_copies)
{
	int n = 100;
	blk::list<int> list(n);
	BOOST_CHECK(list.size() == n);
	for (auto item : list)
		BOOST_CHECK(item == int());
}

/*
BOOST_AUTO_TEST_CASE(move_list)
{
	blk::list<TestClass> l1;
	l1.emplace_back(0);
	l1.emplace_back(1);
	blk::list<TestClass> l2 = std::move(l1);
	for (auto item : l2)
		item.getValue();
}
*/

BOOST_AUTO_TEST_SUITE_END()