// THIS IS AN AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY
// BY HAND!!
//
// Generated by lcm-gen

#include <string.h>
#include "lcmtypes/srcl_lcm_msgs_LaserScanPoints_t.h"

static int __srcl_lcm_msgs_LaserScanPoints_t_hash_computed;
static uint64_t __srcl_lcm_msgs_LaserScanPoints_t_hash;

uint64_t __srcl_lcm_msgs_LaserScanPoints_t_hash_recursive(const __lcm_hash_ptr *p)
{
    const __lcm_hash_ptr *fp;
    for (fp = p; fp != NULL; fp = fp->parent)
        if (fp->v == __srcl_lcm_msgs_LaserScanPoints_t_get_hash)
            return 0;

    __lcm_hash_ptr cp;
    cp.parent =  p;
    cp.v = __srcl_lcm_msgs_LaserScanPoints_t_get_hash;
    (void) cp;

    uint64_t hash = (uint64_t)0x41cb5b1499974f70LL
         + __srcl_lcm_msgs_Pose_t_hash_recursive(&cp)
         + __int64_t_hash_recursive(&cp)
         + __srcl_lcm_msgs_Point3Df_t_hash_recursive(&cp)
         + __srcl_lcm_msgs_QuadrotorTransform_hash_recursive(&cp)
        ;

    return (hash<<1) + ((hash>>63)&1);
}

int64_t __srcl_lcm_msgs_LaserScanPoints_t_get_hash(void)
{
    if (!__srcl_lcm_msgs_LaserScanPoints_t_hash_computed) {
        __srcl_lcm_msgs_LaserScanPoints_t_hash = (int64_t)__srcl_lcm_msgs_LaserScanPoints_t_hash_recursive(NULL);
        __srcl_lcm_msgs_LaserScanPoints_t_hash_computed = 1;
    }

    return __srcl_lcm_msgs_LaserScanPoints_t_hash;
}

