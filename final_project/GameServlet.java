import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.util.*;

@WebServlet("/game")
public class GameServlet extends HttpServlet {
    private static Monopoly gameInstance;

    @Override
    public void init() throws ServletException {
        if (gameInstance == null) {
            List<Players> playerList = new ArrayList<>();
            playerList.add(new Players("Player 1"));
            playerList.add(new Players("Player 2"));
            playerList.add(new Players("Player 3"));
            playerList.add(new Players("Player 4"));

            List<Grid> gridsList = new ArrayList<>();
            for (int i = 0; i < 76; i++) {
                gridsList.add(new Land("Land " + i, 100 + i * 10)); // TODO: 第二個參數代表價錢，現在為預設，需更改成正確的數值
            }

            gameInstance = new Monopoly("Monopoly", playerList, gridsList);
            gameInstance.init();
        }
    }

    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response)
            throws ServletException, IOException {
        String action = request.getParameter("action");
        response.setContentType("application/json");

        if ("start".equals(action)) {
            response.getWriter().write("{\"message\": \"Game Initialized\"}");
        } else if ("nextPlayer".equals(action)) {
            gameInstance.nextPlayer();
            Players currentPlayer = gameInstance.getPlayers().get(gameInstance.getCurrentPlayerIndex());
            Grid landedGrid = gameInstance.getGridsList().get(currentPlayer.getLocation());

            String json = String.format(
                "{\"currentPlayer\": \"%s\", \"position\": %d, \"landedGrid\": \"%s\"}",
                currentPlayer.getPlayerName(), currentPlayer.getLocation(), landedGrid.getGridName()
            );
            response.getWriter().write(json);
        } else {
            response.getWriter().write("{\"error\": \"Invalid action\"}");
        }
    }
}
