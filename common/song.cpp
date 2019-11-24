#include "FMOD/lowlevel/inc/fmod.hpp"
#include "FMOD/common.h"
#include "song.hpp"


FMOD::System       *sistema;
FMOD::Sound        *som;
FMOD::Channel      *canal;
FMOD::DSP          *meudsp;
FMOD::ChannelGroup *mastergrupo;
void               *dadosControlador = 0;
float vol;


FMOD_RESULT F_CALLBACK myDSPCallback(FMOD_DSP_STATE *dsp_state, float *inbuffer, float *outbuffer, unsigned int length, int inchannels, int *outchannels) 
{
    mydsp_data_t *data = (mydsp_data_t *)dsp_state->plugindata;

    /*
        This loop assumes inchannels = outchannels, which it will be if the DSP is created with '0' 
        as the number of channels in FMOD_DSP_DESCRIPTION.  
        Specifying an actual channel count will mean you have to take care of any number of channels coming in,
        but outputting the number of channels specified. Generally it is best to keep the channel 
        count at 0 for maximum compatibility.
    */
    for (unsigned int samp = 0; samp < length; samp++) 
    { 
        /*
            Feel free to unroll this.
        */
        for (int chan = 0; chan < *outchannels; chan++)
        {
            /* 
                This DSP filter just halves the volume! 
                Input is modified, and sent to output.
            */
            data->buffer[(samp * *outchannels) + chan] = outbuffer[(samp * inchannels) + chan] = inbuffer[(samp * inchannels) + chan] * data->volume_linear;
        }
    }

    data->channels = inchannels;

    return FMOD_OK; 
} 

/*
    Callback called when DSP is created.   This implementation creates a structure which is attached to the dsp state's 'plugindata' member.
*/
FMOD_RESULT F_CALLBACK myDSPCreateCallback(FMOD_DSP_STATE *dsp_state)
{
    unsigned int blocksize;
    FMOD_RESULT result;

    result = dsp_state->functions->getblocksize(dsp_state, &blocksize);
    ERRCHECK(result);

    mydsp_data_t *data = (mydsp_data_t *)calloc(sizeof(mydsp_data_t), 1);
    if (!data)
    {
        return FMOD_ERR_MEMORY;
    }
    dsp_state->plugindata = data;
    data->volume_linear = 1.0f;
    data->length_samples = blocksize;

    data->buffer = (float *)malloc(blocksize * 8 * sizeof(float));      // *8 = maximum size allowing room for 7.1.   Could ask dsp_state->functions->getspeakermode for the right speakermode to get real speaker count.
    if (!data->buffer)
    {
        return FMOD_ERR_MEMORY;
    }

    return FMOD_OK;
}

/*
    Callback called when DSP is destroyed.   The memory allocated in the create callback can be freed here.
*/
FMOD_RESULT F_CALLBACK myDSPReleaseCallback(FMOD_DSP_STATE *dsp_state)
{
    if (dsp_state->plugindata)
    {
        mydsp_data_t *data = (mydsp_data_t *)dsp_state->plugindata;

        if (data->buffer)
        {
            free(data->buffer);
        }

        free(data);
    }

    return FMOD_OK;
}

/*
    Callback called when DSP::getParameterData is called.   This returns a pointer to the raw floating point PCM data.
    We have set up 'parameter 0' to be the data parameter, so it checks to make sure the passed in index is 0, and nothing else.
*/
FMOD_RESULT F_CALLBACK myDSPGetParameterDataCallback(FMOD_DSP_STATE *dsp_state, int index, void **data, unsigned int *length, char *)
{
    if (index == 0)
    {
        unsigned int blocksize;
        FMOD_RESULT result;
        mydsp_data_t *mydata = (mydsp_data_t *)dsp_state->plugindata;

        result = dsp_state->functions->getblocksize(dsp_state, &blocksize);
        ERRCHECK(result);

        *data = (void *)mydata;
        *length = blocksize * 2 * sizeof(float);

        return FMOD_OK;
    }

    return FMOD_ERR_INVALID_PARAM;
}

/*
    Callback called when DSP::setParameterFloat is called.   This accepts a floating point 0 to 1 volume value, and stores it.
    We have set up 'parameter 1' to be the volume parameter, so it checks to make sure the passed in index is 1, and nothing else.
*/
FMOD_RESULT F_CALLBACK myDSPSetParameterFloatCallback(FMOD_DSP_STATE *dsp_state, int index, float value)
{
    if (index == 1)
    {
        mydsp_data_t *mydata = (mydsp_data_t *)dsp_state->plugindata;

        mydata->volume_linear = value;

        return FMOD_OK;
    }

    return FMOD_ERR_INVALID_PARAM;
}

