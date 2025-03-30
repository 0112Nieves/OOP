import java.util.List;

public class Game {
    protected List<Players> playerList;
    protected String gameName;

    public Game(String gameName, List<Players> playerList) {
        this.gameName = gameName;
        this.playerList = playerList;
    }

    public void init() {
        System.out.println("Initializing game: " + gameName);
        // 初始化玩家
        for (Players player : playerList) {
            player.setLocation(0);  // 設置初始位置
        }
    }

    public void end() {
        System.out.println("Game Over!");
        // 可在這裡添加額外的遊戲結束條件，例如計算最富有的玩家
    }

    public List<Players> getPlayers() {
        return playerList;
    }
}