#include "dlgphaseparm.h"
#include "ui_dlgphaseparm.h"
#include "unit.h"
#include <QDebug>
DlgPhaseParm::DlgPhaseParm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgPhaseParm)
{
    ui->setupUi(this);
    this->com=new CPubSub();
    this->revtimer=new QTimer();
    this->timeout=new QTimer();
    connect(timeout,SIGNAL(timeout()),this,SLOT(warming()));

    ui->spinClearTime->clear();
    ui->spinCrossmsec->clear();
    ui->spinGreenFlash->clear();
    ui->spinGreenLaterTime->clear();
    ui->spinGreenTime->clear();

    ui->spinMaxTime->clear();
    ui->spinMaxTime2->clear();
    ui->spinMinGreenTime->clear();
    ui->radioFixed->setChecked(false);
    ui->radioKey->setChecked(false);
    ui->radioPlay->setChecked(false);
    ui->radioWait->setChecked(false);

    ui->check0->setChecked(false);
    ui->check1->setChecked(false);
    ui->check2->setChecked(false);
    ui->check3->setChecked(false);
    ui->check4->setChecked(false);
    ui->radionull->setChecked(true);
    ui->radiokong->setChecked(true);
    ui->radiokongong->setChecked(true);
    connect(revtimer,SIGNAL(timeout()),this,SLOT(rev()));
}

