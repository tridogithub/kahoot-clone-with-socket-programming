package controllers;

import home.Main;
import javafx.animation.KeyFrame;
import javafx.animation.KeyValue;
import javafx.animation.Timeline;
import javafx.beans.property.IntegerProperty;
import javafx.beans.property.SimpleIntegerProperty;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.chart.PieChart;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;
import javafx.util.Duration;
import socket.ClientSocket;
import socket.MessageBuilder;

import java.io.IOException;
import java.net.URL;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.ResourceBundle;

public class PlayingController implements Initializable {
    private PieChart pieChartAns;

    @FXML
    private TextField textQuestion;

    @FXML
    private Button btnAnswer1;

    @FXML
    private Button btnAnswer2;

    @FXML
    private Button btnAnswer3;

    @FXML
    private Button btnAnswer4;

    @FXML
    private Label labelTotalPoint;

    @FXML
    private Label lblUsername;

    @FXML
    private Label lblTimer;
    @FXML
    private Pane subPane;

    private ClientSocket clientSocket;

    private static final Integer START_TIME = 10;
    private Timeline timeline;
    private IntegerProperty timeSeconds = new SimpleIntegerProperty(START_TIME);
    public static Map<Integer, String[]> currentQuestions = new HashMap<>();
    public static Map<Integer, String> questionAndAnswer = new HashMap<>();
    public static Map<Integer, Integer> currentAnswer = new HashMap<>();
    private int currentQuestionID = 1;
    private static final String CONTINUE = "CON";
    private static final String FINAL = "FINAL";
    private int currentChoice = 0;
    private Button currentChoiceBtn = null;
    private boolean isFinished = false;
    public static Integer totalPoint = 0;
    private String roomCode = "";
    private String trueChoice = "TRUE";


