package socket;

public class MessageBuilder {

    public static String createSignUpRequest(String username, String password) {
        return Constant.REGISTER + "_" + username + ":" + password;
    }

    public static String createSignInRequest(String username, String password) {
        return Constant.LOGIN + "_" + username + ":" + password;
    }

    public static String createQSRequest() {
        return Constant.REQUEST_QS + "_";
    }

    public static String createCRRequest(String questionID) {
        return Constant.CREATE_ROOM + "_" + questionID;
    }

    public static String createJRRequest(String roomCode) {
        return Constant.JOIN_ROOM + "_" + roomCode;
    }

    public static String createGetQuestionRequest(String roomCode, int questionID) {
        return Constant.GET_QUESTION + "_" + roomCode + "_" + questionID;
    }

    public static String createGetAnswerDetailRequest(String roomCode, int questionId) {
        return Constant.GET_ANSWER_DETAIL + "_" + roomCode + "_" + questionId;
    }

    public static String createSendAnswerRequest(String roomCode, int questionId, int answer, String status) {
        return Constant.SEND_ANSWER + "_" + roomCode + "_" + questionId + ":" + answer + ":" + status;
    }

    public static String createAnswerDetailRequest(String roomCode, int questionId) {
        return Constant.REQUEST_ANS_DETAIL + "_" + roomCode + "_" + questionId;
    }

    public static String createRankTableRequest(String roomCode) {
        return Constant.REQUEST_RANK_TABLE + "_" + roomCode;
    }

    public static String createHistoryAnswerRequest(String username) {
        return Constant.REQUEST_HISTORY_ANSWER + "_" + username;
    }

    public static String createNewFileRequest(String fileName) {
        return Constant.REQUEST_CREATE_FILE + "_" + fileName;
    }

    public static String createAppendFileRequest(String fileName, String content) {
        return Constant.REQUEST_APPEND_FILE + "_" + fileName + "-" + content;
    }
}