DlgPhaseParm::~DlgPhaseParm()
{
    delete ui;
    deleteLater();
}
void DlgPhaseParm::setcom(CPubSub *com)
{
    this->com=com;
}
void DlgPhaseParm::dealwith(uchar* rec,int len)
{
    uchar* s =rec;
    for(int i=0; i<len; i++)
    {
        if((*s) != 0x02)
        {
            s++;
            continue;
        }
        uchar* s2 = s;
        unsigned short hight=(*(s2+1))<<8;
        unsigned short low=*(s2+2);
        unsigned short num=hight+low;
        if(i+(num+5)>len)
            return;

        unsigned short crchight=(*(s2+(num+3)))<<8;
        unsigned short crclow=(*(s2+(num+4)));
        unsigned short crcnum=crchight+crclow;

        char buffer[]={0x02,0x00,0x05 ,0x84 ,0x83 ,0x00 ,0x00 ,0x00};
        unsigned short wewewew=CommVerify((uchar*)buffer,8);
        if(crcnum!=CommVerify((uchar*)s,num+3))
        {
            s++;
            qDebug()<<"chucuo!!!!!jiaoyan!!!";
            continue;
        }
        if((uchar)*(s+3)==0x84)//��ѯ�����
        {
            if((uchar)(*(s+4))==0x95)
            {
                init95((uchar*)s);
                ui->display->setText(tr("��ѯ�ɹ�!"));
                timeout->stop();
            }
            if((uchar)(*(s+4))==0x79)
            {
                ui->hang->setText(QString::number(*(s+6),10));
                ui->display->setText(tr("��ѯ�����гɹ�!"));
                timeout->stop();
            }
        }
        if((uchar)*(s+3)==0x85)//���óɹ�����
        {
            if((uchar)(*(s+4))==0x95)
            {
                ui->display->setText(tr("���óɹ�!"));
                timeout->stop();
            }
            if((uchar)(*(s+4))==0x79)
            {
                ui->display->setText(tr("��ճɹ�!"));
                timeout->stop();
            }
        }
        if((uchar)*(s+3)==0x86)//���ô�������
        {
            if((uchar)(*(s+4))==0x95)
            {
                ui->display->setText(tr("�޸�ʧ��!"));
            }
            if((uchar)(*(s+4))==0x79)
            {
                ui->display->setText(tr("�޸�ʧ��!"));
            }
        }
        s +=(num+5);
        i += (num+4);
    }
}
void DlgPhaseParm::init95(uchar *buffer)
{
    unsigned char parm1=*(buffer+7);//��λ��
    unsigned char parm2=*(buffer+8);//���˹����̵�
    unsigned char parm3=*(buffer+9);//�������ʱ��
    unsigned char parm4=*(buffer+10);//��С�̵�ʱ��
    unsigned char parm5=*(buffer+11);//��λ�̵��ӳ�ʱ��
    unsigned char parm6=*(buffer+12);//����̵�ʱ��1
    unsigned char parm7=*(buffer+13);//����̵�ʱ��2
    unsigned char parm8=*(buffer+14);//������λ�̶��̵�ʱ��
    unsigned char parm9=*(buffer+15);//����ʱ��

    unsigned char parm10=*(buffer+16);//��λ����
    unsigned char parm11=*(buffer+17);//��λѡ���
    unsigned char parm12=*(buffer+18);//��չ�ֶ�
   ///////////1///////////////////////
    ui->spinNumber->setValue(parm1);
    if(parm2!=0xff)
    ui->spinCrossmsec->setValue(parm2);
    if(parm3!=0xff)
    ui->spinClearTime->setValue(parm3);
    if(parm4!=0xff)
    ui->spinMinGreenTime->setValue(parm4);
    if(parm5!=0xff)
    ui->spinGreenLaterTime->setValue(parm5);
    if(parm6!=0xff)
    ui->spinMaxTime->setValue(parm6);
    if(parm7!=0xff)
    ui->spinMaxTime2->setValue(parm7);
    if(parm8!=0xff)
    ui->spinGreenTime->setValue(parm8);
    if(parm9!=0xff)
    ui->spinGreenFlash->setValue(parm9);


    if(parm10!=0xff)
    {
        if((parm10&0x80)==0x80)
            ui->radioFixed->setChecked(true);
        if((parm10&0x40)==0x40)
            ui->radioWait->setChecked(true);
        if((parm10&0x20)==0x20)
            ui->radioPlay->setChecked(true);
        if((parm10&0x10)==0x10)
            ui->radioKey->setChecked(true);
    }
    if(parm11!=0xff)
    {
        if((parm11&0x01)==0x01)
            ui->check0->setChecked(true);
        if((parm11&0x02)==0x02)
            ui->check1->setChecked(true);
        if((parm11&0x04)==0x04)
            ui->check2->setChecked(true);
        if((parm11&0x08)==0x08)
            ui->check3->setChecked(true);
        if((parm11&0x10)==0x10)
            ui->check4->setChecked(true);
    }
    if(parm12!=0xff)
    {
        unsigned char value1=0x00;
        unsigned char value2=0x00;
        value2=(parm12>>4);
        value1=(parm12&0x0f);
        qDebug()<<"value1:"+QString::number(value1,10);
        if(value1==0x01)
            ui->radioyuandeng->setChecked(true);
        if(value1==0x02)
            ui->radiozhixingdeng->setChecked(true);
        if(value1==0x03)
            ui->radiozuozhuan->setChecked(true);
        if(value1==0x04)
            ui->radioyouzhuan->setChecked(true);
        if(value1==0x05)
            ui->radiodiaotou->setChecked(true);

        value2=(parm12>>4);
        if((value2==0x01))
            ui->radiodong->setChecked(true);
        if((value2==0x03))
            ui->radioxi->setChecked(true);
        if((value2==0x02))
            ui->radionan->setChecked(true);
        if((value2==0x04))
            ui->radiobei->setChecked(true);
    }
}
void DlgPhaseParm::rev()
{
    char RecBuf[4096];
    memset(RecBuf,NULL,4096);
    QString RecText;
    int Rlen;
    if(com->isopen())
    {
        Rlen=com->GetDataLen();
        //qDebug()<<"here!here!hereherehereherehereherehereherehere";
        if(Rlen>0)
        {
            com->ReadCom(RecBuf,Rlen);
            for(int i=0;i<Rlen;i++)
            {

                    RecText.append(QString::number((uchar)RecBuf[i],16));
                    RecText.append("|");
            }
        }
        dealwith((uchar*)RecBuf,Rlen);
    }
}
void DlgPhaseParm::warming()
{
    ui->display->setText(tr("ʧ��!������!"));
}
void DlgPhaseParm::send95()
{
    char buffer[60];
    buffer[0]=0x02;
    buffer[1]=0x00;
    buffer[2]=0x04;
    buffer[3]=0x80;
    buffer[4]=0x95;
    buffer[5]=0x40;
    buffer[6]=(unsigned char )ui->spinNumber->value();
    int len=7;
    unsigned short rssum=CommVerify((unsigned char *)buffer,len);
    buffer[len]=rssum/256;
    buffer[len+1]=rssum%256;
    this->com->WriteCom(buffer,len+2);
}
void DlgPhaseParm::set95()
{
    unsigned char parm[13]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    parm[1]=ui->spinNumber->value();
    parm[2]=ui->spinCrossmsec->value();
    parm[3]=ui->spinClearTime->value();
    parm[4]=ui->spinMinGreenTime->value();
    parm[5]=ui->spinGreenLaterTime->value();
    parm[6]=ui->spinMaxTime->value();
    parm[7]=ui->spinMaxTime2->value();
    parm[8]=ui->spinGreenTime->value();
    parm[9]=ui->spinGreenFlash->value();

     if((ui->radioFixed->isChecked())|(ui->radioWait->isChecked())|
       (ui->radioPlay->isChecked())|(ui->radioKey->isChecked()))
     {
        if(ui->radioFixed->isChecked())
        parm[10]=parm[10]|0x80;
        if(ui->radioWait->isChecked())
        parm[10]=parm[10]|0x40;
        if(ui->radioPlay->isChecked())
        parm[10]=parm[10]|0x20;
        if(ui->radioKey->isChecked())
        parm[10]=parm[10]|0x10;
    }
     else
    {
        parm[10]=0xff;
    }

    if((ui->check0->isChecked())|(ui->check1->isChecked())|
        (ui->check2->isChecked())|(ui->check3->isChecked())|
        (ui->check4->isChecked()))
    {
        if(ui->check0->isChecked())
        parm[11]=parm[11]|0x01;
        if(ui->check1->isChecked())
        parm[11]=parm[11]|0x02;
        if(ui->check2->isChecked())
        parm[11]=parm[11]|0x04;
        if(ui->check3->isChecked())
        parm[11]=parm[11]|0x08;
        if(ui->check4->isChecked())
        parm[11]=parm[11]|0x10;
    }else
    {
        parm[11]=0xff;
    }

    if(ui->radiodong->isChecked()|
            ui->radioxi->isChecked()|
            ui->radionan->isChecked()|
            ui->radiobei->isChecked()|
            ui->radioyuandeng->isChecked()|
            ui->radiozhixingdeng->isChecked()|
            ui->radiozuozhuan->isChecked()|
            ui->radioyouzhuan->isChecked()|
            ui->radiodiaotou->isChecked())
     {
         unsigned char value1=0x00;
         if(ui->radiodong->isChecked())
             value1=0x01;
         if(ui->radioxi->isChecked())
             value1=0x03;
         if(ui->radionan->isChecked())
             value1=0x02;
         if(ui->radiobei->isChecked())
             value1=0x04;
         parm[12]=parm[12]|(value1<<4);
         qDebug()<<"parm[12]--value1:"+QString::number(parm[12],10);
         unsigned char value2=0x00;
         if(ui->radioyuandeng->isChecked())
             value2=0x01;
         if(ui->radiozhixingdeng->isChecked())
             value2=0x02;
         if(ui->radiozuozhuan->isChecked())
             value2=0x03;
         if(ui->radioyouzhuan->isChecked())
             value2=0x04;
         if(ui->radiodiaotou->isChecked())
             value2=0x05;
         qDebug()<<"parm[12]--value2:"+QString::number(parm[12],10);
         parm[12]=parm[12]|value2;
     }else
    {
        parm[12]=0xff;
    }
    char buf[60];
    buf[0]=0x02;
    buf[1]=0x00;
    buf[2]=0x10;
    buf[3]=0x81;
    buf[4]=0x95;
    buf[5]=0x40;
    buf[6]=parm[1];
    buf[7]=parm[1];

    buf[8]=parm[2];//����
    buf[9]=parm[3];//���
    buf[10]=parm[4];//��С�̵�
    buf[11]=parm[5];//��λ�̵�
    buf[12]=parm[6];//����̵�1

    buf[13]=parm[7];//����̵�2
    buf[14]=parm[8];
    buf[15]=parm[9];//����
    buf[16]=parm[10];//��λ����
    buf[17]=parm[11];//��λѡ��

    buf[18]=0xff;//parm[12];//���Ӳ���
    int len=19;
    int i=0;
    for (i;i<20;i++)
      qDebug()<<"buf+"+QString::number(i,10)+":"+QString::number(buf[i],16);
    unsigned short rssum=CommVerify((unsigned char *)buf,len);
    buf[len]=rssum/256;
    buf[len+1]=rssum%256;
    this->com->WriteCom(buf,len+2);
    qDebug()<<"shezhi haole ";
}

