/**
 * Window modes
 */
export enum WindowModes {
  /**
   * Startup mode
   */
  Startup = 0,

  /**
   * System mode - displays system status
   */
  System = 1,

  /**
   * Self test mode - shows LED strip channels
   */
  SelfTest = 2,

  /**
   * Raw mode - renders color exactly as given
   */
  Raw = 3,

  /**
   * Daylight mode - renders outside color
   */
  Daylight = 4,

  /**
   * Stained glass mode - emulate a stained glass window
   */
  StainedGlass = 5,
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
