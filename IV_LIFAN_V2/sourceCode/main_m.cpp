//
// File: main_m.cpp
//
// Code generated for Simulink model 'main_m'.
//
// Model version                  : 1.32
// Simulink Coder version         : 8.9 (R2015b) 13-Aug-2015
// C/C++ source code generated on : Sun Sep  3 07:59:46 2017
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Linux 64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "main_m.h"
#include "main_m_private.h"
#include "app_m.h"
#include "input_m.h"

// model instance variable for '<Root>/APPLICATION'
output_mModelClass APPLICATIONMDLOBJ1;

// model instance variable for '<Root>/INPUT'
input_mModelClass INPUTMDLOBJ2;

// Model step function
void main_mModelClass::step(const VAR_PLAT *arg_VAR_PLAT, VAR_OUTPUT
  *arg_VAR_OUTPUT)
{
  // local block i/o variables
  VAR_PLAT rtb_BusConversion_InsertedFor_INPUT_at_inport_0_BusCreator1;
  VAR_INPUT rtb_INPUT;
  VAR_INPUT rtb_BusConversion_InsertedFor_APPLICATION_at_inport_0_BusCreator1;
  VAR_OUTPUT rtb_APPLICATION;
  VAR_ACTUATOR rtb_OUTPUT;
  VAR_OUTPUT rtb_BusConversion_InsertedFor_VAR_OUTPUT_at_inport_0_BusCreator1;

  // BusCreator: '<Root>/BusConversion_InsertedFor_INPUT_at_inport_0' incorporates:
  //   Inport: '<Root>/VAR_PLAT'

  rtb_BusConversion_InsertedFor_INPUT_at_inport_0_BusCreator1 = *arg_VAR_PLAT;

  // ModelReference: '<Root>/INPUT'
  INPUTMDLOBJ2.step(&rtb_BusConversion_InsertedFor_INPUT_at_inport_0_BusCreator1,
                    &rtb_INPUT);

  // BusCreator: '<Root>/BusConversion_InsertedFor_APPLICATION_at_inport_0'
  rtb_BusConversion_InsertedFor_APPLICATION_at_inport_0_BusCreator1 = rtb_INPUT;

  // ModelReference: '<Root>/APPLICATION'
  APPLICATIONMDLOBJ1.step
    (&rtb_BusConversion_InsertedFor_APPLICATION_at_inport_0_BusCreator1,
     &rtb_APPLICATION);

  // BusCreator: '<Root>/BusConversion_InsertedFor_OUTPUT_at_inport_0'
  rtb_BusConversion_InsertedFor_VAR_OUTPUT_at_inport_0_BusCreator1 =
    rtb_APPLICATION;

  // ModelReference: '<Root>/OUTPUT'
  OUTPUTMDLOBJ3.step
    (&rtb_BusConversion_InsertedFor_VAR_OUTPUT_at_inport_0_BusCreator1,
     &rtb_OUTPUT);

  // BusCreator: '<Root>/BusConversion_InsertedFor_VAR_OUTPUT_at_inport_0'
  rtb_BusConversion_InsertedFor_VAR_OUTPUT_at_inport_0_BusCreator1.gear_sts =
    rtb_OUTPUT.gear_sts;
  rtb_BusConversion_InsertedFor_VAR_OUTPUT_at_inport_0_BusCreator1.gear_enable =
    rtb_OUTPUT.gear_enable;
  rtb_BusConversion_InsertedFor_VAR_OUTPUT_at_inport_0_BusCreator1.brake_pressure
    = rtb_OUTPUT.brake_pressure;
  rtb_BusConversion_InsertedFor_VAR_OUTPUT_at_inport_0_BusCreator1.brake_enable =
    rtb_OUTPUT.brake_enable;
  rtb_BusConversion_InsertedFor_VAR_OUTPUT_at_inport_0_BusCreator1.gas_pedal_open
    = rtb_OUTPUT.gas_pedal_open;
  rtb_BusConversion_InsertedFor_VAR_OUTPUT_at_inport_0_BusCreator1.gas_pedal_enable
    = rtb_OUTPUT.gas_pedal_enable;
  rtb_BusConversion_InsertedFor_VAR_OUTPUT_at_inport_0_BusCreator1.steering_angle
    = rtb_OUTPUT.steering_angle;
  rtb_BusConversion_InsertedFor_VAR_OUTPUT_at_inport_0_BusCreator1.steering_angle_enable
    = rtb_OUTPUT.steering_angle_enable;
  rtb_BusConversion_InsertedFor_VAR_OUTPUT_at_inport_0_BusCreator1.left_lamp =
    rtb_OUTPUT.left_lamp;
  rtb_BusConversion_InsertedFor_VAR_OUTPUT_at_inport_0_BusCreator1.right_lamp =
    rtb_OUTPUT.right_lamp;
  rtb_BusConversion_InsertedFor_VAR_OUTPUT_at_inport_0_BusCreator1.horn =
    rtb_OUTPUT.horn;

  // Outport: '<Root>/VAR_OUTPUT'
  *arg_VAR_OUTPUT =
    rtb_BusConversion_InsertedFor_VAR_OUTPUT_at_inport_0_BusCreator1;
}

// Model initialize function
void main_mModelClass::initialize()
{
  // Registration code

  // initialize error status
  rtmSetErrorStatus((&main_m_M), (NULL));

  // Model Initialize fcn for ModelReference Block: '<Root>/APPLICATION'

  // initialize real-time model for ModelReference Block: '<Root>/APPLICATION'
  APPLICATIONMDLOBJ1.initializeRTM();

  // Set error status pointer for ModelReference Block: '<Root>/APPLICATION'
  APPLICATIONMDLOBJ1.setErrorStatusPointer(rtmGetErrorStatusPointer((&main_m_M)));
  APPLICATIONMDLOBJ1.initialize();

  // Model Initialize fcn for ModelReference Block: '<Root>/INPUT'

  // initialize real-time model for ModelReference Block: '<Root>/INPUT'
  INPUTMDLOBJ2.initializeRTM();

  // Set error status pointer for ModelReference Block: '<Root>/INPUT'
  INPUTMDLOBJ2.setErrorStatusPointer(rtmGetErrorStatusPointer((&main_m_M)));
  INPUTMDLOBJ2.initialize();

  // Model Initialize fcn for ModelReference Block: '<Root>/OUTPUT'

  // initialize real-time model for ModelReference Block: '<Root>/OUTPUT'
  OUTPUTMDLOBJ3.initializeRTM();

  // Set error status pointer for ModelReference Block: '<Root>/OUTPUT'
  OUTPUTMDLOBJ3.setErrorStatusPointer(rtmGetErrorStatusPointer((&main_m_M)));
  OUTPUTMDLOBJ3.initialize();
}

// Model terminate function
void main_mModelClass::terminate()
{
  // (no terminate code required)
}

// Constructor
main_mModelClass::main_mModelClass()
{
}

// Destructor
main_mModelClass::~main_mModelClass()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
RT_MODEL_main_m_T * main_mModelClass::getRTM()
{
  return (&main_m_M);
}

//
// File trailer for generated code.
//
// [EOF]
//
