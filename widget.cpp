#include "widget.h"
#include "ui_widget.h"

#include <QtNetwork/QHostAddress>
#include <QDebug>
#include <QString>
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setFixedSize(560,650);
    ui->stackedWidget->setCurrentIndex(0);

    QMovie*movie = new QMovie("C:/Users/iot/Desktop/sujeong_backup/yogiyo/banner.gif");
    ui->label_6->setMovie(movie);
    movie->setScaledSize(QSize(563,104));
    movie->start();

    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(Messageread()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(login())); // 로그인
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(membership())); // 회원가입
    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(search())); // 검색창
    connect(ui->buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(category())); // 카테고리별 정보 출력
    connect(ui->buttonGroup_2, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(category_2()));
    connect(ui->buttonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(buttonGroup_clicked()));
    connect(ui->pushButton_38, SIGNAL(clicked()), this, SLOT(lastorder())); // 메뉴 선택
    connect(ui->lineEdit_2, SIGNAL(returnPressed()), this, SLOT(chat()));
    connect(ui->order, SIGNAL(clicked()), this, SLOT(orhistory()));
    connect(ui->order_2, SIGNAL(clicked()), this, SLOT(orhistory()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked() // 사용자가 로고버튼클릭(접속)하면 서버 연결
{
    QString serverIP = "10.10.20.123";
    QString serverPort = "20000";
    QHostAddress serverAddress(serverIP);

    tcpSocket->connectToHost(serverAddress, serverPort.toUShort());

    ui->stackedWidget->setCurrentIndex(3);
}

void Widget::login() // 로그인 데이터 서버에 전송
{
    QString EMAIL = ui->email->text();
    QString PW = ui->password->text();
    QString msg = "U:Login:";
    msg = msg+EMAIL+":"+PW;

    tcpSocket->write(msg.toUtf8());
}

void Widget::membership() // 회원가입 데이터 서버에 전송
{
    QString EMAIL = ui->mbs_email->text();
    QString PW = ui->mbs_pw->text();
    QString NAME = ui->mbs_name->text();
    QString NUMBER = ui->mbs_number->text();
    QString ADDR = ui->mbs_addr->text();
    QString msg = "U:Membership:";
    msg = msg+EMAIL+":"+PW+":"+NAME+":"+NUMBER+":"+ADDR;

    tcpSocket->write(msg.toUtf8());
}

void Widget::search() // 유저 검색 단어 전송
{
    QString SEARCH = ui->lineEdit->text();
    QString msg = "U:Search:";
    msg = msg+SEARCH;

    tcpSocket->write(msg.toUtf8());

    ui->lineEdit->clear();
}

void Widget::category() // 메뉴 카테고리 전송
{
    QString cate = ui->buttonGroup->checkedButton()->text();
    QString msg = "U:Category:";
    msg = msg+cate;
    tcpSocket->write(msg.toUtf8());
}

void Widget::category_2() // 메뉴 카테고리 전송
{
    QString cate_2 = ui->buttonGroup_2->checkedButton()->text();
    QString msg = "U:Category:";
    msg = msg+cate_2;
    tcpSocket->write(msg.toUtf8());
}

void Widget::shop_menu() // 매장 이름 서버에 전송
{
    QPushButton * shop = (QPushButton*)sender(); // 신호를 준 버튼...
    QString shopname = shop->objectName();
    t.append(shopname);
    QString msg = "U:Shop:";
    msg = msg+shopname;
    tcpSocket->write(msg.toUtf8());
}

void Widget::menu_name() // 메뉴 이름 전송
{
    QPushButton * menu = (QPushButton*)sender();
    QString menuname = menu->objectName();
    QString msg = "U:Menu:";
    msg = msg+menuname;
    tcpSocket->write(msg.toUtf8());
}

void Widget::lastorder() // 최종 주문내용 전송
{
    QString shopimage = t[0];
    QString menuname = order[1];
    QString pmenuname;
    for(int i = 3; i<order.size(); i++)
    {
        pmenuname += order[i];
        if(i<order.size()-1)
        {
            pmenuname+="+";
        }
    }
    QString price = ui->label_30->text();
    QString shoprequest = ui->lineEdit_3->text();
    QString riderrequest = ui->lineEdit_4->text();
    QString pay = ui->buttonGroup_3->checkedButton()->text();
    QString msg = "U:Order:";
    msg = msg+shopimage+":"+menuname+":"+pmenuname+":"+price+":"+shoprequest+":"+riderrequest+":"+pay;
    tcpSocket->write(msg.toUtf8());
}

void Widget::orhistory()
{
    QString msg = "U:OrderList";
    tcpSocket->write(msg.toUtf8());
}

void Widget::Messageread() // 서버가 보내는 데이터 수신
{
    if(tcpSocket->bytesAvailable() >=0)
    {
        QString readData = tcpSocket->readAll();

        qDebug()<<"서버가 준 데이터"<<readData;
        if(readData.first(1)=="&")
        {
            msglist = readData.split("#");
        }
        else if(readData.first(1)=="@")
        {
            msglist = readData.split("#");
            menu_print();
        }
        else if(readData.first(1)=="^")
        {
            msglist = readData.split("#");
            plus_menu();
        }
        else if(readData.first(1)=="O")
        {
            msglist = readData.split("#");
            orderHistory();
        }
        else
        {
            msglist = readData.split(":");
        }
        if(msglist[0] == "L")
        {
            if(msglist[1] == "NO")
            {
                QMessageBox::critical(this,"경고","로그인 실패");
                ui->email->clear();
                ui->password->clear();
            }
            else if(msglist[1] == "OK")
            {
                QMessageBox::information(this,"결과","로그인 완료");
                ui->stackedWidget->setCurrentIndex(2);
                ui->stackedWidget_2->setCurrentIndex(1);
                ui->namelbl->setText(msglist[3]);
                ui->emaillbl->setText(msglist[2]);
            }
        }
        if(msglist[0] == "M")
        {
            if(msglist[1]=="ALL")
            {
                QMessageBox::critical(this,"경고","이메일/전화번호 중복");
                qDebug()<<"이메일/전화번호 중복";
            }
            else if(msglist[1]=="EMAIL")
            {
                QMessageBox::critical(this,"경고","이메일 중복");
                qDebug()<<"이메일 중복";
            }
            else if(msglist[1]=="PHONE")
            {
                QMessageBox::critical(this,"경고","전화번호 중복");
                qDebug()<<"전화번호 중복";
            }
            else
            {
                QMessageBox::information(this,"결과","회원가입 완료");
                qDebug()<<"회원가입 완료";
                ui->stackedWidget->setCurrentIndex(3);
            }
        }
        if(msglist[0]=="&")
        {
            category_print(msglist);
        }
        if(msglist[0]=="@")
        {

            ui->label_10->setText(msglist[1]);
            ui->label_11->setText(msglist[5]+" 가게배달");

            QList<QString> msglist1; // 배달비/최소주문
            msglist1 = msglist[3].split(":");
            ui->label_13->setText("배달비  "+msglist1[1]);
            ui->label_12->setText("최소주문   "+ msglist[4]+"원");

            ui->label_17->setText(msglist[6]+"분 후 픽업 가능");
            ui->label_18->setText(msglist[7]);

            menu_cate();
        }
        if(msglist[0]=="C")
        {
            ui->textEdit->append(msglist[1]);
            ui->textEdit->setAlignment(Qt::AlignLeft);            // 서버에서 전송한 채팅데이터 수신
        }
        if(msglist[0]=="S")
        {
            QMessageBox::information(this,"알림","배달이 시작되었습니다!");
        }
    }
}

void Widget::category_print(QList<QString> msglist)
{
    if(h>0)
    {
        ui->verticalLayout_6->takeAt(0)->widget()->hide();
    }
    QScrollArea *scrollArea;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    for(int i=0; i<(msglist.size()-1)/4; i++)
    {

        groupb = new QGroupBox;
        groupb->setFixedSize(500,170);

        image[i] = new QPushButton;
        image[i]->setObjectName(msglist[2+(i*4)]);
        image[i]->setStyleSheet(tr("border-image:url(:/image/image/%1.png);").arg(msglist[2+(i*4)]));
        image[i]->setFixedSize(150,150);
        shopname[i] = new QLabel;
        shopname[i]->setText(msglist[1+(i*4)]);
        shopname[i]->setFixedSize(170,60);
        dinfo[i] = new QLabel;
        dinfo[i]->setText(msglist[4+(i*4)]);
        dinfo[i]->setFixedSize(150,30);
        price[i] = new QLabel("최소금액");
        price[i]->setText(msglist[3+(i*4)]+"원");
        price[i]->setFixedSize(170,30);

        QHBoxLayout *box = new QHBoxLayout;
        box->addWidget(price[i]);
        box->addWidget(dinfo[i]);

        QVBoxLayout *hbox = new QVBoxLayout;
        hbox->addWidget(shopname[i]);
        hbox->addLayout(box);

        QHBoxLayout *vbox = new QHBoxLayout;
        vbox->addWidget(image[i]);
        vbox->addLayout(hbox);

        groupb->setLayout(vbox);

        mainLayout->addWidget(groupb);


        connect(image[i], SIGNAL(clicked()), this, SLOT(shop_menu())); // 메뉴 보내는 시그널
    }

    QWidget*scrollAreaContent = new QWidget;
    scrollAreaContent->setLayout(mainLayout);

    scrollArea = new QScrollArea;
    scrollArea->setWidget(scrollAreaContent);

    ui->verticalLayout_6->addWidget(scrollArea);
    h++;
}

void Widget::menu_print()
{
    ui->stackedWidget->setCurrentIndex(6);
    QScrollArea *scrollArea;

    if(k>0)
    {
        QWidget*removewidget = ui->verticalLayout_10->takeAt(0)->widget();
        ui->verticalLayout_10->removeWidget(removewidget);
        delete removewidget;
    }
    QVBoxLayout *mainLayout = new QVBoxLayout;
    for(int i=0; i<(msglist.size()-8)/4; i++)
    {
        groupbox = new QGroupBox;
        groupbox->setFixedSize(500,170);

        image[i] = new QPushButton;
        image[i]->setObjectName(msglist[8+(i*4)]);
        image[i]->setStyleSheet(tr("border-image:url(:/image/image/%1.png);").arg(msglist[8+(i*4)]));
        image[i]->setFixedSize(150,150);
        menu[i] = new QLabel;
        menu[i]->setText(msglist[9+(i*4)]);
        menu[i]->setFixedSize(170,60);
        price[i] = new QLabel;
        price[i]->setText(msglist[10+(i*4)]+"원");
        price[i]->setFixedSize(170,60);

        QVBoxLayout *box = new QVBoxLayout;
        box->addWidget(menu[i]);
        box->addWidget(price[i]);

        QHBoxLayout *hbox = new QHBoxLayout;
        hbox->addLayout(box);
        hbox->addWidget(image[i]);

        groupbox->setLayout(hbox);

        mainLayout->addWidget(groupbox);

        connect(image[i], SIGNAL(clicked()), this, SLOT(menu_name()));
        connect(image[i], SIGNAL(clicked()), this, SLOT(on_pushButton_menu_clicked()));
    }

    QWidget*scrollAreaContent = new QWidget;
    scrollAreaContent->setLayout(mainLayout);

    scrollArea = new QScrollArea;
    scrollArea->setWidget(scrollAreaContent);

    ui->verticalLayout_10->addWidget(scrollArea);

    k++;
}

void Widget::on_pushButton_menu_clicked() // 상세 메뉴 출력
{
    ui->stackedWidget->setCurrentIndex(7);
    QPushButton *test = (QPushButton*)sender();
    ui->label_14->setStyleSheet(tr("border-image:url(:/image/image/%1.png);").arg(test->objectName()));
    int n = msglist.indexOf(test->objectName());
    ui->label_15->setText(msglist[n+1]);
    ui->label_16->setText(msglist[n+2]+"원");

    order.append(test->objectName());
    order.append(ui->label_15->text());
    order.append(ui->label_16->text());
}

void Widget::plus_menu()
{
    QScrollArea *scrollArea;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    if(l>0)
    {
        ui->verticalLayout_8->takeAt(0)->widget()->hide();
    }

    qDebug()<<"상세메뉴"<<msglist;
    for(int j = 0; j<(msglist.size()-1)/4; j++)
    {
        msglist1 = msglist[1+(j*4)].split(":");

        msglist2 = msglist[2+(j*4)].split(":");

        if(msglist[4]=="X")
        {
            groupbox = new QGroupBox;
            groupbox->setTitle(msglist[3+(j*4)]);
            QVBoxLayout *vbox = new QVBoxLayout;

            for(int i = 0; i<msglist1.size(); i++)
            {
                groupbox->setFixedSize(500,300);

                rdbtn = new QRadioButton;
                rdbtn->setText(msglist1[i]); // 추가메뉴 출력

                connect(rdbtn, SIGNAL(clicked()), this, SLOT(testclick1()));
                price[i] = new QLabel;
                price[i]->setText(msglist2[i]);

                pmenu.insert(msglist1[i], msglist2[i]);
                QHBoxLayout *hbox = new QHBoxLayout;
                hbox->addWidget(rdbtn);
                hbox->addWidget(price[i]);


                vbox->addLayout(hbox);
            }
            groupbox->setLayout(vbox);

            mainLayout->addWidget(groupbox);
        }
    }
    for(int j = 0; j<(msglist.size()-1)/4; j++)
    {
        msglist1 = msglist[1+(j*4)].split(":");

        msglist2 = msglist[2+(j*4)].split(":");

        if(msglist[4]=="O")
        {
            groupbox = new QGroupBox;
            groupbox->setTitle(msglist[3+(j*4)]);
            QVBoxLayout *vbox = new QVBoxLayout;

            for(int i = 0; i<msglist1.size(); i++)
            {
                groupbox->setFixedSize(500,200);

                ckbtn[i] = new QCheckBox;
                ckbtn[i]->setText(msglist1[i]); // 추가메뉴 출력

                price[i] = new QLabel;
                price[i]->setText(msglist2[i]);

                pmenu.insert(msglist1[i], msglist2[i]);

                QHBoxLayout *hbox = new QHBoxLayout;
                hbox->addWidget(ckbtn[i]);
                hbox->addWidget(price[i]);

                vbox->addLayout(hbox);
            }
            groupbox->setLayout(vbox);

            mainLayout->addWidget(groupbox);
        }
    }
    QWidget*scrollAreaContent = new QWidget;
    scrollAreaContent->setLayout(mainLayout);

    scrollArea = new QScrollArea;
    scrollArea->setWidget(scrollAreaContent);

    ui->verticalLayout_8->addWidget(scrollArea);

    l++;
}

void Widget::testclick1()
{
    QRadioButton*rdb = (QRadioButton*)sender();
    QString won = pmenu.value(rdb->text());
    won.erase(won.end()-1);
    total += won.toInt();
    order.append(rdb->text());
    qDebug()<<total;
}

void Widget::testclick2()
{
    QCheckBox*ckb = (QCheckBox*)sender();
    order.append(ckb->text());
    order.append(pmenu.value(ckb->text()));
}

void Widget::on_pushButton_30_clicked() // 주문하기 버튼 눌렀을때
{
    qDebug()<<order;
    ui->stackedWidget->setCurrentIndex(8);
    QScrollArea *scrollArea;
    QVBoxLayout *mainLayout = new QVBoxLayout;

    for(int i=0; i<1; i++)
    {
        groupbox = new QGroupBox;
        groupbox->setFixedSize(500,170);

        shopname[i] = new QLabel;
        shopname[i]->setFixedSize(519,30);
        image1[i] = new QLabel;
        image1[i]->setFixedSize(100,100);
        image1[i]->setStyleSheet(tr("border-image:url(:/image/image/%1.png);").arg(order[0]));
        menu[i] = new QLabel;
        menu[i]->setText(order[1]);
        menu[i]->setFixedSize(300,30);

        QString a;
        plusmenu[i] = new QLabel;
        for(int i = 3; i<order.size();i++)
        {
            a += order[i];
            if(i<order.size()-1)
            {
                a+="+";
            }
        }
        plusmenu[i]->setText(a);
        plusmenu[i]->setFixedSize(300,20);

        price[i] = new QLabel;
        QString won = order[2];
        won.erase(won.end()-1);
        total+=won.toInt();
        price[i]->setText(QString::number(total)+"원");
        price[i]->setFixedSize(300,30);

        QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addWidget(menu[i]);
        vbox->addWidget(plusmenu[i]);
        vbox->addWidget(price[i]);

        QHBoxLayout *hbox = new QHBoxLayout;
        hbox->addWidget(image1[i]);
        hbox->addLayout(vbox);

        groupbox->setLayout(hbox);

        mainLayout->addWidget(groupbox);
    }

    QWidget*scrollAreaContent = new QWidget;
    scrollAreaContent->setLayout(mainLayout);

    scrollArea = new QScrollArea;
    scrollArea->setWidget(scrollAreaContent);

    ui->verticalLayout_12->addWidget(scrollArea);
    ui->label_30->setText(QString::number(total)+"원");
    ui->pushButton_38->setText(QString::number(total)+"원 결제하기");
}

void Widget::menu_cate() // 매장 들어갔을때 작게 나오는 카테고리
{
    if(j>0)
    {
        ui->horizontalLayout_6->takeAt(0)->widget()->hide();
    }
    QScrollArea *scrollArea;
    QHBoxLayout *mainLayout = new QHBoxLayout;
    groupbox = new QGroupBox;
    groupbox->setFixedSize(1000,50);
    QHBoxLayout * hbox= new QHBoxLayout;;
    qDebug()<< "여기서 읽는 데이터" << msglist;
    QSet<QString> catelist;
    for(int i = 0; i < msglist.size()-8; i=i+4)
    {
        catelist.insert(msglist[11+i]);
    }
    qDebug() << catelist;
    int i = 0;
    QSet<QString>::Iterator p;
    for(p = catelist.begin(); p != catelist.end(); ++p)
    {
        btn[i] = new QPushButton;
        btn[i]->setText(*p);
        btn[i]->setFixedSize(100,20);

        hbox->addWidget(btn[i]);

        connect(btn[i], SIGNAL(clicked()), this, SLOT(on_category_menu()));
        ++i;
    }
    groupbox->setLayout(hbox);

    mainLayout->addWidget(groupbox);

    QWidget*scrollAreaContent = new QWidget;
    scrollAreaContent->setLayout(mainLayout);

    scrollArea = new QScrollArea;
    scrollArea->setWidget(scrollAreaContent);

    ui->horizontalLayout_6->addWidget(scrollArea);
    j++;
}

void Widget::on_category_menu() //미니 카테고리 눌렀을때 메뉴 출력
{
    QScrollArea *scrollArea;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QPushButton*btn = (QPushButton*)sender();

    for(int i=0; i<(msglist.size()-8)/4; i++)
    {
        if(k>0)
        {
            ui->verticalLayout_10->takeAt(0)->widget()->hide();
        }
        if(msglist[11+(i*4)] == btn->text())
        {
            groupbox = new QGroupBox;
            groupbox->setFixedSize(500,170);

            image[i] = new QPushButton;
            image[i]->setObjectName(msglist[11+(i*4)-3]);
            image[i]->setStyleSheet(tr("border-image:url(:/image/image/%1.png);").arg(msglist[8+(i*4)]));
            image[i]->setFixedSize(150,150);
            menu[i] = new QLabel;
            menu[i]->setText(msglist[11+(i*4)-2]);
            menu[i]->setFixedSize(170,60);
            price[i] = new QLabel;
            price[i]->setText(msglist[11+(i*4)-1]+"원");
            price[i]->setFixedSize(170,60);

            QVBoxLayout *box = new QVBoxLayout;
            box->addWidget(menu[i]);
            box->addWidget(price[i]);

            QHBoxLayout *hbox = new QHBoxLayout;
            hbox->addLayout(box);
            hbox->addWidget(image[i]);

            groupbox->setLayout(hbox);

            mainLayout->addWidget(groupbox);

            connect(image[i], SIGNAL(clicked()), this, SLOT(on_pushButton_menu_clicked()));
        }
        QWidget*scrollAreaContent = new QWidget;
        scrollAreaContent->setLayout(mainLayout);

        scrollArea = new QScrollArea;
        scrollArea->setWidget(scrollAreaContent);

        ui->verticalLayout_10->addWidget(scrollArea);
        a++;
    }
}

void Widget::orderHistory() // 주문 내역
{
    ui->stackedWidget->setCurrentIndex(10);
    QScrollArea *scrollArea;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    for(int i=0; i<msglist.size()-1; i++)
    {
        if(c>0)
        {
            ui->verticalLayout_16->takeAt(0)->widget()->hide();
        }
        msglist3 = msglist[i+1].split(":");
        groupb = new QGroupBox;
        groupb->setFixedSize(500,170);
        groupb->setTitle(msglist3[0]);


        image[i] = new QPushButton;
        image[i]->setObjectName(msglist3[1]);
        image[i]->setStyleSheet(tr("border-image:url(:/image/image/%1.png);").arg(msglist3[1]));
        image[i]->setFixedSize(140,140);
        menu[i] = new QLabel;
        menu[i]->setText(msglist3[2]);
        menu[i]->setFixedSize(150,30);
        plusmenu[i] = new QLabel;
        plusmenu[i]->setText(msglist3[3]);
        plusmenu[i]->setFixedSize(150,30);
        price[i] = new QLabel;
        price[i]->setText(msglist3[4]);
        price[i]->setFixedSize(170,30);

        QVBoxLayout *vbox = new QVBoxLayout;
        vbox->addWidget(menu[i]);
        vbox->addWidget(plusmenu[i]);
        vbox->addWidget(price[i]);

        QHBoxLayout *hbox = new QHBoxLayout;
        hbox->addWidget(image[i]);
        hbox->addLayout(vbox);

        groupb->setLayout(hbox);

        mainLayout->addWidget(groupb);

        connect(image[i], SIGNAL(clicked()), this, SLOT(order_detail()));
    }

    QWidget*scrollAreaContent = new QWidget;
    scrollAreaContent->setLayout(mainLayout);

    scrollArea = new QScrollArea;
    scrollArea->setWidget(scrollAreaContent);

    ui->verticalLayout_16->addWidget(scrollArea);

    c++;
}

void Widget::order_detail() // 주문 상세내역
{
    ui->stackedWidget->setCurrentIndex(11);

    button = (QPushButton*)sender();
    if(button->objectName() == msglist3[1])
    {
        for(int i = 0; i<msglist.size()-1; i++)
        {
            qDebug()<<msglist3;
            msglist3 = msglist[i+1].split(":");
            ui->label_28->setText(msglist3[0]); // 가게이름

            ui->label_32->setText(msglist3[2]); // 메뉴
            ui->label_40->setText(msglist3[3]); // 추가 메뉴

            ui->label_34->setText(msglist3[4]); // 가격


            ui->label_36->setText(msglist3[8]); // 전화번호
            ui->label_41->setText(msglist3[9]); // 주소
            ui->label_37->setText(msglist3[7]); // 결제방법
            ui->label_38->setText(msglist3[4]); // 가게 요청사항
            ui->label_39->setText(msglist3[5]); //라이더 요청사항
        }
    }
}

void Widget::chat() // 채팅 클라이언트
{
    QString chatmsg = ui->lineEdit_2->text().trimmed();

    if(!chatmsg.isEmpty())
    {
        QString msg = "C:";
        msg = msg+chatmsg;
        ui->textEdit->append(chatmsg);
        ui->textEdit->setAlignment(Qt::AlignRight);
        tcpSocket->write(msg.toUtf8());
    }

    ui->lineEdit_2->clear();
    ui->lineEdit_2->setFocus();
}

// 버튼 클릭 슬롯 모음
void Widget::on_membership_clicked() // 이메일 회원가입 버튼 누르면 회원가입 창으로
{
    ui->stackedWidget->setCurrentIndex(4);
}

void Widget::on_home_2_clicked() // 홈버튼 누르면 카테고리 페이지로
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Widget::buttonGroup_clicked() // 카테고리 버튼 누르면 매장 출력 페이지로
{
    ui->stackedWidget->setCurrentIndex(5);
}

void Widget::on_pushButton_27_clicked() // 이전 버튼
{
    ui->stackedWidget->setCurrentIndex(5);
}

void Widget::on_pushButton_31_clicked()
{
    ui->stackedWidget->setCurrentIndex(7);
}

void Widget::on_profile_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void Widget::on_loginbtn_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(9);
}

void Widget::on_pushButton_39_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void Widget::on_pushButton_40_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}

// void Widget::on_order_clicked()
// {
//     ui->stackedWidget->setCurrentIndex(10);
// }


void Widget::on_pushButton_45_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void Widget::on_pushButton_44_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}


void Widget::on_pushButton_43_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void Widget::on_pushButton_41_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void Widget::on_pushButton_42_clicked()
{
    ui->stackedWidget->setCurrentIndex(10);
}


void Widget::on_pushButton_38_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

