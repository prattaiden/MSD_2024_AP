import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.math.BigInteger;
import java.net.ServerSocket;
import java.net.Socket;
import java.security.*;
import java.security.cert.Certificate;
import java.security.cert.CertificateEncodingException;
import java.security.cert.CertificateException;
import java.security.spec.InvalidKeySpecException;
import java.util.Arrays;

public class Server {
    static BigInteger serverDHpriv_;
    private static byte[] prk_;
    private static byte[] serverEncrypt_;
    private static byte[] serverMac_;
    private static byte[] serverIV_;

    public static void main(String[] args) throws IOException, ClassNotFoundException, NoSuchAlgorithmException, InvalidKeySpecException, SignatureException, InvalidKeyException, CertificateEncodingException {
        //--setting up server--//
        final int server_port = 5678;
        ServerSocket serverListener = new ServerSocket(server_port);
        ByteArrayOutputStream history = new ByteArrayOutputStream();
        System.out.println("listening at port: " + server_port);
        Socket socket = serverListener.accept();
        ObjectInputStream input = new ObjectInputStream(socket.getInputStream());
        System.out.println("heard from " + socket.getInetAddress() + " port: " + socket.getPort());


        //----------------------------------------handshake--------------------------------------//
        //(part1 - recieve nonce)
        //recieving nonce from the client
        BigInteger recievedNonce = (BigInteger) input.readObject();
        history.write(recievedNonce.toByteArray());
        System.out.println("server RECEIVED nonce from client");

        //(part2 - send the server certificate, serverDHPubKey, and signed DH public key)//
        //creating a server DH private key AND the DH public key

        serverDHpriv_ = new BigInteger(new SecureRandom().generateSeed(32));
        BigInteger serverDHPubKey = Shared.getDHPublicKey(serverDHpriv_);
        Certificate serverCertificate = Shared.getCertificate("../../CASignedServerCertificate.pem");
        //todo signed DH public key as Enc(serverRSA, serverDHPUB)
        BigInteger signedServerDHPublicKey = Shared.getSignedDHPubKey("../../serverPrivateKey.der", serverDHPubKey);

        //sending
        ObjectOutputStream outputStream = new ObjectOutputStream(socket.getOutputStream());
        outputStream.writeObject(serverCertificate);
        outputStream.writeObject(serverDHPubKey);
        outputStream.writeObject(signedServerDHPublicKey);
        //writing these to history
        assert serverCertificate != null;
        history.write(serverCertificate.getEncoded());
        history.write(serverDHPubKey.toByteArray());
        history.write(signedServerDHPublicKey.toByteArray());
        System.out.println("server has SENT : server certificate, server DH public key, signed server DH public key");

        //(part3 - recieve client certificate, clientDHPubKey, and signed client DH public key)//
        Certificate clientCertificate = (Certificate) input.readObject(); //read client certificate
        BigInteger clientDHPubKey = (BigInteger) input.readObject(); //read client DH pub key
        BigInteger signedClientDHPubKey = (BigInteger) input.readObject(); //read singed dh pub key

        //read history from those
        history.write(clientCertificate.getEncoded());
        history.write(clientDHPubKey.toByteArray());
        history.write(signedClientDHPubKey.toByteArray());
        System.out.println("server has RECEIVED : cleint certificate, client DH public key, signed client DH public key");

        //(part4- compute shared DH secret!)//
        BigInteger sharedSecret = Shared.computeSharedSecret(serverDHpriv_, clientDHPubKey);
        System.out.println("private secret! " + Arrays.toString(sharedSecret.toByteArray()));

        //creating the server's secret keys
        makeServerSecretKeys(recievedNonce.toByteArray(), sharedSecret.toByteArray());

        byte[] handshakeMsgHstry = history.toByteArray();
        byte[] hmac = Shared.computeHMAC(serverMac_, handshakeMsgHstry);

        outputStream.writeObject(hmac);
        //final history add, maybe not necessary
        history.write(hmac);
        System.out.println(Arrays.toString(hmac));
        System.out.println("server has SENT: HMAC of message history");


        //reading the clients last message?
        byte[] clientHMAC = (byte[]) input.readObject();
        System.out.println("sever has RECEIVED HMAC of message history from client");
        System.out.println(Arrays.toString(clientHMAC));

        socket.close();
        serverListener.close();
    }

    private static void makeServerSecretKeys(byte[] ogNonce, byte[] DHSharedSecret) throws NoSuchAlgorithmException, InvalidKeyException {
        Mac mac = Mac.getInstance("HmacSHA256");
        SecretKeySpec spec = new SecretKeySpec(DHSharedSecret, "HmacSHA256");
        mac.init(spec);
        prk_ = mac.doFinal(DHSharedSecret);
        serverEncrypt_ = Shared.hdkfExpand(prk_, "server encrypt");
        serverMac_ = Shared.hdkfExpand(serverEncrypt_, "server MAC");
        serverIV_ = Shared.hdkfExpand(serverMac_, "server IV");
    }

}
