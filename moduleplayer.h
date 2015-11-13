#ifndef MODULEPLAYER_H
#define MODULEPLAYER_H
#include <iostream>
#include <string>
#include <portaudio.h>
#include <fstream>
#include <libopenmpt_stream_callbacks_file.h>
#include <cstdint>
#include <thread>
#include <chrono>
#include <sstream>
#include <libopenmpt.h>
using namespace std;

void playsong(string bob)
{
    int interpol = 1;
    int ss = 100;
    int repeattimes = -1;
    bool forky = false;
    const int BUFFERSIZE = 2480;
    const int SAMPLERATE = 48000;
    static int16_t left[BUFFERSIZE];
    static int16_t right[BUFFERSIZE];
    static int16_t * const buffers[2] = { left, right };
    FILE * file = 0;
    stringstream arg;

    openmpt_module * mod = 0;
    size_t count = 0;
    PaStream * stream = 0;
    if(!bob.empty())
    {
        fclose(stdout);
        fclose(stderr);
        Pa_Initialize();

        freopen("/dev/tty", "w", stdout);
        freopen("/dev/tty", "w", stderr);

        Pa_OpenDefaultStream(&stream, 0, 2, paInt16 | paNonInterleaved, SAMPLERATE, paFramesPerBufferUnspecified, NULL, NULL);
        Pa_StartStream(stream);
        cout << bob.c_str() << endl;
        file = fopen(bob.c_str(), "rb");
        if(file != NULL)
        {
            if(openmpt_could_open_propability(openmpt_stream_get_file_callbacks(),file,1,NULL,NULL) >= 0.4)
            {
                mod = openmpt_module_create(openmpt_stream_get_file_callbacks(), file, NULL, NULL, NULL);
                openmpt_module_set_render_param(mod, OPENMPT_MODULE_RENDER_INTERPOLATIONFILTER_LENGTH, interpol);
                openmpt_module_set_render_param(mod, OPENMPT_MODULE_RENDER_STEREOSEPARATION_PERCENT, ss);
                openmpt_module_set_repeat_count(mod,repeattimes);
                int numtimes = 0;
                fclose(file);
                while (1) {




                    count = openmpt_module_read_stereo(mod, SAMPLERATE, BUFFERSIZE, left, right);
                    if (count == 0) {
                        //goto recursive;
                        if(openmpt_module_get_repeat_count(mod) == 0)
                        {
                            break;
                        }
                        else
                            count = openmpt_module_read_stereo(mod, SAMPLERATE, BUFFERSIZE, left, right);
                        //break;
                    }
                    Pa_WriteStream(stream, buffers, (unsigned long)count);
                }

                Pa_StopStream(stream);
                Pa_CloseStream(stream);
                Pa_Terminate();
                openmpt_module_destroy(mod);
            }
            else
            {
                cout << "Not a valid module." << endl;
            }
        }
    }

}
#endif // MODULEPLAYER_H
