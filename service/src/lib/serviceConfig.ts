export default class ServiceConfig {
  /**
   * List of device keys; used for MQTT broker.
   */
  static deviceKeys: string[] = [];

  /**
   * List of API keys; used for accessing the MQTT broker or HTTP API.
   */
  static apiKeys: string[] = [];
}
