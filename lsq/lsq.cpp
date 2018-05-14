//
// Created by yj374 on 5/11/18.
//

#include "lsq.h"

namespace lsq {
	solver::solver(lsq_config _cfg):cfg(_cfg) {
		nonlinear::find_min_config find_min_cfg;
		find_min_cfg.func = cfg.V;
		find_min_cfg.max_iter = cfg.max_iter;
		find_min_cfg.method = nonlinear::FIND_MIN_QUASI_NEWTON;
		find_min_cfg.n_var = cfg.n_var;
		find_min_cfg.tol = cfg.tol;
		find_min_cfg.x = cfg.a;
		fm = new nonlinear::find_min(find_min_cfg);
	}
	
	solver::~solver() {
	}
}