/******************************************************
 Copyright (c/c++) 2013-doomsday by Aleksey Slovesnov 
 homepage http://slovesnov.users.sourceforge.net/?parser
 email slovesnov@yandex.ru
 All rights reserved.
 ******************************************************/

#ifndef _expressionEstimator_h_
#define _expressionEstimator_h_

#include <ctime>   // time
#include <vector>
#include <cstring> //strchr
#include <cstdlib> //srand
#include "node.h"
#include "exception.h"

class ExpressionEstimator {
	friend class Node;

	std::string m_expression;
	double m_tokenValue;
	OPERATOR_ENUM m_operator;
	unsigned m_position;
	const double *m_argument;
	int m_argumentSize;
	unsigned m_arguments;
	Node *m_root;
	std::vector<Node*> m_vnode;
	static unsigned m_c;

	inline bool isLetter() {
		return isalpha(m_expression[m_position]) != 0;
	}

	inline bool isDigit() {
		return isdigit(m_expression[m_position]) != 0;
	}

	inline static bool isDigit(char c) {
		return isdigit(c) != 0;
	}

	inline bool isPoint() {
		return m_expression[m_position] == '.';
	}

	inline bool isFunctionSymbol() {
		char c = m_expression[m_position];
		return isalnum(c) != 0 || c == '_';
	}

	void getToken();

	Node* parse();
	Node* parse1();
	Node* parse2();
	Node* parse3();

	inline Node* createNode(OPERATOR_ENUM _operator, Node *left) {
		Node *node = new Node();
		node->init(this, _operator, 0, left);
		return node;
	}

	inline Node* createNode(OPERATOR_ENUM _operator) {
		Node *node = new Node();
		node->init(this, _operator, 0, NULL);
		return node;

	}

	inline Node* createNode(OPERATOR_ENUM _operator, double value) {
		Node *node = new Node();
		node->init(this, _operator, value, NULL);
		return node;
	}

	inline void checkBracketBalance(OPERATOR_ENUM open) {
		if ((open == LEFT_BRACKET && m_operator != RIGHT_BRACKET)
				|| (open == LEFT_SQUARE_BRACKET
						&& m_operator != RIGHT_SQUARE_BRACKET)
				|| (open == LEFT_CURLY_BRACKET
						&& m_operator != RIGHT_CURLY_BRACKET)) {
			throw Exception(
					"close bracket expected or another type of close bracket");
		}
	}

	static int getIndex(const char *a[], const char *v);

	void clear();

public:

#ifdef _DEBUG
	static int totalCreated;
	static int totalDestroyed;
#endif

	ExpressionEstimator(unsigned a=0) {
		m_root = NULL; //don't remove
		//printf("srand%d\n",unsigned(time(NULL)+a));
		srand(time(NULL)+a);

		//fix eclipse warnings
//		m_operator = END;
//		m_argument = NULL;
//		m_tokenValue = m_argumentSize = m_arguments = 0;
//		m_position = -1;
	}

	ExpressionEstimator(const char *s):ExpressionEstimator(){
		try{
			compile(s);
		}
		catch(Exception& e){
			clear();
			throw;
		}
	}

	ExpressionEstimator(std::string const & s) : ExpressionEstimator(s.c_str()){
	}

	bool compile(const char *expression);
	bool compile(const std::string &expression) {
		return compile(expression.c_str());
	}

	double calculate(const double *x, const unsigned long long size) {
		m_argument = x;
		m_argumentSize = size;
		return calculate();
	}

	double calculate(double *x, const unsigned long long size) {
		m_argument = x;
		m_argumentSize = size;
		return calculate();
	}

	double calculate(const double *x, const int size) {
		m_argument = x;
		m_argumentSize = size;
		return calculate();
	}

	double calculate(double *x, const int size) {
		m_argument = x;
		m_argumentSize = size;
		return calculate();
	}

	double calculate(std::initializer_list<double> const &v) {
		return calculate(v.begin(), v.size());
	}

	double calculate(std::vector<double> const &v) {
		return calculate(v.data(), v.size());
	}

	template <typename...A>
	double calculate(A...a){
		std::vector<double> v={std::forward<double>(a)...};
		return calculate(v);
	}

/*
	double calculate(const double *x, const unsigned long long size) {
		m_argument = x;
		m_argumentSize = size;
		return calculate();
	}

	double calculate(double *x, const unsigned long long size) {
		m_argument = x;
		m_argumentSize = size;
		return calculate();
	}

	double calculate(const double *x, const int size) {
		m_argument = x;
		m_argumentSize = size;
		return calculate();
	}

	double calculate(double *x, const int size) {
		m_argument = x;
		m_argumentSize = size;
		return calculate();
	}
*/
/*
	double calculate(std::initializer_list<double> const &v) {
		return calculate(v.begin(), v.size());
	}

	double calculate(std::vector<double> const &v) {
		return calculate(v.data(), v.size());
	}

	template <typename A,typename B>
	double calculate(A a,B b){
		if( (std::is_same<A,double*>::value || std::is_same<A,const double*>::value) &&
				(std::is_same<B,unsigned long long>::value)
				){
			m_argument = a;
			m_argumentSize = b;
			return calculate();
		}
		else{
			std::vector<double> v={std::forward<double>(a),std::forward<double>(b)};
			return calculate(v);
		}
	}
*/

	double calculate();

	/**
	 * 
	 * @return number of expression arguments
	 */
	inline int getArguments() {
		return m_arguments;
	}

	static double calculate(const char *s);
	static double calculate(std::string const & s){
		return calculate(s.c_str());
	}

	~ExpressionEstimator();
};

#endif
