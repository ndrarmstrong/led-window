import { Server as AedesServer, Aedes, AuthenticateError, Client } from 'aedes';
import { createServer, Server } from 'net';
import ServiceConfig from './serviceConfig';

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
    this.aedesServer = AedesServer({
      authenticate: (_client: Client, _username: string, password: Buffer, callback) => {
        if (password) {
          const pwString = password.toString();
          const keys = ServiceConfig.deviceKeys.concat(ServiceConfig.apiKeys);
          if (keys.indexOf(pwString) >= 0) {
            callback(null, true);
            return;
          }
        }
        const error = new Error() as AuthenticateError;
        error.returnCode = 1;
        callback(error, false);
      },
    });
    this.netServer = createServer(this.aedesServer.handle);
  }

  /**
   * Start the broker.
   */
  listen(): void {
    this.aedesServer.on('client', (client) => {
      this.log(`connected`, client);
    });

    this.aedesServer.on('clientDisconnect', (client) => {
      this.log(`disconnected`, client);
    });

    this.aedesServer.on('clientError', (client) => {
      this.log(`error`, client);
    });

    this.aedesServer.on('subscribe', (subs, client) => {
      for (const sub of subs) {
        this.log(`subscribed to '${sub.topic}'`, client);
      }
    });

    this.aedesServer.on('unsubscribe', (subs, client) => {
      for (const sub of subs) {
        this.log(`unsubscribed from '${sub}'`, client);
      }
    });

    this.aedesServer.on('publish', (packet, client) => {
      // Don't print internal messages
      if (!client) {
        return;
      }

      // Limited-length preview of payload
      let payloadStr = packet.payload.toString();
      payloadStr = payloadStr.length > 100 ? `${payloadStr.substr(0, 40)}...` : payloadStr;
      this.log(`published ${packet.retain ? 'RETAINED' : ''}to ${packet.topic} with payload '${payloadStr}'`, client);
    });

    this.netServer.listen(this.port, this.host, () => {
      this.log(`Broker running on ${this.host}:${this.port}`);
    });
  }

  /**
   * Log with timestamp and tag
   */
  private log(message: string, client?: Client): void {
    const clientStr = client ? `client '${client.id}' ` : '';
    console.log(`[${new Date().toISOString()}] MQTT: ${clientStr}${message}`);
  }
}
