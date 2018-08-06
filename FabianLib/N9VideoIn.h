#pragma once

#ifdef VID_EXPORTS
#define VID_API __declspec(dllexport)
#else
#define VID_API __declspec(dllimport)
#endif

#define CVBS 3
#define SVIDEO 8

/* -------------------- Prototypes of Exported Functions ------------------- */

/* Declare the external entry points here. Use declspec so we don't need a
   .def file. Bracketed with extern C to avoid mangling in C++. */

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */

VID_API HRESULT VID_ShowOverlay(PRECT sourcerect, PRECT targetrect);
VID_API void VID_HideOverlay();
VID_API HRESULT VID_CreateOverlay(DWORD dwSizeX, DWORD dwSizeY);

VID_API BYTE VID_GetReg(BYTE reg);
VID_API void VID_SetReg(BYTE reg, BYTE val);
VID_API void VID_WriteRegs();
VID_API void VID_ReadRegs();
VID_API void VID_SetDefault();
VID_API void VID_ResetScaler();

VID_API void VID_SetZoom(DWORD dwZoomX, DWORD dwZoomY, BOOL bInterpolate);
VID_API void VID_SetInput(DWORD dwInput);

#ifdef __cplusplus
} // extern "C"
#endif
