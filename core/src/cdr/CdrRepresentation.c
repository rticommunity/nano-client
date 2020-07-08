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

#include "nano/nano_core.h"


#if NANO_FEAT_CDR

/******************************************************************************
 *                           AgentRepresentation
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_AgentRepresentation_deserialize_cdr(
    NANO_XRCE_AgentRepresentation *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;

#if NANO_FEAT_EPROSIMA
    /* TODO Deserialize result_status */
#endif /* NANO_FEAT_EPROSIMA */
    
    NANO_CHECK_RC(
        NANO_XRCE_Cookie_deserialize_cdr(&self->xrce_cookie, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE Cookie"));
    
    NANO_CHECK_RC(
        NANO_XRCE_Version_deserialize_cdr(&self->xrce_version, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE Version"));
    
    NANO_CHECK_RC(
        NANO_XRCE_VendorId_deserialize_cdr(&self->xrce_vendor_id, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE VendorId"));

    /* TODO Deserialize properties sequence */

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_AgentRepresentation_serialize_cdr(
    const NANO_XRCE_AgentRepresentation *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;

#if NANO_FEAT_EPROSIMA
    /* TODO Serialize result_status */
#endif /* NANO_FEAT_EPROSIMA */

    NANO_CHECK_RC(
        NANO_XRCE_Cookie_serialize_cdr(&self->xrce_cookie, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE Cookie"));
    
    NANO_CHECK_RC(
        NANO_XRCE_Version_serialize_cdr(&self->xrce_version, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE Version"));
    
    NANO_CHECK_RC(
        NANO_XRCE_VendorId_serialize_cdr(&self->xrce_vendor_id, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE VendorId"));
    
    /* TODO Serialize properties sequence */
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

/******************************************************************************
 *                          ClientRepresentation
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_ClientRepresentation_deserialize_cdr(
    NANO_XRCE_ClientRepresentation *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;

    NANO_CHECK_RC(
        NANO_XRCE_Cookie_deserialize_cdr(&self->xrce_cookie, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE Cookie"));

    NANO_CHECK_RC(
        NANO_XRCE_Version_deserialize_cdr(&self->xrce_version, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE Version"));

    NANO_CHECK_RC(
        NANO_XRCE_VendorId_deserialize_cdr(&self->xrce_vendor_id, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE VendorId"));

    NANO_CHECK_RC(
        NANO_XRCE_ClientKey_deserialize_cdr(&self->client_key, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE ClientKey"));

    NANO_CHECK_RC(
        NANO_XRCE_SessionId_deserialize_cdr(&self->session_id, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE SessionId"));
    
    NANO_CHECK_RC(
        NANO_bool_deserialize_cdr(&self->has_properties, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE has_properties"));

#if NANO_FEAT_OPTIONAL && \
    NANO_FEAT_PROPERTY
    if (self->has_properties)
    {
        NANO_CHECK_RC(
            NANO_PropertySeq_deserialize_cdr(&self->properties, stream),
            NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE properties"));
    }
#endif

#if NANO_FEAT_MTU_IN_CLIENT_REPR
    NANO_CHECK_RC(
        NANO_u16_deserialize_cdr(&self->mtu, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE mtu"));
#endif /* NANO_FEAT_MTU_IN_CLIENT_REPR */

    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_ClientRepresentation_serialize_cdr(
    const NANO_XRCE_ClientRepresentation *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;

    NANO_CHECK_RC(
        NANO_XRCE_Cookie_serialize_cdr(&self->xrce_cookie, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE Cookie"));

    NANO_CHECK_RC(
        NANO_XRCE_Version_serialize_cdr(&self->xrce_version, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE Version"));

    NANO_CHECK_RC(
        NANO_XRCE_VendorId_serialize_cdr(&self->xrce_vendor_id, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE VendorId"));

    NANO_CHECK_RC(
        NANO_XRCE_ClientKey_serialize_cdr(&self->client_key, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE ClientKey"));

    NANO_CHECK_RC(
        NANO_XRCE_SessionId_serialize_cdr(&self->session_id, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE SessionId"));

#if NANO_FEAT_OPTIONAL && \
    NANO_FEAT_PROPERTY
    if (self->has_properties)
    {
        NANO_CHECK_RC(
            NANO_PropertySeq_serialize_cdr(&self->properties, stream),
            NANO_LOG_ERROR_MSG("FAILED to SERIALIZE properties"));
    }
#endif

#if NANO_FEAT_MTU_IN_CLIENT_REPR
    NANO_CHECK_RC(
        NANO_u16_serialize_cdr(&self->mtu, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE mtu"));
#endif /* NANO_FEAT_MTU_IN_CLIENT_REPR */

    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}

#endif /* NANO_FEAT_CDR */

#if NANO_FEAT_CDR

#if NANO_FEAT_OBJECT && NANO_FEAT_AGENT
/******************************************************************************
 *                              RepresentationFormats
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_RepresentationFormats_deserialize_cdr(
    NANO_XRCE_RepresentationFormat *const format,
    NANO_XRCE_RefRepresentationFormat *const ref,
    NANO_XRCE_XmlRepresentationFormat *const xml,
    NANO_XRCE_BinRepresentationFormat *const bin,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;

    NANO_LOG_FN_ENTRY
    
    NANO_CHECK_RC(
        NANO_XRCE_RepresentationFormat_deserialize_cdr(format, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE RepresentationFormat"));
    
    NANO_LOG_DEBUG("DESERIALIZE object representation",
        NANO_LOG_H8("format",*format))

    switch (*format)
    {
    case NANO_XRCE_REPRESENTATION_BY_REFERENCE:
    {
        if (ref == NULL)
        {
            NANO_LOG_ERROR_MSG("representation by reference NOT SUPPORTED")
            rc = NANO_RETCODE_INVALID_ARGS;
            goto done;
        }

        NANO_CHECK_RC(
            NANO_XRCE_RefRepresentationFormat_deserialize_cdr(ref, stream),
            NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE RefRepresentation"));
        break;
    }
    case NANO_XRCE_REPRESENTATION_AS_XML_STRING:
    {
        if (xml == NULL)
        {
            NANO_LOG_ERROR_MSG("representation by xml NOT SUPPORTED")
            rc = NANO_RETCODE_INVALID_ARGS;
            goto done;
        }

        NANO_CHECK_RC(
            NANO_XRCE_XmlRepresentationFormat_deserialize_cdr(xml, stream),
            NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE XmLRepresentation"));
        break;
    }
    case NANO_XRCE_REPRESENTATION_IN_BINARY:
    {
        if (bin == NULL)
        {
            NANO_LOG_ERROR_MSG("representation by binary NOT SUPPORTED")
            rc = NANO_RETCODE_INVALID_ARGS;
            goto done;
        }

        NANO_CHECK_RC(
            NANO_XRCE_BinRepresentationFormat_deserialize_cdr(bin, stream),
            NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE BinRepresentation"));
        break;
    }
    default:
    {
        NANO_LOG_ERROR_MSG("UNKNOWN representation")
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }
    }

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_RepresentationFormats_serialize_cdr(
    const NANO_XRCE_RepresentationFormat *const format,
    const NANO_XRCE_RefRepresentationFormat *const ref,
    const NANO_XRCE_XmlRepresentationFormat *const xml,
    const NANO_XRCE_BinRepresentationFormat *const bin,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;

    NANO_LOG_FN_ENTRY

    NANO_CHECK_RC(
        NANO_XRCE_RepresentationFormat_serialize_cdr(format, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE RepresentationFormat"));

    switch (*format)
    {
    case NANO_XRCE_REPRESENTATION_BY_REFERENCE:
    {
        if (ref == NULL)
        {
            NANO_LOG_ERROR_MSG("representation by reference NOT SUPPORTED")
            rc = NANO_RETCODE_INVALID_ARGS;
            goto done;
        }

        NANO_CHECK_RC(
            NANO_XRCE_RefRepresentationFormat_serialize_cdr(ref, stream),
            NANO_LOG_ERROR_MSG("FAILED to SERIALIZE RefRepresentation"));
        break;
    }
    case NANO_XRCE_REPRESENTATION_AS_XML_STRING:
    {
        if (xml == NULL)
        {
            NANO_LOG_ERROR_MSG("representation by xml NOT SUPPORTED")
            rc = NANO_RETCODE_INVALID_ARGS;
            goto done;
        }

        NANO_CHECK_RC(
            NANO_XRCE_XmlRepresentationFormat_serialize_cdr(xml, stream),
            NANO_LOG_ERROR_MSG("FAILED to SERIALIZE XmlRepresentation"));
        break;
    }
    case NANO_XRCE_REPRESENTATION_IN_BINARY:
    {
        if (bin == NULL)
        {
            NANO_LOG_ERROR_MSG("representation by binary NOT SUPPORTED")
            rc = NANO_RETCODE_INVALID_ARGS;
            goto done;
        }

        NANO_CHECK_RC(
            NANO_XRCE_BinRepresentationFormat_serialize_cdr(bin, stream),
            NANO_LOG_ERROR_MSG("FAILED to SERIALIZE BinRepresentation"));
        break;
    }
    default:
    {
        NANO_LOG_ERROR_MSG("UNKNOWN representation")
        rc = NANO_RETCODE_INVALID_ARGS;
        goto done;
    }
    }

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

#if NANO_FEAT_OBJECT_KIND_PARTICIPANT
/******************************************************************************
 *                     DomainParticipantRepresentation
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_DomainParticipantRepresentation_deserialize_cdr(
    NANO_XRCE_DomainParticipantRepresentation *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;

    NANO_LOG_FN_ENTRY
    
    NANO_CHECK_RC(
        NANO_XRCE_Representation3_deserialize_cdr(&self->base, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE Representation3"));

    NANO_CHECK_RC(
        NANO_XRCE_DomainId_deserialize_cdr(&self->domain_id, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE DomainId"));

    NANO_LOG_DEBUG("XML DP repr",
        NANO_LOG_STR("xml",self->base.repr.value.xml.value))
    
    rc = NANO_RETCODE_OK;
done:
    NANO_LOG_FN_EXIT_RC(rc)
    return rc;
}

NANO_RetCode
NANO_XRCE_DomainParticipantRepresentation_serialize_cdr(
    const NANO_XRCE_DomainParticipantRepresentation *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_Representation3_serialize_cdr(&self->base, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE Representation3"));

    NANO_CHECK_RC(
        NANO_XRCE_DomainId_serialize_cdr(&self->domain_id, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE DomainId"));
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}


NANO_RetCode
NANO_XRCE_DomainParticipantBinary_deserialize_cdr(
    NANO_XRCE_DomainParticipantBinary *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;

    NANO_OPTIONALMEMBER_DESERIALIZE(self, domain_ref, stream,
            NANO_XRCE_ObjectRef_deserialize_cdr,
            rc);
    NANO_CHECK_RC(rc,NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE ObjectRef"));

    NANO_OPTIONALMEMBER_DESERIALIZE(self, qos_profile_ref, stream,
            NANO_XRCE_ObjectRef_deserialize_cdr,
            rc);
    NANO_CHECK_RC(rc,NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE ObjectRef"));
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_DomainParticipantBinary_serialize_cdr(
    const NANO_XRCE_DomainParticipantBinary *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_OPTIONALMEMBER_SERIALIZE(self, domain_ref, stream,
            NANO_XRCE_ObjectRef_serialize_cdr,
            rc);
    NANO_CHECK_RC(rc,NANO_LOG_ERROR_MSG("FAILED to SERIALIZE ObjectRef"));

    NANO_OPTIONALMEMBER_SERIALIZE(self, qos_profile_ref, stream,
            NANO_XRCE_ObjectRef_serialize_cdr,
            rc);
    NANO_CHECK_RC(rc,NANO_LOG_ERROR_MSG("FAILED to SERIALIZE ObjectRef"));
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

#endif /* NANO_FEAT_OBJECT_KIND_PARTICIPANT */

#if NANO_FEAT_OBJECT_KIND_TOPIC
/******************************************************************************
 *                          TopicRepresentation
 ******************************************************************************/
NANO_RetCode
NANO_XRCE_TopicRepresentation_deserialize_cdr(
    NANO_XRCE_TopicRepresentation *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_Representation3_deserialize_cdr(&self->base, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE Representation3"));

    NANO_CHECK_RC(
        NANO_XRCE_ObjectId_deserialize_cdr(&self->participant_id, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE ObjectId"));
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_TopicRepresentation_serialize_cdr(
    const NANO_XRCE_TopicRepresentation *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_Representation3_serialize_cdr(&self->base, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE Representation3"));

    NANO_CHECK_RC(
        NANO_XRCE_ObjectId_serialize_cdr(&self->participant_id, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE ObjectId"));
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}


NANO_RetCode
NANO_XRCE_TopicBinary_deserialize_cdr(
    NANO_XRCE_TopicBinary *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_TopicName_deserialize_cdr(&self->topic_name,stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE TopicName"));

    NANO_OPTIONALMEMBER_DESERIALIZE(self,type_ref,stream,
        NANO_XRCE_TypeRef_deserialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE TypeRef"));

    NANO_OPTIONALMEMBER_DESERIALIZE(self,type_id,stream,
        NANO_XRCE_TypeId_deserialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE TypeId"));
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_TopicBinary_serialize_cdr(
    const NANO_XRCE_TopicBinary *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_TopicName_serialize_cdr(&self->topic_name,stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE TopicName"));

    NANO_OPTIONALMEMBER_SERIALIZE(self,type_ref,stream,
        NANO_XRCE_TypeRef_serialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to SERIALIZE TypeRef"));

    NANO_OPTIONALMEMBER_SERIALIZE(self,type_id,stream,
        NANO_XRCE_TypeId_serialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to SERIALIZE TypeId"));

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

#endif /* NANO_FEAT_OBJECT_KIND_TOPIC */

#if NANO_FEAT_OBJECT_KIND_TOPIC || \
    NANO_FEAT_OBJECT_KIND_PUBLISHER || \
    NANO_FEAT_OBJECT_KIND_SUBSCRIBER || \
    NANO_FEAT_OBJECT_KIND_DATAWRITER || \
    NANO_FEAT_OBJECT_KIND_DATAREADER

/******************************************************************************
 *                          ContainedBinAndXmlObject
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_ContainedBinAndXmlObjectRepresentation_deserialize_cdr(
    NANO_XRCE_ContainedBinAndXmlObjectRepresentation *self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;

    NANO_CHECK_RC(
        NANO_XRCE_RepresentationBinAndXml_deserialize_cdr(&self->base, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE RepresentationBinAndXml"));

    NANO_CHECK_RC(
        NANO_XRCE_ObjectId_deserialize_cdr(&self->parent_id, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE ObjectId"));

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_ContainedBinAndXmlObjectRepresentation_serialize_cdr(
    const NANO_XRCE_ContainedBinAndXmlObjectRepresentation *self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_RepresentationBinAndXml_serialize_cdr(&self->base, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE RepresentationBinAndXml"));

    NANO_CHECK_RC(
        NANO_XRCE_ObjectId_serialize_cdr(&self->parent_id, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE ObjectId"));
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

#endif /* NANO_FEAT_OBJECT_KIND_TOPIC || \
          NANO_FEAT_OBJECT_KIND_PUBLISHER || \
          NANO_FEAT_OBJECT_KIND_SUBSCRIBER || \
          NANO_FEAT_OBJECT_KIND_DATAWRITER || \
          NANO_FEAT_OBJECT_KIND_DATAREADER */

#if NANO_FEAT_OBJECT_KIND_PUBLISHER || \
    NANO_FEAT_OBJECT_KIND_SUBSCRIBER
/******************************************************************************
 *                               PartitionSeq
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_Partition_deserialize_cdr(
    NANO_XRCE_Partition *const self,
    NANO_CDR_Stream *const stream)
{
    return NANO_String_deserialize_cdr_max(
                &self->value, stream, NANO_STRING_LENGTH_UNBOUNDED);
}

NANO_RetCode
NANO_XRCE_Partition_serialize_cdr(
    const NANO_XRCE_Partition *const self,
    NANO_CDR_Stream *const stream)
{
    return NANO_String_serialize_cdr_max(
                &self->value, stream, NANO_STRING_LENGTH_UNBOUNDED);
}

NANO_usize
NANO_XRCE_Partition_serialized_size_cdr(
    const NANO_XRCE_Partition *const self,
    const NANO_CDR_Stream *const stream)
{
    return NANO_String_serialized_size_cdr_max(
                &self->value, stream, NANO_STRING_LENGTH_UNBOUNDED);
}


/******************************************************************************
 *                              EndpointContainer
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_EndpointContainerQosBinary_deserialize_cdr(
    NANO_XRCE_EndpointContainerQosBinary *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_OPTIONALMEMBER_DESERIALIZE(self,partitions,stream,
            NANO_XRCE_PartitionSeq_deserialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE PartitionSeq"));

    NANO_OPTIONALMEMBER_DESERIALIZE(self,group_data,stream,
            NANO_XRCE_GroupData_deserialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE GroupData"));
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_EndpointContainerQosBinary_serialize_cdr(
    const NANO_XRCE_EndpointContainerQosBinary *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_OPTIONALMEMBER_SERIALIZE(self,partitions,stream,
            NANO_XRCE_PartitionSeq_serialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to SERIALIZE PartitionSeq"));

    NANO_OPTIONALMEMBER_SERIALIZE(self,group_data,stream,
            NANO_XRCE_GroupData_serialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to SERIALIZE GroupData"));

    rc = NANO_RETCODE_OK;
done:
    return rc;
}


NANO_RetCode
NANO_XRCE_EndpointContainerBinary_deserialize_cdr(
    NANO_XRCE_EndpointContainerBinary *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_OPTIONALMEMBER_DESERIALIZE(self,entity_name,stream,
            NANO_XRCE_EntityName_deserialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE EntityName"));

    NANO_OPTIONALMEMBER_DESERIALIZE(self,qos,stream,
            NANO_XRCE_EndpointContainerQosBinary_deserialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE EndpointContainerQosBinary"));
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_EndpointContainerBinary_serialize_cdr(
    const NANO_XRCE_EndpointContainerBinary *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_OPTIONALMEMBER_SERIALIZE(self,entity_name,stream,
            NANO_XRCE_EntityName_serialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to SERIALIZE EntityName"));

    NANO_OPTIONALMEMBER_SERIALIZE(self,qos,stream,
            NANO_XRCE_EndpointContainerQosBinary_serialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to SERIALIZE EndpointContainerQosBinary"));

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

#endif /* NANO_FEAT_OBJECT_KIND_PUBLISHER || \
            NANO_FEAT_OBJECT_KIND_SUBSCRIBER */


#if NANO_FEAT_OBJECT_KIND_DATAWRITER || \
    NANO_FEAT_OBJECT_KIND_DATAREADER
/******************************************************************************
 *                            EndpointQosBinary
 ******************************************************************************/


NANO_RetCode
NANO_XRCE_EndpointQosBinary_deserialize_cdr(
    NANO_XRCE_EndpointQosBinary *const self,
    NANO_CDR_Stream *stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_EndpointQosFlags_deserialize_cdr(&self->qos_flags, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE EndpointQosFlags"));

    NANO_OPTIONALMEMBER_DESERIALIZE(self,history_depth,stream,
            NANO_u16_deserialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE history_depth"));

    NANO_OPTIONALMEMBER_DESERIALIZE(self,deadline_msec,stream,
            NANO_u32_deserialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE deadline_msec"));

    NANO_OPTIONALMEMBER_DESERIALIZE(self,lifespan_msec,stream,
            NANO_u32_deserialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE lifespan_msec"));

    NANO_OPTIONALMEMBER_DESERIALIZE(self,user_data,stream,
            NANO_XRCE_UserData_deserialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE user_data"));
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_EndpointQosBinary_serialize_cdr(
    const NANO_XRCE_EndpointQosBinary *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;

    NANO_CHECK_RC(
        NANO_XRCE_EndpointQosFlags_serialize_cdr(&self->qos_flags, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE EndpointQosFlags"));

    NANO_OPTIONALMEMBER_SERIALIZE(self,history_depth,stream,
            NANO_u16_serialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to SERIALIZE history_depth"));

    NANO_OPTIONALMEMBER_SERIALIZE(self,deadline_msec,stream,
            NANO_u32_serialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to SERIALIZE deadline_msec"));

    NANO_OPTIONALMEMBER_SERIALIZE(self,lifespan_msec,stream,
            NANO_u32_serialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to SERIALIZE lifespan_msec"));

    NANO_OPTIONALMEMBER_SERIALIZE(self,user_data,stream,
            NANO_XRCE_UserData_serialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to SERIALIZE user_data"));

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

#endif /* NANO_FEAT_OBJECT_KIND_DATAWRITER || \
            NANO_FEAT_OBJECT_KIND_DATAREADER */

#if NANO_FEAT_OBJECT_KIND_DATAWRITER
/******************************************************************************
 *                        DataWriterRepresentation
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_DataWriterQosBinary_deserialize_cdr(
    NANO_XRCE_DataWriterQosBinary *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_EndpointQosBinary_deserialize_cdr(&self->base, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE EndpointQosBinary"));

    NANO_OPTIONALMEMBER_DESERIALIZE(self,ownership_strength,stream,
            NANO_u32_deserialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE ownership_strength"));
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_DataWriterQosBinary_serialize_cdr(
    const NANO_XRCE_DataWriterQosBinary *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_EndpointQosBinary_serialize_cdr(&self->base, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE EndpointQosBinary"));

    NANO_OPTIONALMEMBER_SERIALIZE(self,ownership_strength,stream,
            NANO_u32_serialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to SERIALIZE ownership_strength"));

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_DataWriterBinary_deserialize_cdr(
    NANO_XRCE_DataWriterBinary *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_TopicName_deserialize_cdr(&self->topic_name, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE TopicName"));

    NANO_OPTIONALMEMBER_DESERIALIZE(self,qos,stream,
            NANO_XRCE_DataWriterQosBinary_deserialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE DataWriterQosBinary"));
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_DataWriterBinary_serialize_cdr(
    const NANO_XRCE_DataWriterBinary *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_TopicName_serialize_cdr(&self->topic_name, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE TopicName"));

    NANO_OPTIONALMEMBER_SERIALIZE(self,qos,stream,
            NANO_XRCE_DataWriterQosBinary_serialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to SERIALIZE DataWriterQosBinary"));

    rc = NANO_RETCODE_OK;
done:
    return rc;
}
#endif /* NANO_FEAT_OBJECT_KIND_DATAWRITER */

#if NANO_FEAT_OBJECT_KIND_DATAREADER
/******************************************************************************
 *                        DataReaderRepresentation
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_DataReaderQosBinary_deserialize_cdr(
    NANO_XRCE_DataReaderQosBinary *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_EndpointQosBinary_deserialize_cdr(&self->base, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE EndpointQosBinary"));

    NANO_OPTIONALMEMBER_DESERIALIZE(self,timebased_filter_msec,stream,
            NANO_u32_deserialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE timebased_filter_msec"));

    NANO_OPTIONALMEMBER_DESERIALIZE(self,contentbased_filter,stream,
            NANO_XRCE_ContentFilterExpr_deserialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE contentbased_filter"));
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_DataReaderQosBinary_serialize_cdr(
    const NANO_XRCE_DataReaderQosBinary *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_EndpointQosBinary_serialize_cdr(&self->base, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE EndpointQosBinary"));

    NANO_OPTIONALMEMBER_SERIALIZE(self,timebased_filter_msec,stream,
            NANO_u32_serialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to SERIALIZE timebased_filter_msec"));

    NANO_OPTIONALMEMBER_SERIALIZE(self,contentbased_filter,stream,
            NANO_XRCE_ContentFilterExpr_serialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to SERIALIZE contentbased_filter"));

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_DataReaderBinary_deserialize_cdr(
    NANO_XRCE_DataReaderBinary *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_TopicName_deserialize_cdr(&self->topic_name, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE TopicName"));

    NANO_OPTIONALMEMBER_DESERIALIZE(self,qos,stream,
            NANO_XRCE_DataReaderQosBinary_deserialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE DataReaderQosBinary"));
    
    rc = NANO_RETCODE_OK;
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_DataReaderBinary_serialize_cdr(
    const NANO_XRCE_DataReaderBinary *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_TopicName_serialize_cdr(&self->topic_name, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE TopicName"));

    NANO_OPTIONALMEMBER_SERIALIZE(self,qos,stream,
            NANO_XRCE_DataReaderQosBinary_serialize_cdr, rc);
    NANO_CHECK_RC(rc, NANO_LOG_ERROR_MSG("FAILED to SERIALIZE DataReaderQosBinary"));

    rc = NANO_RETCODE_OK;
done:
    return rc;
}

#endif /* NANO_FEAT_OBJECT_KIND_DATAREADER */

/******************************************************************************
 *                          ObjectVariant
 ******************************************************************************/

NANO_RetCode
NANO_XRCE_ObjectVariant_deserialize_cdr(
    NANO_XRCE_ObjectVariant *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;

    NANO_LOG_FN_ENTRY
    
    NANO_CHECK_RC(
        NANO_XRCE_ObjectKind_deserialize_cdr(&self->kind, stream),
        NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE ObjectKind"));

    switch (self->kind)
    {
    case NANO_XRCE_OBJK_PARTICIPANT:
    {
        NANO_CHECK_RC(
            NANO_XRCE_DomainParticipantRepresentation_deserialize_cdr(
                &self->value.participant, stream),
            NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE DomainParticipantRepresentation"));
        break;
    }
    case NANO_XRCE_OBJK_TOPIC:
    {
        NANO_CHECK_RC(
            NANO_XRCE_TopicRepresentation_deserialize_cdr(
                &self->value.topic, stream),
            NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE TopicRepresentation"));
        break;
    }
    case NANO_XRCE_OBJK_PUBLISHER:
    {
        NANO_CHECK_RC(
            NANO_XRCE_PublisherRepresentation_deserialize_cdr(
                &self->value.publisher, stream),
            NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE PublisherRepresentation"));
        break;
    }
    case NANO_XRCE_OBJK_SUBSCRIBER:
    {
        NANO_CHECK_RC(
            NANO_XRCE_SubscriberRepresentation_deserialize_cdr(
                &self->value.subscriber, stream),
            NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE SubscriberRepresentation"));
        break;
    }
    case NANO_XRCE_OBJK_DATAWRITER:
    {
        NANO_CHECK_RC(
            NANO_XRCE_DataWriterRepresentation_deserialize_cdr(
                &self->value.data_writer, stream),
            NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE DataWriterRepresentation"));
        break;
    }
    case NANO_XRCE_OBJK_DATAREADER:
    {
        NANO_CHECK_RC(
            NANO_XRCE_DataReaderRepresentation_deserialize_cdr(
                &self->value.data_reader, stream),
            NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE DataReaderRepresentation"));
        break;
    }
    case NANO_XRCE_OBJK_DOMAIN:
    {
        NANO_CHECK_RC(
            NANO_XRCE_DomainRepresentation_deserialize_cdr(
                &self->value.domain, stream),
            NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE DomainRepresentation"));
        break;
    }
    case NANO_XRCE_OBJK_QOSPROFILE:
    {
        NANO_CHECK_RC(
            NANO_XRCE_QosProfileRepresentation_deserialize_cdr(
                &self->value.qos_profile, stream),
            NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE QosProfileRepresentation"));
        break;
    }
    case NANO_XRCE_OBJK_TYPE:
    {
        NANO_CHECK_RC(
            NANO_XRCE_TypeRepresentation_deserialize_cdr(
                &self->value.type, stream),
            NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE TypeRepresentation"));
        break;
    }
    case NANO_XRCE_OBJK_APPLICATION:
    {
        NANO_CHECK_RC(
            NANO_XRCE_ApplicationRepresentation_deserialize_cdr(
                &self->value.application, stream),
            NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE ApplicationRepresentation"));
        break;
    }
    case NANO_XRCE_OBJK_AGENT:
    {
        NANO_CHECK_RC(
            NANO_XRCE_AgentRepresentation_deserialize_cdr(
                &self->value.agent, stream),
            NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE AgentRepresentation"));
        break;
    }
    case NANO_XRCE_OBJK_CLIENT:
    {
        NANO_CHECK_RC(
            NANO_XRCE_ClientRepresentation_deserialize_cdr(
                &self->value.client, stream),
            NANO_LOG_ERROR_MSG("FAILED to DESERIALIZE ClientRepresentation"));
        break;
    }
    default:
        goto done;
    }
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}

NANO_RetCode
NANO_XRCE_ObjectVariant_serialize_cdr(
    const NANO_XRCE_ObjectVariant *const self,
    NANO_CDR_Stream *const stream)
{
    NANO_RetCode rc = NANO_RETCODE_ERROR;
    
    NANO_CHECK_RC(
        NANO_XRCE_ObjectKind_serialize_cdr(&self->kind, stream),
        NANO_LOG_ERROR_MSG("FAILED to SERIALIZE ObjectKind"));

    switch (self->kind)
    {
    case NANO_XRCE_OBJK_PARTICIPANT:
    {
        NANO_CHECK_RC(
            NANO_XRCE_DomainParticipantRepresentation_serialize_cdr(
                &self->value.participant, stream),
            NANO_LOG_ERROR_MSG("FAILED to SERIALIZE DomainParticipantRepresentation"));
        break;
    }
    case NANO_XRCE_OBJK_TOPIC:
    {
        NANO_CHECK_RC(
            NANO_XRCE_TopicRepresentation_serialize_cdr(
                &self->value.topic, stream),
            NANO_LOG_ERROR_MSG("FAILED to SERIALIZE TopicRepresentation"));
        break;
    }
    case NANO_XRCE_OBJK_PUBLISHER:
    {
        NANO_CHECK_RC(
            NANO_XRCE_PublisherRepresentation_serialize_cdr(
                &self->value.publisher, stream),
            NANO_LOG_ERROR_MSG("FAILED to SERIALIZE PublisherRepresentation"));
        break;
    }
    case NANO_XRCE_OBJK_SUBSCRIBER:
    {
        NANO_CHECK_RC(
            NANO_XRCE_SubscriberRepresentation_serialize_cdr(
                &self->value.subscriber, stream),
            NANO_LOG_ERROR_MSG("FAILED to SERIALIZE SubscriberRepresentation"));
        break;
    }
    case NANO_XRCE_OBJK_DATAWRITER:
    {
        NANO_CHECK_RC(
            NANO_XRCE_DataWriterRepresentation_serialize_cdr(
                &self->value.data_writer, stream),
            NANO_LOG_ERROR_MSG("FAILED to SERIALIZE DataWriterRepresentation"));
        break;
    }
    case NANO_XRCE_OBJK_DATAREADER:
    {
        NANO_CHECK_RC(
            NANO_XRCE_DataReaderRepresentation_serialize_cdr(
                &self->value.data_reader, stream),
            NANO_LOG_ERROR_MSG("FAILED to SERIALIZE DataReaderRepresentation"));
        break;
    }
    case NANO_XRCE_OBJK_DOMAIN:
    {
        NANO_CHECK_RC(
            NANO_XRCE_DomainRepresentation_serialize_cdr(
                &self->value.domain, stream),
            NANO_LOG_ERROR_MSG("FAILED to SERIALIZE DomainRepresentation"));
        break;
    }
    case NANO_XRCE_OBJK_QOSPROFILE:
    {
        NANO_CHECK_RC(
            NANO_XRCE_QosProfileRepresentation_serialize_cdr(
                &self->value.qos_profile, stream),
            NANO_LOG_ERROR_MSG("FAILED to SERIALIZE QosProfileRepresentation"));
        break;
    }
    case NANO_XRCE_OBJK_TYPE:
    {
        NANO_CHECK_RC(
            NANO_XRCE_TypeRepresentation_serialize_cdr(
                &self->value.type, stream),
            NANO_LOG_ERROR_MSG("FAILED to SERIALIZE TypeRepresentation"));
        break;
    }
    case NANO_XRCE_OBJK_APPLICATION:
    {
        NANO_CHECK_RC(
            NANO_XRCE_ApplicationRepresentation_serialize_cdr(
                &self->value.application, stream),
            NANO_LOG_ERROR_MSG("FAILED to SERIALIZE ApplicationRepresentation"));
        break;
    }
    case NANO_XRCE_OBJK_AGENT:
    {
        NANO_CHECK_RC(
            NANO_XRCE_AgentRepresentation_serialize_cdr(
                &self->value.agent, stream),
            NANO_LOG_ERROR_MSG("FAILED to SERIALIZE AgentRepresentation"));
        break;
    }
    case NANO_XRCE_OBJK_CLIENT:
    {
        NANO_CHECK_RC(
            NANO_XRCE_ClientRepresentation_serialize_cdr(
                &self->value.client, stream),
            NANO_LOG_ERROR_MSG("FAILED to SERIALIZE ClientRepresentation"));
        break;
    }
    default:
        goto done;
    }
    
    rc = NANO_RETCODE_OK;
    
done:
    return rc;
}

#endif /* NANO_FEAT_OBJECT */

#endif /* NANO_FEAT_CDR */