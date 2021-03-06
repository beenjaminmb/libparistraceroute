#ifndef LIBPT_PROTOCOL_FIELD_H
#define LIBPT_PROTOCOL_FIELD_H

/**
 * \file protocol_field.h
 * \brief Header for the data fields of a protocol
 */

#include <stdint.h>
#include <stdbool.h>

#include "use.h"    // USE_BITS
#include "field.h"

/**
 * \struct protocol_field_t
 * \brief Structure describing a data field for a protocol
 */

typedef struct {
    const char  * key;            /**< Pointer to an identifying key */
    fieldtype_t   type;           /**< Enum to set the type of data stored in the field */
    size_t        offset;         /**< Offset from start of segment data */
#ifdef USE_BITS
    // The following fields are usually set to 0 if the field is made of n*8bits and/or non-aligned fields.
    size_t        offset_in_bits; /**< Additional offset in bits. */
    size_t        size_in_bits;   /**< Set iff the size of the field in bits. */
#endif

    // The following callbacks allows to perform specific treatment when we translate
    // field content in packet content and vice versa. Most of time there are set
    // to NULL and we call default functions which manage endianness and so on.
    // Typical usage:
    // - field.size_in_bits % 8 != 0
    // - field.offset_in_bits != 0

    field_t     * (*get)(const uint8_t * segment);                  /**< Allocate a field_t instance corresponding to this field */
    bool          (*set)(uint8_t * segment, const field_t * field); /**< Update a segment according to a field. Return true iif successful */
} protocol_field_t;

/**
 * \brief Retrieve the size (in bytes) to a protocol field.
 * \param protocol_field A pointer to the protocol_field_t instance.
 * \return The corresponding size (in bytes).
 * If the protocol_field->type == TYPE_BITS, the size is in bits.
 */

size_t protocol_field_get_size(const protocol_field_t * protocol_field);

/**
 * \brief Retrieve the size (in bits) to a protocol field.
 * \param protocol_field A pointer to the protocol_field_t instance.
 * \return The corresponding size (in bits).
 */

size_t protocol_field_get_size_in_bits(const protocol_field_t * protocol_field);

/**
 * \brief Write in a segment (a section of packet) the value stored in a field
 *   according to the size and the offset stored in a protocol_field_t instance.
 * \param protocol_field A pointer to the corresponding protocol field.
 * \param segment The segment related to the layer we're setting.
 * \param field The field storing the value to write in the segment.
 * \return true iif successful
 */

bool protocol_field_set(const protocol_field_t * protocol_field, uint8_t * segment, const field_t * field);

/**
 * \brief Retrieve the offset stored in a protocol_field_t instance.
 * \param protocol_field A pointer to a protocol_field_t instance.
 * \return The corresponding offset.
 */

size_t protocol_field_get_offset(const protocol_field_t * protocol_field);

/**
 * \brief Print information related to a protocol_field_t instance
 * \param protocol_field The protocol_field_t instance to print
 */

void protocol_field_dump(const protocol_field_t * protocol_field);

#endif // LIBPT_PROTOCOL_FIELD_H
