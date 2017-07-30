"""LCM type definitions
This file automatically generated by lcm.
DO NOT MODIFY BY HAND!!!!
"""

try:
    import cStringIO.StringIO as BytesIO
except ImportError:
    from io import BytesIO
import struct

import srcl_lcm_msgs.WayPoint_t

class Path_t(object):
    __slots__ = ["waypoint_num", "waypoints", "const_velocity"]

    def __init__(self):
        self.waypoint_num = 0
        self.waypoints = []
        self.const_velocity = 0.0

    def encode(self):
        buf = BytesIO()
        buf.write(Path_t._get_packed_fingerprint())
        self._encode_one(buf)
        return buf.getvalue()

    def _encode_one(self, buf):
        buf.write(struct.pack(">q", self.waypoint_num))
        for i0 in range(self.waypoint_num):
            assert self.waypoints[i0]._get_packed_fingerprint() == srcl_lcm_msgs.WayPoint_t._get_packed_fingerprint()
            self.waypoints[i0]._encode_one(buf)
        buf.write(struct.pack(">f", self.const_velocity))

    def decode(data):
        if hasattr(data, 'read'):
            buf = data
        else:
            buf = BytesIO(data)
        if buf.read(8) != Path_t._get_packed_fingerprint():
            raise ValueError("Decode error")
        return Path_t._decode_one(buf)
    decode = staticmethod(decode)

    def _decode_one(buf):
        self = Path_t()
        self.waypoint_num = struct.unpack(">q", buf.read(8))[0]
        self.waypoints = []
        for i0 in range(self.waypoint_num):
            self.waypoints.append(srcl_lcm_msgs.WayPoint_t._decode_one(buf))
        self.const_velocity = struct.unpack(">f", buf.read(4))[0]
        return self
    _decode_one = staticmethod(_decode_one)

    _hash = None
    def _get_hash_recursive(parents):
        if Path_t in parents: return 0
        newparents = parents + [Path_t]
        tmphash = (0xee74c1373a1dee5b+ srcl_lcm_msgs.WayPoint_t._get_hash_recursive(newparents)) & 0xffffffffffffffff
        tmphash  = (((tmphash<<1)&0xffffffffffffffff)  + (tmphash>>63)) & 0xffffffffffffffff
        return tmphash
    _get_hash_recursive = staticmethod(_get_hash_recursive)
    _packed_fingerprint = None

    def _get_packed_fingerprint():
        if Path_t._packed_fingerprint is None:
            Path_t._packed_fingerprint = struct.pack(">Q", Path_t._get_hash_recursive([]))
        return Path_t._packed_fingerprint
    _get_packed_fingerprint = staticmethod(_get_packed_fingerprint)

