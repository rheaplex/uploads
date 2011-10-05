#include "emotions.h"

////////////////////////////////////////////////////////////////////////////////
// Emotions
////////////////////////////////////////////////////////////////////////////////

// Ekman's basic emotions

//const std::vector<std::string> emotions = {"anger", "disgust", "fear", "happiness", "sadness", "surprise"};

// Descartes' basic emotions

const std::vector<std::string> emotions = {"wonder", "love", "hatred", "desire", "joy", "sadness"};

// The emotion with the higest count. Pointer to make write atomic
// NOTE: Non-atomic, written to by one thread, read by another, probably bad
// NOTE: Check glibc implementation, it's probaby a pointer internally

std::string current_emotion;
