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

/**
 * @file nano_core_xrce_message.h
 * @brief XRCE Messages
 * 
 * This header file contains definitions for core types used by the XRCE
 * protocol and throughout its implementation.
 * 
 */

#ifndef nano_core_xrce_message_h
#define nano_core_xrce_message_h

#if NANO_FEAT_REQUEST
/******************************************************************************
 *                       RequestId
 ******************************************************************************/
/**
 * @addtogroup nano_api_clientrequests
 * @{
 */
/*e
 * @brief TODO
 */
#define NANO_XRCE_REQUESTID_VALUE_LEN       2

/*e
 * @brief TODO
 */
typedef struct NANODllExport NANO_XRCE_RequestIdI
{
    /*e
     * @brief TODO
     */
    NANO_u8 value[NANO_XRCE_REQUESTID_VALUE_LEN];

} NANO_XRCE_RequestId;

NANO_bool
NANO_XRCE_RequestId_initialize(
    NANO_XRCE_RequestId *const self);

#define NANO_XRCE_RequestId_initialize(s_) \
    (NANO_OSAPI_Memory_zero((s_)->value, NANO_XRCE_REQUESTID_VALUE_LEN),\
    NANO_BOOL_TRUE)

/*e
 * @brief TODO
 */
#define NANO_XRCE_REQUESTID_INVALID \
    { { 0x00, 0x00 }}


/** @} *//* nano_api_clientrequests */

/******************************************************************************
 *                          BaseObjectRequest
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_req_common
 * @{
 */

/*e
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_BaseObjectRequestI
{
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_RequestId request_id;
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_ObjectId object_id;
} NANO_XRCE_BaseObjectRequest;

#define NANO_XRCE_BASEOBJECTREQUEST_INITIALIZER \
{\
    NANO_XRCE_REQUESTID_INVALID, /*  request_id */\
    NANO_XRCE_OBJECTID_INVALID /* object_id */\
}

#define NANO_XRCE_BASEOBJECTREQUEST_SERIALIZED_SIZE_MAX \
    (sizeof(NANO_XRCE_RequestId) + sizeof(NANO_XRCE_ObjectId))

NANO_bool
NANO_XRCE_BaseObjectRequest_initialize(
    NANO_XRCE_BaseObjectRequest *const self);

#define NANO_XRCE_BaseObjectRequest_initialize(s_) \
    (NANO_XRCE_RequestId_initialize(&(s_)->request_id) && \
        NANO_XRCE_ObjectId_initialize(&(s_)->object_id))


/*e
 * @brief TODO
 * 
 */
typedef NANO_XRCE_BaseObjectRequest NANO_XRCE_RelatedObjectRequest;


#define NANO_XRCE_RELATEDOBJECTREQUEST_INITIALIZER \
    NANO_XRCE_BASEOBJECTREQUEST_INITIALIZER

NANO_bool
NANO_XRCE_RelatedObjectRequest_initialize(
    NANO_XRCE_RelatedObjectRequest *const self);

#define NANO_XRCE_RelatedObjectRequest_initialize(s_) \
    NANO_XRCE_BaseObjectRequest_initialize((s_))

#define NANO_XRCE_RELATEDOBJECTREQUEST_SERIALIZED_SIZE_MAX \
    NANO_XRCE_BASEOBJECTREQUEST_SERIALIZED_SIZE_MAX

/*i @} *//* nano_api_xrce_payload_req_common */

#endif /* NANO_FEAT_REQUEST */

#if NANO_FEAT_REPLY
/******************************************************************************
 *                          BaseObjectReply
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_req_common
 * @{
 */

/*e
 * @brief TODO
 * 
 * @param self 
 * @return NANO_bool 
 */
NANO_bool
NANO_XRCE_RelatedObjectRequest_is_valid(
    const NANO_XRCE_RelatedObjectRequest *const self);

#define NANO_XRCE_RelatedObjectRequest_is_valid(s_) \
    NANO_XRCE_BaseObjectRequest_is_valid((s_))

/*e
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_BaseObjectReplyI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_RelatedObjectRequest related_request;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_ResultStatus result;
} NANO_XRCE_BaseObjectReply;

#define NANO_XRCE_BASEOBJECTREPLY_INITIALIZER \
{\
    NANO_XRCE_RELATEDOBJECTREQUEST_INITIALIZER, /* related_request */\
    NANO_XRCE_RESULTSTATUS_INITIALIZER /* result */\
}

#define NANO_XRCE_BASEOBJECTREPLY_SERIALIZED_SIZE_MAX \
    (NANO_XRCE_RELATEDOBJECTREQUEST_SERIALIZED_SIZE_MAX + \
        NANO_XRCE_RESULTSTATUS_SERIALIZED_SIZE_MAX)

NANO_bool
NANO_XRCE_BaseObjectReply_initialize(
    NANO_XRCE_BaseObjectReply *const self);

#define NANO_XRCE_BaseObjectReply_initialize(s_) \
    (NANO_XRCE_RelatedObjectRequest_initialize(&(s_)->related_request) && \
        NANO_XRCE_ResultStatus_initialize(&(s_)->result))

/*i @} *//* nano_api_xrce_payload_req_common */


#endif /* NANO_FEAT_REPLY */

#if NANO_FEAT_PUBLISH || NANO_FEAT_SUBSCRIBE
/******************************************************************************
 *                             DataFormat
 ******************************************************************************/
/**
 * @addtogroup nano_api_clientrequests
 * @{
 */

/*e
 * @brief Identifier for the type of formatting of user data.
 */
typedef NANO_u8 NANO_XRCE_DataFormat;

NANO_bool
NANO_XRCE_DataFormat_initialize(
    NANO_XRCE_DataFormat *const self);

#define NANO_XRCE_DataFormat_initialize(s_) \
    (*(s_) = NANO_XRCE_FORMAT_INVALID,\
    NANO_BOOL_TRUE)

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_FORMAT_INVALID            ((NANO_XRCE_DataFormat)0xFF)

/*e
 * @brief Constant to indicate data format DATA.
 */
#define NANO_XRCE_FORMAT_DATA               ((NANO_XRCE_DataFormat)0x00)

/*e
 * @brief Constant to indicate data format SAMPLE.
 */
#define NANO_XRCE_FORMAT_SAMPLE             ((NANO_XRCE_DataFormat)0x02)

/*e
 * @brief Constant to indicate data format DATA_SEQ.
 */
#define NANO_XRCE_FORMAT_DATA_SEQ           ((NANO_XRCE_DataFormat)0x08)

/*e
 * @brief Constant to indicate data format SAMPLE_SEQ.
 */
#define NANO_XRCE_FORMAT_SAMPLE_SEQ         ((NANO_XRCE_DataFormat)0x0A)

/*e
 * @brief Constant to indicate data format PACKED_SAMPLE.
 */
#define NANO_XRCE_FORMAT_PACKED_SAMPLES     ((NANO_XRCE_DataFormat)0x0E)



/** @} *//* nano_api_clientrequests */

#endif /* NANO_FEAT_PUBLISH || NANO_FEAT_SUBSCRIBE */

#if NANO_FEAT_DELIVERY_CTRL
/******************************************************************************
 *                          DataDeliveryControl
 ******************************************************************************/
/**
 * @addtogroup nano_api_clientrequests
 * @{
 */

/*e
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_DataDeliveryControlI
{
    /*e
     * @brief TODO
     * 
     */
    NANO_u16 max_samples;
    /*e
     * @brief TODO
     * 
     */
    NANO_u16 max_elapsed_time;
    /*e
     * @brief TODO
     * 
     */
    NANO_u16 max_bytes_per_second;
    /*e
     * @brief TODO
     * 
     */
    NANO_u16 min_pace_period;


#if NANO_CPP

    NANO_XRCE_DataDeliveryControlI()
    {
        max_samples = 0;
        max_elapsed_time = 0;
        max_bytes_per_second = 0;
        min_pace_period = 0;
    }

#endif /* NANO_CPP */

} NANO_XRCE_DataDeliveryControl;

#define NANO_XRCE_DELIVERYCTRL_SERIALIZED_SIZE_MAX \
    (sizeof(NANO_u16) + \
    sizeof(NANO_u16) + \
    sizeof(NANO_u16) + \
    sizeof(NANO_u16))

#define NANO_XRCE_DATADELIVERYCONTROL_INITIALIZER \
{\
    0, /* max_samples */\
    0, /* max_elapsed_time */\
    0, /* max_bytes_per_second */\
    0  /* min_pace_period */\
}

NANO_bool
NANO_XRCE_DataDeliveryControl_initialize(
    NANO_XRCE_DataDeliveryControl *const self);

#define NANO_XRCE_DataDeliveryControl_initialize(s_) \
    ((s_)->max_samples = 0, \
    (s_)->max_elapsed_time = 0, \
    (s_)->max_bytes_per_second = 0, \
    (s_)->min_pace_period = 0, \
    NANO_BOOL_TRUE)


/** @} *//* nano_api_clientrequests */

#endif /* NANO_FEAT_DELIVERY_CTRL */

#if NANO_FEAT_SUBSCRIBE
/********************************************************************************
 *                              ReadSpecification
 ******************************************************************************/
/**
 * @addtogroup nano_api_clientrequests
 * @{
 */

/*e
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_ReadSpecificationI
{
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_StreamId preferred_stream_id;
    /*e
     * @brief TODO
     * 
     */
    NANO_XRCE_DataFormat data_format;
#if NANO_FEAT_CONTENT_FILTER && NANO_FEAT_OPTIONAL
    /*e
     * @brief TODO
     * 
     */
    NANO_OPTIONALMEMBER_DEFN(NANO_XRCE_ContentFilterExpr, content_filter_expr);
#else
    NANO_bool has_content_filter_expr;
#endif /* NANO_FEAT_CONTENT_FILTER && NANO_FEAT_OPTIONAL */
#if NANO_FEAT_DELIVERY_CTRL && NANO_FEAT_OPTIONAL
    /*e
     * @brief TODO
     * 
     */
    NANO_OPTIONALMEMBER_DEFN(NANO_XRCE_DataDeliveryControl, delivery_ctrl);
#else
    NANO_bool has_delivery_ctrl;
#endif /* NANO_FEAT_DELIVERY_CTRL */

#if NANO_CPP

    NANO_XRCE_ReadSpecificationI() :
        preferred_stream_id(NANO_XRCE_STREAMID_NONE),
        data_format(NANO_XRCE_FORMAT_INVALID)
#if NANO_FEAT_OPTIONAL
#if NANO_FEAT_CONTENT_FILTER
        ,
        NANO_OPTIONALMEMBER_INITIALIZER_CPP(content_filter_expr,)
#endif /* NANO_FEAT_CONTENT_FILTER */
#if NANO_FEAT_DELIVERY_CTRL
        ,
        NANO_OPTIONALMEMBER_INITIALIZER_CPP(delivery_ctrl,)
#endif /* NANO_FEAT_DELIVERY_CTRL */
#else
        ,
        has_content_filter_expr(NANO_BOOL_FALSE),
        has_delivery_ctrl(NANO_BOOL_FALSE)
#endif /* NANO_FEAT_OPTIONAL */
    {
        
    }

#endif /* NANO_CPP */

} NANO_XRCE_ReadSpecification;