/*
    Callback called when DSP::getParameterFloat is called.   This returns a floating point 0 to 1 volume value.
    We have set up 'parameter 1' to be the volume parameter, so it checks to make sure the passed in index is 1, and nothing else.
    An alternate way of displaying the data is provided, as a string, so the main app can use it.
*/
FMOD_RESULT F_CALLBACK myDSPGetParameterFloatCallback(FMOD_DSP_STATE *dsp_state, int index, float *value, char *valstr)
{
    if (index == 1)
    {
        mydsp_data_t *mydata = (mydsp_data_t *)dsp_state->plugindata;

        *value = mydata->volume_linear;
        if (valstr)
        {
            sprintf(valstr, "%d", (int)((*value * 100.0f)+0.5f));
        }

        return FMOD_OK;
    }

    return FMOD_ERR_INVALID_PARAM;
}

void FMOD_PlayPause(int option){
    FMOD_RESULT         result;
    if (option)
    {
       canal->setPaused(option);
    }else{
        canal->setPaused(option);
    }
}

void FMOD_Config(char *pathAudio)
{
    FMOD_RESULT         result;
    unsigned int        version;
    Common_Init(&dadosControlador);

    
    result = FMOD::System_Create(&sistema);
    ERRCHECK(result);

    result = sistema->getVersion(&version);
    ERRCHECK(result);

    if (version < FMOD_VERSION)
    {
        Common_Fatal("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
    }

    result = sistema->init(32, FMOD_INIT_NORMAL, dadosControlador);
    ERRCHECK(result);

 	result = sistema->createSound(pathAudio, FMOD_LOOP_NORMAL, 0, &som);
    ERRCHECK(result);

    result = sistema->playSound(som, 0, false, &canal);
    ERRCHECK(result);

  
    { 
        FMOD_DSP_DESCRIPTION dspdesc; 
        memset(&dspdesc, 0, sizeof(dspdesc));
        FMOD_DSP_PARAMETER_DESC wavedata_desc;
        FMOD_DSP_PARAMETER_DESC volume_desc;
        FMOD_DSP_PARAMETER_DESC *paramdesc[2] = 
        {
            &wavedata_desc,
            &volume_desc
        };

        FMOD_DSP_INIT_PARAMDESC_DATA(wavedata_desc, "wave data", "", "wave data", FMOD_DSP_PARAMETER_DATA_TYPE_USER);
        FMOD_DSP_INIT_PARAMDESC_FLOAT(volume_desc, "volume", "%", "linear volume in percent", 0, 1, 1);

        strncpy(dspdesc.name, "My first DSP unit", sizeof(dspdesc.name));
        dspdesc.version             = 0x00010000;
        dspdesc.numinputbuffers     = 1;
        dspdesc.numoutputbuffers    = 1;
        dspdesc.read                = myDSPCallback; 
        dspdesc.create              = myDSPCreateCallback;
        dspdesc.release             = myDSPReleaseCallback;
        dspdesc.getparameterdata    = myDSPGetParameterDataCallback;
        dspdesc.setparameterfloat   = myDSPSetParameterFloatCallback;
        dspdesc.getparameterfloat   = myDSPGetParameterFloatCallback;
        dspdesc.numparameters       = 2;
        dspdesc.paramdesc           = paramdesc;

        result = sistema->createDSP(&dspdesc, &meudsp); 
        ERRCHECK(result); 
    } 

    result = sistema->getMasterChannelGroup(&mastergrupo);
    ERRCHECK(result);

    result = mastergrupo->addDSP(0, meudsp);
    ERRCHECK(result);
}


void decreaseVolume(){

	bool bypass;
	FMOD_RESULT         result;
	Common_Update();
	result = meudsp->getBypass(&bypass);
    ERRCHECK(result);
		
	float vol;

    result = meudsp->getParameterFloat(1, &vol, 0, 0);
    ERRCHECK(result);

    if (vol > 0.0f)
    {
        vol -= 0.1f;
    }
    result = meudsp->setParameterFloat(1, vol);
	ERRCHECK(result);

	result = sistema->update();
    ERRCHECK(result);
}

void killSong(){
	FMOD_RESULT         result;
	result = som->release();
    ERRCHECK(result);

    result = mastergrupo->removeDSP(meudsp);
    ERRCHECK(result);
    result = meudsp->release();
    ERRCHECK(result);

    result = sistema->close();
    ERRCHECK(result);
    result = sistema->release();
    ERRCHECK(result);

    Common_Close();
}

void increaseVolume(){
	bool bypass;
	FMOD_RESULT         result;
	Common_Update();
	result = meudsp->getBypass(&bypass);
    ERRCHECK(result);

    result = meudsp->getParameterFloat(1, &vol, 0, 0);
    ERRCHECK(result);

    if (vol < 1.0f)
    {
        vol += 0.1f;
    }
    result = meudsp->setParameterFloat(1, vol);
	ERRCHECK(result);
 
	result = sistema->update();
    ERRCHECK(result);        
}
