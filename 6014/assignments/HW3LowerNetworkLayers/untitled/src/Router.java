

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

public class Router {

    private HashMap<Router, Integer> distances;
    private String name;
    public Router(String name) {
        this.distances = new HashMap<>();
        this.name = name;
    }

    public void onInit() throws InterruptedException {
        Set<Router> routers = Network.getRouters();
        HashMap<Router, Integer> distance = new HashMap<>(distances);
        for(Router neighbor: routers){
            if(!neighbor.equals(this)){
                distances.put(neighbor, Integer.MAX_VALUE/2);
            }
        }
        distances.put(this, 0);
        for(Router neighbor : routers){
            if(!neighbor.equals(this)){
                Message message = new Message(this, neighbor, distance);
                Network.sendDistanceMessage(message);
            }
        }
    }

    public void onDistanceMessage(Message message) throws InterruptedException {
		//update your distance table and broadcast it to your neighbors if it changed
        Router source = message.sender;
        HashMap<Router, Integer> receivedTable = message.distances;
        boolean tableChanged = false;
        for(Map.Entry<Router, Integer> entry : receivedTable.entrySet()){
            Router router = entry.getKey();
            int newDistance = entry.getValue();
            if(distances.get(router) > distances.get(source) + newDistance){
                distances.put(router, distances.get(source) + newDistance);
                tableChanged = true;
            }
        }
        if(tableChanged){
            Set<Router> routers = Network.getRouters();
            for (Router neighbor : routers) {
                if (!neighbor.equals(this) && !neighbor.equals(source)) {
                    Message broadcastMessage = new Message(this, neighbor, distances);
                    Network.sendDistanceMessage(broadcastMessage);
                }
            }
        }
    }


    public void dumpDistanceTable() {
        System.out.println("router: " + this);
        for(Router r : distances.keySet()){
            System.out.println("\t" + r + "\t" + distances.get(r));
        }
    }

    @Override
    public String toString(){
        return "Router: " + name;
    }
}