#if NANO_FEAT_CONTENT_FILTER && NANO_FEAT_OPTIONAL
#define NANO_XRCE_READSPECIFICATION_CONTENTFILTEREXPR_SERIALIZED_SIZE_MAX \
    (sizeof(NANO_bool) + \
    NANO_XRCE_CONTENTFILTEREXPR_SERIALIZED_SIZE_MAX)
#define NANO_XRCE_READSPECIFICATION_CONTENTFILTEREXPR_INITIALIZER \
    NANO_OPTIONALMEMBER_INITIALIZER(\
            NANO_XRCE_CONTENTFILTEREXPR_INITIALIZER)
#else /* !(NANO_FEAT_CONTENT_FILTER && NANO_FEAT_OPTIONAL) */
#define NANO_XRCE_READSPECIFICATION_CONTENTFILTEREXPR_SERIALIZED_SIZE_MAX \
    sizeof(NANO_bool)
#define NANO_XRCE_READSPECIFICATION_CONTENTFILTEREXPR_INITIALIZER \
    NANO_BOOL_FALSE
#endif /* NANO_FEAT_CONTENT_FILTER && NANO_FEAT_OPTIONAL */

#if NANO_FEAT_DELIVERY_CTRL && NANO_FEAT_OPTIONAL
#define NANO_XRCE_READSPECIFICATION_DELIVERYCTRL_SERIALIZED_SIZED_MAX \
    (sizeof(NANO_bool) + \
    NANO_XRCE_DELIVERYCTRL_SERIALIZED_SIZE_MAX)
#define NANO_XRCE_READSPECIFICATION_DELIVERYCTRL_INITIALIZER \
    NANO_OPTIONALMEMBER_INITIALIZER(\
            NANO_XRCE_DATADELIVERYCONTROL_INITIALIZER)
#else /* !(NANO_FEAT_DELIVERY_CTRL && NANO_FEAT_OPTIONAL) */
#define NANO_XRCE_READSPECIFICATION_DELIVERYCTRL_SERIALIZED_SIZED_MAX \
    sizeof(NANO_bool)
#define NANO_XRCE_READSPECIFICATION_DELIVERYCTRL_INITIALIZER \
    NANO_BOOL_FALSE
#endif /* NANO_FEAT_DELIVERY_CTRL && NANO_FEAT_OPTIONAL */

#define NANO_XRCE_READSPECIFICATION_INITIALIZER \
{\
    NANO_XRCE_STREAMID_NONE, /* preferred_stream_id */\
    NANO_XRCE_FORMAT_INVALID, /* data_format */\
    NANO_XRCE_READSPECIFICATION_CONTENTFILTEREXPR_INITIALIZER, /* content_filter_expr */\
    NANO_XRCE_READSPECIFICATION_DELIVERYCTRL_INITIALIZER /* delivery_ctrl */\
}

#if NANO_FEAT_CONTENT_FILTER
/* Include content filter's length (and some padding) */
#define NANO_XRCE_READSPECIFICATION_HEADER_SERIALIZED_SIZED_MAX \
    NANO_OSAPI_Memory_align_size_up(\
        (sizeof(NANO_XRCE_StreamId) + \
            sizeof(NANO_XRCE_DataFormat) + \
            sizeof(NANO_bool) + \
            sizeof(NANO_u32)), sizeof(NANO_u32))
#else
#define NANO_XRCE_READSPECIFICATION_HEADER_SERIALIZED_SIZED_MAX \
    (sizeof(NANO_XRCE_StreamId) + \
        sizeof(NANO_XRCE_DataFormat) + \
        sizeof(NANO_bool) + \
        sizeof(NANO_bool))
#endif /* NANO_FEAT_CONTENT_FILTER */

#if NANO_FEAT_CONTENT_FILTER
/* if we have a content filter, we move the delivery ctrl's boolean
   to the tail payload */
#define NANO_XRCE_READSPECIFICATION_TAIL_SERIALIZED_SIZED_MAX \
    NANO_OSAPI_Memory_align_size_up(\
        (sizeof(NANO_bool) +\
        NANO_XRCE_DELIVERYCTRL_SERIALIZED_SIZE_MAX),sizeof(NANO_u32))
#else
#define NANO_XRCE_READSPECIFICATION_TAIL_SERIALIZED_SIZED_MAX \
    NANO_XRCE_DELIVERYCTRL_SERIALIZED_SIZE_MAX
#endif /* NANO_FEAT_CONTENT_FILTER */

NANO_bool
NANO_XRCE_ReadSpecification_initialize(
    NANO_XRCE_ReadSpecification *const self);

#define NANO_XRCE_ReadSpecification_initialize(s_) \
    (NANO_XRCE_StreamId_initialize(&(s_)->preferred_stream_id) && \
    NANO_XRCE_DataFormat_initialize(&(s_)->data_format) && \
    NANO_XRCE_ContentFilterExpr_initialize(\
        NANO_OPTIONALMEMBER_AS_REF((s_),content_filter_expr)) && \
    NANO_XRCE_DataDeliveryControl_initialize(\
        NANO_OPTIONALMEMBER_AS_REF((s_),delivery_ctrl)))

NANO_bool
NANO_XRCE_ReadSpecification_finalize(
    NANO_XRCE_ReadSpecification *const self);

#define NANO_XRCE_ReadSpecification_finalize(s_) \
    NANO_XRCE_ContentFilterExpr_finalize(\
        NANO_OPTIONALMEMBER_AS_REF((s_),content_filter_expr))

/** @} *//* nano_api_clientrequests */

#endif /* NANO_FEAT_SUBSCRIBE */

#if NANO_FEAT_SAMPLEINFO || NANO_FEAT_DATA_FORMAT_PACKED

/******************************************************************************
 *                              SampleInfoFlags
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_support_sampleinfoflags
 * @{
 */

/*i
 * @brief TODO
 * 
 */
typedef NANO_u8 NANO_XRCE_SampleInfoFlags;

NANO_bool
NANO_XRCE_SampleInfoFlags_initialize(
    NANO_XRCE_SampleInfoFlags *const self);

#define NANO_XRCE_SampleInfoFlags_initialize(s_) \
    (*(s_) = NANO_XRCE_SAMPLEINFOFLAGS_DEFAULT,\
        NANO_BOOL_TRUE)

/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_SAMPLEINFOFLAGS_DEFAULT           (0x00)

/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_SAMPLEINFOFLAGS_INSTANCE_STATE_UNREGISTERED \
    ((NANO_XRCE_SampleInfoFlags)0x01)

/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_SAMPLEINFOFLAGS_INSTANCE_STATE_DISPOSED \
    ((NANO_XRCE_SampleInfoFlags)0x01 << 1)

/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_SAMPLEINFOFLAGS_VIEW_STATE_NEW \
    ((NANO_XRCE_SampleInfoFlags)0x01 << 2)

/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_SAMPLEINFOFLAGS_SAMPLE_STATE_READ \
    ((NANO_XRCE_SampleInfoFlags)0x01 << 3)


/*i @} *//* nano_api_xrce_payload_support_sampleinfoflags */

#endif /* NANO_FEAT_SAMPLEINFO || NANO_FEAT_DATA_FORMAT_PACKED */

#if NANO_FEAT_SAMPLEINFO
/******************************************************************************
 *                              SampleInfoFormat
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_support_sampleinfofmt
 * @{
 */

/*i
 * @brief TODO
 * 
 */
typedef NANO_u8 NANO_XRCE_SampleInfoFormat;

NANO_bool
NANO_XRCE_SampleInfoFormat_initialize(
    NANO_XRCE_SampleInfoFormat *const self);

#define NANO_XRCE_SampleInfoFormat_initialize(s_) \
    (*(s_) = NANO_XRCE_SAMPLEINFOFORMAT_INVALID,\
        NANO_BOOL_TRUE)

/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_SAMPLEINFOFORMAT_INVALID \
    ((NANO_XRCE_SampleInfoFormat)0xFF)

/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_SAMPLEINFOFORMAT_FORMAT_EMPTY \
    ((NANO_XRCE_SampleInfoFormat)0x00)

/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_SAMPLEINFOFORMAT_FORMAT_SEQNUM \
    ((NANO_XRCE_SampleInfoFormat)0x01)

/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_SAMPLEINFOFORMAT_FORMAT_TIMESTAMP \
    ((NANO_XRCE_SampleInfoFormat)0x02)

/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_SAMPLEINFOFORMAT_FORMAT_SEQN_TIMS \
    ((NANO_XRCE_SampleInfoFormat)0x03)


/*i @} *//* nano_api_xrce_payload_support_sampleinfofmt */


/******************************************************************************
 *                              SeqNumAndTimestamp
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_support_seqnumandts
 * @{
 */

/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_SeqNumAndTimestampI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_u32 sequence_number;
    /*i
     * @brief TODO
     * 
     */
    NANO_u32 session_time_offset;
} NANO_XRCE_SeqNumAndTimestamp;

#define NANO_XRCE_SEQNUMANDTIMESTAMP_SERIALIZED_SIZE_MAX \
    (sizeof(NANO_u32) + sizeof(NANO_u32))

#define NANO_XRCE_SEQNUMANDTIMESTAMP_INITIALIZER \
{\
    0, /* sequence_number */\
    0 /* session_time_offset */\
}

NANO_bool
NANO_XRCE_SeqNumAndTimestamp_initialize(
    NANO_XRCE_SeqNumAndTimestamp *const self);

#define NANO_XRCE_SeqNumAndTimestamp_initialize(s_) \
    ((s_)->sequence_number = 0, \
        (s_)->session_time_offset = 0, \
            NANO_BOOL_TRUE)

/*i @} *//* nano_api_xrce_payload_support_seqnumandts */


/******************************************************************************
 *                              SampleInfoDetail
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_support_sampleinfodetail
 * @{
 */

/*i
 * @brief TODO
 * 
 */
typedef union NANODllExport NANO_XRCE_SampleInfoDetailValueI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_u32 sequence_number;
    /*i
     * @brief TODO
     * 
     */
    NANO_u32 session_time_offset;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_SeqNumAndTimestamp seqnum_n_timestamp;
} NANO_XRCE_SampleInfoDetailValue;

#define NANO_XRCE_SAMPLEINFODETAILVALUE_SERIALIZED_SIZE_MAX \
    NANO_XRCE_SEQNUMANDTIMESTAMP_SERIALIZED_SIZE_MAX

#define NANO_XRCE_SAMPLEINFODETAILVALUE_INITIALIZER \
{\
    0 /* sequence_number */ \
}

NANO_bool
NANO_XRCE_SampleInfoDetailValue_initialize(
    NANO_XRCE_SampleInfoDetailValue *const self);

#define NANO_XRCE_SampleInfoDetailValue_initialize(s_) \
    (NANO_OSAPI_Memory_zero((s_),sizeof(NANO_XRCE_SampleInfoDetailValue)),\
    NANO_BOOL_TRUE)

/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_SampleInfoDetailI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_SampleInfoFormat format;

    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_SampleInfoDetailValue value;
} NANO_XRCE_SampleInfoDetail;

