//
// File: opt_m.h
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
#ifndef RTW_HEADER_opt_m_h_
#define RTW_HEADER_opt_m_h_
#include <string.h>
#include <stddef.h>
#ifndef opt_m_COMMON_INCLUDES_
# define opt_m_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 // opt_m_COMMON_INCLUDES_

#include "opt_m_types.h"

// Shared type includes
#include "model_reference_types.h"
#include <string.h>
#include <stddef.h>

// Real-time Model Data Structure
struct tag_RTM_opt_m_T {
  const char_T **errorStatus;
};

// Class declaration for model opt_m
class opt_mModelClass {
  // public data and function members
 public:
  // Model entry point functions

  // model initialize function
  void initialize();

  // model step function
  void step(const VAR_OUTPUT *arg_VAR_OUTPUT, VAR_ACTUATOR *arg_VAR_ACTUATOR);

  // Constructor
  opt_mModelClass();

  // Destructor
  ~opt_mModelClass();

  // Real-Time Model get method
  RT_MODEL_opt_m_T * getRTM();

  // member function to initialize Real-Time model
  void initializeRTM();

  //member function to setup error status pointer
  void setErrorStatusPointer(const char_T **rt_errorStatus);

  // private data and function members
 private:
  // Real-Time Model
  RT_MODEL_opt_m_T opt_m_M;
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
//  '<Root>' : 'opt_m'

#endif                                 // RTW_HEADER_opt_m_h_

//
// File trailer for generated code.
//
// [EOF]
//
