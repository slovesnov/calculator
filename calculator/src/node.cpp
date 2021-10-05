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

	case NUMBER:
		return m_value;

	case PLUS:
		return m_left->calculate() + m_right->calculate();

	case MINUS:
		return m_left->calculate() - m_right->calculate();

	case MULTIPLY:
		return m_left->calculate() * m_right->calculate();

	case DIVIDE:
		return m_left->calculate() / m_right->calculate();

	case UNARY_MINUS:
		return -m_left->calculate();

	case SIN:
		return sin(m_left->calculate());

	case COS:
		return cos(m_left->calculate());

	case TAN:
		return tan(m_left->calculate());

	case COT:
		return 1 / tan(m_left->calculate());

	case SEC:
		return 1 / cos(m_left->calculate());

	case CSC:
		return 1 / sin(m_left->calculate());

	case ASIN:
		return asin(m_left->calculate());

	case ACOS:
		return acos(m_left->calculate());

	case ATAN:
		return atan(m_left->calculate());

	case ACOT:
		return M_PI / 2 - atan(m_left->calculate());

	case ASEC:
		return acos(1 / m_left->calculate());

	case ACSC:
		return asin(1 / m_left->calculate());

	case SINH:
		x = m_left->calculate();
		return (exp(x) - exp(-x)) / 2;

	case COSH:
		x = m_left->calculate();
		return (exp(x) + exp(-x)) / 2;

	case TANH:
		x = m_left->calculate();
		return (exp(2 * x) - 1) / (exp(2 * x) + 1);

	case COTH:
		x = m_left->calculate();
		return (exp(2 * x) + 1) / (exp(2 * x) - 1);

	case SECH:
		x = m_left->calculate();
		return 2 / (exp(x) + exp(-x));

	case CSCH:
		x = m_left->calculate();
		return 2 / (exp(x) - exp(-x));

	case ASINH:
		x = m_left->calculate();
		return log(x + sqrt(x * x + 1));

	case ACOSH:
		x = m_left->calculate();
		return log(x + sqrt(x * x - 1));

	case ATANH:
		x = m_left->calculate();
		return log((1 + x) / (1 - x)) / 2;

	case ACOTH:
		x = m_left->calculate();
		return log((x + 1) / (x - 1)) / 2;

	case ASECH:
		x = m_left->calculate();
		return log((1 + sqrt(1 - x * x)) / x);

	case ACSCH:
		x = m_left->calculate();
		return log(1 / x + sqrt(1 + x * x) / fabs(x));

	case RANDOM:
		return double(rand()) / RAND_MAX;

	case CEIL:
		return ceil(m_left->calculate());

	case FLOOR:
		return floor(m_left->calculate());

	case ROUND:
		return round(m_left->calculate());

	case ABS:
		return fabs(m_left->calculate());

	case EXP:
		return exp(m_left->calculate());

	case LOG:
		return log(m_left->calculate());

	case SQRT:
		return sqrt(m_left->calculate());

	case POW:
		return pow(m_left->calculate(), m_right->calculate());

	case ATAN2:
		return atan2(m_left->calculate(), m_right->calculate());

	case MIN:
		return min(m_left->calculate(), m_right->calculate());

	case MAX:
		return max(m_left->calculate(), m_right->calculate());

	case X:
		return m_estimator->m_argument[(int) m_value];

	default:
		throw Exception("Node.calculate error");
	}
}