#define NANO_XRCE_SAMPLEINFODETAIL_SERIALIZED_SIZE_MAX \
    NANO_OSAPI_Memory_align_size_up(\
        (sizeof(NANO_XRCE_SampleInfoFormat) + \
            NANO_XRCE_SAMPLEINFODETAILVALUE_SERIALIZED_SIZE_MAX),\
        sizeof(NANO_u32)) /* align size up to account for padding in serialization */


#define NANO_XRCE_SAMPLEINFODETAIL_INITIALIZER \
{\
    NANO_XRCE_SAMPLEINFOFORMAT_INVALID, /* format */\
    NANO_XRCE_SAMPLEINFODETAILVALUE_INITIALIZER /* value */\
}

NANO_bool
NANO_XRCE_SampleInfoDetail_initialize(
    NANO_XRCE_SampleInfoDetail *const self);

#define NANO_XRCE_SampleInfoDetail_initialize(s_) \
    (NANO_XRCE_SampleInfoFormat_initialize(&(s_)->format) && \
    NANO_XRCE_SampleInfoDetailValue_initialize(&(s_)->value))

/*i @} *//* nano_api_xrce_payload_support_sampleinfodetail */


/******************************************************************************
 *                                 SampleInfo
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_data_sampleinfo
 * @{
 */


/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_SampleInfoI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_SampleInfoFlags state;

    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_SampleInfoDetail detail;
} NANO_XRCE_SampleInfo;

#define NANO_XRCE_SAMPLEINFO_SERIALIZED_SIZE_MAX \
    (sizeof(NANO_XRCE_SampleInfoFlags) + \
    NANO_XRCE_SAMPLEINFODETAIL_SERIALIZED_SIZE_MAX)

#define NANO_XRCE_SAMPLEINFO_INITIALIZER \
{\
    NANO_XRCE_SAMPLEINFOFLAGS_DEFAULT, /* state */\
    NANO_XRCE_SAMPLEINFODETAIL_INITIALIZER /* detail */\
}

NANO_bool
NANO_XRCE_SampleInfo_initialize(
    NANO_XRCE_SampleInfo *const self);

#define NANO_XRCE_SampleInfo_initialize(s_) \
    (NANO_XRCE_SampleInfoFlags_initialize(&(s_)->state) && \
    NANO_XRCE_SampleInfoDetail_initialize(&(s_)->detail))

/*i @} *//* nano_api_xrce_payload_data_sampleinfo */

#endif /* NANO_FEAT_SAMPLEINFO */

#if NANO_FEAT_DATA_FORMAT_PACKED
/******************************************************************************
 *                              SampleInfoDelta
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_support_decisecond
 * @{
 */

/*i
 * @brief TODO
 * 
 */
typedef NANO_u16    NANO_XRCE_DeciSecond;

/*i @} *//* nano_api_xrce_payload_support_decisecond */

/*i
 * @addtogroup nano_api_xrce_payload_support_sampleinfodelta
 * @{
 */
/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_SampleInfoDeltaI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_SampleInfoFlags state;

    /*i
     * @brief TODO
     * 
     */
    NANO_u8 seq_number_delta;

    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_DeciSecond timestamp_delta;

} NANO_XRCE_SampleInfoDelta;

#define NANO_XRCE_SAMPLEINFODELTA_INITIALIZER \
{\
    NANO_XRCE_SAMPLEINFOFLAGS_DEFAULT, /* state */\
    0, /* seq_number_delta */\
    0 /* timestamp_delta */\
}

NANO_bool
NANO_XRCE_SampleInfoDelta_initialize(
    NANO_XRCE_SampleInfoDelta *const self);

#define NANO_XRCE_SampleInfoDelta_initialize(s_) \
    ((s_)->seq_number_delta = 0, \
    (s_)->timestamp_delta = 0, \
    NANO_XRCE_SampleInfoFlags_initialize(&(s_)->state))

/*i @} *//* nano_api_xrce_payload_support_sampleinfodelta */

#endif /* NANO_FEAT_DATA_FORMAT_PACKED */

#if NANO_FEAT_DATA_FORMAT_DATA
/******************************************************************************
 *                              SampleData
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_data_sampledata
 * @{
 */

/******************************************************************************
 *                              Data
 ******************************************************************************/

typedef struct NANODllExport NANO_XRCE_DataI
{
    NANO_u8 *value;
    NANO_usize value_len;
} NANO_XRCE_Data;

#define NANO_XRCE_DATA_INITIALIZER \
{\
    NULL, /* value */\
    0 /* value_len */\
}


/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_SampleDataI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_Data serialized_data;
} NANO_XRCE_SampleData;

#define NANO_XRCE_SAMPLEDATA_INITIALIZER \
{\
    NANO_XRCE_DATA_INITIALIZER /* serialized_data */\
}

NANO_bool
NANO_XRCE_SampleData_initialize(
    NANO_XRCE_SampleData *const self);

#define NANO_XRCE_SampleData_initialize(s_) \
    NANO_XRCE_Data_initialize(&(s_)->serialized_data)

void
NANO_XRCE_SampleData_finalize(
    NANO_XRCE_SampleData *const self);

#define NANO_XRCE_SampleData_finalize(s_) \
    NANO_XRCE_Data_finalize(&(s_)->serialized_data)


/*i @} *//* nano_api_xrce_payload_data_sampledata */
#endif /* NANO_FEAT_DATA_FORMAT_DATA */

#if NANO_FEAT_DATA_FORMAT_DATASEQ && NANO_FEAT_TYPED_SEQUENCE
/******************************************************************************
 *                          SampleDataSeq
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_data_sampledataseq
 * @{
 */

#define T           NANO_XRCE_SampleData
#define TSeq        NANO_XRCE_SampleDataSeq
#define TSeq_API    NANO_SEQUENCE_API_LEVEL_TYPED
#include "nano/nano_core_osapi_seq_decl.h"

#define NANO_XRCE_SAMPLEDATASEQ_INITIALIZER \
        NANO_TSEQIMPL_INITIALIZER(NANO_XRCE_SampleData)

#define NANO_XRCE_SampleDataSeq_initialize(s_) \
    NANO_TSeqImpl_initialize(s_, NANO_XRCE_SampleData)

#define NANO_XRCE_SampleDataSeq_set_contiguous_buffer(s_,b_,m_,l_) \
    NANO_TSeqImpl_set_contiguous_buffer(s_, b_, m_, l_, NANO_XRCE_SAMPLEDATA_INITIALIZER)

#define NANO_XRCE_SampleDataSeqBuffer_initialize(s_) \
    NANO_SADB_IMPL_INITIALIZE(NANO_XRCE_SampleDataSeqBuffer,(s_))

#define NANO_XRCE_SampleDataSeqBuffer_finalize(s_) \
    NANO_SADB_IMPL_FINALIZE(NANO_XRCE_SampleDataSeqBuffer,(s_))
    
#define NANO_XRCE_SampleDataSeq_initialized(s_) \
    NANO_TSeqImpl_initialized(s_)

#define NANO_XRCE_SampleDataSeq_finalize(s_) \
    NANO_TSeqImpl_finalize(s_, NANO_XRCE_SampleDataSeqBuffer)

#define NANO_XRCE_SampleDataSeq_as_seq(s_) \
    NANO_TSeqImpl_as_seq(s_)
    
#define NANO_XRCE_SampleDataSeq_as_seq_mut(s_) \
    NANO_TSeqImpl_as_seq_mut(s_)
    
#define NANO_XRCE_SampleDataSeq_reference(s_,i_) \
    NANO_TSeqImpl_reference(s_,i_, NANO_XRCE_SampleData)
    
#define NANO_XRCE_SampleDataSeq_reference_mut(s_,i_) \
    NANO_TSeqImpl_reference_mut(s_,i_, NANO_XRCE_SampleData)

#define NANO_XRCE_SampleDataSeq_contiguous_buffer(s_) \
    NANO_TSeqImpl_contiguous_buffer(s_, NANO_XRCE_SampleData)
    
#define NANO_XRCE_SampleDataSeq_contiguous_buffer_mut(s_) \
    NANO_TSeqImpl_contiguous_buffer_mut(s_, NANO_XRCE_SampleData)
    
#define NANO_XRCE_SampleDataSeq_maximum(s_) \
    NANO_TSeqImpl_maximum(s_)
    
#define NANO_XRCE_SampleDataSeq_length(s_) \
    NANO_TSeqImpl_length(s_)
    
#define NANO_XRCE_SampleDataSeq_set_length(s_, l_) \
    NANO_TSeqImpl_set_length(s_,l_)

#define NANO_XRCE_SampleDataSeq_copy(s_, f_) \
    NANO_TSeqImpl_copy(s_,f_)

/*i @} *//* nano_api_xrce_payload_data_sampledataseq */

#endif /* NANO_FEAT_DATA_FORMAT_DATASEQ && NANO_FEAT_TYPED_SEQUENCE */

#if NANO_FEAT_DATA_FORMAT_SAMPLE
/******************************************************************************
 *                               Sample
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_data_sample
 * @{
 */


/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_SampleI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_SampleInfo info;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_SampleData data;
} NANO_XRCE_Sample;

#define NANO_XRCE_SAMPLE_INITIALIZER \
{\
    NANO_XRCE_SAMPLEINFO_INITIALIZER, /* info */\
    NANO_XRCE_SAMPLEDATA_INITIALIZER /* data */\
}

NANO_bool
NANO_XRCE_Sample_initialize(
    NANO_XRCE_Sample *const self);

#define NANO_XRCE_Sample_initialize(s_) \
    (NANO_XRCE_SampleInfo_initialize((s_)->info) && \
    NANO_XRCE_SampleData_initialize((s_)->data))

void
NANO_XRCE_Sample_finalize(
    NANO_XRCE_Sample *const self);

#define NANO_XRCE_Sample_finalize(s_) \
    NANO_XRCE_SampleData_finalize(&(s_)->data)

/*i @} *//* nano_api_xrce_payload_data_sample */
#endif /* NANO_FEAT_DATA_FORMAT_SAMPLE */

#if NANO_FEAT_DATA_FORMAT_SAMPLESEQ && NANO_FEAT_TYPED_SEQUENCE
/******************************************************************************
 *                          SampleSeq
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_data_sampleseq
 * @{
 */

#define T           NANO_XRCE_Sample
#define TSeq        NANO_XRCE_SampleSeq
#define TSeq_API    NANO_SEQUENCE_API_LEVEL_TYPED

#include "nano/nano_core_osapi_seq_decl.h"

#define NANO_XRCE_SAMPLESEQ_INITIALIZER \
        NANO_TSEQIMPL_INITIALIZER(NANO_XRCE_Sample)

#define NANO_XRCE_SampleSeq_initialize(s_) \
    NANO_TSeqImpl_initialize(s_, NANO_XRCE_Sample)

#define NANO_XRCE_SampleSeq_set_contiguous_buffer(s_,b_,m_,l_) \
    NANO_TSeqImpl_set_contiguous_buffer(s_, b_, m_, l_, NANO_XRCE_SAMPLE_INITIALIZER)

#define NANO_XRCE_SampleSeq_finalize(s_) \
    NANO_TSeqImpl_finalize(s_)
    
#define NANO_XRCE_SampleSeq_initialized(s_) \
    NANO_TSeqImpl_initialized(s_)

