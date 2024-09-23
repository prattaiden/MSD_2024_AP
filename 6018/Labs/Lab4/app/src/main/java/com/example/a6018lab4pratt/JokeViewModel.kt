import android.app.Application
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.viewModelScope
import com.example.a6018lab4pratt.JokeDAO
import com.example.a6018lab4pratt.JokeDatabase
import com.example.a6018lab4pratt.JokeEntity
import kotlinx.coroutines.launch

class JokeViewModel(application: Application) : AndroidViewModel(application) {
    private val jokeDao: JokeDAO = JokeDatabase.getDatabase(application).jokeDao()

    private val _jokeHistory = MutableLiveData<List<String>>(emptyList())
    val jokeHistory: LiveData<List<String>> = _jokeHistory

    init {
        viewModelScope.launch {
            jokeDao.getAllJokes().collect { jokeEntities ->
                _jokeHistory.value = jokeEntities.map { it.joke }
            }
        }
    }

    fun addJoke(joke: String) {
        viewModelScope.launch {
            jokeDao.addJoke(JokeEntity(joke = joke))

            _jokeHistory.value = _jokeHistory.value?.plus(joke)
        }
    }
}

