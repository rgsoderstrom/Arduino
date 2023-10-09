
//
// TextMessage.cpp
//

#include "TextMessage.h"

TextMessage::TextMessage (char txt []) : TextMsg_Auto ()
{
    strncpy (data.text, txt, Data::TextBufferSize - 1);
	data.text [Data::TextBufferSize - 1] = '\0';
}