void DlgPhaseParm::on_butfind_clicked()
{
    ui->display->setText(tr("��ѯ��......"));
    ui->spinClearTime->clear();
    ui->spinCrossmsec->clear();
    ui->spinGreenFlash->clear();
    ui->spinGreenLaterTime->clear();
    ui->spinGreenTime->clear();

    ui->spinMaxTime->clear();
    ui->spinMaxTime2->clear();
    ui->spinMinGreenTime->clear();
    ui->radioFixed->setChecked(false);
    ui->radioKey->setChecked(false);
    ui->radioPlay->setChecked(false);
    ui->radioWait->setChecked(false);

    ui->check0->setChecked(false);
    ui->check1->setChecked(false);
    ui->check2->setChecked(false);
    ui->check3->setChecked(false);
    ui->check4->setChecked(false);
    ui->radionull->setChecked(true);
    ui->radiokong->setChecked(true);
    ui->radiokongong->setChecked(true);
    QTimer::singleShot(1,this,SLOT(send95()));
    revtimer->start(20);
    timeout->start(1000);
}

void DlgPhaseParm::on_butok_clicked()
{
    ui->display->clear();
    set95();
    revtimer->start(20);
    timeout->start(1000);
}

void DlgPhaseParm::on_butclear_clicked()
{
    ui->display->clear();
    unsigned char parm[13]={0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
    parm[1]=ui->spinNumber->value();
    char buf[60];
    buf[0]=0x02;
    buf[1]=0x00;
    buf[2]=0x10;
    buf[3]=0x81;
    buf[4]=0x95;
    buf[5]=0x40;
    buf[6]=parm[1];
    buf[7]=parm[1];

    buf[8]=parm[2];//����
    buf[9]=parm[3];//���
    buf[10]=parm[4];//��С�̵�
    buf[11]=parm[5];//��λ�̵�
    buf[12]=parm[6];//����̵�1

    buf[13]=parm[7];//����̵�2
    buf[14]=parm[8];
    buf[15]=parm[9];//����
    buf[16]=parm[10];//��λ����
    buf[17]=parm[11];//��λѡ��

    buf[18]=parm[12];//���Ӳ���
    int len=19;
    unsigned short rssum=CommVerify((unsigned char *)buf,len);
    buf[len]=rssum/256;
    buf[len+1]=rssum%256;
    this->com->WriteCom(buf,len+2);
}

void DlgPhaseParm::on_butquery_clicked()
{
    ui->display->setText(tr("��ѯ��......"));
    ui->hang->clear();
    char buffer[60];
    buffer[0]=0x02;
    buffer[1]=0x00;
    buffer[2]=0x04;
    buffer[3]=0x80;
    buffer[4]=0x79;
    buffer[5]=0x00;
    buffer[6]=0x95;
    int len=7;
    unsigned short rssum=CommVerify((unsigned char *)buffer,len);
    buffer[len]=rssum/256;
    buffer[len+1]=rssum%256;
    this->com->WriteCom(buffer,len+2);
    revtimer->start(20);
    timeout->start(1000);
}

void DlgPhaseParm::on_butclearall_clicked()
{
    ui->display->clear();
    char buffer[60];
    buffer[0]=0x02;
    buffer[1]=0x00;
    buffer[2]=0x04;
    buffer[3]=0x81;
    buffer[4]=0x79;
    buffer[5]=0x00;
    buffer[6]=0x95;
    int len=7;
    unsigned short rssum=CommVerify((unsigned char *)buffer,len);
    buffer[len]=rssum/256;
    buffer[len+1]=rssum%256;
    this->com->WriteCom(buffer,len+2);
    revtimer->start(20);
    timeout->start(1000);
}
