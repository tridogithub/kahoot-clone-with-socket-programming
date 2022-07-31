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
import javafx.stage.FileChooser;
import javafx.stage.Stage;
import socket.ClientSocket;
import socket.MessageBuilder;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.net.URL;
import java.util.ResourceBundle;

public class CreateQuestionSuiteController implements Initializable {
    @FXML
    private TextField txtFilePath;

    @FXML
    private Button btnJoin;

    @FXML
    private Button btnBack;

    @FXML
    private Label labelMessage;

    @FXML
    private Button btnChooseFile;

    private File questionSuiteFile;
    private ClientSocket clientSocket;

    @FXML
    void handleBackBtnClick(MouseEvent event) throws IOException {
        Node node = (Node) event.getSource();
        Stage stage = (Stage) node.getScene().getWindow();
        //stage.setMaximized(true);
        stage.close();
        Parent root = FXMLLoader.load(getClass().getClassLoader().getResource("fxml/CreateRoom.fxml"));
        Scene scene = new Scene(root);
        stage.setScene(scene);
        stage.show();
    }

    @FXML
    void handleChooseFileBtnClick(MouseEvent event) {
        Stage primaryStage = (Stage) ((Node) event.getSource()).getScene().getWindow();
        FileChooser fileChooser = new FileChooser();
        questionSuiteFile = fileChooser.showOpenDialog(primaryStage);
        if (questionSuiteFile != null) {
            System.out.println("File absolute path: " + questionSuiteFile.getAbsolutePath());
            txtFilePath.setText(questionSuiteFile.getAbsolutePath());
        }
    }

    @FXML
    void handleSaveBtnClick(MouseEvent event) throws IOException {
        sendCreateFileRequest();
        BufferedReader reader;
        try {
            reader = new BufferedReader(new FileReader(questionSuiteFile));
            String line = reader.readLine();
            while (line != null) {
                System.out.println(line);
                // read next line
                sendAppendFileRequest(line);
                line = reader.readLine();
            }
            labelMessage.setText("Save question suite successfully.");
            labelMessage.setTextFill(Color.GREEN);
            reader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        clientSocket = Main.client;
        labelMessage.setText("");
    }

    private void sendCreateFileRequest() throws IOException {
        String sendMessage = MessageBuilder.createNewFileRequest(questionSuiteFile.getName());
        String receivedMessage = clientSocket.sendMessageToServer(sendMessage);
    }

    private void sendAppendFileRequest(String content) throws IOException {
        String sendMessage = MessageBuilder.createAppendFileRequest(questionSuiteFile.getName(), content);
        String receivedMessage = clientSocket.sendMessageToServer(sendMessage);

    }

}
