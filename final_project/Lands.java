public class Lands extends Grids{
    private int price;
    private int toll;
    private Players owner;
    public Lands(String gridName, int price){
        super(gridName);
        this.price=price;
        this.toll=-1;
        this.owner=null;
    }

    public Players getOwner(){
        return this.owner;
    }

    public int getPrice(){
        return this.price;
    }

    public int getToll(){
        return this.toll;
    }

    @Override
    public int effect(Players player){
        if(this.owner==null){
            //ask the player if they want to buy the land (handled in JSP)
            if(player.getMoney() >= this.price){
                player.setMoney(player.getMoney() - this.price);
                this.owner=player;
                this.toll=(int)(0.4*this.price); //set toll to 40% of land price
                return 1;
            }
            else{
                //post a pop up says "you can't afford the land" at frontend
            	return 0;
            }
        }
        else if(!this.owner.equals(player)){
            if (player.getMoney() >= this.toll) {
                player.setMoney(player.getMoney() - this.toll);
                this.owner.setMoney(this.owner.getMoney() + this.toll);
                //Pay toll if another player owns the land
                return -1;
            }
        }
        else{
        	player.bankrupt();
        }
        //player can't afford the toll, handle bankrupt
        return -2;
    }
}
