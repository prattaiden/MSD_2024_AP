import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.io.ByteArrayInputStream;
import java.io.IOException;

import static org.junit.jupiter.api.Assertions.assertEquals;

public class DNSQuestionTest {
    byte[] questionBytes;
    DNSQuestion question;
    DNSMessage dnsMessage;

    @BeforeEach
    void setUp() {
        //spells out |7|e|x|a|m|p|l|e|3|c|o|m|00|Q-type|Q-class
        questionBytes = new byte[]{0x07, 0x65, 0x78, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x03, 0x63, 0x6f, 0x6d, 0x00, 0x00, 0x01, 0x00, 0x01 };

    }

    //TODO: test pointerToPriorOccurrence()
    @Test
    void testDecodeQuestion() throws IOException {
        // Create ByteArrayInputStream from test data
        ByteArrayInputStream inputStream = new ByteArrayInputStream(questionBytes);

        // Call decodeQuestion
        question = DNSQuestion.decodeQuestion(inputStream, dnsMessage);

        // Assert the expected values
        assertEquals("example.com.", question.getQname_());
        assertEquals(1, question.getQtype_());
        assertEquals(1, question.getQclass_());
    }


}
