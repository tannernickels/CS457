#include "log.h"

void log_message(String& message, String& file){
    FileIO logger(file, "a");
    logger.writeString(message + "\n");
}