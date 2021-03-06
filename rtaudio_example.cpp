#include "RtAudio.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
// Pass-through function.
int inout( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
           double streamTime, RtAudioStreamStatus status, void *data )
{
  // Since the number of input and output channels is equal, we can do
  // a simple buffer copy operation here.
  if ( status ) std::cout << "Stream over/underflow detected." << std::endl;
  unsigned long *bytes = (unsigned long *) data;
  //memcpy( outputBuffer, inputBuffer, *bytes );

  return 0;
}
int main() {
 RtAudio adac;
  if ( adac.getDeviceCount() < 1 ) {
    std::cout << "\nNo audio devices found!\n";
    exit( 0 );
  }
  // Set the same number of channels for both input and output.
  unsigned int bufferBytes, bufferFrames = 512;
  RtAudio::StreamParameters iParams, oParams;
  iParams.deviceId = 0; // first available device
  iParams.nChannels = 0;
  //iParams.nChannels = 2;
  oParams.deviceId = 0; // first available device
  oParams.nChannels = 2;
  try {
    adac.openStream( &oParams, &iParams, RTAUDIO_SINT32, 44100, &bufferFrames, &inout, (void *)&bufferBytes );
  }
  catch ( RtError& e ) {
    e.printMessage();
    exit( 0 );
  }
  bufferBytes = bufferFrames * 2 * 4;
  try {
    adac.startStream();
    char input;
    std::cout << "\nRunning ... press <enter> to quit.\n";
    std::cin.get(input);
    // Stop the stream.
    adac.stopStream();
  }
  catch ( RtError& e ) {
    e.printMessage();
    goto cleanup;
  }
 cleanup:
  if ( adac.isStreamOpen() ) adac.closeStream();
  return 0;
}
