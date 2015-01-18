// Twin sine oscillator demo
// for testing purposes, use MIDI to set oscillator frequency
// and serial debug to display frequency

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine1;          //xy=157,221
AudioSynthWaveformSine   sine2;          //xy=157,285
AudioEffectEnvelope      envelope2;      //xy=348,284
AudioEffectEnvelope      envelope1;      //xy=351,221
AudioOutputI2S           i2s1;           //xy=547,246
AudioConnection          patchCord1(sine1, envelope1);
AudioConnection          patchCord2(sine2, envelope2);
AudioConnection          patchCord3(envelope2, 0, i2s1, 1);
AudioConnection          patchCord4(envelope1, 0, i2s1, 0);
AudioControlSGTL5000     audioShield;     //xy=534,180
// GUItool: end automatically generated code

byte leftnote = 255, rightnote = 255;  // MIDI note numbers

void setup(void)
{
  AudioMemory(8);
  audioShield.enable();
  //audioShield.volume(0.5);
  audioShield.unmuteLineout();
  sine1.amplitude(0.7);  
  sine2.amplitude(0.7);  
  // use default values for envelopes
  // use MIDI as a quick way to set the frequency
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn);
  // debug output actual frequency
  Serial.begin(9600);
}

void loop() {
  usbMIDI.read(); // USB MIDI receive
}

float note2frequency (byte note) {
  float freq = ((float)note - 69) / 12.0;  // semitones above or below A4
  freq =  440.0 * powf(2.0, freq);
  Serial.println(freq, 8);
  return freq;
}

void OnNoteOn(byte channel, byte note, byte velocity) {
  // accept input channels 1 and 2, and ignore velocity
  
  if ((channel = 1) && (leftnote <= 127))  { // check for a currently held note and brutally kill it
  // this sounds bad so only play one note at once, retro style
    envelope1.noteOff();
    delay(3);  // briefest period for killed note to stop sounding
  }
  else if ((channel = 2) && (rightnote <= 127)) {
       envelope2.noteOff();
    delay(3);  // briefest period for killed note to stop sounding
  } 
  if (channel = 1) {
    sine1.frequency(note2frequency(note));
    envelope1.noteOn();
  }
  else if (channel = 2) {
    sine2.frequency(note2frequency(note));
    envelope2.noteOn();
  }
}
  
void OnNoteOff(byte channel, byte note, byte velocity) {
if ((channel = 1) && (leftnote = note)) {
    envelope1.noteOff();
  }
  else if ((channel = 2) && (rightnote = note)) {
    envelope2.noteOff();
  }
}


  
