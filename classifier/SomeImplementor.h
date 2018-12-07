#ifndef _C45_TREE_H_
#define _C45_TREE_H_

template<
	typename ObjT, 
	typename ResultT, 
	typename ResultComparatorT=ResultComparator<ResultT> 
>
class SomeImplementor : public Classifier<ObjT, ResultT, ResultComparatorT>
{
public:
	SomeImplementor();

public:
	virtual void train();
	virtual ResultT classify(const ObjT &obj) const;
};

#endif

