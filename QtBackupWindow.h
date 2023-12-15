#pragma once

#include <QWidget>
#include "ui_QtBackupWindow.h"
#include <qtimer.h>
#include <QLabel>
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <qlineedit.h>
#include <qdatetimeedit.h>
#include <QScrollArea>
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

extern std::string target_folder;
extern std::vector <std::string> cho;

class QtBackupWindow : public QWidget
{
	Q_OBJECT

public:
	QtBackupWindow(QWidget *parent = nullptr);
	~QtBackupWindow();
	void choose_folder();// 文件夹
	void choose_files();// 文件
	void excute();// 打包
	void back();// 返回
	void timerEvent(QTimerEvent* event);// 重写timerEvent

private:
	Ui::QtBackupWindowClass ui;
	
	QPushButton* folder_b;  // choose folder 按钮
	QPushButton* file_b;	// choose file 按钮
	QPushButton* pack_b;	// pack up 按钮 
	QPushButton* back_b;	// back 按钮
	QPushButton* close_b;	// close 按钮
	QLabel* label; // 显示选择的文件夹
	QLabel* label2; // 显示选择的文件
	QLabel* label3; // key_txt_title
	QLabel* label4; // aes_mode_title
	QScrollArea* scrolllabel2; // 显示选择的文件加上滚动条
	 
	// 打包方式
	QGroupBox* pack_mode_group;
	QRadioButton* mode1_b;
	QRadioButton* mode2_b;
	QRadioButton* mode3_b;
	QRadioButton* mode4_b;
	QRadioButton* mode5_b;
	QVBoxLayout* Layout;

	// aes密钥输入框
	QLineEdit* key_txt;

	// aes密钥格式
	QGroupBox* aes_mode_group;
	QRadioButton* aes128_b;
	QRadioButton* aes192_b;
	QRadioButton* aes256_b;
	QHBoxLayout* Layout2;

	// 定时备份模式
	QGroupBox* scheduled_group;
	QRadioButton* once_b;
	QRadioButton* repeat_b;
	QVBoxLayout* Layout3;

	// once 时间Edit文本框
	QLabel* onceLabel;
	QDateTimeEdit* datetimeEdit;

	// repeat 时间Edit文本框
	QLabel* repeatLabel; // title
	QLineEdit* dayEdit;	
	QLabel* dayLabel; // "days"
	QTimeEdit* timeEdit;
	QLabel* timeLabel; // "hh:mm:ss"

	// repeat 中使用qTimer定时器
	QTimer* qtimer;
	int timer_id = -1;

};
