import java.io.*;
import java.util.Arrays;
import java.util.Date;
import java.util.HashMap;

public class DNSRecord {
    //|+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+|
    //|                  NAME                           |  unknown size
    //|+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    //|                  TYPE                           |  2 BYTE
    //|+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+|
    //|                  CLASS                          |  2 BYTE
    //|+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+|
    //|                   TTL                           |
    //|                                                 |  4 BYTE
    //| +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
    //|                 RDLENGTH                        |  2 BYTE
    //|+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+-- |
    //|                  RDATA                          |  unknown size
    //|+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+|


    String[] name_;
    int type_;
    short class_;
    int ttl_; //time interval that the resource may be cached
    short rdLength_;
    byte[] rdata_;
    Date date_;


    //read in , if first two bits are 11  it is compressed
    //pointer can be distinguished
    //if it is 11 it is a pointer to somewhere else you need to look

    static DNSRecord decodeRecord(InputStream inputStream, DNSMessage dnsMessage) throws IOException {
        DNSRecord dnsRecord = new DNSRecord();
        dnsRecord.date_ = new Date();
        DataInputStream dataInputStream = new DataInputStream(inputStream);

//        inputStream.mark(2);
//
//        int firstByte = dataInputStream.readUnsignedByte();
//        if(compressed(firstByte)){
//            int pointer = firstByte & (short) 0x3FFF;
//            dnsRecord.name_ = dnsMessage.readDomainName(pointer);
//        }
//        else{
//            inputStream.reset();

      //  }

        dnsRecord.name_ = dnsMessage.readDomainName(inputStream);
       // dnsRecord.name_ = (dnsMessage.readDomainName(inputStream));
        dnsRecord.type_ = dataInputStream.readShort();
        dnsRecord.class_ = dataInputStream.readShort();
        dnsRecord.ttl_ = dataInputStream.readInt();
        dnsRecord.rdLength_ = dataInputStream.readShort();

        //reading the bytes from the length of rd length
        dnsRecord.rdata_ = new byte[dnsRecord.rdLength_];
        dnsRecord.rdata_ = dataInputStream.readNBytes(dnsRecord.rdLength_);


        return dnsRecord;
    }

    public static Boolean compressed(int length){
        return (0xC000 & length) == 0xC000;
    }

    public void writeBytes(ByteArrayOutputStream byteArrayOutputStream, HashMap<String, Integer> hashMap) throws IOException {
        DataOutputStream dataOutputStream = new DataOutputStream(byteArrayOutputStream);
        DNSMessage.writeDomainName(dataOutputStream, hashMap, name_);
        dataOutputStream.writeShort(type_);
        dataOutputStream.writeShort(class_);
        dataOutputStream.writeInt(ttl_);
        dataOutputStream.writeShort(rdLength_);
        dataOutputStream.write(rdata_);
    }

    //return whether the creation date + the time to live is after the current time.
    //The Date and Calendar classes will be useful for this.
    boolean isExpired() {
        Date nowTime = new Date();

        long currentTime = nowTime.getTime();

        //getting the expiration time and ttl is telling how long until it is expired
        long expirationTime = date_.getTime() + (ttl_ * 1000L);

        return currentTime > expirationTime;
    }
    @Override
    public String toString() {
        return "DNSRecord{" +
                "name_=" + Arrays.toString(name_) +
                ", type_=" + type_ +
                ", class_=" + class_ +
                ", ttl_=" + ttl_ +
                ", rdLength_=" + rdLength_ +
                ", rdata_=" + Arrays.toString(rdata_) +
                ", date_=" + date_ +
                '}';
    }
}

