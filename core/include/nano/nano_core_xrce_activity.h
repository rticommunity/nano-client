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
 * 
 ******************************************************************************/ 

#ifndef nano_core_xrce_activity_h
#define nano_core_xrce_activity_h

#if NANO_FEAT_OBJECT_INFO

/******************************************************************************
 *                              InfoMask
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_support_infomask
 * @{
 */

/*i
 * @brief TODO
 * 
 */
typedef NANO_u32 NANO_XRCE_InfoMask;

NANO_bool
NANO_XRCE_InfoMask_initialize(NANO_XRCE_InfoMask *const self);

#define NANO_XRCE_InfoMask_initialize(s_) \
    (*(s_) = NANO_XRCE_INFOMASK_DEFAULT, NANO_BOOL_TRUE)

/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_INFOMASK_DEFAULT      (0)

/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_INFOMASK_INFO_CONFIGURATION ((NANO_XRCE_InfoMask)0x01 << 0)

/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_INFOMASK_INFO_ACTIVITY ((NANO_XRCE_InfoMask)0x01<<1)


/*i @} *//* nano_api_xrce_payload_support_infomask */

/******************************************************************************
 *                              AGENT_ActivityInfo
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_activity_agent
 * @{
 */

/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_AgentActivityInfoI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_i16 availability;

#if NANO_FEAT_TYPED_SEQUENCE
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_TransportLocatorSeq address_seq;
#endif /* NANO_FEAT_TYPED_SEQUENCE */
} NANO_XRCE_AgentActivityInfo;

#if NANO_FEAT_TYPED_SEQUENCE
#define NANO_XRCE_AGENTACTIVITYINFO_ADDRESS_INITIALIZER \
    ,\
    NANO_XRCE_TRANSPORTLOCATORSEQ_INITIALIZER /* address_seq */\

#define NANO_XRCE_AGENTACTIVITYINFO_SERIALIZED_SIZE_MAX \
    NANO_OSAPI_Memory_align_size_up(\
        (sizeof(NANO_u16) + \
        /* sequence length */\
        sizeof(NANO_u32)), \
        sizeof(NANO_u32)) + \
        (NANO_XRCE_TRANSPORTLOCATOR_SERIALIZED_SIZE_MAX * \
            NANO_LIMIT_TRANSPORTLOCATORSEQ_MAX_LENGTH)
    /* total >= 8 + (24 * SEQ_MAX) >= 32 */
#else
#define NANO_XRCE_AGENTACTIVITYINFO_ADDRESS_INITIALIZER
#define NANO_XRCE_AGENTACTIVITYINFO_SERIALIZED_SIZE_MAX \
    NANO_OSAPI_Memory_align_size_up(\
        sizeof(NANO_u16) + \
        /* sequence length */\
        sizeof(NANO_u32),\
        sizeof(NANO_u32)) /* total = 8 */
#endif /* NANO_FEAT_TYPED_SEQUENCE */

#define NANO_XRCE_AGENTACTIVITYINFO_INITIALIZER \
{\
    0 /* availability */\
    NANO_XRCE_AGENTACTIVITYINFO_ADDRESS_INITIALIZER /* address_seq */\
}

NANO_RetCode
NANO_XRCE_AgentActivityInfo_initialize(
    NANO_XRCE_AgentActivityInfo *const self);

#define NANO_XRCE_AgentActivityInfo_initialize(s_) \
    NANO_XRCE_TransportLocatorSeq_initialize(&(s_)->address_seq)


void
NANO_XRCE_AgentActivityInfo_finalize(
    NANO_XRCE_AgentActivityInfo *const self);

#define NANO_XRCE_AgentActivityInfo_finalize(s_) \
    NANO_XRCE_TransportLocatorSeq_finalize(&(s_)->address_seq)

/*i @} *//* nano_api_xrce_payload_activity_agent */

/******************************************************************************
 *                          DATAWRITER_ActivityInfo
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_activity_dw
 * @{
 */

/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_DataWriterActivityInfoI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_u64 sample_seq_num;
    /*i
     * @brief TODO
     * 
     */
    NANO_u16 stream_seq_num;
} NANO_XRCE_DataWriterActivityInfo;

#define NANO_XRCE_DATAWRITERACTIVITYINFO_SERIALIZED_SIZE_MAX \
    NANO_OSAPI_Memory_align_size_up(\
        sizeof(NANO_u64) + sizeof(NANO_u16), sizeof(NANO_u32))
    /* total = 16 */

#define NANO_XRCE_DATAWRITERACTIVITYINFO_INITIALIZER \
{\
    0, /* sample_seq_num */\
    0 /* stream_seq_num */\
}

NANO_RetCode
NANO_XRCE_DataWriterActivityInfo_initialize(
    NANO_XRCE_DataWriterActivityInfo *const self);

