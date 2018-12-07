/*
 * 实现一个能对实体类对象进行分类的分类器.
 *
 * 实体类有一些属性:
 * - 属性可能是基本类型,也可能是自定义的复合类型;
 * - 属性可能是离散的,也可能是连续的.
 *
 * 对于应用程序,需要将实体类中的各属性注册到分类器中.
 */
#ifndef _CLASSIFIER_H_
#define _CLASSIFIER_H_

#include <string>
#include <map>
#include <set>
#include <boost/shared_ptr.hpp>

struct Property;

template<typename PropertyT>
struct PropertyComparator
{
	bool operator()(const PropertyT &a1, const PropertyT &a2) const
	{
		return a1<a2;
	}
};

template<typename ResultT> 
struct ResultComparator : PropertyComparator<ResultT>
{
};

/*
 * ObjT : 待分类的实体类;
 * ResultT/ResultComparatorT : 分类的结果及其比较器;
 * ClassifierImplementor : 分类的具体实现.
 */
template<
	typename ObjT, 
	typename ResultT, 
	typename ResultComparatorT/*=ResultComparator*/, 
	typename ClassifierImplementor>
class Classifier
{
public:
	Classifier();

public:
	/*
	 * 注册实体类某项(离散的)属性
	 *
	 * 需要明确下列信息:
	 * - 属性的名称(必需);
	 * - 属性的类型(必需);
	 * - 属性的访问接口(必需);
	 * - 属性的比较器(选择,仿函数形式,严格弱序).
	 */
	template<
		typename PropertyT, typename PropertyGetterT, 
		typename PropertyComparatorT=PropertyComparator<PropertyT> >
	void regDiscreteProperty(std::string propertyName, PropertyGetterT getter);

	/*
	 * 注册实体类某项(连续的)属性
	 *
	 * 需要明确下列信息:
	 * - 属性的名称(必需);
	 * - 属性的类型(必需);
	 * - 属性的访问接口(必需).
	 */
	template<typename PropertyT, typename PropertyGetterT>
	void regContinousProperty(std::string propertyName, PropertyGetterT getter);

	/*
	 * 获取属性
	 */
	boost::shared_ptr<Property> getProperty(const std::string &property_name) const;
	std::set<boost::shared_ptr<Property> > getContinousProperties() const;
	std::set<boost::shared_ptr<Property> > getDiscreteProperties() const;

public:
	/*
	 * 添加已分类的数据
	 */
	void addSample(const ObjT &sample, const ResultT &result);

	template<typename SampleIteratorT, typename ResultIteratorT>
	void addSamples(
		SampleIteratorT sampleBegin, SampleIteratorT sampleEnd, 
		ResultIteratorT resultBegin, ResultIteratorT resultEnd
	);

public:
	/*
	 * 训练&分类
	 */
	void train();
	ResultT classify(const ObjT &obj) const;

protected:
	ResultComparatorT _result_comparator;
	ClassifierImplementor _implementor;
	std::map<std::string, boost::shared_ptr<Property> > _properties; // 属性
};

#include "ClassifierImpl.h"

#endif

