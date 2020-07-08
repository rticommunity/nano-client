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

#ifndef nano_core_xrce_repr_h
#define nano_core_xrce_repr_h

#if NANO_FEAT_OBJECT
/******************************************************************************
 *                          RepresentationFormat
 ******************************************************************************/

/**
 * @addtogroup nano_api_obj
 * @{
 */

/*e
 * @brief TODO
 * 
 */
typedef NANO_u8 NANO_XRCE_RepresentationFormat;


/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_REPRESENTATION_INVALID \
    ((NANO_XRCE_RepresentationFormat)0x00)

/*e
 * @brief 
 * 
 */
#define NANO_XRCE_REPRESENTATION_BY_REFERENCE \
    ((NANO_XRCE_RepresentationFormat)0x01)

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_REPRESENTATION_AS_XML_STRING \
    ((NANO_XRCE_RepresentationFormat)0x02)

/*e
 * @brief TODO
 * 
 */
#define NANO_XRCE_REPRESENTATION_IN_BINARY \
    ((NANO_XRCE_RepresentationFormat)0x03)

NANO_bool
NANO_XRCE_RepresentationFormat_initialize(
    NANO_XRCE_RepresentationFormat *const self);

#define NANO_XRCE_RepresentationFormat_initialize(s_) \
    ((s_) = NANO_XRCE_REPRESENTATION_INVALID, NANO_BOOL_TRUE)


/*e
 * @brief TODO
 * 
 * @param self 
 * @return NANO_bool 
 */
NANO_bool
NANO_XRCE_RepresentationFormat_is_valid(
    const NANO_XRCE_RepresentationFormat *const self);

#define NANO_XRCE_RepresentationFormat_is_valid(s_) \
   ((s_) == NANO_XRCE_REPRESENTATION_BY_REFERENCE || \
    (s_) == NANO_XRCE_REPRESENTATION_AS_XML_STRING || \
    (s_) == NANO_XRCE_REPRESENTATION_IN_BINARY)


/** @} *//* nano_api_obj */

/******************************************************************************
 *                          RefRepresentationFormat
 ******************************************************************************/

/*i
 * @addtogroup  nano_api_xrce_payload_objrepr_common
 * @{
 */
/*i
 * @brief TODO
 * 
 */
typedef NANO_XRCE_ObjectRef NANO_XRCE_RefRepresentationFormat;

#define NANO_XRCE_REFREPRESENTATIONFORMAT_INITIALIZER \
    NANO_XRCE_OBJECTREF_INITIALIZER

NANO_bool
NANO_XRCE_RefRepresentationFormat_initialize(
    NANO_XRCE_RefRepresentationFormat *const self);

#define NANO_XRCE_RefRepresentationFormat_initialize(s_) \
    NANO_XRCE_ObjectRef_initialize((s_))

void
NANO_XRCE_RefRepresentationFormat_finalize(
    NANO_XRCE_RefRepresentationFormat *const self);

#define NANO_XRCE_RefRepresentationFormat_finalize(s_) \
    NANO_XRCE_ObjectRef_finalize(s_))


/*i @} *//* nano_api_xrce_payload_objrepr_common */

/******************************************************************************
 *                          XmlRepresentationFormat
 ******************************************************************************/

/*i
 * @addtogroup  nano_api_xrce_payload_objrepr_common
 * @{
 */

typedef struct NANODllExport NANO_XRCE_XmlRepresentationFormat_initialize
{
    char *value;
} NANO_XRCE_XmlRepresentationFormat;

#define NANO_XRCE_XMLREPRESENTATIONFORMAT_INITIALIZER \
{\
    NULL /* value */\
}

NANO_bool
NANO_XRCE_XmlRepresentationFormat_is_valid(
    const NANO_XRCE_XmlRepresentationFormat *const self);

#define NANO_XRCE_XmlRepresentationFormat_is_valid(s_) \
   ((s_)->value != NULL && \
        NANO_OSAPI_String_length((s_)->value) > 0 && \
        NANO_OSAPI_String_length((s_)->value) <= \
                NANO_LIMIT_XMLREPRESENTATION_MAX_LENGTH)

/*i @} *//* nano_api_xrce_payload_objrepr_common */

/******************************************************************************
 *                              BinData
 ******************************************************************************/

/*i
 * @addtogroup  nano_api_xrce_payload_objrepr_common
 * @{
 */

#if NANO_FEAT_TYPED_SEQUENCE
#define T           NANO_u8
#define TSeq        NANO_XRCE_BinData
#define TSeq_API    NANO_SEQUENCE_API_LEVEL_TYPED

#include "nano/nano_core_osapi_seq_decl.h"

#define NANO_XRCE_BINDATA_INITIALIZER \
        NANO_TSEQIMPL_INITIALIZER(NANO_u8)

#define NANO_XRCE_BinData_initialize(s_) \
    NANO_TSeqImpl_initialize(s_)

#define NANO_XRCE_BinData_set_contiguous_buffer(s_,b_,m_,l_) \
    NANO_TSeqImpl_set_contiguous_buffer_primitive(s_, b_, m_, l_)

#define NANO_XRCE_BinData_finalize(s_) \
    NANO_TSeqImpl_finalize(s_)
    
#define NANO_XRCE_BinData_initialized(s_) \
    NANO_TSeqImpl_initialized(s_)

#define NANO_XRCE_BinData_as_seq(s_) \
    NANO_TSeqImpl_as_seq(s_)
    
#define NANO_XRCE_BinData_as_seq_mut(s_) \
    NANO_TSeqImpl_as_seq_mut(s_)
    
#define NANO_XRCE_BinData_reference(s_,i_) \
    NANO_TSeqImpl_reference(s_,i_, NANO_u8)
    
#define NANO_XRCE_BinData_reference_mut(s_,i_) \
    NANO_TSeqImpl_reference_mut(s_,i_, NANO_u8)

#define NANO_XRCE_BinData_contiguous_buffer(s_) \
    NANO_TSeqImpl_contiguous_buffer(s_, NANO_u8)
    
#define NANO_XRCE_BinData_contiguous_buffer_mut(s_) \
    NANO_TSeqImpl_contiguous_buffer_mut(s_, NANO_u8)
    
#define NANO_XRCE_BinData_maximum(s_) \
    NANO_TSeqImpl_maximum(s_)
    
#define NANO_XRCE_BinData_length(s_) \
    NANO_TSeqImpl_length(s_)
    
#define NANO_XRCE_BinData_set_length(s_, l_) \
    NANO_TSeqImpl_set_length(s_,l_)

#endif /* NANO_FEAT_TYPED_SEQUENCE */

/******************************************************************************
 *                          BinRepresentationFormat
 ******************************************************************************/

#if NANO_FEAT_TYPED_SEQUENCE
/*i
 * @brief TODO
 * 
 */
typedef NANO_XRCE_BinData NANO_XRCE_BinRepresentationFormat;

#define NANO_XRCE_BINREPRESENTATIONFORMAT_INITIALIZER \
    NANO_XRCE_BINDATA_INITIALIZER

NANO_bool
NANO_XRCE_BinRepresentationFormat_initialize(
    NANO_XRCE_BinRepresentationFormat *const self);

#define NANO_XRCE_BinRepresentationFormat_initialize(s_) \
    NANO_XRCE_BinData_initialize((s_))


void
NANO_XRCE_BinRepresentationFormat_finalize(
    NANO_XRCE_BinRepresentationFormat *const self);

#define NANO_XRCE_BinRepresentationFormat_finalize(s_) \
    NANO_XRCE_BinData_finalize((s_))


#else

typedef struct NANODllExport NANO_XRCE_BinRepresentationFormatI
{
    NANO_u8 *value;
    NANO_u16 value_len;
} NANO_XRCE_BinRepresentationFormat;

#define NANO_XRCE_BINREPRESENTATIONFORMAT_INITIALIZER \
{\
    NULL, /* value */ \
    0 /* value_len */ \
}

NANO_bool
NANO_XRCE_BinRepresentationFormat_is_valid(
    const NANO_XRCE_BinRepresentationFormat *const self);

#define NANO_XRCE_BinRepresentationFormat_is_valid(s_) \
    ((s_)->value != NULL && (s_)->value_len > 0)

#endif /* NANO_FEAT_TYPED_SEQUENCE */

/*i @} *//* nano_api_xrce_payload_objrepr_common */


/******************************************************************************
 *                          Representation3
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_common
 * @{
 */

/*i
 * @brief TODO
 * 
 */
typedef union NANODllExport NANO_XRCE_Representation3FormatsValueI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_RefRepresentationFormat ref;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_BinRepresentationFormat bin;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_XmlRepresentationFormat xml;
} NANO_XRCE_Representation3FormatsValue;

#define NANO_XRCE_REPRESENTATION3FORMATSVALUE_INITIALIZER \
{\
    NANO_XRCE_REFREPRESENTATIONFORMAT_INITIALIZER /* ref */\
}

NANO_bool
NANO_XRCE_Representation3FormatsValue_initialize(
    NANO_XRCE_Representation3FormatsValue *const self);

#define NANO_XRCE_Representation3FormatsValue_initialize(s_) \
    (NANO_OSAPI_Memory_zero(\
        (s_),sizeof(NANO_XRCE_Representation3FormatsValue)),\
    NANO_BOOL_TRUE)

/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_Representation3FormatsI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_RepresentationFormat format;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_Representation3FormatsValue value;
} NANO_XRCE_Representation3Formats;

#define NANO_XRCE_REPRESENTATION3FORMATS_INITIALIZER \
{\
    NANO_XRCE_REPRESENTATION_INVALID, /* format */\
    NANO_XRCE_REPRESENTATION3FORMATSVALUE_INITIALIZER /* value */\
}

/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_Representation3I
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_Representation3Formats repr;
} NANO_XRCE_Representation3;

#define NANO_XRCE_REPRESENTATION3_INITIALIZER \
{\
    NANO_XRCE_REPRESENTATION3FORMATS_INITIALIZER /* repr */\
}


/*i @} *//* nano_api_xrce_payload_objrepr_common */

/******************************************************************************
 *                          RepresentationRefAndXml
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_common
 * @{
 */

/*i
 * @brief TODO
 * 
 */
typedef union NANODllExport NANO_XRCE_RepresentationRefAndXmlFormatsValueI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_RefRepresentationFormat ref;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_XmlRepresentationFormat xml;
} NANO_XRCE_RepresentationRefAndXmlFormatsValue;

#define NANO_XRCE_REPRESENTATIONREFANDXMLFORMATSVALUE_INITIALIZER \
{\
    NANO_XRCE_REFREPRESENTATIONFORMAT_INITIALIZER /* ref */\
}

/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_RepresentationRefAndXmlFormatsI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_RepresentationFormat format;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_RepresentationRefAndXmlFormatsValue value;
} NANO_XRCE_RepresentationRefAndXmlFormats;

