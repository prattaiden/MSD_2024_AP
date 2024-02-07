import java.util.HashMap;

public class DNSCache {
    private static final HashMap<DNSQuestion, DNSRecord> cache_ = new HashMap<>();


    public DNSRecord queryCache(DNSQuestion question){
        if(cache_.containsKey(question)){
            DNSRecord record = cache_.get(question);
            if(record.isExpired()){
                cache_.remove(question); //remove if expired
                return null;
            }
            //if the query is not expired
            else{
                return record;
            }
        }
        return null;
    }

    public void addToCache(DNSQuestion question, DNSRecord answer){
        //adding to the cache hashmap
         cache_.put(question, answer);
    }

    //You can just store the first answer for any question in the cache
    // (a response for google.com might return 10 IP addresses, just store the first one).
    // This class should have methods for querying and inserting records into the cache.
    // When you look up an entry, if it is too old (its TTL has expired), remove it and return "not found."


}
