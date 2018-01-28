//
// File: main_m.h
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
#ifndef RTW_HEADER_main_m_h_
#define RTW_HEADER_main_m_h_
#include <stddef.h>
#ifndef main_m_COMMON_INCLUDES_
# define main_m_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 // main_m_COMMON_INCLUDES_

#include "main_m_types.h"

// Shared type includes
#include "model_reference_types.h"

// Child system includes
#include "opt_m.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetErrorStatusPointer
# define rtmGetErrorStatusPointer(rtm) ((const char_T **)(&((rtm)->errorStatus)))
#endif

// Real-time Model Data Structure
struct tag_RTM_main_m_T {
  const char_T * volatile errorStatus;
};

#ifdef __cplusplus

extern "C" {

#endif

#ifdef __cplusplus

}
#endif

// Class declaration for model main_m
class main_mModelClass {
  // public data and function members
 public:
  // Model entry point functions

  // model initialize function
  void initialize();

  // model step function
  void step(const VAR_PLAT *arg_VAR_PLAT, VAR_OUTPUT *arg_VAR_OUTPUT);

  // model terminate function
  void terminate();

  // Constructor
  main_mModelClass();

  // Destructor
  ~main_mModelClass();

  // Real-Time Model get method
  RT_MODEL_main_m_T * getRTM();

  // private data and function members
 private:
  // Real-Time Model
  RT_MODEL_main_m_T main_m_M;

  // model instance variable for '<Root>/OUTPUT'
  opt_mModelClass OUTPUTMDLOBJ3;
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
//  '<Root>' : 'main_m'

#endif                                 // RTW_HEADER_main_m_h_

//
// File trailer for generated code.
//
// [EOF]
//
