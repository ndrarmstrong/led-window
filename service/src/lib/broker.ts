import { Server as AedesServer, Aedes } from 'aedes';
import { createServer, Server } from 'net';

/**
 * MQTT broker.
 */
export default class Broker {
  /**
   * Broker port number
   */
  private readonly port: number;

  /**
   * Broker host address
   */
  private readonly host: string;

  /**
   * Aedes MQTT server
   */
  private readonly aedesServer: Aedes;

  /**
   * Node server
   */
  private readonly netServer: Server;

  /**
   * Create an instance of the Broker class.
   * @param port The port number the broker should run on.
   */
  constructor(port: number, host: string) {
    this.port = port;
    this.host = host;
    this.aedesServer = AedesServer();
    this.netServer = createServer(this.aedesServer.handle);
  }

  /**
   * Start the broker.
   */
  listen(): void {
    this.aedesServer.on('client', (client) => {
      console.log(`MQTT: ${client.id} connected`);
    });

    this.aedesServer.on('clientReady', (client) => {
      console.log(`MQTT: ${client.id} is ready`);
    });

    this.aedesServer.on('clientDisconnect', (client) => {
      console.log(`MQTT: ${client.id} disconnected`);
    });

    this.netServer.listen(this.port, this.host, () => {
      console.log(`MQTT: Broker running on ${this.host}:${this.port}`);
    });
  }
}
