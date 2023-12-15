#pragma once

#include <QWidget>
#include <QLabel>
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <qlineedit.h>
#include <iostream>
#include <fstream>
#include <string>
#include <WINDOWS.H>
#include <filesystem>
#include <cstdio>
#include <time.h>
#include <algorithm>
#include "choose_files.h"
#include "choose_folder.h"
#include "my_pack_and_encode.h"
#include "QtMainWindow.h"
#include "ui_QtRestoreWindow.h"

extern std::string target_folder2;
extern std::vector <std::string> cho2;

class QtRestoreWindow : public QWidget
{
	Q_OBJECT

public:
	QtRestoreWindow(QWidget *parent = nullptr);
	~QtRestoreWindow();
	void choose_folder();// 文件夹
	void choose_files();// 文件
	void excute();// 打包
	void back();// 返回

private:
	Ui::QtRestoreWindowClass ui;
	QPushButton* folder_b;  // choose folder 按钮
	QPushButton* file_b;	// choose file 按钮
	QPushButton* restore_b;	// pack up 按钮 
	QPushButton* back_b;	// back 按钮
	QPushButton* close_b;	// close 按钮
	QLabel* label; // 显示选择的文件夹
	QLabel* label2; // 显示选择的文件
	QLabel* label3; // key_txt_title
	QScrollArea* scrolllabel2; // 显示选择的文件加上滚动条

	// aes密钥输入框
	QLineEdit* key_txt;

	// aes密钥格式
	QGroupBox* aes_mode_group;
	QRadioButton* aes128_b;
	QRadioButton* aes192_b;
	QRadioButton* aes256_b;
	QVBoxLayout* Layout2;
};
