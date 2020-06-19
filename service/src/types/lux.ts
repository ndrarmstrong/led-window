/**
 * Lux sensor gain
 */
export type LuxGain = 0.125 | 0.25 | 0.5 | 1 | 2;

/**
 * Lux sensor integration time (in milliseconds)
 */
export type LuxIntegrationMs = 25 | 50 | 100 | 200 | 400 | 800;

/**
 * Lux sensor configuration.
 */
export interface LuxConfig {
  /**
   * Sensor gain
   */
  gain?: LuxGain;

  /**
   * Sensor integration time (in ms)
   */
  integrationTimeMs?: LuxIntegrationMs;
}