#define NANO_XRCE_DataWriterActivityInfo_initialize(s_) \
    ((s_)->sample_seq_num = 0, \
    (s_)->stream_seq_num = 0, \
    NANO_BOOL_TRUE)



/*i @} *//* nano_api_xrce_payload_activity_dw */

/******************************************************************************
 *                          DATAREADER_ActivityInfo
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_activity_dr
 * @{
 */

/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_DataReaderActivityInfoI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_u16 highest_acked_seq_num;
} NANO_XRCE_DataReaderActivityInfo;


#define NANO_XRCE_DATAREADERACTIVITYINFO_SERIALIZED_SIZE_MAX \
    sizeof(NANO_u16)

#define NANO_XRCE_DATAREADERACTIVITYINFO_INITIALIZER \
{\
    0 /* highest_acked_seq_num */\
}

NANO_RetCode
NANO_XRCE_DataReaderActivityInfo_initialize(
    NANO_XRCE_DataReaderActivityInfo *const self);

#define NANO_XRCE_DataReaderActivityInfo_initialize(s_) \
    ((s_)->highest_acked_seq_num = 0, \
    NANO_BOOL_TRUE)



/*i @} *//* nano_api_xrce_payload_activity_dr */


/******************************************************************************
 *                          ActivityInfoVariant
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_activity
 * @{
 */

/*i
 * @brief TODO
 * 
 */
typedef union NANODllExport NANO_XRCE_ActivityInfoVariantValueI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_AgentActivityInfo agent;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_DataWriterActivityInfo data_writer;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_DataReaderActivityInfo data_reader;
} NANO_XRCE_ActivityInfoVariantValue;

#define NANO_XRCE_ACTIVITYINFOVARIANTVALUE_INITIALIZER \
{\
    NANO_XRCE_AGENTACTIVITYINFO_INITIALIZER /* agent */\
}

NANO_bool
NANO_XRCE_ActivityInfoVariantValue_initialize(
    NANO_XRCE_ActivityInfoVariantValue *const self);

#define NANO_XRCE_ActivityInfoVariantValue_initialize(s_) \
    (NANO_OSAPI_Memory_zero((s_),sizeof(NANO_XRCE_ActivityInfoVariantValue)),\
    NANO_BOOL_TRUE)


/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_ActivityInfoVariantI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_ObjectKind kind;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_ActivityInfoVariantValue value;
} NANO_XRCE_ActivityInfoVariant;

#if NANO_FEAT_TYPED_SEQUENCE
#define NANO_XRCE_ACTIVITYINFOVARIANT_SERIALIZED_SIZE_MAX \
    (sizeof(NANO_u32) /* use u32 for kind to account for padding */ +\
    NANO_XRCE_AGENTACTIVITYINFO_SERIALIZED_SIZE_MAX)
#else
#define NANO_XRCE_ACTIVITYINFOVARIANT_SERIALIZED_SIZE_MAX \
    (sizeof(NANO_u32) /* use u32 for kind to account for padding */ +\
    NANO_XRCE_DATAWRITERACTIVITYINFO_SERIALIZED_SIZE_MAX)
#endif

#define NANO_XRCE_ACTIVITYINFOVARIANT_INITIALIZER \
{\
    NANO_XRCE_OBJK_INVALID, /* kind */\
    NANO_XRCE_ACTIVITYINFOVARIANTVALUE_INITIALIZER /* value */\
}

NANO_bool
NANO_XRCE_ActivityInfoVariant_initialize(
    NANO_XRCE_ActivityInfoVariant *const self);

#define NANO_XRCE_ActivityInfoVariant_initialize(s_) \
    (NANO_XRCE_ObjectKind_initialize(&(s_)->kind) &&\
    NANO_XRCE_ActivityInfoVariantValue_initialize(&(s_)->value))

void
NANO_XRCE_ActivityInfoVariant_finalize(
    NANO_XRCE_ActivityInfoVariant *const self);

#define NANO_XRCE_ActivityInfoVariant_finalize(s_) \
    (NANO_XRCE_ActivityInfoVariantValue_finalize(&(s_)->value))

/*i @} *//* nano_api_xrce_payload_activity */


/******************************************************************************
 *                          ObjectInfo
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_activity
 * @{
 */


/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_ObjectInfoI
{
#if NANO_FEAT_OBJECT_INFO_ACTIVITY
    /*i
     * @brief TODO
     * 
     */
    NANO_OPTIONALMEMBER_DEFN(NANO_XRCE_ActivityInfoVariant, activity);
#else
    const NANO_bool has_activity;
#endif /* NANO_FEAT_OBJECT_INFO_ACTIVITY */
#if NANO_FEAT_OBJECT_INFO_CONFIG
    /*i
     * @brief TODO
     * 
     */
    NANO_OPTIONALMEMBER_DEFN(NANO_XRCE_ObjectVariant, config);
#else
    const NANO_bool has_config;
#endif /* NANO_FEAT_OBJECT_INFO_CONFIG */
} NANO_XRCE_ObjectInfo;

