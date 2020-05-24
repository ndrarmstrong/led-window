import { connectAsync } from 'async-mqtt';

/**
 * MQTT client and associated utilities
 */
export default class MqttClient {
  static topicNamespace = 'ledwin';

  /**
   * Connect, send a request, and wait for a response.
   * @param address Broker address
   * @param port Broker port
   * @param topic Topic to request on
   * @param message Request message
   */
  static async singleRequestResponse<T>(
    address: string,
    port: number,
    topic: string,
    message: string,
    selfReply?: () => T
  ): Promise<T> {
    const timeoutMs = 10 * 1000;
    const url = `mqtt://${address}:${port}`;
    console.log(`Connecting to MQTT broker ${url}`);
    const client = await connectAsync(url);

    const resPromise = new Promise<Buffer>((resolve, reject) => {
      const timeout = setTimeout(() => {
        reject('Timeout waiting for response');
      }, timeoutMs);
      client.on('message', (_topic, resMessage) => {
        clearTimeout(timeout);
        resolve(resMessage);
      });
    });

    const reqTopic = `${MqttClient.topicNamespace}/${topic}/req`;
    const resTopic = `${MqttClient.topicNamespace}/${topic}`;
    await client.subscribe(resTopic);
    console.log(`Sending request to ${reqTopic}`);
    await client.publish(reqTopic, message);
    console.log(`Waiting for response on ${resTopic}`);

    // Generate a reply to one's own request by publishing
    // sample data on the response topic (for testing)
    if (selfReply) {
      setTimeout(() => {
        client.publish(resTopic, JSON.stringify(selfReply()));
      }, 1000);
    }

    let resBody: T | undefined = undefined;

    const res = await resPromise;
    resBody = JSON.parse(res.toString()) as T;

    client.end();
    return resBody;
  }
}
