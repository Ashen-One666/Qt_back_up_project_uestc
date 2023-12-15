#include "QtBackupWindow.h"

std::string target_folder;
std::vector <std::string> cho;
int mode_flag = 1;
int aes_flag = 1;
int is_scheduled = 0;
int scheduled_mode = 0;

void ss()
{
    qDebug() << "1";
}
QtBackupWindow::QtBackupWindow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
    this->setFixedSize(600, 600);

    
  //  timer_id = startTimer(1000);

    // 文件夹
    folder_b = new QPushButton(this);
    folder_b->setText("choose folder");
    folder_b->move(0, 20);

    // 文件
    file_b = new QPushButton(this);
    file_b->setText("choose files");
    file_b->move(0, 160);

    // 执行
    pack_b = new QPushButton(this);
    pack_b->setText("pack up");
    pack_b->move(320, 550);

    // back
    back_b = new QPushButton(this);
    back_b->setText("back");
    back_b->move(410, 550);

    // close
    close_b = new QPushButton(this);
    close_b->setText("close");
    close_b->move(500, 550);

    // 文件夹
    label = new QLabel(this);
    label->setStyleSheet("color: black; font-size: 15px;");
    label->setAlignment(Qt::AlignLeft);
    label->setVisible(true);
    label->setFixedWidth(500);
    label->setFixedHeight(20);
    label->setStyleSheet("QLabel{background-color:transparent;color:black;}");
    label->setFrameShape(QFrame::Box);
    label->setText("save to folder: ");

    

    // 文件
    label2 = new QLabel(this);
    label2->setStyleSheet("color: black; font-size: 12px;");
    label2->setAlignment(Qt::AlignLeft);
    label2->setVisible(true);
    label2->move(0, 60);
    label2->setText("files choosed: ");
    label2->adjustSize();
    
    //
    scrolllabel2 = new QScrollArea(this);
    scrolllabel2->setGeometry(0, 60, 500, 100);
    scrolllabel2->setWidget(label2);

    // folder_b: mySlot1
    connect(folder_b, &QPushButton::released, this, &QtBackupWindow::choose_folder);
    // file_b: mySlot2
    connect(file_b, &QPushButton::released, this, &QtBackupWindow::choose_files);
    // pack_b: 执行
    connect(pack_b, &QPushButton::released, this, &QtBackupWindow::excute);
    // back_b: 返回
    connect(back_b, &QPushButton::released, this, &QtBackupWindow::back);
    // close_b: 关闭界面
    connect(close_b, &QPushButton::pressed, this, &QtBackupWindow::close);


    // 创建分组对象
    pack_mode_group = new QGroupBox(this);
    pack_mode_group->setTitle("pack_mode");
    pack_mode_group->setGeometry(10, 200, 200, 150);

    // 创建五个单选框对象
    mode1_b = new QRadioButton("only back up", pack_mode_group);
    mode2_b = new QRadioButton(".tar", pack_mode_group);
    mode3_b = new QRadioButton(".tar.lz", pack_mode_group);
    mode4_b = new QRadioButton(".tar.aes", pack_mode_group);
    mode5_b = new QRadioButton(".tar.lz.aes", pack_mode_group);

    // 备份选项为默认值
    mode1_b->setChecked(true);

    // 获取RadioButton值
    connect(mode1_b, &QRadioButton::toggled, [=](bool isChecked) {
        if (isChecked == true) {
            mode_flag = 1; 
            label3->setVisible(false);
            label4->setVisible(false);
            key_txt->setVisible(false);
            aes128_b->setVisible(false);
            aes192_b->setVisible(false);
            aes256_b->setVisible(false);
            aes_mode_group->setStyleSheet("QGroupBox {border: 0px solid transparent;}");
            aes_mode_group->setTitle("");
        }
            
        });
    connect(mode2_b, &QRadioButton::toggled, [=](bool isChecked) {
        if (isChecked == true) {
            mode_flag = 2; 
            label3->setVisible(false);
            label4->setVisible(false);
            key_txt->setVisible(false);
            aes128_b->setVisible(false);
            aes192_b->setVisible(false);
            aes256_b->setVisible(false);
            aes_mode_group->setStyleSheet("QGroupBox {border: 0px solid transparent;}");
            aes_mode_group->setTitle("");
        }
            
        });
    connect(mode3_b, &QRadioButton::toggled, [=](bool isChecked) {
        if (isChecked == true) {
            mode_flag = 3; 
            label3->setVisible(false);
            label4->setVisible(false);
            key_txt->setVisible(false);
            aes128_b->setVisible(false);
            aes192_b->setVisible(false);
            aes256_b->setVisible(false);
            aes_mode_group->setStyleSheet("QGroupBox {border: 0px solid transparent;}");
            aes_mode_group->setTitle("");
        }
            
        });
    connect(mode4_b, &QRadioButton::toggled, [=](bool isChecked) {
        if (isChecked == true) {
            mode_flag = 4;
            label3->setVisible(true);
            label4->setVisible(true);
            key_txt->setVisible(true);
            aes128_b->setVisible(true);
            aes192_b->setVisible(true);
            aes256_b->setVisible(true);
            aes_mode_group->setStyleSheet("QGroupBox {border: 0;}");
            //aes_mode_group->setTitle("aes key mode");
        }
             
        });
    connect(mode5_b, &QRadioButton::toggled, [=](bool isChecked) {
        if (isChecked == true) {
            mode_flag = 5;
            label3->setVisible(true);
            label4->setVisible(true);
            key_txt->setVisible(true);
            aes128_b->setVisible(true);
            aes192_b->setVisible(true);
            aes256_b->setVisible(true);
            
            aes_mode_group->setStyleSheet("QGroupBox {border: 0;}");
            //aes_mode_group->setTitle("aes key mode");
        }
             
        });

    // 创建布局: 垂直布局, 指定其父对象为分组控件
    Layout = new QVBoxLayout(pack_mode_group);
    Layout->addWidget(mode1_b);
    Layout->addWidget(mode2_b);
    Layout->addWidget(mode3_b);
    Layout->addWidget(mode4_b);
    Layout->addWidget(mode5_b);

    // 将布局控件添加到组中
    pack_mode_group->setLayout(Layout);

    // keykey_txt_title
    label3 = new QLabel(this);
    label3->setStyleSheet("color: black; font-size: 12px;");
    label3->setAlignment(Qt::AlignLeft);
    label3->setVisible(false);
    label3->setFixedWidth(200);
    label3->setFixedHeight(20);
    label3->setText("please set the key for .aes:");
    label3->move(250, 200);

    // 创建key_txt文本框
    key_txt = new QLineEdit(this);
    key_txt->setGeometry(250, 230, 300, 20);
    key_txt->setVisible(false);
    key_txt->setEchoMode(QLineEdit::Password); // 密码用圆点代替

    // aes_mode_title
    label4 = new QLabel(this);
    label4->setStyleSheet("color: black; font-size: 12px;");
    label4->setAlignment(Qt::AlignLeft);
    label4->setVisible(false);
    label4->setFixedWidth(200);
    label4->setFixedHeight(20);
    label4->setText("aes mode:");
    label4->move(250, 270);

    // 创建aes密钥格式分组对象
    aes_mode_group = new QGroupBox(this);
    //aes_mode_group->setTitle("aes key mode");
    aes_mode_group->setGeometry(240, 280, 300, 40);
    aes_mode_group->setStyleSheet("QGroupBox {border: 0;}");

    // 创建三个单选框对象
    aes128_b = new QRadioButton("AES128", aes_mode_group);
    aes192_b = new QRadioButton("AES192", aes_mode_group);
    aes256_b = new QRadioButton("AES256", aes_mode_group);
    aes128_b->setVisible(false);
    aes192_b->setVisible(false);
    aes256_b->setVisible(false);

    // aes128选项为默认值
    aes128_b->setChecked(true);

    // 获取aes_b值
    connect(aes128_b, &QRadioButton::toggled, [=](bool isChecked) {
        if (isChecked == true)
            aes_flag = 1; 
        });
    connect(aes192_b, &QRadioButton::toggled, [=](bool isChecked) {
        if (isChecked == true)
            aes_flag = 2; 
        });
    connect(aes256_b, &QRadioButton::toggled, [=](bool isChecked) {
        if (isChecked == true)
            aes_flag = 3; 
        });

    // 创建布局: 垂直布局, 指定其父对象为分组控件
    Layout2 = new QHBoxLayout(aes_mode_group);
    Layout2->addWidget(aes128_b);
    Layout2->addWidget(aes192_b);
    Layout2->addWidget(aes256_b);

    // 将布局控件添加到组中
    aes_mode_group->setLayout(Layout2);


    // 创建时间分组对象
    scheduled_group = new QGroupBox(this);
    scheduled_group->setTitle("scheduled back up");
    scheduled_group->setGeometry(10, 400, 200, 100);
    scheduled_group->setCheckable(true);
    scheduled_group->setChecked(false);

    // 创建两个单选框对象
    QRadioButton* once_b = new QRadioButton("only once", scheduled_group);
    QRadioButton* repeat_b = new QRadioButton("repeat", scheduled_group);

    // once_b选项为默认值
    //once_b->setChecked(true);

    // 获取once_b值
    connect(once_b, &QRadioButton::toggled, [=](bool isChecked) {
        if (isChecked == true) {
            scheduled_mode = 0;
            // once组件显示
            onceLabel->setVisible(true);
            datetimeEdit->setVisible(true);
            // repeat组件隐藏
            repeatLabel->setVisible(false);
            dayEdit->setVisible(false);
            dayLabel->setVisible(false);
            timeEdit->setVisible(false);
            timeLabel->setVisible(false);
        }
        else {
            scheduled_mode = 1;
            // once组件隐藏
            onceLabel->setVisible(false);
            datetimeEdit->setVisible(false);
            // repeat组件显示
            repeatLabel->setVisible(true);
            dayEdit->setVisible(true);
            dayLabel->setVisible(true);
            timeEdit->setVisible(true);
            timeLabel->setVisible(true);
        }
        });

    // 创建时间组件布局
    Layout3 = new QVBoxLayout;
    Layout3->addWidget(once_b);
    Layout3->addWidget(repeat_b);

    // 将布局控件添加到组中
    scheduled_group->setLayout(Layout3);

    // 是否勾选定时备份
    connect(scheduled_group, &QGroupBox::toggled, [=](bool isChecked) {
        if (isChecked == true) {
            is_scheduled = 1;
            scheduled_mode = 0;
            once_b->setChecked(true);

            // once组件显示
            onceLabel->setVisible(true);
            datetimeEdit->setVisible(true);
            // repeat组件隐藏
            repeatLabel->setVisible(false);
            dayEdit->setVisible(false);
            dayLabel->setVisible(false);
            timeEdit->setVisible(false);
            timeLabel->setVisible(false);
        }
        else {
            is_scheduled = 0;
            scheduled_mode = -1;
            once_b->setChecked(false);
            repeat_b->setChecked(false);

            // once组件隐藏
            onceLabel->setVisible(false);
            datetimeEdit->setVisible(false);
            // repeat组件隐藏
            repeatLabel->setVisible(false);
            dayEdit->setVisible(false);
            dayLabel->setVisible(false);
            timeEdit->setVisible(false);
            timeLabel->setVisible(false);
        }
        });

    // 时间组件 once
    onceLabel = new QLabel(this);
    onceLabel->setStyleSheet("color: black; font-size: 12px;");
    onceLabel->setAlignment(Qt::AlignLeft);
    onceLabel->setVisible(false);
    onceLabel->setFixedWidth(200);
    onceLabel->setFixedHeight(20);
    onceLabel->setText("please set the date:");
    onceLabel->move(250, 400);

    datetimeEdit = new QDateTimeEdit(this);
    datetimeEdit->move(250, 430);
    datetimeEdit->setVisible(false);
    datetimeEdit->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    datetimeEdit->setDateTime(QDateTime::currentDateTime());


    
    // 时间组件 interval
    repeatLabel = new QLabel(this);
    repeatLabel->setStyleSheet("color: black; font-size: 12px;");
    repeatLabel->setAlignment(Qt::AlignLeft);
    repeatLabel->setVisible(false);
    repeatLabel->setFixedWidth(200);
    repeatLabel->setFixedHeight(20);
    repeatLabel->setText("please set the interval:");
    repeatLabel->move(250, 400);
    
    dayEdit = new QLineEdit(this);
    dayEdit->move(250, 430);
    dayEdit->setGeometry(250, 430, 50, 20);
    dayEdit->setVisible(false);
    dayEdit->setValidator(new QIntValidator(dayEdit));
    dayEdit->setText("0");

    dayLabel = new QLabel(this);
    dayLabel->setStyleSheet("color: black; font-size: 12px;");
    dayLabel->setAlignment(Qt::AlignLeft);
    dayLabel->setVisible(false);
    dayLabel->setFixedWidth(30);
    dayLabel->setFixedHeight(20);
    dayLabel->setText("days");
    dayLabel->move(305, 432);

    timeEdit = new QTimeEdit(this);
    timeEdit->move(350, 430);
    timeEdit->setVisible(false);
    timeEdit->setDisplayFormat("hh:mm:ss");
    timeEdit->adjustSize();

    timeLabel = new QLabel(this);
    timeLabel->setStyleSheet("color: black; font-size: 12px;");
    timeLabel->setAlignment(Qt::AlignLeft);
    timeLabel->setVisible(false);
    timeLabel->setFixedWidth(100);
    timeLabel->setFixedHeight(20);
    timeLabel->setText("hh:mm:ss");
    timeLabel->move(430, 432);
}