#define NANO_XRCE_SampleSeq_as_seq(s_) \
    NANO_TSeqImpl_as_seq(s_)
    
#define NANO_XRCE_SampleSeq_as_seq_mut(s_) \
    NANO_TSeqImpl_as_seq_mut(s_)
    
#define NANO_XRCE_SampleSeq_reference(s_,i_) \
    NANO_TSeqImpl_reference(s_,i_, NANO_XRCE_Sample)
    
#define NANO_XRCE_SampleSeq_reference_mut(s_,i_) \
    NANO_TSeqImpl_reference_mut(s_,i_, NANO_XRCE_Sample)


#define NANO_XRCE_SampleSeq_contiguous_buffer(s_) \
    NANO_TSeqImpl_contiguous_buffer(s_, NANO_XRCE_Sample)
    
#define NANO_XRCE_SampleSeq_contiguous_buffer_mut(s_) \
    NANO_TSeqImpl_contiguous_buffer_mut(s_, NANO_XRCE_Sample)
    
#define NANO_XRCE_SampleSeq_maximum(s_) \
    NANO_TSeqImpl_maximum(s_)
    
#define NANO_XRCE_SampleSeq_length(s_) \
    NANO_TSeqImpl_length(s_)
    
#define NANO_XRCE_SampleSeq_set_length(s_, l_) \
    NANO_TSeqImpl_set_length(s_,l_)

#define NANO_XRCE_SampleSeq_copy(s_, f_) \
    NANO_TSeqImpl_copy(s_,f_)

/*i @} *//* nano_api_xrce_payload_data_sampleseq */

#endif /* NANO_FEAT_DATA_FORMAT_SAMPLESEQ && NANO_FEAT_TYPED_SEQUENCE */

#if NANO_FEAT_DATA_FORMAT_PACKED
/******************************************************************************
 *                              SampleDelta
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_support_sampledelta
 * @{
 */


/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_SampleDeltaI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_SampleInfoDelta info_delta;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_SampleData data;

} NANO_XRCE_SampleDelta;

#define NANO_XRCE_SAMPLEDELTA_INITIALIZER \
{\
    NANO_XRCE_SAMPLEINFODELTA_INITIALIZER, /* info_delta */\
    NANO_XRCE_SAMPLEDATA_INITIALIZER /* data */\
}

NANO_bool
NANO_XRCE_SampleDelta_initialize(
    NANO_XRCE_SampleDelta *const self);

#define NANO_XRCE_SampleDelta_initialize(s_) \
    (NANO_XRCE_SampleInfoDelta_intialize(&(s_)->info_delta) && \
    NANO_XRCE_SampleData_initialize(&(s_)->data))


void
NANO_XRCE_SampleDelta_finalize(
    NANO_XRCE_SampleDelta *const self);

#define NANO_XRCE_SampleDelta_finalize(s_) \
    NANO_XRCE_SampleData_finalize(&(s_)->data)

/*i @} *//* nano_api_xrce_payload_support_sampledelta */


#if  NANO_FEAT_TYPED_SEQUENCE
/******************************************************************************
 *                          SampleDeltaSeq
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_support_sampledeltaseq
 * @{
 */

#define T           NANO_XRCE_SampleDelta
#define TSeq        NANO_XRCE_SampleDeltaSeq
#define TSeq_API    NANO_SEQUENCE_API_LEVEL_TYPED

#include "nano/nano_core_osapi_seq_decl.h"

#define NANO_XRCE_SAMPLEDELTASEQ_INITIALIZER \
        NANO_TSEQIMPL_INITIALIZER(NANO_XRCE_SampleDelta)

#define NANO_XRCE_SampleDeltaSeq_initialize(s_) \
    NANO_TSeqImpl_initialize(s_, NANO_XRCE_SampleDelta)

#define NANO_XRCE_SampleDeltaSeq_set_contiguous_buffer(s_,b_,m_,l_) \
    NANO_TSeqImpl_set_contiguous_buffer(s_, b_, m_, l_, NANO_XRCE_SAMPLEDELTA_INITIALIZER)

#define NANO_XRCE_SampleDeltaSeq_finalize(s_) \
    NANO_TSeqImpl_finalize(s_)
    
#define NANO_XRCE_SampleDeltaSeq_initialized(s_) \
    NANO_TSeqImpl_initialized(s_)

#define NANO_XRCE_SampleDeltaSeq_as_seq(s_) \
    NANO_TSeqImpl_as_seq(s_)
    
#define NANO_XRCE_SampleDeltaSeq_as_seq_mut(s_) \
    NANO_TSeqImpl_as_seq_mut(s_)
    
#define NANO_XRCE_SampleDeltaSeq_reference(s_,i_) \
    NANO_TSeqImpl_reference(s_,i_, NANO_XRCE_SampleDelta)
    
#define NANO_XRCE_SampleDeltaSeq_reference_mut(s_,i_) \
    NANO_TSeqImpl_reference_mut(s_,i_, NANO_XRCE_SampleDelta)

#define NANO_XRCE_SampleDeltaSeq_contiguous_buffer(s_) \
    NANO_TSeqImpl_contiguous_buffer(s_, NANO_XRCE_SampleDelta)
    
#define NANO_XRCE_SampleDeltaSeq_contiguous_buffer_mut(s_) \
    NANO_TSeqImpl_contiguous_buffer_mut(s_, NANO_XRCE_SampleDelta)
    
#define NANO_XRCE_SampleDeltaSeq_maximum(s_) \
    NANO_TSeqImpl_maximum(s_)
    
#define NANO_XRCE_SampleDeltaSeq_length(s_) \
    NANO_TSeqImpl_length(s_)
    
#define NANO_XRCE_SampleDeltaSeq_set_length(s_, l_) \
    NANO_TSeqImpl_set_length(s_,l_)

#define NANO_XRCE_SampleDeltaSeq_copy(s_, f_) \
    NANO_TSeqImpl_copy(s_,f_)

#endif /* NANO_FEAT_TYPED_SEQUENCE */

/*i @} *//* nano_api_xrce_payload_support_sampledeltaseq */


/******************************************************************************
 *                              PackedSamples
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_data_packed
 * @{
 */



/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_PackedSamplesI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_SampleInfo info;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_SampleDeltaSeq sample_delta_seq;

} NANO_XRCE_PackedSamples;

#define NANO_XRCE_PACKEDSAMPLES_SERIALIZED_SIZE_MIN \
    NANO_OSAPI_Memory_align_size_up(\
        (NANO_XRCE_SAMPLEINFO_SERIALIZED_SIZE_MAX + \
        sizeof(NANO_u32)), sizeof(NANO_u32)) \
    /* padding should be zero, but align up anyway for robustness */

#define NANO_XRCE_PACKEDSAMPLES_INITIALIZER \
{\
    NANO_XRCE_SAMPLEINFO_INITIALIZER, /* info */ \
    NANO_XRCE_SAMPLEDELTASEQ_INITIALIZER /* sample_delta_seq */\
}

NANO_bool
NANO_XRCE_PackedSamples_initialize(
    NANO_XRCE_PackedSamples *const self);

#define NANO_XRCE_PackedSamples_initialize(s_) \
    (NANO_XRCE_SampleInfo_intialize(&(s_)->info) && \
    NANO_XRCE_SampleDeltaSeq_initialize(&(s_)->sample_delta_seq))


void
NANO_XRCE_PackedSamples_finalize(
    NANO_XRCE_PackedSamples *const self);

#define NANO_XRCE_PackedSamples_finalize(s_) \
    NANO_XRCE_SampleDeltaSeq_finalize(&(s_)->sample_delta_seq)



/*i @} *//* nano_api_xrce_payload_data_packed */

#endif /* NANO_FEAT_DATA_FORMAT_PACKED */


#if NANO_FEAT_DATA
/******************************************************************************
 *                              DataRepresentation
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_data
 * @{
 */


/*i
 * @brief TODO
 * 
 */
typedef union NANODllExport NANO_XRCE_DataRepresentationValueI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_SampleData data;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_Sample sample;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_SampleDataSeq data_seq;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_SampleSeq sample_seq;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_PackedSamples packed_samples;

} NANO_XRCE_DataRepresentationValue;

#define NANO_XRCE_DATAREPRESENTATIONVALUE_INITIALIZER \
{\
    NANO_XRCE_SAMPLEDATA_INITIALIZER /* data */\
}

NANO_bool
NANO_XRCE_DataRepresentationValue_initialize(
    NANO_XRCE_DataRepresentationValue *const self);

#define NANO_XRCE_DataRepresentationValue_initialize(s_) \
    (NANO_OSAPI_Memory_zero((s_),sizeof(NANO_XRCE_DataRepresentationValue)),\
    NANO_BOOL_TRUE)


/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_DataRepresentationI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_DataFormat format;

    /*i
     * @brief TODO
     * 
     */
    NANO_bool little_endian;

    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_DataRepresentationValue value;

} NANO_XRCE_DataRepresentation;

#define NANO_XRCE_DATAREPRESENTATION_INITIALIZER \
{\
    NANO_XRCE_FORMAT_INVALID, /* format */\
    NANO_BOOL_FALSE, /* little_endian */\
    NANO_XRCE_DATAREPRESENTATIONVALUE_INITIALIZER /* value */\
}

NANO_bool
NANO_XRCE_DataRepresentation_initialize(
    NANO_XRCE_DataRepresentation *const self);

#define NANO_XRCE_DataRepresentation_initialize(s_) \
    ((s_)->format = NANO_XRCE_FORMAT_INVALID, \
    NANO_XRCE_DataRepresentationValue_initialize(&(s_)->value))

void
NANO_XRCE_DataRepresentation_finalize(
    NANO_XRCE_DataRepresentation *const self);

#define NANO_XRCE_DataRepresentation_finalize(s_) \
{\
    switch ((s_)->format)\
    {\
    case NANO_XRCE_FORMAT_DATA:\
    {\
        NANO_XRCE_SampleData_finalize(&(s_)->value.data);\
        break\
    }\
    case NANO_XRCE_FORMAT_SAMPLE:\
    {\
        NANO_XRCE_Sample_finalize(&(s_)->value.sample);\
        break\
    }\
    case NANO_XRCE_FORMAT_DATA_SEQ:\
    {\
        NANO_XRCE_SampleDataSeq_finalize(&(s_)->value.data_seq);\
        break\
    }\
    case NANO_XRCE_FORMAT_SAMPLE_SEQ:\
    {\
        NANO_XRCE_SampleSeq_finalize(&(s_)->value.sample_seq);\
        break\
    }\
    case NANO_XRCE_FORMAT_PACKED_SAMPLES:\
    {\
        NANO_XRCE_PackedSamples_finalize(&(s_)->value.packed_samples);\
        break\
    }\
    default:\
    {\
        /* TODO nanolog */\
        break;\
    }\
    }\
}


/*i @} *//* nano_api_xrce_payload_data */

#endif /* NANO_FEAT_DATA */

/******************************************************************************
 *                              CREATE_CLIENT_Payload
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_req_createclient
 * @{
 */

/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_CreateClientPayloadI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_ClientRepresentation client_repr;

} NANO_XRCE_CreateClientPayload;

