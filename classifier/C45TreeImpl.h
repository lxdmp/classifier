#ifndef _C_45_TREE_IMPL_H_
#define _C_45_TREE_IMPL_H_

template<typename ObjT, typename ResultT, typename ResultComparatorT>
C45Tree<ObjT, ResultT, ResultComparatorT>::C45Tree() : Classifier<ObjT, ResultT, ResultComparatorT>()
{
}


template<typename ObjT, typename ResultT, typename ResultComparatorT>
void C45Tree<ObjT, ResultT, ResultComparatorT>::train()
{
}

template<typename ObjT, typename ResultT, typename ResultComparatorT>
ResultT C45Tree<ObjT, ResultT, ResultComparatorT>::classify(const ObjT &obj) const
{
	ResultT ret;
	return ret;
}

#endif

