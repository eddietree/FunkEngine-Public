
#pragma once


#if !defined(FUNK_FINAL)

void CheckGLError();
void CheckGLBufferBindingID(int target, int id);

#else

#define CheckGLError()
#define CheckGLBufferBindingID(target, id) 

#endif  // FUNK_FINAL
