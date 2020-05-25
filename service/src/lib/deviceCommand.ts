import { Command, flags } from '@oclif/command';
import { AcknowledgeResponses, Acknowledgement } from '../types/ack';

/**
 * Base type for device commands - can be requested over MQTT or HTTP.
 */
export default abstract class DeviceCommand extends Command {
  static flags = {
    help: flags.help({ char: 'h' }),
    port: flags.integer({ description: 'Server port', default: 1883 }),
    address: flags.string({ description: 'Server address', default: '127.0.0.1' }),
    method: flags.string({ description: 'Request method', options: ['http', 'mqtt'], default: 'mqtt' }),
    reply: flags.boolean({ description: 'Self-reply to request', hidden: true }),
  };

  static deviceArg = { name: 'device', description: 'Device ID', required: true };

  /**
   * Get the device topic
   * @param id Device ID
   */
  protected deviceTopic(id: string): string {
    return `device/${id}`;
  }

  /**
   * Reply with a sample acknowledgement
   */
  protected selfAcknowledge(): Acknowledgement {
    return {
      result: AcknowledgeResponses.Success,
    };
  }
}