#define NANO_XRCE_REPRESENTATIONREFANDXMLFORMATS_INITIALIZER \
{\
    NANO_XRCE_REPRESENTATION_INVALID, /* format */\
    NANO_XRCE_REPRESENTATIONREFANDXMLFORMATSVALUE_INITIALIZER /* value */\
}


/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_RepresentationRefAndXmlI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_RepresentationRefAndXmlFormats repr;
} NANO_XRCE_RepresentationRefAndXml;

#define NANO_XRCE_REPRESENTATIONREFANDXML_INITIALIZER \
{\
    NANO_XRCE_REPRESENTATIONREFANDXMLFORMATS_INITIALIZER /* repr */\
}

/*i
 * @brief TODO
 * 
 * @param self 
 * @return NANO_bool 
 */
NANO_bool
NANO_XRCE_RepresentationRefAndXml_is_valid(
    const NANO_XRCE_RepresentationRefAndXml *const self);

#define NANO_XRCE_RepresentationRefAndXml_is_valid(s_) \
   NANO_XRCE_RepresentationRefAndXmlFormats_is_valid(&(s_)->repr)


/*i
 * @brief Return
 * 
 * @param self 
 * @return NANO_XRCE_ObjectRef* 
 */
const NANO_XRCE_ObjectRef*
NANO_XRCE_RepresentationRefAndXml_as_ref(
    const NANO_XRCE_RepresentationRefAndXml *const self);

#define NANO_XRCE_RepresentationRefAndXml_as_ref(s_) \
    NANO_XRCE_RepresentationRefAndXmlFormats_as_ref(&(s_)->repr)


/*i
 * @brief TODO
 * 
 * @param self 
 * @return const char** 
 */
const char *const *
NANO_XRCE_RepresentationRefAndXml_as_xml(
    const NANO_XRCE_RepresentationRefAndXml *const self);

#define NANO_XRCE_RepresentationRefAndXml_as_xml(s_) \
    NANO_XRCE_RepresentationRefAndXmlFormats_as_xml(&(s_)->repr)


/*i
 * @brief Return
 * 
 * @param self 
 * @return NANO_XRCE_ObjectRef* 
 */
NANO_XRCE_ObjectRef*
NANO_XRCE_RepresentationRefAndXml_as_ref_mut(
    NANO_XRCE_RepresentationRefAndXml *const self);

#define NANO_XRCE_RepresentationRefAndXml_as_ref_mut(s_) \
    NANO_XRCE_RepresentationRefAndXmlFormats_as_ref_mut(&(s_)->repr)


/*i
 * @brief TODO
 * 
 * @param self 
 * @return const char** 
 */
const char**
NANO_XRCE_RepresentationRefAndXml_as_xml_mut(
    NANO_XRCE_RepresentationRefAndXml *const self);

#define NANO_XRCE_RepresentationRefAndXml_as_xml_mut(s_) \
    NANO_XRCE_RepresentationRefAndXmlFormats_as_xml_mut(&(s_)->repr)

/*i @} *//* nano_api_xrce_payload_objrepr_common */

/******************************************************************************
 *                         RepresentationBinAndXml
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_common
 * @{
 */

/*i
 * @brief TODO
 * 
 */
typedef union NANODllExport NANO_XRCE_RepresentationBinAndXmlFormatsValueI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_BinRepresentationFormat bin;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_XmlRepresentationFormat xml;

#if NANO_FEAT_ALL_BY_REF
    NANO_XRCE_RefRepresentationFormat ref;
#endif /* NANO_FEAT_ALL_BY_REF */
} NANO_XRCE_RepresentationBinAndXmlFormatsValue;

#define NANO_XRCE_REPRESENTATIONBINANDXMLFORMATSVALUE_INITIALIZER \
{\
    NANO_XRCE_BINREPRESENTATIONFORMAT_INITIALIZER /* bin */\
}

/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_RepresentationBinAndXmlFormatsI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_RepresentationFormat format;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_RepresentationBinAndXmlFormatsValue value;
} NANO_XRCE_RepresentationBinAndXmlFormats;

#define NANO_XRCE_REPRESENTATIONBINANDXMLFORMATS_INITIALIZER \
{\
    NANO_XRCE_REPRESENTATION_INVALID, /* format */\
    NANO_XRCE_REPRESENTATIONBINANDXMLFORMATSVALUE_INITIALIZER /* value */\
}

/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_RepresentationBinAndXmlI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_RepresentationBinAndXmlFormats repr;
} NANO_XRCE_RepresentationBinAndXml;

#define NANO_XRCE_REPRESENTATIONBINANDXML_INITIALIZER \
{\
    NANO_XRCE_REPRESENTATIONBINANDXMLFORMATS_INITIALIZER /* repr */\
}

/*i @} *//* nano_api_xrce_payload_objrepr_common */

#endif /* NANO_FEAT_OBJECT */


#if NANO_FEAT_OBJECT_KIND_QOSPROFILE
/******************************************************************************
 *                          QOSPROFILE_Representation
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_objrepr_qosprofile
 * @{
 */

/*i
 * @brief TODO
 */
typedef struct NANODllExport NANO_XRCE_QosProfileRepresentationI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_RepresentationRefAndXml base;
} NANO_XRCE_QosProfileRepresentation;

#define NANO_XRCE_QOSPROFILEREPRESENTATION_INITIALIZER \
{\
    NANO_XRCE_REPRESENTATIONREFANDXML_INITIALIZER /* base */\
}

/*i @} *//* nano_api_xrce_payload_objrepr_qosprofile */

#endif /* NANO_FEAT_OBJECT_KIND_QOSPROFILE */

/******************************************************************************
 *                            TYPE_Representation
 ******************************************************************************/
#if NANO_FEAT_OBJECT_KIND_TYPE
/*i
 * @addtogroup nano_api_xrce_payload_objrepr_type
 * @{
 */

/*i
 * @brief TODO
 */
typedef struct NANODllExport NANO_XRCE_TypeRepresentationI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_RepresentationRefAndXml base;
} NANO_XRCE_TypeRepresentation;


#define NANO_XRCE_TYPEREPRESENTATION_INITIALIZER \
{\
    NANO_XRCE_REPRESENTATIONREFANDXML_INITIALIZER /* base */\
}

/*i @} *//* nano_api_xrce_payload_objrepr_type */
#endif /* NANO_FEAT_OBJECT_KIND_TYPE */

#if NANO_FEAT_OBJECT_KIND_DOMAIN
/******************************************************************************
 *                            DOMAIN_Representation
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_objrepr_domain
 * @{
 */

/*i
 * @brief TODO
 */
typedef struct NANODllExport NANO_XRCE_DomainRepresentationI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_RepresentationRefAndXml base;
} NANO_XRCE_DomainRepresentation;

#define NANO_XRCE_DOMAINREPRESENTATION_INITIALIZER \
{\
    NANO_XRCE_REPRESENTATIONREFANDXML_INITIALIZER /* base */\
}

/*i @} *//* nano_api_xrce_payload_objrepr_domain */
#endif /* NANO_FEAT_OBJECT_KIND_DOMAIN */

#if NANO_FEAT_OBJECT_KIND_APPLICATION
/******************************************************************************
 *                            APPLICATION_Representation
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_objrepr_app
 * @{
 */

/*i
 * @brief TODO
 */
typedef struct NANODllExport NANO_XRCE_ApplicationRepresentationI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_RepresentationRefAndXml base;
} NANO_XRCE_ApplicationRepresentation;

#define NANO_XRCE_APPLICATIONREPRESENTATION_INITIALIZER \
{\
    NANO_XRCE_REPRESENTATIONREFANDXML_INITIALIZER /* base */\
}


/*i @} *//* nano_api_xrce_payload_objrepr_app */

#endif /* NANO_FEAT_OBJECT_KIND_APPLICATION */

#if NANO_FEAT_OBJECT_KIND_PUBLISHER || \
    NANO_FEAT_OBJECT_KIND_SUBSCRIBER || \
    NANO_FEAT_OBJECT_KIND_TOPIC || \
    NANO_FEAT_OBJECT_KIND_DATAWRITER || \
    NANO_FEAT_OBJECT_KIND_DATAREADER

/******************************************************************************
 *                         ContainedBinAndXmlObject
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_objrepr_common
 * @{
 */

/*i
 * @brief TODO
 */
typedef struct NANODllExport NANO_XRCE_ContainedBinAndXmlObjectRepresentationI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_RepresentationBinAndXml base;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_ObjectId parent_id;

} NANO_XRCE_ContainedBinAndXmlObjectRepresentation;

#define NANO_XRCE_CONTAINEDBINANDXMLOBJECTREPRESENTATION_INITIALIZER \
{\
    NANO_XRCE_REPRESENTATIONBINANDXML_INITIALIZER, /* base */\
    NANO_XRCE_OBJECTID_INVALID /* parent_id */\
}

NANO_bool
NANO_XRCE_ContainedBinAndXmlObjectRepresentation_initialize(
    NANO_XRCE_ContainedBinAndXmlObjectRepresentation *const self);

#define NANO_XRCE_ContainedBinAndXmlObjectRepresentation_initialize(s_) \
    (NANO_XRCE_ObjectId_initialize(&(s_)->parent_id) && \
    NANO_XRCE_RepresentationBinAndXml_initialize(&(s_)->base))

void
NANO_XRCE_ContainedBinAndXmlObjectRepresentation_finalize(
    NANO_XRCE_ContainedBinAndXmlObjectRepresentation *const self);

#define NANO_XRCE_ContainedBinAndXmlObjectRepresentation_finalize(s_) \
    NANO_XRCE_RepresentationBinAndXml_finalize(&(s_)->base)


/*i @} *//* nano_api_xrce_payload_objrepr_common */

#endif /* NANO_FEAT_OBJECT_KIND_PUBLISHER || \
          NANO_FEAT_OBJECT_KIND_SUBSCRIBER || \
          NANO_FEAT_OBJECT_KIND_TOPIC || \
          NANO_FEAT_OBJECT_KIND_DATAWRITER || \
          NANO_FEAT_OBJECT_KIND_DATAREADER */

#if NANO_FEAT_OBJECT_KIND_PUBLISHER || \
    NANO_FEAT_OBJECT_KIND_SUBSCRIBER

/******************************************************************************
 *                              EntityName
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_support_entityname
 * @{
 */

typedef struct NANODllExport NANO_XRCE_EntityNameI
{
    char *value;
} NANO_XRCE_EntityName;

#define NANO_XRCE_ENTITYNAME_INITIALIZER \
{\
    NULL /* value */\
}
/*i @} *//* nano_api_xrce_payload_support_entityname */

/******************************************************************************
 *                              Partition
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_support_partition
 * @{
 */
typedef struct NANODllExport NANO_XRCE_PartitionI
{
    char *value;
} NANO_XRCE_Partition;

#define NANO_XRCE_PARTITION_INITIALIZER \
{\
    NULL /* value */\
}

/******************************************************************************
 *                              PartitionSeq
 ******************************************************************************/
#if NANO_FEAT_TYPED_SEQUENCE
#define T           NANO_XRCE_Partition
#define TSeq        NANO_XRCE_PartitionSeq
#define TSeq_API    NANO_SEQUENCE_API_LEVEL_TYPED

