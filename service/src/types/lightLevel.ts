/**
 * Light sensor read response.
 */
export interface LightLevelReading {
  /**
   * Light intensity, in Lux.
   * NB: This is read by a separate sensor than the red/green/blue values.
   */
  lux: number;

  /**
   * Red level (not in lux).
   * NB: This is read by a separate sensor than the lux value.
   */
  red: number;

  /**
   * Green level (not in lux).
   * NB: This is read by a separate sensor than the lux value.
   */
  green: number;

  /**
   * Blue level (not in lux).
   * NB: This is read by a separate sensor than the lux value.
   */
  blue: number;
}
