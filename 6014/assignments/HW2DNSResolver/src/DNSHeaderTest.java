import org.junit.jupiter.api.Test;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.sql.Array;
import java.util.ArrayList;
import java.util.Arrays;

import static org.junit.jupiter.api.Assertions.*;

class DNSHeaderTest {

    byte[] headerBytes;
    DNSHeader header;

    @org.junit.jupiter.api.BeforeEach
    void setUp() {
        headerBytes = new byte[]{0x00, 0x01, 0x55, 0x05, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01};

    }

    //Test decodeHeader and parseFlags
        //TODO outlier/edge case testing
    @Test
    void testDecodeHeader() throws IOException {
        // Create ByteArrayInputStream from test data
        ByteArrayInputStream inputStream = new ByteArrayInputStream(headerBytes);

        // Call the method to be tested
         header = DNSHeader.decodeHeader(inputStream);


        // Assert the expected values
        assertEquals(1, header.getId()); //reads in 0x0001
        assertEquals((short) 21765, header.getFlags()); //reads in 0x5555
        assertEquals(0, header.getQdCount()); //reads in 0x0000
        assertEquals(1, header.getAnCount());
        assertEquals(0, header.getNsCount());
        assertEquals(1, header.getArCount());


        //PARSING FLAGS
        assertEquals(0, header.getQr());
        assertEquals(10, header.getOpCode()); //bit 6-3 is 1010
        assertEquals(1, header.getAa());
        assertEquals(0, header.getTc());
        assertEquals(1, header.getRd());
        assertEquals(0, header.getRa());
        assertEquals(5, header.getRCode()); //bit 3-0 is 0101
        assertEquals(0, header.getZ()); //bit 3-0 is 0101

    }
    //TODO test function -buildHeaderForResponse


    @Test
    void testWriteBytes() throws IOException {
        header = DNSHeader.decodeHeader(new ByteArrayInputStream(headerBytes));

        ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        header.writeBytes(outputStream);

        byte[] writtenBytes = outputStream.toByteArray();
        System.out.println("HeaderBytes: " + Arrays.toString(headerBytes));
        System.out.println("WrittenBytes: " + Arrays.toString(writtenBytes));

        // Assert that the written bytes match the original headerBytes
        assertTrue(Arrays.equals(headerBytes, writtenBytes));
    }

//    @Test
//    void testToString() throws IOException {
//        header = DNSHeader.decodeHeader(new ByteArrayInputStream(headerBytes));
//        header.toString();
//    }


}