#include "nano/nano_core_osapi_seq_decl.h"

#define NANO_XRCE_PARTITIONSEQ_INITIALIZER \
        NANO_TSEQIMPL_INITIALIZER(NANO_XRCE_Partition)

#define NANO_XRCE_PartitionSeq_initialize(s_) \
    NANO_TSeqImpl_initialize(s_)

#define NANO_XRCE_PartitionSeq_set_contiguous_buffer(s_,b_,m_,l_) \
    NANO_TSeqImpl_set_contiguous_buffer(s_, b_, m_, l_, NANO_XRCE_PARTITION_INITIALIZER)

#define NANO_XRCE_PartitionSeq_finalize(s_) \
    NANO_TSeqImpl_finalize(s_)

#define NANO_XRCE_PartitionSeq_initialized(s_) \
    NANO_TSeqImpl_initialized(s_)

#define NANO_XRCE_PartitionSeq_as_seq(s_) \
    NANO_TSeqImpl_as_seq(s_)
    
#define NANO_XRCE_PartitionSeq_as_seq_mut(s_) \
    NANO_TSeqImpl_as_seq_mut(s_)
    
#define NANO_XRCE_PartitionSeq_reference(s_,i_) \
    NANO_TSeqImpl_reference(s_,i_, NANO_XRCE_Partition)
    
#define NANO_XRCE_PartitionSeq_reference_mut(s_,i_) \
    NANO_TSeqImpl_reference_mut(s_,i_, NANO_XRCE_Partition)

#define NANO_XRCE_PartitionSeq_contiguous_buffer(s_) \
    NANO_TSeqImpl_contiguous_buffer(s_, NANO_XRCE_Partition)
    
#define NANO_XRCE_PartitionSeq_contiguous_buffer_mut(s_) \
    NANO_TSeqImpl_contiguous_buffer_mut(s_, NANO_XRCE_Partition)
    
#define NANO_XRCE_PartitionSeq_maximum(s_) \
    NANO_TSeqImpl_maximum(s_)
    
#define NANO_XRCE_PartitionSeq_length(s_) \
    NANO_TSeqImpl_length(s_)
    
#define NANO_XRCE_PartitionSeq_set_length(s_, l_) \
    NANO_TSeqImpl_set_length(s_,l_)

#endif /* NANO_FEAT_TYPED_SEQUENCE */

/*i @} *//* nano_api_xrce_payload_support_partition */


/******************************************************************************
 *                              GroupData
 ******************************************************************************/
#if NANO_FEAT_TYPED_SEQUENCE
/*i
 * @addtogroup nano_api_xrce_payload_support_groupdata
 * @{
 */

#define T           NANO_u8
#define TSeq        NANO_XRCE_GroupData
#define TSeq_API    NANO_SEQUENCE_API_LEVEL_TYPED

#include "nano/nano_core_osapi_seq_decl.h"

#define NANO_XRCE_GROUPDATA_INITIALIZER \
        NANO_TSEQIMPL_INITIALIZER(NANO_u8)

#define NANO_XRCE_GroupData_initialize(s_) \
    NANO_TSeqImpl_initialize(s_)

#define NANO_XRCE_GroupData_set_contiguous_buffer(s_,b_,m_,l_) \
    NANO_TSeqImpl_set_contiguous_buffer_primitive(s_, b_, m_, l_)

#define NANO_XRCE_GroupData_finalize(s_) \
    NANO_TSeqImpl_finalize(s_)
    
#define NANO_XRCE_GroupData_initialized(s_) \
    NANO_TSeqImpl_initialized(s_)

#define NANO_XRCE_GroupData_as_seq(s_) \
    NANO_TSeqImpl_as_seq(s_)
    
#define NANO_XRCE_GroupData_as_seq_mut(s_) \
    NANO_TSeqImpl_as_seq_mut(s_)
    
#define NANO_XRCE_GroupData_reference(s_,i_) \
    NANO_TSeqImpl_reference(s_,i_, NANO_u8)
    
#define NANO_XRCE_GroupData_reference_mut(s_,i_) \
    NANO_TSeqImpl_reference_mut(s_,i_, NANO_u8)

#define NANO_XRCE_GroupData_contiguous_buffer(s_) \
    NANO_TSeqImpl_contiguous_buffer(s_, NANO_u8)
    
#define NANO_XRCE_GroupData_contiguous_buffer_mut(s_) \
    NANO_TSeqImpl_contiguous_buffer_mut(s_, NANO_u8)
    
#define NANO_XRCE_GroupData_maximum(s_) \
    NANO_TSeqImpl_maximum(s_)
    
#define NANO_XRCE_GroupData_length(s_) \
    NANO_TSeqImpl_length(s_)
    
#define NANO_XRCE_GroupData_set_length(s_, l_) \
    NANO_TSeqImpl_set_length(s_,l_)

#endif /* NANO_FEAT_TYPED_SEQUENCE */

#endif /* NANO_FEAT_OBJECT_KIND_PUBLISHER || \
          NANO_FEAT_OBJECT_KIND_SUBSCRIBER */

/*i @} *//* nano_api_xrce_payload_support_groupdata */


#if NANO_FEAT_OBJECT_KIND_DATAWRITER || \
    NANO_FEAT_OBJECT_KIND_DATAREADER
/******************************************************************************
 *                              UserData
 ******************************************************************************/
#if NANO_FEAT_TYPED_SEQUENCE
/*i
 * @addtogroup nano_api_xrce_payload_support_userdata
 * @{
 */

#define T           NANO_u8
#define TSeq        NANO_XRCE_UserData
#define TSeq_API    NANO_SEQUENCE_API_LEVEL_TYPED

#include "nano/nano_core_osapi_seq_decl.h"

#define NANO_XRCE_USERDATA_INITIALIZER \
        NANO_TSEQIMPL_INITIALIZER(NANO_u8)

#define NANO_XRCE_UserData_initialize(s_) \
    NANO_TSeqImpl_initialize(s_)

#define NANO_XRCE_UserData_set_contiguous_buffer(s_,b_,m_,l_) \
    NANO_TSeqImpl_set_contiguous_buffer_primitive(s_, b_, m_, l_)

#define NANO_XRCE_UserData_finalize(s_) \
    NANO_TSeqImpl_finalize(s_)
    
#define NANO_XRCE_UserData_initialized(s_) \
    NANO_TSeqImpl_initialized(s_)

#define NANO_XRCE_UserData_as_seq(s_) \
    NANO_TSeqImpl_as_seq(s_)
    
#define NANO_XRCE_UserData_as_seq_mut(s_) \
    NANO_TSeqImpl_as_seq_mut(s_)
    
#define NANO_XRCE_UserData_reference(s_,i_) \
    NANO_TSeqImpl_reference(s_,i_, NANO_u8)
    
#define NANO_XRCE_UserData_reference_mut(s_,i_) \
    NANO_TSeqImpl_reference_mut(s_,i_, NANO_u8)

#define NANO_XRCE_UserData_contiguous_buffer(s_) \
    NANO_TSeqImpl_contiguous_buffer(s_, NANO_u8)
    
#define NANO_XRCE_UserData_contiguous_buffer_mut(s_) \
    NANO_TSeqImpl_contiguous_buffer_mut(s_, NANO_u8)
    
#define NANO_XRCE_UserData_maximum(s_) \
    NANO_TSeqImpl_maximum(s_)
    
#define NANO_XRCE_UserData_length(s_) \
    NANO_TSeqImpl_length(s_)
    
#define NANO_XRCE_UserData_set_length(s_, l_) \
    NANO_TSeqImpl_set_length(s_,l_)

#endif /* NANO_FEAT_TYPED_SEQUENCE */

/*i @} *//* nano_api_xrce_payload_support_userdata */


#endif /* NANO_FEAT_OBJECT_KIND_DATAWRITER || \
          NANO_FEAT_OBJECT_KIND_DATAREADER */

#if NANO_FEAT_OBJECT_KIND_PUBLISHER || \
    NANO_FEAT_OBJECT_KIND_SUBSCRIBER

/******************************************************************************
 *                          EndpointContainerQosBinary
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_containerqos
 * @{
 */

/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_EndpointContainerQosBinaryI
{
#if NANO_FEAT_OPTIONAL && NANO_FEAT_TYPED_SEQUENCE
    /*i
     * @brief TODO
     * 
     */
    NANO_OPTIONALMEMBER_DEFN(NANO_XRCE_PartitionSeq, partitions);
    /*i
     * @brief TODO
     * 
     */
    NANO_OPTIONALMEMBER_DEFN(NANO_XRCE_GroupData, group_data);
#else
    const NANO_bool has_partition;

    const NANO_bool has_group_data;

#endif /* NANO_FEAT_OPTIONAL */
} NANO_XRCE_EndpointContainerQosBinary;

#if NANO_FEAT_OPTIONAL && NANO_FEAT_TYPED_SEQUENCE
#define NANO_XRCE_ENDPOINTCONTAINERQOSBINARY_OPTIONAL_INITIALIZER \
    NANO_OPTIONALMEMBER_INITIALIZER(\
        NANO_XRCE_PARTITIONSEQ_INITIALIZER), /* partitions */\
    NANO_OPTIONALMEMBER_INITIALIZER(\
        NANO_XRCE_GROUPDATA_INITIALIZER) /* group_data */
#else
#define NANO_XRCE_ENDPOINTCONTAINERQOSBINARY_OPTIONAL_INITIALIZER \
    NANO_BOOL_FALSE, /* has_partition */\
    NANO_BOOL_FALSE, /* has_group_data */
#endif /* NANO_FEAT_OPTIONAL */
#define NANO_XRCE_ENDPOINTCONTAINERQOSBINARY_INITIALIZER \
{\
    NANO_XRCE_ENDPOINTCONTAINERQOSBINARY_OPTIONAL_INITIALIZER \
}

#if NANO_FEAT_OPTIONAL
NANO_bool
NANO_XRCE_EndpointContainerQosBinary_initialize(
    NANO_XRCE_EndpointContainerQosBinary *const self);

#define NANO_XRCE_EndpointContainerQosBinary_initialize(s_) \
    NANO_XRCE_PartitionSeq_initialize(\
        NANO_OPTIONALMEMBER_AS_REF((s_),partitions)) && \
    NANO_XRCE_GroupData_initialize(\
        NANO_OPTIONALMEMBER_AS_REF((s_),group_data))

void
NANO_XRCE_EndpointContainerQosBinary_finalize(
    NANO_XRCE_EndpointContainerQosBinary *const self);

#define NANO_XRCE_EndpointContainerQosBinary_finalize(s_) \
{\
    NANO_XRCE_PartitionSeq_finalize(\
        NANO_OPTIONALMEMBER_AS_REF((s_),partitions));\
    NANO_XRCE_GroupData_finalize(\
        NANO_OPTIONALMEMBER_AS_REF((s_),group_data));\
}

#endif /* NANO_FEAT_OPTIONAL */


