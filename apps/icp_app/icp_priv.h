/**
 * @file    icp_priv.h
 * @author  Cypherock X1 Team
 * @brief   Support for icp app internal operations
 *          This file is defined to separate ICP's internal use
 * functions, flows, common APIs
 * @copyright Copyright (c) 2024 HODL TECH PTE LTD
 * <br/> You may obtain a copy of license at <a href="https://mitcc.org/"
 * target=_blank>https://mitcc.org/</a>
 */
#ifndef ICP_PRIV_H
#define ICP_PRIV_H
/*****************************************************************************
 * INCLUDES
 *****************************************************************************/
#include <icp/core.pb.h>
#include <stdint.h>

#include "icp_context.h"

/*****************************************************************************
 * TYPEDEFS
 *****************************************************************************/
typedef struct {
  /**
   * The structure holds the wallet information of the transaction.
   * @note Populated by icp_handle_initiate_query()
   */
  icp_sign_txn_initiate_request_t init_info;

  // remembers the allocated buffer for holding complete unsigned transaction
  uint8_t *transaction;

  // decoded raw txn
  icp_unsigned_txn *raw_txn;

} icp_txn_context_t;

/*****************************************************************************
 * EXPORTED VARIABLES
 *****************************************************************************/

/*****************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
 *****************************************************************************/

/**
 * @brief Handler for ICP public key derivation.
 * @details This flow expects ICP_GET_PUBLIC_KEY_REQUEST_INITIATE_TAG as initial
 * query, otherwise the flow is aborted
 *
 * @param query object for address public key query
 */
void icp_get_pub_keys(icp_query_t *query);

/**
 * @brief Handler for signing a transaction on icp.
 * @details The expected request type is ICP_SIGN_TXN_REQUEST_INITIATE_TAG. The
 * function controls the complete data exchange with host, user prompts and
 * confirmations for signing an ICP based transaction.
 *
 * @param query Reference to the decoded query struct from the host app
 */
void icp_sign_transaction(icp_query_t *query);

#endif /* ICP_PRIV_H */
