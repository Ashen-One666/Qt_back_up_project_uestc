#include "QtMainWindow.h"

int RadioButton_flag = 0;

QtMainWindow::QtMainWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setFixedSize(600, 600);
	RadioButton_flag = 0;

	// 创建分组对象
	Group = new QGroupBox(this);
	Group->setTitle("backup or restore");
	Group->setGeometry(30, 50, 200, 100);
	
	// 创建两个单选框对象
	backup_b = new QRadioButton("backup", Group);
	restore_b = new QRadioButton("restore", Group);

	// 备份选项为默认值
	backup_b->setChecked(true);

	// 获取RadioButton值
	connect(backup_b, &QRadioButton::toggled, [=](bool isChecked) {
		if (isChecked == true)
			RadioButton_flag = 0;
		else if (isChecked == false)
			RadioButton_flag = 1;
		});

	// 创建布局: 垂直布局, 指定其父对象为分组控件
	Layout = new QVBoxLayout(Group);
	Layout->addWidget(backup_b);
	Layout->addWidget(restore_b);

	// 将布局控件添加到组中
	Group->setLayout(Layout);

	// 创建next按钮
	QPushButton* next_b = new QPushButton(this);
	next_b->setText("next");
	next_b->move(420, 550);
	connect(next_b, &QPushButton::released, this, &QtMainWindow::next);

	// 创建close按钮
	QPushButton* close_b = new QPushButton(this);
	close_b->setText("close");
	close_b->move(500, 550);
	connect(close_b, &QPushButton::released, this, &QtMainWindow::close);
}

QtMainWindow::~QtMainWindow()
{}

void QtMainWindow::next()
{
	if (RadioButton_flag == 0) {
		qDebug() << "backup";
		this->close();
		QtBackupWindow* pic = new QtBackupWindow();
		pic->show();
	}

	else if (RadioButton_flag == 1) {
		this->close();
		QtRestoreWindow* pic = new QtRestoreWindow();
		pic->show();
		qDebug() << "restore";
	}

	else qDebug() << "error";
}