/*i @} *//* nano_api_xrce_payload_support_containerqos */

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_common
 * @{
 */
/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_EndpointContainerBinaryI
{
#if NANO_FEAT_OPTIONAL
    /*i
     * @brief TODO
     * 
     */
    NANO_OPTIONALMEMBER_DEFN(NANO_XRCE_EntityName, entity_name);
    /*i
     * @brief TODO
     * 
     */
    NANO_OPTIONALMEMBER_DEFN(NANO_XRCE_EndpointContainerQosBinary, qos);

#else
    const NANO_bool has_entity_name;

    const NANO_bool has_qos;

#endif /* NANO_FEAT_OPTIONAL */
} NANO_XRCE_EndpointContainerBinary;


#if NANO_FEAT_OPTIONAL
#define NANO_XRCE_ENDPOINTCONTAINERBINARY_OPTIONAL_INITIALIZER \
    NANO_OPTIONALMEMBER_INITIALIZER(\
        NANO_XRCE_ENTITYNAME_INITIALIZER), /* entity_name */\
    NANO_OPTIONALMEMBER_INITIALIZER(\
        NANO_XRCE_ENDPOINTCONTAINERQOSBINARY_INITIALIZER) /* qos */
#else
#define NANO_XRCE_ENDPOINTCONTAINERBINARY_OPTIONAL_INITIALIZER \
    NANO_BOOL_FALSE, /* has_entity_name */\
    NANO_BOOL_FALSE, /* has_qos */
#endif /* NANO_FEAT_OPTIONAL */


#define NANO_XRCE_ENDPOINTCONTAINERBINARY_INITIALIZER \
{\
    NANO_XRCE_ENDPOINTCONTAINERBINARY_OPTIONAL_INITIALIZER \
}

#if NANO_FEAT_OPTIONAL
NANO_bool
NANO_XRCE_EndpointContainerBinary_initialize(
    NANO_XRCE_EndpointContainerBinary *const self);

#define NANO_XRCE_EndpointContainerBinary_initialize(s_) \
    NANO_XRCE_EntityName_initialize(\
        NANO_OPTIONALMEMBER_AS_REF((s_),entity_name)) && \
    NANO_XRCE_EndpointContainerQosBinary_initialize(\
        NANO_OPTIONALMEMBER_AS_REF((s_),qos))

void
NANO_XRCE_EndpointContainerBinary_finalize(
    NANO_XRCE_EndpointContainerBinary *const self);

#define NANO_XRCE_EndpointContainerBinary_finalize(s_) \
{\
    NANO_XRCE_EntityName_finalize(\
        NANO_OPTIONALMEMBER_AS_REF((s_),entity_name));\
    NANO_XRCE_EndpointContainerQosBinary_finalize(\
        NANO_OPTIONALMEMBER_AS_REF((s_),qos));\
}

#endif /* NANO_FEAT_OPTIONAL */

/*i @} *//* nano_api_xrce_payload_objrepr_common */

#endif /* NANO_FEAT_OBJECT_KIND_PUBLISHER || \
          NANO_FEAT_OBJECT_KIND_SUBSCRIBER */

#if NANO_FEAT_OBJECT_KIND_PUBLISHER
/******************************************************************************
 *                            PUBLISHER_Representation
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_objrepr_pub
 * @{
 */

/*i
 * @brief TODO
 */
typedef NANO_XRCE_ContainedBinAndXmlObjectRepresentation
    NANO_XRCE_PublisherRepresentation;

#define NANO_XRCE_PUBLISHERREPRESENTATION_INITIALIZER \
    NANO_XRCE_CONTAINEDBINANDXMLOBJECTREPRESENTATION_INITIALIZER

/*i @} *//* nano_api_xrce_payload_objrepr_pub */

/*i
 * @addtogroup nano_api_xrce_payload_support_pubqos
 * @{
 */
/*i
 * @brief TODO
 * 
 */
typedef NANO_XRCE_EndpointContainerQosBinary NANO_XRCE_PublisherQosBinary;

#define NANO_XRCE_PUBLISHERQOSBINARY_INITIALIZER \
    NANO_XRCE_ENDPOINTCONTAINERQOSBINARY_INITIALIZER

NANO_bool
NANO_XRCE_PublisherQosBinary_initialize(
    NANO_XRCE_PublisherQosBinary *const self);

#define NANO_XRCE_PublisherQosBinary_initialize(s_) \
    NANO_XRCE_EndpointContainerQosBinary_initialize((s_))

void
NANO_XRCE_PublisherQosBinary_finalize(
    NANO_XRCE_PublisherQosBinary *const self);

#define NANO_XRCE_PublisherQosBinary_finalize(s_) \
    NANO_XRCE_EndpointContainerQosBinary_finalize((s_))

/*i @} *//* nano_api_xrce_payload_support_pubqos */



/*i
 * @addtogroup nano_api_xrce_payload_objrepr_pub
 * @{
 */
/*i
 * @brief TODO
 * 
 */
typedef NANO_XRCE_EndpointContainerBinary NANO_XRCE_PublisherBinary;

#define NANO_XRCE_PUBLISHERBINARY_INITIALIZER \
    NANO_XRCE_ENDPOINTCONTAINERBINARY_INITIALIZER

NANO_bool
NANO_XRCE_PublisherBinary_initialize(
    NANO_XRCE_PublisherBinary *const self);

#define NANO_XRCE_PublisherBinary_initialize(s_) \
    NANO_XRCE_EndpointContainerBinary_initialize((s_))

void
NANO_XRCE_PublisherBinary_finalize(
    NANO_XRCE_PublisherQosBinary *const self);

#define NANO_XRCE_PublisherBinary_finalize(s_) \
    NANO_XRCE_EndpointContainerBinary_finalize((s_))

/*i @} *//* nano_api_xrce_payload_objrepr_pub */

#endif /* NANO_FEAT_OBJECT_KIND_PUBLISHER */

#if NANO_FEAT_OBJECT_KIND_SUBSCRIBER
/******************************************************************************
 *                            SUBSCRIBER_Representation
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_objrepr_sub
 * @{
 */

/*i
 * @brief TODO
 */
typedef NANO_XRCE_ContainedBinAndXmlObjectRepresentation 
    NANO_XRCE_SubscriberRepresentation;

#define NANO_XRCE_SUBSCRIBERREPRESENTATION_INITIALIZER \
    NANO_XRCE_CONTAINEDBINANDXMLOBJECTREPRESENTATION_INITIALIZER


NANO_bool
NANO_XRCE_SubscriberRepresentation_initialize(
    NANO_XRCE_SubscriberRepresentation *const self);

#define NANO_XRCE_SubscriberRepresentation_initialize(s_) \
    NANO_XRCE_ContainedBinAndXmlObjectRepresentation_initialize((s_))

void
NANO_XRCE_SubscriberRepresentation_finalize(
    NANO_XRCE_SubscriberRepresentation *const self);

#define NANO_XRCE_SubscriberRepresentation_finalize(s_) \
    NANO_XRCE_ContainedBinAndXmlObjectRepresentation_finalize((s_))

/*i
 * @brief TODO
 * 
 * @param self 
 * @return NANO_bool 
 */
NANO_bool
NANO_XRCE_SubscriberRepresentation_is_valid(
    const NANO_XRCE_SubscriberRepresentation *const self);

#define NANO_XRCE_SubscriberRepresentation_is_valid(s_) \
   NANO_XRCE_ContainedBinAndXmlObjectRepresentation_is_valid(\
        NANO_XRCE_OBJK_PARTICIPANT)
/*i @} *//* nano_api_xrce_payload_objrepr_sub */

/*i
 * @addtogroup nano_api_xrce_payload_support_subqos
 * @{
 */

/*i
 * @brief TODO
 * 
 */
typedef NANO_XRCE_EndpointContainerQosBinary NANO_XRCE_SubscriberQosBinary;

#define NANO_XRCE_SUBSCRIBERQOSBINARY_INITIALIZER \
    NANO_XRCE_ENDPOINTCONTAINERQOSBINARY_INITIALIZER

NANO_bool
NANO_XRCE_SubscriberQosBinary_initialize(
    NANO_XRCE_SubscriberQosBinary *const self);

#define NANO_XRCE_SubscriberQosBinary_initialize(s_) \
    NANO_XRCE_EndpointContainerQosBinary_initialize((s_))

void
NANO_XRCE_SubscriberQosBinary_finalize(
    NANO_XRCE_SubscriberQosBinary *const self);

#define NANO_XRCE_SubscriberQosBinary_finalize(s_) \
    NANO_XRCE_EndpointContainerQosBinary_finalize((s_))

/*i @} *//* nano_api_xrce_payload_support_subqos */

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_sub
 * @{
 */

/*i
 * @brief TODO
 * 
 */
typedef NANO_XRCE_EndpointContainerBinary NANO_XRCE_SubscriberBinary;

#define NANO_XRCE_SUBSCRIBERBINARY_INITIALIZER \
    NANO_XRCE_ENDPOINTCONTAINERBINARY_INITIALIZER

NANO_bool
NANO_XRCE_SubscriberBinary_initialize(
    NANO_XRCE_SubscriberBinary *const self);

#define NANO_XRCE_SubscriberBinary_initialize(s_) \
    NANO_XRCE_EndpointContainerBinary_initialize((s_))

void
NANO_XRCE_SubscriberBinary_finalize(
    NANO_XRCE_SubscriberBinary *const self);

#define NANO_XRCE_SubscriberBinary_finalize(s_) \
    NANO_XRCE_EndpointContainerBinary_finalize((s_))

/*i @} *//* nano_api_xrce_payload_objrepr_sub */

#endif /* NANO_FEAT_OBJECT_KIND_SUBSCRIBER */

#if NANO_FEAT_OBJECT_KIND_DATAWRITER || \
    NANO_FEAT_OBJECT_KIND_DATAREADER
/******************************************************************************
 *                          Endpoint_Representation
 ******************************************************************************/

/*i
 * @addtogroup nano_api_xrce_payload_support_endpointqos
 * @{
 */

/*i
 * @brief TODO
 * 
 */
typedef NANO_u16 NANO_XRCE_EndpointQosFlags;

/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_ENDPOINTQOSFLAGS_DEFAULT      (0x0000)

/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_ENDPOINTQOSFLAGS_IS_RELIABLE \
    ((NANO_XRCE_EndpointQosFlags)0x01)

/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_ENDPOINTQOSFLAGS_IS_HISTORY_KEEP_ALL \
    ((NANO_XRCE_EndpointQosFlags)0x01<<1)

/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_ENDPOINTQOSFLAGS_IS_OWNERSHIP_EXCLUSIVE \
    ((NANO_XRCE_EndpointQosFlags)0x01<<2)

/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_ENDPOINTQOSFLAGS_IS_DURABILITY_TRANSIENT_LOCAL \
    ((NANO_XRCE_EndpointQosFlags)0x01<<3)

/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_ENDPOINTQOSFLAGS_IS_DURABILITY_TRANSIENT \
    ((NANO_XRCE_EndpointQosFlags)0x01<<4)

/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_ENDPOINTQOSFLAGS_IS_DURABILITY_PERSISTENT \
    ((NANO_XRCE_EndpointQosFlags)0x01<<5)


/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_EndpointQosBinaryI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_EndpointQosFlags qos_flags;
#if NANO_FEAT_OPTIONAL
    /*i
     * @brief TODO
     * 
     */
    NANO_OPTIONALMEMBER_DEFN(NANO_u16, history_depth);
    /*i
     * @brief TODO
     * 
     */
    NANO_OPTIONALMEMBER_DEFN(NANO_u32, deadline_msec);
    /*i
     * @brief TODO
     * 
     */
    NANO_OPTIONALMEMBER_DEFN(NANO_u32, lifespan_msec);
#if NANO_FEAT_TYPED_SEQUENCE
    /*i
     * @brief TODO
     * 
     */
    NANO_OPTIONALMEMBER_DEFN(NANO_XRCE_UserData, user_data);
#else
    const NANO_bool has_user_data;
#endif /* NANO_FEAT_TYPED_SEQUENCE */
#else
    const NANO_bool has_history_depth;

    const NANO_bool has_deadline_msec;

    const NANO_bool has_lifespan_msec;

    const NANO_bool has_user_data;

#endif /* NANO_FEAT_OPTIONAL */

} NANO_XRCE_EndpointQosBinary;

#if NANO_FEAT_OPTIONAL
#if NANO_FEAT_TYPED_SEQUENCE
#define NANO_XRCE_ENDPOINTQOSBINARY_OPTIONAL_INITIALIZER \
    NANO_OPTIONALMEMBER_INITIALIZER(0), /* history_depth */\
    NANO_OPTIONALMEMBER_INITIALIZER(0), /* deadline_msec */\
    NANO_OPTIONALMEMBER_INITIALIZER(0), /* lifespan_msec */\
    NANO_OPTIONALMEMBER_INITIALIZER(\
        NANO_XRCE_USERDATA_INITIALIZER) /* user_data */
#else
#define NANO_XRCE_ENDPOINTQOSBINARY_OPTIONAL_INITIALIZER \
    NANO_OPTIONALMEMBER_INITIALIZER(0), /* history_depth */\
    NANO_OPTIONALMEMBER_INITIALIZER(0), /* deadline_msec */\
    NANO_OPTIONALMEMBER_INITIALIZER(0)  /* lifespan_msec */
#endif /* NANO_FEAT_TYPED_SEQUENCE */
#else
#define NANO_XRCE_ENDPOINTQOSBINARY_OPTIONAL_INITIALIZER \
    NANO_BOOL_FALSE, /* has_history_depth */\
    NANO_BOOL_FALSE, /* has_deadline_msec */\
    NANO_BOOL_FALSE, /* has_lifespan_msec */\
    NANO_BOOL_FALSE /* has_user_data */
#endif /* NANO_FEAT_OPTIONAL */

#define NANO_XRCE_ENDPOINTQOSBINARY_INITIALIZER \
{\
    NANO_XRCE_ENDPOINTQOSFLAGS_DEFAULT, /* qos_flags */\
    NANO_XRCE_ENDPOINTQOSBINARY_OPTIONAL_INITIALIZER /* optional fields */ \
}

NANO_bool
NANO_XRCE_EndpointQosBinary_initialize(
    NANO_XRCE_EndpointQosBinary *const self);

#define NANO_XRCE_EndpointQosBinary_initialize(s_) \
    NANO_XRCE_UserData_initialize(&(s_)->user_data)

NANO_bool
NANO_XRCE_EndpointQosBinary_finalize(
    NANO_XRCE_EndpointQosBinary *const self);

#define NANO_XRCE_EndpointQosBinary_finalize(s_) \
    NANO_XRCE_UserData_finalize(&(s_)->user_data)


/*i @} *//* nano_api_xrce_payload_support_endpointqos */

#endif /* NANO_FEAT_OBJECT_KIND_DATAWRITER || \
          NANO_FEAT_OBJECT_KIND_DATAREADER */


#if NANO_FEAT_OBJECT_KIND_DATAWRITER
/******************************************************************************
 *                            DATAWRITER_Representation
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_objrepr_dw
 * @{
 */

/*i
 * @brief TODO
 */
typedef NANO_XRCE_ContainedBinAndXmlObjectRepresentation 
    NANO_XRCE_DataWriterRepresentation;

#define NANO_XRCE_DATAWRITERREPRESENTATION_INITIALIZER \
    NANO_XRCE_CONTAINEDBINANDXMLOBJECTREPRESENTATION_INITIALIZER

NANO_bool
NANO_XRCE_DataWriterRepresentation_initialize(
    NANO_XRCE_DataWriterRepresentation *const self);

#define NANO_XRCE_DataWriterRepresentation_initialize(s_) \
    NANO_XRCE_ContainedBinAndXmlObjectRepresentation_initialize((s_))

void
NANO_XRCE_DataWriterRepresentation_finalize(
    NANO_XRCE_DataWriterRepresentation *const self);

#define NANO_XRCE_DataWriterRepresentation_finalize(s_) \
    NANO_XRCE_ContainedBinAndXmlObjectRepresentation_finalize((s_))

/*i @} *//* nano_api_xrce_payload_objrepr_dw */

/*i
 * @addtogroup nano_api_xrce_payload_support_dwqos
 * @{
 */
/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_DataWriterQosBinaryI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_EndpointQosBinary base;
#if NANO_FEAT_OPTIONAL
    /*i
     * @brief TODO
     * 
     */
    NANO_OPTIONALMEMBER_DEFN(NANO_u32, ownership_strength);
#else
    const NANO_bool has_ownership_strength;
#endif /* NANO_FEAT_OPTIONAL */
} NANO_XRCE_DataWriterQosBinary;

