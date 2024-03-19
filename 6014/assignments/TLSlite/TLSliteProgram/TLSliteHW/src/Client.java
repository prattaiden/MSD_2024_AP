import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.math.BigInteger;
import java.net.Socket;
import java.security.*;
import java.security.cert.Certificate;
import java.security.cert.CertificateEncodingException;
import java.security.cert.CertificateException;
import java.security.spec.InvalidKeySpecException;
import java.util.ArrayList;
import java.util.Arrays;


public class Client {
    static BigInteger clientDHPriv_;
    private static byte[] prk_;
    private static byte[] clientEncrypt_;
    private static byte[] clientMac_;
    private static byte[] clientIV;




    public static void main(String[] args) throws IOException, ClassNotFoundException, NoSuchAlgorithmException, InvalidKeySpecException, SignatureException, InvalidKeyException, CertificateEncodingException {
        //--setting up the client's ports//
        final int server_port = 5678;
        Socket clientSocket = new Socket("localhost", server_port);
        ByteArrayOutputStream history = new ByteArrayOutputStream();
        ObjectOutputStream clientOutputStream = new ObjectOutputStream(clientSocket.getOutputStream());

        //--------------------------------------handshake------------------------------------------//
        //(part1 - send nonce)
        //generating the nonce to send to server
        BigInteger nonce = new BigInteger(new SecureRandom().generateSeed(32));
        clientOutputStream.writeObject(nonce);
        history.write(nonce.toByteArray());


        //(part2 - recieve first server message)
        //todo 3 times?
        ObjectInputStream inputStream = new ObjectInputStream(clientSocket.getInputStream());
        Certificate serverCert = (Certificate) inputStream.readObject(); //read Certificate from Server
        BigInteger serverDHPublicKey = (BigInteger) inputStream.readObject(); //read DH public from server
        BigInteger signedServerDHKey = (BigInteger) inputStream.readObject(); //read sgiend DH from server

        history.write(serverCert.getEncoded());
        history.write(serverDHPublicKey.toByteArray());
        history.write(signedServerDHKey.toByteArray());
        System.out.println("client has RECEIVED: server certificate, server DH pub key, signed server DH key");


        //(part3 - client sends)
        //server and client agree on DH parameters g and N
        //creating the client DH public key
       clientDHPriv_ = new BigInteger(new SecureRandom().generateSeed(32));
       BigInteger clientDHPubKey = Shared.getDHPublicKey(clientDHPriv_);
       Certificate clientCertificate = Shared.getCertificate("../../CASignedClientCertificate.pem");
       BigInteger signedClientDHPubKey = Shared.getSignedDHPubKey("../../clientPrivateKey.der", clientDHPubKey);

       clientOutputStream.writeObject(clientCertificate);
       clientOutputStream.writeObject(clientDHPubKey);
       clientOutputStream.writeObject(signedClientDHPubKey);

        assert clientCertificate != null;
        history.write(clientCertificate.getEncoded());
       history.write(clientDHPubKey.toByteArray());
       history.write(signedClientDHPubKey.toByteArray());

        System.out.println("client has SENT: client certificate, client DH public key, signed client public DH key");

        //(part4- compute shared DH secret!)//
       BigInteger sharedSecret = Shared.computeSharedSecret(clientDHPriv_, serverDHPublicKey);
//       System.out.println("private secret! " + Arrays.toString(sharedSecret.toByteArray()));

       //this should be reading the server's HMAC of the history
       byte[] serverHMAC = (byte[]) inputStream.readObject();
       //adding it to history before sending back client version
       history.write(serverHMAC);
       //System.out.println(Arrays.toString(serverHMAC));
       System.out.println("client has RECEIVED HMAC of message history from server");

       makeClientSecretKeys(nonce.toByteArray(),sharedSecret.toByteArray());
       byte[] hmac = Shared.computeHMAC(clientMac_, history.toByteArray());

       clientOutputStream.writeObject(hmac);
       System.out.println("client has SENT the HMAC of message history");
       //final history add, maybe not necessary
       history.write(hmac);
//       System.out.println(Arrays.toString(hmac));


       clientSocket.close();
    }

    //makeSecretKeys method
    private static void makeClientSecretKeys(byte[] ogNonce, byte[] DHSharedSecret) throws NoSuchAlgorithmException, InvalidKeyException {
        Mac mac = Mac.getInstance("HmacSHA256");
        SecretKeySpec spec = new SecretKeySpec(DHSharedSecret, "HmacSHA256");
        mac.init(spec);
        prk_ = mac.doFinal(DHSharedSecret);
        clientEncrypt_ = Shared.hdkfExpand(prk_, "client encrypt");
        clientMac_ = Shared.hdkfExpand(clientEncrypt_, "client MAC");
        clientIV = Shared.hdkfExpand(clientMac_, "client IV");
    }
}
