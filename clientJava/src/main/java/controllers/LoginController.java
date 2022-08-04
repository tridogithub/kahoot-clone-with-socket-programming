package controllers;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

import home.Main;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;
import javafx.stage.Stage;
import socket.ClientSocket;
import socket.Constant;
import socket.MessageBuilder;

import java.io.IOException;
import java.net.URL;
import java.util.ResourceBundle;

/**
 * @author oXCToo
 */
public class LoginController implements Initializable {
    public static String username = "";
    @FXML
    private Label lblErrors;

    @FXML
    private TextField txtUsername;

    @FXML
    private TextField txtPassword;

    @FXML
    private Button btnSignin;

    @FXML
    private Button btnSignup;

    private ClientSocket clientSocket;

    @FXML
    public void handleButtonAction(MouseEvent event) throws IOException {

        if (event.getSource() == btnSignin) {
            //login here
            String receivedMessage = logIn();
            if (Constant.AUTHENTICATED.equals(receivedMessage)) {
                try {
                    username = txtUsername.getText();
                    //add you loading or delays - ;-)
                    Node node = (Node) event.getSource();
                    Stage stage = (Stage) node.getScene().getWindow();
                    //stage.setMaximized(true);
                    stage.close();
                    Parent root = FXMLLoader.load(getClass().getClassLoader().getResource("fxml/Welcome.fxml"));
                    Scene scene = new Scene(root);
                    stage.setScene(scene);
                    stage.show();

                } catch (IOException ex) {
                    System.err.println(ex.getMessage());
                }
            } else if (!"Empty credentials".equals(receivedMessage)) {
                setLblError(Color.TOMATO, "Unauthenticated!");
            }
        }
    }

    @FXML
    public void handleSignUpBtn(MouseEvent event) throws IOException {
        if (event.getSource() == btnSignup) {
            String email = txtUsername.getText();
            String password = txtPassword.getText();
            if (email.isEmpty() || password.isEmpty()) {
                setLblError(Color.TOMATO, "Empty credentials");
            } else {
                //TODO: send request to server socket
                String sendMessage = MessageBuilder.createSignUpRequest(email, password);
                String receivedMessage = clientSocket.sendMessageToServer(sendMessage);
                if (Constant.SUCCESS.equals(receivedMessage)) {
                    setLblError(Color.GREEN, "Register Successfully. Login with your new account now!");
                } else {
                    setLblError(Color.TOMATO, "Register Fail!");
                }
            }
        }
    }

    @Override
    public void initialize(URL url, ResourceBundle rb) {
        // TODO
        clientSocket = Main.client;
//        if (con == null) {
//            lblErrors.setTextFill(Color.TOMATO);
//            lblErrors.setText("Server Error : Check");
//        } else {
//            lblErrors.setTextFill(Color.GREEN);
//            lblErrors.setText("Server is up : Good to go");
//        }
    }

    public LoginController() {

    }

    //we gonna use string to check for status
    private String logIn() throws IOException {
        String email = txtUsername.getText();
        String password = txtPassword.getText();
        if (email.isEmpty() || password.isEmpty()) {
            setLblError(Color.TOMATO, "Empty credentials");
            return "Empty credentials";
        } else {
            //TODO: send request to server socket
            String sendMessage = MessageBuilder.createSignInRequest(email, password);
            String receivedMessage = clientSocket.sendMessageToServer(sendMessage);
            return receivedMessage;
        }
    }

    private void setLblError(Color color, String text) {
        lblErrors.setTextFill(color);
        lblErrors.setText(text);
    }
}
