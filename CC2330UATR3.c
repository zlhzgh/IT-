#include "segmentdialog.h"
#include "ui_segmentdialog.h"

int m=0;
int ld=0;
int lp=0;

struct segmented{
    int segmentnum;
    int segmentlong;
    int segmentaddr;
    int segmentstate;
};

struct segmented segment[100];

SegmentDialog::SegmentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SegmentDialog)
{
    ui->setupUi(this);
}

SegmentDialog::~SegmentDialog()
{
    delete ui;
}

void SegmentDialog::on_pushButton_clicked()//确定段数
{
    int i;
    QString sm;
    int flag = 0;
    m=0;
    sm = ui->lineEdit->text();
    QByteArray qm = sm.toUtf8();
    char *cm = qm.data();
    int lcm = strlen(cm);
    for(i=0;i<lcm;i++)
    {
        if(cm[i]>='0'&&cm[i]<='9')
        {
            m*=10;
            m+=cm[i]-'0';
        }
        else
        {
            flag = 1;
        }
    }
    if(flag == 1)
    {
        QMessageBox::information(this,tr("提示框"),QObject::tr("无效输入!"),QMessageBox::Ok);
    }
    qDebug()<<"段表数："<<m<<endl;
}

void SegmentDialog::on_pushButton_2_clicked()//设置段表
{
    if(m!=0)
    {
        int i,num=0,flag=0;
        QString message;
        QString ssegment = ui->textEdit->toPlainText();
        QByteArray qsegment = ssegment.toUtf8();
        char *csegment = qsegment.data();
        int lsegment = strlen(csegment);
        qDebug()<<"ssegment:"<<ssegment<<endl;
        qDebug()<<"qsegment"<<qsegment<<endl;
        qDebug()<<"csegment"<<csegment<<endl;
        qDebug()<<"lsegment:"<<lsegment<<endl;
        for(i=0;i<lsegment;i++)
        {
            if(csegment[i]>='0'&&csegment[i]<='9')
            {
                num*=10;
                num+=csegment[i]-'0';
            }
            else
            {
                if(flag%4 == 0)
                {
                    segment[flag/4].segmentnum=num;
                    num=0;
                    flag++;
                }
                else if(flag%4 == 1)
                {
                    segment[flag/4].segmentlong=num;
                    num=0;
                    flag++;
                }
                else if(flag%4 == 2)
                {
                    if(csegment[i]=='-')
                    {
                        segment[flag/4].segmentaddr=-1;
                        num=0;
                        flag++;
                        i++;
                    }
                    else
                    {
                        segment[flag/4].segmentaddr=num;
                        num=0;
                        flag++;
                    }
                }
                else if(flag%4 == 3)
                {
                    segment[flag/4].segmentstate=num;
                    num=0;
                    flag++;
                }
            }
        }
        for(i=0;i<m;i++)
        {
            qDebug()<<"segment:"<<segment[i].segmentnum<<" "<<segment[i].segmentlong<<" "<<segment[i].segmentaddr<<" "<<segment[i].segmentstate<<endl;
        }
        ui->textEdit_2->clear();
        ui->textEdit_2->append("查询当前页表：");
        ui->textEdit_2->append("段号 段长 段基址 状态");
        ui->textEdit_2->textCursor().insertText("\n");
        for(i=0;i<m;i++)
        {
            message = QString::number(segment[i].segmentnum,10);
            ui->textEdit_2->textCursor().insertText(message+"    ");
            message = QString::number(segment[i].segmentlong,10);
            ui->textEdit_2->textCursor().insertText(message+"    ");
            message = QString::number(segment[i].segmentaddr,10);
            ui->textEdit_2->textCursor().insertText(message+"    ");
            message = QString::number(segment[i].segmentstate,10);
            ui->textEdit_2->textCursor().insertText(message+"\n");
        }
    }
    else
    {
        QMessageBox::information(this,tr("提示框"),tr("段表数为0,无法建立段表!"),QMessageBox::Ok);
    }
}

void SegmentDialog::on_pushButton_4_clicked()//获得逻辑地址
{
    QString sl = ui->lineEdit_2->text();
    QByteArray ql = sl.toUtf8();
    char *cl = ql.data();
    int i,num=0;
    int flag = 0;
    int lcl = strlen(cl);
    for(i = 0;i<lcl;i++)
    {
        if(cl[i]>='0'&&cl[i]<='9')
        {
            num*=10;
            num+=cl[i]-'0';
        }
        else
        {
            if(flag == 0)
            {
                ld = num;
                num = 0;
                flag=1;
            }
            else if(flag == 1)
            {
                lp = num;
                num = 0;
                flag=-1;
            }
            //else;
        }
    }
    qDebug()<<"ld:"<<ld<<endl;
    qDebug()<<"lp:"<<lp<<endl;
}

