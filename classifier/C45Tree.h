#ifndef _C45_TREE_H_
#define _C45_TREE_H_

template<
	typename ObjT, 
	typename ResultT, 
	typename ResultComparatorT=ResultComparator<ResultT> 
>
class C45Tree : public Classifier<ObjT, ResultT, ResultComparatorT>
{
public:
	C45Tree();

protected:
	

public:
	virtual void train();
	virtual ResultT classify(const ObjT &obj) const;
};

#endif

