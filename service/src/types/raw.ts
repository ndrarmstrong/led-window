/**
 * Raw mode configure payload.
 */
export default class RawRequest {
  /**
   * Red channel level; 0-255
   */
  protected _r = 0;

  /**
   * Green channel level; 0-255
   */
  protected _g = 0;

  /**
   * Blue channel level; 0-255
   */
  protected _b = 0;

  /**
   * Daylight white channel level; 0-255
   */
  protected _dw = 0;

  /**
   * Get red level.
   */
  get r(): number {
    return this._r;
  }

  /**
   * Set red level.
   */
  set r(level: number) {
    if (level < 0 || level > 255) {
      throw new Error(`Red level '${level}' out of range; must be 0-255`);
    }
    this._r = level;
  }

  /**
   * Get green level.
   */
  get g(): number {
    return this._g;
  }

  /**
   * Set green level.
   */
  set g(level: number) {
    if (level < 0 || level > 255) {
      throw new Error(`Green level '${level}' out of range; must be 0-255`);
    }
    this._g = level;
  }

  /**
   * Get blue level.
   */
  get b(): number {
    return this._b;
  }

  /**
   * Set blue level.
   */
  set b(level: number) {
    if (level < 0 || level > 255) {
      throw new Error(`Blue level '${level}' out of range; must be 0-255`);
    }
    this._b = level;
  }

  /**
   * Get daylight white level.
   */
  get dw(): number {
    return this._dw;
  }

  /**
   * Set daylight white level.
   */
  set dw(level: number) {
    if (level < 0 || level > 255) {
      throw new Error(`Daylight white level '${level}' out of range; must be 0-255`);
    }
    this._dw = level;
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
    req.r = r;
    req.g = g;
    req.b = b;
    req.dw = dw;
    return req;
  }
}