#if NANO_FEAT_OPTIONAL
#define NANO_XRCE_DATAWRITERQOSBINARY_OPTIONAL_INITIALIZER \
    NANO_OPTIONALMEMBER_INITIALIZER(0) /* ownership_strength */
#else
#define NANO_XRCE_DATAWRITERQOSBINARY_OPTIONAL_INITIALIZER \
    NANO_BOOL_FALSE /* has_ownership_strength */
#endif /* NANO_FEAT_OPTIONAL */

#define NANO_XRCE_DATAWRITERQOSBINARY_INITIALIZER \
{\
    NANO_XRCE_ENDPOINTQOSBINARY_INITIALIZER, /* base */\
    NANO_XRCE_DATAWRITERQOSBINARY_OPTIONAL_INITIALIZER /* optional fields */\
}

NANO_bool
NANO_XRCE_DataWriterQosBinary_initialize(
    NANO_XRCE_DataWriterQosBinary *const self);

#define NANO_XRCE_DataWriterQosBinary_initialize(s_) \
    NANO_XRCE_EndpointQosBinary_initialize(&(s_)->base)

void
NANO_XRCE_DataWriterQosBinary_finalize(
    NANO_XRCE_DataWriterQosBinary *const self);

#define NANO_XRCE_DataWriterQosBinary_finalize(s_) \
    NANO_XRCE_EndpointQosBinary_finalize(&(s_)->base)

/*i @} *//* nano_api_xrce_payload_support_dwqos */

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_dw
 * @{
 */
/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_DataWriterBinaryI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_TopicName topic_name;

#if NANO_FEAT_OPTIONAL
    /*i
     * @brief TODO
     * 
     */
    NANO_OPTIONALMEMBER_DEFN(NANO_XRCE_DataWriterQosBinary, qos);
#else
    const NANO_bool has_qos;
#endif /* NANO_FEAT_OPTIONAL */
} NANO_XRCE_DataWriterBinary;

#if NANO_FEAT_OPTIONAL
#define NANO_XRCE_DATAWRITERBINARY_OPTIONAL_INITIALIZER \
    NANO_OPTIONALMEMBER_INITIALIZER(\
        NANO_XRCE_DATAWRITERQOSBINARY_INITIALIZER) /* qos */
#else
#define NANO_XRCE_DATAWRITERBINARY_OPTIONAL_INITIALIZER \
    NANO_BOOL_FALSE /* has_qos */
#endif /* NANO_FEAT_OPTIONAL */

#define NANO_XRCE_DATAWRITERBINARY_INITIALIZER \
{\
    NANO_XRCE_TOPICNAME_INITIALIZER, /* topic_name */\
    NANO_XRCE_DATAWRITERBINARY_OPTIONAL_INITIALIZER /* optional fields */\
}

NANO_bool
NANO_XRCE_DataWriterBinary_initialize(
    NANO_XRCE_DataWriterBinary *const self);

#define NANO_XRCE_DataWriterBinary_initialize(s_) \
    (NANO_XRCE_TopicName_initialize(&(s_)->topic_name) && \
        NANO_XRCE_DataWriterQosBinary_initialize(&(s_)->qos))


void
NANO_XRCE_DataWriterBinary_finalize(
    NANO_XRCE_DataWriterBinary *const self);

#define NANO_XRCE_DataWriterBinary_finalize(s_) \
{\
    NANO_XRCE_TopicName_finalize(&(s_)->topic_name);\
    NANO_XRCE_DataWriterQosBinary_finalize(&(s_)->qos);\
}

/*i @} *//* nano_api_xrce_payload_objrepr_dw */

#endif /* NANO_FEAT_OBJECT_KIND_DATAWRITER */


#if NANO_FEAT_OBJECT_KIND_DATAREADER
/******************************************************************************
 *                            DATAREADER_Representation
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_objrepr_dr
 * @{
 */

/*i
 * @brief TODO
 */
typedef NANO_XRCE_ContainedBinAndXmlObjectRepresentation 
    NANO_XRCE_DataReaderRepresentation;

#define NANO_XRCE_DATAREADERREPRESENTATION_INITIALIZER \
    NANO_XRCE_CONTAINEDBINANDXMLOBJECTREPRESENTATION_INITIALIZER

NANO_bool
NANO_XRCE_DataReaderRepresentation_initialize(
    NANO_XRCE_DataReaderRepresentation *const self);

#define NANO_XRCE_DataReaderRepresentation_initialize(s_) \
    NANO_XRCE_ContainedBinAndXmlObjectRepresentation_initialize((s_))

void
NANO_XRCE_DataReaderRepresentation_finalize(
    NANO_XRCE_DataReaderRepresentation *const self);

#define NANO_XRCE_DataReaderRepresentation_finalize(s_) \
    NANO_XRCE_ContainedBinAndXmlObjectRepresentation_finalize((s_))

/*i @} *//* nano_api_xrce_payload_objrepr_dr */

/*i
 * @addtogroup nano_api_xrce_payload_support_drqos
 * @{
 */

/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_DataReaderQosBinaryI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_EndpointQosBinary base;

#if NANO_FEAT_OPTIONAL
    /*i
     * @brief TODO
     * 
     */
    NANO_OPTIONALMEMBER_DEFN(NANO_u32, timebased_filter_msec);
    /*i
     * @brief TODO
     * 
     */
    NANO_OPTIONALMEMBER_DEFN(NANO_XRCE_ContentFilterExpr, contentbased_filter);
