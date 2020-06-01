/**
 * Window modes
 */
export enum WindowModes {
  /**
   * Off mode
   */
  Off = 0,

  /**
   * Startup mode
   */
  Startup = 1,

  /**
   * System mode - displays system status
   */
  System = 2,

  /**
   * Self test mode - shows LED strip channels
   */
  SelfTest = 3,

  /**
   * Raw mode - renders color exactly as given
   */
  Raw = 4,

  /**
   * Daylight mode - renders outside color
   */
  Daylight = 5,

  /**
   * Stained glass mode - emulate a stained glass window
   */
  StainedGlass = 6,
}

/**
 * Payload of a window mode request
 */
export interface WindowModePayload {
  /**
   * Active or requested mode
   */
  mode: WindowModes;
}
