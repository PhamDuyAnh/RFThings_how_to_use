// String str = "\r\nABC=XYZ\r\nOk\r\n";
// String pieces[3];
// splitStringToCMD(str, sizeof(pieces) / sizeof(String), "=");

// Function to find the separation Symbol and split the string into different pieces.
// @ _str              -> The string needs to be split
// @ numOfPieces       -> Number of pieces
// @ separatorSymbol   -> Separation symbol
// @ pieces            -> String array to save
// Ex:
// String str = "\r\nABC=XYZ\r\nOk\r\n";
// String pieces[3];
// splitStringToCMD(str, sizeof(pieces) / sizeof(String), "=");
// return: pieces[0]=ABC pieces[1]=XYZ pieces[2]=OK
void splitStringToCMD(String _str, byte numOfPieces, String separatorSymbol) {
  _str.trim();                            // Get a version of the String with any leading and trailing whitespace removed
  _str.replace("\r\n", separatorSymbol);  // Replace \r\n with separatorSymbol

  // Keep track of current position in array
  int counter = 0;
  // Keep track of the last comma so we know where to start the substring
  int lastIndex = 0;

  byte length = _str.length();

  for (int i = 0; i < numOfPieces; i++) {
    pieces[i] = "";
  }

  for (int i = 0; i < length; i++) {
    // Loop through each character and check if it's a comma
    if ((_str.substring(i, i + 1) == separatorSymbol) && (counter < numOfPieces - 1)) {
      // Grab the piece from the last index up to the current position and store it
      pieces[counter] = _str.substring(lastIndex, i);
      lastIndex = i + 1;  // Update the last position and add 1, so it starts from the next character
      counter++;          // Increase the position in the array that we store into
    }
    if (counter <= numOfPieces - 1) {
      pieces[counter] = _str.substring(lastIndex, length);
    }
  }

  dbPrintln("STR : " + _str);
  for (int i = 0; i < numOfPieces; i++) {
    dbPrint("CMD ");
    dbPrint(i + 1);
    dbPrintln(": " + pieces[i]);
  }

  // if (pieces[0] == "set") {
  //   Serial.print("Set ");
  //   if (pieces[1] == "url") {
  //     Serial.print("URL:");
  //     Serial.println(pieces[2]);
  //   }
  //   if (pieces[1] == "ussid") {
  //     Serial.print("USSID: ");
  //     Serial.println(pieces[2]);
  //   }
  // } else if (pieces[0] == "get") {
  //   Serial.print("Get info");
  // } else {
  //   Serial.println("not...");
  // }
  // } else {
  //   Serial.println("split String To CMD error");
  // }
  // counter = 0;
  // lastIndex = 0;
}

// Function to find a string of numbers in a string of characters
// @ _str              -> The string needs to be find (only char: -./0123456789)
// @ return            -> String of numbers if found
// Ex:
// String str = "34.50 degree C";
// getNumFromString(str)
// return: 34.50
String getNumFromString(String _str) {
  String _s = "";
  byte _l = _str.length() + 1;
  char _c[_l];
  _str.toCharArray(_c, _l);

  if (_l < 20) {  // add maximum length
    for (byte _i = 0; _i < _l; _i++) {
      if (45 <= _c[_i] && _c[_i] <= 57) {  // only char: -./0123456789 -- https://docs.arduino.cc/built-in-examples/strings/CharacterAnalysis
        _s += _c[_i];
      }
    }
  }

  return _s;
}