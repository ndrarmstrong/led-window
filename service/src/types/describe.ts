/**
 * Device describe response
 */
export interface DescribeResponse {
  /**
   * Device ID
   */
  device: string;

  /**
   * Free heap space on the device
   */
  freeHeapB?: number;

  /**
   * Size of the running sketch
   */
  sketchSizeB?: number;

  /**
   * Free sketch space
   */
  freeSketchSpaceB?: number;

  /**
   * Whether OTA updates are enabled
   */
  otaEnabled?: boolean;

  /**
   * The device temperture, in celcius.
   */
  temperatureC?: number;

  /**
   * The relative humidity at the device, in percent.
   */
  relativeHumidityPct?: number;
}