#if NANO_FEAT_OBJECT_INFO_ACTIVITY
#define NANO_XRCE_OBJECTINFO_ACTIVITY_SERIALIZED_SIZE_MAX \
    (sizeof(NANO_bool) + \
    /* activity info variant size already includes possible padding */\
    NANO_XRCE_ACTIVITYINFOVARIANT_SERIALIZED_SIZE_MAX)
#else
#define NANO_XRCE_OBJECTINFO_ACTIVITY_SERIALIZED_SIZE_MAX \
    sizeof(NANO_bool)
#endif /* NANO_FEAT_OBJECT_INFO_ACTIVITY */

#if NANO_FEAT_OBJECT_INFO_CONFIG
#define NANO_XRCE_OBJECTINFO_CONFIG_SERIALIZED_SIZE_MAX \
    (sizeof(NANO_bool) + \
    /* activity info variant size already includes possible padding */\
    NANO_XRCE_OBJECTVARIANT_SERIALIZED_SIZE_MAX)
#else
#define NANO_XRCE_OBJECTINFO_CONFIG_SERIALIZED_SIZE_MAX \
    sizeof(NANO_bool)
#endif /* NANO_FEAT_OBJECT_INFO_ACTIVITY */

#define NANO_XRCE_OBJECTINFO_SERIALIZED_SIZE_MAX \
    NANO_XRCE_OBJECTINFO_ACTIVITY_SERIALIZED_SIZE_MAX + \
    NANO_XRCE_OBJECTINFO_CONFIG_SERIALIZED_SIZE_MAX

#if NANO_FEAT_OPTIONAL && \
    (NANO_FEAT_OBJECT_INFO_ACTIVITY || \
        NANO_FEAT_OBJECT_INFO_CONFIG)
#if NANO_FEAT_OBJECT_INFO_ACTIVITY && \
        NANO_FEAT_OBJECT_INFO_CONFIG
#define NANO_XRCE_OBJECTINFO_INITIALIZER \
{\
    NANO_OPTIONALMEMBER_INITIALIZER(\
        NANO_XRCE_ACTIVITYINFOVARIANT_INITIALIZER), /* activity */\
    NANO_OPTIONALMEMBER_INITIALIZER(\
        NANO_XRCE_OBJECTVARIANT_INITIALIZER) /* config */\
}
#elif NANO_FEAT_OBJECT_INFO_ACTIVITY
#define NANO_XRCE_OBJECTINFO_INITIALIZER \
{\
    NANO_OPTIONALMEMBER_INITIALIZER(\
        NANO_XRCE_ACTIVITYINFOVARIANT_INITIALIZER), /* activity */\
    NANO_BOOL_FALSE /* has_config */\
}
#else
#define NANO_XRCE_OBJECTINFO_INITIALIZER \
{\
    NANO_BOOL_FALSE, /* activity */\
    NANO_OPTIONALMEMBER_INITIALIZER(\
        NANO_XRCE_OBJECTVARIANT_INITIALIZER) /* config */\
}
#endif /* NANO_FEAT_OBJECT_INFO_ACTIVITY */
#else
#define NANO_XRCE_OBJECTINFO_INITIALIZER \
{\
    NANO_BOOL_FALSE, /* has_activity */\
    NANO_BOOL_FALSE /* has_config  */\
}
#endif /* NANO_FEAT_OPTIONAL */

NANO_bool
NANO_XRCE_ObjectInfo_initialize(
    NANO_XRCE_ObjectInfo *const self);

#define NANO_XRCE_ObjectInfo_initialize(s_) \
    (NANO_XRCE_ActivityInfoVariant_initialize(\
        NANO_OPTIONALMEMBER_AS_REF((s_),activity)) && \
    NANO_XRCE_ObjectVariant_initialize(\
        NANO_OPTIONALMEMBER_AS_REF((s_),config)))

/*i
 * @brief TODO
 * 
 * @param self 
 * @return NANO_bool 
 */
NANO_bool
NANO_XRCE_ObjectInfo_is_valid(
    const NANO_XRCE_ObjectInfo *const self);

#define NANO_XRCE_ObjectInfo_is_valid(s_) \
    ((NANO_OPTIONALMEMBER_IS_SET((s_),activity)?\
        NANO_XRCE_ActivityInfoVariant_is_valid(NANO_OPTIONALMEMBER_AS_REF((s_),activity)):\
        NANO_BOOL_TRUE) && \
    (NANO_OPTIONALMEMBER_IS_SET((s_),config)?\
        NANO_XRCE_ObjectVariant_is_valid(NANO_OPTIONALMEMBER_AS_REF((s_),config)):\
        NANO_BOOL_TRUE))

/*i @} *//* nano_api_xrce_payload_activity */

#endif /* NANO_FEAT_OBJECT_INFO */

#endif /* nano_core_xrce_activity_h */
