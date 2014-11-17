#include "cpubsub.h"

CPubSub::CPubSub()
{
    com=new SerialPort();
}
CPubSub::~CPubSub()
{
    delete com;
    com=NULL;
}
//===========================================================
// ���ܣ�����Դ
// ���������ں�
//Ĭ�ϲ�������115200,��д��ʽ��!
//===========================================================
 bool CPubSub::OpenCom(const QString name,int index)
{
    this->com->setPort(name);
     int i=115200;
    if(index==0x00)
        i=1200;
    if(index==0x01)
        i=2400;
    if(index==0x02)
        i=4800;
    if(index==0x03)
        i=9600;
    if(index==0x04)
        i=19200;
    if(index==0x05)
        i=38400;
    if(index==0x06)
        i=57600;
    if(index==0x07)
        i=115200;
    if(index==-1)
        return false;

    if (com->open(QIODevice::ReadWrite))
    {
        if (com->setRate(i)
                && com->setDataBits(SerialPort::Data8)
                && com->setParity(SerialPort::NoParity)
                && com->setStopBits(SerialPort::OneStop)
                /*&& m_serialport->setFlowControl(p.flowControl)*/)
        {
            com->setReadBufferSize(4096);
            return true;
        }

    }

    return false;
}
 //================================================
 // ���ܣ���ȡ���п����ݻ��������ֽ���(�Ѵ��ڵ�)
 //================================================
int CPubSub::GetDataLen()
{
    if(!com->isOpen())
        return 0;
    int num=com->bytesAvailable();
    return com->bytesAvailable();
}
//===================================================
// ���ܣ������п�
//===================================================
unsigned int CPubSub::ReadCom(char *pBuff,int nCount)
{
    if(!com->isOpen())
      return 0;
    unsigned int read=0;
    read=com->read(pBuff,nCount);
    return read;
}
//==================================================
// ���ܣ�д���п�
// ������pBuff:���ݻ�����,nCount:Ҫд�����ݵ��ֽ���
//==================================================
bool CPubSub::WriteCom(char *pBuff, int nCount)
{
    if(!com->isOpen())
       return false;
    unsigned int written=0;
    written=com->write(pBuff,nCount);
    if(written==nCount)
        return true;
//    if(com->write(pBuff,nCount))
//        return true;
    return false;
}
//==============================================
// ���ܣ��رմ��п�
//==============================================
bool CPubSub::CloseCom()
{
    if(this->com->isOpen())
       com->close();
    return (!com->isOpen());
}
bool CPubSub::isopen()
{
    return (this->com->isOpen());
}
