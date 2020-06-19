/**
 * Light intensity range
 * In a dark environment 375Lux is best, otherwise 10KLux is likely the best option
 */
export enum RgbIntensityRange {
  Range375Lux = 0x00,
  Range10kLux = 0x08,
}

/**
 * Upper or lower range of IR filtering
 */
export enum RgbIRFilterOffset {
  Off = 0x00,
  On = 0x80,
}

/**
 * Sets amount of IR filtering, can use these presets or any value between 0x00 and 0x3F
 * Consult datasheet for detailed IR filtering calibration
 */
export enum RgbIRFilterLevel {
  Low = 0x00,
  Mid = 0x20,
  High = 0x3f,
}

/**
 * RGB sensor configuration
 */
export interface RgbConfig {
  intensityRange?: RgbIntensityRange;
  irFilterOffset?: RgbIRFilterOffset;
  irFilterLevel?: RgbIRFilterLevel;
}
