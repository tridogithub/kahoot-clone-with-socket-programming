package controllers;

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

public class JoinRoomController implements Initializable {
    @FXML
    private TextField txtRoomCode;

    @FXML
    private Button btnJoin;

    @FXML
    private Button btnBack;

    @FXML
    private Label labelMessage;

    private ClientSocket clientSocket;

    public static String joinedRoomCOde = "";

    @FXML
    void handleBackBtnClick(MouseEvent event) throws IOException {
        Node node = (Node) event.getSource();
        Stage stage = (Stage) node.getScene().getWindow();
        //stage.setMaximized(true);
        stage.close();
        Parent root = FXMLLoader.load(getClass().getClassLoader().getResource("fxml/Welcome.fxml"));
        Scene scene = new Scene(root);
        stage.setScene(scene);
        stage.show();
    }

    @FXML
    void handleJoinBtnClick(MouseEvent event) throws IOException {
        String roomCode = txtRoomCode.getText();
        if (roomCode.isEmpty() || roomCode.length() != 7) {
            labelMessage.setTextFill(Color.TOMATO);
            labelMessage.setText("Please enter a 7-length room code");
        } else {
            String sendMessage = MessageBuilder.createJRRequest(roomCode);
            String receivedMessage = clientSocket.sendMessageToServer(sendMessage);
            if (Constant.SUCCESS.equals(receivedMessage)) {
                joinedRoomCOde = roomCode;
                Node node = (Node) event.getSource();
                Stage stage = (Stage) node.getScene().getWindow();
                //stage.setMaximized(true);
                stage.close();
                Parent root = FXMLLoader.load(getClass().getClassLoader().getResource("fxml/PlayingScreen.fxml"));
                Scene scene = new Scene(root);
                stage.setScene(scene);
                stage.show();
            } else {
                labelMessage.setTextFill(Color.TOMATO);
                labelMessage.setText("Room not found");
            }
        }
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        clientSocket = Main.client;
        if (!CreateRoomController.roomCode.isEmpty()) {
            txtRoomCode.setText(CreateRoomController.roomCode);
            txtRoomCode.setEditable(false);
        }
    }
}
