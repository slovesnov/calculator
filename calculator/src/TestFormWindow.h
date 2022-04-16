/*
 * TestFormWindow.h
 *
 *  Created on: 16.04.2022
 *      Author: alexey slovesnov
 * copyright(c/c++): 2014-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         homepage: slovesnov.users.sourceforge.net
 */

#ifndef TESTFORMWINDOW_H_
#define TESTFORMWINDOW_H_

#include "aslov.h"
#include "estimator/expressionEstimator.h"

enum{
	ENTRY_EXPRESSION1,
	ENTRY_EXPRESSION2,
	ENTRY_VARIABLES,
	ENTRY_VALUES,
	ENTRY_SIZE
};

class TestFormWindow {
	GtkWidget *m_window,*m_entry[ENTRY_SIZE],*m_label[2];
	ExpressionEstimator e;
public:
	TestFormWindow();
	virtual ~TestFormWindow();
	void inputChanged(GtkWidget*w);
	void recount(int i);
};

#endif /* TESTFORMWINDOW_H_ */
