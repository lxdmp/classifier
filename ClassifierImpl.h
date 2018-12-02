#ifndef _CLASSIFIER_IMPL_H_
#define _CLASSIFIER_IMPL_H_

#include <sstream>
#include <boost/any.hpp>

/*
 * 各类属性定义
 */
struct Property
{
	Property(const std::string &property_name) : _property_name(property_name){}
	std::string _property_name; // 属性名

	virtual boost::any property_cast(boost::any obj_ptr) = 0;
};

template<typename ObjT, typename PropertyT, typename PropertyGetterT>
struct ContinousProperty : Property
{
	ContinousProperty(const std::string &property_name) : Property(property_name), 
		_getter(&ObjT::PropertyGetterT)
	{
	}

	virtual boost::any property_cast(boost::any obj_ptr)
	{
		const ObjT *obj = boost::any_cast<ObjT*>(obj_ptr);
		return (obj->*_getter)();
	}

	typedef PropertyT (ObjT::*PropertyGetterDecl)() const;
	PropertyGetterDecl _getter; // 属性获取方法指针
};

template<typename ObjT, typename PropertyT, typename PropertyGetterT, typename PropertyComparatorT>
struct DiscreteProperty : ContinousProperty<ObjT, PropertyT, PropertyGetterT>
{
	DiscreteProperty(const std::string &property_name) : 
		ContinousProperty<ObjT, PropertyT, PropertyGetterT>(property_name), 
		_property_comparator(PropertyComparatorT())
	{
	}

	PropertyComparatorT _property_comparator;
};

/*
 * 构造
 */
template<typename ObjT, typename ResultT, typename ResultComparatorT, typename ClassifierImplementor>
Classifier<ObjT,ResultT,ResultComparatorT,ClassifierImplementor>::Classifier() : 
	_result_comparator(ResultComparatorT()), 
	_implementor(*this)
{
}

/*
 * 注册实体类某项(离散的)属性
 */
template<typename ObjT, typename ResultT, typename ResultComparatorT, typename ClassifierImplementor>
template<typename PropertyT, typename PropertyGetterT, typename PropertyComparatorT>
void Classifier<ObjT,ResultT,ResultComparatorT,ClassifierImplementor>::regDiscreteProperty(
	std::string propertyName, PropertyGetterT getter
)
{
	boost::shared_ptr<Property> property(
		new DiscreteProperty<ObjT, PropertyT, PropertyGetterT, PropertyComparatorT>(propertyName)
	);
	auto insert_result = this->_discrete_properties.insert(std::make_pair(propertyName, property));
	if(!insert_result.second){
		std::stringstream s;
		s<<"duplicate property name \""<<propertyName<<"\"";
		throw std::runtime_error(s.str().c_str());
	}
}

/*
 * 注册实体类某项(连续的)属性
 */
template<typename ObjT, typename ResultT, typename ResultComparatorT, typename ClassifierImplementor>
template<typename PropertyT, typename PropertyGetterT>
void Classifier<ObjT,ResultT,ResultComparatorT,ClassifierImplementor>::regContinousProperty(
	std::string propertyName, PropertyGetterT getter
)
{
	boost::shared_ptr<Property> property(
		new ContinousProperty<ObjT, PropertyT, PropertyGetterT>(propertyName)
	);
	auto insert_result = this->_continous_properties.insert(std::make_pair(propertyName, property));
	if(!insert_result.second){
		std::stringstream s;
		s<<"duplicate property name \""<<propertyName<<"\"";
		throw std::runtime_error(s.str().c_str());
	}
}

/*
 * 添加已分类的数据
 */
template<typename ObjT, typename ResultT, typename ResultComparatorT, typename ClassifierImplementor>
void Classifier<ObjT,ResultT,ResultComparatorT,ClassifierImplementor>::addSample(
	const ObjT &sample, const ResultT &result
)
{
}

template<typename ObjT, typename ResultT, typename ResultComparatorT, typename ClassifierImplementor>
template<typename SampleIteratorT, typename ResultIteratorT>
void Classifier<ObjT,ResultT,ResultComparatorT,ClassifierImplementor>::addSamples(
	SampleIteratorT sampleBegin, SampleIteratorT sampleEnd, 
	ResultIteratorT resultBegin, ResultIteratorT resultEnd
)
{
	for(; sampleBegin!=sampleEnd && resultBegin!=resultEnd; ++sampleBegin, ++resultBegin)
		this->addSample(*sampleBegin, *resultBegin);
}

/*
 * 训练&分类
 */
template<typename ObjT, typename ResultT, typename ResultComparatorT, typename ClassifierImplementor>
void Classifier<ObjT,ResultT,ResultComparatorT,ClassifierImplementor>::train()
{
	_implementor.train();
}


template<typename ObjT, typename ResultT, typename ResultComparatorT, typename ClassifierImplementor>
ResultT Classifier<ObjT,ResultT,ResultComparatorT,ClassifierImplementor>::classify(const ObjT &obj) const
{
	return ResultT();
}

#endif

