/**
 * Raw mode configure payload.
 */
export default class RawRequest {
  /**
   * Red channel level; 0-255
   */
  protected r = 0;

  /**
   * Green channel level; 0-255
   */
  protected g = 0;

  /**
   * Blue channel level; 0-255
   */
  protected b = 0;

  /**
   * Daylight white channel level; 0-255
   */
  protected dw = 0;

  /**
   * Get red level.
   */
  get red(): number {
    return this.r;
  }

  /**
   * Set red level.
   */
  set red(level: number) {
    if (!(level >= 0 && level <= 255)) {
      throw new Error(`Red level '${level}' out of range; must be 0-255`);
    }
    this.r = level;
  }

  /**
   * Get green level.
   */
  get green(): number {
    return this.g;
  }

  /**
   * Set green level.
   */
  set green(level: number) {
    if (!(level >= 0 && level <= 255)) {
      throw new Error(`Green level '${level}' out of range; must be 0-255`);
    }
    this.g = level;
  }

  /**
   * Get blue level.
   */
  get blue(): number {
    return this.b;
  }

  /**
   * Set blue level.
   */
  set blue(level: number) {
    if (!(level >= 0 && level <= 255)) {
      throw new Error(`Blue level '${level}' out of range; must be 0-255`);
    }
    this.b = level;
  }

  /**
   * Get daylight white level.
   */
  get daylightWhite(): number {
    return this.dw;
  }

  /**
   * Set daylight white level.
   */
  set daylightWhite(level: number) {
    if (!(level >= 0 && level <= 255)) {
      throw new Error(`Daylight white level '${level}' out of range; must be 0-255`);
    }
    this.dw = level;
  }

  /**
   * Create an instance of the RawRequest class.
   * @param r Red level
   * @param g Green level
   * @param b Blue level
   * @param dw Daylight white level
   */
  static fromValues(r: number, g: number, b: number, dw: number): RawRequest {
    const req = new RawRequest();
    req.red = r;
    req.green = g;
    req.blue = b;
    req.daylightWhite = dw;
    return req;
  }
}
