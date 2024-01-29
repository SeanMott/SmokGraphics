Smok Graphics


Smok Graphics is a smaller Util library for Vulkan. It supports getting a instance as a graphics context and choosing a GPU.

Smok Graphics uses Vulkan Memory Allocator for creating buffers, images, vertex and index buffers, and other memory allocations

Smok Graphics depends on Vulkan Memory Allocator anbd BTDSTD

Smok Graphics is written in a C style Pod Of Data style but still with namespaces. Ie Smok::Graphics::VertexBuffer_Create(&GPU, &vertexBuffer, &VBCreateInfo)