#define NANO_XRCE_CREATECLIENTPAYLOAD_HEADER_SERIALIZED_SIZE_MIN \
    NANO_XRCE_CLIENTREPRESENTATION_SERIALIZED_SIZE_MIN

#define NANO_XRCE_CREATECLIENTPAYLOAD_HEADER_SERIALIZED_SIZE_MAX \
    NANO_XRCE_CLIENTREPRESENTATION_SERIALIZED_SIZE_MAX

#define NANO_XRCE_CREATECLIENTPAYLOAD_INITIALIZER \
{\
    NANO_XRCE_CLIENTREPRESENTATION_INITIALIZER /* client_repr */\
}

NANO_bool
NANO_XRCE_CreateClientPayload_initialize(
    NANO_XRCE_CreateClientPayload *const self);

#define NANO_XRCE_CreateClientPayload_initialize(s_) \
    NANO_XRCE_ClientRepresentation_initialize(&(s_)->client_repr)

/*i
 * @brief TODO
 * 
 * @param self 
 * @return NANO_bool 
 */
NANO_bool
NANO_XRCE_CreateClientPayload_is_valid(
    const NANO_XRCE_CreateClientPayload *const self);

#define NANO_XRCE_CreateClientPayload_is_valid(s_) \
    NANO_XRCE_ClientRepresentation_is_valid((s_)->client_repr)

/*i @} *//* nano_api_xrce_payload_req_createclient */

#if NANO_FEAT_OBJECT_CREATE

/******************************************************************************
 *                              CREATE_Payload
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_req_create
 * @{
 */


/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_CreatePayloadI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_BaseObjectRequest base;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_ObjectVariant object_repr;

} NANO_XRCE_CreatePayload;

#define NANO_XRCE_CREATEPAYLOAD_INITIALIZER \
{\
    NANO_XRCE_BASEOBJECTREQUEST_INITIALIZER, /* base */\
    NANO_XRCE_OBJECTVARIANT_INITIALIZER /* object_repr */\
}

#define NANO_XRCE_CREATEPAYLOAD_HEADER_SERIALIZED_SIZE_MAX \
    NANO_XRCE_CREATEPAYLOAD_STRING_SERIALIZED_SIZE_MAX

/* The tail of a CREATE message contains either an ObjectId or a Domain Id,
   so 2 bytes. We round it up to 4 to account for possible padding */
#define NANO_XRCE_CREATEPAYLOAD_TAIL_SERIALIZED_SIZE_MAX \
    sizeof(NANO_u32)

#define NANO_XRCE_CREATEPAYLOAD_STRING_SERIALIZED_SIZE_MAX \
NANO_OSAPI_Memory_align_size_up(\
    (NANO_XRCE_BASEOBJECTREQUEST_SERIALIZED_SIZE_MAX + \
    sizeof(NANO_XRCE_ObjectKind) + \
    sizeof(NANO_XRCE_RepresentationFormat) +\
    sizeof(NANO_u32)), sizeof(NANO_u32)) \
    /* align size to account for required padding */

NANO_bool
NANO_XRCE_CreatePayload_initialize(
    NANO_XRCE_CreatePayload *const self);

#define NANO_XRCE_CreatePayload_initialize(s_) \
    (NANO_XRCE_BaseObjectRequest_initialize(&(s_)->base) && \
    NANO_XRCE_ObjectVariant_initialize(&(s_)->object_repr))


/*i @} *//* nano_api_xrce_payload_req_create */

#endif /* NANO_FEAT_OBJECT_CREATE */


#if NANO_FEAT_OBJECT_INFO
/******************************************************************************
 *                              GET_INFO_Payload
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_req_getinfo
 * @{
 */


/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_GetInfoPayloadI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_BaseObjectRequest base;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_InfoMask info_mask;

} NANO_XRCE_GetInfoPayload;


#define NANO_XRCE_GETINFOPAYLOAD_HEADER_SERIALIZED_SIZE_MAX \
    NANO_OSAPI_Memory_align_size_up(\
        (NANO_XRCE_BASEOBJECTREQUEST_SERIALIZED_SIZE_MAX + \
        sizeof(NANO_XRCE_InfoMask)), NANO_u32_alignment)

#define NANO_XRCE_GETINFOPAYLOAD_INITIALIZER \
{\
    NANO_XRCE_BASEOBJECTREQUEST_INITIALIZER, /* base */\
    NANO_XRCE_INFOMASK_DEFAULT /* info_mask */\
}

NANO_bool
NANO_XRCE_GetInfoPayload_initialize(
    NANO_XRCE_GetInfoPayload *const self);

#define NANO_XRCE_GetInfoPayload_initialize(s_) \
    (NANO_XRCE_BaseObjectRequest_initialize(&(s_)->base) && \
    NANO_XRCE_InfoMask_initialize(&(s_)->info_mask))

/*i @} *//* nano_api_xrce_payload_req_getinfo */

#endif /* NANO_FEAT_OBJECT_INFO */

#if NANO_FEAT_REQUEST
/******************************************************************************
 *                              DELETE_Payload
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_req_delete
 * @{
 */


/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_DeletePayloadI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_BaseObjectRequest base;

} NANO_XRCE_DeletePayload;

#define NANO_XRCE_DELETEPAYLOAD_INITIALIZER \
{\
    NANO_XRCE_BASEOBJECTREQUEST_INITIALIZER /* base */\
}

#define NANO_XRCE_DELETEPAYLOAD_HEADER_SERIALIZED_SIZE_MAX \
    NANO_XRCE_BASEOBJECTREQUEST_SERIALIZED_SIZE_MAX

NANO_bool
NANO_XRCE_DeletePayload_initialize(
    NANO_XRCE_DeletePayload *const self);

#define NANO_XRCE_DeletePayload_initialize(s_) \
    NANO_XRCE_BaseObjectRequest_initialize(&(s_)->base)

/*i @} *//* nano_api_xrce_payload_req_delete */

#endif /* NANO_FEAT_REQUEST */

/******************************************************************************
 *                              STATUS_AGENT_Payload
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_req_statusagent
 * @{
 */


/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_StatusAgentPayloadI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_AgentRepresentation agent_info;

} NANO_XRCE_StatusAgentPayload;

#define NANO_XRCE_STATUSAGENTPAYLOAD_HEADER_SERIALIZED_SIZE_MAX \
    NANO_XRCE_AGENTREPRESENTATION_SERIALIZED_SIZE_MIN


#define NANO_XRCE_STATUSAGENTPAYLOAD_INITIALIZER \
{\
    NANO_XRCE_AGENTREPRESENTATION_INITIALIZER /* agent_info */\
}

NANO_bool
NANO_XRCE_StatusAgentPayload_initialize(
    NANO_XRCE_StatusAgentPayload *const self);

#define NANO_XRCE_StatusAgentPayload_initialize(s_) \
    NANO_XRCE_AgentRepresentation_initialize(&(s_)->agent_info)

/*i @} *//* nano_api_xrce_payload_req_statusagent */

#if NANO_FEAT_REPLY

/******************************************************************************
 *                              STATUS_Payload
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_req_status
 * @{
 */


/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_StatusPayloadI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_BaseObjectReply base;

} NANO_XRCE_StatusPayload;

#define NANO_XRCE_STATUSPAYLOAD_INITIALIZER \
{\
    NANO_XRCE_BASEOBJECTREPLY_INITIALIZER /* base */\
}

#define NANO_XRCE_STATUSPAYLOAD_SERIALIZED_SIZE_MAX \
    NANO_XRCE_BASEOBJECTREPLY_SERIALIZED_SIZE_MAX

NANO_bool
NANO_XRCE_StatusPayload_initialize(
    NANO_XRCE_StatusPayload *const self);

#define NANO_XRCE_StatusPayload_initialize(s_) \
    NANO_XRCE_BaseObjectReply_initialize(&(s_)->base)

/*i @} *//* nano_api_xrce_payload_req_status */

#endif /* NANO_FEAT_REPLY */

#if NANO_FEAT_OBJECT_INFO
/******************************************************************************
 *                              INFO_Payload
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_req_info
 * @{
 */

/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_InfoPayloadI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_BaseObjectReply base;

    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_ObjectInfo object_info;

} NANO_XRCE_InfoPayload;

#define NANO_XRCE_INFOPAYLOAD_SERIALIZED_SIZE_MAX \
    (NANO_XRCE_BASEOBJECTREPLY_SERIALIZED_SIZE_MAX + \
    NANO_XRCE_OBJECTINFO_SERIALIZED_SIZE_MAX)

#define NANO_XRCE_INFOPAYLOAD_INITIALIZER \
{\
    NANO_XRCE_BASEOBJECTREPLY_INITIALIZER, /* base */\
    NANO_XRCE_OBJECTINFO_INITIALIZER /* object_info */\
}

NANO_bool
NANO_XRCE_InfoPayload_initialize(
    NANO_XRCE_InfoPayload *const self);

#define NANO_XRCE_InfoPayload_initialize(s_) \
    (NANO_XRCE_BaseObjectReply_initialize(&(s_)->base) &&\
    NANO_XRCE_ObjectInfo_initialize(&(s_)->object_info))



/*i @} *//* nano_api_xrce_payload_req_info */

#endif /* NANO_FEAT_OBJECT_INFO */

#if NANO_FEAT_SUBSCRIBE
/******************************************************************************
 *                              READ_DATA_Payload
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_req_readdata
 * @{
 */


/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_ReadDataPayloadI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_BaseObjectRequest base;

    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_ReadSpecification read_spec;

} NANO_XRCE_ReadDataPayload;

#define NANO_XRCE_READDATAPAYLOAD_INITIALIZER \
{\
    NANO_XRCE_BASEOBJECTREQUEST_INITIALIZER, /* base */\
    NANO_XRCE_READSPECIFICATION_INITIALIZER /* read_spec */\
}

#define NANO_XRCE_READDATAPAYLOAD_HEADER_SERIALIZED_SIZE_MAX \
    (NANO_XRCE_BASEOBJECTREQUEST_SERIALIZED_SIZE_MAX + \
    NANO_XRCE_READSPECIFICATION_HEADER_SERIALIZED_SIZED_MAX)

#define NANO_XRCE_READDATAPAYLOAD_TAIL_SERIALIZED_SIZE_MAX \
    NANO_XRCE_READSPECIFICATION_TAIL_SERIALIZED_SIZED_MAX

NANO_bool
NANO_XRCE_ReadDataPayload_initialize(
    NANO_XRCE_ReadDataPayload *const self);

#define NANO_XRCE_ReadDataPayload_initialize(s_) \
    (NANO_XRCE_BaseObjectRequest_initialize(&(s_)->base) && \
        NANO_XRCE_ReadSpecification_initialize(&(s_)->read_spec))


/*i
 * @brief TODO
 * 
 * @param self 
 * @return NANO_bool 
 */
NANO_bool
NANO_XRCE_ReadDataPayload_is_valid(
    const NANO_XRCE_ReadDataPayload *const self);

#define NANO_XRCE_ReadDataPayload_is_valid(s_) \
    (NANO_XRCE_BaseObjectRequest_is_valid(&(s_)->base) && \
    NANO_XRCE_ReadSpecification_is_valid(&(s_)->read_spec))

/*i @} *//* nano_api_xrce_payload_req_readdata */

