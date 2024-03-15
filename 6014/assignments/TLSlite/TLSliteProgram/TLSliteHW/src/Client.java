import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.math.BigInteger;
import java.security.SecureRandom;


public class Client {
    static BigInteger clientDHPriv_;

    public static void main(String[] args) throws IOException {

        //server and client agree on DH parameters g and N
       clientDHPriv_ = new BigInteger(new SecureRandom().generateSeed(32));

       


        BigInteger nonce = new BigInteger(new SecureRandom().generateSeed(32));

        Socket clientSocket = new Socket("localhost", 8080);
        ObjectOutputStream clientOutputStream = new ObjectOutputStream(clientSocket.getOutputStream());


    }
}
