/**
 * Types of acknowledgement responses.
 */
export enum AcknowledgeResponses {
  /**
   * The action was successful.
   */
  Success = 0,

  /**
   * The action caused an unknown error.
   */
  UnknownError = 1,
}

/**
 * Generic acknowledgement response.
 */
export interface Acknowledgement {
  result: AcknowledgeResponses;
}