#else
    const NANO_bool has_timebased_filter_msec;

    const NANO_bool has_contentbased_filter;
#endif /* NANO_FEAT_OPTIONAL */
    
} NANO_XRCE_DataReaderQosBinary;

#if NANO_FEAT_OPTIONAL
#define NANO_XRCE_DATAREADERQOSBINARY_OPTIONAL_INITIALIZER \
    NANO_OPTIONALMEMBER_INITIALIZER(0), /* timebased_filter_msec */\
    NANO_OPTIONALMEMBER_INITIALIZER(\
        NANO_XRCE_CONTENTFILTEREXPR_INITIALIZER) /* contentbased_filter */
#else
#define NANO_XRCE_DATAREADERQOSBINARY_OPTIONAL_INITIALIZER \
    NANO_BOOL_FALSE, /* has_timebased_filter_msec */\
    NANO_BOOL_FALSE  /* has_contentbased_filter */
#endif /* NANO_FEAT_OPTIONAL */

#define NANO_XRCE_DATAREADERQOSBINARY_INITIALIZER \
{\
    NANO_XRCE_ENDPOINTQOSBINARY_INITIALIZER, /* base */\
    NANO_XRCE_DATAREADERQOSBINARY_OPTIONAL_INITIALIZER /* optional fields */\
}

NANO_bool
NANO_XRCE_DataReaderQosBinary_initialize(
    NANO_XRCE_DataReaderQosBinary *const self);

#define NANO_XRCE_DataReaderQosBinary_initialize(s_) \
    (NANO_XRCE_EndpointQosBinary_initialize(&(s_)->base) && \
        NANO_XRCE_ContentFilterExpr_initialize(\
            NANO_OPTIONALMEMBER_AS_REF((s_), contentbased_filter)))

void
NANO_XRCE_DataReaderQosBinary_finalize(
    NANO_XRCE_DataReaderQosBinary *const self);

#define NANO_XRCE_DataReaderQosBinary_finalize(s_) \
{\
    NANO_XRCE_EndpointQosBinary_finalize(&(s_)->base);\
    NANO_XRCE_ContentFilterExpr_finalize(\
            NANO_OPTIONALMEMBER_AS_REF((s_), contentbased_filter));\
}
/*i @} *//* nano_api_xrce_payload_support_drqos */

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_dr
 * @{
 */

/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_DataReaderBinaryI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_TopicName topic_name;
#if NANO_FEAT_OPTIONAL
    /*i
     * @brief TODO
     * 
     */
    NANO_OPTIONALMEMBER_DEFN(NANO_XRCE_DataReaderQosBinary, qos);
#else
    const NANO_bool has_qos;
#endif /* NANO_FEAT_OPTIONAL */
} NANO_XRCE_DataReaderBinary;

#if NANO_FEAT_OPTIONAL
#define NANO_XRCE_DATAREADERBINARY_OPTIONAL_INITIALIZER \
    NANO_OPTIONALMEMBER_INITIALIZER(\
        NANO_XRCE_DATAREADERQOSBINARY_INITIALIZER) /* qos */
#else
#define NANO_XRCE_DATAREADERBINARY_OPTIONAL_INITIALIZER \
    NANO_BOOL_FALSE /* qos */
#endif /* NANO_FEAT_OPTIONAL */

#define NANO_XRCE_DATAREADERBINARY_INITIALIZER \
{\
    NANO_XRCE_TOPICNAME_INITIALIZER, /* topic_name */ \
    NANO_XRCE_DATAREADERBINARY_OPTIONAL_INITIALIZER /* optional fields */\
}


NANO_bool
NANO_XRCE_DataReaderBinary_initialize(
    NANO_XRCE_DataReaderBinary *const self);

#define NANO_XRCE_DataReaderBinary_initialize(s_) \
    (NANO_XRCE_TopicName_initialize(&(s_)->topic_name) && \
        NANO_XRCE_DataReaderQosBinary_initialize(&(s_)->qos))


void
NANO_XRCE_DataReaderBinary_finalize(
    NANO_XRCE_DataReaderBinary *const self);

#define NANO_XRCE_DataReaderBinary_finalize(s_) \
{\
    NANO_XRCE_TopicName_finalize(&(s_)->topic_name); \
    NANO_XRCE_DataReaderQosBinary_finalize(&(s_)->qos);\
}

/*i @} *//* nano_api_xrce_payload_objrepr_dr */

#endif /* NANO_FEAT_OBJECT_KIND_DATAREADER */


#if NANO_FEAT_OBJECT_KIND_PARTICIPANT
/******************************************************************************
 *                            PARTICIPANT_Representation
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_support_domainid
 * @{
 */

/*i
 * @brief TODO
 * 
 */
typedef NANO_i16    NANO_XRCE_DomainId;

/*i
 * @brief TODO
 * 
 */
#define NANO_XRCE_DOMAINID_INVALID        -1

NANO_bool
NANO_XRCE_DomainId_initialize(
    NANO_XRCE_DomainId *const self);

#define NANO_XRCE_DomainId_initialize(s_) \
    (*(s_) = NANO_XRCE_DOMAINID_INVALID, NANO_BOOL_TRUE)

/*i
 * @brief TODO
 * 
 * @param self 
 * @return NANO_bool 
 */
NANO_bool
NANO_XRCE_DomainId_is_valid(const NANO_XRCE_DomainId self);

#define NANO_XRCE_DomainId_is_valid(s_) \
   ((s_) > 0)

/*i @} *//* nano_api_xrce_payload_support_domainid */

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_dp
 * @{
 */
/*i
 * @brief TODO
 */
typedef struct NANODllExport NANO_XRCE_DomainParticipantRepresentationI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_Representation3 base;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_DomainId domain_id;
} NANO_XRCE_DomainParticipantRepresentation;

#define NANO_XRCE_DOMAINPARTICIPANTREPRESENTATION_INITIALIZER \
{\
    NANO_XRCE_REPRESENTATION3_INITIALIZER, /* base */\
    NANO_XRCE_DOMAINID_INVALID /* domain_id */\
}

NANO_bool
NANO_XRCE_DomainParticipantRepresentation_initialize(
    NANO_XRCE_DomainParticipantRepresentation *const self);

#define NANO_XRCE_DomainParticipantRepresentation_initialize(s_) \
    (NANO_XRCE_DomainId_initialize(&(s_)->domain_id) && \
    NANO_XRCE_Representation3_initialize(&(s_)->base))

void
NANO_XRCE_DomainParticipantRepresentation_finalize(
    NANO_XRCE_DomainParticipantRepresentation *const self);

#define NANO_XRCE_DomainParticipantRepresentation_finalize(s_) \
    NANO_XRCE_Representation3_finalize(&(s_)->base)

/*i @} *//* nano_api_xrce_payload_objrepr_dp */

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_dp
 * @{
 */

/*i
 *  @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_DomainParticipantBinaryI
{
#if NANO_FEAT_OPTIONAL
    /*i
     * @brief TODO
     * 
     */
    NANO_OPTIONALMEMBER_DEFN(NANO_XRCE_ObjectRef, domain_ref);
    /*i
     * @brief TODO
     * 
     */
    NANO_OPTIONALMEMBER_DEFN(NANO_XRCE_ObjectRef, qos_profile_ref);
#else
    const NANO_bool has_domain_ref;

    const NANO_bool has_qos_profile_ref;
#endif /* NANO_FEAT_OPTIONAL */

} NANO_XRCE_DomainParticipantBinary;

#if NANO_FEAT_OPTIONAL
#define NANO_XRCE_DOMAINPARTICIPANTBINARY_INITIALIZER \
{\
    NANO_OPTIONALMEMBER_INITIALIZER(\
        NANO_XRCE_OBJECTREF_INITIALIZER), /* domain_ref */\
    NANO_OPTIONALMEMBER_INITIALIZER(\
        NANO_XRCE_OBJECTREF_INITIALIZER) /* qos_profile_ref */\
}
#else
#define NANO_XRCE_DOMAINPARTICIPANTBINARY_INITIALIZER \
{\
    NANO_BOOL_FALSE, /* has_domain_ref */\
    NANO_BOOL_FALSE  /* has_qos_profile_ref */\
}
#endif /* NANO_FEAT_OPTIONAL */

NANO_bool
NANO_XRCE_DomainParticipantBinary_initialize(
    NANO_XRCE_DomainParticipantBinary *const self);

#define NANO_XRCE_DomainParticipantBinary_initialize(s_) \
    (NANO_XRCE_ObjectRef_initialize(\
        NANO_OPTIONALMEMBER_AS_REF((s_),domain_ref)) && \
    NANO_XRCE_ObjectRef_initialize(\
        NANO_OPTIONALMEMBER_AS_REF((s_),qos_profile_ref)))

void
NANO_XRCE_DomainParticipantBinary_finalize(
    NANO_XRCE_DomainParticipantBinary *const self);

#define NANO_XRCE_DomainParticipantBinary_finalize(s_) \
{\
    NANO_XRCE_ObjectRef_finalize(\
        NANO_OPTIONALMEMBER_AS_REF((s_),domain_ref));\
    NANO_XRCE_ObjectRef_finalize(\
        NANO_OPTIONALMEMBER_AS_REF((s_),qos_profile_ref));\
}

/*i @} *//* nano_api_xrce_payload_objrepr_dp */

#endif /* NANO_FEAT_OBJECT_KIND_PARTICIPANT */


#if NANO_FEAT_OBJECT_KIND_TOPIC
/******************************************************************************
 *                            TOPIC_Representation
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_objrepr_topic
 * @{
 */

/*i
 * @brief TODO
 */
typedef struct NANODllExport NANO_XRCE_TopicRepresentationI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_Representation3 base;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_ObjectId participant_id;

} NANO_XRCE_TopicRepresentation;

#define NANO_XRCE_TOPICREPRESENTATION_INITIALIZER \
{\
    NANO_XRCE_REPRESENTATION3_INITIALIZER, /* base */\
    NANO_XRCE_OBJECTID_INVALID /* participant_id */\
}

NANO_bool
NANO_XRCE_TopicRepresentation_initialize(
    NANO_XRCE_TopicRepresentation *const self);

#define NANO_XRCE_TopicRepresentation_initialize(s_) \
    (NANO_XRCE_ObjectId_initialize(&(s_)->participant_id) && \
    NANO_XRCE_Representation3_initialize(&(s_)->base))

void
NANO_XRCE_TopicRepresentation_finalize(
    NANO_XRCE_TopicRepresentation *const self);

#define NANO_XRCE_TopicRepresentation_finalize(s_) \
    NANO_XRCE_Representation3_finalize(&(s_)->base)

/*i @} *//* nano_api_xrce_payload_objrepr_topic */

/*i
 * @addtogroup nano_api_xrce_payload_objrepr_topic
 * @{
 */
/*i
 * @brief TODO
 * 
 */
