#include "includes.h"

/*
decisionMake.auto_manual = remote_ctrl.auto_manual;
decisionMake.launch_stop = remote_ctrl.launch_stop;
decisionMake.urg_brake_flag = remote_ctrl.urg_brake_flag | brake.brk_urg_sts;
*/

void DecisionMake::selectMode()
{
//    this->gear_en = 1;
//    this->gear_value = 14; //D:14, N:12

 //   urg_brake_flag = false;
 //   braking_urg = false;
  //  auto_manual = true;
  //  launch_stop = true;

    if(this->urg_brake_flag == true || this->braking_urg == true)//urg_brake_flag is from lowlayer and braking_urg is from the computer
    {//braking_urg is handled by ECUs, the computer set the flag
        //for clear 0
        this->steering_en = false;
        this->steering_angle = 0;

        this->braking_en = false;
        this->braking_pressure = 0;
        //this->braking_urg = false;

         this->throttle_en = false;
         this->throttle_open = 0;

        this-> left_lamp = false;//turn on the lamp
        this->right_lamp = false;

         this->gear_en = 0;
         this->gear_value = GEAR_N; //D:14, N:12
           //action
         this-> left_lamp = true;//turn on the lamp
         this->right_lamp = true;
         this->launch_go = false;
    }
    else
    {
            if(this->auto_manual == false)//manual mode
            {
                //for clear 0
                this->steering_en = false;
                this->steering_angle = 0;

                this->braking_en = false;
                this->braking_pressure = 0;
                //this->braking_urg = false;

                 this->throttle_en = false;
                 this->throttle_open = 0;

                 this->gear_en = 0;
                 this->gear_value = GEAR_N;

                 this-> left_lamp = false;
                 this->right_lamp = false;

               this->initVariables();
            }
            else if(this->auto_manual == true) //auto mode
            {
                    if(this->launch_stop == false) // stop
                    {
                        this->steering_en = true;
                        this->steering_angle = 0;

                        this->braking_en = true;
                        this->braking_pressure = 1; //MPa
                        //this->braking_urg = false;
                         this->throttle_en = true;
                         this->throttle_open = 0;

                         this->gear_en = true;
                         this->gear_value = GEAR_N;

                          this->initVariables();
                    }
                    else  if(this->launch_stop == true) //start
                    {
                        if((this->lamp_color_m == true) || (decisionMake.key_code ==KEY_GO))
                        {
                                this->launch_go = true;
                        }

                        if(this->launch_go == true)
                        {
                            this->gear_en = true;
                            this->gear_value = GEAR_D;//gear_obj_in_auto;
                            if(this->cur_gear == GEAR_E || this->cur_gear == GEAR_D)
                            {
                                 this->steering_en = true;
                                 this->braking_en = true;
                                 this->throttle_en = true;
                                this->controlSpeed();
                            }
                        }
                        else
                        {
                            this->gear_en = true;
                            this->gear_value = GEAR_N;//gear_obj_in_auto;

                            this->steering_en = true;
                            this->braking_en = true;
                            this->throttle_en = true;
                           this->controlSpeed();
                        }
                     }
             }
    }
}