void QtBackupWindow::choose_folder() {  //文件夹

    target_folder = CSelectFolderDlg::Show();
    //b2->setText(QString::fromStdString(target_folder));
    std::string label_txt = "folder: " + target_folder;
    label->setText(QString::fromStdString(label_txt));

}

void QtBackupWindow::choose_files() {  //文件
    cho = chooseFiles();
    std::string file_name;
    for (auto path : cho) {
        file_name += path + "\n";
    }
    std::string label2_txt = "files choosed:\n" + file_name;
    label2->setText(QString::fromStdString(label2_txt));
    label2->adjustSize();
}

void QtBackupWindow::excute() {
    // void setEchoMode(EchoMode)
   

    QString name = key_txt->text();
    std::string s = name.toStdString();
    const char *key = s.c_str();
    // qtimer = new QTimerEvent();


    if (is_scheduled == 0) {
        my_packup(target_folder, cho, key, aes_flag, mode_flag);
    }
    else
    {
        if (scheduled_mode == 0)
        {
            QDateTime dateTime1 = QDateTime::currentDateTime(); 
            QDateTime dateTime2 = datetimeEdit->dateTime();
            int seconds = dateTime1.secsTo(dateTime2);
            seconds += 1;
            qDebug() << dateTime1<<" "<<dateTime2 <<" "<<seconds<<"\n";
            if (seconds > 0) {
                qDebug() << target_folder << " " << key << " " << aes_flag << " " << mode_flag << "\n";
                qtimer = new QTimer();
                qtimer->singleShot(seconds*1000, this, [=] {
                    //想要执行的代码
                    my_packup(target_folder, cho, key, aes_flag, mode_flag);
                    });
            }
        }
        else
        {
            
            QString dTime_QS = dayEdit->text();
            std::string dTime_s = dTime_QS.toStdString();
            int dTime = atoi(dTime_s.c_str());
            int htime = timeEdit->time().hour(), mtime = timeEdit->time().minute(), stime = timeEdit->time().second();
            int seconds = (int)(86400*dTime * +3600 * htime + 60 * mtime + stime);
            qDebug() <<dTime << " " << htime << " " << mtime<<" "<<stime<<" "<<seconds << "\n";
            if (seconds > 0) {
                // qDebug() << target_folder << " " << key << " " << aes_flag << " " << mode_flag << "\n";
                timer_id = startTimer(seconds*1000);
            }
        }
    }
 
  //--------------------old version (without QT)--------------------
  
  //  my_packup(target_folder,cho,key,aes_flag,mode_flag);
  /*
    for (auto path : cho)
        file_to_folder(path, target_folder);
    
  
    QString name = key_txt->text();
    std::string key = name.toStdString();
    qDebug() << "key: " << key << "\n";
    qDebug() << "mode flag: " << mode_flag << "\n";
    qDebug() << "aes flag: " << aes_flag << "\n";
   
    QString name = key_txt->text();
    std::string key = name.toStdString();

    my_pack(target_folder, "C:\\Users\\fez0618\\Desktop\\123.tar");
    my_encode("C:\\Users\\fez0618\\Desktop\\123.tar", "C:\\Users\\fez0618\\Desktop\\123.tar.aes", key.c_str(), aes::AES_128);
    my_decode("C:\\Users\\fez0618\\Desktop\\123.tar.aes", "C:\\Users\\fez0618\\Desktop\\123_decode.tar", key.c_str(), aes::AES_128);
    my_unpack("C:\\Users\\fez0618\\Desktop\\123_decode.tar", "C:\\Users\\fez0618\\Desktop\\123_decode_unpack");
     */

    // aes Aes;
    // Aes.setKey((unsigned char*)"123", aes::AES_128);
    // Aes.encryptFile("C:\\Users\\fez0618\\Desktop\\0.pdf", "C:\\Users\\fez0618\\Desktop\\0.pdf.aes");
    // Aes.decryptFile("C:\\Users\\fez0618\\Desktop\\0.pdf.aes", "C:\\Users\\fez0618\\Desktop\\0_decode.pdf");
}

void QtBackupWindow::back() {
    this->close();
    QtMainWindow* pic = new QtMainWindow();
    pic->show();
}

void QtBackupWindow::timerEvent(QTimerEvent *event) {
    if (event->timerId() == timer_id) {
        QString name = key_txt->text();
        std::string s = name.toStdString();
        const char* key = s.c_str();
        my_packup(target_folder, cho, key, aes_flag, mode_flag);
    }
}

QtBackupWindow::~QtBackupWindow()
{}