typedef struct NANODllExport NANO_XRCE_TopicBinaryI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_TopicName topic_name;

#if NANO_FEAT_OPTIONAL
    /*i
     * @brief TODO
     * 
     */
    NANO_OPTIONALMEMBER_DEFN(NANO_XRCE_TypeRef, type_ref);
    /*i
     * @brief TODO
     * 
     */
    NANO_OPTIONALMEMBER_DEFN(NANO_XRCE_TypeId, type_id);

#else
    const NANO_bool has_type_ref;

    const NANO_bool has_type_id;

#endif /* NANO_FEAT_OPTIONAL */
} NANO_XRCE_TopicBinary;



#if NANO_FEAT_OPTIONAL
#define NANO_XRCE_TOPICBINARY_OPTIONAL_INITIALIZER \
    NANO_OPTIONALMEMBER_INITIALIZER(\
        NANO_XRCE_TYPEREF_INITIALIZER), /* type_ref */\
    NANO_OPTIONALMEMBER_INITIALIZER(\
        NANO_XRCE_TYPEID_INITIALIZER) /* type_id */
#else
#define NANO_XRCE_TOPICBINARY_OPTIONAL_INITIALIZER \
    NANO_BOOL_FALSE, /* has_type_ref */\
    NANO_BOOL_FALSE, /* has_type_id */
#endif /* NANO_FEAT_OPTIONAL */

#define NANO_XRCE_TOPICBINARY_INITIALIZER \
{\
    NANO_XRCE_TOPICNAME_INITIALIZER, /* topic_name */\
    NANO_XRCE_TOPICBINARY_OPTIONAL_INITIALIZER\
}

#if NANO_FEAT_OPTIONAL

NANO_bool
NANO_XRCE_TopicBinary_initialize(
    NANO_XRCE_TopicBinary *const self);

#define NANO_XRCE_TopicBinary_initialize(s_) \
    (NANO_XRCE_TopicName_initialize(&(s_)->topic_name) && \
    NANO_XRCE_TypeRef_initialize(\
        NANO_OPTIONALMEMBER_AS_REF((s_),type_ref)) && \
    NANO_XRCE_TypeId_initialize(\
        NANO_OPTIONALMEMBER_AS_REF((s_),type_id)))

void
NANO_XRCE_TopicBinary_finalize(
    NANO_XRCE_TopicBinary *const self);

#define NANO_XRCE_TopicBinary_finalize(s_) \
{\
    NANO_XRCE_TopicName_finalize(&(s_)->topic_name);\
    NANO_XRCE_TypeRef_finalize(\
        NANO_OPTIONALMEMBER_AS_REF((s_),type_ref)); \
    NANO_XRCE_TypeId_finalize(\
        NANO_OPTIONALMEMBER_AS_REF((s_),type_id));\
}

#endif /* NANO_FEAT_OPTIONAL */

/*i @} *//* nano_api_xrce_payload_objrepr_topic */

#endif /* NANO_FEAT_OBJECT_KIND_TOPIC */

/******************************************************************************
 *                          CLIENT_Representation
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_objrepr_client
 * @{
 */

/*i
 * @brief A data type providing basic information about an XRCE Client.
 */
typedef struct NANODllExport NANO_XRCE_ClientRepresentationI
{
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_Cookie xrce_cookie;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_Version xrce_version;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_VendorId xrce_vendor_id;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_ClientKey client_key;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_SessionId session_id;

#if NANO_FEAT_OPTIONAL && \
    NANO_FEAT_PROPERTY && \
    NANO_FEAT_TYPED_SEQUENCE
    /*i
     * @brief TODO
     * 
     */
    NANO_OPTIONALMEMBER_DEFN(NANO_PropertySeq, properties);
#else /* !(NANO_FEAT_PROPERTY && NANO_FEAT_OPTIONAL) */
    NANO_bool has_properties;
#endif /* NANO_FEAT_PROPERTY && NANO_FEAT_OPTIONAL */

#if NANO_FEAT_MTU_IN_CLIENT_REPR
    NANO_u16 mtu;
#endif /* NANO_FEAT_MTU_IN_CLIENT_REPR */

} NANO_XRCE_ClientRepresentation;

#if NANO_FEAT_MTU_IN_CLIENT_REPR
#define NANO_XRCE_CLIENTREPRESENTATION_SERIALIZED_SIZE_MIN \
    (sizeof(NANO_XRCE_Cookie) + \
        sizeof(NANO_XRCE_Version) + \
        sizeof(NANO_XRCE_VendorId) + \
        sizeof(NANO_XRCE_ClientKey) + \
        sizeof(NANO_XRCE_SessionId) + \
        sizeof(NANO_bool) + \
        sizeof(NANO_u16))

#else
#define NANO_XRCE_CLIENTREPRESENTATION_SERIALIZED_SIZE_MIN \
    (sizeof(NANO_XRCE_Cookie) + \
        sizeof(NANO_XRCE_Version) + \
        sizeof(NANO_XRCE_VendorId) + \
        sizeof(NANO_XRCE_ClientKey) + \
        sizeof(NANO_XRCE_SessionId) + \
        sizeof(NANO_bool))

#endif /* NANO_FEAT_MTU_IN_CLIENT_REPR */


#if NANO_FEAT_OPTIONAL && \
    NANO_FEAT_PROPERTY && \
    NANO_FEAT_TYPED_SEQUENCE
#define NANO_XRCE_CLIENTREPRESENTATION_SERIALIZED_SIZE_MAX \
    NANO_OSAPI_Memory_align_size_up(\
        (NANO_XRCE_CLIENTREPRESENTATION_SERIALIZED_SIZE_MIN +\
        sizeof(NANO_u32)), sizeof(NANO_u32))
#else
#define NANO_XRCE_CLIENTREPRESENTATION_SERIALIZED_SIZE_MAX \
    NANO_XRCE_CLIENTREPRESENTATION_SERIALIZED_SIZE_MIN
#endif



#if NANO_FEAT_MTU_IN_CLIENT_REPR
#define NANO_XRCE_CLIENTREPRESENTATION_LEGACY_INITIALIZER \
    , 0 /* mtu */
#else
#define NANO_XRCE_CLIENTREPRESENTATION_LEGACY_INITIALIZER
#endif /* NANO_FEAT_MTU_IN_CLIENT_REPR */

#if NANO_FEAT_OPTIONAL && \
    NANO_FEAT_PROPERTY && \
    NANO_FEAT_TYPED_SEQUENCE
#define NANO_XRCE_CLIENTREPRESENTATION_PROPERTIES_INITIALIZER \
    NANO_OPTIONALMEMBER_INITIALIZER(\
            NANO_PROPERTYSEQ_INITIALIZER)
#else
#define NANO_XRCE_CLIENTREPRESENTATION_PROPERTIES_INITIALIZER \
    NANO_BOOL_FALSE
#endif /* NANO_FEAT_OPTIONAL && \
            NANO_FEAT_PROPERTY && \
            NANO_FEAT_TYPED_SEQUENCE */

#define NANO_XRCE_CLIENTREPRESENTATION_INITIALIZER \
{\
    NANO_XRCE_COOKIE_DEFAULT, /* xrce_cookie */\
    NANO_XRCE_VERSION_DEFAULT, /* xrce_version */\
    NANO_XRCE_VENDORID_DEFAULT, /* xrce_vendor_id */\
    NANO_XRCE_CLIENTKEY_INVALID, /* client_key */\
    NANO_XRCE_SESSIONID_NONE_WITHOUT_CLIENT, /* session_id */\
    NANO_XRCE_CLIENTREPRESENTATION_PROPERTIES_INITIALIZER /* properties */\
    NANO_XRCE_CLIENTREPRESENTATION_LEGACY_INITIALIZER /* legacy */\
}

#if NANO_FEAT_PROPERTY

NANO_bool
NANO_XRCE_ClientRepresentation_initialize(
    NANO_XRCE_ClientRepresentation *const self);

#define NANO_XRCE_ClientRepresentation_initialize(s_) \
    (NANO_XRCE_Cookie_initialize(&(s_)->xrce_cookie) && \
    NANO_XRCE_Version_initialize(&(s_)->xrce_version) && \
    NANO_XRCE_VendorId_initialize(&(s_)->xrce_vendor_id) && \
    NANO_XRCE_ClientKey_initialize(&(s_)->client_key) && \
    NANO_XRCE_SessionId_initialize(&(s_)->session_id) && \
    NANO_PropertySeq_initialize(NANO_OPTIONALMEMBER_AS_REF((s_),properties)))

void
NANO_XRCE_ClientRepresentation_finalize(
    NANO_XRCE_ClientRepresentation *const self);

#define NANO_XRCE_ClientRepresentation_finalize(s_) \
{\
    NANO_PropertySeq_finalize(NANO_OPTIONALMEMBER_AS_REF((s_),properties));\
}

#endif /* NANO_FEAT_PROPERTY */


/*i
 * @brief TODO
 * 
 * @param self 
 * @return NANO_bool 
 */
NANO_bool
NANO_XRCE_ClientRepresentation_is_valid(
    const NANO_XRCE_ClientRepresentation *const self);

#define NANO_XRCE_ClientRepresentation_is_valid(s_) \
   (NANO_XRCE_Cookie_is_valid(&(s_)->xrce_cookie) && \
    NANO_XRCE_Version_is_compatible(&(s_)->xrce_version) && \
   ((NANO_XRCE_SessionId_has_client_key((s_)->session_id) && \
        NANO_XRCE_ClientKey_is_valid(&(s_)->client_key)) || \
   !NANO_XRCE_SessionId_has_client_key((s_)->session_id)))

NANO_RetCode
NANO_XRCE_ClientRepresentation_serialize(
    const NANO_XRCE_ClientRepresentation *const self,
    NANO_MessageBuffer *const payload);


/*i @} *//* nano_api_xrce_payload_objrepr_client */


/******************************************************************************
 *                          AGENT_Representation
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_objrepr_agent
 * @{
 */

/*i
 * @brief A data type providing basic information about an XRCE Agent.
 */
typedef struct NANODllExport NANO_XRCE_AgentRepresentationI
{
#if NANO_FEAT_EPROSIMA
    NANO_u8 result_status[2];
#endif /* NANO_FEAT_EPROSIMA */
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_Cookie xrce_cookie;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_Version xrce_version;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_VendorId xrce_vendor_id;

#if NANO_FEAT_OPTIONAL && \
    NANO_FEAT_PROPERTY && \
    NANO_FEAT_TYPED_SEQUENCE
    /*i
     * @brief TODO
     * 
     */
    NANO_OPTIONALMEMBER_DEFN(NANO_PropertySeq, properties);
#else /* !(NANO_FEAT_PROPERTY && NANO_FEAT_OPTIONAL) */
    NANO_bool has_properties;
#endif /* NANO_FEAT_PROPERTY && NANO_FEAT_OPTIONAL */

} NANO_XRCE_AgentRepresentation;