#endif /* NANO_FEAT_SUBSCRIBE */

#if NANO_FEAT_PUBLISH
/******************************************************************************
 *                              WRITE_DATA_Payload
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_req_writedata
 * @{
 */

/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_WriteDataPayloadI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_BaseObjectRequest base;
#if NANO_FEAT_DATA
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_DataRepresentation data_repr;
#endif /* NANO_FEAT_DATA */

} NANO_XRCE_WriteDataPayload;

#if NANO_FEAT_DATA
#define NANO_XRCE_WRITEDATAPAYLOAD_REPR_INITIALIZER \
    ,NANO_XRCE_DATAREPRESENTATION_INITIALIZER
#else
#define NANO_XRCE_WRITEDATAPAYLOAD_REPR_INITIALIZER
#endif /* NANO_FEAT_DATA */

#if NANO_FEAT_DATA_FORMAT_DATASEQ || \
        NANO_FEAT_DATA_FORMAT_SAMPLESEQ
/* Include the sequence length too */
#define NANO_XRCE_WRITEDATAPAYLOAD_FORMAT_EXTRA_SERIALIZED_SIZE_SEQ \
    sizeof(NANO_u32)
#else
#define NANO_XRCE_WRITEDATAPAYLOAD_FORMAT_EXTRA_SERIALIZED_SIZE_SEQ \
    0
#endif
#if NANO_FEAT_DATA_FORMAT_SAMPLE
/* Include SampleInfo too */
#define NANO_XRCE_WRITEDATAPAYLOAD_FORMAT_EXTRA_SERIALIZED_SIZE_SAMPLE \
    NANO_XRCE_SAMPLEINFO_SERIALIZED_SIZE_MAX
#else
#define NANO_XRCE_WRITEDATAPAYLOAD_FORMAT_EXTRA_SERIALIZED_SIZE_SAMPLE \
    0
#endif
#if NANO_FEAT_DATA_FORMAT_PACKED
/* Include SampleInfo and sequence length too */
#define NANO_XRCE_WRITEDATAPAYLOAD_FORMAT_EXTRA_SERIALIZED_SIZE_PACKED \
    NANO_XRCE_PACKEDSAMPLES_SERIALIZED_SIZE_MIN
#else
#define NANO_XRCE_WRITEDATAPAYLOAD_FORMAT_EXTRA_SERIALIZED_SIZE_PACKED \
    0
#endif

#define NANO_XRCE_WRITEDATAPAYLOAD_FORMAT_EXTRA_SERIALIZED_SIZE \
    0

#define NANO_XRCE_WRITEDATAPAYLOAD_HEADER_SERIALIZED_SIZE_MAX \
    (NANO_XRCE_BASEOBJECTREQUEST_SERIALIZED_SIZE_MAX + \
    NANO_XRCE_WRITEDATAPAYLOAD_FORMAT_EXTRA_SERIALIZED_SIZE)

#define NANO_XRCE_WRITEDATAPAYLOAD_INITIALIZER \
{\
    NANO_XRCE_BASEOBJECTREQUEST_INITIALIZER /* base */\
    NANO_XRCE_WRITEDATAPAYLOAD_REPR_INITIALIZER /* data_repr */\
}

NANO_bool
NANO_XRCE_WriteDataPayload_initialize(
    NANO_XRCE_WriteDataPayload *const self);

#define NANO_XRCE_WriteDataPayload_initialize(s_) \
    (NANO_XRCE_BaseObjectRequest_initialize(&(s_)->base) && \
        NANO_XRCE_DataRepresentation_initialize(&(s_)->data_repr))


/*i @} *//* nano_api_xrce_payload_req_writedata */

#endif /* NANO_FEAT_PUBLISH */


#if NANO_FEAT_DATA
/******************************************************************************
 *                              DATA_Payload
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_req_data
 * @{
 */


/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_DataPayloadI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_RelatedObjectRequest base;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_DataRepresentation data_repr;

} NANO_XRCE_DataPayload;

#define NANO_XRCE_DATAPAYLOAD_INITIALIZER \
{\
    NANO_XRCE_RELATEDOBJECTREQUEST_INITIALIZER, /*base */ \
    NANO_XRCE_DATAREPRESENTATION_INITIALIZER /* data_repr */\
}

NANO_bool
NANO_XRCE_DataPayload_initialize(
    NANO_XRCE_DataPayload *const self);

#define NANO_XRCE_DataPayload_initialize(s_) \
    (NANO_XRCE_RelatedObjectRequest_initialize(&(s_)->base) && \
        NANO_XRCE_DataRepresentation_initialize(&(s_)->data_repr))


/*i @} *//* nano_api_xrce_payload_req_data */

#endif /* NANO_FEAT_DATA */

#if NANO_FEAT_RELIABILITY
/******************************************************************************
 *                              AckNackBitmap
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_acknackbmap
 * @{
 */

#define NANO_XRCE_ACKNACKBITMAP_VALUE_LEN               2

typedef struct NANODllExport NANO_XRCE_AckNackBitmapI
{
    NANO_u8 value[NANO_XRCE_ACKNACKBITMAP_VALUE_LEN];

#if NANO_CPP

    NANO_XRCE_AckNackBitmapI()
    {
        NANO_OSAPI_Memory_zero(this->value, sizeof(this->value));
    }

#endif /* NANO_CPP */
} NANO_XRCE_AckNackBitmap;

#define NANO_XRCE_ACKNACKBITMAP_INITIALIZER \
{\
    { 0xFF, 0xFF } \
}

#define NANO_XRCE_ACKNACKBITMAP_BITS \
    (NANO_XRCE_ACKNACKBITMAP_VALUE_LEN * 8)

NANO_bool
NANO_XRCE_AckNackBitmap_initialize(
    NANO_XRCE_AckNackBitmap *const self);

#define NANO_XRCE_AckNackBitmap_initialize(s_) \
    (NANO_XRCE_AckNackBitmap_reset(s_), NANO_BOOL_TRUE)

void
NANO_XRCE_AckNackBitmap_reset(NANO_XRCE_AckNackBitmap *const self);

#define NANO_XRCE_AckNackBitmap_reset(s_) \
    NANO_OSAPI_Memory_set(\
        (s_)->value, 0xFFFFFFFF, NANO_XRCE_ACKNACKBITMAP_VALUE_LEN)

/*i @} *//* nano_api_xrce_payload_support_acknackbmap */

/******************************************************************************
 *                              ACKNACK_Payload
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_req_acknack
 * @{
 */

/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_AckNackPayloadI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_u16 first_unacked_seq_num;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_AckNackBitmap nack_bitmap;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_StreamId stream_id;

} NANO_XRCE_AckNackPayload;

#define NANO_XRCE_ACKNACKPAYLOAD_INITIALIZER \
{\
    0, /* first_unacked_seq_num */\
    NANO_XRCE_ACKNACKBITMAP_INITIALIZER, /* nack_bitmap */\
    NANO_XRCE_STREAMID_NONE /* stream_id */\
}

#define NANO_XRCE_ACKNACKPAYLOAD_SERIALIZED_SIZE_MAX \
    (sizeof(NANO_u16) + \
     sizeof(NANO_XRCE_AckNackBitmap) + \
     sizeof(NANO_XRCE_StreamId))

NANO_bool
NANO_XRCE_AckNackPayload_initialize(
    NANO_XRCE_AckNackPayload *const self);

#define NANO_XRCE_AckNackPayload_initialize(s_) \
    ((s_)->first_unacked_seq_num = 0, \
    NANO_XRCE_AckNackBitmap_initialize(&(s_)->nack_bitmap), \
    NANO_XRCE_StreamId_initialize(&(s_)->stream_id))


/*i @} *//* nano_api_xrce_payload_req_acknack */

/******************************************************************************
 *                              HEARTBEAT_Payload
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_req_heartbeat
 * @{
 */


/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_HeartbeatPayloadI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_u16 first_unacked_seq_num;
    /*i
     * @brief TODO
     * 
     */
    NANO_u16 last_unacked_seq_num;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_StreamId stream_id;
    
} NANO_XRCE_HeartbeatPayload;

#define NANO_XRCE_HEARTBEATPAYLOAD_INITIALIZER \
{\
    0, /* first_unacked_seq_num */\
    0, /* last_unacked_seq_num */\
    NANO_XRCE_STREAMID_NONE /* stream_id */\
}

#define NANO_XRCE_HEARTBEATPAYLOAD_SERIALIZED_SIZE_MAX \
    (sizeof(NANO_u16) + \
    sizeof(NANO_u16) + \
    sizeof(NANO_XRCE_StreamId))

NANO_bool
NANO_XRCE_HeartbeatPayload_initialize(
    NANO_XRCE_HeartbeatPayload *const self);

#define NANO_XRCE_HeartbeatPayload_initialize(s_) \
    ((s_)->first_unacked_seq_num = 0, \
    (s_)->last_unacked_seq_num = 0, \
    NANO_XRCE_StreamId_initialize(&(s_)->stream_id))


/*i @} *//* nano_api_xrce_payload_req_heartbeat */

#endif /* NANO_FEAT_RELIABILITY */

#if NANO_FEAT_TIME_SYNC
/******************************************************************************
 *                              TIMESTAMP_Payload
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_req_timestamp
 * @{
 */


/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_TimestampPayloadI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_Time transmit_timestamp;
    
} NANO_XRCE_TimestampPayload;

#define NANO_XRCE_TIMESTAMPPAYLOAD_HEADER_SERIALIZED_SIZE_MAX \
    NANO_TIME_SERIALIZED_SIZE_MAX

#define NANO_XRCE_TIMESTAMPPAYLOAD_INITIALIZER \
{\
    NANO_TIME_INITIALIZER /* transit_timestamp */\
}

NANO_bool
NANO_XRCE_TimestampPayload_initialize(
    NANO_XRCE_TimestampPayload *const self);

#define NANO_XRCE_TimestampPayload_initialize(s_) \
    ((s_)->transmit_timestamp = 0, NANO_BOOL_TRUE)


/*i @} *//* nano_api_xrce_payload_req_timestamp */

/******************************************************************************
 *                              TIMESTAMP_REPLY_Payload
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_req_timestampreply
 * @{
 */


/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_TimestampReplyPayloadI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_Time transmit_timestamp;
    /*i
     * @brief TODO
     * 
     */
    NANO_Time receive_timestamp;
    /*i
     * @brief TODO
     * 
     */
    NANO_Time originate_timestamp;
    
} NANO_XRCE_TimestampReplyPayload;


#define NANO_XRCE_TIMESTAMPREPLYPAYLOAD_INITIALIZER \
{\
    NANO_TIME_INITIALIZER, /* transit_timestamp */\
    NANO_TIME_INITIALIZER, /* receive_timestamp */\
    NANO_TIME_INITIALIZER /* originate_timestamp */\
}

NANO_bool
NANO_XRCE_TimestampReplyPayload_initialize(
    NANO_XRCE_TimestampReplyPayload *const self);

#define NANO_XRCE_TimestampReplyPayload_initialize(s_) \
    ((s_)->transmit_timestamp = 0, \
    (s_)->receive_timestamp = 0, \
    (s_)->originate_timestamp = 0, \
    NANO_BOOL_TRUE)

