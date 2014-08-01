Signal_generator
================

INTRODUCTION

  The project is called a Signal_generator. The program itself is a console app which can be used to create wav files containing a sound signal with a given set of frequencies and amplitudes.

  It is possible to create a wav file with either 1 or 2 channels. Also, current version is only capable of recording a wave file having discretization frequency equal to 44100 herz.
  
  It is fairly easy to generate a signal for 1 channel. You need to do the following actions:
  
    0) open .exe file with a program itself
    
    1) enter the name of a file
    
    2) set the number of channels to be equal to 1 
    
    3) set the length of the recording in seconds
    
    4) set a signal:
    
        - set an amplitude of the wave(like 1000 or higher to make it sound louder)
        
        - set a frequency of it in herz
        
    5) press q
    
    6) and press any key
    
    Done. Now you might wanna look for a .wav file with a given name in a folder with the program.
      
  Similarly you can record a signal with 2 channels, consecutively setting a signal parameters for both channels. Also, for each channel it is possible to mix several sinusoids. As a result you will create a pretty complex sound pattern.
  
  Try to create binaural beats for fun. For instance, open the program and type this:
  
    binaural
    
    2 
    
    60 
    
    8000
    
    400 
    
    q
    
    8000
    
    404
    
    q
    
  Now you got a file binaural.wav containing binaural beats with 4 herz.
  
