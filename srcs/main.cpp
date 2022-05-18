#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <algorithm>

#if STL
  #include <vector>
  #include <stack>
  #include <map>
  #include <set>
  namespace ft = std;
#else
  #include "vector.hpp"
  #include "stack.hpp"
  #include "map.hpp"
  #include "set.hpp"
#endif

void print_ln(int elem) {
  std::cout << elem << std::endl;
}

int main() {
  ft::vector<int> v(10);
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  for (ft::vector<int>::iterator iter = v.begin(), last = v.end(); iter != last; ++iter) {
    std::cout << *iter << std::endl;
  }
  ft::vector<int> v2(10, 42);
  for (ft::vector<int>::reverse_iterator riter = v2.rbegin(), last = v2.rend(); riter != last; ++riter) {
    std::cout << *riter << std::endl;
  }
  ft::vector<int> v3(v.begin(), v.end());
  for (ft::vector<int>::reverse_iterator riter = v3.rbegin(), last = v3.rend(); riter != last; ++riter) {
    std::cout << *riter << std::endl;
  }

  std::cout << "\nTest for input iterator\n";

  const std::string s("123 456 789");

  std::istringstream iss(s);
  std::istream_iterator<int> isIt(iss);
  std::istream_iterator<int> isItEnd;

  ft::vector<int> v4(isIt, isItEnd);
  std::for_each(v4.begin(), v4.end(), print_ln);

  ft::map<char,int> mymap;

  // first insert function version (single parameter):
  mymap.insert ( ft::pair<char,int>('a',100) );
  mymap.insert ( ft::pair<char,int>('z',200) );

  ft::pair<ft::map<char,int>::iterator,bool> ret;
  ret = mymap.insert ( ft::pair<char,int>('z',500) );
  if (ret.second==false) {
    std::cout << "element 'z' already existed";
    std::cout << " with a value of " << ret.first->second << '\n';
  }

  {
    // second insert function version (with hint position):
    ft::map<char,int>::iterator it = mymap.begin();
    mymap.insert (it, ft::pair<char,int>('b',300));  // max efficiency inserting
    mymap.insert (it, ft::pair<char,int>('c',400));  // no max efficiency inserting

    // third insert function version (range insertion):
    ft::map<char,int> anothermap;
    anothermap.insert(mymap.begin(),mymap.find('c'));

    // showing contents:
    std::cout << "mymap contains:\n";
    for (it=mymap.begin(); it!=mymap.end(); ++it)
      std::cout << it->first << " => " << it->second << '\n';

    std::cout << "anothermap contains:\n";
    for (it=anothermap.begin(); it!=anothermap.end(); ++it)
      std::cout << it->first << " => " << it->second << '\n';
  }

  ft::vector<std::string> Z1;
  ft::vector<std::string> Z2;

  Z1.insert(Z1.begin(), 10, "test");
  Z2.insert(Z2.begin(), Z1.begin(), Z1.end());

  for (ft::vector<std::string>::iterator it = Z1.begin(), end = Z1.end(); it != end; ++it) {
    std::cout << *it << std::endl;
  }

  for (ft::vector<std::string>::iterator it = Z2.begin(), end = Z2.end(); it != end; ++it) {
    std::cout << *it << std::endl;
  }

  {
    ft::set<int> myset;
    ft::set<int>::iterator it;
    ft::pair<ft::set<int>::iterator,bool> ret;

    // set some initial values:
    for (int i=1; i<=5; ++i) myset.insert(i*10);    // set: 10 20 30 40 50

    ret = myset.insert(20);               // no new element inserted

    if (ret.second==false) it=ret.first;  // "it" now points to element 20

    myset.insert(it,25);                 // max efficiency inserting
    myset.insert(it,24);                 // max efficiency inserting
    myset.insert(it,26);                 // no max efficiency inserting

    int myints[]= {5,10,15};              // 10 already in set, not inserted
    myset.insert (myints,myints+3);

    std::cout << "myset contains:";
    for (it=myset.begin(); it!=myset.end(); ++it)
      std::cout << ' ' << *it;
    std::cout << '\n';
  }
  {
    ft::map<char,int> mymap;
    ft::map<char,int>::iterator itlow,itup;

    mymap['a']=20;
    mymap['b']=40;
    mymap['c']=60;
    mymap['d']=80;
    mymap['e']=100;

    itlow=mymap.lower_bound ('c');  // itlow points to b
    itup=mymap.upper_bound ('d');   // itup points to e (not d!)

    mymap.erase(itlow,itup);        // erases [itlow,itup)

    // print content:
    for (ft::map<char,int>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
      std::cout << it->first << " => " << it->second << '\n';
  }

  return 0;
}
