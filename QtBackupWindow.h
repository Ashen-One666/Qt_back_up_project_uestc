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
	void choose_folder();// �ļ���
	void choose_files();// �ļ�
	void excute();// ���
	void back();// ����
	void timerEvent(QTimerEvent* event);// ��дtimerEvent

private:
	Ui::QtBackupWindowClass ui;
	
	QPushButton* folder_b;  // choose folder ��ť
	QPushButton* file_b;	// choose file ��ť
	QPushButton* pack_b;	// pack up ��ť 
	QPushButton* back_b;	// back ��ť
	QPushButton* close_b;	// close ��ť
	QLabel* label; // ��ʾѡ����ļ���
	QLabel* label2; // ��ʾѡ����ļ�
	QLabel* label3; // key_txt_title
	QLabel* label4; // aes_mode_title
	QScrollArea* scrolllabel2; // ��ʾѡ����ļ����Ϲ�����
	 
	// �����ʽ
	QGroupBox* pack_mode_group;
	QRadioButton* mode1_b;
	QRadioButton* mode2_b;
	QRadioButton* mode3_b;
	QRadioButton* mode4_b;
	QRadioButton* mode5_b;
	QVBoxLayout* Layout;

	// aes��Կ�����
	QLineEdit* key_txt;

	// aes��Կ��ʽ
	QGroupBox* aes_mode_group;
	QRadioButton* aes128_b;
	QRadioButton* aes192_b;
	QRadioButton* aes256_b;
	QHBoxLayout* Layout2;

	// ��ʱ����ģʽ
	QGroupBox* scheduled_group;
	QRadioButton* once_b;
	QRadioButton* repeat_b;
	QVBoxLayout* Layout3;

	// once ʱ��Edit�ı���
	QLabel* onceLabel;
	QDateTimeEdit* datetimeEdit;

	// repeat ʱ��Edit�ı���
	QLabel* repeatLabel; // title
	QLineEdit* dayEdit;	
	QLabel* dayLabel; // "days"
	QTimeEdit* timeEdit;
	QLabel* timeLabel; // "hh:mm:ss"

	// repeat ��ʹ��qTimer��ʱ��
	QTimer* qtimer;
	int timer_id = -1;

};
