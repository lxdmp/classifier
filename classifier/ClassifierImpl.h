#ifndef _CLASSIFIER_IMPL_H_
#define _CLASSIFIER_IMPL_H_

#include <sstream>
#include <boost/any.hpp>

/*
 * 各类属性定义
 */
struct Property
{
	Property(const std::string &property_name, bool is_continous) : 
		_property_name(property_name), 
		_is_continous(is_continous)
	{
	}

	virtual boost::any property_cast(boost::any obj_ptr) = 0; // 获取对象实例的该项属性

	std::string _property_name; // 属性名
	bool _is_continous; // 是否连续属性
};

template<typename ObjT, typename PropertyT, typename PropertyGetterT>
struct ContinousProperty : Property
{
	ContinousProperty(const std::string &property_name, bool is_continous=true) : 
		Property(property_name, is_continous), 
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
		ContinousProperty<ObjT, PropertyT, PropertyGetterT>(property_name, false), 
		_property_comparator(PropertyComparatorT())
	{
	}

	PropertyComparatorT _property_comparator;
};

/*
 * 构造
 */
template<typename ObjT, typename ResultT, typename ResultComparatorT>
Classifier<ObjT,ResultT,ResultComparatorT>::Classifier() : 
	_result_comparator(ResultComparatorT())
{
}

/*
 * 注册实体类某项(离散的)属性
 */
template<typename ObjT, typename ResultT, typename ResultComparatorT>
template<typename PropertyT, typename PropertyGetterT, typename PropertyComparatorT>
void Classifier<ObjT,ResultT,ResultComparatorT>::regDiscreteProperty(
	std::string propertyName, PropertyGetterT getter
)
{
	boost::shared_ptr<Property> property(
		new DiscreteProperty<ObjT, PropertyT, PropertyGetterT, PropertyComparatorT>(propertyName)
	);
	auto insert_result = this->_properties.insert(std::make_pair(propertyName, property));
	if(!insert_result.second){
		std::stringstream s;
		s<<"duplicate property name \""<<propertyName<<"\"";
		throw std::runtime_error(s.str().c_str());
	}
}

/*
 * 注册实体类某项(连续的)属性
 */
template<typename ObjT, typename ResultT, typename ResultComparatorT>
template<typename PropertyT, typename PropertyGetterT>
void Classifier<ObjT,ResultT,ResultComparatorT>::regContinousProperty(
	std::string propertyName, PropertyGetterT getter
)
{
	boost::shared_ptr<Property> property(
		new ContinousProperty<ObjT, PropertyT, PropertyGetterT>(propertyName)
	);
	auto insert_result = this->_properties.insert(std::make_pair(propertyName, property));
	if(!insert_result.second){
		std::stringstream s;
		s<<"duplicate property name \""<<propertyName<<"\"";
		throw std::runtime_error(s.str().c_str());
	}
}

/*
 * 获取属性
 */
template<typename ObjT, typename ResultT, typename ResultComparatorT>
boost::shared_ptr<Property> Classifier<ObjT,ResultT,ResultComparatorT>::getProperty(
	const std::string &property_name
) const
{
	boost::shared_ptr<Property> ret;
	auto query_iter=  _properties.find(property_name);
	if(query_iter!=_properties.end())
		ret = query_iter->second;
	return ret;
}

template<typename ObjT, typename ResultT, typename ResultComparatorT>
std::set<boost::shared_ptr<Property> > Classifier<ObjT,ResultT,ResultComparatorT>::getContinousProperties(
) const
{
	std::set<boost::shared_ptr<Property> > ret;
	std::for_each(_properties.begin(), _properties.end(), [&](std::pair<std::string, boost::shared_ptr<Property> > &pair){
		if(pair.second->_is_continous)
			ret.insert(pair.second);
	});
	return ret;
}

template<typename ObjT, typename ResultT, typename ResultComparatorT>
std::set<boost::shared_ptr<Property> > Classifier<ObjT,ResultT,ResultComparatorT>::getDiscreteProperties(
) const
{
	std::set<boost::shared_ptr<Property> > ret;
	std::for_each(_properties.begin(), _properties.end(), [&](std::pair<std::string, boost::shared_ptr<Property> > &pair){
		if(!pair.second->_is_continous)
			ret.insert(pair.second);
	});
	return ret;
}

/*
 * 添加已分类的数据
 */
template<typename ObjT, typename ResultT, typename ResultComparatorT>
void Classifier<ObjT,ResultT,ResultComparatorT>::addSample(
	const ObjT &sample, const ResultT &result
)
{
}

template<typename ObjT, typename ResultT, typename ResultComparatorT>
template<typename SampleIteratorT, typename ResultIteratorT>
void Classifier<ObjT,ResultT,ResultComparatorT>::addSamples(
	SampleIteratorT sampleBegin, SampleIteratorT sampleEnd, 
	ResultIteratorT resultBegin, ResultIteratorT resultEnd
)
{
	for(; sampleBegin!=sampleEnd && resultBegin!=resultEnd; ++sampleBegin, ++resultBegin)
		this->addSample(*sampleBegin, *resultBegin);
}

#endif

