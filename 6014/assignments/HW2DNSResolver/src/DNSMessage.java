import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;

public class DNSMessage {

    //+---------------------
    //|        Header       |
    //+---------------------+
    //|       Question      | the question for the name server
    //+---------------------+
    //|        Answer       | RRs answering the question
    //+---------------------+
    //|      Authority      | RRs pointing toward an authority
    //+---------------------+
    //|      Additional     | RRs holding additional information
    //+---------------------+

    DNSHeader header_;

    private byte[] fullMessage_;
    ArrayList<DNSQuestion> questionsArray_ = new ArrayList<>();
    private ArrayList<DNSRecord> answersArray_ = new ArrayList<>();
    private ArrayList<DNSRecord> authorityRecordsArray_ = new ArrayList<>();
    private ArrayList<DNSRecord> additionalRecordsArray_ = new ArrayList<>();

    ArrayList<DNSQuestion> getQuestionsArray(){
        return questionsArray_;
    }
    ArrayList<DNSRecord> getAnswersArray(){
        return answersArray_;
    }
    ArrayList<DNSRecord> getAuthorityRecordsArray(){
        return authorityRecordsArray_;
    }
    ArrayList<DNSRecord> getAdditionalRecordsArray(){
        return additionalRecordsArray_;
    }


    static DNSMessage decodeMessage(byte[] bytes) throws IOException {
        DNSMessage dnsMessage = new DNSMessage();
        ByteArrayInputStream byteArrayInputStream = new ByteArrayInputStream(bytes);

        dnsMessage.header_ = DNSHeader.decodeHeader(byteArrayInputStream);
        dnsMessage.fullMessage_ = Arrays.copyOf(bytes, bytes.length);

        //header
        int qCount = dnsMessage.header_.getQdCount();

        //adding each question to the questions array
        for(int i = 0; i < qCount; i++){
            dnsMessage.questionsArray_.add(DNSQuestion.decodeQuestion(byteArrayInputStream, dnsMessage));
        }
        //for record--
        int anCount = dnsMessage.header_.getAnCount();//answers
        for(int i = 0; i < anCount; i++){
            dnsMessage.answersArray_.add(DNSRecord.decodeRecord(byteArrayInputStream, dnsMessage));
        }
        int nsCount = dnsMessage.header_.getNsCount();//authority
        for(int i = 0; i < nsCount; i++){
            dnsMessage.authorityRecordsArray_.add(DNSRecord.decodeRecord(byteArrayInputStream, dnsMessage));
        }
        int arCount = dnsMessage.header_.getArCount();//addition
        for(int i = 0; i < arCount; i++){
            dnsMessage.additionalRecordsArray_.add(DNSRecord.decodeRecord(byteArrayInputStream, dnsMessage));
        }
        return dnsMessage;
    }

    String[] readDomainName(InputStream inputStream) throws IOException {
        ArrayList<String> arrayList = new ArrayList<>();
        int length = inputStream.read();

        if(length == 0){
            return new String[0];
        }

        while(length > 0) {

            //getting the label
            byte[] inputLabelArray = new byte[length];
            //reading the bytes to byte array length and store n label bytes
            inputStream.read(inputLabelArray, 0 , inputLabelArray.length);

            String label = new String(inputLabelArray, StandardCharsets.UTF_8);

            arrayList.add(label);

            length = inputStream.read();
        }

        return arrayList.toArray(new String[0]);
    }

    public String[] readDomainName(int firstByte) throws IOException {
        ByteArrayInputStream byteArrayInputStream = new ByteArrayInputStream(fullMessage_, firstByte, fullMessage_.length - firstByte);

        return readDomainName(byteArrayInputStream);
    }

    //use later when sending message back to client
    public static String joinDomainName(String[] domainPieces){
        //join domains and put a dot between
        return String.join(".", domainPieces);
    }


    //TODO
    static void writeDomainName(DataOutputStream dataOutputStream, HashMap<String,Integer> domainLocations, String[] domainPieces) throws IOException {
        String fullDomainName = joinDomainName(domainPieces); //from array list to example.com

        //if it is a record domain name, in record class
        if (domainLocations.containsKey(fullDomainName)) {
            int location = domainLocations.get(fullDomainName);
            location |= (0xC000);
            System.out.println("location in write domain: " + location);
            // Set the pointer flag and write the pointer (2 bytes)
            //writing out 11offset for the location
            //tells where to look
            dataOutputStream.writeShort(location);

            //first time seeing it
        } else {
            Integer locationInteger = dataOutputStream.size();
            //adding to hashmap because it is the first time
            domainLocations.put(fullDomainName, locationInteger);
            for (String label : domainPieces) {
                // Write the label length (1 byte)
                dataOutputStream.writeByte(label.length()); //7 //3
                // Write the label bytes
//                byte[] labelBytes = label.getBytes(StandardCharsets.UTF_8);

                dataOutputStream.writeBytes(label);
            }
            dataOutputStream.writeByte(0); //placing zero for termination
        }
    }

    //TODO use in server
   byte[] toBytes() throws IOException {
        HashMap<String, Integer> domainLocation = new HashMap<>();
        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();

        header_.writeBytes(byteArrayOutputStream);

        for(DNSQuestion question : questionsArray_){
            question.writeBytes(byteArrayOutputStream, domainLocation);
        }
        //answersArray_.get(0).writeBytes(byteArrayOutputStream, domainLocation);
       for(DNSRecord answerRecord : answersArray_){
           answerRecord.writeBytes(byteArrayOutputStream, domainLocation);
       }
        for(DNSRecord authorityRecord : authorityRecordsArray_){
            authorityRecord.writeBytes(byteArrayOutputStream, domainLocation);
        }
        for(DNSRecord additionRecord : additionalRecordsArray_){
            additionRecord.writeBytes(byteArrayOutputStream, domainLocation);
        }
        return byteArrayOutputStream.toByteArray();

//          //initialize map for domain location
//          //in this method call the write bytes from header, question, record
//          //this is where building response for client
//          //compose message to send back to the client
//          //write back header, question, send back answers
    }

    //TODO use in server
    //build a response based on the request and the answers you intend to send back.
    static DNSMessage buildResponse(DNSMessage request, ArrayList<DNSRecord> answers){
        // Create a new DNSMessage for the response
        DNSMessage response = new DNSMessage();

        response.answersArray_ = answers; //answers in a google response
        // Build the header for the response
        response.header_ = DNSHeader.buildHeaderForResponse(request, response);
        // Set the number of questions, answers, authorities, and additional records
        response.questionsArray_ = request.getQuestionsArray();
        response.authorityRecordsArray_ = request.getAuthorityRecordsArray();
        response.additionalRecordsArray_ = request.getAdditionalRecordsArray();


        return response;
    }

    int getAnswerCount(){
        return answersArray_.size();
    }

}

