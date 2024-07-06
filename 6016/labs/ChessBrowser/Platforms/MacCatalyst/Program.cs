using ObjCRuntime;
using UIKit;

namespace ChessBrowser;

public class Program
{
	// This is the main entry point of the application.
	static void Main(string[] args)
	{
		// if you want to use a different Application Delegate class from "AppDelegate"
		// you can specify it here.

       // var filePath = "../PGNFiles/kb1.pgn";

       // List<ChessGame> gamesTest = PgnReader.ReadChessGamePgn(filePath);

       // Console.WriteLine(gamesTest[0].black_);

		UIApplication.Main(args, null, typeof(AppDelegate));
	}
}
