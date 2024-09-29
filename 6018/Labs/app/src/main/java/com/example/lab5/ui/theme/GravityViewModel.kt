import androidx.compose.ui.unit.dp
import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel

class GravityViewModel : ViewModel() {
    private val _xOffset = MutableLiveData(0f)
    private val _yOffset = MutableLiveData(0f)

    val xOffset: LiveData<Float> get() = _xOffset
    val yOffset: LiveData<Float> get() = _yOffset

    fun updateOffsets(newX: Float, newY: Float) {
        val scaleFactor = 50f
        val screenWidth = 400
        val screenHeight = 800

        _xOffset.value = (newX * scaleFactor).coerceIn(0f, screenWidth.toFloat() - 40.dp.value)
        _yOffset.value = (-newY * scaleFactor).coerceIn(0f, screenHeight.toFloat() - 40.dp.value)
    }


}