/*i @} *//* nano_api_xrce_payload_req_timestampreply */

#endif /* NANO_FEAT_TIME_SYNC */

/******************************************************************************
 *                          Submessage Types
 ******************************************************************************/

/******************************************************************************
 *                             SubmessageId
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_submsgheader
 * @{
 */

/*i
 * @brief Identifier for different types of XRCE submessages.
 */
typedef NANO_u8 NANO_XRCE_SubmessageId;

/*i
 * @brief An invalid submessage id value.
 */
#define NANO_XRCE_SUBMESSAGEID_UNKNOWN \
    ((NANO_XRCE_SubmessageId)0xFF)

/*i
 * @brief Identifier for submessage CREATE_CLIENT.
 */
#define NANO_XRCE_SUBMESSAGEID_CREATE_CLIENT    ((NANO_XRCE_SubmessageId)0)
/*i
 * @brief Identifier for submessage CREATE.
 */
#define NANO_XRCE_SUBMESSAGEID_CREATE           ((NANO_XRCE_SubmessageId)1)
/*i
 * @brief Identifier for submessage GET_INFO.
 */
#define NANO_XRCE_SUBMESSAGEID_GET_INFO         ((NANO_XRCE_SubmessageId)2)
/*i
 * @brief Identifier for submessage DELETE.
 */
#define NANO_XRCE_SUBMESSAGEID_DELETE           ((NANO_XRCE_SubmessageId)3)
/*i
 * @brief Identifier for submessage STATUS_AGENT.
 */
#define NANO_XRCE_SUBMESSAGEID_STATUS_AGENT     ((NANO_XRCE_SubmessageId)4)
/*i
 * @brief Identifier for submessage STATUS.
 */
#define NANO_XRCE_SUBMESSAGEID_STATUS           ((NANO_XRCE_SubmessageId)5)
/*i
 * @brief Identifier for submessage INFO.
 */
#define NANO_XRCE_SUBMESSAGEID_INFO             ((NANO_XRCE_SubmessageId)6)
/*i
 * @brief Identifier for submessage WRITE_DATA.
 */
#define NANO_XRCE_SUBMESSAGEID_WRITE_DATA       ((NANO_XRCE_SubmessageId)7)
/*i
 * @brief Identifier for submessage READ_DATA.
 */
#define NANO_XRCE_SUBMESSAGEID_READ_DATA        ((NANO_XRCE_SubmessageId)8)
/*i
 * @brief Identifier for submessage DATA.
 */
#define NANO_XRCE_SUBMESSAGEID_DATA             ((NANO_XRCE_SubmessageId)9)
/*i
 * @brief Identifier for submessage ACKNACK.
 */
#define NANO_XRCE_SUBMESSAGEID_ACKNACK          ((NANO_XRCE_SubmessageId)10)
/*i
 * @brief Identifier for submessage HEARTBEAT.
 */
#define NANO_XRCE_SUBMESSAGEID_HEARTBEAT        ((NANO_XRCE_SubmessageId)11)
/*i
 * @brief Identifier for submessage RESET.
 */
#define NANO_XRCE_SUBMESSAGEID_RESET            ((NANO_XRCE_SubmessageId)12)
/*i
 * @brief Identifier for submessage FRAGMENT.
 */
#define NANO_XRCE_SUBMESSAGEID_FRAGMENT         ((NANO_XRCE_SubmessageId)13)
/*i
 * @brief Identifier for submessage TIMESTAMP.
 */
#define NANO_XRCE_SUBMESSAGEID_TIMESTAMP        ((NANO_XRCE_SubmessageId)14)
/*i
 * @brief Identifier for submessage TIMESTAMP_REPLY.
 */
#define NANO_XRCE_SUBMESSAGEID_TIMESTAMP_REPLY  ((NANO_XRCE_SubmessageId)15)

/*i
 * @brief Check if a NANO_XRCE_SubmessageId is one of the known constants.
 * 
 * @param self the value to check
 * @return NANO_BOOL_TRUE if the specified value is one of the known
 * constants, NANO_BOOL_FALSE otherwise.
 */
NANO_bool
NANO_XRCE_SubmessageId_is_valid(const NANO_XRCE_SubmessageId self);

#define NANO_XRCE_SubmessageId_is_valid(s_) \
   ((s_) == NANO_XRCE_SUBMESSAGEID_CREATE_CLIENT || \
    (s_) == NANO_XRCE_SUBMESSAGEID_CREATE || \
    (s_) == NANO_XRCE_SUBMESSAGEID_GET_INFO || \
    (s_) == NANO_XRCE_SUBMESSAGEID_DELETE || \
    (s_) == NANO_XRCE_SUBMESSAGEID_STATUS_AGENT || \
    (s_) == NANO_XRCE_SUBMESSAGEID_STATUS || \
    (s_) == NANO_XRCE_SUBMESSAGEID_INFO || \
    (s_) == NANO_XRCE_SUBMESSAGEID_WRITE_DATA || \
    (s_) == NANO_XRCE_SUBMESSAGEID_READ_DATA || \
    (s_) == NANO_XRCE_SUBMESSAGEID_DATA || \
    (s_) == NANO_XRCE_SUBMESSAGEID_ACKNACK || \
    (s_) == NANO_XRCE_SUBMESSAGEID_HEARTBEAT || \
    (s_) == NANO_XRCE_SUBMESSAGEID_RESET || \
    (s_) == NANO_XRCE_SUBMESSAGEID_FRAGMENT || \
    (s_) == NANO_XRCE_SUBMESSAGEID_TIMESTAMP || \
    (s_) == NANO_XRCE_SUBMESSAGEID_TIMESTAMP_REPLY)

const char*
NANO_XRCE_SubmessageId_as_str(const NANO_XRCE_SubmessageId self);

#define NANO_XRCE_SubmessageId_as_str(s_) \
    (((s_) == NANO_XRCE_SUBMESSAGEID_CREATE_CLIENT)?\
        "CREATE_CLIENT":\
    ((s_) == NANO_XRCE_SUBMESSAGEID_CREATE)?\
        "CREATE":\
    ((s_) == NANO_XRCE_SUBMESSAGEID_GET_INFO)?\
        "GET_INFO":\
    ((s_) == NANO_XRCE_SUBMESSAGEID_DELETE)?\
        "DELETE":\
    ((s_) == NANO_XRCE_SUBMESSAGEID_STATUS_AGENT)?\
        "STATUS_AGENT":\
    ((s_) == NANO_XRCE_SUBMESSAGEID_STATUS)?\
        "STATUS":\
    ((s_) == NANO_XRCE_SUBMESSAGEID_INFO)?\
        "INFO":\
    ((s_) == NANO_XRCE_SUBMESSAGEID_WRITE_DATA)?\
        "WRITE_DATA":\
    ((s_) == NANO_XRCE_SUBMESSAGEID_READ_DATA)?\
        "READ_DATA":\
    ((s_) == NANO_XRCE_SUBMESSAGEID_DATA)?\
        "DATA":\
    ((s_) == NANO_XRCE_SUBMESSAGEID_ACKNACK)?\
        "ACKNACK":\
    ((s_) == NANO_XRCE_SUBMESSAGEID_HEARTBEAT)?\
        "HEARTBEAT":\
    ((s_) == NANO_XRCE_SUBMESSAGEID_RESET)?\
        "RESET":\
    ((s_) == NANO_XRCE_SUBMESSAGEID_FRAGMENT)?\
        "FRAGMENT":\
    ((s_) == NANO_XRCE_SUBMESSAGEID_TIMESTAMP)?\
        "TIMESTAMP":\
    ((s_) == NANO_XRCE_SUBMESSAGEID_TIMESTAMP_REPLY)?\
        "TIMESTAMP_REPLY":\
        "UNKNOWN")

/*i @} *//* nano_api_xrce_submsgheader */

/******************************************************************************
 *                              SubmessageFlags
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_submsgheader
 * @{
 */

/*i
 * @brief Bitmask type to provide additional information about an XRCE
 * submessage.
 * 
 */
typedef NANO_u8 NANO_XRCE_SubmessageFlags;

#define NANO_XRCE_SUBMESSAGEFLAGS_INITIALIZER   0x00

#define NANO_XRCE_SUBMESSAGEFLAGS_ENDIANNESS    (0x01)

#define NANO_XRCE_SUBMESSAGEFLAGS_PIGGYBACKHB   (0x01 << 7)

#if NANO_ENDIAN_LITTLE
/*i
 * @brief Default flags for target architecture.
 */
#define NANO_XRCE_SUBMESSAGEFLAGS_DEFAULT   \
    (NANO_XRCE_SUBMESSAGEFLAGS_INITIALIZER | \
        NANO_XRCE_SUBMESSAGEFLAGS_ENDIANNESS)
#else /* NANO_ENDIAN_BIG */
#define NANO_XRCE_SUBMESSAGEFLAGS_DEFAULT \
    NANO_XRCE_SUBMESSAGEFLAGS_INITIALIZER
#endif


/*i
 * @brief Check if the associated XRCE submessage is encoded using little-endian
 * endianness.
 * 
 * @param self The flags to inspect
 * @return NANO_BOOL_TRUE if the submessages is encoded as little-endian,
 * NANO_BOOL_FALSE otherwise.
 */
NANO_bool
NANO_XRCE_SubmessageFlags_is_little_endian(NANO_XRCE_SubmessageFlags self);

#define NANO_XRCE_SubmessageFlags_is_little_endian(f_) \
    (((NANO_XRCE_SubmessageFlags)(f_)) & NANO_XRCE_SUBMESSAGEFLAGS_ENDIANNESS)

/*i
 * @brief Check if the associated XRCE submessage has an implied Heartbeat.
 * 
 * @param self The flags to inspect
 * @return NANO_BOOL_TRUE if the submessages has an implied Heartbeat,
 * NANO_BOOL_FALSE otherwise.
 */
NANO_bool
NANO_XRCE_SubmessageFlags_has_heartbeat(NANO_XRCE_SubmessageFlags self);

#define NANO_XRCE_SubmessageFlags_has_heartbeat(f_) \
    (((NANO_XRCE_SubmessageFlags)(f_)) & NANO_XRCE_SUBMESSAGEFLAGS_PIGGYBACKHB)

NANO_bool
NANO_XRCE_SubmessageFlags_needs_swap(NANO_XRCE_SubmessageFlags self);
#if NANO_ENDIAN_LITTLE
#define NANO_XRCE_SubmessageFlags_needs_swap(s_) \
    (!NANO_XRCE_SubmessageFlags_is_little_endian(s_))
#else
#define NANO_XRCE_SubmessageFlags_needs_swap(s_) \
    (NANO_XRCE_SubmessageFlags_is_little_endian(s_))
#endif /* NANO_ENDIAN_LITTLE */


#define NANO_XRCE_SUBMESSAGEFLAGS_FRAGMENT_LAST (0x01 << 1)
#define NANO_XRCE_SUBMESSAGEFLAGS_FRAGMENT_FIRST (0x01 << 2)

/*i
 * @brief TODO
 * 
 * @param self 
 * @return NANO_bool 
 */
NANO_bool
NANO_XRCE_SubmessageFlags_FRAGMENT_is_last(NANO_XRCE_SubmessageFlags self);

