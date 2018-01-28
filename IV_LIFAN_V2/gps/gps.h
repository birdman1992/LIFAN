#ifndef GPS_H
#define GPS_H
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <string.h>


class GpsData
{
public:
       GpsData();
       void processData();


public:
       int  m_strWeek;                       //GPS周
       double m_strTime;                 //GPS秒
       double m_strHeading;           //偏航角
       double m_strPitch;                 //俯仰角
       double m_strRoll;                   //横滚角
       double m_strLatitude;           //纬度
       double m_strLongitude;        //经度
       double m_strAltitude;            //高度
       double m_strVe;                      //东向速度
       double m_strVn;                      // 北向速度
       double m_strVu;                      //天向速度
       double m_strBaseline;            //基线长度
       int  m_strNSV1;                        //天线1卫星数
       int  m_strNSV2;                        //天线2卫星数
       int  m_strStatus;                       //系统状态判断
};




#endif // GPS_H
