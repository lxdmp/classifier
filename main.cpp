#include "Classifier.h"
#include <string>
#include <iostream>
#include <typeinfo>

template<typename PropertyT, typename PropertyGetterT, typename ComparatorT=std::string>
void regProperty(std::string propertyName, PropertyGetterT getter)
{
	std::cout<<typeid(PropertyT).name()<<std::endl;
	std::cout<<typeid(PropertyGetterT).name()<<std::endl;
	std::cout<<typeid(ComparatorT).name()<<std::endl;
}

struct test_t
{
	void test1(){}
	int test2(){return 0;}
	int func()
	{
		return 0;
	}
};

int main()
{
	test_t a;
	a.test1();
	int val = 1;
	regProperty<test_t>("abc", val);
	std::cout<<&test_t::func<<std::endl;
	return 0;
}

