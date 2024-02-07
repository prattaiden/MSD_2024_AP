import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Objects;

public class DNSQuestion {

        /*
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
        |                                               |
        |                      QNAME                    |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
        |                      QTYPE                    |
        +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
        |                      QCLASS                   |
         +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
         */
    public static final int MAX_LABEL_LENGTH = 63;
    private String[] qname_;
    private int qtype_;
    private int qclass_;

    //constructor
    DNSQuestion() {
        qname_ = new String[0];
        qtype_ = 0;
        qclass_ = 0;
    }
    //getters and setters
    public void setQname(String[] qname) {this.qname_ = qname;}
    public String[] getQname_() {return qname_;}
    public void setQtype(int qtype) {this.qtype_ = qtype;}
    public int getQtype_() {return qtype_;}
    public void setQclass(int qclass) {this.qclass_ = qclass;}
    public int getQclass_(){return qclass_;}


    static DNSQuestion decodeQuestion(InputStream inputStream, DNSMessage dnsMessage) throws IOException {
        DNSQuestion dnsQuestion = new DNSQuestion();

        DataInputStream dataInputStream = new DataInputStream(inputStream);
        dnsQuestion.setQname(dnsMessage.readDomainName(inputStream));
        dnsQuestion.setQtype(dataInputStream.readShort());
        dnsQuestion.setQclass(dataInputStream.readShort());
        return dnsQuestion;
    }

    //Write the question bytes which will be sent to the client. The hash map is used for us to compress the message,
    void writeBytes(ByteArrayOutputStream byteArrayOutputStream, HashMap<String,Integer> domainNameLocations) throws IOException {
        DataOutputStream dataOutputStream = new DataOutputStream(byteArrayOutputStream);
        DNSMessage.writeDomainName(dataOutputStream, domainNameLocations, qname_);
        dataOutputStream.writeShort(qclass_);
        dataOutputStream.writeShort(qtype_);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        DNSQuestion that = (DNSQuestion) o;
        return qtype_ == that.qtype_ && qclass_ == that.qclass_ && Arrays.equals(qname_, that.qname_);
    }

    @Override
    public int hashCode() {
        int result = Objects.hash(qtype_, qclass_);
        result = 31 * result + Arrays.hashCode(qname_);
        return result;
    }

    @Override
    public String toString() {
        return "DNSQuestion{" +
                "qname_=" + Arrays.toString(qname_) +
                ", qtype_=" + qtype_ +
                ", qclass_=" + qclass_ +
                '}';
    }
}
