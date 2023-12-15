#include "QtRestoreWindow.h"

std::string target_folder2;
std::vector <std::string> cho2;
int file_format = 0;
int aes_flag2 = 1;

QtRestoreWindow::QtRestoreWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->setFixedSize(600, 600);

	// 文件
	file_b = new QPushButton(this);
	file_b->setText("choose files");
	file_b->move(0, 100);

	// 文件夹
	folder_b = new QPushButton(this);
	folder_b->setText("choose folder");
	folder_b->move(0, 160);

	// 执行
	restore_b = new QPushButton(this);
	restore_b->setText("restore");
	restore_b->move(320, 550);

	// back
	back_b = new QPushButton(this);
	back_b->setText("back");
	back_b->move(410, 550);

	// close
	close_b = new QPushButton(this);
	close_b->setText("close");
	close_b->move(500, 550);

	// 文件
	label2 = new QLabel(this);
	label2->setStyleSheet("color: black; font-size: 12px;");
	label2->setAlignment(Qt::AlignLeft);
	label2->setVisible(true);
	label2->setText("files choosed: ");
	label2->adjustSize();

	//
	scrolllabel2 = new QScrollArea(this);
	scrolllabel2->setGeometry(0, 0, 500, 100);
	scrolllabel2->setWidget(label2);

	// 文件夹
	label = new QLabel(this);
	label->setStyleSheet("color: black; font-size: 15px;");
	label->setAlignment(Qt::AlignLeft);
	label->setVisible(true);
	label->move(0, 140);
	label->setFixedWidth(500);
	label->setFixedHeight(20);
	label->setStyleSheet("QLabel{background-color:transparent;color:black;}");
	label->setFrameShape(QFrame::Box);
	label->setText("restore to folder: ");

	// file_b: choose_files
	connect(file_b, &QPushButton::released, this, &QtRestoreWindow::choose_files);
	// folder_b: choose_folder
	connect(folder_b, &QPushButton::released, this, &QtRestoreWindow::choose_folder);
	// restore_b: 执行
	connect(restore_b, &QPushButton::released, this, &QtRestoreWindow::excute);
	// back_b: 返回
	connect(back_b, &QPushButton::released, this, &QtRestoreWindow::back);
	// close_b: 关闭界面
	connect(close_b, &QPushButton::pressed, this, &QtRestoreWindow::close);

	// keykey_txt_title
	label3 = new QLabel(this);
	label3->setStyleSheet("color: black; font-size: 12px;");
	label3->setAlignment(Qt::AlignLeft);
	label3->setVisible(true);
	label3->setFixedWidth(200);
	label3->setFixedHeight(20);
	label3->setText("use the key for all the .aes files:");
	label3->move(5, 210);

	// 创建key_txt文本框
	key_txt = new QLineEdit(this);
	key_txt->setGeometry(0, 230, 200, 20);
	key_txt->setEchoMode(QLineEdit::Password); // 密码用圆点代替

	// 创建aes密钥格式分组对象
	aes_mode_group = new QGroupBox(this);
	aes_mode_group->setTitle("aes key mode");
	aes_mode_group->setGeometry(0, 260, 200, 90);

	// 创建三个单选框对象
	aes128_b = new QRadioButton("AES128", aes_mode_group);
	aes192_b = new QRadioButton("AES192", aes_mode_group);
	aes256_b = new QRadioButton("AES256", aes_mode_group);

	// aes128选项为默认值
	aes128_b->setChecked(true);

	// 获取aes_b值
	connect(aes128_b, &QRadioButton::toggled, [=](bool isChecked) {
		if (isChecked == true)
			aes_flag2 = 1;
		});
	connect(aes192_b, &QRadioButton::toggled, [=](bool isChecked) {
		if (isChecked == true)
			aes_flag2 = 2;
		});
	connect(aes256_b, &QRadioButton::toggled, [=](bool isChecked) {
		if (isChecked == true)
			aes_flag2 = 3;
		});

	// 创建布局: 垂直布局, 指定其父对象为分组控件
	Layout2 = new QVBoxLayout(aes_mode_group);
	Layout2->addWidget(aes128_b);
	Layout2->addWidget(aes192_b);
	Layout2->addWidget(aes256_b);

	// 将布局控件添加到组中
	aes_mode_group->setLayout(Layout2);
}

void QtRestoreWindow::choose_folder() {  //文件夹

	target_folder2 = CSelectFolderDlg::Show();
	//b2->setText(QString::fromStdString(target_folder2));
	std::string label_txt = "folder: " + target_folder2;
	label->setText(QString::fromStdString(label_txt));

}

void QtRestoreWindow::choose_files() {  //文件
	cho2 = chooseFiles();
	std::string file_name;
	for (auto path : cho2) {
		file_name += path + "\n";
	}
	std::string label2_txt = "files choosed:\n" + file_name;
	label2->setText(QString::fromStdString(label2_txt));
	label2->adjustSize();
}

void QtRestoreWindow::excute() {
	QString name = key_txt->text();
	std::string s = name.toStdString();
	const char* key = s.c_str();
	qDebug() << key << "\n";
//	qDebug() << target_folder2 << "\n";
	my_restore(target_folder2, cho2, key, aes_flag2);
	
}

void QtRestoreWindow::back() {
	this->close();
	QtMainWindow* pic = new QtMainWindow();
	pic->show();
}

QtRestoreWindow::~QtRestoreWindow()
{}
