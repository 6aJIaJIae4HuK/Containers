#define BOOST_TEST_NO_LIB
#include <boost/test/unit_test.hpp>

#include "../list.h"

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

BOOST_AUTO_TEST_CASE(insert_test)
{
	blk::list<int> list;
	list.insert(list.end(), 2);
	list.insert(list.begin(), 1);
	::ListIterator<int> it = list.insert(list.end(), 3);
	BOOST_CHECK(*it == 3);
	BOOST_CHECK(*list.begin() == 1);
	BOOST_CHECK(*list.rbegin() == 3);
}

BOOST_AUTO_TEST_CASE(erase_list_test)
{
	blk::list<int> list;
	for (int i = 0; i < 10; i++)
		list.insert(list.end(), i);
	BOOST_CHECK(*(list.erase(list.begin())) == 1);
}

BOOST_AUTO_TEST_SUITE_END()
