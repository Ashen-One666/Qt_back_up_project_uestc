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
	void choose_folder();// �ļ���
	void choose_files();// �ļ�
	void excute();// ���
	void back();// ����

private:
	Ui::QtRestoreWindowClass ui;
	QPushButton* folder_b;  // choose folder ��ť
	QPushButton* file_b;	// choose file ��ť
	QPushButton* restore_b;	// pack up ��ť 
	QPushButton* back_b;	// back ��ť
	QPushButton* close_b;	// close ��ť
	QLabel* label; // ��ʾѡ����ļ���
	QLabel* label2; // ��ʾѡ����ļ�
	QLabel* label3; // key_txt_title
	QScrollArea* scrolllabel2; // ��ʾѡ����ļ����Ϲ�����

	// aes��Կ�����
	QLineEdit* key_txt;

	// aes��Կ��ʽ
	QGroupBox* aes_mode_group;
	QRadioButton* aes128_b;
	QRadioButton* aes192_b;
	QRadioButton* aes256_b;
	QVBoxLayout* Layout2;
};
