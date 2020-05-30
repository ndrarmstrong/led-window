import { flags } from '@oclif/command';
import { AcknowledgeResponses, Acknowledgement } from '../types/ack';
import DeviceCommand from './deviceCommand';

/**
 * Base type for device commands - can be requested over MQTT or HTTP.
 */
export default abstract class DeviceRequestCommand extends DeviceCommand {
  static flags = {
    ...DeviceCommand.flags,
    method: flags.string({ description: 'Request method', options: ['http', 'mqtt'], default: 'mqtt' }),
    reply: flags.boolean({ description: 'Self-reply to request', hidden: true }),
  };

  /**
   * Reply with a sample acknowledgement
   */
  protected selfAcknowledge(): Acknowledgement {
    return {
      result: AcknowledgeResponses.Success,
    };
  }
}
