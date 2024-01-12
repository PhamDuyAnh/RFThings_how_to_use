// initial Serial  ////////////////////////////////////////////////////////////////////////20231211
void initSerial() {
  Serial.begin(BAUDRATE);
  // timeout to wait Serial is 2s
  unsigned long t = millis();
  while (!Serial && (millis() - t < 2000))
    ;
  Serial.setTimeout(TIMEOUT);
  dbPrintln("\n\nCreated by " + String(CRE_NAME));
  dbPrintln("  Project " + String(PRJ_NAME) + ", edited on " + String(UPDATED));
  char dateStr[11];
  dateReFormat(__DATE__, dateStr);
  dbPrintln("  Compiled on: " + String(__TIME__) + " - " + String(dateStr) + "\n");

  dbPrint(String(millis()) + "ms - ");
  dbPrintln("Initial Serial!");
}
void dateReFormat(char const *date, char *buff) {
  int month, day, year;
  static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
  sscanf(date, "%s %d %d", buff, &day, &year);

  month = (strstr(month_names, buff) - month_names) / 3 + 1;
  //sprintf(buff, "%d%02d%02d", year, month, day);
  sprintf(buff, "%d/%02d/%02d", year, month, day);
}
///////////////////////////////////////////////////////////////////////////////////////////20231114