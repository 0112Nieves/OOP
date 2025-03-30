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
        for (Players player : playerList) {
            player.setLocation(0);
        }
    }

    public void end() {
        System.out.println("Game Over!");
        // TODO: 遊戲結束條件判斷
    }

    public List<Players> getPlayers() {
        return playerList;
    }
}