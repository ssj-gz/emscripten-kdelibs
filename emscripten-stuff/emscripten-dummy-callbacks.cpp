// Any executable linking to emscripten-qt needs these functions defined - add dummy definitions for
// the executables builds along with kdelibs (kde4-config, etc) - KDE4_ADD_EXECUTABLE adds this file
// automatically.
extern "C"
{
    void EMSCRIPTENQT_resetTimerCallback(long milliseconds)
    {
    }
    void EMSCRIPTENQT_attemptedLocalEventLoop()
    {
    }
}
