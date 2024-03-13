import javax.xml.crypto.Data;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.ArrayList;

public class DNSServer {

    public static DNSCache serverCache_ = new DNSCache();
    public static ArrayList<DNSRecord> dnsRecordAnswer_;

    public static void main(String[] args) {
        try {
            int server_port = 8053;
            System.out.println("Listening at " + server_port + "\n");

            //MAIN SOCKET? TODO
            DatagramSocket socket = new DatagramSocket(server_port);
            //arraylist of answers records
            while(true) {
                byte[] data = new byte[512];
                DatagramPacket pkt = new DatagramPacket(data, data.length);
                //TODO keep track of how many bytes received in the buffer

                socket.receive(pkt);
                int packtL = pkt.getLength();

                System.out.println("received from: " + pkt.getAddress() + "\n");
                //message tp be decoded from the packet
                DNSMessage dnsMessage = DNSMessage.decodeMessage(pkt.getData());

                //adding the query to an arraylist of questions
                ArrayList<DNSQuestion> questions = dnsMessage.getQuestionsArray();

                System.out.println("checking if question exists in cache \n");

                //checking if the question is already in my cache
                DNSRecord cacheRecord = serverCache_.queryCache(questions.get(0));

                    //if the record is not expired
                    if(cacheRecord != null){
                        ArrayList<DNSRecord> temp = new ArrayList<>();
                        temp.add(cacheRecord);
                        dnsRecordAnswer_ = temp;

                        System.out.println("question is in cache!\n");
                    }

                    else{  //when the record is not in the cache
                        System.out.println("not found in cache!\n");
                        //setting the IP address of google for asking google
                        InetAddress googleDNSIP = InetAddress.getByName("8.8.8.8");

                        DatagramPacket googlePkt = new DatagramPacket(data, packtL, googleDNSIP, 53);
                        socket.send(googlePkt);

                        //receive from google
                        byte[] googleResponseArray = new byte[512];
                        DatagramPacket googleResponsePacket = new DatagramPacket(googleResponseArray, googleResponseArray.length);

                        socket.receive(googleResponsePacket);
                        System.out.println("receiving packet from google:" + googlePkt.getAddress() + "\n");

                        DNSMessage googleResponse = DNSMessage.decodeMessage(googleResponsePacket.getData());
                        dnsRecordAnswer_ = googleResponse.getAnswersArray();
                        //adding to the cache
                        if(!dnsRecordAnswer_.isEmpty()){
                            serverCache_.addToCache(questions.get(0), dnsRecordAnswer_.get(0));
                        }
                        if(dnsRecordAnswer_.isEmpty()){
                            System.out.println("Error: faulty domain-name!");
                        }

                    }

                //
                DNSMessage responseMessage = DNSMessage.buildResponse(dnsMessage, dnsRecordAnswer_);
                System.out.println("the response message is " + responseMessage);
                byte[] responseBytes = responseMessage.toBytes();
                DatagramPacket responsePacket = new DatagramPacket(responseBytes, responseBytes.length, pkt.getAddress(), pkt.getPort());

                socket.send(responsePacket);
            }

        }
        catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}
