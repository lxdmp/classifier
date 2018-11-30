/*
 * 实现一个能对实体类对象进行分类的分类器.
 *
 * 实体类有一些属性:
 * - 属性可能是基本类型,也可能是自定义的复合类型;
 * - 属性可能是离散的,也可能是连续的.
 * - 对于一个针对某类实体对象的分类器,有一个(离散的)属性作为分类目标.
 *
 * 对于应用程序,需要将实体类中的各属性(作为属性或者分类)注册到分类器中.
 * 作为属性注册时,需要指明该属性是离散的还是连续的.
 */
#ifndef _CLASSIFIER_H_
#define _CLASSIFIER_H_

#include <string>
#include <map>
#include <boost/shared_ptr.hpp>

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

private:
	ResultComparatorT _result_comparator;
	//std::map<std::string, > _discrete_properties; // 离散属性
	//std::map<std::string, > _continous_properties; // 连续属性
	ClassifierImplementor _implementor;
};

#include "ClassifierImpl.h"

#endif

