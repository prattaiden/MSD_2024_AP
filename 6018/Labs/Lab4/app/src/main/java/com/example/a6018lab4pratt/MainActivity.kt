package com.example.a6018lab4pratt

import JokeViewModel
import android.os.Bundle
import androidx.compose.foundation.lazy.items
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.material3.Button
import androidx.compose.material3.ButtonDefaults
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.rememberCoroutineScope
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.lifecycle.viewmodel.compose.viewModel
import androidx.navigation.NavController
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import com.example.a6018lab4pratt.ui.theme._6018Lab4PrattTheme
import com.google.gson.Gson
import io.ktor.client.*
import io.ktor.client.call.body
import io.ktor.client.engine.cio.*
import io.ktor.client.request.*
import kotlinx.coroutines.*



class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            _6018Lab4PrattTheme {
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colorScheme.background
                ) {
                    NavigationGraph()
                }
            }
        }
    }
}

@Composable
fun NavigationGraph() {
    val navController = rememberNavController()
    val jokeViewModel: JokeViewModel = viewModel()

    NavHost(navController = navController, startDestination = "greeting") {
        composable("greeting") { Greeting(navController, jokeViewModel) }
        composable("newPage") { NewPage(navController, jokeViewModel) }
    }
}

@Composable
fun NewPage(navController: NavController, jokeViewModel: JokeViewModel) {
    val allJokeHistory = jokeViewModel.jokeHistory.value ?: emptyList()
    Column(
        horizontalAlignment = Alignment.CenterHorizontally,
        verticalArrangement = Arrangement.Center,
        modifier = Modifier
            .fillMaxSize()
            .padding(16.dp)
    ){
        Text(
            text = "previous funny jokes",
            fontSize = 20.sp,
            fontWeight = FontWeight.Bold,
            modifier = Modifier
                .padding(50.dp)
        )
        Button(onClick = {navController.navigate("greeting")},
            colors = ButtonDefaults.buttonColors(containerColor = Color.Black)){
            Text(text = "main screen")
        }
        Spacer(modifier = Modifier.height(16.dp))

        LazyColumn {
            items(allJokeHistory) { joke ->
                Text(
                    text = joke,
                    color = Color.Red,
                    modifier = Modifier.padding(8.dp),
                    fontSize = 18.sp
                )
            }
        }
    }
}

data class JokeJSON(
    val value:String
)

@Composable
fun Greeting(navController: NavController, jokeViewModel: JokeViewModel) {
    val client = HttpClient(CIO)
    var responseText by remember { mutableStateOf("")}
    val scope = rememberCoroutineScope()

    Column(
        horizontalAlignment = Alignment.CenterHorizontally,
        verticalArrangement = Arrangement.Center,
        modifier = Modifier
            .fillMaxSize()
    ){
        Text(
            text = "CHUCK NORRIS",
            color = Color.Black,
            fontSize = 30.sp,
            fontWeight = FontWeight.Bold,
            modifier = Modifier.padding(50.dp)
        )
        Button(onClick = {
            scope.launch {
                try {
                    val response: String =
                        client.get("https://api.chucknorris.io/jokes/random").body()
                    val joke = parseJSON(response)
                    responseText = joke.value
                    //add joke to the view model as well
                    jokeViewModel.addJoke(joke.value)

                } catch (e: Exception) {
                    responseText = "Error: ${e.message}"
                }
            }
        },
        colors = ButtonDefaults.buttonColors(containerColor = Color.Black))
        {
            Text(text = "new joke")
        }
        Text(text = responseText,
            color = Color.White,
            fontSize = 20.sp,
            modifier = Modifier
                .padding(20.dp)
                .background(Color.Black)
        )
        Spacer(modifier = Modifier.height(20.dp))
        Button(onClick = {navController.navigate("newPage")},
            colors = ButtonDefaults.buttonColors(containerColor = Color.Black)
        ){
            Text(text = "Previous Jokes")
        }

    }
}

fun parseJSON(json: String): JokeJSON{
    return Gson().fromJson(json, JokeJSON::class.java)
}

//@Preview(showBackground = true)
//@Composable
//fun GreetingPreview() {
//    _6018Lab4PrattTheme {
//        Greeting()
//    }
//}