#if NANO_FEAT_EPROSIMA
#define NANO_XRCE_AGENTREPRESENTATION_SERIALIZED_SIZE_MIN \
    (sizeof(NANO_u16) +\
    sizeof(NANO_XRCE_Cookie) + \
    sizeof(NANO_XRCE_Version) + \
    sizeof(NANO_XRCE_VendorId) + \
    sizeof(NANO_bool))
#else
#define NANO_XRCE_AGENTREPRESENTATION_SERIALIZED_SIZE_MIN \
    (sizeof(NANO_XRCE_Cookie) + \
    sizeof(NANO_XRCE_Version) + \
    sizeof(NANO_XRCE_VendorId) + \
    sizeof(NANO_bool))
#endif /* NANO_FEAT_EPROSIMA */

#if 0
/* We never send the PropertySeq so MAX == MIN */
#if NANO_FEAT_OPTIONAL && \
    NANO_FEAT_PROPERTY && \
    NANO_FEAT_TYPED_SEQUENCE
/* Include sequence length */
#define NANO_XRCE_AGENTREPRESENTATION_SERIALIZED_SIZE_MAX \
    NANO_OSAPI_Memory_align_size_up(\
        (NANO_XRCE_AGENTREPRESENTATION_SERIALIZED_SIZE_MIN+sizeof(NANO_u32)),\
        sizeof(NANO_u32))
#else
#define NANO_XRCE_AGENTREPRESENTATION_SERIALIZED_SIZE_MAX \
    NANO_XRCE_AGENTREPRESENTATION_SERIALIZED_SIZE_MIN
#endif
#else
#define NANO_XRCE_AGENTREPRESENTATION_SERIALIZED_SIZE_MAX \
    NANO_XRCE_AGENTREPRESENTATION_SERIALIZED_SIZE_MIN
#endif

#if NANO_FEAT_EPROSIMA
#define NANO_XRCE_AGENTREPRESENTATION_LEGACY_INITIALIZER \
    { 0 },
#else
#define NANO_XRCE_AGENTREPRESENTATION_LEGACY_INITIALIZER
#endif /* NANO_FEAT_EPROSIMA */

#if NANO_FEAT_OPTIONAL && \
    NANO_FEAT_PROPERTY && \
    NANO_FEAT_TYPED_SEQUENCE
#define NANO_XRCE_AGENTREPRESENTATION_PROPERTIES_INITIALIZER \
    NANO_OPTIONALMEMBER_INITIALIZER(\
            NANO_PROPERTYSEQ_INITIALIZER)
#else
#define NANO_XRCE_AGENTREPRESENTATION_PROPERTIES_INITIALIZER \
    NANO_BOOL_FALSE
#endif /* NANO_FEAT_OPTIONAL && \
            NANO_FEAT_PROPERTY && \
            NANO_FEAT_TYPED_SEQUENCE */


#define NANO_XRCE_AGENTREPRESENTATION_INITIALIZER \
{\
    NANO_XRCE_AGENTREPRESENTATION_LEGACY_INITIALIZER /* legacy */\
    NANO_XRCE_COOKIE_DEFAULT, /* xrce_cookie */\
    NANO_XRCE_VERSION_DEFAULT, /* xrce_version */\
    NANO_XRCE_VENDORID_DEFAULT, /* xrce_vendor_id */\
    NANO_XRCE_AGENTREPRESENTATION_PROPERTIES_INITIALIZER /* properties */\
}

#if NANO_FEAT_PROPERTY
NANO_bool
NANO_XRCE_AgentRepresentation_initialize(
    NANO_XRCE_AgentRepresentation *const self);

#define NANO_XRCE_AgentRepresentation_initialize(s_) \
    (NANO_XRCE_Cookie_initialize(&(s_)->xrce_cookie) && \
    NANO_XRCE_Version_initialize(&(s_)->xrce_version) && \
    NANO_XRCE_VendorId_initialize(&(s_)->xrce_vendor_id) && \
    NANO_PropertySeq_initialize(NANO_OPTIONALMEMBER_AS_REF((s_),properties)))

void
NANO_XRCE_AgentRepresentation_finalize(
    NANO_XRCE_AgentRepresentation *const self);

#define NANO_XRCE_AgentRepresentation_finalize(s_) \
{\
    NANO_PropertySeq_finalize(NANO_OPTIONALMEMBER_AS_REF((s_),properties)));\
}

#endif /* NANO_FEAT_PROPERTY */


/*i
 * @brief TODO
 * 
 * @param self 
 * @return NANO_bool 
 */
NANO_bool
NANO_XRCE_AgentRepresentation_is_valid(
    const NANO_XRCE_AgentRepresentation *const self);

#define NANO_XRCE_AgentRepresentation_is_valid(s_) \
   (NANO_XRCE_Cookie_is_valid((s_)->cookie) && \
    NANO_XRCE_Version_is_compatible((s_)->xrce_version))


/*i @} *//* nano_api_xrce_payload_objrepr_agent */


#if NANO_FEAT_OBJECT
/******************************************************************************
 *                          ObjectVariant
 ******************************************************************************/
/*i
 * @addtogroup nano_api_xrce_payload_objrepr_common
 * @{
 */

/*i
 * @brief TODO
 * 
 */
typedef union NANODllExport NANO_XRCE_ObjectVariantValueI
{
#if NANO_FEAT_OBJECT_KIND_DATAREADER
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_DataReaderRepresentation data_reader;
#endif /* NANO_FEAT_OBJECT_KIND_DATAREADER */
#if NANO_FEAT_OBJECT_KIND_DATAWRITER
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_DataWriterRepresentation data_writer;
#endif /* NANO_FEAT_OBJECT_KIND_DATAWRITER */
#if NANO_FEAT_OBJECT_KIND_SUBSCRIBER
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_SubscriberRepresentation subscriber;
#endif /* NANO_FEAT_OBJECT_KIND_SUBSCRIBER */
#if NANO_FEAT_OBJECT_KIND_PUBLISHER
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_PublisherRepresentation publisher;
#endif /* NANO_FEAT_OBJECT_KIND_PUBLISHER */
#if NANO_FEAT_OBJECT_KIND_TOPIC
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_TopicRepresentation topic;
#endif /* NANO_FEAT_OBJECT_KIND_TOPIC */
#if NANO_FEAT_OBJECT_KIND_DOMAIN
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_DomainRepresentation domain;
#endif /* NANO_FEAT_OBJECT_KIND_DOMAIN */
#if NANO_FEAT_OBJECT_KIND_TYPE
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_TypeRepresentation type;
#endif /* NANO_FEAT_OBJECT_KIND_TYPE */
#if NANO_FEAT_OBJECT_KIND_QOSPROFILE
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_QosProfileRepresentation qos_profile;
#endif /* NANO_FEAT_OBJECT_KIND_QOSPROFILE */
#if NANO_FEAT_OBJECT_KIND_PARTICIPANT
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_DomainParticipantRepresentation participant;
#endif /* NANO_FEAT_OBJECT_KIND_PARTICIPANT */
#if NANO_FEAT_OBJECT_KIND_APPLICATION
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_ApplicationRepresentation application;
#endif /* NANO_FEAT_OBJECT_KIND_APPLICATION */
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_ClientRepresentation client;
    /*i
     * @brief TODO
     * 
     */
    NANO_XRCE_AgentRepresentation agent;
} NANO_XRCE_ObjectVariantValue;

#if NANO_FEAT_OBJECT_KIND_DATAREADER
#define NANO_XRCE_OBJECTVARIANTVALUE_ELEMENT_INITIALIZER \
    NANO_XRCE_DATAREADERREPRESENTATION_INITIALIZER
#elif NANO_FEAT_OBJECT_KIND_DATAWRITER
#define NANO_XRCE_OBJECTVARIANTVALUE_ELEMENT_INITIALIZER \
    NANO_XRCE_DATAWRITERREPRESENTATION_INITIALIZER
#elif NANO_FEAT_OBJECT_KIND_SUBSCRIBER
#define NANO_XRCE_OBJECTVARIANTVALUE_ELEMENT_INITIALIZER \
    NANO_XRCE_SUBSCRIBERREPRESENTATION_INITIALIZER
#elif NANO_FEAT_OBJECT_KIND_PUBLISHER
#define NANO_XRCE_OBJECTVARIANTVALUE_ELEMENT_INITIALIZER \
    NANO_XRCE_PUBLISHERREPRESENTATION_INITIALIZER
#elif NANO_FEAT_OBJECT_KIND_TOPIC
#define NANO_XRCE_OBJECTVARIANTVALUE_ELEMENT_INITIALIZER \
    NANO_XRCE_TOPICREPRESENTATION_INITIALIZER
#elif NANO_FEAT_OBJECT_KIND_DOMAIN
#define NANO_XRCE_OBJECTVARIANTVALUE_ELEMENT_INITIALIZER \
    NANO_XRCE_DOMAINREPRESENTATION_INITIALIZER
#elif NANO_FEAT_OBJECT_KIND_TYPE
#define NANO_XRCE_OBJECTVARIANTVALUE_ELEMENT_INITIALIZER \
    NANO_XRCE_TYPEREPRESENTATION_INITIALIZER
#elif NANO_FEAT_OBJECT_KIND_QOSPROFILE
#define NANO_XRCE_OBJECTVARIANTVALUE_ELEMENT_INITIALIZER \
    NANO_XRCE_QOSPROFILEREPRESENTATION_INITIALIZER
#elif NANO_FEAT_OBJECT_KIND_PARTICIPANT
#define NANO_XRCE_OBJECTVARIANTVALUE_ELEMENT_INITIALIZER \
    NANO_XRCE_PARTICIPANTREPRESENTATION_INITIALIZER
#elif NANO_FEAT_OBJECT_KIND_APPLICATION
#define NANO_XRCE_OBJECTVARIANTVALUE_ELEMENT_INITIALIZER \
    NANO_XRCE_APPLICATIONREPRESENTATION_INITIALIZER
#else
#define NANO_XRCE_OBJECTVARIANTVALUE_ELEMENT_INITIALIZER \
    NANO_XRCE_CLIENTREPRESENTATION_INITIALIZER
#endif /* NANO_FEAT_OBJECT_KIND_APPLICATION */

#define NANO_XRCE_OBJECTVARIANTVALUE_INITIALIZER \
{\
    NANO_XRCE_OBJECTVARIANTVALUE_ELEMENT_INITIALIZER /* data_reader */\
     /* data_writer */\
}

/*i
 * @brief TODO
 */
typedef struct NANODllExport NANO_XRCE_ObjectVariantI
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
    NANO_XRCE_ObjectVariantValue value;
} NANO_XRCE_ObjectVariant;

#define NANO_XRCE_OBJECTVARIANT_INITIALIZER \
{\
    NANO_XRCE_OBJK_INVALID, /* kind */\
    NANO_XRCE_OBJECTVARIANTVALUE_INITIALIZER /* value */\
}

/*i @} *//* nano_api_xrce_payload_objrepr_common */

#endif /* NANO_FEAT_OBJECT */

#endif /* nano_core_xrce_repr_h */
