/******************************************************
 Copyright (c/c++) 2013-doomsday by Aleksey Slovesnov 
 homepage http://slovesnov.users.sourceforge.net/?parser
 email slovesnov@yandex.ru
 All rights reserved.
 ******************************************************/

#include "node.h"
#include "expressionEstimator.h"

void Node::init(ExpressionEstimator *estimator, OPERATOR_ENUM _operator,
		double value, Node *left) {
	m_estimator = estimator;
	m_estimator->m_vnode.push_back(this);
	m_operator = _operator;
	m_value = value;
	m_left = left;
	m_right = NULL;
#ifdef _DEBUG
	ExpressionEstimator::totalCreated++;
	//printf("create node %lx\n",this);
#endif
}

double Node::calculate() {
	double x;
	switch (m_operator) {

	case OPERATOR_ENUM::NUMBER:
		return m_value;

	case OPERATOR_ENUM::PLUS:
		return m_left->calculate() + m_right->calculate();

	case OPERATOR_ENUM::MINUS:
		return m_left->calculate() - m_right->calculate();

	case OPERATOR_ENUM::MULTIPLY:
		return m_left->calculate() * m_right->calculate();

	case OPERATOR_ENUM::DIVIDE:
		return m_left->calculate() / m_right->calculate();

	case OPERATOR_ENUM::UNARY_MINUS:
		return -m_left->calculate();

	case OPERATOR_ENUM::SIN:
		return sin(m_left->calculate());

	case OPERATOR_ENUM::COS:
		return cos(m_left->calculate());

	case OPERATOR_ENUM::TAN:
		return tan(m_left->calculate());

	case OPERATOR_ENUM::COT:
		return 1 / tan(m_left->calculate());

	case OPERATOR_ENUM::SEC:
		return 1 / cos(m_left->calculate());

	case OPERATOR_ENUM::CSC:
		return 1 / sin(m_left->calculate());

	case OPERATOR_ENUM::ASIN:
		return asin(m_left->calculate());

	case OPERATOR_ENUM::ACOS:
		return acos(m_left->calculate());

	case OPERATOR_ENUM::ATAN:
		return atan(m_left->calculate());

	case OPERATOR_ENUM::ACOT:
		return M_PI / 2 - atan(m_left->calculate());

	case OPERATOR_ENUM::ASEC:
		return acos(1 / m_left->calculate());

	case OPERATOR_ENUM::ACSC:
		return asin(1 / m_left->calculate());

	case OPERATOR_ENUM::SINH:
		x = m_left->calculate();
		return (exp(x) - exp(-x)) / 2;

	case OPERATOR_ENUM::COSH:
		x = m_left->calculate();
		return (exp(x) + exp(-x)) / 2;

	case OPERATOR_ENUM::TANH:
		x = m_left->calculate();
		return (exp(2 * x) - 1) / (exp(2 * x) + 1);

	case OPERATOR_ENUM::COTH:
		x = m_left->calculate();
		return (exp(2 * x) + 1) / (exp(2 * x) - 1);

	case OPERATOR_ENUM::SECH:
		x = m_left->calculate();
		return 2 / (exp(x) + exp(-x));

	case OPERATOR_ENUM::CSCH:
		x = m_left->calculate();
		return 2 / (exp(x) - exp(-x));

	case OPERATOR_ENUM::ASINH:
		x = m_left->calculate();
		return log(x + sqrt(x * x + 1));

	case OPERATOR_ENUM::ACOSH:
		x = m_left->calculate();
		return log(x + sqrt(x * x - 1));

	case OPERATOR_ENUM::ATANH:
		x = m_left->calculate();
		return log((1 + x) / (1 - x)) / 2;

	case OPERATOR_ENUM::ACOTH:
		x = m_left->calculate();
		return log((x + 1) / (x - 1)) / 2;

	case OPERATOR_ENUM::ASECH:
		x = m_left->calculate();
		return log((1 + sqrt(1 - x * x)) / x);

	case OPERATOR_ENUM::ACSCH:
		x = m_left->calculate();
		return log(1 / x + sqrt(1 + x * x) / fabs(x));

	case OPERATOR_ENUM::RANDOM:
		//rand() is not thread safe todo
		//fixed if RAND_MAX=0x7fffffff need to make RAND_MAX + 1. instead of RAND_MAX + 1
		return rand() / (RAND_MAX + 1.);

	case OPERATOR_ENUM::CEIL:
		return ceil(m_left->calculate());

	case OPERATOR_ENUM::FLOOR:
		return floor(m_left->calculate());

	case OPERATOR_ENUM::ROUND:
		return round(m_left->calculate());

	case OPERATOR_ENUM::ABS:
		return fabs(m_left->calculate());

	case OPERATOR_ENUM::EXP:
		return exp(m_left->calculate());

	case OPERATOR_ENUM::LOG:
		return log(m_left->calculate());

	case OPERATOR_ENUM::SQRT:
		return sqrt(m_left->calculate());

	case OPERATOR_ENUM::POW:
		return pow(m_left->calculate(), m_right->calculate());

	case OPERATOR_ENUM::ATAN2:
		return atan2(m_left->calculate(), m_right->calculate());

	case OPERATOR_ENUM::MIN:
		return min(m_left->calculate(), m_right->calculate());

	case OPERATOR_ENUM::MAX:
		return max(m_left->calculate(), m_right->calculate());

	case OPERATOR_ENUM::X:
		return m_estimator->m_argument[(int) m_value];

	default:
		throw std::runtime_error("Node.calculate error");
	}
}

