import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.http.HttpSession;

@WebServlet("/BuyLandServlet")
public class BuyLandServlet {
	private HashMap<Integer, Lands> landsMap = new HashMap<>();
    
    public void init() throws ServletException {
        Monopoly gameInstance = Monopoly.getInstance(); //Get Singleton instance
        List<Lands> landsList = gameInstance.getLandsList(); 

        for (int i = 0; i < landsList.size(); i++) {
            landsMap.put(i, landsList.get(i));
        }
    }
    protected void doPost(HttpServletRequest request, HttpServletResponse response) 
            throws ServletException, IOException {
        
        HttpSession session = request.getSession();
        Players currentPlayer = (Players) session.getAttribute("currentPlayer"); // Get current player
        if (currentPlayer == null) {
            response.getWriter().write("Player not found!"); 
            return;
        }

        int landId = Integer.parseInt(request.getParameter("landId"));
        String choice = request.getParameter("choice");
        Lands land = landsMap.get(landId);

        if (land == null) {
            response.getWriter().write("Invalid land ID.");
            return;
        }
        
        String message = null;
        if ("yes".equals(choice) || "確定".equals(choice)) {
        	int ret=land.effect(currentPlayer);
        	switch (ret){
        		case 1: //successful buy
        			message = "you bought " + land.getGridName() + " for $" + land.getPrice();
        			break;
        		case 0: //not enough money
        			message = "you can't afford the land";
        			break;
        		case -1: //the land is occupied
        			message = "this land is occupied by the other one";
        			break;
        		case -2:
        			message = "you can't afford the toll, you are bankrupt";
        	}
        }
        else{
            message = "You chose to save your money for better place";
        }

        response.setContentType("text/plain");
        response.getWriter().write(message);
    }
}
