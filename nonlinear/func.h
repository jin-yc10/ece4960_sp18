//
// Created by yj374 on 3/30/18.
//

#ifndef ECE4960_SP18_FUNC_H
#define ECE4960_SP18_FUNC_H

#include <vector>

class func {
	virtual void eval(
			std::vector<double>& in,
			std::vector<double>& out
	);
	
	void eval_jacobi()(
			std::vector<double>& in,
			std::vector<double>& out
	);
	
	void eval_hesian()(
			std::vector<double>& in,
			std::vector<double>& out
	);
};


#endif //ECE4960_SP18_FUNC_H
