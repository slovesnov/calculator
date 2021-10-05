/******************************************************
 Copyright (c/c++) 2013-doomsday by Aleksey Slovesnov 
 homepage http://slovesnov.users.sourceforge.net/?parser
 email slovesnov@yandex.ru
 All rights reserved.
 ******************************************************/

#ifndef _node_h_
#define _node_h_

#define _USE_MATH_DEFINES //for M_PI
#include <cmath>/*floor*/

#include "operatorEnum.h"
#include "exception.h"

class ExpressionEstimator;

class Node {

	friend class ExpressionEstimator;

	ExpressionEstimator *m_estimator;

	inline static double round(double d) {
		return floor(d + 0.5);
	}

	inline static double min(double x, double y) {
		return x <= y ? x : y;
	}

	inline static double max(double x, double y) {
		return x >= y ? x : y;
	}

	OPERATOR_ENUM m_operator;
	double m_value;
	Node *m_left, *m_right;

	void init(ExpressionEstimator *estimator, OPERATOR_ENUM _operator,
			double value, Node *left);

	double calculate();

};

#endif
