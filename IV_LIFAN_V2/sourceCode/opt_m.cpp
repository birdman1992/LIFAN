//
// File: opt_m.cpp
//
// Code generated for Simulink model 'opt_m'.
//
// Model version                  : 1.6
// Simulink Coder version         : 8.9 (R2015b) 13-Aug-2015
// C/C++ source code generated on : Sun Sep  3 07:59:43 2017
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Linux 64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "opt_m.h"
#include "opt_m_private.h"

// Output and update for referenced model: 'opt_m'
void opt_mModelClass::step(const VAR_OUTPUT *arg_VAR_OUTPUT, VAR_ACTUATOR
  *arg_VAR_ACTUATOR)
{
  // BusCreator: '<Root>/BusConversion_InsertedFor_VAR_ACTUATOR_at_inport_0'
  arg_VAR_ACTUATOR->gear_sts = arg_VAR_OUTPUT->gear_sts;
  arg_VAR_ACTUATOR->gear_enable = arg_VAR_OUTPUT->gear_enable;
  arg_VAR_ACTUATOR->brake_pressure = arg_VAR_OUTPUT->brake_pressure;
  arg_VAR_ACTUATOR->brake_enable = arg_VAR_OUTPUT->brake_enable;
  arg_VAR_ACTUATOR->gas_pedal_open = arg_VAR_OUTPUT->gas_pedal_open;
  arg_VAR_ACTUATOR->gas_pedal_enable = arg_VAR_OUTPUT->gas_pedal_enable;
  arg_VAR_ACTUATOR->steering_angle = arg_VAR_OUTPUT->steering_angle;
  arg_VAR_ACTUATOR->steering_angle_enable =
    arg_VAR_OUTPUT->steering_angle_enable;
  arg_VAR_ACTUATOR->left_lamp = arg_VAR_OUTPUT->left_lamp;
  arg_VAR_ACTUATOR->right_lamp = arg_VAR_OUTPUT->right_lamp;
  arg_VAR_ACTUATOR->horn = arg_VAR_OUTPUT->horn;
}

// Model initialize function
void opt_mModelClass::initialize()
{
  // (no initialization code required)
}

// Constructor
opt_mModelClass::opt_mModelClass()
{
}

// Destructor
opt_mModelClass::~opt_mModelClass()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
RT_MODEL_opt_m_T * opt_mModelClass::getRTM()
{
  return (&opt_m_M);
}

// member function to initialize Real-Time model
void opt_mModelClass::initializeRTM()
{
  (void) memset((void *)((&opt_m_M)), 0,
                sizeof(RT_MODEL_opt_m_T));
}

// member function to setup error status pointer
void opt_mModelClass::setErrorStatusPointer(const char_T **rt_errorStatus)
{
  rtmSetErrorStatusPointer((&opt_m_M), rt_errorStatus);
}

//
// File trailer for generated code.
//
// [EOF]
//
