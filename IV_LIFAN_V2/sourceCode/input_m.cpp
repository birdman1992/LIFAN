//
// File: input_m.cpp
//
// Code generated for Simulink model 'input_m'.
//
// Model version                  : 1.33
// Simulink Coder version         : 8.9 (R2015b) 13-Aug-2015
// C/C++ source code generated on : Sun Sep  3 07:59:41 2017
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Linux 64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "input_m.h"
#include "input_m_private.h"

// Output and update for referenced model: 'input_m'
void input_mModelClass::step(const VAR_PLAT *arg_VAR_PLAT, VAR_INPUT
  *arg_VAR_INPUT)
{
  // BusCreator: '<Root>/BusConversion_InsertedFor_VAR_INPUT_at_inport_0'
  arg_VAR_INPUT->longitude = arg_VAR_PLAT->longitude;
  arg_VAR_INPUT->latitude = arg_VAR_PLAT->latitude;
  arg_VAR_INPUT->center_point_x = arg_VAR_PLAT->center_point_x;
  arg_VAR_INPUT->center_point_y = arg_VAR_PLAT->center_point_y;
  arg_VAR_INPUT->obj_size_x = arg_VAR_PLAT->obj_size_x;
  arg_VAR_INPUT->obj_size_y = arg_VAR_PLAT->obj_size_y;
  arg_VAR_INPUT->obj_kind = arg_VAR_PLAT->obj_kind;
  arg_VAR_INPUT->auto_manual = arg_VAR_PLAT->auto_manual;
  arg_VAR_INPUT->launch_stop = arg_VAR_PLAT->launch_stop;
  arg_VAR_INPUT->vehicle_speed = arg_VAR_PLAT->vehicle_speed;
}

// Model initialize function
void input_mModelClass::initialize()
{
  // (no initialization code required)
}

// Constructor
input_mModelClass::input_mModelClass()
{
}

// Destructor
input_mModelClass::~input_mModelClass()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
RT_MODEL_input_m_T * input_mModelClass::getRTM()
{
  return (&input_m_M);
}

// member function to initialize Real-Time model
void input_mModelClass::initializeRTM()
{
  (void) memset((void *)((&input_m_M)), 0,
                sizeof(RT_MODEL_input_m_T));
}

// member function to setup error status pointer
void input_mModelClass::setErrorStatusPointer(const char_T **rt_errorStatus)
{
  rtmSetErrorStatusPointer((&input_m_M), rt_errorStatus);
}

//
// File trailer for generated code.
//
// [EOF]
//