void SegmentDialog::on_pushButton_5_clicked()//计算
{
    if(m!=0)
    {
        int i;
        int im=0;
        int iaddr=0;
        int  flag = 0;
        for(i=0;i<m;i++){
            if(segment[i].segmentnum==ld){
                if(segment[i].segmentstate==1){
                    iaddr = segment[i].segmentaddr;
                    if(lp<=segment[i].segmentlong){//段内地址小于段长
                        im=segment[i].segmentaddr+lp;
                    }
                    else{
                        flag = 1;
                        QMessageBox::information(this,tr("提示框"),tr("段内地址大于段长，该段为非法段,越界中断"),QMessageBox::Ok);
                        break;
                    }
                }
                else{
                    flag = 1;
                    QMessageBox::information(this,tr("提示框"),tr("状态位为0，该段在辅存中，缺段中断"),QMessageBox::Ok);
                    break;
                }
            }
            if(i==m)
            {
                flag=1;
                QMessageBox::information(this,tr("提示框"),QObject::tr("所给页表中查无此项"),QMessageBox::Ok);
            }
        }
        if(flag == 0)
        {
            QString sd = QString::number(ld,10);
            ui->lineEdit_3->setText(sd);
            QString saddr = QString::number(iaddr,10);
            ui->lineEdit_4->setText(saddr);
            QString sp = QString::number(lp,10);
            ui->lineEdit_5->setText(sp);
            QString sm = QString::number(im,10);
            ui->lineEdit_6->setText(sm);
        }
    }
    else
    {
        QMessageBox::information(this,tr("提示框"),tr("尚未建立段表!"),QMessageBox::Ok);
    }
}

void SegmentDialog::on_pushButton_6_clicked()
{
    close();
}

void SegmentDialog::on_pushButton_3_clicked()//文件导入
{
    int i,num=0,flag=0;
    QString message;
    QFile file("segment.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(this,tr("提示框"),tr("文件打开失败"),QMessageBox::Ok);
    }
    QByteArray qsegment = file.readAll();
    qDebug()<<"qpage"<<qsegment<<endl;
    char *csegment = qsegment.data();
    int lsegment = strlen(csegment);
    qDebug()<<"qsegment"<<qsegment<<endl;
    qDebug()<<"csegment"<<csegment<<endl;
    qDebug()<<"lsegment:"<<lsegment<<endl;
    qDebug()<<"lsegment"<<lsegment<<endl;
    for(i=0;i<lsegment;i++)
    {
        if(csegment[i]>='0'&&csegment[i]<='9')
        {
            num*=10;
            num+=csegment[i]-'0';
        }
        else
        {
            if(flag%4 == 0)
            {
                segment[flag/4].segmentnum=num;
                num=0;
                flag++;
            }
            else if(flag%4 == 1)
            {
                segment[flag/4].segmentlong=num;
                num=0;
                flag++;
            }
            else if(flag%4 == 2)
            {
                if(csegment[i]=='-')
                {
                    segment[flag/4].segmentaddr=-1;
                    num=0;
                    flag++;
                    i++;
                }
                else
                {
                    segment[flag/4].segmentaddr=num;
                    num=0;
                    flag++;
                }
            }
            else if(flag%4 == 3)
            {
                segment[flag/4].segmentstate=num;
                num=0;
                flag++;
            }
        }
    }
    m=flag/4;
    for(i=0;i<m;i++)
    {
        qDebug()<<"segment:"<<segment[i].segmentnum<<" "<<segment[i].segmentlong<<" "<<segment[i].segmentaddr<<" "<<segment[i].segmentstate<<endl;
    }
    ui->textEdit_2->clear();
    ui->textEdit_2->append("查询当前页表：");
    ui->textEdit_2->append("段号 段长 段基址 状态");
    ui->textEdit_2->textCursor().insertText("\n");
    for(i=0;i<m;i++)
    {
        message = QString::number(segment[i].segmentnum,10);
        ui->textEdit_2->textCursor().insertText(message+"    ");
        message = QString::number(segment[i].segmentlong,10);
        ui->textEdit_2->textCursor().insertText(message+"    ");
        message = QString::number(segment[i].segmentaddr,10);
        ui->textEdit_2->textCursor().insertText(message+"    ");
        message = QString::number(segment[i].segmentstate,10);
        ui->textEdit_2->textCursor().insertText(message+"\n");
    }
}

void SegmentDialog::on_pushButton_7_clicked()//保存到文件
{
    QString dpage = ui->textEdit->toPlainText();
    qDebug()<<"segment"<<dpage<<endl;
    QFile file("page.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(dpage.toUtf8());
    file.close();
}