#define NANO_XRCE_SubmessageFlags_FRAGMENT_is_last(f_) \
    (((NANO_XRCE_SubmessageFlags)(f_)) & NANO_XRCE_SUBMESSAGEFLAGS_FRAGMENT_LAST)

#define NANO_XRCE_SubmessageFlags_FRAGMENT_is_first(f_) \
    (((NANO_XRCE_SubmessageFlags)(f_)) & NANO_XRCE_SUBMESSAGEFLAGS_FRAGMENT_FIRST)


#define NANO_XRCE_SUBMESSAGEFLAGS_CREATE_REUSE      (0x01 << 1)
#define NANO_XRCE_SUBMESSAGEFLAGS_CREATE_REPLACE    (0x01 << 2)

/*i
 * @brief Check if the associated CREATE submessage should be processed with
 * reuse semantics or not.
 * 
 * @param self The flags to inspect
 * @return NANO_BOOL_TRUE if the reuse flag is set, NANO_BOOL_FALSE otherwise.
 */
NANO_bool
NANO_XRCE_SubmessageFlags_CREATE_reuse(NANO_XRCE_SubmessageFlags self);

#define NANO_XRCE_SubmessageFlags_CREATE_reuse(f_) \
    (((NANO_XRCE_SubmessageFlags)(f_)) &\
            NANO_XRCE_SUBMESSAGEFLAGS_CREATE_REUSE)



/*i
 * @brief Check if the associated CREATE submessage should be processed with
 * replace semantics or not.
 * 
 * @param self The flags to inspect
 * @return NANO_BOOL_TRUE if the replace flag is set, NANO_BOOL_FALSE otherwise.
 */
NANO_bool
NANO_XRCE_SubmessageFlags_CREATE_replace(NANO_XRCE_SubmessageFlags self);

#define NANO_XRCE_SubmessageFlags_CREATE_replace(f_) \
    (((NANO_XRCE_SubmessageFlags)(f_)) &\
        NANO_XRCE_SUBMESSAGEFLAGS_CREATE_REPLACE)


NANO_u8
NANO_XRCE_SubmessageFlags_DATA_format_bits(
    NANO_XRCE_SubmessageFlags self);

#define NANO_XRCE_SubmessageFlags_DATA_format_bits(f_) \
    ((NANO_u8)((((NANO_XRCE_SubmessageFlags)(f_)) & 0x0E) >> 1))

/*i
 * @brief Retrieve the format of data contained in the associated WRITE_DATA
 * submessage
 * 
 * @param self The flags to inspect
 * @return A NANO_XRCE_DataFormat constant
 */
NANO_XRCE_DataFormat
NANO_XRCE_SubmessageFlags_DATA_format(
    NANO_XRCE_SubmessageFlags self);


#define NANO_XRCE_SubmessageFlags_DATA_format(f_) \
    ((NANO_XRCE_SubmessageFlags_DATA_format_bits(f_) == 0x00)?\
        NANO_XRCE_FORMAT_DATA : \
    (NANO_XRCE_SubmessageFlags_DATA_format_bits(f_) == 0x01)?\
        NANO_XRCE_FORMAT_SAMPLE : \
    (NANO_XRCE_SubmessageFlags_DATA_format_bits(f_) == 0x04)?\
        NANO_XRCE_FORMAT_DATA_SEQ : \
    (NANO_XRCE_SubmessageFlags_DATA_format_bits(f_) == 0x05)?\
        NANO_XRCE_FORMAT_SAMPLE_SEQ : \
    (NANO_XRCE_SubmessageFlags_DATA_format_bits(f_) == 0x07)?\
        NANO_XRCE_FORMAT_PACKED_SAMPLES : \
        NANO_XRCE_FORMAT_INVALID)


void
NANO_XRCE_SubmessageFlags_DATA_set_format(
    NANO_XRCE_SubmessageFlags *self,
    NANO_XRCE_DataFormat fmt,
    NANO_bool little_endian);

#define NANO_XRCE_SubmessageFlags_DATA_set_format(s_,f_,le_) \
{\
    if ((le_)) \
    {\
        *(s_) |= NANO_XRCE_SUBMESSAGEFLAGS_ENDIANNESS;\
    }\
    else\
    {\
        *(s_) &= ~NANO_XRCE_SUBMESSAGEFLAGS_ENDIANNESS;\
    }\
    *(s_) &= ~0x0E;\
    switch ((f_))\
    {\
    case NANO_XRCE_FORMAT_DATA:\
    {\
        break;\
    }\
    case NANO_XRCE_FORMAT_SAMPLE:\
    {\
        *(s_) |= 0x02;\
        break;\
    }\
    case NANO_XRCE_FORMAT_DATA_SEQ:\
    {\
        *(s_) |= 0x08;\
        break;\
    }\
    case NANO_XRCE_FORMAT_SAMPLE_SEQ:\
    {\
        *(s_) |= 0x0A;\
        break;\
    }\
    case NANO_XRCE_FORMAT_PACKED_SAMPLES:\
    {\
        *(s_) |= 0x0E;\
        break;\
    }\
    default:\
    {\
        NANO_PCOND(NANO_BOOL_FALSE)\
        break;\
    }\
    }\
}\

#define NANO_XRCE_SUBMESSAGEFLAGS_WRITEDATA_CONFIRM (0x01 << 4)

#define NANO_XRCE_SubmessageFlags_WRITEDATA_confirm(s_ ) \
    (((NANO_XRCE_SubmessageFlags)(s_)) & \
        NANO_XRCE_SUBMESSAGEFLAGS_WRITEDATA_CONFIRM)


#define NANO_XRCE_SUBMESSAGEFLAGS_READDATA_CONFIRM (0x01 << 1)

#define NANO_XRCE_SubmessageFlags_READDATA_confirm(s_ ) \
    (((NANO_XRCE_SubmessageFlags)(s_)) & \
        NANO_XRCE_SUBMESSAGEFLAGS_READDATA_CONFIRM)


/*i @} *//* nano_api_xrce_submsgheader */

/******************************************************************************
 *                              SubmessageHeader
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_submsgheader
 * @{
 */

/*i
 * @brief A type that can express the length of an XRCE submessage.
 * 
 * Currently, submessages are limited to 64KB, so they require at most
 * 16-bit to represent their length.
 * 
 */

#define NANO_XRCE_SUBMESSAGEHEADER_SERIALIZED_SIZE_MAX      (4)

/*i
 * @brief The header of all XRCE submessages.
 */
typedef struct NANODllExport NANO_XRCE_SubmessageHeaderI
{
    /*i
     * @brief Type of the associated submessage.
     */
    NANO_XRCE_SubmessageId id;
    /*i
     * @brief Flags of the associated submessage.
     * 
     */
    NANO_XRCE_SubmessageFlags flags;
    /*i
     * @brief Length of the associated submessage
     */
    NANO_u16 length;
} NANO_XRCE_SubmessageHeader;

/*i
 * @brief A static initializer for instances of NANO_XRCE_SubmessageHeader.
 */
#define NANO_XRCE_SUBMESSAGEHEADER_INITIALIZER \
{ \
    NANO_XRCE_SUBMESSAGEID_UNKNOWN, /* id */ \
    NANO_XRCE_SUBMESSAGEFLAGS_DEFAULT, /* flags */ \
    0, /* length */ \
}

/*i
 * @brief Check if a submessage's header is valid.
 * 
 * The header if valid if:
 * 
 * * The submessage id is valid.
 * * The length is as expected for the submessage id.
 * 
 * @param self The header to check
 * @return NANO_BOOL_TRUE if the header is valid, NANO_BOOL_FALSE otherwise.
 */
NANO_bool
NANO_XRCE_SubmessageHeader_is_valid(
    const NANO_XRCE_SubmessageHeader *const self);

#define NANO_XRCE_SubmessageHeader_is_valid(s_) \
    NANO_XRCE_SubmessageId_is_valid((s_)->id)


#define NANO_XRCE_Submessage_alignment      (4)

/*i @} *//* nano_api_xrce_submsgheader */

/******************************************************************************
 *                           MessageHeader
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_header
 * @{
 */

#define NANO_XRCE_MESSAGEHEADER_SERIALIZED_SIZE_MIN               (4)

#define NANO_XRCE_MESSAGEHEADER_SERIALIZED_SIZE_MAX               (8)

/*i
 * @brief The header of all XRCE messages.
 */
typedef struct NANODllExport NANO_XRCE_MessageHeaderI
{
    /*i
     * @brief Id of the session to which the message belongs.
     */
    NANO_XRCE_SessionId session_id;
    /*i
     * @brief Id of the stream to which the message belongs 
     * 
     */
    NANO_XRCE_StreamId stream_id;
    /*i
     * @brief Sequence number of the message.
     */
    NANO_XRCE_SeqNum sn;
    /*i
     * @brief The key of the client that send the message.
     */
    NANO_XRCE_ClientKey client_key;
} NANO_XRCE_MessageHeader;

/*i
 * @brief A static initializer for instances of NANO_XRCE_MessageHeader.
 */
#define NANO_XRCE_MESSAGEHEADER_INITIALIZER \
{ \
    NANO_XRCE_SESSIONID_NONE_WITH_CLIENT, /* session_id */ \
    NANO_XRCE_STREAMID_NONE, /* stream_id */ \
    NANO_XRCE_SEQNUM_INITIALIZER, /* sn */ \
    NANO_XRCE_CLIENTKEY_INVALID, /* client_key */ \
}

/*i
 * @brief Check if a header is valid.
 * 
 * The header if valid if:
 * 
 * * The session id indicates there is a client key, and the key is valid.
 * * The session id indicates there is no client key, and the key is invalid.
 * 
 * @param self The header to check
 * @return NANO_BOOL_TRUE if the header is valid, NANO_BOOL_FALSE otherwise.
 */
NANO_bool
NANO_XRCE_MessageHeader_is_valid(const NANO_XRCE_MessageHeader *const self);


#define NANO_XRCE_MessageHeader_is_valid(s_) \
    ((NANO_XRCE_SessionId_has_client_key((s_)->session_id))?\
        NANO_XRCE_ClientKey_is_valid(&(s_)->client_key) : \
        !NANO_XRCE_ClientKey_is_valid(&(s_)->client_key))


void
NANO_XRCE_MessageHeader_copy(
    NANO_XRCE_MessageHeader *const self,
    const NANO_XRCE_MessageHeader *const from);

#define NANO_XRCE_MessageHeader_copy(s_,f_) \
{\
    (s_)->session_id = (f_)->session_id;\
    (s_)->stream_id = (f_)->stream_id;\
    (s_)->sn = (f_)->sn;\
    if (NANO_XRCE_SessionId_has_client_key((s_)->session_id))\
    {\
        (s_)->client_key = (f_)->client_key;\
    }\
}

#define NANO_XRCE_MESSAGE_SERIALIZED_SIZE_MIN \
    (NANO_XRCE_MESSAGEHEADER_SERIALIZED_SIZE_MIN + \
        NANO_XRCE_SUBMESSAGEHEADER_SERIALIZED_SIZE_MAX)

/*i @} *//* nano_api_xrce_header */


#endif /* nano_core_xrce_message_h */
