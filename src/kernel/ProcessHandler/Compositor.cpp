#include "Compositor.h"
#include "ProcessHandler.h"

namespace Compositor
{
    bool RedrawRequest = false;
    bool BufferSwapRequest = false;

    void Update(uint32_t i)
    {
        for (uint32_t j = i + 1; j < ProcessHandler::Processes.Length(); j++)
        {
            if (ProcessHandler::Processes[i]->Contains(ProcessHandler::Processes[j]))
            {
                RedrawRequest = true;
                return;
            }
        }
        ProcessHandler::Processes[i]->Render();
        BufferSwapRequest = true;
    }

    void Update()
    {
        if (RedrawRequest)
        {        
            for (uint32_t i = 0; i < ProcessHandler::Processes.Length(); i++)
            {
                ProcessHandler::Processes[i]->Render();
            }    
            Renderer::SwapBuffers();
            RedrawRequest = false;
            BufferSwapRequest = false;     
        }
        else if (BufferSwapRequest)
        {                
            Renderer::SwapBuffers();
            BufferSwapRequest = false;
        }
    }
}