#pragma once

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include "ui_QtMainWindow.h"
#include "QtMainWindow.h"
#include "QtBackupWindow.h"
#include "QtRestoreWindow.h"

class QtMainWindow : public QWidget
{
	Q_OBJECT

public:
	QtMainWindow(QWidget *parent = nullptr);
	~QtMainWindow();
	void next();
	//void close();

private:
	Ui::QtMainWindowClass ui;
	QLabel* title; // Ö÷Ìâ
	QGroupBox* Group;
	QRadioButton* backup_b;
	QRadioButton* restore_b;
	QVBoxLayout* Layout;
	QPushButton* next_b;
	QPushButton* close_b;
};
