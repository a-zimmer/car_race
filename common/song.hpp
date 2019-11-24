#include "FMOD/lowlevel/inc/fmod.hpp"
#include "FMOD/common.h"

#ifndef SONG_HPP
#define SONG_HPP



typedef struct 
{
    float *buffer;
    float volume_linear;
    int   length_samples;
    int   channels;
} mydsp_data_t;

FMOD_RESULT F_CALLBACK myDSPCallback(FMOD_DSP_STATE *dsp_state, float *inbuffer, float *outbuffer, unsigned int length, int inchannels, int *outchannels);
FMOD_RESULT F_CALLBACK myDSPCreateCallback(FMOD_DSP_STATE *dsp_state);
FMOD_RESULT F_CALLBACK myDSPReleaseCallback(FMOD_DSP_STATE *dsp_state);
FMOD_RESULT F_CALLBACK myDSPGetParameterDataCallback(FMOD_DSP_STATE *dsp_state, int index, void **data, unsigned int *length, char *);
FMOD_RESULT F_CALLBACK myDSPSetParameterFloatCallback(FMOD_DSP_STATE *dsp_state, int index, float value);
FMOD_RESULT F_CALLBACK myDSPGetParameterFloatCallback(FMOD_DSP_STATE *dsp_state, int index, float *value, char *valstr);
int FMOD_Principal();
void FMOD_Config(char *pathAudio);
void decreaseVolume();
void increaseVolume();
void killSong();
void FMOD_PlayPause(int option);

#endif
