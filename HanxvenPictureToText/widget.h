#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileSystemModel>
#include <QPixmap>
#include <QImage>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:


    void on_btnSet_clicked();

    void on_btnAddFiles_clicked();

    void on_btnDelete_clicked();

    void on_btnBegin_clicked();

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_doubleSpinBox_2_valueChanged(double arg1);



    void on_list_currentRowChanged(int currentRow);



private:
    Ui::Widget *ui;
    double ratiow = 1.0;//比例的修改
    double ratioh = 1.0;//比例的修改
    QStringList chars;//字符的设定
    void convert(const QString &path,int count);
    void inithis();//初始化的
    QString checkisuffix(const QString &str);
    QPixmap pix;//位图
    QImage img;//图片修改
    QStringList paths;//地址链接

};
#endif // WIDGET_H
