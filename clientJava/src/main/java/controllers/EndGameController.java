package controllers;

import com.jfoenix.controls.JFXButton;
import home.Main;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.geometry.Side;
import javafx.scene.Cursor;
import javafx.scene.Group;
import javafx.scene.control.Button;
import javafx.scene.control.ContentDisplay;
import javafx.scene.control.Label;
import javafx.scene.control.ScrollPane;
import javafx.scene.control.TextArea;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.Pane;
import javafx.scene.text.Font;
import org.controlsfx.control.HiddenSidesPane;
import socket.ClientSocket;
import socket.MessageBuilder;

import java.io.IOException;
import java.net.URL;
import java.util.ResourceBundle;

public class EndGameController implements Initializable {
    @FXML
    private Label btnPoint;

    @FXML
    private Button btnHistoryAns;

    @FXML
    private Button btnRankTable;

    @FXML
    private Label lblUsername;

    @FXML
    private ImageView imageViewAvatar;

    @FXML
    private HiddenSidesPane hiddenPane;

    @FXML
    private ImageView openHidden;

    @FXML
    private Group canvasGroup;

    @FXML
    private Pane viewer;

    private Integer totalPoint = 0;
    public AnchorPane hiddenRoot = new AnchorPane();
    public static TextArea textFlow = new TextArea();
    public ScrollPane textContainer = new ScrollPane();
    boolean pinned = false;
    private ClientSocket clientSocket;
    private String roomCode = "";

    @FXML
    void handleBtnRankTableClicked(MouseEvent event) throws IOException {
        String sendMessage = MessageBuilder.createRankTableRequest(roomCode);
        String receivedMessage = clientSocket.sendMessageToServer(sendMessage);
        String[] parts = receivedMessage.split("_");
        clearTextFlow();
        textFlow.setText("Rank Table");
        for (int i = 1; i < parts.length; i++) {
            textFlow.setText(textFlow.getText() + "\n" + i + ". " + parts[i]);
        }
    }

    @FXML
    void handleHistoryAnsBtnClicked(MouseEvent event) throws IOException {
        String sendMessage = MessageBuilder.createHistoryAnswerRequest(LoginController.username);
        String receivedMessage = clientSocket.sendMessageToServer(sendMessage);

        clearTextFlow();
        textFlow.setText("History Answer of " + LoginController.username);
        PlayingController.currentAnswer.forEach((integer, integer2) -> {
            String question = PlayingController.currentQuestions.get(integer)[0];
            String userAnswer = PlayingController.currentQuestions.get(integer)[integer2];
            String result = PlayingController.questionAndAnswer.get(integer).equals(integer2.toString()) ? "TRUE" : "FALSE";
            textFlow.setText(textFlow.getText() + "\n" + integer + ") " + question + "\n" + "Ans: " + userAnswer + "-" + result);
            textFlow.setText(textFlow.getText() + "\n" + "-----");
        });
    }

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        lblUsername.setText(LoginController.username);
        btnPoint.setText(PlayingController.totalPoint.toString());
        clientSocket = Main.client;
        roomCode = JoinRoomController.joinedRoomCOde;

        setUpHiddenPane();
    }

    private void setUpHiddenPane() {
        hiddenPane.setContent(canvasGroup);

        hiddenRoot.setPrefWidth(220);
        hiddenRoot.setPrefHeight(581);

        hiddenRoot.setCursor(Cursor.DEFAULT);
        // Set Label "Detail"
        Label detailLabel = new Label("Detail");
        detailLabel.setPrefSize(hiddenRoot.getPrefWidth() - 20, 38);
        detailLabel.setAlignment(Pos.CENTER);
        detailLabel.setFont(new Font("Roboto", 20));
        detailLabel.setPadding(new Insets(7, 40, 3, -10));
        detailLabel.setStyle("-fx-background-color: #dcdde1;");
        detailLabel.setLayoutX(35);

        // Set TextFlow pane properties
        textFlow.setPrefSize(hiddenRoot.getPrefWidth(), hiddenRoot.getPrefHeight() - 2);
//        textFlow.prefHeightProperty().bind(hiddenRoot.heightProperty());
        textFlow.setStyle("-fx-background-color: #dfe6e9;");
        textFlow.setLayoutY(39);
        textContainer.setLayoutY(textFlow.getLayoutY());
        textFlow.setPadding(new Insets(5, 0, 0, 5));
        textFlow.setEditable(false);
        textContainer.setContent(textFlow);

        // Set Pin/Unpin Button
        JFXButton pinUnpin = new JFXButton();
        pinUnpin.setContentDisplay(ContentDisplay.GRAPHIC_ONLY);

        ImageView imgPin = new ImageView(new Image(getClass().getResourceAsStream("/images/pinned.png")));
        imgPin.setFitHeight(20);
        imgPin.setFitWidth(20);
        ImageView imgUnpin = new ImageView(new Image(getClass().getResourceAsStream("/images/unpinned.png")));
        imgUnpin.setFitHeight(20);
        imgUnpin.setFitWidth(20);
        pinUnpin.setGraphic(imgPin);

        pinUnpin.setPrefSize(20, 39);
        pinUnpin.setButtonType(JFXButton.ButtonType.FLAT);
        pinUnpin.setStyle("-fx-background-color: #dcdde1;");
        pinUnpin.setOnMouseClicked(e -> {
            if (pinned) {
                pinUnpin.setGraphic(imgPin);
                hiddenPane.setPinnedSide(null);
                pinned = false;
            } else {
                pinUnpin.setGraphic(imgUnpin);
                hiddenPane.setPinnedSide(Side.RIGHT);
                pinned = true;
            }
        });

        // Add Label and TextFlow to hiddenPane
        hiddenRoot.getChildren().addAll(pinUnpin, detailLabel, textContainer);
        hiddenPane.setRight(hiddenRoot);
        hiddenRoot.setOnMouseEntered(e -> {
            hiddenPane.setPinnedSide(Side.RIGHT);
            openHidden.setVisible(false);
            e.consume();
        });
        hiddenRoot.setOnMouseExited(e -> {
            if (!pinned) {
                hiddenPane.setPinnedSide(null);
                openHidden.setVisible(true);
            }
            e.consume();
        });
        hiddenPane.setTriggerDistance(100);
    }

    private void clearTextFlow() {
        textFlow.clear();
    }
}
