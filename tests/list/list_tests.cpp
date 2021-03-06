#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../../include/list.h"
#include "../test_class.h"

BOOST_AUTO_TEST_SUITE(list)

BOOST_AUTO_TEST_CASE(insert_test)
{
	blk::list<int> list;
	list.insert(list.end(), 2);
	list.insert(list.begin(), 1);
	blk::list<int>::iterator it = list.insert(list.end(), 3);
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

BOOST_AUTO_TEST_CASE(iterate_list_test)
{
	int num = 10;
	int cnt = 0;
	int sum = 0;
	blk::list<TestClass> list;
	for (int i = 0; i < num; i++)
		list.insert(list.end(), TestClass(i));
	for (auto it = list.begin(); it != list.end(); ++it)
	{
		cnt++;
		sum += it->getValue();
	}
	BOOST_CHECK(cnt == num);
	BOOST_CHECK(sum == (num * (num - 1)) / 2);

	cnt = 0;
	sum = 0;
	for (auto it = list.rbegin(); it != list.rend(); ++it)
	{
		cnt++;
		sum += it->getValue();
	}
	BOOST_CHECK(cnt == num);
	BOOST_CHECK(sum == (num * (num - 1)) / 2);
}

BOOST_AUTO_TEST_CASE(iterate_range_list_test)
{
	int num = 10;
	int cnt = 0;
	int sum = 0;
	blk::list<int> list;
	for (int i = 0; i < num; i++)
		list.insert(list.end(), i);

	for (auto i : list)
	{
		cnt++;
		sum += i;
	}
	BOOST_CHECK(cnt == num);
	BOOST_CHECK(sum == (num * (num - 1)) / 2);
}

BOOST_AUTO_TEST_CASE(remove_test)
{
	blk::list<int> list;
	list.push_back(1);
	list.push_back(3);
	list.push_back(1);
	list.push_back(2);
	list.push_back(1);
	list.remove(1);
	BOOST_CHECK(*list.begin() == 3 && *list.rbegin() == 2);
}

BOOST_AUTO_TEST_CASE(remove_predicate_test)
{
	blk::list<int> list;
	list.push_back(1);
	list.push_back(3);
	list.push_back(1);
	list.push_back(2);
	list.push_back(1);
	list.remove_if([](int n) { return n == 1; });
	BOOST_CHECK(*list.begin() == 3 && *list.rbegin() == 2);
}

BOOST_AUTO_TEST_SUITE_END()