int __srcl_lcm_msgs_LaserScanPoints_t_encode_array(void *buf, int offset, int maxlen, const srcl_lcm_msgs_LaserScanPoints_t *p, int elements)
{
    int pos = 0, element;
    int thislen;

    for (element = 0; element < elements; element++) {

        thislen = __srcl_lcm_msgs_Pose_t_encode_array(buf, offset + pos, maxlen - pos, &(p[element].trans_sensor2world), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &(p[element].point_num), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __srcl_lcm_msgs_Point3Df_t_encode_array(buf, offset + pos, maxlen - pos, p[element].points, p[element].point_num);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __srcl_lcm_msgs_QuadrotorTransform_encode_array(buf, offset + pos, maxlen - pos, &(p[element].pose), 1);
        if (thislen < 0) return thislen; else pos += thislen;

    }
    return pos;
}

int srcl_lcm_msgs_LaserScanPoints_t_encode(void *buf, int offset, int maxlen, const srcl_lcm_msgs_LaserScanPoints_t *p)
{
    int pos = 0, thislen;
    int64_t hash = __srcl_lcm_msgs_LaserScanPoints_t_get_hash();

    thislen = __int64_t_encode_array(buf, offset + pos, maxlen - pos, &hash, 1);
    if (thislen < 0) return thislen; else pos += thislen;

    thislen = __srcl_lcm_msgs_LaserScanPoints_t_encode_array(buf, offset + pos, maxlen - pos, p, 1);
    if (thislen < 0) return thislen; else pos += thislen;

    return pos;
}

int __srcl_lcm_msgs_LaserScanPoints_t_encoded_array_size(const srcl_lcm_msgs_LaserScanPoints_t *p, int elements)
{
    int size = 0, element;
    for (element = 0; element < elements; element++) {

        size += __srcl_lcm_msgs_Pose_t_encoded_array_size(&(p[element].trans_sensor2world), 1);

        size += __int64_t_encoded_array_size(&(p[element].point_num), 1);

        size += __srcl_lcm_msgs_Point3Df_t_encoded_array_size(p[element].points, p[element].point_num);

        size += __srcl_lcm_msgs_QuadrotorTransform_encoded_array_size(&(p[element].pose), 1);

    }
    return size;
}

int srcl_lcm_msgs_LaserScanPoints_t_encoded_size(const srcl_lcm_msgs_LaserScanPoints_t *p)
{
    return 8 + __srcl_lcm_msgs_LaserScanPoints_t_encoded_array_size(p, 1);
}

int __srcl_lcm_msgs_LaserScanPoints_t_decode_array(const void *buf, int offset, int maxlen, srcl_lcm_msgs_LaserScanPoints_t *p, int elements)
{
    int pos = 0, thislen, element;

    for (element = 0; element < elements; element++) {

        thislen = __srcl_lcm_msgs_Pose_t_decode_array(buf, offset + pos, maxlen - pos, &(p[element].trans_sensor2world), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __int64_t_decode_array(buf, offset + pos, maxlen - pos, &(p[element].point_num), 1);
        if (thislen < 0) return thislen; else pos += thislen;

        p[element].points = (srcl_lcm_msgs_Point3Df_t*) lcm_malloc(sizeof(srcl_lcm_msgs_Point3Df_t) * p[element].point_num);
        thislen = __srcl_lcm_msgs_Point3Df_t_decode_array(buf, offset + pos, maxlen - pos, p[element].points, p[element].point_num);
        if (thislen < 0) return thislen; else pos += thislen;

        thislen = __srcl_lcm_msgs_QuadrotorTransform_decode_array(buf, offset + pos, maxlen - pos, &(p[element].pose), 1);
        if (thislen < 0) return thislen; else pos += thislen;

    }
    return pos;
}

int __srcl_lcm_msgs_LaserScanPoints_t_decode_array_cleanup(srcl_lcm_msgs_LaserScanPoints_t *p, int elements)
{
    int element;
    for (element = 0; element < elements; element++) {

        __srcl_lcm_msgs_Pose_t_decode_array_cleanup(&(p[element].trans_sensor2world), 1);

        __int64_t_decode_array_cleanup(&(p[element].point_num), 1);

        __srcl_lcm_msgs_Point3Df_t_decode_array_cleanup(p[element].points, p[element].point_num);
        if (p[element].points) free(p[element].points);

        __srcl_lcm_msgs_QuadrotorTransform_decode_array_cleanup(&(p[element].pose), 1);

    }
    return 0;
}

int srcl_lcm_msgs_LaserScanPoints_t_decode(const void *buf, int offset, int maxlen, srcl_lcm_msgs_LaserScanPoints_t *p)
{
    int pos = 0, thislen;
    int64_t hash = __srcl_lcm_msgs_LaserScanPoints_t_get_hash();

    int64_t this_hash;
    thislen = __int64_t_decode_array(buf, offset + pos, maxlen - pos, &this_hash, 1);
    if (thislen < 0) return thislen; else pos += thislen;
    if (this_hash != hash) return -1;

    thislen = __srcl_lcm_msgs_LaserScanPoints_t_decode_array(buf, offset + pos, maxlen - pos, p, 1);
    if (thislen < 0) return thislen; else pos += thislen;

    return pos;
}

int srcl_lcm_msgs_LaserScanPoints_t_decode_cleanup(srcl_lcm_msgs_LaserScanPoints_t *p)
{
    return __srcl_lcm_msgs_LaserScanPoints_t_decode_array_cleanup(p, 1);
}

int __srcl_lcm_msgs_LaserScanPoints_t_clone_array(const srcl_lcm_msgs_LaserScanPoints_t *p, srcl_lcm_msgs_LaserScanPoints_t *q, int elements)
{
    int element;
    for (element = 0; element < elements; element++) {

        __srcl_lcm_msgs_Pose_t_clone_array(&(p[element].trans_sensor2world), &(q[element].trans_sensor2world), 1);

        __int64_t_clone_array(&(p[element].point_num), &(q[element].point_num), 1);

        q[element].points = (srcl_lcm_msgs_Point3Df_t*) lcm_malloc(sizeof(srcl_lcm_msgs_Point3Df_t) * q[element].point_num);
        __srcl_lcm_msgs_Point3Df_t_clone_array(p[element].points, q[element].points, p[element].point_num);

        __srcl_lcm_msgs_QuadrotorTransform_clone_array(&(p[element].pose), &(q[element].pose), 1);

    }
    return 0;
}

srcl_lcm_msgs_LaserScanPoints_t *srcl_lcm_msgs_LaserScanPoints_t_copy(const srcl_lcm_msgs_LaserScanPoints_t *p)
{
    srcl_lcm_msgs_LaserScanPoints_t *q = (srcl_lcm_msgs_LaserScanPoints_t*) malloc(sizeof(srcl_lcm_msgs_LaserScanPoints_t));
    __srcl_lcm_msgs_LaserScanPoints_t_clone_array(p, q, 1);
    return q;
}

void srcl_lcm_msgs_LaserScanPoints_t_destroy(srcl_lcm_msgs_LaserScanPoints_t *p)
{
    __srcl_lcm_msgs_LaserScanPoints_t_decode_array_cleanup(p, 1);
    free(p);
}

int srcl_lcm_msgs_LaserScanPoints_t_publish(lcm_t *lc, const char *channel, const srcl_lcm_msgs_LaserScanPoints_t *p)
{
      int max_data_size = srcl_lcm_msgs_LaserScanPoints_t_encoded_size (p);
      uint8_t *buf = (uint8_t*) malloc (max_data_size);
      if (!buf) return -1;
      int data_size = srcl_lcm_msgs_LaserScanPoints_t_encode (buf, 0, max_data_size, p);
      if (data_size < 0) {
          free (buf);
          return data_size;
      }
      int status = lcm_publish (lc, channel, buf, data_size);
      free (buf);
      return status;
}

struct _srcl_lcm_msgs_LaserScanPoints_t_subscription_t {
    srcl_lcm_msgs_LaserScanPoints_t_handler_t user_handler;
    void *userdata;
    lcm_subscription_t *lc_h;
};
static
void srcl_lcm_msgs_LaserScanPoints_t_handler_stub (const lcm_recv_buf_t *rbuf,
                            const char *channel, void *userdata)
{
    int status;
    srcl_lcm_msgs_LaserScanPoints_t p;
    memset(&p, 0, sizeof(srcl_lcm_msgs_LaserScanPoints_t));
    status = srcl_lcm_msgs_LaserScanPoints_t_decode (rbuf->data, 0, rbuf->data_size, &p);
    if (status < 0) {
        fprintf (stderr, "error %d decoding srcl_lcm_msgs_LaserScanPoints_t!!!\n", status);
        return;
    }

    srcl_lcm_msgs_LaserScanPoints_t_subscription_t *h = (srcl_lcm_msgs_LaserScanPoints_t_subscription_t*) userdata;
    h->user_handler (rbuf, channel, &p, h->userdata);

    srcl_lcm_msgs_LaserScanPoints_t_decode_cleanup (&p);
}

srcl_lcm_msgs_LaserScanPoints_t_subscription_t* srcl_lcm_msgs_LaserScanPoints_t_subscribe (lcm_t *lcm,
                    const char *channel,
                    srcl_lcm_msgs_LaserScanPoints_t_handler_t f, void *userdata)
{
    srcl_lcm_msgs_LaserScanPoints_t_subscription_t *n = (srcl_lcm_msgs_LaserScanPoints_t_subscription_t*)
                       malloc(sizeof(srcl_lcm_msgs_LaserScanPoints_t_subscription_t));
    n->user_handler = f;
    n->userdata = userdata;
    n->lc_h = lcm_subscribe (lcm, channel,
                                 srcl_lcm_msgs_LaserScanPoints_t_handler_stub, n);
    if (n->lc_h == NULL) {
        fprintf (stderr,"couldn't reg srcl_lcm_msgs_LaserScanPoints_t LCM handler!\n");
        free (n);
        return NULL;
    }
    return n;
}

int srcl_lcm_msgs_LaserScanPoints_t_subscription_set_queue_capacity (srcl_lcm_msgs_LaserScanPoints_t_subscription_t* subs,
                              int num_messages)
{
    return lcm_subscription_set_queue_capacity (subs->lc_h, num_messages);
}

int srcl_lcm_msgs_LaserScanPoints_t_unsubscribe(lcm_t *lcm, srcl_lcm_msgs_LaserScanPoints_t_subscription_t* hid)
{
    int status = lcm_unsubscribe (lcm, hid->lc_h);
    if (0 != status) {
        fprintf(stderr,
           "couldn't unsubscribe srcl_lcm_msgs_LaserScanPoints_t_handler %p!\n", hid);
        return -1;
    }
    free (hid);
    return 0;
}

