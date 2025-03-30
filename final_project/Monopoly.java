import java.util.*;

public class Monopoly extends Game {
    private List<Grid> gridsList;
    private int currentPlayerIndex;

    public Monopoly(String gameName, List<Players> playerList, List<Grid> gridsList) {
        super(gameName, playerList);
        this.gridsList = gridsList;
        this.currentPlayerIndex = 0;
    }

    public void nextPlayer() {
        Players currentPlayer = playerList.get(currentPlayerIndex);
        int diceRoll = currentPlayer.rollDice();
        
        // 計算新位置
        int newPosition = (currentPlayer.getLocation() + diceRoll) % gridsList.size();
        currentPlayer.setLocation(newPosition);

        // 觸發格子效果
        Grid landedGrid = gridsList.get(newPosition);
        String result = landedGrid.effect(currentPlayer);
        System.out.println(result);

        // 切換到下一位玩家
        currentPlayerIndex = (currentPlayerIndex + 1) % playerList.size();
    }

    public List<Grid> getGridsList() {
        return gridsList;
    }
}