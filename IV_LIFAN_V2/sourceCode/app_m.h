//
// File: app_m.h
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
#ifndef RTW_HEADER_app_m_h_
#define RTW_HEADER_app_m_h_
#include <string.h>
#include <stddef.h>
#ifndef app_m_COMMON_INCLUDES_
# define app_m_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 // app_m_COMMON_INCLUDES_

#include "app_m_types.h"

// Shared type includes
#include "model_reference_types.h"
#include <string.h>
#include <stddef.h>

// Real-time Model Data Structure
struct tag_RTM_app_m_T {
  const char_T **errorStatus;
};

//
//  Exported States
//
//  Note: Exported states are block states with an exported global
//  storage class designation.  Code generation will declare the memory for these
//  states and exports their symbols.
//

extern real32_T steeringAngle;         // '<Root>/Data Store Memory'

// Class declaration for model app_m
class output_mModelClass {
  // public data and function members
 public:
  // Model entry point functions

  // model initialize function
  void initialize();

  // model step function
  void step(const VAR_INPUT *arg_VAR_INPUT, VAR_OUTPUT *arg_VAR_OUTPUT);

  // Constructor
  output_mModelClass();

  // Destructor
  ~output_mModelClass();

  // Real-Time Model get method
  RT_MODEL_app_m_T * getRTM();

  // member function to initialize Real-Time model
  void initializeRTM();

  //member function to setup error status pointer
  void setErrorStatusPointer(const char_T **rt_errorStatus);

  // private data and function members
 private:
  // Real-Time Model
  RT_MODEL_app_m_T app_m_M;
};

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'app_m'
//  '<S1>'   : 'app_m/Auto Mode'
//  '<S2>'   : 'app_m/Auto stop mode'
//  '<S3>'   : 'app_m/Manual Mode'
//  '<S4>'   : 'app_m/Subsystem'
//  '<S5>'   : 'app_m/Auto Mode/Distance judge'
//  '<S6>'   : 'app_m/Auto Mode/steering angle'
//  '<S7>'   : 'app_m/Auto Mode/Distance judge/Brake mode'
//  '<S8>'   : 'app_m/Auto Mode/Distance judge/Moving mode'
//  '<S9>'   : 'app_m/Auto Mode/Distance judge/Moving mode/15--25'
//  '<S10>'  : 'app_m/Auto Mode/Distance judge/Moving mode/<15'
//  '<S11>'  : 'app_m/Auto Mode/Distance judge/Moving mode/>25'
//  '<S12>'  : 'app_m/Auto Mode/steering angle/Subsystem'
//  '<S13>'  : 'app_m/Auto Mode/steering angle/Subsystem/Enabled Subsystem'
//  '<S14>'  : 'app_m/Auto Mode/steering angle/Subsystem/Enabled Subsystem1'

#endif                                 // RTW_HEADER_app_m_h_

//
// File trailer for generated code.
//
// [EOF]
//
