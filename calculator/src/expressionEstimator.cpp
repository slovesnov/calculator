/******************************************************
 Copyright (c/c++) 2013-doomsday by Aleksey Slovesnov 
 homepage http://slovesnov.users.sourceforge.net/?parser
 email slovesnov@yandex.ru
 All rights reserved.
 ******************************************************/

#include "exception.h"
#include "expressionEstimator.h"

const char *CONSTANT_NAME[] = { "PI", "E", "SQRT2", "SQRT1_2", "LN2", "LN10",
		"LOG2E", "LOG10E", NULL };
const double CONSTANT_VALUE[] = { M_PI, exp(1), sqrt(2), sqrt(0.5), log(2), log(
		10), 1 / log(2), 1 / log(10) };

const char *OPERATOR[] = { "PLUS", "MINUS", "MULTIPLY", "DIVIDE",
		"LEFT_BRACKET", "RIGHT_BRACKET", "LEFT_SQUARE_BRACKET",
		"RIGHT_SQUARE_BRACKET", "LEFT_CURLY_BRACKET", "RIGHT_CURLY_BRACKET",
		"COMMA", "SIN", "COS", "TAN", "COT", "SEC", "CSC", "ASIN", "ACOS",
		"ATAN", "ACOT", "ASEC", "ACSC", "SINH", "COSH", "TANH", "COTH", "SECH",
		"CSCH", "ASINH", "ACOSH", "ATANH", "ACOTH", "ASECH", "ACSCH", "RANDOM",
		"CEIL", "FLOOR", "ROUND", "ABS", "EXP", "LOG", "SQRT", "POW", "ATAN2",
		"MIN", "MAX", "X", "NUMBER", "UNARY_MINUS", "END", NULL };

#ifdef _DEBUG
	int	ExpressionEstimator::totalDestroyed=0;
	int	ExpressionEstimator::totalCreated=0;
#endif

unsigned ExpressionEstimator::m_c=0;

int ExpressionEstimator::getIndex(const char *a[], const char *v) {
	const char *n;
	int i;
	for (i = 0; (n = a[i]) != NULL; i++) {
		if (strcmp(n, v) == 0) {
			return i;
		}
	}
	throw IllegalArgumentException();
}

void ExpressionEstimator::getToken() {
	unsigned i;
	char *c;
	const char T[] = "+-*/()[]{},";

	if (m_position == m_expression.length()) {
		m_operator = END;
	} else if ((c = strchr(T, m_expression[m_position])) != NULL) {
		m_position++;
		m_operator = (OPERATOR_ENUM) (c - T);
	} else if (isLetter()) {
		for (i = m_position++;
				m_position < m_expression.length() && isFunctionSymbol();
				m_position++)
			;
		std::string token = m_expression.substr(i, m_position - i);

		try {
			if (token[0] == 'X' && token.length() == 1) {
				throw Exception("unknown keyword");
			} else if (token[0] == 'X' && token.length() > 1
					&& isDigit(token[1])) {
				i = unsigned(atoi(token.c_str() + 1));
				if (m_arguments < i + 1) {
					m_arguments = i + 1;
				}
				m_operator = X;
				m_tokenValue = i;
			} else {
				m_operator = (OPERATOR_ENUM) getIndex(OPERATOR, token.c_str());
				i = m_operator;
				if (i < SIN || i > MAX) {
					throw IllegalArgumentException();
				}
			}
		} catch (IllegalArgumentException &e1) {
			try {
				m_tokenValue = CONSTANT_VALUE[getIndex(CONSTANT_NAME,
						token.c_str())];
				m_operator = NUMBER;
			} catch (IllegalArgumentException &e2) {
				throw Exception("unknown keyword");
			}
		}
	} else if (isDigit() || isPoint()) {
		m_operator = NUMBER;
		/*21aug2018 version 1.14 fixed bug with parsing (213.45-206.75)*2 and 213.45-206.75*27
		 cann't use const char*p=m_expression.substr(m_position).c_str(); 
		 because local string is created and destroyed, so use 
		 const char*p=m_expression.c_str()+m_position;
		 */
		const char *p = m_expression.c_str() + m_position;
		char *pEnd;
		m_tokenValue = strtod(p, &pEnd);
		m_position += pEnd - p;
	} else {
		throw Exception("unknown symbol");
	}

}

