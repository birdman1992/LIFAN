#include "includes.h"
#include"gps.h"
#include<QObject>

//ls -l /dev/ttyS0
QSerialPort *serial;

void InitGPS(void)
{
;
}

GpsData::GpsData()
{
;
}
int first=1;
void GpsData::processData()
{
    if(first)
    {
        first=0;
        QString serialname, serialnameTemp;
/*
        foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
             qDebug() << "Name : " << info.portName();
            QSerialPort serial_temp;
            serial_temp.setPort(info);
           if(serial_temp.open(QIODevice::ReadWrite))
            {
                 serialname=serial_temp.portName();
            }
        }*/
       serialname = "ttyUSB0"; //ttyS4

        serial = new QSerialPort;
        //设置串口名
         serial->setPortName(serialname);
        //打开串口
        serial->open(QIODevice::ReadWrite);
        //设置波特率
        serial->setBaudRate(115200);
        //设置数据位数
         serial->setDataBits(QSerialPort::Data8);
        //设置奇偶校验
        serial->setParity(QSerialPort::NoParity);
        //设置停止位
        serial->setStopBits(QSerialPort::OneStop);
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl);
        //%读取接收到的数据
        const QByteArray first_order= "$cmd,output,com0,gpfpd,0.01*ff";
        serial->write(first_order);
    }

    QString frame;
    QByteArray buf;
    QString data;

    if(serial->isOpen())
    {
        serial->waitForReadyRead(20);
        buf = serial->readAll();
        if(!buf.isEmpty())
        {
                while(!data.length())
                {
                   serial->waitForReadyRead(20);
                   buf = serial->readAll();
                  // qDebug()<<buf;
                    if(!buf.isEmpty())
                    {
                        QString str;
                        str+=buf;
                        // qDebug()<<str;
                         int i=0;
                         int end=0;
                         if(str.indexOf("$")!=(-1))
                              i=str.indexOf("$");
                         else
                              continue;
                          str=str.mid(i);

                          if(str.indexOf("\n")!=(-1))
                          {
                              end=str.indexOf("\n");
                              data=str.mid(0,end);
                          }
                          else
                          {
                              while(str.indexOf("\n")==(-1))
                              {
                              serial->waitForReadyRead(20);
                              buf = serial->readAll();
                              str+=buf;
                              }
                              end=str.indexOf("\n");
                              data=str.mid(0,end);
                          }//if(str.indexOf("\n")!=(-1))..else
                     }// if(!buf.isEmpty())
            }//while(!data.length())
    }//if(!buf.isEmpty())
    else
    {
           this->m_strStatus  = 0;
    }
    frame = data;
   qDebug()<<frame;

    QString str[16];
    //frame为实际获取的字符串
    //qDebug()<<frame.length();
     if(frame.length()>23&&frame.startsWith("$GPFPD"))
     {
        // qDebug()<<"m";
          int comma[16]={};
         //得到指定序号以后的逗号位置
         for(int i=1;i<16;i++)
         {
                 comma[0]=frame.indexOf(",");
                 //qDebug()<<comma[i-1];
                 comma[i]=frame.indexOf(",",comma[i-1]+1);
                 if(comma[i]!=comma[i-1]+1)
                 {
                     str[i]=frame.mid(comma[i-1]+1,comma[i]-comma[i-1]-1);
                 }
                 //对各项数据分别处理
                switch (i)
                {
                    case 1:                 //GPS周(wwww)
                        this->m_strWeek = str[1].toInt();
                        break;
                    case 2:                               //GPS秒(ssssss.sss)
                        this->m_strTime = str[2].toDouble() ;
                        break;
                      case 3:                                 //偏航角(hhh.hh)
                        this->m_strHeading = str[3].toDouble();
                         break;
                      case 4:                               //俯仰角(+/-pp.pp)
                        this->m_strPitch = str[4].toDouble();
                        break;
                      case 5:                              //横滚角(+/-rrr.rr)
                        this->m_strRoll = str[5].toDouble();
                        break;
                      case 6:         //纬度(+/-ll.lllllll)(‘+’-北纬，‘-’-南纬)
                        this->m_strLatitude = str[6].toDouble();
                        break;
                      case 7:         //经度(+/-lll.lllllll)(‘+’-东经，‘-’-西经)
                        this->m_strLongitude = str[7].toDouble();
                        break;
                      case 8:                      //高度(+/-aaaaa.aa)  单位米
                        this->m_strAltitude = str[8].toDouble();
                        break;
                      case 9:                //东向速度(+/-eee.eee) 单位：米/秒
                        this->m_strVe = str[9].toDouble();
                        break;
                    case 10:               //北向速度(+/-nnn.nnn) 单位：米/秒
                         this->m_strVn = str[10].toDouble();
                         break;
                    case 11:               //天向速度(+/-uuu.uuu) 单位：米/秒
                         this->m_strVu = str[11].toDouble();
                         break;
                    case 12:                       //基线长度(bb.bbb) 单位：米
                         this->m_strBaseline = str[12].toDouble();
                         break;
                    case 13:                                //天线1卫星数(nn)
                          this->m_strNSV1 = str[13].toInt();
                          break;
                    case 14:                               //天线2卫星数(nn)
                          this->m_strNSV2 = str[14].toInt();
                          break;
                    case 15:
                {
                    QString temp;
                    temp=str[15].mid(0,2);
                    this->m_strStatus = temp.toInt();
                    if(this->m_strStatus ==0)
                    {
                        if(!(temp.compare("0A")))
                            this->m_strStatus=10;
                        else if(!(temp.compare("0B")))
                            this->m_strStatus=11;
                        else if(!(temp.compare("0C")))
                            this->m_strStatus=12;
                        else
                            this->m_strStatus=0;
                    }
                }
                    default:
                        break;
                } //switch (i)
          }//for(int i=1;i<15;i++)
     }// if(frame.length()>23&&frame.startsWith("$GPFPD"))
     frame.clear();
    }
    else //if  if(serial->isOpen())
    {
        this->m_strStatus = 0;
    }

    if((this->m_strStatus  == 0x05) || (this->m_strStatus  == 0x0B) || (this->m_strStatus  == 0x08)) //05,0b, difference, 08:immu
    {
        flags. gps_err = false;
    }
    else
    {
        flags. gps_err = true;
    }
}//end of function
