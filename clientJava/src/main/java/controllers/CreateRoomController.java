package controllers;

import home.Main;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ListView;
import javafx.scene.control.TextArea;
import javafx.scene.input.MouseEvent;
import javafx.stage.Stage;
import socket.ClientSocket;
import socket.MessageBuilder;

import java.io.IOException;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;
import java.util.ResourceBundle;

public class CreateRoomController implements Initializable {
    @FXML
    private ListView<String> listViewQS;

    @FXML
    private Button btnCreateRoom;

    @FXML
    private TextArea txtDescription;

    @FXML
    private Button btnCreateQuestionSuite;

    @FXML
    private Button btnBack;

    private ClientSocket clientSocket;
    public static String roomCode = "";
    private Map<String, String> questionSuiteAndDescription = new HashMap<>();

    @FXML
    void handleCreateRoomBtnClick(MouseEvent event) throws IOException {
        String selectedItem = listViewQS.getSelectionModel().getSelectedItem();
        String sendMessage = MessageBuilder.createCRRequest(selectedItem);
        String receivedMessage = clientSocket.sendMessageToServer(sendMessage);
        String[] parts = receivedMessage.split("_");
        roomCode = parts[1];

        Node node = (Node) event.getSource();
        Stage stage = (Stage) node.getScene().getWindow();
        //stage.setMaximized(true);
        stage.close();
        Parent root = FXMLLoader.load(getClass().getClassLoader().getResource("fxml/JoinRoom.fxml"));
        Scene scene = new Scene(root);
        stage.setScene(scene);
        stage.show();
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        clientSocket = Main.client;
        roomCode = "";
        String sendMessage = MessageBuilder.createQSRequest();
        try {
            String receivedMessage = clientSocket.sendMessageToServer(sendMessage);
            String[] parts = receivedMessage.split("_");
            String[] questionIDParts = parts[1].split(":");

            for (int i = 0; i < questionIDParts.length; i++) {
                String questionSuiteID = questionIDParts[i].split("-")[0];
                String description = questionIDParts[i].split("-")[1];
                questionSuiteAndDescription.put(questionSuiteID, description);
                listViewQS.getItems().add(questionSuiteID);
                listViewQS.scrollTo(listViewQS.getItems().size() - 1);
                listViewQS.layout();
                listViewQS.edit(listViewQS.getItems().size() - 1);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @FXML
    void handleItemClicked(MouseEvent event) {
        String description = questionSuiteAndDescription.get(listViewQS.getSelectionModel().getSelectedItem());
        txtDescription.setText(description);
    }

    @FXML
    void handleBtnBack(MouseEvent event) throws IOException {
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
    void handleCreateQSBtnClick(MouseEvent event) throws IOException {
        Node node = (Node) event.getSource();
        Stage stage = (Stage) node.getScene().getWindow();
        //stage.setMaximized(true);
        stage.close();
        Parent root = FXMLLoader.load(getClass().getClassLoader().getResource("fxml/CreateQuestionSuite.fxml"));
        Scene scene = new Scene(root);
        stage.setScene(scene);
        stage.show();
    }
}
