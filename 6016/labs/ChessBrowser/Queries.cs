using Microsoft.Maui.Controls;
using MySqlConnector;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChessBrowser
{
  internal class Queries
  {

    /// <summary>
    /// This function runs when the upload button is pressed.
    /// Given a filename, parses the PGN file, and uploads
    /// each chess game to the user's database.
    /// </summary>
    /// <param name="PGNfilename">The path to the PGN file</param>
    internal static async Task InsertGameData( string PGNfilename, MainPage mainPage )
    {
      // This will build a connection string to your user's database on atr,
      // assuimg you've typed a user and password in the GUI
      System.Diagnostics.Debug.WriteLine("File: " + PGNfilename);
  
      //------------------------PRINTING THE FIRST GAME IN THE PGN FILE --------------------------//
       List<ChessGame> gamesRead = PgnReader.ReadChessGamePgn(PGNfilename);
      //testing the games are parsed and output
      // System.Diagnostics.Debug.WriteLine(gamesRead[0].event_);
      // System.Diagnostics.Debug.WriteLine(gamesRead[0].site_);
      // System.Diagnostics.Debug.WriteLine(gamesRead[0].round_);
      // System.Diagnostics.Debug.WriteLine(gamesRead[0].white_);
      // System.Diagnostics.Debug.WriteLine(gamesRead[0].whiteElo_);
      // System.Diagnostics.Debug.WriteLine(gamesRead[0].black_);
      // System.Diagnostics.Debug.WriteLine(gamesRead[0].blackElo_);
      // System.Diagnostics.Debug.WriteLine(gamesRead[0].result_);
      // System.Diagnostics.Debug.WriteLine(gamesRead[0].date_);
      // System.Diagnostics.Debug.WriteLine(gamesRead[0].moves_);
    

      string connection = mainPage.GetConnectionString();

      mainPage.SetNumWorkItems(gamesRead.Count);

      using ( MySqlConnection conn = new MySqlConnection( connection ) )
      {
        try
        {
          // Open a connection
          conn.Open();
          //now loop through the games in the file to be added to the database
          foreach(var game in gamesRead){
    
            //-----------------------INSERTING PLAYER DATA--------------------------------------------------------//
              // +-------+------------------+------+-----+---------+----------------+
              // | Field | Type             | Null | Key | Default | Extra          |
              // +-------+------------------+------+-----+---------+----------------+
              // | Name  | varchar(255)     | YES  | UNI | NULL    |                |
              // | Elo   | int(10) unsigned | YES  |     | NULL    |                |
              // | pID   | int(10) unsigned | NO   | PRI | NULL    | auto_increment |
              // +-------+------------------+------+-----+---------+----------------+
            string insertSQLPlayer = @"INSERT INTO Players (Name, Elo, pID) 
            VALUES (@Name, @Elo, @pID) ON DUPLICATE KEY UPDATE Elo = GREATEST(Elo, Values(Elo))";

            using (MySqlCommand PlayerCommand = new MySqlCommand(insertSQLPlayer, conn)){
                PlayerCommand.Parameters.AddWithValue("@Name", game.white_);
                PlayerCommand.Parameters.AddWithValue("@Elo", game.whiteElo_);
                PlayerCommand.Parameters.AddWithValue("@pID", 0);
                await PlayerCommand.ExecuteNonQueryAsync();
                //now update or add the data for the next player
                PlayerCommand.Parameters["@Name"].Value = game.black_;
                PlayerCommand.Parameters["@Elo"].Value = game.blackElo_;
                PlayerCommand.Parameters["@pID"].Value = 0;
                await PlayerCommand.ExecuteNonQueryAsync();
            }

            int whitePlayerID = await GetPlayerID(conn, game.white_);
            int blackPlayerID = await GetPlayerID(conn, game.black_);
            

       //----------------------------INSERTING EVENTS DATA-------------------------------------------------------//
            // +-------+------------------+------+-----+---------+----------------+
            // | Field | Type             | Null | Key | Default | Extra          |
            // +-------+------------------+------+-----+---------+----------------+
            // | Name  | varchar(255)     | NO   | MUL | NULL    |                |
            // | Site  | varchar(255)     | NO   |     | NULL    |                |
            // | Date  | date             | NO   |     | NULL    |                |
            // | eID   | int(10) unsigned | NO   | PRI | NULL    | auto_increment |
            // +-------+------------------+------+-----+---------+----------------+

            string insertSQLEvent = @"INSERT INTO Events (Name, Site, Date, eID) 
            VALUES (@Name, @Site, @Date, @eID) 
            ON DUPLICATE KEY UPDATE Name = Values(Name) , Site = Values(Site), Date = Values(Date)";
            using(MySqlCommand EventCommand = new MySqlCommand(insertSQLEvent, conn)){
              EventCommand.Parameters.AddWithValue("@Name", game.event_);
              EventCommand.Parameters.AddWithValue("@Site", game.site_);
              EventCommand.Parameters.AddWithValue("@Date", game.date_);
              EventCommand.Parameters.AddWithValue("@eID", 0);
              await EventCommand.ExecuteNonQueryAsync(); 
            }

            int eventID = await getEventID(conn, game.event_, game.site_, game.date_);

            //-----------------------------INSERTING GAME DATA--------------------------------------------------------//
            // +-------------+------------------+------+-----+---------+-------+
            // | Field       | Type             | Null | Key | Default | Extra |
            // +-------------+------------------+------+-----+---------+-------+
            // | Round       | varchar(10)      | NO   | PRI | NULL    |       |
            // | Result      | char(1)          | NO   |     | NULL    |       |
            // | Moves       | varchar(2000)    | NO   |     | NULL    |       |
            // | BlackPlayer | int(10) unsigned | NO   | PRI | NULL    |       |
            // | WhitePlayer | int(10) unsigned | NO   | PRI | NULL    |       |
            // | eID         | int(10) unsigned | NO   | PRI | NULL    |       |
            // +-------------+------------------+------+-----+---------+-------+
            string insertSQLGame = @"INSERT INTO Games (Round, Result, Moves, BlackPlayer, WhitePlayer, eID)
            VALUES (@Round, @Result, @Moves, @BlackPlayer, @WhitePlayer, @eID)";

            using (MySqlCommand GameCommand = new MySqlCommand(insertSQLGame, conn)){
              GameCommand.Parameters.AddWithValue("@Round", game.round_);
              GameCommand.Parameters.AddWithValue("@Result", game.result_);
              GameCommand.Parameters.AddWithValue("@Moves", game.moves_);
              GameCommand.Parameters.AddWithValue("@BlackPlayer", blackPlayerID);
              GameCommand.Parameters.AddWithValue("@WhitePlayer", whitePlayerID);
              GameCommand.Parameters.AddWithValue("@eID", eventID);
              await GameCommand.ExecuteNonQueryAsync();
            }
            await mainPage.NotifyWorkItemCompleted();
          }
        }
        catch ( Exception e ){
          System.Diagnostics.Debug.WriteLine( e.Message );
        }
      }
    }

    private static async Task<int> GetPlayerID(MySqlConnection conn, string playerName){
      string sql = "SELECT pID FROM Players WHERE Name = @Name";
      using (MySqlCommand cmd = new MySqlCommand(sql, conn)){
        cmd.Parameters.AddWithValue("@Name", playerName);
        object result = await cmd.ExecuteScalarAsync();
        if (result != null){
          return Convert.ToInt32(result);
        }
        else{
          throw new Exception($"Player ID not found for player: {playerName}");
        }
      }
    }

    private static async Task<int> getEventID(MySqlConnection conn, string eventName, string site, string date){
      string selectSql = "SELECT eID FROM Events WHERE Name = @Name AND Site = @Site AND Date = @Date";
      using (MySqlCommand cmd = new MySqlCommand(selectSql, conn)){
        cmd.Parameters.AddWithValue("@Name", eventName);
        cmd.Parameters.AddWithValue("@Site", site);
        cmd.Parameters.AddWithValue("@Date", date);
        object result = await cmd.ExecuteScalarAsync();
        if(result != null){
          return Convert.ToInt32(result);
        }
        else{
          throw new Exception($"Event ID not found for event: {eventName}");
        }
      }
    }

    /// <summary>
    /// Queries the database for games that match all the given filters.
    /// The filters are taken from the various controls in the GUI.
    /// </summary>
    /// <param name="white">The white player, or null if none</param>
    /// <param name="black">The black player, or null if none</param>
    /// <param name="opening">The first move, e.g. "1.e4", or null if none</param>
    /// <param name="winner">The winner as "W", "B", "D", or null if none</param>
    /// <param name="useDate">True if the filter includes a date range, False otherwise</param>
    /// <param name="start">The start of the date range</param>
    /// <param name="end">The end of the date range</param>
    /// <param name="showMoves">True if the returned data should include the PGN moves</param>
    /// <returns>A string separated by newlines containing the filtered games</returns>
    internal static string PerformQuery( string white, string black, string opening,
      string winner, bool useDate, DateTime start, DateTime end, bool showMoves,
      MainPage mainPage ){
      // This will build a connection string to your user's database on atr,
      // assuimg you've typed a user and password in the GUI
      string connection = mainPage.GetConnectionString();

      // Build up this string containing the results from your query
      string parsedResult = "";



      // Use this to count the number of rows returned by your query
      // (see below return statement)
      int numRows = 0;

      using ( MySqlConnection conn = new MySqlConnection( connection ) )
      {
        try
        {
          // Open a connection
          conn.Open();

          //combine all the tables
           // Simple query to get all games
          string query = @"SELECT Events.Name AS Event, Events.Site, Events.Date, PWhite.Name AS White, PWhite.Elo AS WhiteElo, PBlack.Name AS Black, PBlack.Elo AS BlackElo, 
                    Games.Result, Games.Moves FROM Games INNER JOIN Players PWhite ON Games.WhitePlayer = PWhite.pID INNER JOIN Players PBlack ON Games.BlackPlayer = PBlack.pID 
                INNER JOIN Events ON Games.eID = Events.eID WHERE 1=1";



          if(!string.IsNullOrWhiteSpace(white)){
            query += " AND PWhite.Name = @White";
          }

          if(!string.IsNullOrWhiteSpace(black)){
            query+= " AND PBlack.Name = @Black";
          }

          if(!string.IsNullOrWhiteSpace(opening)){
            query+= " AND Games.Moves LIKE @Opening";
          }

          if(!string.IsNullOrWhiteSpace(winner)){
            query+= " AND Games.Result = @Winner";
          }


          if(useDate){
            query+= "AND Events.Date BETWEEN @Start AND @End";
          }

        
          //calling the sql query
          MySqlCommand cmd = new MySqlCommand(query, conn);

          if(!string.IsNullOrEmpty(white)){
            cmd.Parameters.AddWithValue("@White", white);
          }

          if(!string.IsNullOrEmpty(black)){
            cmd.Parameters.AddWithValue("@Black", black);
          }

          if (!string.IsNullOrEmpty(opening)){
            cmd.Parameters.AddWithValue("@Opening", opening + "%");
          }

          if (!string.IsNullOrEmpty(winner)){
            cmd.Parameters.AddWithValue("@Winner", winner);  
          }

          if(useDate){
                  cmd.Parameters.AddWithValue("@Start", start.ToString("yyyy-MM-dd"));
                cmd.Parameters.AddWithValue("@End", end.ToString("yyyy-MM-dd"));
          }

       
        
          using var reader = cmd.ExecuteReader();
          while (reader.Read()){
            numRows++;
            parsedResult += $"Event: {reader["Event"]}\n";
            parsedResult += $"Site: {reader["Site"]}\n";
            parsedResult += $"Date: {reader["Date"]}\n";
            parsedResult += $"White: {reader["White"]} ({reader["WhiteElo"]})\n";
            parsedResult += $"Black: {reader["Black"]} ({reader["BlackElo"]})\n";
            parsedResult += $"Result: {reader["Result"]}\n";
        
            if(showMoves){
              parsedResult += $"Moves: {reader["Moves"]}\n"; 
            }

            parsedResult += "\n";
          

          }

        
        }
          catch ( Exception e )
          {
           System.Diagnostics.Debug.WriteLine( e.Message );
          }
      }
      return numRows + " results\n" + parsedResult;
    }
  }
}

