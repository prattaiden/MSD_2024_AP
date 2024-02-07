import java.io.*;
import java.lang.reflect.Field;
import java.util.Objects;

public class DNSHeader {

    /*      +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
             15                       7                    0
            |                      ID                       |
            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
             7            3  2  1  0 7            3        0
            |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
            15                                             0
            |                    QDCOUNT                    |
            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
            15                                             0
            |                    ANCOUNT                    |
            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
            15                                             0
            |                    NSCOUNT                    |
            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
            15                                             0
            |                    ARCOUNT                    |
            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+

     */
    private int id_; //dns query, matches to a response
    protected Short flags_;
    protected int qr_; //tells if msg is a query(0) or a response(1)
    protected int opCode_; //tells the kind of query in msg; standard(0), inverse(1), server status(2)...
    protected int aa_; //is responding DNS server authoritative
    protected int tc_; //is msg truncated (too big to send in 1 packet)
    protected int rd_; //recursive?
    protected int ra_; //does responding DNS server support queries?
    protected int z_; // always 0
    protected int rCode_; //tells result of query; NoError(0), FormatError(1), ServerFailure(2)...
    private int qdCount_; //# of entries in the QUESTION section
    private int anCount_; //# of entries in the ANSWER section
    private int nsCount_; //# of entries in the AUTHORITY section
    private int arCount_; //# of entries in the ADDITIONAL section

    // Private constructor to prevent instantiation outside the class
    protected DNSHeader() {
        //set default header data to 0
        this.id_ = 0;
        this.flags_ = 0;
        this.qr_ = 0;
        this.opCode_ = 0;
        this.aa_ = 0;
        this.tc_ = 0;
        this.rd_ = 0;
        this.ra_ = 0;
        this.z_ = 0;
        this.rCode_ = 0;
        this.qdCount_ = 0;
        this.anCount_ = 0;
        this.nsCount_ = 0;
        this.arCount_ = 0;
    }

    public static DNSHeader decodeHeader(ByteArrayInputStream byteArrayInputStream) throws IOException {
        //Initialize header object
        DNSHeader header = new DNSHeader();
        //Read in header and store it's data to header obj
        try (DataInputStream dataInputStream = new DataInputStream(byteArrayInputStream)) {
            // Read 2 bytes for ID using readShort()
            header.id_ = dataInputStream.readShort();

            // Read 2 bytes for flags, then parse flags
            header.flags_ = dataInputStream.readShort();
            parseFlags(header.flags_, header);

            // Read 2 bytes for qdcount
            header.qdCount_ = dataInputStream.readShort();

            // Read 2 bytes for ancount
            header.anCount_ = dataInputStream.readShort();

            // Read 2 bytes for nscount
            header.nsCount_ = dataInputStream.readShort();

            // Read 2 bytes for arcount
            header.arCount_ = dataInputStream.readShort();
        }

        return header;
    }

    //Parse the short that contains all the head flags
    public static void parseFlags(Short flags, DNSHeader header) {
        //BYTE 4
        // qr flag -> bit 7
        header.qr_ = (flags >> 15) & 0x1; //moves the bit at position 15 to the rightmost position (bit 0) then bitwise AND with 0x1 (binary 00000001) ~extracting the rightmost bit

        // opcode flag -> bit 6-3
        header.opCode_ = (flags >> 11) & 0xF; //moves bits at positions 11-15 to the rightmost positions (bits 0-4) then bitwise AND with 0xF (binary 00001111)

        // aa flag -> bit 2
        header.aa_ = (flags >> 10) & 0x1;

        // tc flag -> bit 1
        header.tc_ = (flags >> 9) & 0x1;

        // rd flag -> bit 0
        header.rd_ = (flags >> 8) & 0x1;

        //BYTE 3
        // ra flag -> bit 7
        header.ra_ = (flags >> 7) & 0x1;

        // rcode flag -> bit 3-0
        header.rCode_ = flags & 0xF;
    }

