//
// File: app_m.cpp
//
// Code generated for Simulink model 'app_m'.
//
// Model version                  : 1.120
// Simulink Coder version         : 8.9 (R2015b) 13-Aug-2015
// C/C++ source code generated on : Sun Sep  3 07:59:35 2017
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Linux 64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "app_m.h"
#include "app_m_private.h"

// Exported block states
real32_T steeringAngle;                // '<Root>/Data Store Memory'

// Output and update for referenced model: 'app_m'
void output_mModelClass::step(const VAR_INPUT *arg_VAR_INPUT, VAR_OUTPUT
  *arg_VAR_OUTPUT)
{
  uint8_T rtb_Merge;
  uint8_T rtb_Merge1;
  real32_T rtb_Merge2;
  uint8_T rtb_Merge3;
  int32_T rtb_Merge4;
  uint8_T rtb_Merge5;
  int32_T rtb_Merge6;
  uint8_T rtb_Merge7;

  // If: '<Root>/If' incorporates:
  //   Constant: '<S12>/Constant2'
  //   Constant: '<S12>/Constant3'
  //   Constant: '<S12>/steering_enable_c'
  //   Constant: '<S13>/Constant'
  //   Constant: '<S14>/Constant'
  //   Constant: '<S1>/Constant'
  //   Constant: '<S1>/Constant1'
  //   Constant: '<S2>/Constant'
  //   Constant: '<S2>/Constant1'
  //   Constant: '<S2>/Constant10'
  //   Constant: '<S2>/Constant3'
  //   Constant: '<S2>/Constant4'
  //   Constant: '<S2>/Constant5'
  //   Constant: '<S2>/Constant9'
  //   Constant: '<S3>/Constant'
  //   Constant: '<S3>/Constant1'
  //   Constant: '<S3>/Constant10'
  //   Constant: '<S3>/Constant2'
  //   Constant: '<S3>/Constant3'
  //   Constant: '<S3>/Constant4'
  //   Constant: '<S3>/Constant5'
  //   Constant: '<S3>/Constant9'
  //   Logic: '<S12>/Logical Operator'
  //   Logic: '<S12>/Logical Operator1'
  //   RelationalOperator: '<S12>/Relational Operator2'
  //   RelationalOperator: '<S12>/Relational Operator3'

  if ((arg_VAR_INPUT->auto_manual == 1) && (arg_VAR_INPUT->launch_stop == 1)) {
    // Outputs for IfAction SubSystem: '<Root>/Auto Mode' incorporates:
    //   ActionPort: '<S1>/Action Port'

    rtb_Merge = 13U;
    rtb_Merge1 = 1U;

    // If: '<S5>/If' incorporates:
    //   Constant: '<S1>/Constant'
    //   Constant: '<S1>/Constant1'
    //   Constant: '<S5>/Constant'
    //   Constant: '<S5>/Constant1'
    //   Constant: '<S7>/Constant1'
    //   Constant: '<S7>/Constant3'
    //   Constant: '<S7>/Constant4'
    //   Constant: '<S8>/Constant'
    //   Constant: '<S8>/Constant2'
    //   If: '<S8>/If'

    if ((arg_VAR_INPUT->center_point_x <= 25.0F) &&
        (arg_VAR_INPUT->center_point_x >= 1.0F)) {
      // Outputs for IfAction SubSystem: '<S5>/Brake mode' incorporates:
      //   ActionPort: '<S7>/Action Port'

      // DataTypeConversion: '<S7>/Data Type Conversion' incorporates:
      //   Constant: '<S7>/1MPa'

      rtb_Merge2 = 1.5F;
      rtb_Merge3 = 1U;
      rtb_Merge4 = 0;
      rtb_Merge5 = 1U;

      // End of Outputs for SubSystem: '<S5>/Brake mode'
    } else {
      // Outputs for IfAction SubSystem: '<S5>/Moving mode' incorporates:
      //   ActionPort: '<S8>/Action Port'

      if (arg_VAR_INPUT->vehicle_speed >= 25) {
        // Outputs for IfAction SubSystem: '<S8>/>25' incorporates:
        //   ActionPort: '<S11>/Action Port'

        // If: '<S8>/If' incorporates:
        //   Constant: '<S11>/1MPa'
        //   Constant: '<S11>/Constant1'
        //   Constant: '<S11>/Constant3'
        //   Constant: '<S11>/Constant4'
        //   DataTypeConversion: '<S11>/Data Type Conversion'

        rtb_Merge2 = 0.4F;
        rtb_Merge3 = 1U;
        rtb_Merge4 = 0;
        rtb_Merge5 = 1U;

        // End of Outputs for SubSystem: '<S8>/>25'
      } else if (arg_VAR_INPUT->vehicle_speed <= 15) {
        // Outputs for IfAction SubSystem: '<S8>/<15' incorporates:
        //   ActionPort: '<S10>/Action Port'

        // If: '<S8>/If' incorporates:
        //   Constant: '<S10>/1MPa'
        //   Constant: '<S10>/Constant1'
        //   Constant: '<S10>/Constant3'
        //   Constant: '<S10>/Constant4'

        rtb_Merge2 = 0.0F;
        rtb_Merge3 = 0U;
        rtb_Merge4 = 15;
        rtb_Merge5 = 1U;

        // End of Outputs for SubSystem: '<S8>/<15'
      } else {
        // Outputs for IfAction SubSystem: '<S8>/15--25' incorporates:
        //   ActionPort: '<S9>/Action Port'

        // If: '<S8>/If' incorporates:
        //   Constant: '<S9>/1MPa'
        //   Constant: '<S9>/Constant1'
        //   Constant: '<S9>/Constant3'
        //   Constant: '<S9>/Constant4'

        rtb_Merge2 = 0.0F;
        rtb_Merge3 = 0U;
        rtb_Merge4 = 5;
        rtb_Merge5 = 1U;

        // End of Outputs for SubSystem: '<S8>/15--25'
      }

      // End of Outputs for SubSystem: '<S5>/Moving mode'
    }

    // End of If: '<S5>/If'
    rtb_Merge7 = 1U;

    // Outputs for Enabled SubSystem: '<S12>/Enabled Subsystem' incorporates:
    //   EnablePort: '<S13>/Enable'

    // Outputs for Enabled SubSystem: '<S12>/Enabled Subsystem1' incorporates:
    //   EnablePort: '<S14>/Enable'

    if (!((arg_VAR_INPUT->longitude <= 112.85) && (arg_VAR_INPUT->latitude >=
          28.295065))) {
      rtb_Merge6 = 2;
    } else {
      rtb_Merge6 = 60;
    }

    // End of Outputs for SubSystem: '<S12>/Enabled Subsystem1'
    // End of Outputs for SubSystem: '<S12>/Enabled Subsystem'
    // End of Outputs for SubSystem: '<Root>/Auto Mode'
  } else if ((arg_VAR_INPUT->auto_manual == 1) && (arg_VAR_INPUT->launch_stop ==
              0)) {
    // Outputs for IfAction SubSystem: '<Root>/Auto stop mode' incorporates:
    //   ActionPort: '<S2>/Action Port'

    rtb_Merge = 12U;
    rtb_Merge1 = 1U;

    // DataTypeConversion: '<S2>/Data Type Conversion' incorporates:
    //   Constant: '<S2>/Constant'
    //   Constant: '<S2>/Constant1'
    //   Constant: '<S2>/Constant2'

    rtb_Merge2 = 0.7F;
    rtb_Merge3 = 1U;
    rtb_Merge4 = 0;
    rtb_Merge5 = 1U;
    rtb_Merge6 = 0;
    rtb_Merge7 = 1U;

    // End of Outputs for SubSystem: '<Root>/Auto stop mode'
  } else {
    // Outputs for IfAction SubSystem: '<Root>/Manual Mode' incorporates:
    //   ActionPort: '<S3>/Action Port'

    rtb_Merge = 0U;
    rtb_Merge1 = 0U;
    rtb_Merge2 = 0.0F;
    rtb_Merge3 = 0U;
    rtb_Merge4 = 0;
    rtb_Merge5 = 0U;
    rtb_Merge6 = 0;
    rtb_Merge7 = 0U;

    // End of Outputs for SubSystem: '<Root>/Manual Mode'
  }

  // End of If: '<Root>/If'

  // BusCreator: '<Root>/BusConversion_InsertedFor_VAR_OUTPUT_at_inport_0'
  arg_VAR_OUTPUT->gear_sts = rtb_Merge;
  arg_VAR_OUTPUT->gear_enable = rtb_Merge1;
  arg_VAR_OUTPUT->brake_pressure = rtb_Merge2;
  arg_VAR_OUTPUT->brake_enable = rtb_Merge3;
  arg_VAR_OUTPUT->gas_pedal_open = (real32_T)rtb_Merge4;
  arg_VAR_OUTPUT->gas_pedal_enable = rtb_Merge5;
  arg_VAR_OUTPUT->steering_angle = (real32_T)rtb_Merge6;
  arg_VAR_OUTPUT->steering_angle_enable = rtb_Merge7;
  arg_VAR_OUTPUT->left_lamp = 0U;
  arg_VAR_OUTPUT->right_lamp = 0U;
  arg_VAR_OUTPUT->horn = 0U;
}

// Model initialize function
void output_mModelClass::initialize()
{
  // Registration code

  // states (dwork)

  // exported global states
  steeringAngle = 0.0F;
}

// Constructor
output_mModelClass::output_mModelClass()
{
}

// Destructor
output_mModelClass::~output_mModelClass()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
RT_MODEL_app_m_T * output_mModelClass::getRTM()
{
  return (&app_m_M);
}

// member function to initialize Real-Time model
void output_mModelClass::initializeRTM()
{
  (void) memset((void *)((&app_m_M)), 0,
                sizeof(RT_MODEL_app_m_T));
}

// member function to setup error status pointer
void output_mModelClass::setErrorStatusPointer(const char_T **rt_errorStatus)
{
  rtmSetErrorStatusPointer((&app_m_M), rt_errorStatus);
}

//
// File trailer for generated code.
//
// [EOF]
//
