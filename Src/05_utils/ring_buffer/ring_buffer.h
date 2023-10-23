#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint-gcc.h>

typedef volatile void* ring_buffer_handle_t;

typedef enum
{
    RB_ERR_OK,
    RB_ERR_NOT_OK,
    RB_ERR_FULL,
    RB_ERR_EMPTY,
    RB_ERR_NO_SPACE,
    RB_ERR_NOT_ENOUGH_DATA
} rb_err_t;

rb_err_t rb_init(ring_buffer_handle_t* handle, uint16_t size);

rb_err_t rb_deinit(ring_buffer_handle_t handle);

rb_err_t rb_insert(ring_buffer_handle_t handle, uint8_t data);

rb_err_t rb_insertMultiple(ring_buffer_handle_t handle, uint8_t *data, uint16_t data_size);

rb_err_t rb_pop(ring_buffer_handle_t handle, uint8_t *data);

rb_err_t rb_popMultiple(ring_buffer_handle_t handle, uint8_t *data, uint16_t data_len);

uint8_t rb_isFull(ring_buffer_handle_t handle);

uint8_t rb_isEmpty(ring_buffer_handle_t handle);

uint16_t rb_spaceLeft(ring_buffer_handle_t handle);

uint16_t rb_spaceUsed(ring_buffer_handle_t handle);

rb_err_t rb_clear(ring_buffer_handle_t handle);


#endif /* RING_BUFFER_H */
