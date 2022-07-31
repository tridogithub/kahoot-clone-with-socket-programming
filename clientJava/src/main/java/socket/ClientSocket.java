package socket;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;

public class ClientSocket {
    private Socket connection;
    private int serverPort;
    private String address;
    private String username;


    public ClientSocket(int serverPort, String address) throws IOException {
        this.serverPort = serverPort;
        this.address = address;
        this.connection = initialConnection(address, serverPort);
    }

    private Socket initialConnection(String address, int serverPort) throws IOException {
        final Socket connection = new Socket(InetAddress.getByName(address), serverPort);
        return connection;
    }

    public synchronized String sendMessageToServer(String message) throws IOException {
        System.out.println("Send message: " + message);
        PrintWriter out = new PrintWriter(new BufferedWriter(new OutputStreamWriter(this.connection.getOutputStream())), true);
        out.println(message);
//        sendRequest.close();
        BufferedReader receivedResponse = new BufferedReader(new InputStreamReader(this.connection.getInputStream()));
//        char[] buff = new char[4096];
//        int buffSize = receivedResponse.read(buff);
//        buff[13] = '\0';
//        System.out.println(buff);
//        System.out.println(String.valueOf(buff));
//        String receivedString = buff.toString();
        String receivedString = receivedResponse.readLine();
        System.out.println("Received String: " + receivedString);
        return receivedString;
    }

    public void closeConnection() throws IOException {
        connection.close();
    }

    // Getter and Setter
    public Socket getConnection() {
        return connection;
    }

    public void setConnection(Socket connection) {
        this.connection = connection;
    }

    public int getServerPort() {
        return serverPort;
    }

    public void setServerPort(int serverPort) {
        this.serverPort = serverPort;
    }

    public String getAddress() {
        return address;
    }

    public void setAddress(String address) {
        this.address = address;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }
}
