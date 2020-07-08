/******************************************************************************
 *
 * (c) 2020 Copyright, Real-Time Innovations, Inc. (RTI)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ******************************************************************************/ 

/**
 * @file nano_client_config.h
 * @brief Library configuration.
 * 
 */

#ifndef nano_client_config_h
#define nano_client_config_h

#include "nano/nano_core.h"

/**
 * @addtogroup nano_api_libcfg_client
 * @{
 */

/******************************************************************************
 * Optional Features
 ******************************************************************************/

#ifndef NANO_FEAT_EXTENDED_API
/*e
 * @brief Enable Client's extended API
 * 
 */
#define NANO_FEAT_EXTENDED_API               1
#endif /* NANO_FEAT_EXTENDED_API */

#ifndef NANO_FEAT_AGENT_DISCOVERY
/*e
 * @brief Enable support for dynamically discovering Agent's location.
 * 
 */
#define NANO_FEAT_AGENT_DISCOVERY           1
#endif /* NANO_FEAT_AGENT_DISCOVERY */


/******************************************************************************
 * Resource Limits
 ******************************************************************************/

#ifndef NANO_LIMIT_CLIENT_MAX_PENDING_REQUESTS
/*e
 * @brief Default maximum number of pending requests for a client.
 * 
 */
#define NANO_LIMIT_CLIENT_MAX_PENDING_REQUESTS          4
#endif /* NANO_LIMIT_CLIENT_MAX_PENDING_REQUESTS */

#ifndef NANO_LIMIT_CLIENT_INCLUDE_KEY_IN_MESSAGE_DEFAULT
/**
 * @brief TODO
 * 
 */
#define NANO_LIMIT_CLIENT_INCLUDE_KEY_IN_MESSAGE_DEFAULT        NANO_BOOL_FALSE
#endif /* NANO_LIMIT_CLIENT_INCLUDE_KEY_IN_MESSAGE_DEFAULT */

#ifndef NANO_LIMIT_CLIENT_SESSION_ID_WITH_KEY_DEFAULT
/**
 * @brief TODO
 * 
 */
#define NANO_LIMIT_CLIENT_SESSION_ID_WITH_KEY_DEFAULT           0x01
#endif /* NANO_LIMIT_CLIENT_SESSION_ID_WITH_KEY_DEFAULT */

#ifndef NANO_LIMIT_CLIENT_SESSION_ID_WITHOUT_KEY_DEFAULT
/**
 * @brief TODO
 * 
 */
#define NANO_LIMIT_CLIENT_SESSION_ID_WITHOUT_KEY_DEFAULT        0x81
#endif /* NANO_LIMIT_CLIENT_SESSION_ID_WITHOUT_KEY_DEFAULT */

/** @} *//* nano_api_libcfg_client */

#endif /* nano_client_config_h */