    @Override
    public void initialize(URL location, ResourceBundle resources) {
        textQuestion.setEditable(false);
        clientSocket = Main.client;
        lblUsername.setText(LoginController.username);
        totalPoint = 0;
        labelTotalPoint.setText(String.valueOf(totalPoint));
        roomCode = JoinRoomController.joinedRoomCOde;
        currentChoiceBtn = btnAnswer1;
        pieChartAns = new PieChart();

        try {
            getQuestionFromServer(roomCode, currentQuestionID);
//            currentQuestionID += 1;
            startTimeCounter();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    private void getQuestionFromServer(String roomCode, int questionID) throws IOException {
        String sendMessage = MessageBuilder.createGetQuestionRequest(roomCode, questionID);
        String receivedMessage = clientSocket.sendMessageToServer(sendMessage);

        String[] parts = receivedMessage.split("_"); //getQ_1_question:answer1:answer2:answer3:answer4:correctAns_CON
        String code = parts[0];
        String questionIDString = parts[1];
        String questionAndAnswer = parts[2];
        String status = questionAndAnswer.split(":")[6];

        saveQuestionAndAnswer(questionAndAnswer, questionID);
        if (FINAL.equals(status)) {
            //TODO
            isFinished = true;
        }
        visualizeQuestion();
    }

    private void saveQuestionAndAnswer(String questionAndAnswerFormatted, int questionID) {
        String[] parts = questionAndAnswerFormatted.split(":");
        currentQuestions.put(questionID, Arrays.copyOfRange(parts, 0, 5));
        questionAndAnswer.put(questionID, parts[5]);
    }

    private void visualizeQuestion() {
        textQuestion.setText(currentQuestions.get(currentQuestionID)[0]);
        btnAnswer1.setText(currentQuestions.get(currentQuestionID)[1]);
        btnAnswer2.setText(currentQuestions.get(currentQuestionID)[2]);
        btnAnswer3.setText(currentQuestions.get(currentQuestionID)[3]);
        btnAnswer4.setText(currentQuestions.get(currentQuestionID)[4]);
    }

    private void startTimeCounter() {
        lblTimer.textProperty().bind(timeSeconds.asString());
        if (timeline != null) {
            timeline.stop();
        }
        timeSeconds.set(START_TIME);
        timeline = new Timeline();
        timeline.getKeyFrames().add(
                new KeyFrame(Duration.seconds(START_TIME + 1),
                        new KeyValue(timeSeconds, 0)));
        timeline.playFromStart();
        //TODO on finish time counter
        timeline.setOnFinished(event -> {
            //calculate total point
            if (String.valueOf(currentChoice).equals(questionAndAnswer.get(currentQuestionID))) {
                trueChoice = "TRUE";
                totalPoint += 10;
                labelTotalPoint.setText(totalPoint.toString());
                currentChoiceBtn.setStyle(currentChoiceBtn.getStyle() + "-fx-border-color: #06ff00; -fx-border-width: 5px;");
            } else {
                trueChoice = "FALSE";
                currentChoiceBtn.setStyle(currentChoiceBtn.getStyle() + "-fx-border-color: #ff0000; -fx-border-width: 5px;");
            }
            //send answer
            sendAnswer();
            Timeline timeline2 = new Timeline(new KeyFrame(Duration.seconds(2), event1 -> {
                //show pie chart
                showPieChart();
            }));
            timeline2.play();

            if (isFinished) {
                try {
                    Stage stage = (Stage) btnAnswer1.getScene().getWindow();
                    stage.close();
                    Parent root = null;
                    root = FXMLLoader.load(getClass().getClassLoader().getResource("fxml/EndGameScreen.fxml"));
                    Scene scene = new Scene(root);
                    stage.setScene(scene);
                    stage.show();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            } else {
                Timeline timeline1 = new Timeline(new KeyFrame(Duration.seconds(5), event1 -> {
                    reset();
                    try {
                        currentQuestionID += 1;
                        getQuestionFromServer(roomCode, currentQuestionID);
                        startTimeCounter();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }));
                timeline1.play();
            }
        });
    }

    private void showPieChart() {
        try {
            String sendMessage = MessageBuilder.createAnswerDetailRequest(roomCode, currentQuestionID);
            String receivedMessage = clientSocket.sendMessageToServer(sendMessage);
            String[] parts = receivedMessage.split("_");
            String[] answerParts = parts[1].split(":");

            PieChart.Data slice1 = new PieChart.Data(Integer.valueOf(answerParts[0]) + " answers", Integer.valueOf(answerParts[0]));

            PieChart.Data slice2 = new PieChart.Data(Integer.valueOf(answerParts[1]) + " answers", Integer.valueOf(answerParts[1]));
            PieChart.Data slice3 = new PieChart.Data(Integer.valueOf(answerParts[2]) + " answers", Integer.valueOf(answerParts[2]));
            PieChart.Data slice4 = new PieChart.Data(Integer.valueOf(answerParts[3]) + " answers", Integer.valueOf(answerParts[3]));

            pieChartAns.getData().addAll(slice1, slice2, slice3, slice4);
            pieChartAns.setVisible(true);
            pieChartAns.setMaxSize(400, 260);
            if (!subPane.getChildren().contains(pieChartAns)) {
                subPane.getChildren().add(pieChartAns);
            }
            slice1.getNode().setStyle("-fx-pie-color: red");
            slice2.getNode().setStyle("-fx-pie-color:  #1368ce");
            slice3.getNode().setStyle("-fx-pie-color:  #ffdd33");
            slice4.getNode().setStyle("-fx-pie-color:  #26890c");

            lblTimer.setVisible(false);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void sendAnswer() {
        try {
            String sendMessage = MessageBuilder.createSendAnswerRequest(roomCode, currentQuestionID, currentChoice, trueChoice);
            String receivedMessage = clientSocket.sendMessageToServer(sendMessage);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void handleAnswer1Click(MouseEvent event) {
        currentChoice = 1;
        currentChoiceBtn = btnAnswer1;
        currentAnswer.put(currentQuestionID, 1);
        btnAnswer1.setStyle(btnAnswer1.getStyle() + "-fx-border-color: #ffff00; -fx-border-width: 5px;");
        btnAnswer2.setDisable(true);
        btnAnswer3.setDisable(true);
        btnAnswer4.setDisable(true);

    }

    public void handleAnswer2Click(MouseEvent event) {
        currentChoice = 2;
        currentChoiceBtn = btnAnswer2;
        currentAnswer.put(currentQuestionID, 2);
        btnAnswer2.setStyle(btnAnswer2.getStyle() + "-fx-border-color: #ffff00; -fx-border-width: 5px;");
        btnAnswer1.setDisable(true);
        btnAnswer3.setDisable(true);
        btnAnswer4.setDisable(true);
    }

    public void handleAnswer3Click(MouseEvent event) {
        currentChoice = 3;
        currentChoiceBtn = btnAnswer3;
        currentAnswer.put(currentQuestionID, 3);
        btnAnswer3.setStyle(btnAnswer3.getStyle() + "-fx-border-color: #ffff00; -fx-border-width: 5px;");
        btnAnswer2.setDisable(true);
        btnAnswer1.setDisable(true);
        btnAnswer4.setDisable(true);
    }

    public void handleAnswer4Click(MouseEvent event) {
        currentChoice = 4;
        currentChoiceBtn = btnAnswer4;
        currentAnswer.put(currentQuestionID, 4);
        btnAnswer4.setStyle(btnAnswer4.getStyle() + "-fx-border-color: #ffff00; -fx-border-width: 5px;");
        btnAnswer2.setDisable(true);
        btnAnswer3.setDisable(true);
        btnAnswer1.setDisable(true);
    }

    private void reset() {
        btnAnswer1.setStyle(btnAnswer1.getStyle() + "-fx-border-color: none;");
        btnAnswer2.setStyle(btnAnswer2.getStyle() + "-fx-border-color: none;");
        btnAnswer3.setStyle(btnAnswer3.getStyle() + "-fx-border-color: none;");
        btnAnswer4.setStyle(btnAnswer4.getStyle() + "-fx-border-color: none;");
        btnAnswer1.setDisable(false);
        btnAnswer2.setDisable(false);
        btnAnswer3.setDisable(false);
        btnAnswer4.setDisable(false);

        lblTimer.setVisible(true);
        pieChartAns.setVisible(false);
        pieChartAns.getData().clear();
    }
}