bool ExpressionEstimator::compile(const char *expression) {
	m_position = 0;
	m_arguments = 0;
	m_expression = "";
	m_root = NULL;
	clear();
	m_argument = NULL;

	//parse dot as decimal separator for strtod, so setup standard locale
	const char *lorig = _strdup(setlocale(LC_ALL, NULL));
	setlocale(LC_NUMERIC, "C");

	const char *pc = expression;
	for (; *pc != 0; pc++) { //make upper string without emply symbols
		if (*pc == ' ' || *pc == '\t') {
			continue;
		}
		m_expression += toupper(*pc);
	}

	getToken();
	if (m_operator == END) {
		throw Exception("unexpected end of expression");
	}
	m_root = parse();
	if (m_operator != END) {
		throw Exception("end of expression expected");
	}

	//return original locale
	setlocale(LC_ALL, lorig);
	free((void*) lorig);

	return true;
}

Node* ExpressionEstimator::parse() {
	Node *node = parse1();
	while (m_operator == PLUS || m_operator == MINUS) {
		node = createNode(m_operator, node);
		getToken();
		if (m_operator == PLUS || m_operator == MINUS) {
			throw Exception("two operators in a row");
		}
		node->m_right = parse1();
	}
	return node;
}

Node* ExpressionEstimator::parse1() {
	Node *node = parse2();
	while (m_operator == MULTIPLY || m_operator == DIVIDE) {
		node = createNode(m_operator, node);
		getToken();
		if (m_operator == PLUS || m_operator == MINUS) {
			throw Exception("two operators in a row");
		}
		node->m_right = parse2();
	}
	return node;
}

Node* ExpressionEstimator::parse2() {
	Node *node;
	if (m_operator == MINUS) {
		getToken();
		node = createNode(UNARY_MINUS, parse3());
	} else {
		if (m_operator == PLUS) {
			getToken();
		}
		node = parse3();
	}
	return node;
}

Node* ExpressionEstimator::parse3() {
	Node *node;
	OPERATOR_ENUM open;

	if (m_operator >= SIN && m_operator <= MAX) {
		int arguments;
		if (m_operator >= POW && m_operator <= MAX) {
			arguments = 2;
		} else {
			arguments = m_operator == RANDOM ? 0 : 1;
		}

		node = createNode(m_operator);
		getToken();
		open = m_operator;
		if (m_operator != LEFT_BRACKET && m_operator != LEFT_SQUARE_BRACKET
				&& m_operator != LEFT_CURLY_BRACKET) {
			throw Exception("open bracket expected");
		}
		getToken();

		if (arguments > 0) {
			node->m_left = parse();

			if (arguments == 2) {
				if (m_operator != COMMA) {
					throw Exception("comma expected");
				}
				getToken();
				node->m_right = parse();
			}
		}
		checkBracketBalance(open);
	} else {
		switch (m_operator) {

		case X:
		case NUMBER:
			node = createNode(m_operator, m_tokenValue);
			break;

		case LEFT_BRACKET:
		case LEFT_SQUARE_BRACKET:
		case LEFT_CURLY_BRACKET:
			open = m_operator;
			getToken();
			node = parse();
			checkBracketBalance(open);
			break;

		default:
			throw Exception("unexpected operator");
		}

	}
	getToken();
	return node;
}

double ExpressionEstimator::calculate() {
	if (m_root == NULL) {
		throw Exception("using of calculate() without compile()");
	}

	unsigned length = m_argument == NULL ? 0 : m_argumentSize;

	if (length != m_arguments) {
		throw Exception("invalid number of expression arguments");
	}
	return m_root->calculate();
}

double ExpressionEstimator::calculate(const char *s) {
	/* uses m_c to avoid same results returns by
	 * double v1=calculate("random()");
	 * double v2=calculate("random()");
	 * need v1 != v2
	 *
	 */
	ExpressionEstimator estimator(m_c);
	m_c+=123456;//somehow big number gives big dispersion
	estimator.compile(s);
	estimator.m_argument = NULL; //clear all arguments
	return estimator.calculate();
}

void ExpressionEstimator::clear() {
	std::vector<Node*>::iterator it;
	for (it = m_vnode.begin(); it != m_vnode.end(); it++) {
		delete *it;
	}
#ifdef _DEBUG
	if(m_vnode.size()!=0){
		printf("destroyed %d node(s)\n",m_vnode.size());
	}
	totalDestroyed+=m_vnode.size();
#endif
	m_vnode.clear();

}

ExpressionEstimator::~ExpressionEstimator() {
	clear();
}