    //encode the header to bytes to be sent back to the client.
    // The OutputStream interface has methods to write a single byte or an array of bytes.
    public void writeBytes(ByteArrayOutputStream byteArrayOutputStream) throws IOException {
       DataOutputStream dataOutputStream = new DataOutputStream(byteArrayOutputStream);
        // Write 2 bytes for ID
        dataOutputStream.writeShort(id_);
        flags_ = convertFlagsToShort();
        dataOutputStream.writeShort(flags_);
        dataOutputStream.writeShort(qdCount_);
        dataOutputStream.writeShort(anCount_);
        dataOutputStream.writeShort(nsCount_);
        dataOutputStream.writeShort(arCount_);
    }

    //change qr to 1
   // create the header for the response. It will copy some fields from the request
    public static DNSHeader buildHeaderForResponse(DNSMessage request, DNSMessage response) {
        DNSHeader requestHeader = request.header_;

        // Create a new header using the request's header as a template
        DNSHeader responseHeader = new DNSHeader();
        responseHeader.id_ = (requestHeader.getId());
        responseHeader.flags_ = (requestHeader.getFlags());
        responseHeader.qdCount_ = (requestHeader.getQdCount());
        responseHeader.nsCount_ = (requestHeader.getNsCount());
        responseHeader.arCount_ = (requestHeader.getArCount());

        // Set QR flag to indicate a response
        responseHeader.qr_ = 1;

        if(response.getAnswerCount() > 0){
            responseHeader.anCount_ = 1;
        }

        return responseHeader;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        DNSHeader dnsHeader = (DNSHeader) o;
        return id_ == dnsHeader.id_ && qr_ == dnsHeader.qr_ && opCode_ == dnsHeader.opCode_ && aa_ == dnsHeader.aa_ && tc_ == dnsHeader.tc_ && rd_ == dnsHeader.rd_ && ra_ == dnsHeader.ra_ && z_ == dnsHeader.z_ && rCode_ == dnsHeader.rCode_ && qdCount_ == dnsHeader.qdCount_ && anCount_ == dnsHeader.anCount_ && nsCount_ == dnsHeader.nsCount_ && arCount_ == dnsHeader.arCount_ && Objects.equals(flags_, dnsHeader.flags_);
    }

    @Override
    public int hashCode() {
        return Objects.hash(id_, flags_, qr_, opCode_, aa_, tc_, rd_, ra_, z_, rCode_, qdCount_, anCount_, nsCount_, arCount_);
    }

    //Return a human-readable string version of a header object.
    @Override
    public String toString() {
        return "DNSHeader{" +
                "id=" + id_ +
                ", flags=" + flags_ +
                ", qdcount=" + qdCount_ +
                ", ancount=" + anCount_ +
                ", nscount=" + nsCount_ +
                ", arcount=" + arCount_ +
                '}';
    }

    //Helper method to convert flag int into two bytes (short)
    public short convertFlagsToShort() {
        short result = 0;
        // BYTE 4
        // qr flag -> bit 7
        result |= (short) ((qr_ & 0x1) << 15);
        // opcode flag -> bit 6-3
        result |= (short) ((opCode_ & 0xF) << 11);
        // aa flag -> bit 2
        result |= (short) ((aa_ & 0x1) << 10);
        // tc flag -> bit 1
        result |= (short) ((tc_ & 0x1) << 9);
        // rd flag -> bit 0
        result |= (short) ((rd_ & 0x1) << 8);
        // BYTE 3
        // ra flag -> bit 7
        result |= (short) ((ra_ & 0x1) << 7);
        // Z bits -> bits 6-4
        result |= (short) ((z_ & 0x7) << 4);
        // rcode flag -> bit 3-0
        result |= (short) (rCode_ & 0xF);
        return result;
    }

    //---------------------------------GETTERS--------------------------------
    public int getId() {return id_;}
    public Short getFlags() {return flags_;}
    public int getQr() {return qr_;}
    public int getOpCode() {return opCode_;}
    public int getAa() {return aa_;}
    public int getTc() {return tc_;}
    public int getRd() {return rd_;}
    public int getRa() {return ra_;}
    public int getZ() {return z_;}
    public int getRCode() {return rCode_;}
    public int getQdCount() {return qdCount_;}
    public int getAnCount() {return anCount_;}
    public int getNsCount() {return nsCount_;}
    public int getArCount() {return arCount_;}
}

