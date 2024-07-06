
using Microsoft.EntityFrameworkCore.Query.SqlExpressions;

static class PgnReader{


    public static List<ChessGame> ReadChessGamePgn(string filePath){

        List<ChessGame> games = new List<ChessGame>();

        if(!File.Exists(filePath)){
            throw new FileNotFoundException("file not found", filePath);
        }

        string[] pgnLines = File.ReadAllLines(filePath);
        ChessGame currentGame = null;
        foreach(var line in pgnLines){
            //check for a blank line at the end of a game
            if(string.IsNullOrWhiteSpace(line)){
                //if the current game is not null, then add it to the games Lis
                if(currentGame.moves_!= null){
    
                    games.Add(currentGame);

                    //reset 
                    currentGame = null;
               
                }

            }

           else if(line.StartsWith('[')){
                // System.Diagnostics.Debug.WriteLine("made it to starts with []");
                //switch cases to find each value to add to chess game?

                //establish a new game if the current is null
                if (currentGame == null){
                    currentGame = new ChessGame();
                }

                //Trim removes the beginning and trailing '[]'
                //Split splits the tag into two substrings seperated by the ' ' space
                var tagType = line.Trim('[',']').Split(' ', 2);
                var tag = tagType[0]; //should just give 'Event ...' followed by name etc.
                var value = tagType[1].Trim('"');

                int intValue = 0;
                
                if(Int32.TryParse(value, out intValue)){
                    // System.Diagnostics.Debug.WriteLine("int value for elo: " + intValue);
                }
    
                switch(tag){
                    case("Event"):
                        currentGame.event_ = value;
                        break;
                    case("Site"):
                        currentGame.site_ = value;
                        break;
                    case("Round"):
                        currentGame.round_ = value;
                        break;
                    case("White"):
                        currentGame.white_ = value;
                        break;
                    case("Black"):
                        currentGame.black_ = value;
                        break;
                    case("WhiteElo"):
                        currentGame.whiteElo_ = intValue;
                        break;
                    case("BlackElo"):
                        currentGame.blackElo_ = intValue;
                        break;
                    case("Result"):
                    //TODO add W or B for who wins
                        if(value == "1/2-1/2"){
                            currentGame.result_ = "D";
                        }
                        if(value == "1-0"){
                            currentGame.result_ = "W";
                        }
                        if(value == "0-1"){
                            currentGame.result_ = "B";
                        }

                      
                        break;
                    case("EventDate"):
                        currentGame.date_ = value;
                        break;
                }


            }
            else{
                currentGame.moves_ += line;
            }
                
        }
        
        System.Diagnostics.Debug.WriteLine("completed parsing the chess game pgn");
        return games;
 
    }
}
    
