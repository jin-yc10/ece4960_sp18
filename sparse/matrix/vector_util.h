//
// Created by yj374 on 3/7/18.
//

#ifndef ECE4960_SP18_VECTOR_UTIL_H
#define ECE4960_SP18_VECTOR_UTIL_H

template<class T>
T norm_vector_diff(const std::vector<T>& v1,
       const std::vector<T>& v2) {
	if( v1.size() != v2.size() ) return (T)-1;
	T s = (T)0;
	for(int i=0; i<v1.size(); i++) {
		s += std::abs(v1[i]-v2[i])*std::abs(v1[i]-v2[i]);
	}
	s = std::sqrt(s);
	return s;
}

#endif //ECE4960_SP18_VECTOR_UTIL_H
