#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QFileInfo>
#include <QString>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QNetworkInterface>
#include <QMovie>
#include <QGroupBox>
// #include <QLabel>
#include <QtWidgets>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    QTcpSocket *tcpSocket;
    QGroupBox*groupb;
    QGroupBox*groupbox;

    QPushButton*btn[100];
    QPushButton*image[100];
    QLabel*image1[100];
    QLabel*shopname[100];
    QLabel*dinfo[100];
    QLabel*price[100];
    QLabel*menu[100];
    QLabel*plusmenu[100];
    QRadioButton *rdbtn;
    QCheckBox *ckbtn[100];
    // QLabel * title[100];

    int h = 0;
    int k = 0;
    int j = 0;
    int l = 0;
    int a = 0;
    int b = 0;
    int c = 0;
    int total = 0;
    void category_print(QList<QString> msglist);
    QList<QString> msglist;
    void plus_menu();

    QList<QString> order;
    QList<QString> t;
    QMap<QString, QString> pmenu;

    QList<QString> msglist1;
    QList<QString> msglist2;
    QList<QString> msglist3;

    QPushButton*button;

private slots:
    void login(); // 로그인 데이터
    void membership(); // 회원가입 데이터
    void search(); // 검색 데이터
    void shop_menu(); // 가게 데이터
    void menu_name(); // 메뉴이름 데이터
    void lastorder(); // 주문 내용 데이터
    void Messageread(); // 서버 데이터 수신
    void orhistory(); //서버에 주문내역 요청 수신
    void category();
    void category_2();
    void menu_print();
    void menu_cate();
    void on_category_menu();
    void orderHistory();
    // void on_pushButton_shop_clicked();
    void chat();
    void testclick1();
    void testclick2();
    void order_detail();


    //버튼 연결
    void on_pushButton_clicked();
    void on_membership_clicked();
    void buttonGroup_clicked();
    void on_home_2_clicked();
    void on_pushButton_27_clicked();
    void on_pushButton_menu_clicked();
    void on_pushButton_30_clicked();
    void on_pushButton_31_clicked();
    void on_profile_clicked();
    void on_loginbtn_2_clicked();
    void on_pushButton_39_clicked();
    // void on_pushButton_38_clicked();
    void on_pushButton_40_clicked();

    void on_pushButton_45_clicked();
    void on_pushButton_44_clicked();
    void on_pushButton_43_clicked();
    void on_pushButton_41_clicked();
    void on_pushButton_42_clicked();
    void on_pushButton_38_clicked();
};
#endif // WIDGET_H
