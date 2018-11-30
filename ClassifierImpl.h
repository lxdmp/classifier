#ifndef _CLASSIFIER_IMPL_H_
#define _CLASSIFIER_IMPL_H_

/*
 * 各类属性定义
 */
template<typename ObjT>
struct Property
{
	Property(const std::string &property_name) : _property_name(property_name)
	{
	}

	virtual ~Property(){}

	//virtual getProperty(const ObjT &obj) const;

	std::string _property_name; // 属性名
};

template<typename ObjT, typename PropertyT, typename PropertyGetterT>
struct ContinousProperty : Property<ObjT>
{
	ContinousProperty(const std::string &property_name) : Property<ObjT>(property_name), 
		_getter(&ObjT::PropertyGetterT)
	{
	}

	typedef PropertyT (ObjT::*PropertyGetterDecl)() const;
	PropertyGetterDecl _getter;
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
Classifier<ObjT,ResultT,ResultComparatorT,ClassifierImplementor>::Classifier() : _implementor(*this)
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

