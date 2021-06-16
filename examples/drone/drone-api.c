#include "drone-api.h"

void serialize_sensor_data(
    const struct SensorCombined *const sensor_data,
    NANO_u8 *const buffer,
    NANO_usize *const buffer_len)
{
    NANO_CDR_Stream stream = NANO_CDR_STREAM_INITIALIZER;
    const NANO_usize buffer_max = *buffer_len;
    const char *str_ptr = NULL;

    *buffer_len = 0;

    NANO_CDR_Stream_initialize(
        &stream,
        buffer,
        buffer_max,
        NANO_CDR_ENDIANNESS_NATIVE,
        NANO_BOOL_FALSE /* owned */);

    NANO_CDR_Stream_serialize_u64(&stream, &sensor_data->timestamp);
    NANO_CDR_Stream_serialize_octets(&stream,
      (const NANO_u8*)sensor_data->gyro_rad, sizeof(sensor_data->gyro_rad));
    NANO_CDR_Stream_serialize_u32(&stream, &sensor_data->gyro_integral_dt);
    NANO_CDR_Stream_serialize_i32(&stream,
      &sensor_data->accelerometer_timestamp_relative);
    NANO_CDR_Stream_serialize_octets(&stream,
      (const NANO_u8*)sensor_data->accelerometer_m_s2,
      sizeof(sensor_data->accelerometer_m_s2));
    NANO_CDR_Stream_serialize_u32(&stream, &sensor_data->accelerometer_integral_dt);
    NANO_CDR_Stream_serialize_u8(&stream, &sensor_data->accelerometer_clipping);

    *buffer_len = NANO_CDR_Stream_offset(&stream);
}

int deserialize_sensor_data(
    struct SensorCombined *const sensor_data,
    const NANO_u8 *const buffer,
    const NANO_usize buffer_len,
    const NANO_bool little_endian)
{
    int rc = 1;
    const NANO_u8 *data_ptr = buffer;
    NANO_CDR_Stream stream = NANO_CDR_STREAM_INITIALIZER;
    char *str_buf = NULL;
    NANO_usize str_len = 0;

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_initialize(
                &stream,
                (NANO_u8*)buffer,
                buffer_len,
                little_endian,
                NANO_BOOL_FALSE))
    {
        goto done;
    }

    if (NANO_RETCODE_OK !=
            NANO_CDR_Stream_deserialize_u64(&stream, &sensor_data->timestamp))
    {
        goto done;
    }
    
    if (NANO_RETCODE_OK !=
        NANO_CDR_Stream_deserialize_octets(&stream,
          (NANO_u8 *)sensor_data->gyro_rad, sizeof(sensor_data->gyro_rad)))
    {
        goto done;
    }

    if (NANO_RETCODE_OK !=
        NANO_CDR_Stream_deserialize_u32(&stream, &sensor_data->gyro_integral_dt))
    {
        goto done;
    }

    if (NANO_RETCODE_OK !=
        NANO_CDR_Stream_deserialize_i32(&stream, &sensor_data->accelerometer_timestamp_relative))
    {
        goto done;
    }

    if (NANO_RETCODE_OK !=
        NANO_CDR_Stream_deserialize_octets(&stream,
          (NANO_u8 *)sensor_data->accelerometer_m_s2,
          sizeof(sensor_data->accelerometer_m_s2)))
    {
        goto done;
    }

    if (NANO_RETCODE_OK !=
        NANO_CDR_Stream_deserialize_u32(&stream, &sensor_data->accelerometer_integral_dt))
    {
        goto done;
    }

    if (NANO_RETCODE_OK !=
        NANO_CDR_Stream_deserialize_u8(&stream, &sensor_data->accelerometer_clipping))
    {
        goto done;
    }

    rc = 0;
done:
    return rc;
}
