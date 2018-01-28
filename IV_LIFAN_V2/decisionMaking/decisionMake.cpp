#include "includes.h"

using namespace std;

extern double specifiedZone[ZONE_ROW][ZONE_COL];

DecisionMake::DecisionMake()
{
    this->aim_point_sn = 1;
}

void DecisionMake::initVariables()
{
    this->current_lane = CENTER_LANE;
    this->which_lane = CENTER_LANE;

    this->launch_go = false;
    flags.in_zone_county_lane_end = false;
    flags.in_zone_county_lane_start = false;
    flags.in_zone_county_lane_flag = false;

    flags.in_zone_whole_lane_end = false;
    flags.in_zone_whole_lane_end_flag = false;
}


void DecisionMake::initiate(void)
{
    this->re_seek_original_flag  = false;
    this->initiateTrack();
    this->initVariables();
    specifiedZone[0][0] = gps_track[track_row-1][1]; //x_cor
    specifiedZone[0][1] = gps_track[track_row-1][2]; //y_cor
   specifiedZone[0][2] = gps_track[track_row-1][3]; //amzimuth
   specifiedZone[0][3] = 0; //x_cor

}
//double t1 = 0, t2 = 0;


void DecisionMake::initiateTrack(void)
{
        string str;
        ifstream infile;
        vector<double>  values;
        unsigned int i = 0;
        unsigned int size = 0, rowNum = 0, colNum = 0;
        unsigned int row = 0, col = 0;
        unsigned int col_ori =  7;
        infile.open("../IV_LIFAN_V2/gpsObjTrack.txt");      //open file.
        if(infile)
        {
            copy(istream_iterator<double> (infile), istream_iterator<double>(),  back_inserter(values));
            size = values.size();//caculate  nums of 1 dim array
            rowNum = size / col_ori;
            colNum = col_ori ; //3 cols;   1, x, y
            gps_track.resize(rowNum, vector<double>(colNum)); //set the size for 2 dims array

            for(i = 0; i < size ;  i++) //re-arrange datum into 2 dimensions
               {
                    if(col >= col_ori)
                    {
                         row++;
                         col = 0;
                    }
                    gps_track[row][col] = values[i];
                     col++;
               }

               track_row =  rowNum;//gps_track.size();
               track_col = colNum;//gps_track[0].size();
               qDebug() << "gps track is ready!" << endl;
        }
        else
        {
            qDebug() << "not foudn gps track data!";
        }

        qDebug() << gps_track[0][1];
        qDebug() << gps_track[0][2];

        infile.close();//close file
}

void DecisionMake::inputData()
{
   // this->longtitude = 112.8592304; //for test only
   // this->latitude = 28.2174641;

    /*turn current latitude and longitude into x and y coordinate*/
    this->CovertBLToXY(this->latitude, this->longtitude,114, &x_cor, &y_cor, &c_cor);

  /**********for test only******************/
  //      x_cor = 388000.49; //point 159,
  //      y_cor = 3123001.78;
   // uNum = 1483;
    //this->cur_azimuth = 291;
    //x_cor = this->gps_track[uNum - 1][1]; //longitude 388011.84;//
    //y_cor = this->gps_track[uNum - 1][2]; //latitude 3123060.50;//
    /****************************************/
}

unsigned int obst_times = 0;
void DecisionMake::arrangeProg()
{
    this->inputData();
    /*put seekOriPoint() and seekAimPoint() together.*/
    this->seekOriPoint();
    this->seekAimPoint();
    obst_times++;
  //  if((obst_times >= 2))// && (liar4_data_confirm == true))
    {
        obst_times = 0;
         this->checkObstacles();
         this->checkObstacles_l();
         this->checkObstacles_r();
    }
    /****************************************/
    this->manyConditions();
    this->followTrack();
}


