//
// File: input_m_types.h
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
#ifndef RTW_HEADER_input_m_types_h_
#define RTW_HEADER_input_m_types_h_
#include "rtwtypes.h"
#include "model_reference_types.h"
#ifndef _DEFINED_TYPEDEF_FOR_VAR_PLAT_
#define _DEFINED_TYPEDEF_FOR_VAR_PLAT_

typedef struct {
  real_T longitude;
  real_T latitude;
  real32_T center_point_x;
  real32_T center_point_y;
  real32_T obj_size_x;
  real32_T obj_size_y;
  uint16_T obj_kind;
  uint8_T auto_manual;
  uint8_T launch_stop;
  uint8_T vehicle_speed;
} VAR_PLAT;

#endif

#ifndef _DEFINED_TYPEDEF_FOR_VAR_INPUT_
#define _DEFINED_TYPEDEF_FOR_VAR_INPUT_

typedef struct {
  real_T longitude;
  real_T latitude;
  real32_T center_point_x;
  real32_T center_point_y;
  real32_T obj_size_x;
  real32_T obj_size_y;
  uint16_T obj_kind;
  uint8_T auto_manual;
  uint8_T launch_stop;
  uint8_T vehicle_speed;
} VAR_INPUT;

#endif

// Forward declaration for rtModel
typedef struct tag_RTM_input_m_T RT_MODEL_input_m_T;

#endif                                 // RTW_HEADER_input_m_types_h_

//
// File trailer for generated code.
//
// [EOF]
//
