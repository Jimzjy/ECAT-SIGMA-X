#ifndef __ECAT_PARAMS__
#define __ECAT_PARAMS__

#include <stddef.h>
#include <stdint.h>

typedef enum {
  DISPLAY_FORMAT_BYTE_ARRAY = 0,
  DISPLAY_FORMAT_DEC
} ParamDisplayFormat;

typedef struct Parameter {
  uint16_t paramIndex;
  uint8_t selectParamBit;
  uint32_t rawData; // 真正的数据
  uint32_t data_min;
  uint32_t data_max;
  int dataSize;
  ParamDisplayFormat displayFormat;
  uint8_t selectDisplayBit;
} Parameter;

typedef struct ImportParameter {
  uint16_t paramIndex;
  uint32_t rawData;
  int dataSize;
} ImportParameter;

typedef struct ParameterRule {
  uint16_t paramIndex;
  uint8_t dataSize;
  ParamDisplayFormat displayFormat;
  uint32_t data_min;
  uint32_t data_max;
} ParameterRule;

extern const ImportParameter importParamStore_0[];
extern const uint8_t importParamStore_0_size;
extern const ImportParameter importParamStore_1[];
extern const uint8_t importParamStore_1_size;

extern const ParameterRule parameterRules[];
extern const uint16_t parameterRules_size;

#endif