//
// File: model_reference_types.h
//
// Code generated for Simulink model 'app_m'.
//
// Model version                  : 1.120
// Simulink Coder version         : 8.9 (R2015b) 13-Aug-2015
// C/C++ source code generated on : Sun Sep  3 07:59:35 2017
//
#ifndef __MODEL_REFERENCE_TYPES_H__
#define __MODEL_REFERENCE_TYPES_H__
#include "rtwtypes.h"
#ifndef __MODEL_REFERENCE_TYPES__
#define __MODEL_REFERENCE_TYPES__

//===========================================================================*
//  Model reference type definitions                                          *
// ===========================================================================
//
//  This structure is used by model reference to
//  communicate timing information through the hierarchy.

typedef struct _rtTimingBridge_tag rtTimingBridge;
struct _rtTimingBridge_tag {
  uint32_T nTasks;
  uint32_T** clockTick;
  uint32_T** clockTickH;
  uint32_T* taskCounter;
  real_T** taskTime;
  boolean_T** rateTransition;
  boolean_T *firstInitCond;
};

#endif                                 // __MODEL_REFERENCE_TYPES__
#endif                                 // __MODEL_REFERENCE_TYPES_H__

//
// File trailer for generated code.
//
// [EOF]
//
