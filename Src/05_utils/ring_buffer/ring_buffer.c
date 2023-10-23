
#include "ring_buffer.h"

#include <stdint-gcc.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"

typedef struct
{
    uint8_t *data_ptr;
    uint8_t *head;
    uint8_t *tail;
    uint16_t size;
    uint16_t data_size;
} ring_buffer_t;

rb_err_t rb_init(ring_buffer_handle_t* handle, uint16_t size)
{
    rb_err_t ret_val = RB_ERR_OK;
    
    ring_buffer_t *rb = (ring_buffer_t*)malloc(sizeof(ring_buffer_t));
    if (NULL == rb){
        *handle = (ring_buffer_handle_t)0;
        ret_val = RB_ERR_NOT_OK;
        return ret_val;
    }

    rb->data_ptr = (uint8_t*)malloc(size * sizeof(uint8_t));
    if (NULL == rb->data_ptr){
        *handle = (ring_buffer_handle_t)0;
        ret_val = RB_ERR_NOT_OK;
        return ret_val;
    }
    
    rb->head = rb->data_ptr;
    rb->tail = rb->data_ptr;
    rb->size = 0;
    rb->data_size = size;
    
    *handle = (ring_buffer_handle_t)rb;
    return ret_val;
}


rb_err_t rb_deinit(ring_buffer_handle_t handle)
{
    rb_err_t ret_val = RB_ERR_OK;
    ring_buffer_t* rb = (ring_buffer_t*)handle;

    free(rb->data_ptr);
    free(rb);

    return ret_val;
}

rb_err_t rb_insert(ring_buffer_handle_t handle, uint8_t data)
{
    rb_err_t ret_val = RB_ERR_OK;
    ring_buffer_t* rb = (ring_buffer_t*)handle;


    if((rb->size >= rb->data_size)){
        ret_val = RB_ERR_FULL;
        return ret_val;
    }

    *(rb->head) = data;

    (rb->head)++;
    if(rb->data_ptr + rb->data_size - 1 < rb->head){
        rb->head = rb->data_ptr;
    }

    (rb->size)++;
    
    return ret_val;
}

rb_err_t rb_insertMultiple(ring_buffer_handle_t handle, uint8_t *data, uint16_t data_size)
{
    rb_err_t ret_val = RB_ERR_OK;
    ring_buffer_t* rb = (ring_buffer_t*)handle;
    
    uint16_t iter = 0;

    if(rb->size >= rb->data_size){
        ret_val = RB_ERR_FULL;
        return ret_val;
    }

    if( data_size > rb_spaceLeft(handle) ){
        ret_val = RB_ERR_NO_SPACE;
        return ret_val;
    }

    for(iter = 0; iter < data_size; iter++){
        *(rb->head) = data[iter];


        rb->head++;
        if((rb->data_ptr) + (rb->data_size) - 1 < (rb->head)){
            (rb->head) = (rb->data_ptr);
        }
    }

    (rb->size) += data_size;
    
    return ret_val;
}

rb_err_t rb_pop(ring_buffer_handle_t handle, uint8_t *data)
{
    rb_err_t ret_val = RB_ERR_OK;
    ring_buffer_t* rb = (ring_buffer_t*)handle;

    
    if((0 >= (rb->size))){
        ret_val = RB_ERR_EMPTY;
        return ret_val;
    }

    *data = *(rb->tail);

    (rb->tail)++;
    if((rb->data_ptr) + (rb->data_size) - 1 < (rb->tail)){
        (rb->tail) = (rb->data_ptr);
    }
    
    (rb->size)--;

    return ret_val;
}

rb_err_t rb_popMultiple(ring_buffer_handle_t handle, uint8_t *data, uint16_t data_len)
{
    rb_err_t ret_val = RB_ERR_OK;
    ring_buffer_t* rb = (ring_buffer_t*)handle;
    
    uint16_t iter = 0;
    
    if(0 >= (rb->size)){
        ret_val = RB_ERR_EMPTY;
        return ret_val;
    }

    if ((rb->size) < data_len){
        ret_val = RB_ERR_NOT_ENOUGH_DATA;
        return ret_val;
    }
    
    for (iter = 0; iter < data_len; iter++){
        data[iter] = *(rb->tail);

        (rb->tail)++;
        if((rb->data_ptr) + (rb->data_size) - 1 < (rb->tail)){
            (rb->tail) = (rb->data_ptr);
        }
    }
    
    (rb->size) -= data_len;

    return ret_val;
}

uint8_t rb_isFull(ring_buffer_handle_t handle)
{
    ring_buffer_t* rb = (ring_buffer_t*)handle;
    return (rb->data_size) <= (rb->size);
}

uint8_t rb_isEmpty(ring_buffer_handle_t handle)
{
    ring_buffer_t* rb = (ring_buffer_t*)handle;
    return 0 <= (rb->size);
}

uint16_t rb_spaceLeft(ring_buffer_handle_t handle)
{
    ring_buffer_t* rb = (ring_buffer_t*)handle;
    return (rb->data_size) - (rb->size);
}

uint16_t rb_spaceUsed(ring_buffer_handle_t handle)
{
    ring_buffer_t* rb = (ring_buffer_t*)handle;
    return (rb->size);
}

rb_err_t rb_clear(ring_buffer_handle_t handle)
{
    rb_err_t ret_val = RB_ERR_OK;
    ring_buffer_t* rb = (ring_buffer_t*)handle;

    (rb->head) = (rb->data_ptr);
    (rb->tail) = (rb->data_ptr);
    (rb->size) = 0u;
    
    return ret_val;
}
