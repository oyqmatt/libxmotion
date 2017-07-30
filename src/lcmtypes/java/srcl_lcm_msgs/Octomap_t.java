/* LCM type definition class file
 * This file was automatically generated by lcm-gen
 * DO NOT MODIFY BY HAND!!!!
 */

package srcl_lcm_msgs;
 
import java.io.*;
import java.util.*;
import lcm.lcm.*;
 
public final class Octomap_t implements lcm.lcm.LCMEncodable
{
    public boolean binary;
    public String id;
    public double resolution;
    public int data_size;
    public byte data[];
 
    public Octomap_t()
    {
    }
 
    public static final long LCM_FINGERPRINT;
    public static final long LCM_FINGERPRINT_BASE = 0xe71a0720e69af8afL;
 
    static {
        LCM_FINGERPRINT = _hashRecursive(new ArrayList<Class<?>>());
    }
 
    public static long _hashRecursive(ArrayList<Class<?>> classes)
    {
        if (classes.contains(srcl_lcm_msgs.Octomap_t.class))
            return 0L;
 
        classes.add(srcl_lcm_msgs.Octomap_t.class);
        long hash = LCM_FINGERPRINT_BASE
            ;
        classes.remove(classes.size() - 1);
        return (hash<<1) + ((hash>>63)&1);
    }
 
    public void encode(DataOutput outs) throws IOException
    {
        outs.writeLong(LCM_FINGERPRINT);
        _encodeRecursive(outs);
    }
 
    public void _encodeRecursive(DataOutput outs) throws IOException
    {
        char[] __strbuf = null;
        outs.writeByte( this.binary ? 1 : 0); 
 
        __strbuf = new char[this.id.length()]; this.id.getChars(0, this.id.length(), __strbuf, 0); outs.writeInt(__strbuf.length+1); for (int _i = 0; _i < __strbuf.length; _i++) outs.write(__strbuf[_i]); outs.writeByte(0); 
 
        outs.writeDouble(this.resolution); 
 
        outs.writeInt(this.data_size); 
 
        if (this.data_size > 0)
            outs.write(this.data, 0, data_size);
 
    }
 
    public Octomap_t(byte[] data) throws IOException
    {
        this(new LCMDataInputStream(data));
    }
 
    public Octomap_t(DataInput ins) throws IOException
    {
        if (ins.readLong() != LCM_FINGERPRINT)
            throw new IOException("LCM Decode error: bad fingerprint");
 
        _decodeRecursive(ins);
    }
 
    public static srcl_lcm_msgs.Octomap_t _decodeRecursiveFactory(DataInput ins) throws IOException
    {
        srcl_lcm_msgs.Octomap_t o = new srcl_lcm_msgs.Octomap_t();
        o._decodeRecursive(ins);
        return o;
    }
 
    public void _decodeRecursive(DataInput ins) throws IOException
    {
        char[] __strbuf = null;
        this.binary = ins.readByte()!=0;
 
        __strbuf = new char[ins.readInt()-1]; for (int _i = 0; _i < __strbuf.length; _i++) __strbuf[_i] = (char) (ins.readByte()&0xff); ins.readByte(); this.id = new String(__strbuf);
 
        this.resolution = ins.readDouble();
 
        this.data_size = ins.readInt();
 
        this.data = new byte[(int) data_size];
        ins.readFully(this.data, 0, data_size); 
    }
 
    public srcl_lcm_msgs.Octomap_t copy()
    {
        srcl_lcm_msgs.Octomap_t outobj = new srcl_lcm_msgs.Octomap_t();
        outobj.binary = this.binary;
 
        outobj.id = this.id;
 
        outobj.resolution = this.resolution;
 
        outobj.data_size = this.data_size;
 
        outobj.data = new byte[(int) data_size];
        if (this.data_size > 0)
            System.arraycopy(this.data, 0, outobj.data, 0, this.data_size); 
        return outobj;
    }
 
}

