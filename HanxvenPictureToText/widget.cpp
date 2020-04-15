#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QPixmap>
#include <QImage>
#include <QListWidget>
#include <QListWidgetItem>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //ProgramCreatedByHanxven 2020/04/08
    inithis();
}

Widget::~Widget()
{
    delete ui;
}




void Widget::on_btnSet_clicked()
{
    QStringList tmp;

    tmp << ui->char1->text()
        << ui->char2->text()
        << ui->char3->text()
        << ui->char4->text()
        << ui->char5->text()
        << ui->char6->text()
        << ui->char7->text()
        << ui->char8->text()
        << ui->char9->text();

    //检查
    int a = 0;
    for(int i = 0 ; i < 9 ; i++)
    {
        if(tmp.at(i).isEmpty())
        {
            QMessageBox::information(this,"注意",QString::asprintf("设置失败，因为第%d项为空",i+1));
            return;
        }
        if(tmp.at(i).length() !=1)
            a = 1;
    }

    chars = tmp;

    if(a==1)
        QMessageBox::information(this,"注意","Set,但是有点项目不止一个字符");
    else
        QMessageBox::information(this,"Set","Set");



}

void Widget::on_btnAddFiles_clicked()
{
    //选择文件夹
    paths = QFileDialog::getOpenFileNames(this,"打开文件","","图片文件(*.jpg *.png)");

    if(paths.isEmpty())
        return;


    ui->list->addItems(paths);
    ui->lab->setText(QString::asprintf("总计%d文件",ui->list->count()));
    ui->list->setCurrentRow(ui->list->count()-1);

}

void Widget::on_btnDelete_clicked()
{
    if(ui->list->count() == 0)
    {
        QMessageBox::information(this,"提示","啥也没有，删了个寂寞");
        return;
    }
    ui->list->takeItem(ui->list->currentRow());
}

void Widget::on_btnBegin_clicked()
{
    //创建文件夹
    QDir dir;
    if (!dir.exists("result"))
        dir.mkpath("result");


    if(ui->list->count() == 0)
    {
        QMessageBox::information(this,"提示","啥也没有，转了个寂寞");
        return;
    }

    for(int i = 0 ; i < ui->list->count() ; i++)
    {
        convert(ui->list->item(i)->text(),i);
    }


    QMessageBox::information(this,"HanxvenPicText","文件已经保存在目录result下");
}

//初始化主进程
void Widget::inithis()
{
    //初始化对象
    chars << " " << "." << "-" << "*"
          << "+" << "&" << "#" << "%"
          << "@" ;
    //设定前缀
    ui->doubleSpinBox->setPrefix("宽 ");
    ui->doubleSpinBox_2->setPrefix("高 ");
    ui->lab->setText("无项目");
}

void Widget::convert(const QString &path,int count)
{

    QFile file(QString("result/") + QString::asprintf("to_TEXT%d.txt",count));
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this,"警告",path + QString("无法打开"));
        return;
    }



    if(QString::compare("jpg",checkisuffix(path),Qt::CaseInsensitive) && QString::compare("png",checkisuffix(path),Qt::CaseInsensitive))
    {
        file.write("这张“图片”，我可转换不了");
        file.close();
        return;
    }


    pix.load(path);
    pix = pix.scaled(pix.height() * ratiow,pix.height() * ratioh);
    img = pix.toImage();


    //图片去色
    for (int i = 0; i < img.width(); i++)
    {
        for (int j= 0; j < img.height(); j++)
        {
            QRgb color = img.pixel(i, j);
            int gray = qGray(color);
            img.setPixel(i, j, qRgba(gray, gray, gray, qAlpha(color)));
        }
    }



    //转换
    for(int i = 0 ; i < img.height() ;i++)
    {
        for(int j = 0 ;j < img.width() ; j++)
        {
            if(QColor(img.pixel(j,i)).red()>=241 && QColor(img.pixel(j,i)).red()<=255)
            {
                file.write(chars.at(0).toUtf8());

            }
            else if(QColor(img.pixel(j,i)).red()>=211 && QColor(img.pixel(j,i)).red()<=240)
            {
                file.write(chars.at(1).toUtf8());
            }
            else if(QColor(img.pixel(j,i)).red()>=181 && QColor(img.pixel(j,i)).red()<=210)
            {
                file.write(chars.at(2).toUtf8());
            }
            else if(QColor(img.pixel(j,i)).red()>=151 && QColor(img.pixel(j,i)).red()<=180)
            {
                file.write(chars.at(3).toUtf8());
            }
            else if(QColor(img.pixel(j,i)).red()>=121 && QColor(img.pixel(j,i)).red()<=150)
            {
                file.write(chars.at(4).toUtf8());
            }
            else if(QColor(img.pixel(j,i)).red()>=91 && QColor(img.pixel(j,i)).red()<=120)
            {
                file.write(chars.at(5).toUtf8());
            }
            else if(QColor(img.pixel(j,i)).red()>=61 && QColor(img.pixel(j,i)).red()<=90)
            {
                file.write(chars.at(6).toUtf8());
            }
            else if(QColor(img.pixel(j,i)).red()>=31 && QColor(img.pixel(j,i)).red()<=60)
            {
                file.write(chars.at(7).toUtf8());
            }
            else if(QColor(img.pixel(j,i)).red()>=0 && QColor(img.pixel(j,i)).red()<=30)
            {
                file.write(chars.at(8).toUtf8());
            }
        }
        file.write("\n");
    }
    file.close();
}


//宽,高比例
void Widget::on_doubleSpinBox_valueChanged(double arg1)
{
    ratiow = arg1;
}

void Widget::on_doubleSpinBox_2_valueChanged(double arg1)
{
    ratioh = arg1;
}





void Widget::on_list_currentRowChanged(int currentRow)
{
    ui->lab->setText(QString::asprintf("文件列表:第%d/%d个文件",currentRow,ui->list->count()));
}

QString Widget::checkisuffix(const QString &str)
{
    //返回扩展名
    int i = str.length();
    int j = str.lastIndexOf(".");
    return str.right(i-j-1);
}

